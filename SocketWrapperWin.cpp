#include <string>
#include "UniSocket.hpp"
#include "SocketWrapperWin.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>		// inetPton()
#include <windows.h>
#include <cstdio>			// sprintf()
// #include <chrono>			// sleep_for
// #include <thread>			// sleep_for

// Client -----------------

using namespace std;

SocketWrapper::SocketWrapper(void) {}

SocketWrapper::SocketWrapper(const string & ip, const int & port) {
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 0); // Request WinSock v2.0
	if (::WSAStartup(wVersionRequested, &wsaData) != 0)
		throw UniSocketException("Unable to load WinSock DLL");

	_ip = ip;	
	_socket = (int) ::socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == INVALID_SOCKET)
		throw UniSocketException("Invalid socket");

	memset(&_address, 0, sizeof(SOCKADDR_IN));
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = inet_addr(_ip.c_str());
	_address.sin_port = htons(port);

	int flag = ::connect(_socket, (SOCKADDR *)& _address, sizeof(SOCKADDR));
	if (flag == SOCKET_ERROR)
		throw UniSocketException("Error during connect()");
}

void SocketWrapper::send(const string & data) {
	string msg = to_string(data.length()) + HEADERSPLITTER + data;
	::send((SOCKET)_socket, msg.c_str(), msg.length(), 0);
/*	string msg = to_string(data.length());
	::send((SOCKET)_socket, msg.c_str(), msg.length(), 0);
	this_thread::sleep_for(chrono::seconds(1)); // c++2011
	msg = HEADERSPLITTER + data;
	::send((SOCKET)_socket, msg.c_str(), msg.length(), 0);
*/
}

string SocketWrapper::recv(bool polling) {
	static bool headerReaded = false;
	static string result;
	static int datasize;
	int len;
	int flags = 0;
	u_long iMode;
	if(polling == true) { 
		iMode = 1;
		ioctlsocket(_socket, FIONBIO, &iMode);
	}
	if(headerReaded == false) {
		// header search
		do {
			char tmp = '\0';
			len = ::recv(_socket, &tmp, 1, flags);
			if(len > 0) {
				result += tmp;
			} else {
				throw UniSocketException("timeout");
			}
		} while(result.find(HEADERSPLITTER) == string::npos);
		headerReaded = true;
		int datastart = result.find(HEADERSPLITTER);
		datasize = stoi(result.substr(0, datastart).c_str());
		result.clear();
	}
	iMode = 0;
	ioctlsocket(_socket, FIONBIO, &iMode);
	char * data = new char[datasize+1]();
	::recv(_socket, data, datasize, MSG_WAITALL);
	headerReaded = false;
	return data;
}

void SocketWrapper::close(void) {
	closesocket((SOCKET)_socket);
}

// Server -----------------

SocketWrapper::SocketWrapper(
	const int & port,
	const int & queueSize
) {
	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(2, 0); // Request WinSock v2.0
	if (::WSAStartup(wVersionRequested, &wsaData) != 0)
		throw UniSocketException("Unable to load WinSock DLL");

	_socket = (int) ::socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == INVALID_SOCKET)
		throw UniSocketException("Invalid socket");

	int i = 1;
	::setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&i, sizeof(i));

	memset(&_address, 0, sizeof(SOCKADDR_IN));
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = ADDR_ANY;
	_address.sin_port = htons(port);

	int r = ::bind(_socket, (SOCKADDR *)& _address, sizeof(SOCKADDR_IN));
	if (r == SOCKET_ERROR)
		throw UniSocketException("Error binding the socket");

	r = ::listen(_socket, queueSize);
	if (r == SOCKET_ERROR)
		throw UniSocketException("Error listening on socket");
}

SocketWrapper::SocketWrapper(
	const SOCKADDR_IN & myAddress,
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
	int addrlen = sizeof(struct sockaddr_in);

	int conn = ::accept(_socket, (SOCKADDR *)& _address, &addrlen);
	if (conn == INVALID_SOCKET)
		throw UniSocketException("Erorr using accept() on this socket");

	SocketWrapper so = SocketWrapper(_address, conn);
	return so;
}
