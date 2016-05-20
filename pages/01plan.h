#ifndef PAGES_01PLAN_H
#define PAGES_01PLAN_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QSqlRelationalTableModel;
class QTimer;
class QModelIndex;
class QAbstractButton;
class QDialog;
QT_END_NAMESPACE

namespace Ui
{
    class MainWindow;
    class PlanDialog;
}

class PagePlan:public QObject
{
    Q_OBJECT
public:
    PagePlan(Ui::MainWindow *pui,QObject *parent = 0);
    virtual bool event ( QEvent * event ); // for refresh use
private slots:
    void refresh();
    void showDialog(const QModelIndex &);
    void closeDialog(QAbstractButton *);
    void plan1TableViewVerticalScrollBarChangeValue(int);
    void plan1TableViewHorizontalScrollBarChangeValue(int);
    void plan2TableViewVerticalScrollBarChangeValue(int);
    void plan2TableViewHorizontalScrollBarChangeValue(int);
private:
    Ui::MainWindow *ui;
    Ui::PlanDialog* planDialog;
    QSqlRelationalTableModel* planTableModel;
    QTimer* timer;
    QDialog* showPlanDialog;
    int pushPlanIndex;
};

#endif // PAGES_01PLAN_H
