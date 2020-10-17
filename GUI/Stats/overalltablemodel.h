#ifndef OVERALLTABLEMODEL_H
#define OVERALLTABLEMODEL_H

#include <QAbstractTableModel>

struct StatsOverall
{
    QString name;
    int kills;
    int assists;
    int deaths;
    float kdr;
    int enemyFlashes;
    int mTeamFlashes;
    int utilityDMG;

    bool operator==(const StatsOverall &other) const
    {
        return name == other.name;
    }
};

class OverallTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    OverallTableModel();
    OverallTableModel(QList<StatsOverall> players, QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    QList<StatsOverall> getStatsOverall() const;

private:
    QList<StatsOverall> mStatsOverall;
};

#endif // OVERALLTABLEMODEL_H
