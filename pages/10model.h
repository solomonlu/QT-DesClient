#ifndef PAGES_10MODEL_H
#define PAGES_10MODEL_H

#include <QWidget>

namespace Ui
{
    class MainWindow;
}

class PageModel:public QObject
{
    Q_OBJECT
public:
    PageModel(Ui::MainWindow *pui,QObject *parent = 0);
    virtual bool event ( QEvent * event ); // for refresh use
private:
    Ui::MainWindow *ui;
};

#endif // PAGES_10MODEL_H
