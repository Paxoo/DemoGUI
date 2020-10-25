#ifndef DMGPIECHARTTABLEMODEL_H
#define DMGPIECHARTTABLEMODEL_H

#include <QAbstractTableModel>


struct DamagePieChart
{
    QString name;
    QString type;
    QString weapon;
    int dmgValue;
};

class DMGPieChartTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    DMGPieChartTableModel();
    DMGPieChartTableModel(QList<DamagePieChart> dmg, QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;

    QList<DamagePieChart> getDamageList() const;

private:
    QList<DamagePieChart> mListDMG;
};

#endif // DMGPIECHARTTABLEMODEL_H
