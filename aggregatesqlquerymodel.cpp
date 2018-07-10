#include "aggregatesqlquerymodel.h"
aggregateSqlQueryModel::aggregateSqlQueryModel(QString query, QObject *parent):
    QSqlQueryModel(parent),
    query(query)
{

}

void aggregateSqlQueryModel::update()
{
    this->setQuery(query);
}

