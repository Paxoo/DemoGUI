#include "statswindow.h"
#include "ui_statswindow.h"
#include <QBarSet>

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

    // BarChart Model
    pDmgBarStats = new DmgBarTableModel();
    proxyModelDmgBarStats = new QSortFilterProxyModel(this);
    proxyModelDmgBarStats->setSourceModel(pDmgBarStats);

    // use ComboBox to filter proxyModelDmgBarStats
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        proxyModelDmgBarStats->setFilterRegExp(QRegExp(ui->comboBox->itemText(index), Qt::CaseInsensitive,QRegExp::FixedString));
    });
}

StatsWindow::~StatsWindow()
{
    delete ui;
    delete pTableOverallStats;
    delete proxyModelOverallStats;
    delete pDmgBarStats;
    delete proxyModelDmgBarStats;
    delete pBarchart;
    delete pBarchartAxisX;
    delete pBarchartAxisY;
    delete pBarchartSeries;
    delete pBarchartMapper;
    delete pBarchartView;
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
        int total_utilityDMG = 0;

        // sum up all stats over each round
        for (int r=0; r < match->getRounds().size(); r++){
            int round_utilityDMG = 0;
            int round_weaponDMG = 0;
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
                            total_utilityDMG = total_utilityDMG + match->getRounds().at(r)->getListPlayer().at(j)->getDMGdone().value(utility.at(u));
                            round_utilityDMG = round_utilityDMG + match->getRounds().at(r)->getListPlayer().at(j)->getDMGdone().value(utility.at(u));
                        }
                    }

                    QList<QString> weapons = {"P2000","Glock","P250","Deagle","FiveSeven","DualBerettas","Tec9","CZ","USP","Revolver",
                                            "MP7","MP9","Bizon","Mac10","UMP","P90","MP5","SawedOff","Nova","Swag7","XM1014","M249",
                                            "Negev","Galil","Famas","AK47","M4A4","M4A1","Scout","SG556","AUG","AWP","Scar20","G3SG1","Zeus","Knife"};
                    for(int w = 0; w < weapons.size(); w++){
                        if(match->getRounds().at(r)->getListPlayer().at(j)->getDMGdone().contains(weapons.at(w)) == true){
                            round_weaponDMG = round_weaponDMG + match->getRounds().at(r)->getListPlayer().at(j)->getDMGdone().value(weapons.at(w));
                        }
                    }
                    // Bar Chart dmg done during round
                    addEntryDmgBarChart(name, r, round_weaponDMG, round_utilityDMG);

                    // Dont know how to adjust axes so we do this shit
                    if(maxDMG < round_utilityDMG){
                        maxDMG = round_utilityDMG;
                    }
                    if(maxDMG < round_weaponDMG){
                        maxDMG = round_weaponDMG;
                    }
                }
            }
        }
        kdr = (float)kills / (float)deaths;
        // Overall Stats add entry
        addEntryOverallStats(name, kills, assists, deaths, kdr, enemyFlashes, teamFlashes, total_utilityDMG);

        // Fill comboBox with Playernames
        ui->comboBox->addItem(name);
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

    index = pDmgBarStats->index(0, 1, QModelIndex());
    pDmgBarStats->setData(index, r, Qt::EditRole);

    index = pDmgBarStats->index(0, 2, QModelIndex());
    pDmgBarStats->setData(index, w, Qt::EditRole);

    index = pDmgBarStats->index(0, 3, QModelIndex());
    pDmgBarStats->setData(index, u, Qt::EditRole);
}

void StatsWindow::createBarChart()
{
    // Chart
    pBarchart = new QChart();
    pBarchart->setAnimationOptions(QChart::AllAnimations);
    pBarchart->setTitle("Damage done during each round");
    pBarchart->legend()->setVisible(true);
    pBarchart->legend()->setAlignment(Qt::AlignBottom);

    // create chart axes
    QStringList categories;
    for (int i =0; i<proxyModelDmgBarStats->rowCount(); i++){
        categories << QString::number(i+1);
    }
    pBarchartAxisX = new QBarCategoryAxis();
    pBarchartAxisX->append(categories);
    pBarchart->addAxis(pBarchartAxisX, Qt::AlignBottom);
    pBarchartAxisY = new QValueAxis();
    pBarchartAxisY->setRange(0, maxDMG);
    pBarchartAxisY->setTickCount(10);
    pBarchart->addAxis(pBarchartAxisY, Qt::AlignLeft);

    // BarSeries
    pBarchartSeries = new QBarSeries;
    pBarchartSeries->setBarWidth(1);
    pBarchartSeries->attachAxis(pBarchartAxisX);

    // Data Mapper
    pBarchartMapper = new QVBarModelMapper(this);
    pBarchartMapper->setFirstBarSetColumn(2);
    pBarchartMapper->setLastBarSetColumn(3);
    pBarchartMapper->setFirstRow(0);
    pBarchartMapper->setRowCount(proxyModelDmgBarStats->rowCount());
    pBarchartMapper->setSeries(pBarchartSeries);
    pBarchartMapper->setModel(proxyModelDmgBarStats);
    pBarchart->addSeries(pBarchartSeries);
    pBarchartSeries->attachAxis(pBarchartAxisY);

    // Chart View
    pBarchartView = new QChartView(pBarchart);
    pBarchartView->setRenderHint(QPainter::Antialiasing);
    ui->horizontalLayout->addWidget(pBarchartView);
}
