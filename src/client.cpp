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
