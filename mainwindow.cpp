#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QFileDialog>
#include <QProcess>
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QTimeLine>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QPauseAnimation>

#include <QStandardItemModel>
#include <GUI\Data\Player\player.h>

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
    delete group;
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

        Player* playerPaxo = new Player(123, "Paxo");
        playerPaxo->setPlayerSide("CT");


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

                if (str.contains("PLAYER INFO")){
                    QStringList items = str.split(", ");
                    // items[2] = roundnumber -> round 2
                    if(items[2] == "2"){
                        // items[6] = playername
                        if(items[6] == "Paxo"){
                            bool helmet = false;
                            bool kit = false;
                            if (items[12] == "true") helmet = true;
                            if (items[13] == "true") kit = true;
                            playerPaxo->addPlayerInfo(items[1].toInt(), QPointF(items[4].toFloat()/this->mapRatio, items[5].toFloat()/this->mapRatio),
                                                    items[8].toFloat(), items[9].toUShort(), items[10].toUShort(), items[11].toUShort(), helmet, kit, items[14]);

                        }
                    }
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

        for(int i=0; i<playerPaxo->getListPlayerInfo().size(); i++){
            qDebug() << playerPaxo->getListPlayerInfo().takeAt(i).getTick() << playerPaxo->getListPlayerInfo().takeAt(i).getArmor() << playerPaxo->getListPlayerInfo().takeAt(i).getHealth() << playerPaxo->getListPlayerInfo().takeAt(i).getPlayerPosition() << endl;
        }


    }
}



void MainWindow::on_playButton_clicked()
{
    if(this->group != nullptr){
        if(this->gAnimationStarted == false){
            this->group->start();
        }else{
            this->group->setPaused(false);
        }

    }
}

void MainWindow::on_stopButton_clicked()
{
    if(this->group != nullptr){
        this->group->setPaused(true);
        this->gAnimationStarted = true;
    }
}
