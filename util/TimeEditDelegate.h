#ifndef UTIL_TIMEEDITDELEGATE_H
#define UTIL_TIMEEDITDELEGATE_H

#include <QItemDelegate>

class TimeEditDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    TimeEditDelegate(const QString timeFormat = "dd.MM.yyyy hh:mm:ss",QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;
private:
    QString timeformat;
};
#endif // UTIL_TIMEEDITDELEGATE_H
