#ifndef UTIL_QQUERY2TABLEMODEL_H
#define UTIL_QQUERY2TABLEMODEL_H

#include <QMap>
#include <QtSql>
#include <QAbstractItemModel>

class QQuery2TableModel:public QAbstractItemModel
{
    Q_OBJECT
public:
    QQuery2TableModel(QSqlRecord pSourceQuery,int iStartPos,int iEndPos,int iSplitSize,QObject *parent = 0);
    virtual ~QQuery2TableModel();

    virtual QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;
    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value,
                               int role = Qt::EditRole);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    QSqlRecord& record();
private:
    QSqlRecord sourceQuery;
    int startPos;
    int endPos;
    int splitSize;
    QMap<int,QVariant> verticalHeaderData;
    QMap<int,QVariant> horizontalHeaderData;
};


#endif // UTIL_QQUERY2TABLEMODEL_H
