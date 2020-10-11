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
#include <GUI\Data\parser.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->pMatch = new Match();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
    delete pMatch;
    delete group;
}


void MainWindow::on_actionOpen_Demo_triggered()
{
    // open file browser
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Demo"), QDir::currentPath(), tr("Demo Files (*.dem)"));
    qDebug() << fileName << endl;

    if( !fileName.isNull()){
        // get the information!
        Parser parser(*this->pMatch, this->mapRatio);
        parser.runParser(fileName);

        // load map as Pixmap
        QString mapPath = ".\\map\\";
        mapPath.append(this->pMatch->getMap());
        mapPath.append(".jpg");
        QPixmap map(mapPath);
        map = map.scaled(1024/this->mapRatio, 1024/this->mapRatio, Qt::KeepAspectRatio);

        // Add  graphics scene
        this->scene = new QGraphicsScene();
        ui->graphicsView->setScene(this->scene);    // Set grapihcs scene into graphicsView
        this->scene->addPixmap(map);                // Add Map

        ui->teamNameA->setText(this->pMatch->getTeamNameA());
        ui->teamNameB->setText(this->pMatch->getTeamNameB());


        //qDebug() << round << endl;
        qDebug() << this->pMatch->getRounds().size() << endl;


        int eflashes = 0;
        int tflashes = 0;
        for (int i=0; i < this->pMatch->getRounds().size(); i++){
            qDebug() << "--- --- Round" << i+1 << endl;
            qDebug() << this->pMatch->getRounds().at(i)->getStartTick()
                     << this->pMatch->getRounds().at(i)->getFreezeEndTick()
                     << this->pMatch->getRounds().at(i)->getEndTick()
                     << this->pMatch->getRounds().at(i)->getStartTscore() << this->pMatch->getRounds().at(i)->getStartCTscore()
                     << this->pMatch->getRounds().at(i)->getReasonRoundEnd() << endl;
            qDebug() << this->pMatch->getRounds().at(i)->getTmoneySpentTotal() << this->pMatch->getRounds().at(i)->getTmoneySpentRound() << this->pMatch->getRounds().at(i)->getTfreezetimeEndEquipValue() << endl;
            qDebug() << this->pMatch->getRounds().at(i)->getCTmoneySpentTotal() << this->pMatch->getRounds().at(i)->getCTmoneySpentRound() << this->pMatch->getRounds().at(i)->getCTfreezetimeEndEquipValue() << endl;

            qDebug() << "--- Player ---" << this->pMatch->getRounds().at(i)->getListPlayer().size() << endl;
            for(int j=0; j < this->pMatch->getRounds().at(i)->getListPlayer().size(); j++){
                if(this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getID() == "76561197968320879"){
                    qDebug() << this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getID()
                             << this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getName()
                             << this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getPlayerSide()
                             << this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getEnemyFlashes()
                             << endl;
                    eflashes = eflashes + this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getEnemyFlashes();
                    tflashes = tflashes + this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getTeamFlahes();

                    /*qDebug() << this->pMatch->getRounds().at(i)->getListPlayer().last()->getListPlayerInfo().at(i).getTick()
                             << this->pMatch->getRounds().at(i)->getListPlayer().last()->getListPlayerInfo().at(i).getHealth()
                             << this->pMatch->getRounds().at(i)->getListPlayer().last()->getListPlayerInfo().at(i).getArmor()
                             << this->pMatch->getRounds().at(i)->getListPlayer().last()->getListPlayerInfo().at(i).getActiveWeapon()
                             << this->pMatch->getRounds().at(i)->getListPlayer().last()->getListPlayerInfo().at(i).getPlayerPosition() << endl;*/
                }
            }
        }

        qDebug() << eflashes << tflashes << endl;


        /*for(int i=0; i<this->pMatch->getRounds().first().getListPlayer().first().getListPlayerInfo().size(); i++){
            qDebug() << this->pMatch->getRounds().first().getListPlayer().first().getListPlayerInfo().at(i).getTick() << this->pMatch->getRounds().last().getListPlayer().first().getListPlayerInfo().at(i).getActiveWeapon() << endl;
        }*/

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
