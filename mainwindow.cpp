#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QFileDialog>
#include <QProcess>
#include <QDebug>
#include <QMessageBox>

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
    listRoundButton.append(ui->roundButton); listRoundButton.append(ui->roundButton_2);
    listRoundButton.append(ui->roundButton_3); listRoundButton.append(ui->roundButton_4);
    listRoundButton.append(ui->roundButton_5); listRoundButton.append(ui->roundButton_6);
    listRoundButton.append(ui->roundButton_7); listRoundButton.append(ui->roundButton_8);
    listRoundButton.append(ui->roundButton_9); listRoundButton.append(ui->roundButton_10);
    listRoundButton.append(ui->roundButton_11); listRoundButton.append(ui->roundButton_12);
    listRoundButton.append(ui->roundButton_13); listRoundButton.append(ui->roundButton_14);
    listRoundButton.append(ui->roundButton_15); listRoundButton.append(ui->roundButton_16);
    listRoundButton.append(ui->roundButton_17); listRoundButton.append(ui->roundButton_18);
    listRoundButton.append(ui->roundButton_19); listRoundButton.append(ui->roundButton_20);
    listRoundButton.append(ui->roundButton_21); listRoundButton.append(ui->roundButton_22);
    listRoundButton.append(ui->roundButton_23); listRoundButton.append(ui->roundButton_24);
    listRoundButton.append(ui->roundButton_25); listRoundButton.append(ui->roundButton_26);
    listRoundButton.append(ui->roundButton_27); listRoundButton.append(ui->roundButton_28);
    listRoundButton.append(ui->roundButton_29); listRoundButton.append(ui->roundButton_30);
    listRoundButton.append(ui->roundButton_31); listRoundButton.append(ui->roundButton_32);
    listRoundButton.append(ui->roundButton_33); listRoundButton.append(ui->roundButton_34);
    listRoundButton.append(ui->roundButton_35); listRoundButton.append(ui->roundButton_36);
    listRoundButton.append(ui->roundButton_37); listRoundButton.append(ui->roundButton_38);
    listRoundButton.append(ui->roundButton_39); listRoundButton.append(ui->roundButton_40);
    listRoundButton.append(ui->roundButton_41); listRoundButton.append(ui->roundButton_42);
    listRoundButton.append(ui->roundButton_43); listRoundButton.append(ui->roundButton_44);
    listRoundButton.append(ui->roundButton_45); listRoundButton.append(ui->roundButton_46);
    listRoundButton.append(ui->roundButton_47); listRoundButton.append(ui->roundButton_48);

    for (int i = 0; i<rounds; i++){
        if(i > 48){
            color_roundButtons(listRoundButton.last(), "overtime");
            break;
        }
        qDebug() << rounds << i << endl;
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
        button->setStyleSheet("background-color: red;"
                                       "border :1px solid ;"
                                       "border-top-color : black; "
                                       "border-left-color :black;"
                                       "border-right-color :black;"
                                       "border-bottom-color : black");
    }
}

void MainWindow::set_staticText(int round)
{
    QString score =  "";

    // adjust color of team names and adjust money text
    if(round < 16 or (round >= 34 and round <= 39) or (round >= 46 and round <= 48)){
        ui->teamNameA->setStyleSheet("background-color: rgb(94, 98, 108); color: rgb(248, 150, 0);");
        ui->teamNameB->setStyleSheet("background-color: rgb(94, 98, 108); color: rgb(21, 156, 255);");

        ui->teamAMoneySpentTotal->setText(QString::number(this->pMatch->getRounds().at(round-1)->getTmoneySpentTotal()) + " $");
        ui->teamAMoneySpentRound->setText(QString::number(this->pMatch->getRounds().at(round-1)->getTmoneySpentRound()) + " $");
        ui->teamAFreezetimeEndEqpValue->setText(this->pMatch->getRounds().at(round-1)->getTfreezetimeEndEquipValue());

        ui->teamBMoneySpentTotal->setText(QString::number(this->pMatch->getRounds().at(round-1)->getCTmoneySpentTotal()) + " $");
        ui->teamBMoneySpentRound->setText(QString::number(this->pMatch->getRounds().at(round-1)->getCTmoneySpentRound()) + " $");
        ui->teamBFreezetimeEndEqpValue->setText(this->pMatch->getRounds().at(round-1)->getCTfreezetimeEndEquipValue());

    }else if((round >= 16 and round <= 33) or (round >= 40 and round <= 45)){
        ui->teamNameA->setStyleSheet("background-color: rgb(94, 98, 108); color: rgb(21, 156, 255);");
        ui->teamNameB->setStyleSheet("background-color: rgb(94, 98, 108); color: rgb(248, 150, 0);");

        ui->teamAMoneySpentTotal->setText(QString::number(this->pMatch->getRounds().at(round-1)->getCTmoneySpentTotal()) + " $");
        ui->teamAMoneySpentRound->setText(QString::number(this->pMatch->getRounds().at(round-1)->getCTmoneySpentRound()) + " $");
        ui->teamAFreezetimeEndEqpValue->setText(this->pMatch->getRounds().at(round-1)->getCTfreezetimeEndEquipValue());

        ui->teamBMoneySpentTotal->setText(QString::number(this->pMatch->getRounds().at(round-1)->getTmoneySpentTotal()) + " $");
        ui->teamBMoneySpentRound->setText(QString::number(this->pMatch->getRounds().at(round-1)->getTmoneySpentRound()) + " $");
        ui->teamBFreezetimeEndEqpValue->setText(this->pMatch->getRounds().at(round-1)->getTfreezetimeEndEquipValue());

    }else{
        ui->teamNameA->setStyleSheet("background-color: rgb(94, 98, 108); color: red");
        ui->teamNameB->setStyleSheet("background-color: rgb(94, 98, 108); color: red");

        ui->teamAMoneySpentTotal->setText("Paxo");
        ui->teamAMoneySpentRound->setText("ist");
        ui->teamAFreezetimeEndEqpValue->setText("king");

        ui->teamBMoneySpentTotal->setText("One Piece");
        ui->teamBMoneySpentRound->setText("ist die beste");
        ui->teamBFreezetimeEndEqpValue->setText("story ever");

    }
    ui->teamNameA->setText("  " + this->pMatch->getTeamNameA());
    ui->teamNameB->setText(this->pMatch->getTeamNameB());

    // set score
    // scores are updated a round later -> instead of round 16 (teamswitch) round 17
    if(round < 17 or (round >= 35 and round <= 40) or (round >= 47 and round <= 49)){
        score = QString::number(this->pMatch->getRounds().at(round-1)->getStartTscore()) + " : " + QString::number(this->pMatch->getRounds().at(round-1)->getStartCTscore());
    }else if((round >= 17 and round <= 34) or (round >= 41 and round <= 46)){
        score = QString::number(this->pMatch->getRounds().at(round-1)->getStartCTscore()) + " : " + QString::number(this->pMatch->getRounds().at(round-1)->getStartTscore());
    }
    ui->score->setText(score);
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

        create_roundButtons(this->pMatch->getRounds().size());
        set_staticText(1);


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
            qDebug() << this->pMatch->getTeamNameA() << this->pMatch->getTeamNameB() << endl;

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
    if(this->gDemoParsed == true){
        set_staticText(1);
    }
}

void MainWindow::on_roundButton_2_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(2);
    }
}

void MainWindow::on_roundButton_3_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(3);
    }
}

void MainWindow::on_roundButton_4_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(4);
    }
}

void MainWindow::on_roundButton_5_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(5);
    }
}

void MainWindow::on_roundButton_6_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(6);
    }
}

void MainWindow::on_roundButton_7_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(7);
    }
}

void MainWindow::on_roundButton_8_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(8);
    }
}

void MainWindow::on_roundButton_9_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(9);
    }
}

void MainWindow::on_roundButton_10_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(10);
    }
}

void MainWindow::on_roundButton_11_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(11);
    }
}

void MainWindow::on_roundButton_12_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(12);
    }
}

void MainWindow::on_roundButton_13_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(13);
    }
}

void MainWindow::on_roundButton_14_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(14);
    }
}

void MainWindow::on_roundButton_15_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(15);
    }
}

void MainWindow::on_roundButton_16_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(16);
    }
}

void MainWindow::on_roundButton_17_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(17);
    }
}

void MainWindow::on_roundButton_18_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(18);
    }
}

void MainWindow::on_roundButton_19_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(19);
    }
}

void MainWindow::on_roundButton_20_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(20);
    }
}

void MainWindow::on_roundButton_21_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(21);
    }
}

void MainWindow::on_roundButton_22_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(22);
    }
}

void MainWindow::on_roundButton_23_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(23);
    }
}

void MainWindow::on_roundButton_24_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(24);
    }
}

void MainWindow::on_roundButton_25_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(25);
    }
}

void MainWindow::on_roundButton_26_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(26);
    }
}

void MainWindow::on_roundButton_27_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(27);
    }
}

void MainWindow::on_roundButton_28_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(28);
    }
}

void MainWindow::on_roundButton_29_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(29);
    }
}

void MainWindow::on_roundButton_30_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(30);
    }
}

void MainWindow::on_roundButton_31_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(31);
    }
}

void MainWindow::on_roundButton_32_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(32);
    }
}

void MainWindow::on_roundButton_33_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(34);
    }
}

void MainWindow::on_roundButton_34_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(34);
    }
}

void MainWindow::on_roundButton_35_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(35);
    }
}

void MainWindow::on_roundButton_36_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(36);
    }
}

void MainWindow::on_roundButton_37_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(37);
    }
}

void MainWindow::on_roundButton_38_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(38);
    }
}

void MainWindow::on_roundButton_39_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(39);
    }
}

void MainWindow::on_roundButton_40_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(40);
    }
}

void MainWindow::on_roundButton_41_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(41);
    }
}

void MainWindow::on_roundButton_42_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(42);
    }
}

void MainWindow::on_roundButton_43_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(43);
    }
}

void MainWindow::on_roundButton_44_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(44);
    }
}

void MainWindow::on_roundButton_45_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(45);
    }
}

void MainWindow::on_roundButton_46_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(46);
    }
}

void MainWindow::on_roundButton_47_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(47);
    }
}

void MainWindow::on_roundButton_48_clicked()
{
    if(this->gDemoParsed == true){
        set_staticText(48);
    }
}

void MainWindow::on_actionFreezetimeEndEquipValue_triggered()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Classification equipment value");
    msgBox.setText("Round 1 and 16 = Pistol\n\n"
                   "Value < 2000 = Full Eco\n\n"
                   "2000 <= Value < 8500 = Eco\n\n"
                   "8500 <= Value < 22000 = Half Buy\n\n"
                   "Value >= 22000 = Full Buy");
    msgBox.exec();
}

void MainWindow::on_actionGithub_triggered()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Contact information");
    msgBox.setText("For questions, problems or feedback please use the git repository. In this repository, you will find the source code and issue tracker pertaining to the csgo package.");
    msgBox.setInformativeText("<a href=\"https://github.com/Paxoo/DemoGUI\">Github: Paxoo CSGODemo</a>");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.exec();
}
