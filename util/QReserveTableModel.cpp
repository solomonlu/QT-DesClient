#include "QReserveTableModel.h"

QReserveTableModel::QReserveTableModel(QAbstractItemModel *pSourceTableModel,QObject *parent)
    :QAbstractItemModel(parent),sourceTableModel(pSourceTableModel)
{

}

QReserveTableModel::~QReserveTableModel()
{

}

QModelIndex QReserveTableModel::index(int row, int column,const QModelIndex &) const
{
    return this->createIndex(row,column,0);
}

QModelIndex QReserveTableModel::parent(const QModelIndex &child) const
{
    return sourceTableModel->parent(child);
}

int QReserveTableModel::rowCount(const QModelIndex &) const
{
    return sourceTableModel->columnCount();
}

int QReserveTableModel::columnCount(const QModelIndex &) const
{
    return sourceTableModel->rowCount();
}

QVariant QReserveTableModel::data(const QModelIndex &index, int role) const
{
    QModelIndex newIndex = sourceTableModel->index(index.column(),index.row());
    return sourceTableModel->data(newIndex,role);
}

bool QReserveTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QModelIndex newIndex = sourceTableModel->index(index.column(),index.row());
    return sourceTableModel->setData(newIndex,value,role);
}

Qt::ItemFlags QReserveTableModel::flags(const QModelIndex &) const
{
    return Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled;
}

QVariant QReserveTableModel::headerData(int section, Qt::Orientation orientation,int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Vertical)
        {
            return verticalHeaderData[section];
        }
        else
        {
            return horizontalHeaderData[section];
        }
    }
    else
    {
        return sourceTableModel->headerData(section,orientation,role);
    }
}

bool QReserveTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value,
                           int role)
{
    if (role == Qt::EditRole)
    {
        if (orientation == Qt::Vertical)
        {
            verticalHeaderData[section] = value;
        }
        else
        {
            horizontalHeaderData[section] = value;
        }

        return true;
    }

    return false;
}

