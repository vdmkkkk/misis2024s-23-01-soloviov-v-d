//
// Created by vdmk on 14.05.2024.
//

#ifndef MISIS2024S_23_01_SOLOVIOV_V_D_COMMAND_H
#define MISIS2024S_23_01_SOLOVIOV_V_D_COMMAND_H

#endif //MISIS2024S_23_01_SOLOVIOV_V_D_COMMAND_H
class Command {
public:
    Command() = default;
    ~Command() = default;
    int prevPointer;
    int newPointer;

    virtual void execute(int newPtr);
    void revert();
};

class LEFTCommand : Command {
public:
    LEFTCommand() = default;
    ~LEFTCommand() = default;
    void execute(int newPtr) override;

};