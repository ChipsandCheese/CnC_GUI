#include "mainwindow.h"
#include "./ui_mainwindow.h"

//Build Variables.  Change Based On Compile Target, or tests may not run.
QString relativePath = "../../Microbenchmarks/"; //The relative path of test files.
QString isa = "x86"; //Defines the ISA for the tests to be run. Needs to match the nomenclature used in the test executables.
QString ext = ".exe"; //Defines the file extension for process executables.

ProcessController *process = new ProcessController(); //Used to spawn tests as processes.
QString programName; //The relative path to the test to be run.

QStringList launchArgs; //used to store process launch arguments for tests that require it.

//This runs when the mainwindow object is called/spawned
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
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
    programName = relativePath + "instructionrate/" + isa + "instructionrate" + ext;
    spawnProcess();
}

//Runs when Memory Bandwidth is selected by user
void MainWindow::on_memBandwidthButton_clicked()
{
    programName = relativePath + "MemoryBandwidth/" + isa + "MemoryBandwidth" + ext;
    launchArgs << "-threads " << QString::number(ui->memBandwidthThreadsBox->value());

    if(ui->memBandwidthPrivateButton->isChecked() == true)
        launchArgs << " -private ";
    else
        launchArgs << " -shared ";
    spawnProcess();
}

//Runs when Memory Latency is selected by user
void MainWindow::on_memLatencyButton_clicked()
{
    programName = relativePath + "MemoryLatency/" + isa + "MemoryLatency" + ext;
    if(ui->hugePagesButton->isChecked() == true)
        launchArgs << "-hugepages ";
    else if(ui->autoNumaButton->isChecked() == true)
        launchArgs << "-autonuma ";
    else if(ui->numaButton->isChecked() == true)
        launchArgs << "-numa ";
    spawnProcess();
}

//Runs when Coherency Latency is selected by user
void MainWindow::on_coherencyLatencyButton_clicked()
{
    programName = relativePath + "CoherencyLatency/" + isa + "CoherencyLatency" + ext;
    launchArgs << "-iterations " << QString::number(ui->coherencyLatencyIterationsBox->value());
    spawnProcess();
}

void MainWindow::spawnProcess()
{
    //This blocks ensures the correct test is started, with the correct launch parameters
    process->ProcessController::setProgramName(programName);
    switch(process->ProcessController::startProgram(launchArgs))
    {
        case 0:
            ui->testOutput->appendPlainText("Test Started");
            break;
        case 1:
            ui->testOutput->appendPlainText("Test Failed to Start.\n Check for Missing Files, and ensure you have the correct file permissions.");
            break;
        case 2:
            ui->testOutput->appendPlainText("Test Crashed");
            break;
    }

    ui->testOutput->appendPlainText("test started");

    launchArgs.clear(); //Resets launch arguments for future test starts

    /* Mounts the process output to the test output box.
     * Note: suboptimal solution. should look for a better way to link output stream to UI textbrowser
     */
    connect(process->process, &QProcess::readyReadStandardOutput,
            ui->testOutput, [=](){ui->testOutput->appendPlainText(process->output);});
}

//Stops the test process
void MainWindow::on_stopTestButton_clicked()
{
    if(process->stopProgram() == 0)//Checks if test was actually stopped before printing feedback
        ui->testOutput->appendPlainText("Test Stopped");
}

//Clears the text from test output
void MainWindow::on_clearOutputButton_clicked()
{
    ui->testOutput->clear();
}

