#ifndef PLAYERSSTATISTICMODEL_H
#define PLAYERSSTATISTICMODEL_H
#include "statisticmodel.h"

class PlayersStatisticModel: public StatisticModel
{
Q_OBJECT
public:
    enum Roles{
        NUMBER,
        NAME,
        GOALS,
        ASSISTS,
        YELLOW,
        RED,
        MINUTES
    };

    explicit PlayersStatisticModel(int match_id, QObject* parent = 0);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool insert(const Statistic&);

    bool remove(const QModelIndex& index);

    bool updateStatistic(const Statistic&);

    const Statistic& getPlayer(QModelIndex& index) const;

    QSet<int> getPlayers() const;

public slots:
    void update() override;

private:
    int match_id;
    const int COLUMNS = 7;
};

#endif // PLAYERSSTATISTICMODEL_H
