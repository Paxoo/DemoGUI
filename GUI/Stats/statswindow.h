#ifndef STATSWINDOW_H
#define STATSWINDOW_H

#include <QDialog>
#include "overalltablemodel.h"
#include <QSortFilterProxyModel>
#include "GUI\Data\match.h"

namespace Ui {
class StatsWindow;
}

class StatsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StatsWindow(QWidget *parent = nullptr);
    ~StatsWindow();

    void fillTableOverallStats(Match *match);
    void addEntryOverallStats(QString name, int kills, int assists, int deaths, float kdr, int ef, int tf, int ud);

private:
    Ui::StatsWindow *ui;
    OverallTableModel *pTableOverallStats = nullptr;
    QSortFilterProxyModel *proxyModelOverallStats = nullptr;
};

#endif // STATSWINDOW_H
