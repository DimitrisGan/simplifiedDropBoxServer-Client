cmake_minimum_required(VERSION 3.12)
project(Client)

#set(CMAKE_CXX_STANDARD 11 -pthread)
SET(CMAKE_CXX_FLAGS -pthread)
add_executable(Client main.cpp assistantFunctions.cpp ErrorsCodes.h hashFunction.cpp myString.cpp mylinkedList.h clientProtocol.h clientTuple.h clientTuple.cpp clientProtocol.cpp socketManipulation.cpp socketManipulation.h circularBuffer.cpp circularBuffer.h criticalSection.cpp criticalSection.h )