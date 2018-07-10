#include "statisticmodel.h"

StatisticModel::StatisticModel(QObject* parent):
    CustomModel(parent)
{

}

StatisticModel::~StatisticModel(){}

int StatisticModel::rowCount(const QModelIndex &parent) const
{
    return dataList.size();
}

bool StatisticModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row);
    dataList.insert(row, Statistic());
    endInsertRows();
}

bool StatisticModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row);
    dataList.removeAt(row);
    endRemoveRows();
}


bool StatisticModel::remove(const QModelIndex &index)
{
    const Statistic& rec = dataList.at(index.row());
    db->deleteStatistic(rec.match_id, rec.player_id);

    emit changed();

    return removeRows(index.row(), 1);
}


