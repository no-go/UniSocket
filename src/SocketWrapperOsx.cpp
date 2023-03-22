#include <string>
#include "UniSocket.hpp"
#include "SocketWrapperOsx.hpp"
#include <sys/socket.h>		// socket(), connect(), send(), recv()
#include <arpa/inet.h>		// inet_addr()
#include <unistd.h>			// close()
#include <netinet/in.h>		// sockaddr_in
#include <cstdio>			// sprintf()
#include <cstring>			// strerror
#include <cerrno>
// #include <chrono>			// sleep_for
// #include <thread>			// sleep_for

// Client -----------------

using namespace std;

SocketWrapper::SocketWrapper(void) {}

SocketWrapper::SocketWrapper(
	const string & ip,
	const int & port
) {
	_ip = ip;
	_socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1)
		throw UniSocketException(strerror(errno));

	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = inet_addr(_ip.c_str());
	_address.sin_port = htons(port);
	
	int flag = connect(
		_socket,
		(struct sockaddr *) & _address,
		sizeof(_address)
	);

	if(flag == -1)
		throw UniSocketException(strerror(errno));
}

void SocketWrapper::send(const string & data) {
	::send(_socket, data.c_str(), data.length(), 0);
}

string SocketWrapper::recv(void) {
	static bool headerReaded = false;
	static string result;
	int len;
    bool isDone = false;

    do {
        char tmp = '\0';
        len = ::recv(_socket, &tmp, 1, MSG_DONTWAIT);
        if(len > 0) {
            result += tmp;
        } else {
            isDone = true;
        }
    } while(isDone == false);
	return result;
}

void SocketWrapper::close(void) {
	::close(_socket);
}

// Server -----------------

SocketWrapper::SocketWrapper(
	const int & port,
	const int & queueSize
) {
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	if(_socket == -1)
		throw UniSocketException(strerror(errno));

	int i = 1;
	setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));

	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(port);
	
	int r = bind(
		_socket,
		(struct sockaddr *) & _address,
		sizeof(_address)
	);

	if(r == -1)
		throw UniSocketException(strerror(errno));

	r = listen(_socket, queueSize);
	
	if(r == -1)
		throw UniSocketException(strerror(errno));

}

SocketWrapper::SocketWrapper(
	const sockaddr_in & myAddress,
	const int & mySocket
) {
	_socket = mySocket;
	_address = myAddress;
	
	char buff[18];
	sprintf(buff,
		"%d.%d.%d.%d",
		int(myAddress.sin_addr.s_addr&0xFF),
		int((myAddress.sin_addr.s_addr&0xFF00)>>8),
		int((myAddress.sin_addr.s_addr&0xFF0000)>>16),
		int((myAddress.sin_addr.s_addr&0xFF000000)>>24)
	);
	_ip = buff;
}

SocketWrapper SocketWrapper::accept(void) {
	socklen_t addrlen = sizeof(struct sockaddr_in);
		
	int conn = ::accept(
		_socket,
		(struct sockaddr *) & _address,
		& addrlen
	);
	if(conn == -1)
		throw UniSocketException(strerror(errno));
	
	SocketWrapper so = SocketWrapper(_address, conn);
	return so;
}
