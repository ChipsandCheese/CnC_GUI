#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*Program: Chips and Cheese GUI
 *Class: mainwindow.cpp
 *Programmer: Nintonito
 *Date Created: July 21, 2022
 *Date Updated: September 2, 2022
 *Version 0.2
 *
 *This class generates a main window with various buttons, used for starting micro benchmarks.
 *A text box is provided to view the output of the tests in real-time.S
 *Test execution is handled via instantiating the ProcessController class and calling related functions.
 */

#include <QMainWindow>
#include <string>
#include "processcontroller.h"
#include <iostream>
#include <QString>
#include <QStringList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

    ~MainWindow() override;

    void spawnProcess();

public slots:

    void errorHandle(QProcess::ProcessError error);

    void printOut();

private slots:

    void on_instructionRateButton_clicked();

    void on_memBandwidthButton_clicked();

    void on_memLatencyButton_clicked();

    void on_coherencyLatencyButton_clicked();

    void on_stopTestButton_clicked();

    void on_clearOutputButton_clicked();

private:
    Ui::MainWindow* ui;
};

#endif // MAINWINDOW_H
