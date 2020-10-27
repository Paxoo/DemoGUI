#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QFileDialog>
#include <QProcess>
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QTimeLine>

#include <GUI\Data\parser.h>
#include <GUI\Stats\statswindow.h>

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
    listRoundButton.clear();
}

void MainWindow::create_roundButtons(int rounds)
{
    listRoundButton.append(ui->roundButton);
    listRoundButton.append(ui->roundButton_2);
    listRoundButton.append(ui->roundButton_3);
    listRoundButton.append(ui->roundButton_4);
    listRoundButton.append(ui->roundButton_5);
    listRoundButton.append(ui->roundButton_6);
    listRoundButton.append(ui->roundButton_7);
    listRoundButton.append(ui->roundButton_8);
    listRoundButton.append(ui->roundButton_9);
    listRoundButton.append(ui->roundButton_10);
    listRoundButton.append(ui->roundButton_11);
    listRoundButton.append(ui->roundButton_12);
    listRoundButton.append(ui->roundButton_13);
    listRoundButton.append(ui->roundButton_14);
    listRoundButton.append(ui->roundButton_15);
    listRoundButton.append(ui->roundButton_16);
    listRoundButton.append(ui->roundButton_17);
    listRoundButton.append(ui->roundButton_18);
    listRoundButton.append(ui->roundButton_19);
    listRoundButton.append(ui->roundButton_20);
    listRoundButton.append(ui->roundButton_21);
    listRoundButton.append(ui->roundButton_22);
    listRoundButton.append(ui->roundButton_23);
    listRoundButton.append(ui->roundButton_24);
    listRoundButton.append(ui->roundButton_25);
    listRoundButton.append(ui->roundButton_26);
    listRoundButton.append(ui->roundButton_27);
    listRoundButton.append(ui->roundButton_28);
    listRoundButton.append(ui->roundButton_29);
    listRoundButton.append(ui->roundButton_30);

    for (int i = 0; i<rounds; i++){
        color_roundButtons(listRoundButton.at(i), this->pMatch->getRounds().at(i)->getRoundWinner());
    }

    // hide the other buttons
    for(int i = rounds; i<listRoundButton.size(); i++){
        listRoundButton.at(i)->setVisible(false);
    }
}

void MainWindow::color_roundButtons(QPushButton *button, QString team)
{
    if (team == "T"){
        button->setStyleSheet("background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f86900, stop: 1 #f99145);"
                                       "border :1px solid ;"
                                       "border-top-color : black; "
                                       "border-left-color :black;"
                                       "border-right-color :black;"
                                       "border-bottom-color : black");
    }else if(team == "CT"){
        button->setStyleSheet("background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #0069b3, stop: 1 #4d9ad1);"
                                       "border :1px solid ;"
                                       "border-top-color : black; "
                                       "border-left-color :black;"
                                       "border-right-color :black;"
                                       "border-bottom-color : black;");
    }else{
        button->setStyleSheet("background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #0069b3, stop: 1 #FFFFF);"
                                       "border :1px solid ;"
                                       "border-top-color : black; "
                                       "border-left-color :black;"
                                       "border-right-color :black;"
                                       "border-bottom-color : black");
    }
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

        this->gDemoParsed = true;

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

        create_roundButtons(this->pMatch->getRounds().size());

        /*int eflashes = 0;
        int tflashes = 0;
        int kill = 0;
        int death = 0;
        int assist = 0;*/
        for (int i=0; i < this->pMatch->getRounds().size(); i++){
            qDebug() << "--- --- Round" << i+1 << endl;
            qDebug() << this->pMatch->getRounds().at(i)->getStartTick()
                     << this->pMatch->getRounds().at(i)->getFreezeEndTick()
                     << this->pMatch->getRounds().at(i)->getEndTick()
                     << this->pMatch->getRounds().at(i)->getStartTscore() << this->pMatch->getRounds().at(i)->getStartCTscore()
                     << this->pMatch->getRounds().at(i)->getReasonRoundEnd()
                     << this->pMatch->getRounds().at(i)->getRoundWinner() << endl;
            //qDebug() << this->pMatch->getRounds().at(i)->getTmoneySpentTotal() << this->pMatch->getRounds().at(i)->getTmoneySpentRound() << this->pMatch->getRounds().at(i)->getTfreezetimeEndEquipValue() << endl;
            //qDebug() << this->pMatch->getRounds().at(i)->getCTmoneySpentTotal() << this->pMatch->getRounds().at(i)->getCTmoneySpentRound() << this->pMatch->getRounds().at(i)->getCTfreezetimeEndEquipValue() << endl;

            /*qDebug() << "--- Player ---" << this->pMatch->getRounds().at(i)->getListPlayer().size() << endl;
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

                    eflashes = eflashes + this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getEnemyFlashes();
                    tflashes = tflashes + this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getTeamFlahes();
                    kill = kill + this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getKills();
                    death = death + this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getDeaths();
                    assist = assist + this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getAssists();
                    qDebug() << this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getListPlayerInfo().last()->getTick()
                             << this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getListPlayerInfo().last()->getHealth()
                             << this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getListPlayerInfo().last()->getArmor()
                             << this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getListPlayerInfo().last()->getActiveWeapon()
                             << this->pMatch->getRounds().at(i)->getListPlayer().at(j)->getListPlayerInfo().last()->getPlayerPosition() << endl;
                }
            }*/
        }
    }
}



void MainWindow::on_playButton_clicked()
{
    /*if(this->group != nullptr){
        if(this->gAnimationStarted == false){
            this->group->start();
        }else{
            this->group->setPaused(false);
        }

    }*/
}

void MainWindow::on_stopButton_clicked()
{
    /*if(this->group != nullptr){
        this->group->setPaused(true);
        this->gAnimationStarted = true;
    }*/
}

void MainWindow::on_actionStats_2_triggered()
{
    if(this->gDemoParsed == true){
        StatsWindow *dialog = new StatsWindow();
        dialog->fillTableModels(this->pMatch);
        dialog->createBarChart();
        dialog->createPieChart();
        dialog->show();
    }
}

void MainWindow::on_roundButton_clicked()
{
    qDebug() << "1" << endl;
}

void MainWindow::on_roundButton_2_clicked()
{
    qDebug() << "2" << endl;
}

void MainWindow::on_roundButton_3_clicked()
{
    qDebug() << "3" << endl;
}

void MainWindow::on_roundButton_4_clicked()
{
    qDebug() << "4" << endl;
}

void MainWindow::on_roundButton_5_clicked()
{
    qDebug() << "5" << endl;
}

void MainWindow::on_roundButton_6_clicked()
{
    qDebug() << "6" << endl;
}

void MainWindow::on_roundButton_7_clicked()
{
    qDebug() << "7" << endl;
}

void MainWindow::on_roundButton_8_clicked()
{
    qDebug() << "8" << endl;
}

void MainWindow::on_roundButton_9_clicked()
{
    qDebug() << "9" << endl;
}

void MainWindow::on_roundButton_10_clicked()
{
    qDebug() << "10" << endl;
}

void MainWindow::on_roundButton_11_clicked()
{
    qDebug() << "11" << endl;
}

void MainWindow::on_roundButton_12_clicked()
{
    qDebug() << "12" << endl;
}

void MainWindow::on_roundButton_13_clicked()
{
    qDebug() << "13" << endl;
}

void MainWindow::on_roundButton_14_clicked()
{
    qDebug() << "14" << endl;
}

void MainWindow::on_roundButton_15_clicked()
{
    qDebug() << "15" << endl;
}

void MainWindow::on_roundButton_16_clicked()
{
    qDebug() << "16" << endl;
}

void MainWindow::on_roundButton_17_clicked()
{
    qDebug() << "17" << endl;
}

void MainWindow::on_roundButton_18_clicked()
{
    qDebug() << "18" << endl;
}

void MainWindow::on_roundButton_19_clicked()
{
    qDebug() << "19" << endl;
}

void MainWindow::on_roundButton_20_clicked()
{
    qDebug() << "20" << endl;
}

void MainWindow::on_roundButton_21_clicked()
{
    qDebug() << "21" << endl;
}

void MainWindow::on_roundButton_22_clicked()
{
    qDebug() << "22" << endl;
}

void MainWindow::on_roundButton_23_clicked()
{
    qDebug() << "23" << endl;
}

void MainWindow::on_roundButton_24_clicked()
{
    qDebug() << "24" << endl;
}

void MainWindow::on_roundButton_25_clicked()
{
    qDebug() << "25" << endl;
}

void MainWindow::on_roundButton_26_clicked()
{
    qDebug() << "26" << endl;
}

void MainWindow::on_roundButton_27_clicked()
{
    qDebug() << "27" << endl;
}

void MainWindow::on_roundButton_28_clicked()
{
    qDebug() << "28" << endl;
}

void MainWindow::on_roundButton_29_clicked()
{
    qDebug() << "29" << endl;
}

void MainWindow::on_roundButton_30_clicked()
{
    qDebug() << "30" << endl;
}
