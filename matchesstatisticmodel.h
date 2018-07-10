#ifndef MATCHESSTATISTICMODEL_H
#define MATCHESSTATISTICMODEL_H
#include "statisticmodel.h"


class MatchesStatisticModel:public StatisticModel
{
 Q_OBJECT
public:
    enum Roles{
        NAME,
        DATE,
        GOALS,
        ASSISTS,
        YELLOW,
        RED,
        MINUTES
    };

    explicit MatchesStatisticModel(int player_id, QObject* parent = 0);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    int getGoals() const;

    int getAssists()const;

    int getMinutes()const;

    int getYellows()const;

    int getReds()const;

public slots:
    void update() override;

private:
    const int COLUMNS = 7;
    int player_id;
};

#endif // MATCHESSTATISTICMODEL_H
