#include "10model.h"
#include "ui_mainwindow.h"

PageModel::PageModel(Ui::MainWindow *pui,QObject *parent)
    : QObject(parent), ui(pui)
{

}

bool PageModel::event ( QEvent * )
{
    return true;
}


