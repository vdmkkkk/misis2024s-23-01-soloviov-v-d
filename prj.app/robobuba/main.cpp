//
// Created by vdmk on 14.05.2024.
//
#include <iostream>
#include "Executor.hpp"
#include "Command.hpp"

int main() {
    Executor exec;
    LEFTCommand LEFT;
    void (*func)(int var) = &LEFT.execute;
    exec.execute(func, 123);
    return 0;
}