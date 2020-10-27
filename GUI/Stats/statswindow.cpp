#include "statswindow.h"
#include "ui_statswindow.h"
#include <QBarSet>
#include <QPieLegendMarker>
#include <QtCharts/QPieSlice>

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

    // PieChart Model
    pPiechartTableModel = new DMGPieChartTableModel();
    proxyModelPieChart = new QSortFilterProxyModel(this);
    proxyModelPieChart->setSourceModel(pPiechartTableModel);

    proxyModelPieChartUT = new QSortFilterProxyModel(this);
    proxyModelPieChartUT->setSourceModel(proxyModelPieChart);
    proxyModelPieChartUT->setFilterKeyColumn(1);

    proxyModelPieChartWeapon = new QSortFilterProxyModel(this);
    proxyModelPieChartWeapon->setSourceModel(proxyModelPieChart);
    proxyModelPieChartWeapon->setFilterKeyColumn(1);

    // use ComboBox to filter proxyModelDmgBarStats
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        proxyModelDmgBarStats->setFilterRegExp(QRegExp(ui->comboBox->itemText(index), Qt::CaseInsensitive,QRegExp::FixedString));

        if(pDonutBreakdown != nullptr){
            proxyModelPieChart->setFilterRegExp(QRegExp(ui->comboBox->itemText(index), Qt::CaseInsensitive,QRegExp::FixedString));
            proxyModelPieChartUT->setFilterRegExp(QRegExp("UT", Qt::CaseInsensitive,QRegExp::FixedString));
            pMapperUT->setRowCount(proxyModelPieChartUT->rowCount());
            proxyModelPieChartWeapon->setFilterRegExp(QRegExp("WP", Qt::CaseInsensitive,QRegExp::FixedString));
            pMapperWD->setRowCount(proxyModelPieChartWeapon->rowCount());
            pDonutBreakdown->recalculateAngles({QColor(153,202,83), QColor(32,159,223)});
            pDonutBreakdown->updateLegendMarkers();
        }
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

    delete pPieChart;
    delete pPiechartTableModel;
    delete proxyModelPieChart;
    delete proxyModelPieChartUT;
    delete proxyModelPieChartWeapon;
    delete pSeriesUT;
    delete pMapperUT;
    delete pSeriesWD;
    delete pMapperWD;
    delete pDonutBreakdown;
    delete pPieChartView;
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
        QMap<QString,int> mapDamageUT;
        QMap<QString,int> mapDamageWP;

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

                            if(mapDamageUT.contains(utility.at(u)) == true){
                                qDebug() << mapDamageUT.value(utility.at(u)) << match->getRounds().at(r)->getListPlayer().at(j)->getDMGdone().value(utility.at(u)) << endl;
                                mapDamageUT[utility.at(u)] = mapDamageUT.value(utility.at(u)) + match->getRounds().at(r)->getListPlayer().at(j)->getDMGdone().value(utility.at(u));
                            }else{
                                mapDamageUT.insert(utility.at(u),match->getRounds().at(r)->getListPlayer().at(j)->getDMGdone().value(utility.at(u)));
                            }
                        }
                    }

                    QList<QString> weapons = {"P2000","Glock","P250","Deagle","FiveSeven","DualBerettas","Tec9","CZ","USP","Revolver",
                                            "MP7","MP9","Bizon","Mac10","UMP","P90","MP5","SawedOff","Nova","Swag7","XM1014","M249",
                                            "Negev","Galil","Famas","AK47","M4A4","M4A1","Scout","SG556","AUG","AWP","Scar20","G3SG1","Zeus","Knife"};
                    for(int w = 0; w < weapons.size(); w++){
                        if(match->getRounds().at(r)->getListPlayer().at(j)->getDMGdone().contains(weapons.at(w)) == true){
                            round_weaponDMG = round_weaponDMG + match->getRounds().at(r)->getListPlayer().at(j)->getDMGdone().value(weapons.at(w));

                            if(mapDamageWP.contains(weapons.at(w)) == true){
                                mapDamageWP[weapons.at(w)] = mapDamageWP.value(weapons.at(w)) + match->getRounds().at(r)->getListPlayer().at(j)->getDMGdone().value(weapons.at(w));
                            }else{
                                mapDamageWP.insert(weapons.at(w),match->getRounds().at(r)->getListPlayer().at(j)->getDMGdone().value(weapons.at(w)));
                            }
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
                // end match->getRounds().at(r)->getListPlayer().at(j)->getID()
                }
            // end match->getRounds().at(r)->getListPlayer().size()
            }
        // end match->getRounds().size()
        }
        kdr = (float)kills / (float)deaths;
        // Overall Stats add entry
        addEntryOverallStats(name, kills, assists, deaths, kdr, enemyFlashes, teamFlashes, total_utilityDMG);

        // add Values for PieChart
        QMap<QString, int>::iterator i;
        for (i = mapDamageUT.begin(); i != mapDamageUT.end(); ++i){
            addEntryDmgPieChart(name, "UT", i.key(), i.value());
        }
        for (i = mapDamageWP.begin(); i != mapDamageWP.end(); ++i){
            addEntryDmgPieChart(name, "WP", i.key(), i.value() );
        }

        mapDamageUT.clear();
        mapDamageWP.clear();

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

void StatsWindow::addEntryDmgPieChart(QString name, QString type, QString w, int value)
{
    pPiechartTableModel->insertRows(0, 1, QModelIndex());
    QModelIndex index = pPiechartTableModel->index(0, 0, QModelIndex());
    pPiechartTableModel->setData(index, name, Qt::EditRole);

    index = pPiechartTableModel->index(0, 1, QModelIndex());
    pPiechartTableModel->setData(index, type, Qt::EditRole);

    index = pPiechartTableModel->index(0, 2, QModelIndex());
    pPiechartTableModel->setData(index, w, Qt::EditRole);

    index = pPiechartTableModel->index(0, 3, QModelIndex());
    pPiechartTableModel->setData(index, value, Qt::EditRole);
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

void StatsWindow::createPieChart()
{
    // Series - Data Mapper Utility
    pSeriesUT = new QPieSeries();

    pMapperUT = new QVPieModelMapper(this);
    pMapperUT->setLabelsColumn(2);
    pMapperUT->setValuesColumn(3);
    pMapperUT->setFirstRow(0);
    pMapperUT->setRowCount(proxyModelPieChartUT->rowCount());
    pMapperUT->setSeries(pSeriesUT);
    pMapperUT->setModel(proxyModelPieChartUT);

    // Series - Data Mapper WeaponDMG
    pSeriesWD = new QPieSeries();

    pMapperWD = new QVPieModelMapper(this);
    pMapperWD->setLabelsColumn(2);
    pMapperWD->setValuesColumn(3);
    pMapperWD->setFirstRow(0);
    pMapperWD->setRowCount(proxyModelPieChartWeapon->rowCount());
    pMapperWD->setSeries(pSeriesWD);
    pMapperWD->setModel(proxyModelPieChartWeapon);

    // PieChart
    pDonutBreakdown = new DonutBreakdownChart();
    pDonutBreakdown->setAnimationOptions(QChart::AllAnimations);
    pDonutBreakdown->setTitle("Total damage done");
    pDonutBreakdown->legend()->setAlignment(Qt::AlignRight);
    pDonutBreakdown->addBreakdownSeries(pSeriesUT, QColor(153,202,83));
    pDonutBreakdown->addBreakdownSeries(pSeriesWD, QColor(32,159,223));

    // Chart View
    pPieChartView = new QChartView(pDonutBreakdown);
    pPieChartView->setRenderHint(QPainter::Antialiasing);
    ui->Piepie->addWidget(pPieChartView);

    // update the chart
    proxyModelPieChart->setFilterRegExp(QRegExp(ui->comboBox->itemText(0), Qt::CaseInsensitive,QRegExp::FixedString));
    proxyModelPieChartUT->setFilterRegExp(QRegExp("UT", Qt::CaseInsensitive,QRegExp::FixedString));
    pMapperUT->setRowCount(proxyModelPieChartUT->rowCount());
    proxyModelPieChartWeapon->setFilterRegExp(QRegExp("WP", Qt::CaseInsensitive,QRegExp::FixedString));
    pMapperWD->setRowCount(proxyModelPieChartWeapon->rowCount());
    pDonutBreakdown->recalculateAngles({QColor(153,202,83), QColor(32,159,223)});
    pDonutBreakdown->updateLegendMarkers();
}
