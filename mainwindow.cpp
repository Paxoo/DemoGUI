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


        /*int eflashes = 0;
        int tflashes = 0;
        int kill = 0;
        int death = 0;
        int assist = 0;*/
        for (int i=0; i < this->pMatch->getRounds().size(); i++){
            qDebug() << "--- --- Round" << i+1 << endl;
            /*qDebug() << this->pMatch->getRounds().at(i)->getStartTick()
                     << this->pMatch->getRounds().at(i)->getFreezeEndTick()
                     << this->pMatch->getRounds().at(i)->getEndTick()
                     << this->pMatch->getRounds().at(i)->getStartTscore() << this->pMatch->getRounds().at(i)->getStartCTscore()
                     << this->pMatch->getRounds().at(i)->getReasonRoundEnd() << endl;
            qDebug() << this->pMatch->getRounds().at(i)->getTmoneySpentTotal() << this->pMatch->getRounds().at(i)->getTmoneySpentRound() << this->pMatch->getRounds().at(i)->getTfreezetimeEndEquipValue() << endl;
            qDebug() << this->pMatch->getRounds().at(i)->getCTmoneySpentTotal() << this->pMatch->getRounds().at(i)->getCTmoneySpentRound() << this->pMatch->getRounds().at(i)->getCTfreezetimeEndEquipValue() << endl;
            */
            qDebug() << "--- Player ---" << this->pMatch->getRounds().at(i)->getListPlayer().size() << endl;
            for(int j=0; j < this->pMatch->getRounds().at(i)->getListPlayer().size(); j++){
                if(this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getID() == "76561197968320879"){
                    qDebug() << this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getID()
                             << this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getName()
                             << this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getPlayerSide()
                             << this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getEnemyFlashes()
                             << endl;

                    for(int u=0; u < this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getDMGdone().size(); u++){
                        qDebug() << this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getDMGdone().keys()[u]
                                 << this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getDMGdone().value(this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getDMGdone().keys()[u])
                                 << endl;
                    }

                    /*eflashes = eflashes + this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getEnemyFlashes();
                    tflashes = tflashes + this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getTeamFlahes();
                    kill = kill + this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getKills();
                    death = death + this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getDeaths();
                    assist = assist + this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getAssists();
                    qDebug() << this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getListPlayerInfo().last()->getTick()
                             << this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getListPlayerInfo().last()->getHealth()
                             << this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getListPlayerInfo().last()->getArmor()
                             << this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getListPlayerInfo().last()->getActiveWeapon()
                             << this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getListPlayerInfo().last()->getPlayerPosition() << endl;*/
                }
            }
        }

        //qDebug() << eflashes << tflashes << kill << death << assist << endl;



        /*for(int r = 0; r<5; r++){
            for(int i=0; i<this->pMatch->getRounds().at(r)->getListPlayer().first()->getListPlayerStats().size(); i++){
                qDebug() << this->pMatch->getRounds().at(r)->getListPlayer().first()->getListPlayerStats().at(i)->getTick()
                         << this->pMatch->getRounds().at(r)->getListPlayer().first()->getListPlayerStats().at(i)->getKills()
                         << this->pMatch->getRounds().at(r)->getListPlayer().first()->getListPlayerStats().at(i)->getAssists()
                         << this->pMatch->getRounds().at(r)->getListPlayer().first()->getListPlayerStats().at(i)->getDeaths()
                         << endl;
            }
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
