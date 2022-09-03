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

// launches microbench
int ProcessController::startProgram(QStringList launchargs)
{
    if(!open)
    {
        // process creation and init logic
        open = 1; // activate the lock to prevent new instances from spawning
        process = new QProcess(this); // create new process

        // if the process reports an error -- release the lock, report error, and return appropriate error code
        connect(process, &QProcess::errorOccurred, this, [=]()
        {
            open = 0;
            // debug logic to track errors
            qDebug() << process->error(); // for debugging purposes -- outputs any errors the process reports in the output console. note: QProcess::UnknownError is the default return value for this
            return process->error();
        });

        // Output handling logic
        connect(process, &QProcess::readyReadStandardOutput, [this]()
        {
                output = process->readAllStandardOutput();
            }); // connects to the process' output signal and accepts input from the stdout stream and appends it to a buffer
        connect(process, &QProcess::readyReadStandardError, [this]()
        {
                output = process->readAllStandardError();
            }); // connects to the process' output signal and accepts input from the stderr stream and appends it to a buffer

        process->setProcessChannelMode(QProcess::MergedChannels);
        process->start(program, launchargs); // start the microbenchmark process

        // when the process signals that it has finished -- release the lock and free process memory
        connect(process, &QProcess::finished, this, [=](){
        open = 0;
        delete process;
        });

        return 0;
    }
}

// shuts down the process when called
int ProcessController::stopProgram() {
    if(open == 1)
    {
        process->kill();
        open = 0;
        return 0;
    }
    else
        return 1;
}
