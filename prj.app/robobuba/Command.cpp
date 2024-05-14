//
// Created by vdmk on 14.05.2024.
//
#include "Command.hpp"
#include <iostream>
void Command::execute(int newPtr) {
    std::cout<<"123";
}

void LEFTCommand::execute(int newPtr) {
    std::cout<<"321";
}

void Command::revert() {
    int temp = prevPointer;
    prevPointer = newPointer;
    newPointer = temp;
}