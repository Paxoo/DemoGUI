#include "statswindow.h"
#include "ui_statswindow.h"


StatsWindow::StatsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatsWindow)
{
    ui->setupUi(this);

    // overall player stats
    pTableOverallStats = new OverallTableModel();
    proxyModelOverallStats = new QSortFilterProxyModel(this);
    proxyModelOverallStats->setSourceModel(pTableOverallStats);

    ui->overalStatsView->setModel(proxyModelOverallStats);
    ui->overalStatsView->setSortingEnabled(true);

}

StatsWindow::~StatsWindow()
{
    delete ui;
    delete pTableOverallStats;
    delete proxyModelOverallStats;
}

void StatsWindow::fillTableOverallStats(Match *match)
{
    // Who is playing?
    for(int p=0; p < match->getListPlayerIDs().size(); p++){
        QString name = "";
        int kills = 0;
        int assists = 0;
        int deaths = 0;
        float kdr = 0.0;
        int enemyFlashes = 0;
        int teamFlashes = 0;
        int utilityDMG = 0;

        // sum up all stats over each round
        for (int r=0; r < match->getRounds().size(); r++){
            for(int j=0; j < match->getRounds().at(r)->getListPlayer().size(); j++){
                if(match->getRounds().at(r)->getListPlayer().at(j)->getID() == match->getListPlayerIDs().at(p)){
                    name = match->getRounds().at(r)->getListPlayer().at(j)->getName();
                    kills = kills + match->getRounds().at(r)->getListPlayer().at(j)->getKills();
                    assists = assists + match->getRounds().at(r)->getListPlayer().at(j)->getAssists();
                    deaths = deaths + match->getRounds().at(r)->getListPlayer().at(j)->getDeaths();
                    enemyFlashes = enemyFlashes + match->getRounds().at(r)->getListPlayer().at(j)->getEnemyFlashes();
                    teamFlashes = teamFlashes + match->getRounds().at(r)->getListPlayer().at(j)->getTeamFlahes();

                    QList<QString> utility = {"Decoy", "Molotov", "Incendiary", "Flash", "Smoke", "HE"};
                    for(int u = 0; u < utility.size(); u++){
                        if(match->getRounds().at(r)->getListPlayer().at(j)->getDMGdone().contains(utility.at(u)) == true){
                            utilityDMG = utilityDMG + match->getRounds().at(r)->getListPlayer().at(j)->getDMGdone().value(utility.at(u));
                        }
                    }
                }
            }
        }
        kdr = (float)kills / (float)deaths;
        // add entry
        addEntryOverallStats(name, kills, assists, deaths, kdr, enemyFlashes, teamFlashes, utilityDMG);
    }
}

void StatsWindow::addEntryOverallStats(QString name, int kills, int assists, int deaths, float kdr, int ef, int tf, int ud)
{
    pTableOverallStats->insertRows(0, 1, QModelIndex());
    QModelIndex index = pTableOverallStats->index(0, 0, QModelIndex());
    pTableOverallStats->setData(index, name, Qt::EditRole);

    index = pTableOverallStats->index(0, 1, QModelIndex());
    pTableOverallStats->setData(index, kills, Qt::EditRole);

    index = pTableOverallStats->index(0, 2, QModelIndex());
    pTableOverallStats->setData(index, assists, Qt::EditRole);

    index = pTableOverallStats->index(0, 3, QModelIndex());
    pTableOverallStats->setData(index, deaths, Qt::EditRole);

    index = pTableOverallStats->index(0, 4, QModelIndex());
    pTableOverallStats->setData(index, kdr, Qt::EditRole);

    index = pTableOverallStats->index(0, 5, QModelIndex());
    pTableOverallStats->setData(index, ef, Qt::EditRole);

    index = pTableOverallStats->index(0, 6, QModelIndex());
    pTableOverallStats->setData(index, tf, Qt::EditRole);

    index = pTableOverallStats->index(0, 7, QModelIndex());
    pTableOverallStats->setData(index, ud, Qt::EditRole);
}
