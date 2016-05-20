#ifndef PAGES_04DES_H
#define PAGES_04DES_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QSqlRelationalTableModel;
class QTimer;
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE
class QSqlQueryModel;
class QTimer;
QT_END_NAMESPACE

namespace Ui
{
    class MainWindow;
}

class PageDes:public QObject
{
    Q_OBJECT
public:
    PageDes(Ui::MainWindow *pui,QObject *parent = 0);
    virtual bool event ( QEvent * event ); // for refresh use
private slots:
    void refresh();
private:
    Ui::MainWindow *ui;
    QSqlRelationalTableModel *des1Model;
    QSqlRelationalTableModel *des2Model;
    QTimer* timer;
};

#endif // PAGES_04DES_H
