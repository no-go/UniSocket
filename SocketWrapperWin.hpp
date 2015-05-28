#ifndef SOCKETWRAPPERWIN_H_
#define SOCKETWRAPPERWIN_H_
#define DllExport __declspec( dllexport )
#include <string>
#include <winsock2.h>
#include <windows.h>
class DllExport SocketWrapper {

private:
	SOCKADDR_IN _address;
	int _socket;

public:
	std::string _ip;
	
	SocketWrapper(void);
	
	SocketWrapper(
		const std::string & ip,
		const int & port
	);
	
	void send(const std::string & data);
	std::string recv(bool polling);
	void close(void);

	// server
	SocketWrapper(
		const int & port,
		const int & queueSize
	);
	SocketWrapper(
		const SOCKADDR_IN & myAddress,
		const int & mySocket
	);
	SocketWrapper accept(void);
};

#endif
