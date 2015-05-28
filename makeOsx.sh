#!/bin/bash

g++ -Wall -c UniSocket.cpp
g++ -Wall -c SocketWrapperOsx.cpp

g++ -Wall -std=c++11 -c server.cpp
g++ -Wall -std=c++11 -o serverOsx server.o SocketWrapperOsx.o UniSocket.o

g++ -Wall -c client.cpp
g++ -Wall -o clientOsx client.o SocketWrapperOsx.o UniSocket.o

rm -f *.o

