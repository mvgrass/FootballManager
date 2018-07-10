#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H
#include "custommodel.h"
#include "player.h"

class PlayerModel: public CustomModel
{
 Q_OBJECT
public:

    enum Roles{
        number,
        name,
        position,
        birth,
        height,
        weight
    };

    explicit PlayerModel(QObject *parent = 0);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool insert(const Player&);

    bool remove(const QModelIndex& index);

    bool updatePlayer(const Player&);

    Player& getPlayer(const QModelIndex& index);

    QList<Player>& getPlayers();

public slots:
    void update() override;

private:
    QList<Player> dataList;
    const int COLUMNS = 6;
    const QString POSITIONS[4] = {"GK", "DEF", "MID", "FWR"};
};

#endif // PLAYERMODEL_H
