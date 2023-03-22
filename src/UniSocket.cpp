#include <string>
#include "UniSocket.hpp"

UniSocketException::UniSocketException(std::string msg) {
	_msg = msg;
}

// Client -----------------

UniSocket::UniSocket(
	const std::string & ip,
	const int & port
) {
	_ip = ip;
	_sock = SocketWrapper(_ip, port);
}

void UniSocket::send(const std::string & data) {
	_sock.send(data);
}

std::string UniSocket::recv(void) {
	return _sock.recv();
}

void UniSocket::close(void) {
	_sock.close();
}
std::string UniSocket::getIp(void) {
	return _ip;
}

// fuer Server nuetzlich -----------------

UniSocket::UniSocket(void) {}

UniSocket::UniSocket(const UniSocket & ref) {
	_ip = ref._ip;
	_sock = ref._sock;
}

UniSocket::UniSocket(
 const std::string & ip,
 const SocketWrapper & sock) {
	_ip = ip;
	_sock = sock;
}

UniServerSocket::UniServerSocket(
	const int & port,
	const int & queueSize
) {
	_ssock = SocketWrapper(port, queueSize);
}

UniServerSocket::~UniServerSocket(void) {
	_ssock.close();
}

UniSocket UniServerSocket::accept(void) {
	SocketWrapper sw = _ssock.accept();
	UniSocket us = UniSocket(sw._ip, sw);
	return us;
}
