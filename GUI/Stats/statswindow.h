#ifndef STATSWINDOW_H
#define STATSWINDOW_H

#include <QDialog>
#include "overalltablemodel.h"
#include "dmgbartablemodel.h"
#include <QSortFilterProxyModel>
#include "GUI\Data\match.h"
#include <QChart>
#include <QComboBox>
#include <QValueAxis>
#include <QBarCategoryAxis>
#include <QVBarModelMapper>
#include <QBarSeries>
#include <QChartView>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class StatsWindow;
}

class StatsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StatsWindow(QWidget *parent = nullptr);
    ~StatsWindow();
    void fillTableModels(Match *match);
    void createBarChart();

private:
    Ui::StatsWindow *ui;
    OverallTableModel *pTableOverallStats = nullptr;
    QSortFilterProxyModel *proxyModelOverallStats = nullptr;
    DmgBarTableModel *pDmgBarStats = nullptr;
    QSortFilterProxyModel *proxyModelDmgBarStats = nullptr;
    QChart *pBarchart = nullptr;
    QBarCategoryAxis *pBarchartAxisX = nullptr;
    QValueAxis *pBarchartAxisY = nullptr;
    QBarSeries *pBarchartSeries = nullptr;
    QVBarModelMapper *pBarchartMapper = nullptr;
    QChartView *pBarchartView = nullptr;

    int maxDMG = 0;
    void addEntryOverallStats(QString name, int kills, int assists, int deaths, float kdr, int ef, int tf, int ud);
    void addEntryDmgBarChart(QString name, int r, int w, int u);
};

#endif // STATSWINDOW_H
