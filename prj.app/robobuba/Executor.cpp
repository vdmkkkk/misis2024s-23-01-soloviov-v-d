//
// Created by vdmk on 14.05.2024.
//
#include "Executor.hpp"

void Executor::execute(void (*func)(int var), int var) {
    func(var);
}