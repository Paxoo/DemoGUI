#include "dmgbartablemodel.h"

DmgBarTableModel::DmgBarTableModel()
{

}

DmgBarTableModel::DmgBarTableModel(QList<DamageBarChart> dmg, QObject *parent)
    : QAbstractTableModel(parent)
    , mListDMG(dmg)
{

}

int DmgBarTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mListDMG.size();
}

int DmgBarTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4;
}

QVariant DmgBarTableModel::data(const QModelIndex &index, int role) const
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
            return damage.round;
        }else if (index.column() == 2){
            return damage.weaponDMG;
        }else if (index.column() == 3){
            return damage.utilityDMG;
        }
    }
    return QVariant();
}

QVariant DmgBarTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return tr("Name");
            case 1:
                return tr("Round");
            case 2:
                return tr("WeaponDMG");
            case 3:
                return tr("UtilityDMG");
            default:
                return QVariant();
        }
    }
    return QVariant();
}

bool DmgBarTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();

        auto damage = mListDMG.value(row);

        if (index.column() == 0)
            damage.name = value.toString();
        else if (index.column() == 1)
            damage.round = value.toInt();
        else if (index.column() == 2)
            damage.weaponDMG = value.toInt();
        else if (index.column() == 3)
            damage.utilityDMG = value.toInt();
        else
            return false;

        mListDMG.replace(row, damage);
        emit dataChanged(index, index, {role});

        return true;
    }

    return false;
}

bool DmgBarTableModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
        mListDMG.insert(position, { QString(), int(), int(), int() });

    endInsertRows();
    return true;
}

bool DmgBarTableModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
        mListDMG.removeAt(position);

    endRemoveRows();
    return true;
}

QList<DamageBarChart> DmgBarTableModel::getDamageDone() const
{
    return mListDMG;
}

