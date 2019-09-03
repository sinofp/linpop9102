cmake_minimum_required(VERSION 3.10)
project(qq)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

LINK_DIRECTORIES("/usr/lib/mysql")
LINK_LIBRARIES(mysqlclient)

add_executable(qq epolltest.c msg.h helper.cpp helper.h database.c database.h)
add_executable(client tcpclient.c)
