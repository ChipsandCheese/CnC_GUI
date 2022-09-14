#include "processcontroller.h"

// creates the process in a known, defined state
ProcessController::ProcessController()
{
    process = nullptr;
    open = 0;
}

// sets the microbench process executable relative path
void ProcessController::setProgramName(QString newProgramName)
{
    program = newProgramName;
}

// shuts down the process when called
int ProcessController::stopProgram()
{
    if(open == 1)
    {
        open = 0;
        process->kill();
        return 0;
    }
    else
        return 1;
}
