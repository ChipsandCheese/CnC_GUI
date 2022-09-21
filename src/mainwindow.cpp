#include "mainwindow.h"
#include "./ui_mainwindow.h"

//Handles the working directory for tests
QDir testPath;

// This can be configured using the "RELBENCHPATH" environment variable
QString relativePath = "../../Microbenchmarks/"; //The relative path of test files.

/* This seems completely pointless but it has been left here
 * for now - it can be configured using the "ISAPREFIX"
 * environment variable, or QSysInfo
*/
QString isaPrefix;

// File extension (for windows), should not be a global variable.
QString ext;

ProcessController* process = new ProcessController(); //Used to spawn tests as processes.
QString programName; //The relative path to the test to be run.

QStringList launchArgs; //used to store process launch arguments for tests that require it.

//This runs when the mainwindow object is called/spawned
MainWindow::MainWindow(QWidget* parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    //checks host platform type to assign correct file extension
    if(QSysInfo::productType() == "windows")
        ext = ".exe";

    relativePath = qEnvironmentVariable("RELBENCHPATH", relativePath);
    isaPrefix = qEnvironmentVariable("ISAPREFIX", QString{""});//Unused

    //Set working directory for the tests when window is spawned
    testPath.setPath(QCoreApplication::applicationDirPath());
    testPath.cd(relativePath);

    ui->setupUi(this);
}

//This runs when the mainwindow object is destroyed
MainWindow::~MainWindow()
{
    delete ui;
    process->stopProgram(); //Remove if processes are spawned in sub windows
}

//Runs when Instruction Rate is selected by user
void MainWindow::on_instructionRateButton_clicked()
{
    testPath.cd("instructionrate/");
    programName = testPath.filePath(isaPrefix + "instructionrate" + ext);
    testPath.cdUp();
    spawnProcess();
}

//Runs when Memory Bandwidth is selected by user
void MainWindow::on_memBandwidthButton_clicked()
{
    testPath.cd("MemoryBandwidth/");
    programName = testPath.filePath(isaPrefix + "MemoryBandwidth" + ext);
    testPath.cdUp();

    launchArgs << "-threads " << QString::number(ui->memBandwidthThreadsBox->value());

    if(ui->memBandwidthPrivateButton->isChecked())
        launchArgs << " -private ";
    else
        launchArgs << " -shared ";

    spawnProcess();
}

//Runs when Memory Latency is selected by user
void MainWindow::on_memLatencyButton_clicked()
{
    testPath.cd("MemoryLatency/");
    programName = testPath.filePath(isaPrefix + "MemoryLatency" + ext);
    testPath.cdUp();

    if(ui->hugePagesButton->isChecked())
        launchArgs << "-hugepages ";
    else if(ui->autoNumaButton->isChecked())
        launchArgs << "-autonuma ";
    else if(ui->numaButton->isChecked())
        launchArgs << "-numa ";

    spawnProcess();
}

//Runs when Coherency Latency is selected by user
void MainWindow::on_coherencyLatencyButton_clicked()
{
    testPath.cd("CoherencyLatency/");
    programName = testPath.filePath(isaPrefix + "CoherencyLatency" + ext);
    testPath.cdUp();

    launchArgs << "-iterations " << QString::number(ui->coherencyLatencyIterationsBox->value());

    spawnProcess();
}

void MainWindow::spawnProcess()
{
    //This block ensures the correct test is started, with the correct launch parameters
    process->ProcessController::setProgramName(programName);
    process->ProcessController::startProgram(launchArgs, this);
    launchArgs.clear(); //Resets launch arguments for future test starts
}

//Helper function used to output test output to the UI textbox
void MainWindow::printOut()
{
    ui->testOutput->appendPlainText(process->output);
}

//Stops the test process
void MainWindow::on_stopTestButton_clicked()
{
    // Checks if test was actually stopped before printing feedback
    if(!process->stopProgram())
    {
        ui->testOutput->appendPlainText("Test Stopped");
    }
}

//Clears the text from test output
void MainWindow::on_clearOutputButton_clicked()
{
    ui->testOutput->clear();
}

//Helper function used to handle errors that may occur
void MainWindow::errorHandle(const QProcess &failedInvocation)
{
    //Block errors when process isn't running.  Also blocks crash error when stopProgram() is called.
    if(process->open)
    {
        //Retrieve error information
        const auto error = failedInvocation.error();
        const QString procPath = failedInvocation.program();
        QFileInfo fileInfo{procPath};
        const auto ctxt = fileInfo.exists() ? "File exist, check permissions" : "File doesn't exist";

        //Print file path, and error information
        qDebug() << procPath << "\n\t" << ctxt;
        qDebug() << "\t" << error;
        ui->testOutput->appendPlainText("Test failed, please inspect the log.");
    }
    process->open = false; //Clears the process state variable so it can be restarted.
}
