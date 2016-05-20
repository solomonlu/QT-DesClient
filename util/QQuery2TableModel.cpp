#include "QQuery2TableModel.h"

QQuery2TableModel::QQuery2TableModel(QSqlRecord pSourceQuery,int iStartPos,int iEndPos,int iSplitSize,QObject *parent)
    :QAbstractItemModel(parent),sourceQuery(pSourceQuery),startPos(iStartPos),endPos(iEndPos),splitSize(iSplitSize)
{

}

QQuery2TableModel::~QQuery2TableModel()
{

}

QModelIndex QQuery2TableModel::index(int row, int column,const QModelIndex &) const
{
    return createIndex(row,column);
}

QModelIndex QQuery2TableModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int QQuery2TableModel::rowCount(const QModelIndex &) const
{
    return splitSize;
}

int QQuery2TableModel::columnCount(const QModelIndex &) const
{
    if (endPos == -1)
    {
        return (sourceQuery.count() - startPos) / splitSize;
    }
    else
    {
        return (endPos - startPos) / splitSize;
    }
}

QVariant QQuery2TableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::TextAlignmentRole)
    {
        return int(Qt::AlignHCenter|Qt::AlignVCenter);
    }
    else if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        return sourceQuery.value(index.column()*splitSize + index.row() + startPos);
    }
    else
    {
        return QVariant();
    }
}

bool QQuery2TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        sourceQuery.setValue(index.column()*splitSize + index.row() + startPos,value);
        return true;
    }

    return false;
}

QVariant QQuery2TableModel::headerData(int section, Qt::Orientation orientation,int role) const
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
        return QAbstractItemModel::headerData(section,orientation,role);
    }
}

bool QQuery2TableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value,int role)
{
    if (role == Qt::EditRole)
    {
        if(orientation == Qt::Vertical)
        {
            verticalHeaderData[section] = value;
        }
        else
        {
            horizontalHeaderData[section] = value;
        }
    }

    return QAbstractItemModel::setHeaderData(section,orientation,value,role);
}

Qt::ItemFlags QQuery2TableModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QSqlRecord& QQuery2TableModel::record()
{
    return sourceQuery;
}


