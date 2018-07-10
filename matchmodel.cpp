#include "matchmodel.h"
#include <QColor>

MatchModel::MatchModel(QObject* parent):
    CustomModel(parent)
{

}

void MatchModel::update()
{
    while(!dataList.empty()){
        removeRows(0, 1);
    }

    QList<Match> temp = db->getAllMatches();
    std::sort(temp.begin(), temp.end(), [](const Match& m1, const Match& m2){return m1.date < m2.date;});
    for(Match& match : temp){
        insertRows(dataList.size(), 1);

        setData(createIndex(dataList.size()-1, 0), match.date);
        setData(createIndex(dataList.size()-1, 1), match.opponent);
        setData(createIndex(dataList.size()-1, 2), match.goals);
        setData(createIndex(dataList.size()-1, 3), match.goalsMissed);

        dataList.rbegin()->id = match.id;
        dataList.rbegin()->result = match.result;
    }

    emit changed();
}

QVariant MatchModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role!=Qt::DisplayRole)
        return QVariant();

    if(orientation==Qt::Horizontal){
        switch (section) {
        case Roles::date:
            return tr("Дата");
            break;
        case Roles::opponent:
            return tr("Соперник");
            break;
        case Roles::goals:
            return tr("Забитые мячи");
            break;
        case Roles::goalsMissed:
            return tr("Пропущенные мячи");
            break;
        }
    }

    return QVariant();
}

int MatchModel::rowCount(const QModelIndex &parent) const
{
    return this->dataList.size();
}

int MatchModel::columnCount(const QModelIndex &parent) const
{
    return this->COLUMNS;
}

QVariant MatchModel::data(const QModelIndex &index, int role) const
{
    QVariant result;

    if(role==Qt::BackgroundColorRole){
        int res = dataList.at(index.row()).result;

        switch (res) {
        case 0://WIN
            return QColor(0, 255, 0, 50);
        case 1://LOSE
            return QColor(255, 0, 0, 50);
        case 2://TIE
            return QColor(255, 255, 0, 50);
        case 3://NOT_PLAYED
            return QColor(255, 255, 255, 50);
            break;
        }
    }

    if(!index.isValid() || role!=Qt::DisplayRole)
        return result;


    const Match& match = dataList.at(index.row());
    switch (index.column()) {
    case Roles::date:
        return match.date;
        break;
    case Roles::opponent:
        return match.opponent;
        break;
    case Roles::goals:
        return match.goals;
        break;
    case Roles::goalsMissed:
        return match.goalsMissed;
        break;
    }

    return result;
}

bool MatchModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row);
    dataList.insert(row, Match(QDate(),""));
    endInsertRows();
}

bool MatchModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row);
    dataList.removeAt(row);
    endRemoveRows();
}

bool MatchModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid()&&role == Qt::EditRole){
        Match& rec = dataList[index.row()];
        switch (index.column()) {
        case Roles::date:
            rec.date = value.toDate();
            return true;
            break;
        case Roles::opponent:
            rec.opponent = value.toString();
            return true;
            break;
        case Roles::goals:
            rec.goals = value.toInt();
            return true;
            break;
        case Roles::goalsMissed:
            rec.goalsMissed = value.toInt();
            break;
        default:
            return false;
        }
        return true;
    }

    return false;

}

bool MatchModel::insert(const Match &match)
{
    db->insertMatch(match);

    this->update();

    return true;
}

bool MatchModel::updateMatch(const Match& match)
{
    db->updateMatch(match);

    this->update();

    return true;
}

bool MatchModel::remove(const QModelIndex& index)
{
    db->deleteMatch(dataList.at(index.row()).id);

    emit changed();

    return removeRows(index.row(), 1);
}

Match &MatchModel::getMatch(const QModelIndex &index)
{
    if(index.isValid())
        return dataList[index.row()];
    else
        throw std::runtime_error("Invalid Index");
}

int MatchModel::getWins()
{
    int result = 0;
    for(Match& match : dataList)
        if(match.result == Match::Result::WIN)
            result++;

    return result;
}

int MatchModel::getTies()
{
    int result = 0;
    for(Match& match : dataList)
        if(match.result == Match::Result::TIE)
            result++;

    return result;
}

int MatchModel::getLoses()
{
    int result = 0;
    for(Match& match : dataList)
        if(match.result == Match::Result::LOSE)
            result++;

    return result;
}
