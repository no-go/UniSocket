#ifndef UNISOCKET_H_
#define UNISOCKET_H_

#include <string>

#ifdef __linux
	#include "SocketWrapperLinux.hpp"
	#define DllExport
#elif defined(_WIN32) || defined(_WIN64)
	#include "SocketWrapperWin.hpp"
#elif __APPLE__
	#include "SocketWrapperOsx.hpp"
	#define DllExport
#endif

#define HEADERSPLITTER "\n\n"

class DllExport UniSocketException {
public:
	std::string _msg;
	UniSocketException(std::string msg);
};

// Client -----------------

class DllExport UniSocket {

private:
	SocketWrapper _sock;
	std::string _ip;

public:
	UniSocket(
		const std::string & ip,
		const int & port
	);
	UniSocket(const std::string & ip, const SocketWrapper & sock);
	UniSocket(void);
	UniSocket(const UniSocket & ref);
	
	std::string getIp(void);

	void send(const std::string & data);
	
	/**
	 * Mit dem polling Parameter kann man polling aktivieren.
	 * Das Polling laeuft bis der Header gelesen wurde, dann 
	 * wird jedoch mit blockierendem Lesen so viele bytes gelesen,
	 * wie es im Header steht.
	 * 
	 * @param polling to activate optional non-blocking reading
	 */
	std::string recv(bool polling=false);
	void close(void);
};

// Server -----------------

class DllExport UniServerSocket {
	
private:
	SocketWrapper _ssock;

public:
	UniServerSocket(
		const int & port,
		const int & queueSize
	);
	~UniServerSocket(void);
	UniSocket accept(void);
};

#endif
