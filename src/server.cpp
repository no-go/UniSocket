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
			msg = usock.recv();
			break;
		} catch(UniSocketException & e) {
			this_thread::sleep_for(chrono::microseconds(POLLINGMYSEC));
		}
	}
	cout << usock.getIp() << ": " << msg << endl;
	usock.close();
}
    
int main(int argc, char * argv[]) {
	if (argc < 2) {
		cout<<"usage: "<<argv[0]<<" <port>"<<endl;
		return 1;
	}
	try {
		UniServerSocket svr(atoi(argv[1]), 150);
		while (1) {
			thread t(threadHandle, svr.accept());
			t.detach();
		}
	} catch(UniSocketException e) {
		cout << e._msg << endl;
	}
	return 0;
}
