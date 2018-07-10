#ifndef MATCH_H
#define MATCH_H
#include <QDate>

class Match
{
public:
    enum Result{WIN, LOSE, TIE, NOT_PLAYED};
    Match(const QDate& date, const QString& opponent, int result = NOT_PLAYED, int goals = 0, int goalsMissed = 0, int id = -1)
        :date(date),
         opponent(opponent),
         result((Result)result),
         goals(goals),
         goalsMissed(goalsMissed),
         id(id)
    {

    }

    int id;
    Result result;
    QString opponent;
    QDate date;
    int goals;
    int goalsMissed ;
};

#endif // MATCH_H
