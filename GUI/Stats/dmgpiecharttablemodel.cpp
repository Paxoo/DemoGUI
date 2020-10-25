#include "dmgpiecharttablemodel.h"

DMGPieChartTableModel::DMGPieChartTableModel()
{

}

DMGPieChartTableModel::DMGPieChartTableModel(QList<DamagePieChart> dmg, QObject *parent)
    : QAbstractTableModel(parent)
  , mListDMG(dmg)
{

}

int DMGPieChartTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mListDMG.size();
}

int DMGPieChartTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4;
}

QVariant DMGPieChartTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= mListDMG.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        const auto &damage = mListDMG.at(index.row());

        if (index.column() == 0){
            return damage.name;
        }else if (index.column() == 1){
            return damage.type;
        }else if (index.column() == 2){
            return damage.weapon;
        }else if (index.column() == 3){
            return damage.dmgValue;
        }
    }
    return QVariant();
}

bool DMGPieChartTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();

        auto damage = mListDMG.value(row);

        if (index.column() == 0)
            damage.name = value.toString();
        else if (index.column() == 1)
            damage.type = value.toString();
        else if (index.column() == 2)
            damage.weapon = value.toString();
        else if (index.column() == 3)
            damage.dmgValue = value.toInt();
        else
            return false;

        mListDMG.replace(row, damage);
        emit dataChanged(index, index, {role});

        return true;
    }

    return false;
}

bool DMGPieChartTableModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
        mListDMG.insert(position, { QString(), QString(), QString(), int() });

    endInsertRows();
    return true;
}

bool DMGPieChartTableModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
        mListDMG.removeAt(position);

    endRemoveRows();
    return true;
}

QList<DamagePieChart> DMGPieChartTableModel::getDamageList() const
{
    return mListDMG;
}
