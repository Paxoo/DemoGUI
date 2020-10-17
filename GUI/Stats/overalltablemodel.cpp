#include "overalltablemodel.h"

OverallTableModel::OverallTableModel()
{

}

OverallTableModel::OverallTableModel(QList<StatsOverall> stats, QObject *parent)
    : QAbstractTableModel(parent)
    , mStatsOverall(stats)
{

}

int OverallTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mStatsOverall.size();
}

int OverallTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 8;
}

QVariant OverallTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= mStatsOverall.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        const auto &player = mStatsOverall.at(index.row());

        if (index.column() == 0){
            return player.name;
        }else if (index.column() == 1){
            return player.kills;
        }else if (index.column() == 2){
            return player.assists;
        }else if (index.column() == 3){
            return player.deaths;
        }else if (index.column() == 4){
            return player.kdr;
        }else if (index.column() == 5){
            return player.enemyFlashes;
        }else if (index.column() == 6){
            return player.mTeamFlashes;
        }else if (index.column() == 7){
            return player.utilityDMG;
        }
    }
    return QVariant();
}

QVariant OverallTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return tr("Name");
            case 1:
                return tr("Kills");
            case 2:
                return tr("Assists");
            case 3:
                return tr("Deaths");
            case 4:
                return tr("KDR");
            case 5:
                return tr("EnemyFlashes");
            case 6:
                return tr("TeamFlashes");
            case 7:
                return tr("UtilityDMG");
            default:
                return QVariant();
        }
    }
    return QVariant();
}

bool OverallTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();

        auto player = mStatsOverall.value(row);

        if (index.column() == 0)
            player.name = value.toString();
        else if (index.column() == 1)
            player.kills = value.toInt();
        else if (index.column() == 2)
            player.assists = value.toInt();
        else if (index.column() == 3)
            player.deaths = value.toInt();
        else if (index.column() == 4)
            player.kdr = value.toFloat();
        else if (index.column() == 5)
            player.enemyFlashes = value.toInt();
        else if (index.column() == 6)
            player.mTeamFlashes = value.toInt();
        else if (index.column() == 7)
            player.utilityDMG = value.toInt();

        else
            return false;

        mStatsOverall.replace(row, player);
        emit dataChanged(index, index, {role});

        return true;
    }

    return false;
}

bool OverallTableModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
        mStatsOverall.insert(position, { QString(), int(), int(), int(), int(), int(), int(), int() });

    endInsertRows();
    return true;
}

bool OverallTableModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
        mStatsOverall.removeAt(position);

    endRemoveRows();
    return true;
}


QList<StatsOverall> OverallTableModel::getStatsOverall() const
{
    return mStatsOverall;
}

