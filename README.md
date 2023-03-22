# UniSocket

UniSocket is an universal Socket (Wrapper) in C++ and works 
on Linux, OS X and Windows (VisualC++).

## Features

- makes a c++ object orientated version of the Socket API
- actually only TCP
- polling for *recv()*
- clean splitting of potentially OS dependent Code

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

This object is thrown on errors. You get the thrown message as
string from the public attribute *_msg*.

### SocketWrapper Class

This class protects you from using OS dependent implemetations
and pretends to be OS indendent. For a nice and clean developing
please do not use this Class; only the UniSocket classes should
fulfil your desire!

## Build The Example

For the example on Linux, OS X and Windows you need cmake. Then do:

- mkdir build
- cd build
- cmake ..
- cmake --build . -v

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
    
#define POLLINGMYSEC 100000
    
void threadHandle(UniSocket usock) {
	string msg;
	while(true) {
		try {
			msg = usock.recv(true);
			break;
		} catch(UniSocketException & e) {
			this_thread::sleep_for(chrono::microseconds(POLLINGMYSEC));
		}
	}
	cout << "client "<< usock.getIp() << " says: " << msg << endl;
	usock.send("Hallo "+ usock.getIp() + ". Why " + msg + "?");
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
