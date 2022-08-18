#include "processcontroller.h"

// creates the process in a known, defined state
ProcessController::ProcessController() {
    process = nullptr;
    open = 0;
}

// sets the microbench process executable relative path
void ProcessController::setProgramName(QString newProgramName) {
    program = newProgramName;
}

// launches microbench
int ProcessController::startProgram(QStringList launchargs) {
    if(!open) {
        // process creation and init logic
        open = 1; // activate the lock to prevent new instances from spawning
        process = new QProcess(this); // create new process

        // if the process reports an error -- release the lock (note: we should agree on how we'll handle errors in the future)
        connect(process, &QProcess::errorOccurred, this, [=](){open = 0;});

        // Output handling logic
        connect(process, &QProcess::readyReadStandardOutput, [this]() {
                output = process->readAllStandardOutput();
            }); // connects to the process' output signal and accepts input from the stdout stream and appends it in the text window created in the prev section
        connect(process, &QProcess::readyReadStandardError, [this]() {
                output = process->readAllStandardError();
            }); // connects to the process' output signal and accepts input from the stderr stream and appends it in the text window created in the prev section

        process->setProcessChannelMode(QProcess::MergedChannels);
        process->start(program, launchargs); // start the microbenchmark process

        // debug logic to track errors
        qDebug() << process->error(); // for debugging purposes -- outputs any errors the process reports in the output console. note: QProcess::UnknownError is the default return value for this

        // when the process signals that it has finished -- release the lock and free process memory
        connect(process, &QProcess::finished, this, [=](){
        open = 0;
        delete process;
        });
        return 0;

    }
    else {
        return 1; // error
    }
}

// shuts down the process when called
int ProcessController::stopProgram() {
    if(open == 1)
    {
        process->kill();
        return 0;
    }
    else
        return 1;
}
