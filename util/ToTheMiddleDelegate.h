#ifndef UTIL_TOTHEMIDDLEDELEGATE_H
#define UTIL_TOTHEMIDDLEDELEGATE_H

#include <QItemDelegate>

class ToTheMiddleDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ToTheMiddleDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


#endif // UTIL_TOTHEMIDDLEDELEGATE_H
