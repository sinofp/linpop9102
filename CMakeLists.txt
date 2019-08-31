cmake_minimum_required(VERSION 3.14)
project(qq)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

add_executable(qq epolltest.c msg.h helper.cpp helper.h)
add_executable(client tcpclient.c)