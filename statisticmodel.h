#ifndef STATISTICMODEL_H
#define STATISTICMODEL_H
#include "custommodel.h"
#include "statistic.h"

class StatisticModel: public CustomModel
{
    Q_OBJECT
public:
    explicit StatisticModel(QObject *parent = 0);

    virtual ~StatisticModel();

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override = 0;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override = 0;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const = 0;

    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    virtual bool remove(const QModelIndex& index);

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override = 0;


public slots:
    void update() override = 0;

protected:
    QList<Statistic> dataList;

};
#endif // STATISTICMODEL_H
