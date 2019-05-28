cmake_minimum_required(VERSION 3.12)
project(Client)

set(CMAKE_CXX_STANDARD 11)

add_executable(Client main.cpp assistantFunctions.cpp ErrorsCodes.h hashFunction.cpp myString.cpp mylinkedList.h circularBuffer.h clientProtocol.h tuples.h clientProtocol.cpp socketManipulation.cpp socketManipulation.h)