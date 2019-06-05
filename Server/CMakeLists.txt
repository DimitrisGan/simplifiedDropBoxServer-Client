cmake_minimum_required(VERSION 3.12)
project(Server)

set(CMAKE_CXX_STANDARD 11)

add_executable(Server main.cpp assistantFunctions.cpp mylinkedList.h myString.cpp ErrorsCodes.h serverProtocol.h serverProtocol.cpp socketManipulation.cpp socketManipulation.h clientTuple.cpp clientTuple.h)