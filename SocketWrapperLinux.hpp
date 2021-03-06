#ifndef SOCKETWRAPPERLINUX_H_
#define SOCKETWRAPPERLINUX_H_

#include <string>
#include <netinet/in.h>      // For sockaddr_in

class SocketWrapper {

private:
	sockaddr_in _address;
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
		const sockaddr_in & myAddress,
		const int & mySocket
	);
	SocketWrapper accept(void);
};

#endif
