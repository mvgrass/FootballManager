#ifndef DATABASEUTIL_H
#define DATABASEUTIL_H

#include <QObject>
#include <QSqlDatabase>
#include <QList>

class Player;
class Match;
class Statistic;

class DataBaseUtil : public QObject
{
    Q_OBJECT
public:
    explicit DataBaseUtil(QObject *parent = nullptr);

    virtual ~DataBaseUtil();

    void connectToDB(const QString& db_name);

    void initDB();

    QList<Player> getAllPlayers() const;

    QList<Match> getAllMatches() const;

    QList<Statistic> getAllMatchesStatistic(int player_id) const;

    QList<Statistic> getAllPlayersStatistic(int match_id) const;

    void deletePlayer(int id);

    void deleteMatch(int id);

    void deleteStatistic(int match_id, int player_id);

    void insertPlayer(const Player&);

    void insertMatch(const Match&);

    void insertStatistic(const Statistic&);

    void updatePlayer(const Player&);

    void updateMatch(const Match&);

    void updateStatistic(const Statistic&);

private:
    QSqlDatabase db;
    const QString PLAYERS_TABLE = "players";
    const QString MATCH_TABLE = "matches";
    const QString STATISTIC_TABLE = "statistic";
};

#endif // DATABASEUTIL_H
