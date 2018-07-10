#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QDate>
#include "databaseutil.h"
#include "player.h"
#include "match.h"
#include "statistic.h"
#include <QSqlError>

DataBaseUtil::DataBaseUtil(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
}

void DataBaseUtil::connectToDB(const QString& db_name)
{
    if(db.isOpen())
        db.close();
    db.setDatabaseName(db_name);

    if(!db.open())
        throw std::runtime_error("Ошибка подключения к базе данных");
}


void DataBaseUtil::initDB()
{
    QSqlQuery query;
    query.prepare(QString("CREATE TABLE %1 (id INTEGER PRIMARY KEY AUTOINCREMENT, number INTEGER UNIQUE, name TEXT, position INTEGER, birthYear INTEGER,"
                          " height INTEGER, weight INTEGER);").arg(PLAYERS_TABLE));

    if(!query.exec())
        throw std::runtime_error("Ошибка инициализации базы данных");



    query.prepare(QString("CREATE TABLE %1 (id INTEGER PRIMARY KEY AUTOINCREMENT, opponent TEXT, date TEXT, result INTEGER,"
                          " goals INTEGER, goalsMissed INTEGER);").arg(MATCH_TABLE));

    if(!query.exec())
        throw std::runtime_error("Ошибка инициализации базы данных");


    query.prepare(QString("CREATE TABLE %1 (match_id INTEGER, player_id INTEGER, goals INTEGER, assists INTEGER,"
                          " minutes INTEGER, yellow INTEGER, red INTEGER, CONSTRAINT new_pk PRIMARY KEY(match_id, player_id));")
                            .arg(STATISTIC_TABLE));

    if(!query.exec())
        throw std::runtime_error("Ошибка инициализации базы данных");
}


void DataBaseUtil::insertPlayer(const Player & player)
{
    QSqlQuery query;

    query.prepare(QString("INSERT INTO %1 (number, name, position, birthYear, height, weight)"
                                   "VALUES (%2, \"%3\", %4, %5, %6, %7);").arg(PLAYERS_TABLE,
                                                                     QString::number(player.getNumber()),
                                                                     QString(player.getName().c_str()),
                                                                     QString::number(player.getPos()),
                                                                     QString::number(player.getBirth()),
                                                                     QString::number(player.getHeight()),
                                                                     QString::number(player.getWeight())));
    if(!query.exec())
        throw std::runtime_error("Не удалось вставить элемент");
}

void DataBaseUtil::insertMatch(const Match & match)
{
    QSqlQuery query;

    query.prepare(QString("INSERT INTO %1 (date, opponent, result, goals, goalsMissed) "
                          "VALUES (\"%2\", \"%3\", %4, %5, %6);").arg(MATCH_TABLE,
                                              match.date.toString("dd.MM.yyyy"),
                                              match.opponent,
                                              QString::number(match.result),
                                              QString::number(match.goals),
                                              QString::number(match.goalsMissed)));

    if(!query.exec())
        throw std::runtime_error("Не удалось вставить элемент");

}

void DataBaseUtil::insertStatistic(const Statistic &stat)
{
    QSqlQuery query;

    query.prepare(QString("INSERT INTO %1 (match_id, player_id, goals, assists, minutes, "
                          "yellow, red) "
                          "VALUES (%2, %3, %4, %5, %6, %7, %8);").arg(STATISTIC_TABLE,
                                              QString::number(stat.match_id),
                                              QString::number(stat.player_id),
                                              QString::number(stat.goals),
                                              QString::number(stat.assists),
                                              QString::number(stat.minutes),
                                              QString::number(stat.yellows),
                                              QString::number(stat.reds)));

    if(!query.exec())
        throw std::runtime_error("Не удалось вставить элемент");
}

void DataBaseUtil::updatePlayer(const Player& player)
{
    QSqlQuery query;
    query.prepare(QString("UPDATE %1 SET number = %2, name = \"%3\", position = %4,"
                          " birthYear = %5, height = %6, weight = %7 "
                                   "WHERE id = %8;").arg(PLAYERS_TABLE,
                                                                     QString::number(player.getNumber()),
                                                                     QString(player.getName().c_str()),
                                                                     QString::number(player.getPos()),
                                                                     QString::number(player.getBirth()),
                                                                     QString::number(player.getHeight()),
                                                                     QString::number(player.getWeight()),
                                                                     QString::number(player.getId())));


    if(!query.exec())
        throw std::runtime_error("Не удалось изменить элемент");
}

void DataBaseUtil::updateMatch(const Match & match)
{
    QSqlQuery query;

    query.prepare(QString("UPDATE %1 SET date = \"%2\", opponent = \"%3\", result = %4,"
                          " goals = %5, goalsMissed = %6 "
                          "WHERE id = %7;").arg(MATCH_TABLE,
                                              match.date.toString("dd.MM.yyyy"),
                                              match.opponent,
                                              QString::number(match.result),
                                              QString::number(match.goals),
                                              QString::number(match.goalsMissed),
                                              QString::number(match.id)));

    if(!query.exec())
        throw std::runtime_error("Не удалось обновить элемент");
}

void DataBaseUtil::updateStatistic(const Statistic & stat)
{
    QSqlQuery query;

    query.prepare(QString("UPDATE %1 SET goals = %2, assists = %3, minutes = %4, "
                          "yellow = %5, red = %6 "
                          "WHERE match_id = %7 and player_id = %8;").arg(STATISTIC_TABLE,
                                              QString::number(stat.goals),
                                              QString::number(stat.assists),
                                              QString::number(stat.minutes),
                                              QString::number(stat.yellows),
                                              QString::number(stat.reds),
                                              QString::number(stat.match_id),
                                              QString::number(stat.player_id)));

    QString str = QString("UPDATE %1 SET goals = %2, assists = %3, minutes = %4, "
                          "yellow = %5, red = %6 "
                          "WHERE match_id = %7 and player_id = %8;").arg(STATISTIC_TABLE,
                                              QString::number(stat.goals),
                                              QString::number(stat.assists),
                                              QString::number(stat.minutes),
                                              QString::number(stat.yellows),
                                              QString::number(stat.reds),
                                              QString::number(stat.match_id),
                                              QString::number(stat.player_id));

    if(!query.exec()){
        throw std::runtime_error("Не удалось обновить элемент");

    }
}


QList<Player> DataBaseUtil::getAllPlayers() const
{
    QSqlQuery query;
    query.prepare(QString("SELECT * from %1 ORDER BY number;").arg(PLAYERS_TABLE));
    query.exec();

    QList<Player> result;


    QSqlRecord rec;

    if(query.first()){
        rec = query.record();
        result.append(Player(rec.value("number").toInt(),
                             rec.value("name").toString().toUtf8().constData(),
                             rec.value("birthYear").toInt(),
                             rec.value("weight").toInt(),
                             rec.value("height").toInt(),
                             rec.value("position").toInt(),
                             rec.value("id").toInt()));
    }

    while(query.next()){
        int number = query.value(rec.indexOf("number")).toInt();
        QString name = query.value(rec.indexOf("name")).toString();
        int birth = query.value(rec.indexOf("birthYear")).toInt();
        int weight = query.value(rec.indexOf("weight")).toInt();
        int height = query.value(rec.indexOf("height")).toInt();
        int pos = query.value(rec.indexOf("position")).toInt();
        int id = query.value(rec.indexOf("id")).toInt();
        result.append(Player(number, name.toUtf8().constData(), birth, weight, height, pos, id));
    }

    return std::move(result);
}

QList<Match> DataBaseUtil::getAllMatches() const
{
    QSqlQuery query;
    query.prepare(QString("SELECT * from %1;").arg(MATCH_TABLE));
    query.exec();

    QList<Match> result;

    QSqlRecord rec;

    if(query.first()){
        rec = query.record();
        result.append(Match(QDate::fromString(rec.value("date").toString(),"dd.MM.yyyy"),
                             rec.value("opponent").toString(),
                             rec.value("result").toInt(),
                             rec.value("goals").toInt(),
                             rec.value("goalsMissed").toInt(),
                             rec.value("id").toInt()));

    }

    while(query.next()){
        QDate date = QDate::fromString(query.value(rec.indexOf("date")).toString(), "dd.MM.yyyy");
        QString opponent = query.value(rec.indexOf("opponent")).toString();
        int matchResult = query.value(rec.indexOf("result")).toInt();
        int goals = query.value(rec.indexOf("goals")).toInt();
        int goalsMissed = query.value(rec.indexOf("goalsMissed")).toInt();
        int id = query.value(rec.indexOf("id")).toInt();
        result.append(Match(date, opponent, matchResult, goals, goalsMissed, id));
    }

    return std::move(result);
}

QList<Statistic> DataBaseUtil::getAllMatchesStatistic(int player_id) const
{
    QSqlQuery query;
    query.prepare(QString("SELECT match_id, %1.goals as goals, assists, yellow, red, minutes, number, opponent, date, name"
                 " FROM %1 JOIN %2 ON %2.id = player_id"
                 " JOIN %3 ON %3.id = match_id WHERE player_id = %4 ORDER BY date;").arg(STATISTIC_TABLE,
                                                      PLAYERS_TABLE,
                                                      MATCH_TABLE,
                                                      QString::number(player_id)));
    query.exec();

    QList<Statistic> result;

    QSqlRecord rec;

    if(query.first()){
        rec = query.record();
        result.append(Statistic(rec.value("match_id").toInt(),
                                player_id,
                                rec.value("goals").toInt(),
                                rec.value("assists").toInt(),
                                rec.value("minutes").toInt(),
                                rec.value("yellow").toInt(),
                                rec.value("red").toInt(),
                                rec.value("opponent").toString(),
                                QDate::fromString(rec.value("date").toString(), "dd.MM.yyyy"),
                                rec.value("number").toInt(),
                                rec.value("name").toString()));

    }

    while(query.next()){
        int match_id = query.value(rec.indexOf("match_id")).toInt();
        int goals = query.value(rec.indexOf("goals")).toInt();
        int assists = query.value(rec.indexOf("assists")).toInt();
        int minutes = query.value(rec.indexOf("minutes")).toInt();
        int yellow = query.value(rec.indexOf("yellow")).toInt();
        int red = query.value(rec.indexOf("red")).toInt();
        QDate date = QDate::fromString(query.value(rec.indexOf("date")).toString(), "dd.MM.yyyy");
        QString opponent = query.value(rec.indexOf("opponent")).toString();
        int number =  query.value(rec.indexOf("number")).toInt();
        QString name = query.value(rec.indexOf("name")).toString();

        result.append(Statistic(match_id,
                                player_id,
                                goals,
                                assists,
                                minutes,
                                yellow,
                                red,
                                opponent,
                                date,
                                number,
                                name));
    }


    return std::move(result);
}

QList<Statistic> DataBaseUtil::getAllPlayersStatistic(int match_id) const
{
    QSqlQuery query;
    query.prepare(QString("SELECT player_id, %1.goals as goals, assists, yellow, red, minutes, number, opponent, date, name"
                 " FROM %1 JOIN %2 ON %2.id = player_id"
                 " JOIN %3 ON %3.id = match_id WHERE match_id = %4 ORDER BY number;").arg(STATISTIC_TABLE,
                                                      PLAYERS_TABLE,
                                                      MATCH_TABLE,
                                                      QString::number(match_id)));
    query.exec();

    QList<Statistic> result;

    QSqlRecord rec;

    if(query.first()){
        rec = query.record();
        result.append(Statistic(match_id,
                                rec.value("player_id").toInt(),
                                rec.value("goals").toInt(),
                                rec.value("assists").toInt(),
                                rec.value("minutes").toInt(),
                                rec.value("yellow").toInt(),
                                rec.value("red").toInt(),
                                rec.value("opponent").toString(),
                                QDate::fromString(rec.value("date").toString(), "dd.MM.yyyy"),
                                rec.value("number").toInt(),
                                rec.value("name").toString()));

    }

    while(query.next()){
        int player_id = query.value(rec.indexOf("player_id")).toInt();
        int goals = query.value(rec.indexOf("goals")).toInt();
        int assists = query.value(rec.indexOf("assists")).toInt();
        int minutes = query.value(rec.indexOf("minutes")).toInt();
        int yellow = query.value(rec.indexOf("yellow")).toInt();
        int red = query.value(rec.indexOf("red")).toInt();
        QDate date = QDate::fromString(query.value(rec.indexOf("date")).toString(), "dd.MM.yyyy");
        QString opponent = query.value(rec.indexOf("opponent")).toString();
        int number =  query.value(rec.indexOf("number")).toInt();
        QString name = query.value(rec.indexOf("name")).toString();

        result.append(Statistic(match_id,
                                player_id,
                                goals,
                                assists,
                                minutes,
                                yellow,
                                red,
                                opponent,
                                date,
                                number,
                                name));
    }


    return std::move(result);
}

void DataBaseUtil::deletePlayer(int id)
{
    QSqlQuery query;
    query.prepare(QString("DELETE FROM %1 WHERE id = %2;").arg(PLAYERS_TABLE,
                                                                     QString::number(id)));
    if(!query.exec())
        throw std::runtime_error("Не удалось удалить элемент");

    query.prepare(QString("DELETE FROM %1 WHERE player_id = %2;").arg(STATISTIC_TABLE,
                                                                     QString::number(id)));
    if(!query.exec())
        throw std::runtime_error("Не удалось удалить статистику игрока");

}

void DataBaseUtil::deleteMatch(int id)
{
    QSqlQuery query;
    query.prepare(QString("DELETE FROM %1 WHERE id = %2;").arg(MATCH_TABLE,
                                                                     QString::number(id)));
    if(!query.exec())
        throw std::runtime_error("Не удалось удалить элемент");

    query.prepare(QString("DELETE FROM %1 WHERE match_id = %2;").arg(STATISTIC_TABLE,
                                                                     QString::number(id)));
    if(!query.exec())
        throw std::runtime_error("Не удалось удалить статистику матча");
}

void DataBaseUtil::deleteStatistic(int match_id, int player_id)
{
    QSqlQuery query;
    query.prepare(QString("DELETE FROM %1 WHERE match_id = %2 AND player_id = %3;").arg(STATISTIC_TABLE,
                                                                     QString::number(match_id),
                                                                     QString::number(player_id)));

    if(!query.exec())
        throw std::runtime_error("Не удалось удалить статистику матча");
}


DataBaseUtil::~DataBaseUtil()
{
    if(db.isOpen())
        db.close();
}
