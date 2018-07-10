#ifndef STATISTIC_H
#define STATISTIC_H
#include <QString>
#include <QDate>

class Statistic
{
public:
    Statistic(){}
    Statistic(int m_id, int p_id, int g, int a, int m, int y, int r, const QString& opponentName,
              const QDate& matchDate, int playerNumber, const QString& playerName):
        match_id(m_id),
        player_id(p_id),
        goals(g),
        assists(a),
        minutes(m),
        yellows(y),
        reds(r),
        opponentName(opponentName),
        matchDate(matchDate),
        playerNumber(playerNumber),
        playerName(playerName)
    {}
    int match_id;
    int player_id;
    int goals;
    int assists;
    int minutes;
    int yellows;
    int reds;
    QString opponentName;
    QDate matchDate;
    int playerNumber;
    QString playerName;

};

#endif // STATISTIC_H
