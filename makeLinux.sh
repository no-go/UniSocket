#!/bin/bash

g++ -Wall -std=c++11 -c UniSocket.cpp
g++ -Wall -std=c++11 -c SocketWrapperLinux.cpp

g++ -Wall -std=c++11 -c server.cpp
g++ -Wall -std=c++11 -pthread -o serverLinux server.o SocketWrapperLinux.o UniSocket.o

g++ -Wall -c client.cpp
g++ -Wall -o clientLinux client.o SocketWrapperLinux.o UniSocket.o

rm -f *.o
