#ifndef PAGES_05SLAG_H
#define PAGES_05SLAG_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QSqlRelationalTableModel;
class QTimer;
class QModelIndex;
QT_END_NAMESPACE

namespace Ui
{
    class MainWindow;
}

class PageSlag:public QObject
{
    Q_OBJECT
public:
    PageSlag(Ui::MainWindow *pui,QObject *parent = 0);
    virtual bool event ( QEvent * event ); // for refresh use
private slots:
    void selectSlag1(const QModelIndex &);
    void selectSlag2(const QModelIndex &);
    void refresh();
private:
    Ui::MainWindow *ui;
    QSqlRelationalTableModel *slag1Model;
    QSqlRelationalTableModel *slag2Model;
    QSqlRelationalTableModel *stirInfo1Model;
    QSqlRelationalTableModel *stirInfo2Model;
    QTimer* timer;
};

#endif // PAGES_05SLAG_H
