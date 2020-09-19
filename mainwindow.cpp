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
    delete scene;
}


void MainWindow::on_actionOpen_Demo_triggered()
{
    // open file browser
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Demo"), QDir::currentPath(), tr("Demo Files (*.dem)"));
    qDebug() << fileName << endl;

    if( !fileName.isNull()){
        // configure connect Signal, to extract information
        QProcess* process = new QProcess();
        int round = 0;
        QObject::connect(process,&QProcess::readyRead,process,[&](){
            QString result = process->readAllStandardOutput();
            QStringList firstList = result.split("\n");

            foreach (const QString &str, firstList) {
                if (str.contains("STARTINFO")){
                    QStringList items = str.split(", ");
                    this->mapName = items[1];

                    // Get Teamname A
                    if (items[2].contains("[READY]")){
                        QStringList name = items[2].split("READY] ");
                        this->teamNameA = name[1];
                    }else{
                        this->teamNameA = items[2];
                    }

                    // Get Teamname B
                    if (items[3].contains("[READY]")){
                        QStringList name = items[3].split("READY] ");
                        this->teamNameB = name[1];
                    }else{
                        this->teamNameB = items[3];
                    }
                }
                if (str.contains("ROUND START")){
                    //qDebug() << str << endl;
                    round = round + 1;
                }
            }

        });

        // parse demofile
        QString cmd = "go run parse_demofile.go -demo " + fileName.toUtf8();
        process->start(cmd);
        process->waitForFinished(-1); // sets current thread to sleep and waits for pingProcess end

        // load map as Pixmap
        QString mapPath = ".\\map\\";
        mapPath.append(this->mapName);
        mapPath.append(".jpg");
        QPixmap map(mapPath);
        map = map.scaled(1024/this->mapRatio, 1024/this->mapRatio, Qt::KeepAspectRatio);

        // Add  graphics scene
        this->scene = new QGraphicsScene();
        ui->graphicsView->setScene(this->scene);    // Set grapihcs scene into graphicsView
        this->scene->addPixmap(map);                // Add Map

        ui->teamNameA->setText(this->teamNameA);
        ui->teamNameB->setText(this->teamNameB);
        qDebug() << round << endl;
    }
}
