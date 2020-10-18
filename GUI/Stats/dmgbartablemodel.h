#ifndef DMGDONESTATS_H
#define DMGDONESTATS_H

#include <QAbstractTableModel>


struct DamageBarChart
{
    QString name;
    int round;
    int weaponDMG;
    int utilityDMG;

    bool operator==(const DamageBarChart &other) const
    {
        return name == other.name;
    }
};

class DmgBarTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    DmgBarTableModel();
    DmgBarTableModel(QList<DamageBarChart> dmg, QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    QList<DamageBarChart> getDamageDone() const;

private:
    QList<DamageBarChart> mListDMG;
};

#endif // DMGDONESTATS_H
