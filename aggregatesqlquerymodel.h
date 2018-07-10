#ifndef AGGREGATESQLQUERYMODEL_H
#define AGGREGATESQLQUERYMODEL_H
#include <QSqlQueryModel>

class aggregateSqlQueryModel:public QSqlQueryModel
{
 Q_OBJECT
public:
    aggregateSqlQueryModel(QString query, QObject* parent = 0);

public slots:
    void update();

private:
    QString query;
};

#endif // AGGREGATESQLQUERYMODEL_H
