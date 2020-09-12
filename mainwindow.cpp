#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QFileDialog>
#include <QProcess>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_Demo_triggered()
{
    // open file browser
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Demo"), QDir::currentPath(), tr("Demo Files (*.dem)"));
    qDebug() << fileName << endl;

    if( !fileName.isNull()){
        // lets read the demo information

        QProcess* process = new QProcess();
        int round = 0;
        QObject::connect(process,&QProcess::readyRead,process,[&](){
            QString result = process->readAllStandardOutput();
            QStringList firstList = result.split("\n");

            foreach (const QString &str, firstList) {
                if (str.contains("[ROUND END]")){
                    qDebug() << str << endl;
                }
                if (str.contains("[ROUND START]")){
                    qDebug() << str << endl;
                    round = round + 1;
                }
                if (str.contains("[MATCH START]")){
                    qDebug() << str << endl;
                }
            }

        });

        QString cmd = "go run parse_demofile.go -demo " + fileName.toUtf8();
        process->start(cmd);
        process->waitForFinished(-1); // sets current thread to sleep and waits for pingProcess end
        qDebug() << round << endl;
    }
}
