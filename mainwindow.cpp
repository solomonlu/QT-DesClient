#include <QtSql>
#include <QKeyEvent>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_planconfirmdialog.h"
#include "ui_shiftdialog.h"
#include "database.h"
#include "pages/01plan.h"
#include "pages/02standard.h"
#include "pages/03status.h"
#include "pages/04des.h"
#include "pages/05slag.h"
#include "pages/06ele.h"
#include "pages/07hmp.h"
#include "pages/08info.h"
#include "pages/09alarm.h"
#include "pages/10model.h"
#include "pages/11pour.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    planConfirmDialog(new Ui::planConfirmDialog),
    planConfirmShowDialog(new QDialog),
    shiftDialog(new Ui::shiftDialog),
    shiftShowDialog(new QDialog)
{
    QTextCodec*pCodec=QTextCodec::codecForName("System");//获取系统字体编码

    QTextCodec::setCodecForLocale(pCodec);

    QTextCodec::setCodecForCStrings(pCodec);

    QTextCodec::setCodecForTr(pCodec);

    Database::createConnection();

    ui->setupUi(this);
    planConfirmDialog->setupUi(planConfirmShowDialog);
    shiftDialog->setupUi(shiftShowDialog);
    QPixmap image = QPixmap(":/icons/top2.gif");
    ui->label_3->setPixmap(image);
    image = QPixmap(":/icons/top.jpg");
    ui->label_7->setPixmap(image);
    QFont ft;
    ft.setPointSize(20);
    ui->systemTimeDateTimeLabel->setFont(ft);

    QSqlQuery systemTimeQuery("SELECT SYSDATE FROM DUAL");
    systemTimeQuery.next();
    QString systemTime = systemTimeQuery.value(0).toString();
    ui->systemTimeDateTimeLabel->setText(QDateTime::fromString(systemTime,Qt::ISODate).toString("yyyy-MM-dd HH:mm:ss"));

    pageVector.append(new PagePlan(ui,this));
    pageVector.append(new PageStandard(ui,this));
    pageVector.append(new PageStatus(ui,this));
    pageVector.append(new PageDes(ui,this));
    pageVector.append(new PageSlag(ui,this));
    pageVector.append(new PageEle(ui,this));
    pageVector.append(new PageHmp(ui,this));
    pageVector.append(new PageInfo(ui,this));
    pageVector.append(new PageAlarm(ui,this));
    pageVector.append(new PageModel(ui,this));
    pageVector.append(new PagePour(ui,this));

    connect(ui->listWidget,SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *))
            ,this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));
    connect(planConfirmDialog->planConfirmBtn,SIGNAL(clicked()),this,SLOT(planConfirmClicked()));
    connect(planConfirmDialog->refreshPlanButton,SIGNAL(clicked()),this,SLOT(refreshPlan()));
    connect(shiftDialog->shiftBtn,SIGNAL(clicked()),this,SLOT(shiftClicked()));
    connect(ui->actionPlan,SIGNAL(triggered()),this,SLOT(show01PlanPage()));
    connect(ui->actionStandard,SIGNAL(triggered()),this,SLOT(show02StandardPage()));
    connect(ui->actionStatus,SIGNAL(triggered()),this,SLOT(show03StatusPage()));
    connect(ui->actionDes,SIGNAL(triggered()),this,SLOT(show04DesPage()));
    connect(ui->actionSlag,SIGNAL(triggered()),this,SLOT(show05SlagPage()));
    connect(ui->actionEle,SIGNAL(triggered()),this,SLOT(show06ElePage()));
    connect(ui->actionHmp,SIGNAL(triggered()),this,SLOT(show07HmpPage()));
    connect(ui->actionInfo,SIGNAL(triggered()),this,SLOT(show08InfoPage()));
    connect(ui->actionAlarm,SIGNAL(triggered()),this,SLOT(show09AlramPage()));
    connect(ui->actionModel,SIGNAL(triggered()),this,SLOT(show10ModelPage()));
    connect(ui->actionPour,SIGNAL(triggered()),this,SLOT(show11PourPage()));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refreshSystemTime()));
    timer->start(1000);

    QSettings settings("Option.ini", QSettings::IniFormat);
    stationNo= settings.value("PushConfig/StationNo").toInt();
    isPushPlan = settings.value("PushConfig/PushPlan").toInt();
    isPushShift = settings.value("PushConfig/PushShift").toInt();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    ui->stackedWidget->setCurrentIndex(ui->listWidget->row(current));
    pageVector[ui->listWidget->row(current)]->event(0);
}

void MainWindow::refreshSystemTime()
{
    QSqlQuery systemTimeQuery("SELECT SYSDATE FROM DUAL");
    systemTimeQuery.next();
    QString systemTime = systemTimeQuery.value(0).toString();
    ui->systemTimeDateTimeLabel->setText(QDateTime::fromString(systemTime,Qt::ISODate).toString("yyyy-MM-dd HH:mm:ss"));

    QSqlQuery statusBarLogQuery("SELECT MSG FROM MESSAGESHOW ORDER BY MSGDATE DESC");
    statusBarLogQuery.next();
    QString statusBarLog = statusBarLogQuery.value(0).toString();
    ui->statusBar->showMessage(statusBarLog);

    QSqlQuery pushQuery("SELECT PUSH_PLAN,PUSH_SHIFT FROM PUSH_FORM");
    if (pushQuery.next())
    {
        int pushPlanFlag = pushQuery.value(0).toInt();
        if (isPushPlan == 1 && pushPlanFlag && stationNo == pushPlanFlag)
        {
            if(pushPlanFlag==4)
            {
                QSqlQuery nextHMNoQuery("SELECT to_number(CURRENT_DES4_HM_NO)+1 FROM CURRENT_INFO");
                if (nextHMNoQuery.next())
                {
                    planConfirmDialog->hmNoLable->setText( nextHMNoQuery.value(0).toString());
                }
            }
            else if(pushPlanFlag==5)
            {
                QSqlQuery nextHMNoQuery("SELECT to_number(CURRENT_DES5_HM_NO)+1 FROM CURRENT_INFO");
                if (nextHMNoQuery.next())
                {
                    planConfirmDialog->hmNoLable->setText( nextHMNoQuery.value(0).toString());
                }
            }
            QSqlQueryModel planConfirmModel;
            planConfirmModel.setQuery("SELECT * FROM PLAN WHERE PLAN_STATUS=0");
            planConfirmDialog->planConfirmTableView->setModel(&planConfirmModel);

            int i=0;
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("PLAN_NO"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("PRODUCTION_ID"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("ST_NO"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("ROUTE_ID"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("SMELT_TYPE"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("BOF_NO"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("CCM_NO"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("CAST_NO"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("CAST_TOTAL_HEATS"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("CAST_DIV_NO"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("DES_END_TIME"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("MAT_LOAD_START_TIME"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("BLOW_START_TIME"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("BLOW_END_TIME"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("TAP_START_TIME"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("TAP_END_TIME"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("REFINE_ST1"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("REFINE_ST2"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("REFINE_ST3"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("REFINE_ST4"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("LADLE_ARRIVE_TIME"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("LADLE_START_TIME"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("LADLE_END_TIME"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("LADLE_EARLY_TIME"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("PLAN_STEEL_WEIGHT"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("PLAN_STATUS_DESC"));
            planConfirmModel.setHeaderData(i++, Qt::Horizontal, tr("DS_CODE"));

            QStringList planNoList;
            //planNoList.append("");
            QSqlQuery planNoQuery("SELECT PLAN_NO FROM PLAN WHERE PLAN_STATUS<2");
            while (planNoQuery.next())
            {
                QString planNo = planNoQuery.value(0).toString();
                planNoList.append(planNo);
            }
            planConfirmDialog->PlanNoComboBox->clear();
            planConfirmDialog->PlanNoComboBox->addItems(planNoList);

            QStringList ladleIdList;
            QSqlQuery ladleIdQuery("SELECT LADLE_ID FROM LADLE_STATUS ORDER BY START_TIME ASC");
            ladleIdList.append("");
            while (ladleIdQuery.next())
            {
                QString ladleId = ladleIdQuery.value(0).toString();
                ladleIdList.append(ladleId);
            }
            planConfirmDialog->ladleIdComboBox->clear();
            planConfirmDialog->ladleIdComboBox->addItems(ladleIdList);

            connect(planConfirmDialog->PlanNoComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(planNoSelected(const QString &)));

            planConfirmShowDialog->exec();
        }

        int pushShiftFlag = pushQuery.value(1).toInt();
        if (isPushShift && pushShiftFlag && isPushShift == pushShiftFlag)
        {
            QSqlQuery shiftQuery("SELECT CURRENT_DES_SHIFT FROM CURRENT_INFO");
            shiftQuery.exec();
            if(shiftQuery.next())
            {
                int currentDesShift=shiftQuery.value(0).toInt();
                shiftDialog->shiftComboBox->setCurrentIndex(currentDesShift-1);
            }
            shiftShowDialog->exec();
        }
    }
}

void MainWindow::planConfirmClicked()
{
    QSqlQuery pushQuery("SELECT PUSH_PLAN,PUSH_SHIFT FROM PUSH_FORM");
    int pushPlanFlag=0;
    if (pushQuery.next())
    {
        pushPlanFlag = pushQuery.value(0).toInt();
    }

    //planConfirmDialog->hmNoLable->

    if (planConfirmDialog->PlanNoComboBox->currentText() == "")
    {
        QMessageBox::critical(0, qApp->tr("Error"),
            qApp->tr("计划号不能为空")
                     , QMessageBox::Ok);
        return;
    }

    if (planConfirmDialog->dsCodeComboBox->currentText() == "")
    {
        QMessageBox::critical(0, qApp->tr("Error"),
            qApp->tr("是否脱硫不能为空")
                     , QMessageBox::Ok);
        return;
    }

    QString stationNo=QString::number(pushPlanFlag,10);
    QString planNo=planConfirmDialog->PlanNoComboBox->currentText();
    QString dsCode=QString::number(planConfirmDialog->dsCodeComboBox->currentIndex(),10);
    QString ladleId=planConfirmDialog->ladleIdComboBox->currentText();;
    QSqlQuery sendLadleArriveQuery;
    sendLadleArriveQuery.prepare("DECLARE RET NUMBER; BEGIN RET:=PAKTEST.calc('L1_COM',4,'c8@9999,i4@"+stationNo+",c12@"+planNo+",i4@"+dsCode+",c12@"+ladleId+"',10); END;");
    sendLadleArriveQuery.exec();

    QSqlQuery UpdatePushQuery("UPDATE PUSH_FORM SET PUSH_PLAN=0");
    UpdatePushQuery.exec();

    planConfirmShowDialog->close();
}

void MainWindow::refreshPlan()
{
    QStringList planNoList;
    planNoList.append("");
    QSqlQuery planNoQuery("SELECT PLAN_NO FROM PLAN WHERE PLAN_STATUS=0");
    while (planNoQuery.next())
    {
        QString planNo = planNoQuery.value(0).toString();
        planNoList.append(planNo);
    }
    planConfirmDialog->PlanNoComboBox->clear();
    planConfirmDialog->PlanNoComboBox->addItems(planNoList);

    QStringList ladleIdList;
    QSqlQuery ladleIdQuery("SELECT LADLE_ID FROM LADLE_STATUS ORDER BY START_TIME ASC");
    ladleIdList.append("");
    while (ladleIdQuery.next())
    {
        QString ladleId = ladleIdQuery.value(0).toString();
        ladleIdList.append(ladleId);
    }
    planConfirmDialog->ladleIdComboBox->clear();
    planConfirmDialog->ladleIdComboBox->addItems(ladleIdList);

    QSqlRelationalTableModel *planConfirmModel;
    planConfirmModel = new QSqlRelationalTableModel(this);
    planConfirmModel->setTable("PLAN");
    planConfirmModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    planConfirmModel->setFilter("PLAN_STATUS=0");
    planConfirmDialog->planConfirmTableView->setModel(planConfirmModel);

    int i=0;
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("PLAN_NO"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("PRODUCTION_ID"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("ST_NO"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("ROUTE_ID"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("SMELT_TYPE"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("BOF_NO"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("CCM_NO"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("CAST_NO"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("CAST_TOTAL_HEATS"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("CAST_DIV_NO"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("DES_END_TIME"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("MAT_LOAD_START_TIME"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("BLOW_START_TIME"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("BLOW_END_TIME"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("TAP_START_TIME"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("TAP_END_TIME"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("REFINE_ST1"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("REFINE_ST2"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("REFINE_ST3"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("REFINE_ST4"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("LADLE_ARRIVE_TIME"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("LADLE_START_TIME"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("LADLE_END_TIME"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("LADLE_EARLY_TIME"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("PLAN_STEEL_WEIGHT"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("PLAN_STATUS_DESC"));
    planConfirmModel->setHeaderData(i++, Qt::Horizontal, tr("DS_CODE"));
    planConfirmModel->select();
}

void MainWindow::shiftClicked()
{
    QSqlQuery shiftDateQuery("SELECT to_char(sysdate,'YYYYMMDD') FROM DUAL");
    QString shift;
    QString shiftDate;
    QString crew;
    if (shiftDateQuery.next())
    {
        shiftDate=shiftDateQuery.value(0).toString();
    }
    shift=QString::number(shiftDialog->shiftComboBox->currentIndex()+1,10);
    crew=QString::number(shiftDialog->crewComboBox->currentIndex()+1,10);

    QSqlQuery UpdateCurrentQuery("UPDATE CURRENT_INFO SET CURRENT_DES_SHIFT='" + shift + "',CURRENT_DES_CREW='" + crew + "'");

    QSqlQuery InsertCrewQuery("INSERT INTO CREW_RESULT(SHIFT_DATE,SHIFT,CREW) VALUES('"
                              + shiftDate + "','"
                              + shift + "','"
                              + crew + "')");

    QSqlQuery UpdatePushQuery("UPDATE PUSH_FORM SET PUSH_SHIFT=0");
    UpdatePushQuery.exec();

    shiftShowDialog->close();
}

void MainWindow::planNoSelected(const QString &selectContent)
{
    QSqlQuery dsCodeQuery("SELECT DS_CODE FROM PLAN WHERE PLAN_NO='"+selectContent+"'");
    dsCodeQuery.next();
    QString dsCode = dsCodeQuery.value(0).toString();
    planConfirmDialog->dsCodeComboBox->setCurrentIndex(dsCode.toInt());
}


void MainWindow::show01PlanPage()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->listWidget->setCurrentRow(0);
}
void MainWindow::show02StandardPage()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->listWidget->setCurrentRow(1);
}
void MainWindow::show03StatusPage()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->listWidget->setCurrentRow(2);
}
void MainWindow::show04DesPage()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->listWidget->setCurrentRow(3);
}
void MainWindow::show05SlagPage()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->listWidget->setCurrentRow(4);
}
void MainWindow::show06ElePage()
{
    ui->stackedWidget->setCurrentIndex(5);
    ui->listWidget->setCurrentRow(5);
}
void MainWindow::show07HmpPage()
{
    ui->stackedWidget->setCurrentIndex(6);
    ui->listWidget->setCurrentRow(6);
}
void MainWindow::show08InfoPage()
{
    ui->stackedWidget->setCurrentIndex(7);
    ui->listWidget->setCurrentRow(7);
}
void MainWindow::show09AlramPage()
{
    ui->stackedWidget->setCurrentIndex(8);
    ui->listWidget->setCurrentRow(8);
}
void MainWindow::show10ModelPage()
{
//    ui->stackedWidget->setCurrentIndex(9);
//    ui->listWidget->setCurrentRow(9);
}

void MainWindow::show11PourPage()
{
    ui->stackedWidget->setCurrentIndex(10);
    ui->listWidget->setCurrentRow(9);
}

void MainWindow::keyPressEvent ( QKeyEvent * event )
{
    switch (event->key())
    {
    case Qt::Key_F1:
        this->show01PlanPage();
        break;
    case Qt::Key_F2:
        this->show02StandardPage();
        break;
    case Qt::Key_F3:
        this->show03StatusPage();
        break;
    case Qt::Key_F4:
        this->show04DesPage();
        break;
    case Qt::Key_F5:
        this->show05SlagPage();
        break;
    case Qt::Key_F6:
        this->show06ElePage();
        break;
    case Qt::Key_F7:
        this->show07HmpPage();
        break;
    case Qt::Key_F8:
        this->show08InfoPage();
        break;
    case Qt::Key_F9:
        this->show09AlramPage();
        break;
    case Qt::Key_F10:
        this->show11PourPage();
        break;
    default:
        QWidget::keyPressEvent(event);
        break;
    }
}

