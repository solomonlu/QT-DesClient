#include <QtSql>
#include "08info.h"
#include "ui_mainwindow.h"
#include "util/ToTheMiddleDelegate.h"
#include "util/QReserveTableModel.h"
#include "util/TimeEditDelegate.h"
#include <QMessageBox>
#include "util/ReadOnlyDelegate.h"

PageInfo::PageInfo(Ui::MainWindow *pui,QObject *parent)
    : QObject(parent), ui(pui)
{
    connect(ui->deviceInsertBtn, SIGNAL(clicked()), this, SLOT(insertDevice()));
    connect(ui->deviceEditBtn, SIGNAL(clicked()), this, SLOT(editDevice()));
    connect(ui->deviceSaveBtn,SIGNAL(clicked()),this,SLOT(saveDevice()));
    connect(ui->deviceCancelBtn,SIGNAL(clicked()),this,SLOT(cancelDevice()));
    connect(ui->simDes4Btn,SIGNAL(clicked()),this,SLOT(simDes4Status()));
    connect(ui->simDes5Btn,SIGNAL(clicked()),this,SLOT(simDes5Status()));
    connect(ui->desReportBtn,SIGNAL(clicked()),this,SLOT(desReport()));
    connect(ui->pourReportBtn,SIGNAL(clicked()),this,SLOT(pourReport()));

    crewInfoModel = new QSqlRelationalTableModel(this);
    crewInfoModel->setTable("CREW_RESULT");
    crewInfoModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    crewInfoModel->setRelation(crewInfoModel->fieldIndex("SHIFT"), QSqlRelation("SHIFT_MAP", "SHIFT", "SHIFT_DESC"));
    crewInfoModel->setRelation(crewInfoModel->fieldIndex("CREW"), QSqlRelation("CREW_MAP", "CREW", "CREW_DESC"));
    crewInfoModel->select();
    ui->crewResultTableView->setModel(crewInfoModel);
    crewInfoModel->setHeaderData(crewInfoModel->fieldIndex("SHIFT_DATE"), Qt::Horizontal, tr("SHIFT_DATE"));
    crewInfoModel->setHeaderData(crewInfoModel->fieldIndex("SHIFT"), Qt::Horizontal, tr("SHIFT"));
    crewInfoModel->setHeaderData(crewInfoModel->fieldIndex("CREW"), Qt::Horizontal, tr("CREW"));
    crewInfoModel->setHeaderData(crewInfoModel->fieldIndex("SHIFT_DESC"), Qt::Horizontal, tr("SHIFT_DESC"));
    crewInfoModel->setHeaderData(crewInfoModel->fieldIndex("CREW_DESC"), Qt::Horizontal, tr("CREW_DESC"));

    ui->crewResultTableView->setItemDelegate(new ToTheMiddleDelegate);

    deviceInfoModel = new QSqlRelationalTableModel(this);
    deviceInfoModel->setTable("DEVICE_INFO");
    deviceInfoModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    deviceInfoModel->select();
    ui->deviceInfoTableView->setModel(deviceInfoModel);

    deviceInfoModel->setHeaderData(deviceInfoModel->fieldIndex("STIR_NO"), Qt::Horizontal, tr("STIR_NO"));
    deviceInfoModel->setHeaderData(deviceInfoModel->fieldIndex("STIR_TIMES"), Qt::Horizontal, tr("STIR_TIMES"));

    ui->deviceInfoTableView->setItemDelegateForColumn(0,new ReadOnlyDelegate(this));

    ui->deviceInfoTableView->setItemDelegate(new ToTheMiddleDelegate);

    currentInfoModel = new QSqlRelationalTableModel(this);
    currentInfoModel->setTable("CURRENT_INFO");
    currentInfoModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    currentInfoModel->setRelation(currentInfoModel->fieldIndex("CURRENT_DES_SHIFT"), QSqlRelation("SHIFT_MAP", "SHIFT", "SHIFT_DESC"));
    currentInfoModel->setRelation(currentInfoModel->fieldIndex("CURRENT_DES_CREW"), QSqlRelation("CREW_MAP", "CREW", "CREW_DESC"));
    currentInfoModel->select();
    ui->currentInfoTableView->setModel(currentInfoModel);

    currentInfoModel->setHeaderData(currentInfoModel->fieldIndex("CURRENT_DES4_HM_NO"), Qt::Horizontal, tr("CURRENT_DES4_HM_NO"));
    currentInfoModel->setHeaderData(currentInfoModel->fieldIndex("CURRENT_DES4_STIR_NO"), Qt::Horizontal, tr("CURRENT_DES4_STIR_NO"));
    currentInfoModel->setHeaderData(currentInfoModel->fieldIndex("CURRENT_DES4_STATUS"), Qt::Horizontal, tr("CURRENT_DES4_STATUS"));
    currentInfoModel->setHeaderData(currentInfoModel->fieldIndex("CURRENT_DES5_HM_NO"), Qt::Horizontal, tr("CURRENT_DES5_HM_NO"));
    currentInfoModel->setHeaderData(currentInfoModel->fieldIndex("CURRENT_DES5_STIR_NO"), Qt::Horizontal, tr("CURRENT_DES5_STIR_NO"));
    currentInfoModel->setHeaderData(currentInfoModel->fieldIndex("CURRENT_DES5_STATUS"), Qt::Horizontal, tr("CURRENT_DES5_STATUS"));
    currentInfoModel->setHeaderData(currentInfoModel->fieldIndex("CURRENT_DES_SHIFT"), Qt::Horizontal, tr("CURRENT_DES_SHIFT"));
    currentInfoModel->setHeaderData(currentInfoModel->fieldIndex("CURRENT_DES_CREW"), Qt::Horizontal, tr("CURRENT_DES_CREW"));
    currentInfoModel->setHeaderData(currentInfoModel->fieldIndex("SHIFT_DESC"), Qt::Horizontal, tr("SHIFT_DESC"));
    currentInfoModel->setHeaderData(currentInfoModel->fieldIndex("CREW_DESC"), Qt::Horizontal, tr("CREW_DESC"));

    ui->currentInfoTableView->setColumnWidth(currentInfoModel->fieldIndex("CURRENT_DES4_HM_NO"),120);
    ui->currentInfoTableView->setColumnWidth(currentInfoModel->fieldIndex("CURRENT_DES4_STIR_NO"),120);
    ui->currentInfoTableView->setColumnWidth(currentInfoModel->fieldIndex("CURRENT_DES4_STATUS"),120);
    ui->currentInfoTableView->setColumnWidth(currentInfoModel->fieldIndex("CURRENT_DES5_HM_NO"),120);
    ui->currentInfoTableView->setColumnWidth(currentInfoModel->fieldIndex("CURRENT_DES5_STIR_NO"),120);
    ui->currentInfoTableView->setColumnWidth(currentInfoModel->fieldIndex("CURRENT_DES5_STATUS"),120);
    ui->currentInfoTableView->setColumnWidth(currentInfoModel->fieldIndex("CURRENT_DES_SHIFT"),120);
    ui->currentInfoTableView->setColumnWidth(currentInfoModel->fieldIndex("CURRENT_DES_CREW"),120);
    ui->currentInfoTableView->setColumnWidth(currentInfoModel->fieldIndex("SHIFT_DESC"),120);
    ui->currentInfoTableView->setColumnWidth(currentInfoModel->fieldIndex("CREW_DESC"),120);

    ui->currentInfoTableView->setItemDelegate(new ToTheMiddleDelegate);

    materialInfoModel = new QSqlRelationalTableModel(this);
    materialInfoModel->setTable("MATERIAL_INFO");
    materialInfoModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    materialInfoModel->select();
    ui->materialInfoTableView->setModel(materialInfoModel);

    materialInfoModel->setHeaderData(materialInfoModel->fieldIndex("CAO"), Qt::Horizontal, tr("CAO"));
    materialInfoModel->setHeaderData(materialInfoModel->fieldIndex("CAF2"), Qt::Horizontal, tr("CAF2"));
    materialInfoModel->setHeaderData(materialInfoModel->fieldIndex("SIO2"), Qt::Horizontal, tr("SIO2"));
    materialInfoModel->setHeaderData(materialInfoModel->fieldIndex("MGO"), Qt::Horizontal, tr("MGO"));
    materialInfoModel->setHeaderData(materialInfoModel->fieldIndex("H2O"), Qt::Horizontal, tr("H2O"));
    materialInfoModel->setHeaderData(materialInfoModel->fieldIndex("NAOH"), Qt::Horizontal, tr("NAOH"));

    ui->materialInfoTableView->setItemDelegate(new ToTheMiddleDelegate);

    statusSimModel = new QSqlRelationalTableModel(this);
    statusSimModel->setTable("V_STATUS");
    statusSimModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    statusSimModel->select();

    statusSimTableModel = new QReserveTableModel(statusSimModel,this);
    ui->statusSimTableView->setModel(statusSimTableModel);


    int i=0;
    statusSimTableModel->setHeaderData(i++, Qt::Horizontal, tr("D1"));
    statusSimTableModel->setHeaderData(i++, Qt::Horizontal, tr("D2"));

    i=1;
    statusSimTableModel->setHeaderData(statusSimModel->fieldIndex("PLAN_NO"), Qt::Vertical, tr("PLAN_NO"));
    statusSimTableModel->setHeaderData(statusSimModel->fieldIndex("PRODUCTION_ID"), Qt::Vertical, tr("PRODUCTION_ID"));
    statusSimTableModel->setHeaderData(statusSimModel->fieldIndex("TREAT_ID"), Qt::Vertical, tr("TREAT_ID"));
    statusSimTableModel->setHeaderData(statusSimModel->fieldIndex("IRON_LADLE_ID"), Qt::Vertical, tr("IRON_LADLE_ID"));
    statusSimTableModel->setHeaderData(statusSimModel->fieldIndex("STEEL_GRADE"), Qt::Vertical, tr("STEEL_GRADE"));
    statusSimTableModel->setHeaderData(statusSimModel->fieldIndex("STATUS_CODE"), Qt::Vertical, tr("STATUS_CODE"));
    statusSimTableModel->setHeaderData(statusSimModel->fieldIndex("NEXT_STATUS"), Qt::Vertical, tr("NEXT_STATUS"));
    statusSimTableModel->setHeaderData(statusSimModel->fieldIndex("TIME_TO_END"), Qt::Vertical, tr("TIME_TO_END"));
    statusSimTableModel->setHeaderData(statusSimModel->fieldIndex("UPDATE_TIME"), Qt::Vertical, tr("UPDATE_TIME"));

    statusSimModel->setRelation(statusSimModel->fieldIndex("STATUS_CODE"), QSqlRelation("STATUS_MAP", "STATUS_CODE", "STATUS_DESC"));
    statusSimModel->setRelation(statusSimModel->fieldIndex("NEXT_STATUS"), QSqlRelation("STATUS_MAP", "STATUS_CODE", "STATUS_DESC"));
    statusSimModel->select();

    ui->statusSimTableView->setItemDelegateForRow(9, new TimeEditDelegate("MM-dd hh:mm"));

    ui->statusSimTableView->setRowHidden(statusSimModel->fieldIndex("STATION_NO"),true); // hide station no

    ui->statusSimTableView->verticalHeader()->setVisible(true);
    ui->statusSimTableView->horizontalHeader()->setVisible(true);

    ui->statusSimTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->statusSimTableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    ui->statusSimTableView->setItemDelegate(new ToTheMiddleDelegate);

    for(i=0;i<2;i++)
    {
        ui->statusSimTableView->setColumnWidth(i,120);
    }

    FillComboBox();
    ui->des4stirNoComboBox->setCurrentIndex(ui->des4stirNoComboBox->findText(currentInfoModel->record(0).value("CURRENT_DES4_STIR_NO").toString()));
    ui->des5stirNoComboBox->setCurrentIndex(ui->des5stirNoComboBox->findText(currentInfoModel->record(0).value("CURRENT_DES5_STIR_NO").toString()));

    connect(ui->des4stirNoComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(des4stirNoSelected(const QString &)));
    connect(ui->des5stirNoComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(des5stirNoSelected(const QString &)));

    ui->desReportDateEdit->setDate(QDate::currentDate());
    ui->pourReportDateEdit->setDate(QDate::currentDate());

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
    timer->start(1000);
}

void PageInfo::refresh()
{
    crewInfoModel->select();
    deviceInfoModel->select();
    currentInfoModel->select();
    materialInfoModel->select();
    statusSimModel->select();
}

bool PageInfo::event ( QEvent * )
{
    return true;
}

void PageInfo::insertDevice()
{
    QSqlQuery insertDeviceQuery("INSERT INTO DEVICE_INFO(STIR_NO) VALUES((SELECT MAX(STIR_NO) FROM DEVICE_INFO)+1)");
    deviceInfoModel->select();
}

void PageInfo::editDevice()
{
    ui->deviceInsertBtn->setDisabled(true);
    ui->deviceEditBtn->setDisabled(true);
    ui->deviceSaveBtn->setDisabled(false);
    ui->deviceCancelBtn->setDisabled(false);
    ui->deviceInfoTableView->setEditTriggers(QAbstractItemView::AllEditTriggers);

    timer->stop();
}

void PageInfo::saveDevice()
{
    ui->deviceInsertBtn->setDisabled(false);
    ui->deviceEditBtn->setDisabled(false);
    ui->deviceSaveBtn->setDisabled(true);
    ui->deviceCancelBtn->setDisabled(true);
    ui->deviceInfoTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    if (deviceInfoModel->submitAll())
    {
        deviceInfoModel->database().commit();
    }
    timer->start();
}

void PageInfo::cancelDevice()
{
    ui->deviceInsertBtn->setDisabled(false);
    ui->deviceEditBtn->setDisabled(false);
    ui->deviceSaveBtn->setDisabled(true);
    ui->deviceCancelBtn->setDisabled(true);
    ui->deviceInfoTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    deviceInfoModel->database().rollback();
    deviceInfoModel->select();
    timer->start();
}

void PageInfo::FillComboBox()
{
    QStringList stirNoList;
    QSqlQuery stirNoQuery("SELECT STIR_NO FROM DEVICE_INFO");
    while (stirNoQuery.next())
    {
        QString stirNo = stirNoQuery.value(0).toString();
        stirNoList.append(stirNo);
    }

    ui->des4stirNoComboBox->clear();
    ui->des4stirNoComboBox->addItems(stirNoList);
    ui->des5stirNoComboBox->clear();
    ui->des5stirNoComboBox->addItems(stirNoList);
}

void PageInfo::des4stirNoSelected(const QString &selectContent)
{
    QSqlQuery updateCurrentQuery("UPDATE CURRENT_INFO SET CURRENT_DES4_STIR_NO="+selectContent);
    currentInfoModel->select();
}

void PageInfo::des5stirNoSelected(const QString &selectContent)
{
    QSqlQuery updateCurrentQuery("UPDATE CURRENT_INFO SET CURRENT_DES5_STIR_NO="+selectContent);
    currentInfoModel->select();
}

void PageInfo::simDes4Status()
{
    QSqlQuery nextStatusQuery("SELECT NEXT_STATUS FROM V_STATUS WHERE STATION_NO = 1");
    if (nextStatusQuery.next())
    {
        QString messageId;
        int nextStatus=nextStatusQuery.value(0).toInt();
        if (nextStatus==1)
        {
            messageId="1001";
        }
        if (nextStatus==4)
        {
            messageId="1002";
        }
        if (nextStatus==5)
        {
            messageId="1003";
        }
        if (nextStatus==8)
        {
            messageId="1004";
        }
        QSqlQuery simDes4StatusQuery;
        simDes4StatusQuery.prepare("DECLARE RET NUMBER; BEGIN RET:=PAKTEST.calc('L1_COM',4,'c8@"+messageId+"',10); END;");
        simDes4StatusQuery.exec();
        statusSimModel->select();
    }
    else
    {
        QMessageBox::critical(0, qApp->tr("Cannot sim des4 status"),
            qApp->tr("由于下一状态为空，无法模拟4号脱硫站状态")
                     , QMessageBox::Ok);
    }
}

void PageInfo::simDes5Status()
{
    QSqlQuery nextStatusQuery("SELECT NEXT_STATUS FROM V_STATUS WHERE STATION_NO = 2");
    if (nextStatusQuery.next())
    {
        QString messageId;
        int nextStatus=nextStatusQuery.value(0).toInt();
        if (nextStatus==1)
        {
            messageId="2001";
        }
        if (nextStatus==4)
        {
            messageId="2002";
        }
        if (nextStatus==5)
        {
            messageId="2003";
        }
        if (nextStatus==8)
        {
            messageId="2004";
        }
        QSqlQuery simDes5StatusQuery;
        simDes5StatusQuery.prepare("DECLARE RET NUMBER; BEGIN RET:=PAKTEST.calc('L1_COM',4,'c8@"+messageId+"',10); END;");
        simDes5StatusQuery.exec();
        statusSimModel->select();
    }
    else
    {
        QMessageBox::critical(0, qApp->tr("Cannot sim des5 status"),
            qApp->tr("由于下一状态为空，无法模拟5号脱硫站状态")
                     , QMessageBox::Ok);
    }
}

void PageInfo::desReport()
{
    QString shiftDate;
    QString shift;
    shiftDate=ui->desReportDateEdit->date().toString("yyyyMMdd");
    shift=QString::number(ui->desReportShiftComboBox->currentIndex()+1,10);
    QSqlQuery desReportQuery("SELECT CREW FROM CREW_RESULT WHERE SHIFT_DATE = '" + shiftDate +"' AND SHIFT='" + shift + "'");
    if (desReportQuery.next())
    {
        QString cmd = "D:\\DesClient_run\\DES_REPORT " + shiftDate + " " + shift;
        system(cmd.toStdString().c_str());
    }
    else
    {
        QMessageBox::critical(0, qApp->tr("Cannot generate report"),
            qApp->tr("由于没有班组实绩，无法生成预处理报表")
                     , QMessageBox::Ok);
    }
}

void PageInfo::pourReport()
{
    QString shiftDate;
    QString shift;
    shiftDate=ui->pourReportDateEdit->date().toString("yyyyMMdd");
    shift=QString::number(ui->pourReportShiftComboBox->currentIndex()+1,10);
    QSqlQuery pourReportQuery("SELECT CREW FROM CREW_RESULT WHERE SHIFT_DATE = '" + shiftDate +"' AND SHIFT='" + shift + "'");
    qDebug()<<"D:\\DesClient_run\\POUR_REPORT " + shiftDate + " " + shift;
    if (pourReportQuery.next())
    {
        QString cmd = "D:\\DesClient_run\\POUR_REPORT " + shiftDate + " " + shift;
        system(cmd.toStdString().c_str());
    }
    else
    {
        QMessageBox::critical(0, qApp->tr("Cannot generate report"),
            qApp->tr("由于没有班组实绩，无法生成倒罐报表")
                     , QMessageBox::Ok);
    }
}
