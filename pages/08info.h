#ifndef PAGES_08INFO_H
#define PAGES_08INFO_H

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
class PageInfo:public QObject
{
    Q_OBJECT
public:
    PageInfo(Ui::MainWindow *pui,QObject *parent = 0);
    virtual bool event ( QEvent * event ); // for refresh use
private slots:
    void insertDevice();
    void editDevice();
    void saveDevice();
    void cancelDevice();
    void des4stirNoSelected(const QString &);
    void des5stirNoSelected(const QString &);
    void simDes4Status();
    void simDes5Status();
    void desReport();
    void pourReport();
    void refresh();
private:
    void FillComboBox();
private:
    Ui::MainWindow *ui;
    QSqlRelationalTableModel *crewInfoModel;
    QSqlRelationalTableModel *deviceInfoModel;
    QSqlRelationalTableModel *currentInfoModel;
    QSqlRelationalTableModel *materialInfoModel;
    QSqlRelationalTableModel *statusSimModel;
    QReserveTableModel* statusSimTableModel;
    QTimer* timer;
};

#endif // PAGES_08INFO_H
