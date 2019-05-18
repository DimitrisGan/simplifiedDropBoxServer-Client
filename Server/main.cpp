#include <iostream>
#include "assistantFunctions.h"

int main(int argc, char **argv) {
    std::cout << "Hello, World!" << std::endl;

    ArgumentsKeeper argmKeeper;
    argmParser(argc, argv , argmKeeper);

    cout<<argmKeeper.portNumber<<endl;



    return 0;
}