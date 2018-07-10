#include <QDateTime>
#include "playermodel.h"

PlayerModel::PlayerModel(QObject* parent):
    CustomModel(parent)
{

}

void PlayerModel::update()
{
    while(!dataList.empty()){
        removeRows(0, 1);
    }

    QList<Player> temp = db->getAllPlayers();
    for(Player& player : temp){
        insertRows(dataList.size(), 1);

        setData(createIndex(dataList.size()-1, 0), player.getNumber());
        setData(createIndex(dataList.size()-1, 1), QString(player.getName().c_str()));
        setData(createIndex(dataList.size()-1, 2), player.getPos());
        setData(createIndex(dataList.size()-1, 3), player.getBirth());
        setData(createIndex(dataList.size()-1, 4), player.getHeight());
        setData(createIndex(dataList.size()-1, 5), player.getWeight());

        dataList.rbegin()->setId(player.getId());
    }

    emit changed();
}

QVariant PlayerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role!=Qt::DisplayRole)
        return QVariant();

    if(orientation==Qt::Horizontal){
        switch (section) {
        case Roles::number:
            return tr("Номер");
            break;
        case Roles::name:
            return tr("Имя");
            break;
        case Roles::birth:
            return tr("Год Рождения");
            break;
        case Roles::weight:
            return tr("Вес");
            break;
        case Roles::height:
            return tr("Рост");
            break;
        case Roles::position:
            return tr("Позиция");
            break;
        }
    }

    return QVariant();
}

int PlayerModel::rowCount(const QModelIndex &parent) const
{
    return this->dataList.size();
}

int PlayerModel::columnCount(const QModelIndex &parent) const
{
    return this->COLUMNS;
}

QVariant PlayerModel::data(const QModelIndex &index, int role) const
{
    QVariant result;
    if(!index.isValid() || role!=Qt::DisplayRole)
        return result;

    const Player& player = dataList.at(index.row());
    switch (index.column()) {
    case Roles::number:
        result = player.getNumber();
        break;
    case Roles::name:
        result = QString(player.getName().c_str());
        break;
    case Roles::birth:
        result = player.getBirth();
        break;
    case Roles::weight:
        result = player.getWeight();
        break;
    case Roles::height:
        result = player.getHeight();
        break;
    case Roles::position:
        result = POSITIONS[player.getPos()];
        break;
    }

    return result;
}

bool PlayerModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row);
    dataList.insert(row, Player(0,"", 0, 0, 0, 0));
    endInsertRows();
}

bool PlayerModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row);
    dataList.removeAt(row);
    endRemoveRows();
}

bool PlayerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid()&&role == Qt::EditRole){
        Player& rec = dataList[index.row()];
        switch (index.column()) {
        case Roles::number:
            rec.setNumber(value.toInt());
            break;
        case Roles::name:
            rec.setName(value.toString().toUtf8().constData());
            break;
        case Roles::birth:
            rec.setBirth(value.toInt());
            break;
        case Roles::weight:
            rec.setWeight(value.toInt());
            break;
        case Roles::height:
            rec.setHeight(value.toInt());
            break;
        case Roles::position:
            rec.setPos(value.toInt());
            break;
        default:
            return false;
        }
        return true;
    }

    return false;

}

bool PlayerModel::insert(const Player &player)
{
    db->insertPlayer(player);

    this->update();

    return true;

}

bool PlayerModel::remove(const QModelIndex& index)
{
    db->deletePlayer(dataList.at(index.row()).getId());

    emit changed();

    return removeRows(index.row(), 1);
}

bool PlayerModel::updatePlayer(const Player &player)
{
    db->updatePlayer(player);

    this->update();

    return true;
}

Player& PlayerModel::getPlayer(const QModelIndex &index)
{
    if(index.isValid())
        return dataList[index.row()];
    else
        throw std::runtime_error("Invalid Index");
}

QList<Player> &PlayerModel::getPlayers()
{
    return this->dataList;
}

