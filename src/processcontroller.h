#ifndef PROCESSCONTROLLER_H
#define PROCESSCONTROLLER_H

#include <QProcess>
#include <QTextBrowser>

/*Program: Chips and Cheese GUI
 *Class: processcontroller.cpp
 *Programmer: Mohamed
 *Date Created: July 21, 2022
 *Date Updated: September 13, 2022
 *Version 0.3
 *
 *This class generates a QProcess object that is used to start program executables as sub-processes of the graphical interface.
 *Launch arguments for the subprocesses can be provided via a QStringList
 *Output is connected to a slot, which can be read for output within an existing QT GUI object
 */

class ProcessController : public QObject
{
Q_OBJECT
private:
    QString program;

public:
    QProcess* process;
    bool open;
    QByteArray output;

    ProcessController();

    void setProgramName(QString newProgramName);

    // launches microbench
    template<typename GUI>
    bool startProgram(const QStringList& launchargs, GUI* gui)
    {
        if (!open)
        {
            // process creation and init logic
            open = true; // activate the lock to prevent new instances from spawning
            process = new QProcess(this); // create new process

            // if the process reports an error -- release the lock, report error, and return appropriate error code
            connect(process, &QProcess::errorOccurred, gui, [=]()
            {
                gui->errorHandle(*process);
            });

            // output handling logic
            process->setProcessChannelMode(QProcess::MergedChannels); // directs all output to the stdout stream
            connect(process, &QProcess::readyReadStandardOutput, gui, [=]()
            {
                output = process->readAllStandardOutput();
                gui->printOut();
            }); // connects to the process' output signal and accepts input from the stdout stream and appends it to a buffer, then signals to the GUI to print the buffer

            process->start(program, launchargs); // start the microbenchmark process

            // when the process signals that it has finished -- release the lock and free process memory
            connect(process, &QProcess::finished, this, [=]()
            {
                open = false;
                delete process;
            });

            return false;
        } else
        {
            return true;
        }
    }

    bool stopProgram();
};

#endif // PROCESSCONTROLLER_H
