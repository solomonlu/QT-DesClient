#ifndef PAGES_03STATUS_H
#define PAGES_03STATUS_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QSqlRelationalTableModel;
class QTimer;
QT_END_NAMESPACE

namespace Ui
{
    class MainWindow;
}

class QReserveTableModel;
class PageStatus:public QObject
{
    Q_OBJECT
public:
    PageStatus(Ui::MainWindow *pui,QObject *parent = 0);
    virtual bool event ( QEvent * event ); // for refresh use
private slots:
    void bofIdSelected();
//    void hmIntoBof();
    void forecastToBof();
    void confirmToBof();
    void forecastEdit();
    void cancelForecast();
    void refresh();
private:
    Ui::MainWindow *ui;
    QSqlRelationalTableModel* statusTableModel;
    QReserveTableModel* reserveTableModel;
    QSqlRelationalTableModel* ladleStatusTableModel;
    QSqlRelationalTableModel* ladleResultTableModel;
    QSqlRelationalTableModel* ladlePourResultTableModel;
    QSqlRelationalTableModel* ladleDesResultTableModel;
    QTimer* timer;
};

#endif // PAGES_03STATUS_H
