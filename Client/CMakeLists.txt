cmake_minimum_required(VERSION 3.12)
project(Client)

set(CMAKE_CXX_STANDARD 11)

add_executable(Client main.cpp assistantFunctions.cpp ErrorsCodes.h hashFunction.cpp myString.cpp mylinkedList.h clientProtocol.h clientTuple.h clientTuple.cpp clientProtocol.cpp socketManipulation.cpp socketManipulation.h threadPool.cpp threadPool.h criticalSection.cpp criticalSection.h )