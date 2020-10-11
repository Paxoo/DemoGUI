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

        for (int i=0; i < this->pMatch->getRounds().size(); i++){
            qDebug() << this->pMatch->getRounds().takeAt(i)->getStartTick()
                     << this->pMatch->getRounds().takeAt(i)->getFreezeEndTick()
                     << this->pMatch->getRounds().takeAt(i)->getEndTick()
                     << this->pMatch->getRounds().takeAt(i)->getStartTscore() << this->pMatch->getRounds().takeAt(i)->getStartCTscore()
                     << this->pMatch->getRounds().takeAt(i)->getReasonRoundEnd() << endl;
            qDebug() << this->pMatch->getRounds().takeAt(i)->getTmoneySpentTotal() << this->pMatch->getRounds().takeAt(i)->getTmoneySpentRound() << this->pMatch->getRounds().takeAt(i)->getTfreezetimeEndEquipValue() << endl;
            qDebug() << this->pMatch->getRounds().takeAt(i)->getCTmoneySpentTotal() << this->pMatch->getRounds().takeAt(i)->getCTmoneySpentRound() << this->pMatch->getRounds().takeAt(i)->getCTfreezetimeEndEquipValue() << endl;
        }



        /*for(int i=0; i<this->pMatch->getRounds().first().getListPlayer().first().getListPlayerInfo().size(); i++){
            qDebug() << this->pMatch->getRounds().first().getListPlayer().first().getListPlayerInfo().takeAt(i).getTick() << this->pMatch->getRounds().last().getListPlayer().first().getListPlayerInfo().takeAt(i).getActiveWeapon() << endl;
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
