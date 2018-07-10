#include "custommodel.h"

CustomModel::CustomModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}

void CustomModel::setDB(DataBaseUtil* database){
    this->db = database;
}

CustomModel::~CustomModel()
{
    this->db = nullptr;
}
