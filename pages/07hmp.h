#ifndef PAGES_07HMP_H
#define PAGES_07HMP_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QSqlRelationalTableModel;
class QTimer;
QT_END_NAMESPACE

namespace Ui
{
    class MainWindow;
}

class PageHmp:public QObject
{
    Q_OBJECT
public:
    PageHmp(Ui::MainWindow *pui,QObject *parent = 0);
    virtual bool event ( QEvent * event ); // for refresh use
private slots:
    void hmNoSelected(const QString &);
    void generateReport();
    void insertStirInfo();
    void editStirInfo();
    void saveStirInfo();
    void cancelStirInfo();
    void insertSlagBefore();
    void insertSlagAfter();
    void editSlag();
    void saveSlag();
    void cancelSlag();
    void insertMaterial();
    void editMaterial();
    void saveMaterial();
    void cancelMaterial();
    void insertTemp();
    void editTemp();
    void saveTemp();
    void cancelTemp();
    void editReserve();
    void saveReserve();
    void cancelReserve();
    void refresh();
    void sendResult();
    void saveWeight();
private:
    Ui::MainWindow *ui;
    QSqlRelationalTableModel *desModel;
    QSqlRelationalTableModel *stirInfoModel;
    QSqlRelationalTableModel *slagModel;
    QSqlRelationalTableModel *materialModel;
    QSqlRelationalTableModel *tempModel;
    QSqlRelationalTableModel *eventModel;
    QSqlRelationalTableModel *reserveModel;
    QSqlRelationalTableModel *ironSteelModel;
    QTimer* timer;
    int stirInfoModifyFlag;
    int materialModifyFlag;
    QString stationNo;
};

#endif // PAGES_07HMP_H
