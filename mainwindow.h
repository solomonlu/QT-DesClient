#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

QT_BEGIN_NAMESPACE
class QListWidgetItem;
class QTimer;
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
class planConfirmDialog;
class shiftDialog;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);
    void planConfirmClicked();
    void refreshPlan();
    void shiftClicked();
    void planNoSelected(const QString &selectContent);
    void refreshSystemTime();
    void show01PlanPage();
    void show02StandardPage();
    void show03StatusPage();
    void show04DesPage();
    void show05SlagPage();
    void show06ElePage();
    void show07HmpPage();
    void show08InfoPage();
    void show09AlramPage();
    void show10ModelPage();
    void show11PourPage();
private:
    virtual void keyPressEvent ( QKeyEvent * event );

    Ui::MainWindow *ui;
    Ui::planConfirmDialog *planConfirmDialog;
    Ui::shiftDialog* shiftDialog;
    QDialog* planConfirmShowDialog;
    QDialog* shiftShowDialog;
    QTimer* timer;
    QVector<QObject*> pageVector;
    int stationNo;
    int isPushPlan;
    int isPushShift;
};

#endif // MAINWINDOW_H
