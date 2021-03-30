//
// Created by dawnoftherent on 3/14/21.
//

#include "debug.h"
#include "Tests.h"

/**
 * Simple test
 * @param x -some int
 * @return 1+x
 */

int Tests::testInt(int x){
    PDBG("testCPP","testInt output in cpp")
    return 1+x;
}

std::string Tests::testStr(){
    PDBG("testCPP","testStr output in cpp")
    return std::string("hi");
}

