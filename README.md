# UniSocket

UniSocket is an universal Socket (Wrapper) in C++ and works 
on Linux, OS X and Windows (VisualC++). An example and
some shell and batch scripts for compilation and linking
are included.

## Features

- makes a c++ object orientated version of the Socket API
- actually only TCP
- polling for *recv()* is an option
- clean splitting of potentially OS dependent Code
- uses a header with the size of the transmitted bytes
- header is seperated by 2 newlines

## The Classes

### UniServerSocket Class

- makes a port listening and set the queueSize of parallel connections
- *accept()* gives you a UniSocket which represents the 
  connction to the client

### UniSocket Class

- make a connection to IP-address and port of UniServerSocket
- *send()*, *recv()* and *close()*
- *getIp()* to get the IP-address of your connection partner

### UniSocketException Class

This object is thrown on errors or if you use polling with
the *recv(true)* method. You get the thrown message as
string from the public attribute *_msg*.

### SocketWrapper Class

This class protects you from using OS dependent implemetations
and pretends to be OS indendent. For a nice and clean developing
please do not use this Class; only the UniSocket classes should
fulfil your desire!

## Hints

- polling: *recv(true)* only runs on header receiving!
  After the header is readed, the *revc(true)* is waiting
  until the expected size is received.
- sorry about my strange english
- see **issues**
- feel free to generate shared Libraries! I did it in the past!

## The Example

For the example on Linux, OS X and Windows you need the files:

- client.cpp (example)
- server.cpp (example)
- UniSocket.cpp
- UniSocket.hpp

E.G. on Linux you need the Linux Wrapper code and the shell script
to compile and link the example:

- SocketWrapperLinux.cpp
- SocketWrapperLinux.hpp
- makeLinux.sh

### client.cpp

```c++
#include <iostream>
#include <string>
#include <cstdlib>		//atoi()
#include "UniSocket.hpp"
using namespace std;
    
int main(int argc, char * argv[]) {
	if (argc < 4) {
		cout<<"usage: "<<argv[0]<<" <ip> <port> <message>"<<endl;
		return 1;
	}
	try {
		UniSocket s(argv[1], atoi(argv[2]));
		s.send(argv[3]);
		// blockierends lesen
		string msg = s.recv();
		cout << "server sagt: " << msg << endl;
		s.close();
	} catch(UniSocketException e) {
		cout << e._msg << endl;
	}
	return 0;
}
```

### server.cpp

```c++
#include <iostream>
#include <chrono>		// sleep_for() -> c++2011
#include <thread>		// -> c++2011
#include <cstdlib>		//atoi()
#include "UniSocket.hpp"
using namespace std;
    
#define POLLINGMYSEC 500000
    
void threadHandle(UniSocket usock) {
	string msg;
	// polling bis header kommt, dann blockierend so viele
	// byte lesen, wie es im header steht
	while(true) {
		try {
			msg = usock.recv(true);
			break;
		} catch(UniSocketException & e) {
			this_thread::sleep_for(chrono::microseconds(POLLINGMYSEC));
		}
	}
	cout << "client "<< usock.getIp() << " sagt: " << msg << endl;
	usock.send("Hallo "+ usock.getIp() + ". Warum " + msg + "?");
	usock.close();
}
    
int main(int argc, char * argv[]) {
	if (argc < 2) {
		cout<<"usage: "<<argv[0]<<" <port>"<<endl;
		return 1;
	}
	try {
		UniServerSocket svr(atoi(argv[1]), 5);
		while (1) {
			thread t(threadHandle, svr.accept());
			t.detach();
		}
	} catch(UniSocketException e) {
		cout << e._msg << endl;
	}
	return 0;
}
```
