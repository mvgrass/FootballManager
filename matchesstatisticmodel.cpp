#include "matchesstatisticmodel.h"



MatchesStatisticModel::MatchesStatisticModel(int player_id, QObject* parent):
    StatisticModel(parent),
    player_id(player_id)
{

}

void MatchesStatisticModel::update()
{
    while(!dataList.empty()){
        removeRows(0, 1);
    }

    QList<Statistic> temp = db->getAllMatchesStatistic(player_id);
    for(Statistic& stat : temp){
        insertRows(dataList.size(), 1);

        setData(createIndex(dataList.size()-1, 0), stat.opponentName);
        setData(createIndex(dataList.size()-1, 1), stat.matchDate);
        setData(createIndex(dataList.size()-1, 2), stat.goals);
        setData(createIndex(dataList.size()-1, 3), stat.assists);
        setData(createIndex(dataList.size()-1, 4), stat.yellows);
        setData(createIndex(dataList.size()-1, 5), stat.reds);
        setData(createIndex(dataList.size()-1, 6), stat.minutes);

        dataList.rbegin()->player_id = stat.player_id;
        dataList.rbegin()->match_id = stat.match_id;
        dataList.rbegin()->playerName = stat.playerName;
        dataList.rbegin()->playerNumber = stat.playerNumber;
    }

    emit changed();
}

QVariant MatchesStatisticModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role!=Qt::DisplayRole)
        return QVariant();

    if(orientation==Qt::Horizontal){
        switch (section) {
        case Roles::NAME:
            return tr("Cоперник");
            break;
        case Roles::DATE:
            return tr("Дата");
            break;
        case Roles::GOALS:
            return tr("Голы");
            break;
        case Roles::ASSISTS:
            return tr("Г. Передачи");
            break;
        case Roles::YELLOW:
            return tr("ЖК");
            break;
        case Roles::RED:
            return tr("КК");
            break;
        case Roles::MINUTES:
            return tr("Минуты");
            break;
        }
    }

    return QVariant();
}

int MatchesStatisticModel::columnCount(const QModelIndex &parent) const
{
    return COLUMNS;
}

QVariant MatchesStatisticModel::data(const QModelIndex &index, int role) const
{
    QVariant result;
    if(!index.isValid() || role!=Qt::DisplayRole)
        return result;

    const Statistic& stat = dataList.at(index.row());
    switch (index.column()) {
    case Roles::NAME:
        result = stat.opponentName;
        break;
    case Roles::DATE:
        result = stat.matchDate;
        break;
    case Roles::GOALS:
        result = stat.goals;
        break;
    case Roles::ASSISTS:
        result = stat.assists;
        break;
    case Roles::YELLOW:
        result = stat.yellows;
        break;
    case Roles::RED:
        result = stat.reds;
        break;
    case Roles::MINUTES:
        result = stat.minutes;
        break;
    }

    return result;
}


bool MatchesStatisticModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid()&&role == Qt::EditRole){
        Statistic& rec = dataList[index.row()];
        switch (index.column()) {
        case Roles::NAME:
            rec.opponentName = value.toString();
            break;
        case Roles::DATE:
            rec.matchDate = value.toDate();
            break;
        case Roles::GOALS:
            rec.goals = value.toInt();
            break;
        case Roles::ASSISTS:
            rec.assists = value.toInt();
            break;
        case Roles::YELLOW:
            rec.yellows = value.toInt();
            break;
        case Roles::RED:
            rec.reds = value.toInt();
            break;
        case Roles::MINUTES:
            rec.minutes = value.toInt();
            break;
        default:
            return false;
        }
        return true;
    }

    return false;
}

int MatchesStatisticModel::getGoals() const
{
    int result = 0;
    for(const Statistic& stat : dataList)
        result+=stat.goals;

    return result;
}

int MatchesStatisticModel::getAssists() const
{
    int result = 0;
    for(const Statistic& stat : dataList)
        result+=stat.assists;

    return result;
}

int MatchesStatisticModel::getMinutes() const
{
    int result = 0;
    for(const Statistic& stat : dataList)
        result+=stat.minutes;

    return result;
}

int MatchesStatisticModel::getYellows() const
{
    int result = 0;
    for(const Statistic& stat : dataList)
        result+=stat.yellows;

    return result;
}

int MatchesStatisticModel::getReds() const
{
    int result = 0;
    for(const Statistic& stat : dataList)
        result+=stat.reds;

    return result;
}

