//
// Created by vdmk on 14.05.2024.
//

#ifndef MISIS2024S_23_01_SOLOVIOV_V_D_EXECUTOR_H
#define MISIS2024S_23_01_SOLOVIOV_V_D_EXECUTOR_H

#endif //MISIS2024S_23_01_SOLOVIOV_V_D_EXECUTOR_H
#include "Command.hpp"
#include "vector"
class Executor {
public:
    Executor() = default;
    ~Executor() = default;
    int i = 0;
    std::vector <Command> list;

    void execute(void (*func)(int var), int var);
};