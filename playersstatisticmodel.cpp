#include "playersstatisticmodel.h"


PlayersStatisticModel::PlayersStatisticModel(int match_id, QObject *parent):
    StatisticModel(parent),
    match_id(match_id)
{

}

void PlayersStatisticModel::update()
{
    while(!dataList.empty()){
        removeRows(0, 1);
    }

    QList<Statistic> temp = db->getAllPlayersStatistic(match_id);
    for(Statistic& stat : temp){
        insertRows(dataList.size(), 1);

        setData(createIndex(dataList.size()-1, 0), stat.playerNumber);
        setData(createIndex(dataList.size()-1, 1), stat.playerName);
        setData(createIndex(dataList.size()-1, 2), stat.goals);
        setData(createIndex(dataList.size()-1, 3), stat.assists);
        setData(createIndex(dataList.size()-1, 4), stat.yellows);
        setData(createIndex(dataList.size()-1, 5), stat.reds);
        setData(createIndex(dataList.size()-1, 6), stat.minutes);

        dataList.rbegin()->player_id = stat.player_id;
        dataList.rbegin()->match_id = stat.match_id;
        dataList.rbegin()->opponentName = stat.opponentName;
        dataList.rbegin()->matchDate = stat.matchDate;
    }

    emit changed();
}

QVariant PlayersStatisticModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role!=Qt::DisplayRole)
        return QVariant();

    if(orientation==Qt::Horizontal){
        switch (section) {
        case Roles::NUMBER:
            return tr("Номер");
            break;
        case Roles::NAME:
            return tr("Имя");
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

int PlayersStatisticModel::columnCount(const QModelIndex &parent) const
{
    return COLUMNS;
}

QVariant PlayersStatisticModel::data(const QModelIndex &index, int role) const
{
    QVariant result;
    if(!index.isValid() || role!=Qt::DisplayRole)
        return result;

    const Statistic& stat = dataList.at(index.row());
    switch (index.column()) {
    case Roles::NUMBER:
        result = stat.playerNumber;
        break;
    case Roles::NAME:
        result = stat.playerName;
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


bool PlayersStatisticModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid()&&role == Qt::EditRole){
        Statistic& rec = dataList[index.row()];
        switch (index.column()) {
        case Roles::NUMBER:
            rec.playerNumber = value.toInt();
            break;
        case Roles::NAME:
            rec.playerName = value.toString();
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

bool PlayersStatisticModel::insert(const Statistic& stat)
{
    db->insertStatistic(stat);

    this->update();

    return true;
}

bool PlayersStatisticModel::remove(const QModelIndex &index)
{
    const Statistic& stat = dataList.at(index.row());

    db->deleteStatistic(stat.match_id, stat.player_id);

    emit changed();

    return removeRows(index.row(), 1);
}

bool PlayersStatisticModel::updateStatistic(const Statistic& stat)
{
    db->updateStatistic(stat);

    this->update();

    return true;
}

const Statistic &PlayersStatisticModel::getPlayer(QModelIndex &index) const
{
    return dataList.at(index.row());
}

QSet<int> PlayersStatisticModel::getPlayers() const
{
    QSet<int> result;

    for(const Statistic& stat : dataList)
        result.insert(stat.player_id);

    return std::move(result);
}
