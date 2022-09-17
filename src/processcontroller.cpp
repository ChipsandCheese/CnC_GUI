#include "processcontroller.h"

#include <utility>

// creates the process in a known, defined state
ProcessController::ProcessController()
{
    process = nullptr;
    open = false;
}

// sets the microbench process executable relative path
void ProcessController::setProgramName(QString newProgramName)
{
    program = std::move(newProgramName);
}

// shuts down the process when called
bool ProcessController::stopProgram()
{
    if (open)
    {
        open = false;//Necessary to block feedback from program once killed.
        process->kill();
        return 0;
    } else
    {
        return 1;
    }
}
