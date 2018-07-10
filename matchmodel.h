#ifndef MATCHMODEL_H
#define MATCHMODEL_H
#include "custommodel.h"
#include "match.h"

class MatchModel: public CustomModel
{
 Q_OBJECT
public:

    enum Roles{
        date,
        opponent,
        goals,
        goalsMissed,
    };

    explicit MatchModel(QObject *parent = 0);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool insert(const Match&);

    bool updateMatch(const Match&);

    bool remove(const QModelIndex& index);

    Match& getMatch(const QModelIndex& index);

    int getWins();

    int getTies();

    int getLoses();

public slots:
    void update() override;

private:
    QList<Match> dataList;
    const int COLUMNS = 4;
};

#endif // MATCHMODEL_H
