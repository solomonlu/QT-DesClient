#ifndef PAGES_11POUR_H
#define PAGES_11POUR_H
#include <QWidget>

QT_BEGIN_NAMESPACE
class QSqlTableModel;
class QTimer;
QT_END_NAMESPACE

namespace Ui
{
    class MainWindow;
}

class PagePour:public QObject
{
    Q_OBJECT
public:
    PagePour(Ui::MainWindow *pui,QObject *parent = 0);
    virtual bool event ( QEvent * event ); // for refresh use
private slots:
    void bfNoSelected(const QString &);
    void bfNoInSelected(const QString &);
    void bfNoOutSelected(const QString &);
    void bfNoOutStdSelected(const QString &);
    void pour();
    void pourStd();
    void ladleLeave();
    void ladleLeaveStd();
    void editIronInfo();
    void saveIronInfo();
    void cancelIronInfo();
    void editIronInfoStd();
    void saveIronInfoStd();
    void cancelIronInfoStd();
    void editPourInfo();
    void savePourInfo();
    void cancelPourInfo();
    void refresh();
    void sendPourResult();
private:
    Ui::MainWindow *ui;
    QSqlTableModel *ironInfoModel;
    QSqlTableModel *stdIronInfoModel;
    QSqlTableModel *pourInfoModel;
    QSqlTableModel *ironInfoHisModel;
    QSqlTableModel *stdIronInfoHisModel;
    QTimer* timer;
};

#endif // PAGES_11POUR_H
