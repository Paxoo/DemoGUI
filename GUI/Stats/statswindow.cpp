#include "statswindow.h"
#include "ui_statswindow.h"
#include <QStackedBarSeries>

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

    // Damage Bar chart
    pDmgBarStats = new DmgBarTableModel();
    proxyModelDmgBarStats = new QSortFilterProxyModel(this);
    proxyModelOverallStats->setSourceModel(pDmgBarStats);

    QChart *chart = new QChart();
    chart->setAnimationOptions(QChart::AllAnimations);
}

StatsWindow::~StatsWindow()
{
    delete ui;
    delete pTableOverallStats;
    delete proxyModelOverallStats;
    delete pDmgBarStats;
    delete proxyModelDmgBarStats;
}

void StatsWindow::fillTableModels(Match *match)
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
        int weaponDMG = 0;

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

                    QList<QString> weapons = {"P2000","Glock","P250","Deagle","FiveSeven","DualBerettas","Tec9","CZ","USP","Revolver",
                                            "MP7","MP9","Bizon","Mac10","UMP","P90","MP5","SawedOff","Nova","Swag7","XM1014","M249",
                                            "Negev","Galil","Famas","AK47","M4A4","M4A1","Scout","SG556","AUG","AWP","Scar20","G3SG1","Zeus","Knife"};
                    for(int u = 0; u < weapons.size(); u++){
                        if(match->getRounds().at(r)->getListPlayer().at(j)->getDMGdone().contains(utility.at(u)) == true){
                            weaponDMG = weaponDMG + match->getRounds().at(r)->getListPlayer().at(j)->getDMGdone().value(utility.at(u));
                        }
                    }
                    // Bar Chart dmg done during round
                    addEntryDmgBarChart(name, r, weaponDMG, utilityDMG);
                }
            }
        }
        kdr = (float)kills / (float)deaths;
        // Overall Stats add entry
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

void StatsWindow::addEntryDmgBarChart(QString name, int r, int w, int u)
{
    pDmgBarStats->insertRows(0, 1, QModelIndex());
    QModelIndex index = pDmgBarStats->index(0, 0, QModelIndex());
    pDmgBarStats->setData(index, name, Qt::EditRole);

    pDmgBarStats->insertRows(0, 1, QModelIndex());
    index = pDmgBarStats->index(0, 1, QModelIndex());
    pDmgBarStats->setData(index, r, Qt::EditRole);

    pDmgBarStats->insertRows(0, 1, QModelIndex());
    index = pDmgBarStats->index(0, 2, QModelIndex());
    pDmgBarStats->setData(index, w, Qt::EditRole);

    pDmgBarStats->insertRows(0, 1, QModelIndex());
    index = pDmgBarStats->index(0, 3, QModelIndex());
    pDmgBarStats->setData(index, u, Qt::EditRole);
}
