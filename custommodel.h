#ifndef CUSTOMMODEL_H
#define CUSTOMMODEL_H
#include <QAbstractTableModel>
#include <QHash>
#include "databaseutil.h"

class CustomModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CustomModel(QObject *parent = 0);

    virtual ~CustomModel();

    void setDB(DataBaseUtil*);

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override = 0;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override = 0;

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override = 0;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override = 0;

    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override = 0;

    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override = 0;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override = 0;

signals:
    void changed();

public slots:
    virtual void update() = 0;

protected:
    DataBaseUtil* db;

};

#endif // CUSTOMMODEL_H
