#ifndef PROCESSCONTROLLER_H
#define PROCESSCONTROLLER_H
#include <QProcess>
#include <QTextBrowser>

/*Program: Chips and Cheese GUI
 *Class: processcontroller.cpp
 *Programmer: Mohamed
 *Date Created: July 21, 2022
 *Date Updated: September 2, 2022
 *Version 0.2
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
    int startProgram(QStringList launchargs);
    int stopProgram();
};

#endif // PROCESSCONTROLLER_H
