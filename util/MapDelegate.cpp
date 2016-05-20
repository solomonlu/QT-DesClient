//#include "MapDelegate.h"
//#include <QMessageBox>
//MapDelegate::MapDelegate(QMap<int,QVariant> MapTable,QObject *parent)
//    :QItemDelegate(parent),mapTable(MapTable)
//{

//}

//void MapDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex& index) const
//{
//    QMessageBox::critical(0, tr("Cannot open database"),
//        tr("Unable to establish a database connection."
//                 "The database reported an error: ")
//                 , QMessageBox::Ok);

//    int key = model->data(index).toInt();
//    if (mapTable.find(key) == mapTable.end())
//    {
//        QItemDelegate::setModelData(editor,model,index);
//    }
//    else
//    {
//        model->setData(index,mapTable[key]);

//    }
//}
