#ifndef PAGES_09ALARM_H
#define PAGES_09ALARM_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QSqlRelationalTableModel;
class QTimer;
QT_END_NAMESPACE

namespace Ui
{
    class MainWindow;
}

class PageAlarm:public QObject
{
    Q_OBJECT
public:
    PageAlarm(Ui::MainWindow *pui,QObject *parent = 0);
    virtual bool event ( QEvent * event ); // for refresh use
private slots:
    void select();
    void refresh();
private:
    Ui::MainWindow *ui;
    QSqlRelationalTableModel* alarmTableModel;
    QTimer* timer;
};

#endif // PAGES_09ALARM_H
