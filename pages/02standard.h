#ifndef PAGES_02STANDARD_H
#define PAGES_02STANDARD_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QSqlQueryModel;
QT_END_NAMESPACE

namespace Ui
{
    class MainWindow;
}

class PageStandard:public QObject
{
    Q_OBJECT
public:
    PageStandard(Ui::MainWindow *pui,QObject *parent = 0);
    virtual bool event ( QEvent * event ); // for refresh use
private slots:
    void stNoSelected(const QString &);
    void InternalToggled ( bool checked );
    void ExternalToggled ( bool checked );
private:
    Ui::MainWindow *ui;
    QSqlQueryModel *standard1Model;
};

#endif // PAGES_02STANDARD_H
