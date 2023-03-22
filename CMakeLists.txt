cmake_minimum_required (VERSION 3.11)
project (uni_socket CXX)
set (CMAKE_CXX_STANDARD 11)
find_package(Threads)
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_library(UniSocket STATIC src/UniSocket.cpp)
target_include_directories(UniSocket PUBLIC include)

add_library(SocketWrapper STATIC src/SocketWrapper.cpp)
target_include_directories(SocketWrapper PUBLIC include)

add_executable(UniSocketClient src/client.cpp)
target_link_libraries(UniSocketClient PRIVATE UniSocket SocketWrapper)

add_executable(UniSocketServer src/server.cpp)
target_link_libraries(UniSocketServer PRIVATE UniSocket SocketWrapper Threads::Threads)
