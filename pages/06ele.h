#ifndef PAGES_06ELE_H
#define PAGES_06ELE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QSqlQueryModel;
class QSqlTableModel;
class QTimer;
QT_END_NAMESPACE

namespace Ui
{
    class MainWindow;
}

class PageEle:public QObject
{
    Q_OBJECT
public:
    PageEle(Ui::MainWindow *pui,QObject *parent = 0);
    virtual bool event ( QEvent * event ); // for refresh use
private slots:
    void changeColor();
    void hmNoSelected(const QString &);
    void setSampleEditable();
    void saveSample();
    void insertSample();
    void cancelSample();
    void InternalToggled ( bool checked );
    void ExternalToggled ( bool checked );
    void ele1TableViewVerticalScrollBarChangeValue(int);
    void ele2TableViewVerticalScrollBarChangeValue(int);
    void refresh();
    void confirmEle();
private:
    Ui::MainWindow *ui;
    QSqlQueryModel *standard1Model;
    QSqlTableModel *ele1Model;
    QTimer* timer;
    QString stationNo;
};

#endif // PAGES_06ELE_H
