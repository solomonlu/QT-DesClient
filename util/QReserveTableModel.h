#ifndef UTIL_QRESERVETABLEMODEL_H
#define UTIL_QRESERVETABLEMODEL_H

#include <QMap>
#include <QAbstractItemModel>

class QReserveTableModel:public QAbstractItemModel
{
    Q_OBJECT
public:
    QReserveTableModel(QAbstractItemModel *pSourceTableModel,QObject *parent = 0);
    virtual ~QReserveTableModel();

    virtual QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;
    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value,
                               int role = Qt::EditRole);

    QAbstractItemModel* GetOriginalModel() {return sourceTableModel;}
private:
    QAbstractItemModel *sourceTableModel;
    QMap<int,QVariant> verticalHeaderData;
    QMap<int,QVariant> horizontalHeaderData;
};


#endif // UTIL_QRESERVETABLEMODEL_H
