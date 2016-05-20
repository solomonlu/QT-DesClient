#include <QtSql>
#include <QMessageBox>
#include "util/TimeEditDelegate.h"
#include "util/QReserveTableModel.h"
#include "util/ReadOnlyDelegate.h"
#include "07hmp.h"
#include "ui_mainwindow.h"
#include "util/ToTheMiddleDelegate.h"

PageHmp::PageHmp(Ui::MainWindow *pui,QObject *parent)
    : QObject(parent), ui(pui)
{
    QSettings settings("Option.ini", QSettings::IniFormat);
    stationNo= settings.value("PushConfig/StationNo").toString();

    QStringList hmNoList;
    QSqlQuery hmNoQuery("SELECT HM_NO FROM V_DES_RESULT WHERE STATION_NO='D"+stationNo+"' ORDER BY EVENT_TIME DESC");
    while (hmNoQuery.next())
    {
        QString hmNo = hmNoQuery.value(0).toString();
        hmNoList.append(hmNo);
    }

    ui->hmpHMNoComboBox->clear();
    ui->hmpHMNoComboBox->addItems(hmNoList);

    if (hmNoList.length() > 0)
    {
        ui->hmpHMNoComboBox->setCurrentIndex(0);
        hmNoSelected(ui->hmpHMNoComboBox->currentText());
    }

    connect(ui->hmpHMNoComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(hmNoSelected(const QString &)));
    //connect(ui->generateReportButton, SIGNAL(clicked()), this, SLOT(generateReport()));
    connect(ui->sendResultButton, SIGNAL(clicked()), this, SLOT(sendResult()));
    connect(ui->hmpStirInfoInsertBtn, SIGNAL(clicked()), this, SLOT(insertStirInfo()));
    connect(ui->hmpStirInfoEditBtn, SIGNAL(clicked()), this, SLOT(editStirInfo()));
    connect(ui->hmpStirInfoSaveBtn,SIGNAL(clicked()),this,SLOT(saveStirInfo()));
    connect(ui->hmpStirInfoCancelBtn,SIGNAL(clicked()),this,SLOT(cancelStirInfo()));
    connect(ui->hmpSlagBeforeInsertBtn, SIGNAL(clicked()), this, SLOT(insertSlagBefore()));
    connect(ui->hmpSlagAfterInsertBtn, SIGNAL(clicked()), this, SLOT(insertSlagAfter()));
    connect(ui->hmpSlagEditBtn, SIGNAL(clicked()), this, SLOT(editSlag()));
    connect(ui->hmpSlagSaveBtn,SIGNAL(clicked()),this,SLOT(saveSlag()));
    connect(ui->hmpSlagCancelBtn,SIGNAL(clicked()),this,SLOT(cancelSlag()));
    connect(ui->hmpMaterialInsertBtn, SIGNAL(clicked()), this, SLOT(insertMaterial()));
    connect(ui->hmpMaterialEditBtn, SIGNAL(clicked()), this, SLOT(editMaterial()));
    connect(ui->hmpMaterialSaveBtn,SIGNAL(clicked()),this,SLOT(saveMaterial()));
    connect(ui->hmpMaterialCancelBtn,SIGNAL(clicked()),this,SLOT(cancelMaterial()));
    connect(ui->hmpTempInsertBtn, SIGNAL(clicked()), this, SLOT(insertTemp()));
    connect(ui->hmpTempEditBtn, SIGNAL(clicked()), this, SLOT(editTemp()));
    connect(ui->hmpTempSaveBtn,SIGNAL(clicked()),this,SLOT(saveTemp()));
    connect(ui->hmpTempCancelBtn,SIGNAL(clicked()),this,SLOT(cancelTemp()));
    connect(ui->hmpReserveEditBtn, SIGNAL(clicked()), this, SLOT(editReserve()));
    connect(ui->hmpReserveSaveBtn,SIGNAL(clicked()),this,SLOT(saveReserve()));
    connect(ui->hmpReserveCancelBtn,SIGNAL(clicked()),this,SLOT(cancelReserve()));
    connect(ui->saveWeightButton,SIGNAL(clicked()),this,SLOT(saveWeight()));

    stirInfoModifyFlag=0;
    materialModifyFlag=0;

    QValidator *validator = new QIntValidator(this);
    ui->ladleTareWeightLineEdit->setValidator(validator);
    ui->initGrossWeightLineEdit->setValidator(validator);
    ui->finalGrossWeightLineEdit->setValidator(validator);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
    timer->start(1000);
}

bool PageHmp::event ( QEvent * )
{
    return true;
}

void PageHmp::hmNoSelected(const QString &selectContent)
{
    if (selectContent == "")
    {
        ui->hmpStirInfoInsertBtn->setDisabled(true);
        ui->hmpStirInfoEditBtn->setDisabled(true);
        ui->hmpStirInfoSaveBtn->setDisabled(true);
        ui->hmpStirInfoCancelBtn->setDisabled(true);
        ui->hmpSlagBeforeInsertBtn->setDisabled(true);
        ui->hmpSlagAfterInsertBtn->setDisabled(true);
        ui->hmpSlagEditBtn->setDisabled(true);
        ui->hmpSlagSaveBtn->setDisabled(true);
        ui->hmpSlagCancelBtn->setDisabled(true);
        ui->hmpMaterialInsertBtn->setDisabled(true);
        ui->hmpMaterialEditBtn->setDisabled(true);
        ui->hmpMaterialSaveBtn->setDisabled(true);
        ui->hmpMaterialCancelBtn->setDisabled(true);
        ui->hmpTempInsertBtn->setDisabled(true);
        ui->hmpTempEditBtn->setDisabled(true);
        ui->hmpTempSaveBtn->setDisabled(true);
        ui->hmpTempCancelBtn->setDisabled(true);
        ui->hmpReserveEditBtn->setDisabled(true);
        ui->hmpReserveSaveBtn->setDisabled(true);
        ui->hmpReserveCancelBtn->setDisabled(true);
    }
    else
    {
        ui->hmpStirInfoInsertBtn->setDisabled(false);
        ui->hmpStirInfoEditBtn->setDisabled(false);
        ui->hmpStirInfoSaveBtn->setDisabled(true);
        ui->hmpStirInfoCancelBtn->setDisabled(true);
        ui->hmpSlagBeforeInsertBtn->setDisabled(false);
        ui->hmpSlagAfterInsertBtn->setDisabled(false);
        ui->hmpSlagEditBtn->setDisabled(false);
        ui->hmpSlagSaveBtn->setDisabled(true);
        ui->hmpSlagCancelBtn->setDisabled(true);
        ui->hmpMaterialInsertBtn->setDisabled(false);
        ui->hmpMaterialEditBtn->setDisabled(false);
        ui->hmpMaterialSaveBtn->setDisabled(true);
        ui->hmpMaterialCancelBtn->setDisabled(true);
        ui->hmpTempInsertBtn->setDisabled(false);
        ui->hmpTempEditBtn->setDisabled(false);
        ui->hmpTempSaveBtn->setDisabled(true);
        ui->hmpTempCancelBtn->setDisabled(true);
        ui->hmpReserveEditBtn->setDisabled(false);
        ui->hmpReserveSaveBtn->setDisabled(true);
        ui->hmpReserveCancelBtn->setDisabled(true);
    }

    desModel = new QSqlRelationalTableModel(this);
    desModel->setTable("DES_RESULT");
    desModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    desModel->setRelation(desModel->fieldIndex("SHIFT_ID"), QSqlRelation("SHIFT_MAP", "SHIFT", "SHIFT_DESC"));
    desModel->setRelation(desModel->fieldIndex("CREW_ID"), QSqlRelation("CREW_MAP", "CREW", "CREW_DESC"));
    desModel->select();
    desModel->setFilter("HM_NO='" + selectContent + "'");
    ui->hmpDesTableView->setModel(desModel);

    ui->hmpDesTableView->setItemDelegate(new ToTheMiddleDelegate);

    desModel->setHeaderData(desModel->fieldIndex("HM_NO"), Qt::Horizontal, tr("HM_NO"));
    desModel->setHeaderData(desModel->fieldIndex("PLAN_NO"), Qt::Horizontal, tr("PLAN_NO"));
    desModel->setHeaderData(desModel->fieldIndex("PRODUCTION_ID"), Qt::Horizontal, tr("PRODUCTION_ID"));
    desModel->setHeaderData(desModel->fieldIndex("ST_NO"), Qt::Horizontal, tr("ST_NO"));
    desModel->setHeaderData(desModel->fieldIndex("STATION_NO"), Qt::Horizontal, tr("STATION_NO"));
    desModel->setHeaderData(desModel->fieldIndex("SHIFT_ID"), Qt::Horizontal, tr("SHIFT_ID"));
    desModel->setHeaderData(desModel->fieldIndex("CREW_ID"), Qt::Horizontal, tr("CREW_ID"));
    desModel->setHeaderData(desModel->fieldIndex("SHIFT_DESC"), Qt::Horizontal, tr("SHIFT_DESC"));
    desModel->setHeaderData(desModel->fieldIndex("CREW_DESC"), Qt::Horizontal, tr("CREW_DESC"));
    desModel->setHeaderData(desModel->fieldIndex("TPD_NO"), Qt::Horizontal, tr("TPD_NO"));
    desModel->setHeaderData(desModel->fieldIndex("DS_CODE"), Qt::Horizontal, tr("DS_CODE"));
    desModel->setHeaderData(desModel->fieldIndex("IRON_LADLE_ID"), Qt::Horizontal, tr("IRON_LADLE_ID"));
    desModel->setHeaderData(desModel->fieldIndex("INIT_TEMP_TIME"), Qt::Horizontal, tr("INIT_TEMP_TIME"));
    desModel->setHeaderData(desModel->fieldIndex("INIT_TEMP"), Qt::Horizontal, tr("INIT_TEMP"));
    desModel->setHeaderData(desModel->fieldIndex("FINAL_TEMP_TIME"), Qt::Horizontal, tr("FINAL_TEMP_TIME"));
    desModel->setHeaderData(desModel->fieldIndex("FINAL_TEMP"), Qt::Horizontal, tr("FINAL_TEMP"));
    desModel->setHeaderData(desModel->fieldIndex("INIT_S"), Qt::Horizontal, tr("INIT_S"));
    desModel->setHeaderData(desModel->fieldIndex("FINAL_S"), Qt::Horizontal, tr("FINAL_S"));
    desModel->setHeaderData(desModel->fieldIndex("INIT_WEIGHT"), Qt::Horizontal, tr("INIT_WEIGHT"));
    desModel->setHeaderData(desModel->fieldIndex("FINAL_WEIGHT"), Qt::Horizontal, tr("FINAL_WEIGHT"));
    desModel->setHeaderData(desModel->fieldIndex("LADLE_TARE_WEIGHT"), Qt::Horizontal, tr("LADLE_TARE_WEIGHT"));
    desModel->setHeaderData(desModel->fieldIndex("HM_TIME"), Qt::Horizontal, tr("HM_TIME"));
    desModel->setHeaderData(desModel->fieldIndex("DES_TIME"), Qt::Horizontal, tr("DES_TIME"));
/*
    int i=0;
    ui->hmpDesTableView->horizontalHeader()->moveSection(ui->hmpDesTableView->horizontalHeader()->visualIndex(desModel->fieldIndex("HM_NO")),i++);
    ui->hmpDesTableView->horizontalHeader()->moveSection(ui->hmpDesTableView->horizontalHeader()->visualIndex(desModel->fieldIndex("PLAN_NO")),i++);
    ui->hmpDesTableView->horizontalHeader()->moveSection(ui->hmpDesTableView->horizontalHeader()->visualIndex(desModel->fieldIndex("PRODUCTION_ID")),i++);
    ui->hmpDesTableView->horizontalHeader()->moveSection(ui->hmpDesTableView->horizontalHeader()->visualIndex(desModel->fieldIndex("ST_NO")),i++);
    ui->hmpDesTableView->horizontalHeader()->moveSection(ui->hmpDesTableView->horizontalHeader()->visualIndex(desModel->fieldIndex("STATION_NO")),i++);
    ui->hmpDesTableView->horizontalHeader()->moveSection(ui->hmpDesTableView->horizontalHeader()->visualIndex(desModel->fieldIndex("SHIFT_ID")),i++);
    ui->hmpDesTableView->horizontalHeader()->moveSection(ui->hmpDesTableView->horizontalHeader()->visualIndex(desModel->fieldIndex("CREW_ID")),i++);
    ui->hmpDesTableView->horizontalHeader()->moveSection(ui->hmpDesTableView->horizontalHeader()->visualIndex(desModel->fieldIndex("TPD_NO")),i++);
    ui->hmpDesTableView->horizontalHeader()->moveSection(ui->hmpDesTableView->horizontalHeader()->visualIndex(desModel->fieldIndex("DS_CODE")),i++);
    ui->hmpDesTableView->horizontalHeader()->moveSection(ui->hmpDesTableView->horizontalHeader()->visualIndex(desModel->fieldIndex("IRON_LADLE_ID")),i++);
    ui->hmpDesTableView->horizontalHeader()->moveSection(ui->hmpDesTableView->horizontalHeader()->visualIndex(desModel->fieldIndex("INIT_TEMP_TIME")),i++);
    ui->hmpDesTableView->horizontalHeader()->moveSection(ui->hmpDesTableView->horizontalHeader()->visualIndex(desModel->fieldIndex("INIT_TEMP")),i++);
    ui->hmpDesTableView->horizontalHeader()->moveSection(ui->hmpDesTableView->horizontalHeader()->visualIndex(desModel->fieldIndex("FINAL_TEMP_TIME")),i++);
    ui->hmpDesTableView->horizontalHeader()->moveSection(ui->hmpDesTableView->horizontalHeader()->visualIndex(desModel->fieldIndex("FINAL_TEMP")),i++);
    ui->hmpDesTableView->horizontalHeader()->moveSection(ui->hmpDesTableView->horizontalHeader()->visualIndex(desModel->fieldIndex("INIT_S")),i++);
    ui->hmpDesTableView->horizontalHeader()->moveSection(ui->hmpDesTableView->horizontalHeader()->visualIndex(desModel->fieldIndex("FINAL_S")),i++);
    ui->hmpDesTableView->horizontalHeader()->moveSection(ui->hmpDesTableView->horizontalHeader()->visualIndex(desModel->fieldIndex("INIT_WEIGHT")),i++);
    ui->hmpDesTableView->horizontalHeader()->moveSection(ui->hmpDesTableView->horizontalHeader()->visualIndex(desModel->fieldIndex("FINAL_WEIGHT")),i++);
    ui->hmpDesTableView->horizontalHeader()->moveSection(ui->hmpDesTableView->horizontalHeader()->visualIndex(desModel->fieldIndex("LADLE_TARE_WEIGHT")),i++);
    ui->hmpDesTableView->horizontalHeader()->moveSection(ui->hmpDesTableView->horizontalHeader()->visualIndex(desModel->fieldIndex("HM_TIME")),i++);
    ui->hmpDesTableView->horizontalHeader()->moveSection(ui->hmpDesTableView->horizontalHeader()->visualIndex(desModel->fieldIndex("DES_TIME")),i++);
*/

    ui->hmpDesTableView->setColumnWidth(desModel->fieldIndex("HM_NO"),80);
    ui->hmpDesTableView->setColumnWidth(desModel->fieldIndex("PLAN_NO"),80);
    ui->hmpDesTableView->setColumnWidth(desModel->fieldIndex("PRODUCTION_ID"),80);
    ui->hmpDesTableView->setColumnWidth(desModel->fieldIndex("ST_NO"),100);
    ui->hmpDesTableView->setColumnWidth(desModel->fieldIndex("STATION_NO"),80);
    ui->hmpDesTableView->setColumnWidth(desModel->fieldIndex("SHIFT_ID"),40);
    ui->hmpDesTableView->setColumnWidth(desModel->fieldIndex("CREW_ID"),40);
    ui->hmpDesTableView->setColumnWidth(desModel->fieldIndex("SHIFT_DESC"),70);
    ui->hmpDesTableView->setColumnWidth(desModel->fieldIndex("CREW_DESC"),40);
    ui->hmpDesTableView->setColumnWidth(desModel->fieldIndex("TPD_NO"),80);
    ui->hmpDesTableView->setColumnWidth(desModel->fieldIndex("DS_CODE"),55);
    ui->hmpDesTableView->setColumnWidth(desModel->fieldIndex("IRON_LADLE_ID"),70);
    ui->hmpDesTableView->setColumnWidth(desModel->fieldIndex("INIT_TEMP_TIME"),100);
    ui->hmpDesTableView->setColumnWidth(desModel->fieldIndex("INIT_TEMP"),55);
    ui->hmpDesTableView->setColumnWidth(desModel->fieldIndex("FINAL_TEMP_TIME"),100);
    ui->hmpDesTableView->setColumnWidth(desModel->fieldIndex("FINAL_TEMP"),55);
    ui->hmpDesTableView->setColumnWidth(desModel->fieldIndex("INIT_S"),55);
    ui->hmpDesTableView->setColumnWidth(desModel->fieldIndex("FINAL_S"),55);
    ui->hmpDesTableView->setColumnWidth(desModel->fieldIndex("INIT_WEIGHT"),70);
    ui->hmpDesTableView->setColumnWidth(desModel->fieldIndex("FINAL_WEIGHT"),70);
    ui->hmpDesTableView->setColumnWidth(desModel->fieldIndex("LADLE_TARE_WEIGHT"),70);
    ui->hmpDesTableView->setColumnWidth(desModel->fieldIndex("HM_TIME"),80);
    ui->hmpDesTableView->setColumnWidth(desModel->fieldIndex("DES_TIME"),80);

    ui->hmpDesTableView->setItemDelegateForColumn(desModel->fieldIndex("INIT_TEMP_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));
    ui->hmpDesTableView->setItemDelegateForColumn(desModel->fieldIndex("FINAL_TEMP_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));

    ui->hmpDesTableView->setColumnHidden(desModel->fieldIndex("STATION_NO"),true);
    ui->hmpDesTableView->setColumnHidden(desModel->fieldIndex("HM_NO"),true);

    ui->hmpDesTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->hmpDesTableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    //设置指定列为只读
    //ui->hmpDesTableView->setItemDelegateForColumn(2,new ReadOnlyDelegate(this));

    stirInfoModel = new QSqlRelationalTableModel(this);
    stirInfoModel->setTable("STIR_INFO");
    stirInfoModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    stirInfoModel->setFilter("HM_NO='" + selectContent + "'");
    stirInfoModel->select();
    if (stirInfoModel->rowCount() > 0)
    {
        ui->hmpStirInfoInsertBtn->setDisabled(true);
    }
    else
    {
        ui->hmpStirInfoInsertBtn->setDisabled(false);
    }

    ui->hmpStirInfoTableView->setModel(stirInfoModel);

    stirInfoModel->setHeaderData(stirInfoModel->fieldIndex("HM_NO"), Qt::Horizontal, tr("HM_NO"));
    stirInfoModel->setHeaderData(stirInfoModel->fieldIndex("STIR_NO"), Qt::Horizontal, tr("STIR_NO"));
    stirInfoModel->setHeaderData(stirInfoModel->fieldIndex("STIR_TIMES"), Qt::Horizontal, tr("STIR_TIMES"));
    stirInfoModel->setHeaderData(stirInfoModel->fieldIndex("LIQUID_LEVEL"), Qt::Horizontal, tr("LIQUID_LEVEL"));
    stirInfoModel->setHeaderData(stirInfoModel->fieldIndex("STIR_SPEED_LOW"), Qt::Horizontal, tr("STIR_SPEED_LOW"));
    stirInfoModel->setHeaderData(stirInfoModel->fieldIndex("STIR_SPEED_MEDIUM"), Qt::Horizontal, tr("STIR_SPEED_MEDIUM"));
    stirInfoModel->setHeaderData(stirInfoModel->fieldIndex("STIR_SPEED_HIGH"), Qt::Horizontal, tr("STIR_SPEED_HIGH"));
    stirInfoModel->setHeaderData(stirInfoModel->fieldIndex("HIGH_SPEED_TIME"), Qt::Horizontal, tr("HIGH_SPEED_TIME"));

    ui->hmpStirInfoTableView->setColumnWidth(stirInfoModel->fieldIndex("HM_NO"),100);
    ui->hmpStirInfoTableView->setColumnWidth(stirInfoModel->fieldIndex("STIR_NO"),100);
    ui->hmpStirInfoTableView->setColumnWidth(stirInfoModel->fieldIndex("STIR_TIMES"),100);
    ui->hmpStirInfoTableView->setColumnWidth(stirInfoModel->fieldIndex("LIQUID_LEVEL"),100);
    ui->hmpStirInfoTableView->setColumnWidth(stirInfoModel->fieldIndex("STIR_SPEED_LOW"),100);
    ui->hmpStirInfoTableView->setColumnWidth(stirInfoModel->fieldIndex("STIR_SPEED_MEDIUM"),100);
    ui->hmpStirInfoTableView->setColumnWidth(stirInfoModel->fieldIndex("STIR_SPEED_HIGH"),100);
    ui->hmpStirInfoTableView->setColumnWidth(stirInfoModel->fieldIndex("HIGH_SPEED_TIME"),100);

    ui->hmpStirInfoTableView->setItemDelegate(new ToTheMiddleDelegate);

    ui->hmpStirInfoTableView->setColumnHidden(stirInfoModel->fieldIndex("HM_NO"),true);

    ui->hmpStirInfoTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->hmpStirInfoTableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    slagModel = new QSqlRelationalTableModel(this);
    slagModel->setTable("SLAG_RESULT");
    slagModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    slagModel->setRelation(slagModel->fieldIndex("SLAG_TYPE"), QSqlRelation("SLAG_MAP", "SLAG_TYPE", "SLAG_DESC"));
    slagModel->select();
    slagModel->setFilter("HM_NO='" + selectContent + "'");

    QSqlQuery slagBeforeQuery("SELECT * FROM SLAG_RESULT WHERE HM_NO = '" + ui->hmpHMNoComboBox->currentText() +"' AND SLAG_TYPE = 1");
    QSqlQuery slagAfterQuery("SELECT * FROM SLAG_RESULT WHERE HM_NO = '" + ui->hmpHMNoComboBox->currentText() +"' AND SLAG_TYPE = 2");
    if (slagBeforeQuery.next())
    {
        ui->hmpSlagBeforeInsertBtn->setDisabled(true);
    }
    else
    {
        ui->hmpSlagBeforeInsertBtn->setDisabled(false);
    }

    if (slagAfterQuery.next())
    {
        ui->hmpSlagAfterInsertBtn->setDisabled(true);
    }
    else
    {
        ui->hmpSlagAfterInsertBtn->setDisabled(false);
    }

    ui->hmpSlagTableView->setModel(slagModel);
    slagModel->setHeaderData(slagModel->fieldIndex("HM_NO"), Qt::Horizontal, tr("HM_NO"));
    slagModel->setHeaderData(slagModel->fieldIndex("SLAG_TYPE"), Qt::Horizontal, tr("SLAG_TYPE"));
    slagModel->setHeaderData(slagModel->fieldIndex("SLAG_DESC"), Qt::Horizontal, tr("SLAG_DESC"));
    slagModel->setHeaderData(slagModel->fieldIndex("SLAG_START_TIME"), Qt::Horizontal, tr("SLAG_START_TIME"));
    slagModel->setHeaderData(slagModel->fieldIndex("SLAG_END_TIME"), Qt::Horizontal, tr("SLAG_END_TIME"));
    slagModel->setHeaderData(slagModel->fieldIndex("SLAG_TIME"), Qt::Horizontal, tr("SLAG_TIME"));
    //slagModel->setHeaderData(slagModel->fieldIndex("FINAL_TEMP_TIME"), Qt::Horizontal, tr("FINAL_TEMP_TIME"));
    //slagModel->setHeaderData(slagModel->fieldIndex("FINAL_TEMP"), Qt::Horizontal, tr("FINAL_TEMP"));
    slagModel->setHeaderData(slagModel->fieldIndex("SLAG_WEIGHT"), Qt::Horizontal, tr("SLAG_WEIGHT"));

    ui->hmpSlagTableView->setColumnWidth(slagModel->fieldIndex("HM_NO"),100);
    ui->hmpSlagTableView->setColumnWidth(slagModel->fieldIndex("SLAG_TYPE"),100);
    ui->hmpSlagTableView->setColumnWidth(slagModel->fieldIndex("SLAG_START_TIME"),100);
    ui->hmpSlagTableView->setColumnWidth(slagModel->fieldIndex("SLAG_END_TIME"),100);
    ui->hmpSlagTableView->setColumnWidth(slagModel->fieldIndex("SLAG_TIME"),100);
    //ui->hmpSlagTableView->setColumnWidth(slagModel->fieldIndex("FINAL_TEMP_TIME"),100);
    //ui->hmpSlagTableView->setColumnWidth(slagModel->fieldIndex("FINAL_TEMP"),100);
    ui->hmpSlagTableView->setColumnWidth(slagModel->fieldIndex("SLAG_WEIGHT"),100);

    ui->hmpSlagTableView->setItemDelegateForColumn(slagModel->fieldIndex("SLAG_START_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));
    ui->hmpSlagTableView->setItemDelegateForColumn(slagModel->fieldIndex("SLAG_END_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));

    ui->hmpSlagTableView->setItemDelegateForColumn(1,new ReadOnlyDelegate(this));

    ui->hmpSlagTableView->setItemDelegate(new ToTheMiddleDelegate);

    ui->hmpSlagTableView->setColumnHidden(slagModel->fieldIndex("HM_NO"),true);
    ui->hmpSlagTableView->setColumnHidden(slagModel->fieldIndex("ID"),true);
    ui->hmpSlagTableView->setColumnHidden(slagModel->fieldIndex("FINAL_TEMP_TIME"),true);
    ui->hmpSlagTableView->setColumnHidden(slagModel->fieldIndex("FINAL_TEMP"),true);

    ui->hmpSlagTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->hmpSlagTableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    materialModel = new QSqlRelationalTableModel(this);
    materialModel->setTable("MATERIAL_RESULT");
    materialModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    materialModel->select();
    materialModel->setFilter("HM_NO='" + selectContent + "'");
    ui->hmpMaterialTableView->setModel(materialModel);

    materialModel->setHeaderData(materialModel->fieldIndex("HM_NO"), Qt::Horizontal, tr("HM_NO"));
    materialModel->setHeaderData(materialModel->fieldIndex("START_TIME"), Qt::Horizontal, tr("START_TIME"));
    materialModel->setHeaderData(materialModel->fieldIndex("END_TIME"), Qt::Horizontal, tr("END_TIME"));
    materialModel->setHeaderData(materialModel->fieldIndex("MATERIAL_WEIGHT"), Qt::Horizontal, tr("MATERIAL_WEIGHT"));

    ui->hmpMaterialTableView->setColumnWidth(materialModel->fieldIndex("HM_NO"),100);
    ui->hmpMaterialTableView->setColumnWidth(materialModel->fieldIndex("START_TIME"),100);
    ui->hmpMaterialTableView->setColumnWidth(materialModel->fieldIndex("END_TIME"),100);
    ui->hmpMaterialTableView->setColumnWidth(materialModel->fieldIndex("MATERIAL_WEIGHT"),80);

    ui->hmpMaterialTableView->setItemDelegateForColumn(materialModel->fieldIndex("START_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));
    ui->hmpMaterialTableView->setItemDelegateForColumn(materialModel->fieldIndex("END_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));

    ui->hmpMaterialTableView->setItemDelegate(new ToTheMiddleDelegate);

    ui->hmpMaterialTableView->setColumnHidden(materialModel->fieldIndex("HM_NO"),true);

    ui->hmpMaterialTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->hmpMaterialTableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    tempModel = new QSqlRelationalTableModel(this);
    tempModel->setTable("TEMP_RESULT");
    tempModel->setRelation(tempModel->fieldIndex("TEMP_FLAG"), QSqlRelation("TEMP_MAP", "TEMP_FLAG", "FLAG_DESC"));
    tempModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tempModel->select();
    tempModel->setFilter("HM_NO='" + selectContent + "'");
    ui->hmpTempTableView->setModel(tempModel);

    tempModel->setHeaderData(tempModel->fieldIndex("HM_NO"), Qt::Horizontal, tr("HM_NO"));
    tempModel->setHeaderData(tempModel->fieldIndex("TEMP"), Qt::Horizontal, tr("TEMP"));
    tempModel->setHeaderData(tempModel->fieldIndex("MEASURE_TIME"), Qt::Horizontal, tr("MEASURE_TIME"));
    tempModel->setHeaderData(tempModel->fieldIndex("TEMP_FLAG"), Qt::Horizontal, tr("TEMP_FLAG"));
    tempModel->setHeaderData(tempModel->fieldIndex("FLAG_DESC"), Qt::Horizontal, tr("FLAG_DESC"));

    int i=0;
    ui->hmpTempTableView->horizontalHeader()->moveSection(ui->hmpTempTableView->horizontalHeader()->visualIndex(tempModel->fieldIndex("HM_NO")),i++);
    ui->hmpTempTableView->horizontalHeader()->moveSection(ui->hmpTempTableView->horizontalHeader()->visualIndex(tempModel->fieldIndex("MEASURE_TIME")),i++);
    ui->hmpTempTableView->horizontalHeader()->moveSection(ui->hmpTempTableView->horizontalHeader()->visualIndex(tempModel->fieldIndex("TEMP")),i++);
    ui->hmpTempTableView->horizontalHeader()->moveSection(ui->hmpTempTableView->horizontalHeader()->visualIndex(tempModel->fieldIndex("TEMP_FLAG")),i++);
    ui->hmpTempTableView->horizontalHeader()->moveSection(ui->hmpTempTableView->horizontalHeader()->visualIndex(tempModel->fieldIndex("FLAG_DESC")),i++);

    ui->hmpTempTableView->setColumnWidth(tempModel->fieldIndex("HM_NO"),100);
    ui->hmpTempTableView->setColumnWidth(tempModel->fieldIndex("TEMP"),70);
    ui->hmpTempTableView->setColumnWidth(tempModel->fieldIndex("MEASURE_TIME"),100);
    ui->hmpTempTableView->setColumnWidth(tempModel->fieldIndex("TEMP_FLAG"),70);
    ui->hmpTempTableView->setColumnWidth(tempModel->fieldIndex("FLAG_DESC"),100);

    ui->hmpTempTableView->setItemDelegateForColumn(tempModel->fieldIndex("MEASURE_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));

    ui->hmpTempTableView->setItemDelegate(new ToTheMiddleDelegate);

    ui->hmpTempTableView->setColumnHidden(tempModel->fieldIndex("HM_NO"),true);

    ui->hmpTempTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->hmpTempTableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    eventModel = new QSqlRelationalTableModel(this);
    eventModel->setTable("EVENT_TIME");
    eventModel->setFilter("HM_NO='" + selectContent + "'");
    eventModel->setRelation(eventModel->fieldIndex("EVENT_NO"), QSqlRelation("EVENT_MAP", "EVENT_NO", "EVENT_DESC"));
    eventModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    eventModel->select();
    ui->hmpEventTableView->setModel(eventModel);

    eventModel->setHeaderData(eventModel->fieldIndex("HM_NO"), Qt::Horizontal, tr("HM_NO"));
    eventModel->setHeaderData(eventModel->fieldIndex("EVENT_TIME"), Qt::Horizontal, tr("EVENT_TIME"));
    eventModel->setHeaderData(eventModel->fieldIndex("EVENT_DESC"), Qt::Horizontal, tr("EVENT_DESC"));

    ui->hmpEventTableView->setColumnWidth(eventModel->fieldIndex("HM_NO"),150);
    ui->hmpEventTableView->setColumnWidth(eventModel->fieldIndex("EVENT_TIME"),120);
    ui->hmpEventTableView->setColumnWidth(eventModel->fieldIndex("EVENT_DESC"),90);

    i=0;
    ui->hmpEventTableView->horizontalHeader()->moveSection(ui->hmpEventTableView->horizontalHeader()->visualIndex(eventModel->fieldIndex("HM_NO")),i++);
    ui->hmpEventTableView->horizontalHeader()->moveSection(ui->hmpEventTableView->horizontalHeader()->visualIndex(eventModel->fieldIndex("EVENT_DESC")),i++);
    ui->hmpEventTableView->horizontalHeader()->moveSection(ui->hmpEventTableView->horizontalHeader()->visualIndex(eventModel->fieldIndex("EVENT_TIME")),i++);

    ui->hmpEventTableView->setItemDelegateForColumn(eventModel->fieldIndex("MEASURE_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));

    ui->hmpEventTableView->setColumnHidden(eventModel->fieldIndex("HM_NO"),true);

    ui->hmpEventTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->hmpEventTableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    reserveModel = new QSqlRelationalTableModel(this);
    reserveModel->setTable("RESERVE_IRON");
    reserveModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    reserveModel->select();
    reserveModel->setFilter("HM_NO='" + selectContent + "'");
    QReserveTableModel* reserveTableModel = new QReserveTableModel(reserveModel,this);

    i=0;
    reserveTableModel->setHeaderData(reserveModel->fieldIndex("HM_NO"), Qt::Vertical, tr("HM_NO"));
    reserveTableModel->setHeaderData(reserveModel->fieldIndex("IRON_ORDER_WEIGHT"), Qt::Vertical, tr("IRON_ORDER_WEIGHT"));
    reserveTableModel->setHeaderData(reserveModel->fieldIndex("IRON_NET_WEIGHT"), Qt::Vertical, tr("IRON_NET_WEIGHT"));
    reserveTableModel->setHeaderData(reserveModel->fieldIndex("REMAININGIRON_WEIGHT"), Qt::Vertical, tr("REMAININGIRON_WEIGHT"));
    reserveTableModel->setHeaderData(reserveModel->fieldIndex("IRON_CHARGE_WEIGHT"), Qt::Vertical, tr("IRON_CHARGE_WEIGHT"));

    ui->hmpReserveTableView->setModel(reserveTableModel);
    ui->hmpReserveTableView->verticalHeader()->setVisible(true);
    ui->hmpReserveTableView->horizontalHeader()->setVisible(false);
    ui->hmpReserveTableView->setRowHidden(reserveModel->fieldIndex("HM_NO"),true);
    ui->hmpReserveTableView->setRowHidden(reserveModel->fieldIndex("IRON_NET_WEIGHT"),true);

    i=0;
    ui->hmpReserveTableView->setColumnWidth(i++,150);
    ui->hmpReserveTableView->setColumnWidth(i++,150);


    ui->hmpReserveTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->hmpReserveTableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    //
    ironSteelModel = new QSqlRelationalTableModel(this);
    ironSteelModel->setTable("IRON_STEEL");
    ironSteelModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ironSteelModel->select();
    ironSteelModel->setFilter("HM_NO='" + selectContent + "'");
    QReserveTableModel* ironSteelTableModel = new QReserveTableModel(ironSteelModel,this);

    i=0;
    ironSteelTableModel->setHeaderData(ironSteelModel->fieldIndex("PLAN_NO"), Qt::Vertical, tr("PLAN_NO"));
    ironSteelTableModel->setHeaderData(ironSteelModel->fieldIndex("PRODUCTION_ID"), Qt::Vertical, tr("PRODUCTION_ID"));
    ironSteelTableModel->setHeaderData(ironSteelModel->fieldIndex("HEAT_ID"), Qt::Vertical, tr("HEAT_ID"));
    ironSteelTableModel->setHeaderData(ironSteelModel->fieldIndex("STATION_NO"), Qt::Vertical, tr("STATION_NO"));
    ironSteelTableModel->setHeaderData(ironSteelModel->fieldIndex("IRON_LADLE_ID"), Qt::Vertical, tr("IRON_LADLE_ID"));
    ironSteelTableModel->setHeaderData(ironSteelModel->fieldIndex("HM_NO"), Qt::Vertical, tr("HM_NO"));

    ui->ironSteelTableView->setModel(ironSteelTableModel);
    ui->ironSteelTableView->verticalHeader()->setVisible(true);
    ui->ironSteelTableView->horizontalHeader()->setVisible(false);
    ui->ironSteelTableView->setRowHidden(ironSteelModel->fieldIndex("HM_NO"),true);

    i=0;
    ui->ironSteelTableView->setColumnWidth(i++,150);
    ui->ironSteelTableView->setColumnWidth(i++,150);


    ui->ironSteelTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->ironSteelTableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    //
    QString ladleTareWeight,initGrossWeight,finalGrossWeight,dsCode,initS,finalS;
    if (desModel->rowCount() > 0)
    {
        ladleTareWeight = desModel->record(0).value("LADLE_TARE_WEIGHT").toString();
        initGrossWeight = QString::number((desModel->record(0).value("INIT_WEIGHT").toInt()+desModel->record(0).value("LADLE_TARE_WEIGHT").toInt()),10);
        finalGrossWeight = QString::number((desModel->record(0).value("FINAL_WEIGHT").toInt()+desModel->record(0).value("LADLE_TARE_WEIGHT").toInt()),10);
        dsCode = desModel->record(0).value("DS_CODE").toString();
        initS = desModel->record(0).value("INIT_S").toString();
        finalS = desModel->record(0).value("FINAL_S").toString();
    }
    else
    {
        ladleTareWeight = "";
        initGrossWeight = "";
        finalGrossWeight = "";
        dsCode = "";
        initS = "";
        finalS = "";
    }

    ui->ladleTareWeightLineEdit->setText(ladleTareWeight);
    ui->initGrossWeightLineEdit->setText(initGrossWeight);
    ui->finalGrossWeightLineEdit->setText(finalGrossWeight);
    if(dsCode=="1")
    {
        ui->dsCodeLineEdit->setText(tr("YES"));
    }
    else
    {
        ui->dsCodeLineEdit->setText(tr("NO"));
    }
    ui->initSLineEdit->setText(initS);
    ui->finalSLineEdit->setText(finalS);
}

void PageHmp::generateReport()
{
    QSqlQuery eventQuery("SELECT * FROM EVENT_TIME WHERE HM_NO = '" + ui->hmpHMNoComboBox->currentText() +"' AND EVENT_NO = 8");
    if (eventQuery.next())
    {
        QString cmd = QString("D:\\Report ") + ui->hmpHMNoComboBox->currentText();
        system(cmd.toStdString().c_str());
    }
    else
    {
        QMessageBox::critical(0, qApp->tr("Cannot generate report"),
            qApp->tr("预处理尚未完成包离开，不能生成报表")
                     , QMessageBox::Ok);
    }
}

void PageHmp::insertStirInfo()
{
    QSqlQuery insertStirInfoQuery("INSERT INTO STIR_INFO(HM_NO) VALUES('"+ui->hmpHMNoComboBox->currentText()+"')");

    hmNoSelected(ui->hmpHMNoComboBox->currentText());
}

void PageHmp::editStirInfo()
{
    ui->hmpStirInfoInsertBtn->setDisabled(true);
    ui->hmpStirInfoEditBtn->setDisabled(true);
    ui->hmpStirInfoSaveBtn->setDisabled(false);
    ui->hmpStirInfoCancelBtn->setDisabled(false);
    ui->hmpHMNoComboBox->setDisabled(true);
    ui->hmpStirInfoTableView->setEditTriggers(QAbstractItemView::AllEditTriggers);

    stirInfoModifyFlag=1;
}

void PageHmp::saveStirInfo()
{
    ui->hmpStirInfoInsertBtn->setDisabled(false);
    ui->hmpStirInfoEditBtn->setDisabled(false);
    ui->hmpStirInfoSaveBtn->setDisabled(true);
    ui->hmpStirInfoCancelBtn->setDisabled(true);
    ui->hmpHMNoComboBox->setDisabled(false);
    ui->hmpStirInfoTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    if (stirInfoModel->submitAll())
    {
        stirInfoModel->database().commit();
    }

    hmNoSelected(ui->hmpHMNoComboBox->currentText());

    stirInfoModifyFlag=0;
}

void PageHmp::cancelStirInfo()
{
    ui->hmpStirInfoInsertBtn->setDisabled(false);
    ui->hmpStirInfoEditBtn->setDisabled(false);
    ui->hmpStirInfoSaveBtn->setDisabled(true);
    ui->hmpStirInfoCancelBtn->setDisabled(true);
    ui->hmpHMNoComboBox->setDisabled(false);
    ui->hmpStirInfoTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    stirInfoModel->database().rollback();

    hmNoSelected(ui->hmpHMNoComboBox->currentText());
    stirInfoModifyFlag=0;
}

void PageHmp::insertSlagBefore()
{
    QSqlQuery insertSlagBeforeQuery("INSERT INTO SLAG_RESULT(HM_NO,SLAG_TYPE,SLAG_START_TIME,SLAG_END_TIME) VALUES('"+ui->hmpHMNoComboBox->currentText()+"','1',SYSDATE,SYSDATE)");

    hmNoSelected(ui->hmpHMNoComboBox->currentText());
}

void PageHmp::insertSlagAfter()
{
    QSqlQuery insertSlagAfterQuery("INSERT INTO SLAG_RESULT(HM_NO,SLAG_TYPE,SLAG_START_TIME,SLAG_END_TIME) VALUES('"+ui->hmpHMNoComboBox->currentText()+"','2',SYSDATE,SYSDATE)");

    hmNoSelected(ui->hmpHMNoComboBox->currentText());
}

void PageHmp::editSlag()
{
    QSqlQuery slagBeforeQuery("SELECT * FROM SLAG_RESULT WHERE HM_NO = '" + ui->hmpHMNoComboBox->currentText() +"' AND SLAG_TYPE = 1");
    QSqlQuery slagAfterQuery("SELECT * FROM SLAG_RESULT WHERE HM_NO = '" + ui->hmpHMNoComboBox->currentText() +"' AND SLAG_TYPE = 2");
    if (slagBeforeQuery.next())
    {
        ui->hmpSlagBeforeInsertBtn->setDisabled(true);
    }
    else
    {
        ui->hmpSlagBeforeInsertBtn->setDisabled(false);
    }

    if (slagAfterQuery.next())
    {
        ui->hmpSlagAfterInsertBtn->setDisabled(true);
    }
    else
    {
        ui->hmpSlagAfterInsertBtn->setDisabled(false);
    }

    ui->hmpSlagEditBtn->setDisabled(true);
    ui->hmpSlagSaveBtn->setDisabled(false);
    ui->hmpSlagCancelBtn->setDisabled(false);
    ui->hmpHMNoComboBox->setDisabled(true);
    ui->hmpSlagTableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
}

void PageHmp::saveSlag()
{
    QSqlQuery slagBeforeQuery("SELECT * FROM SLAG_RESULT WHERE HM_NO = '" + ui->hmpHMNoComboBox->currentText() +"' AND SLAG_TYPE = 1");
    QSqlQuery slagAfterQuery("SELECT * FROM SLAG_RESULT WHERE HM_NO = '" + ui->hmpHMNoComboBox->currentText() +"' AND SLAG_TYPE = 2");
    if (slagBeforeQuery.next())
    {
        ui->hmpSlagBeforeInsertBtn->setDisabled(true);
    }
    else
    {
        ui->hmpSlagBeforeInsertBtn->setDisabled(false);
    }

    if (slagAfterQuery.next())
    {
        ui->hmpSlagAfterInsertBtn->setDisabled(true);
    }
    else
    {
        ui->hmpSlagAfterInsertBtn->setDisabled(false);
    }

    ui->hmpSlagEditBtn->setDisabled(false);
    ui->hmpSlagSaveBtn->setDisabled(true);
    ui->hmpSlagCancelBtn->setDisabled(true);
    ui->hmpHMNoComboBox->setDisabled(false);
    ui->hmpSlagTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    if (slagModel->submitAll())
    {
        slagModel->database().commit();
    }

    hmNoSelected(ui->hmpHMNoComboBox->currentText());

    QSqlQuery desFinishedQuery("SELECT EVENT_TIME FROM EVENT_TIME WHERE HM_NO = '" + ui->hmpHMNoComboBox->currentText() + "' AND EVENT_NO= 8");
    if (desFinishedQuery.next())
    {
        QSqlQuery calcAtLastQuery;
        calcAtLastQuery.prepare("CALL CALC_AT_LAST('" + ui->hmpHMNoComboBox->currentText() + "')");
        calcAtLastQuery.exec();
    }
}

void PageHmp::cancelSlag()
{
    QSqlQuery slagBeforeQuery("SELECT * FROM SLAG_RESULT WHERE HM_NO = '" + ui->hmpHMNoComboBox->currentText() +"' AND SLAG_TYPE = 1");
    QSqlQuery slagAfterQuery("SELECT * FROM SLAG_RESULT WHERE HM_NO = '" + ui->hmpHMNoComboBox->currentText() +"' AND SLAG_TYPE = 2");
    if (slagBeforeQuery.next())
    {
        ui->hmpSlagBeforeInsertBtn->setDisabled(true);
    }
    else
    {
        ui->hmpSlagBeforeInsertBtn->setDisabled(false);
    }

    if (slagAfterQuery.next())
    {
        ui->hmpSlagAfterInsertBtn->setDisabled(true);
    }
    else
    {
        ui->hmpSlagAfterInsertBtn->setDisabled(false);
    }

    ui->hmpSlagEditBtn->setDisabled(false);
    ui->hmpSlagSaveBtn->setDisabled(true);
    ui->hmpSlagCancelBtn->setDisabled(true);
    ui->hmpHMNoComboBox->setDisabled(false);
    ui->hmpSlagTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    slagModel->database().rollback();

    hmNoSelected(ui->hmpHMNoComboBox->currentText());
}

void PageHmp::insertMaterial()
{
    QSqlQuery insertMaterialQuery("INSERT INTO MATERIAL_RESULT(HM_NO,START_TIME,END_TIME) VALUES('"+ui->hmpHMNoComboBox->currentText()+"',SYSDATE,SYSDATE)");

    hmNoSelected(ui->hmpHMNoComboBox->currentText());
}

void PageHmp::editMaterial()
{
    ui->hmpMaterialInsertBtn->setDisabled(true);
    ui->hmpMaterialEditBtn->setDisabled(true);
    ui->hmpMaterialSaveBtn->setDisabled(false);
    ui->hmpMaterialCancelBtn->setDisabled(false);
    ui->hmpHMNoComboBox->setDisabled(true);
    ui->hmpMaterialTableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
    materialModifyFlag=1;
}

void PageHmp::saveMaterial()
{
    ui->hmpMaterialInsertBtn->setDisabled(false);
    ui->hmpMaterialEditBtn->setDisabled(false);
    ui->hmpMaterialSaveBtn->setDisabled(true);
    ui->hmpMaterialCancelBtn->setDisabled(true);
    ui->hmpHMNoComboBox->setDisabled(false);
    ui->hmpMaterialTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    if (materialModel->submitAll())
    {
        materialModel->database().commit();
    }

    hmNoSelected(ui->hmpHMNoComboBox->currentText());
    materialModifyFlag=0;

    QSqlQuery desFinishedQuery("SELECT EVENT_TIME FROM EVENT_TIME WHERE HM_NO = '" + ui->hmpHMNoComboBox->currentText() + "' AND EVENT_NO= 8");
    if (desFinishedQuery.next())
    {
        QSqlQuery calcAtLastQuery;
        calcAtLastQuery.prepare("CALL CALC_AT_LAST('" + ui->hmpHMNoComboBox->currentText() + "')");
        calcAtLastQuery.exec();
    }
}

void PageHmp::cancelMaterial()
{
    ui->hmpMaterialInsertBtn->setDisabled(false);
    ui->hmpMaterialEditBtn->setDisabled(false);
    ui->hmpMaterialSaveBtn->setDisabled(true);
    ui->hmpMaterialCancelBtn->setDisabled(true);
    ui->hmpHMNoComboBox->setDisabled(false);
    ui->hmpMaterialTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    materialModel->database().rollback();

    hmNoSelected(ui->hmpHMNoComboBox->currentText());
    materialModifyFlag=0;
}

void PageHmp::insertTemp()
{
    QSqlQuery insertTempQuery("INSERT INTO TEMP_RESULT(HM_NO,MEASURE_TIME) VALUES('"+ui->hmpHMNoComboBox->currentText()+"',SYSDATE)");

    QSqlQuery setTempFlagQuery;
    setTempFlagQuery.prepare("CALL SET_TEMP_FLAG('" + ui->hmpHMNoComboBox->currentText() + "')");
    setTempFlagQuery.exec();

    hmNoSelected(ui->hmpHMNoComboBox->currentText());
}

void PageHmp::editTemp()
{
    ui->hmpTempInsertBtn->setDisabled(true);
    ui->hmpTempEditBtn->setDisabled(true);
    ui->hmpTempSaveBtn->setDisabled(false);
    ui->hmpTempCancelBtn->setDisabled(false);
    ui->hmpHMNoComboBox->setDisabled(true);
    ui->hmpTempTableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
}

void PageHmp::saveTemp()
{
    ui->hmpTempInsertBtn->setDisabled(false);
    ui->hmpTempEditBtn->setDisabled(false);
    ui->hmpTempSaveBtn->setDisabled(true);
    ui->hmpTempCancelBtn->setDisabled(true);
    ui->hmpHMNoComboBox->setDisabled(false);
    ui->hmpTempTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    if (tempModel->submitAll())
    {
        tempModel->database().commit();
    }

    QSqlQuery setTempFlagQuery;
    setTempFlagQuery.prepare("CALL SET_TEMP_FLAG('" + ui->hmpHMNoComboBox->currentText() + "')");
    setTempFlagQuery.exec();

    hmNoSelected(ui->hmpHMNoComboBox->currentText());

    QSqlQuery desFinishedQuery("SELECT EVENT_TIME FROM EVENT_TIME WHERE HM_NO = '" + ui->hmpHMNoComboBox->currentText() + "' AND EVENT_NO= 8");
    if (desFinishedQuery.next())
    {
        QSqlQuery calcAtLastQuery;
        calcAtLastQuery.prepare("CALL CALC_AT_LAST('" + ui->hmpHMNoComboBox->currentText() + "')");
        calcAtLastQuery.exec();
    }
}

void PageHmp::cancelTemp()
{
    ui->hmpTempInsertBtn->setDisabled(false);
    ui->hmpTempEditBtn->setDisabled(false);
    ui->hmpTempSaveBtn->setDisabled(true);
    ui->hmpTempCancelBtn->setDisabled(true);
    ui->hmpHMNoComboBox->setDisabled(false);
    ui->hmpTempTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tempModel->database().rollback();

    hmNoSelected(ui->hmpHMNoComboBox->currentText());
}

void PageHmp::editReserve()
{
    ui->hmpReserveEditBtn->setDisabled(true);
    ui->hmpReserveSaveBtn->setDisabled(false);
    ui->hmpReserveCancelBtn->setDisabled(false);
    ui->hmpHMNoComboBox->setDisabled(true);
    ui->hmpReserveTableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
}

void PageHmp::saveReserve()
{
    ui->hmpReserveEditBtn->setDisabled(false);
    ui->hmpReserveSaveBtn->setDisabled(true);
    ui->hmpReserveCancelBtn->setDisabled(true);
    ui->hmpHMNoComboBox->setDisabled(false);
    ui->hmpReserveTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    if (reserveModel->submitAll())
    {
        reserveModel->database().commit();
    }

    hmNoSelected(ui->hmpHMNoComboBox->currentText());

    QSqlQuery sendReserveIronQuery;
    sendReserveIronQuery.prepare("DECLARE RET NUMBER; BEGIN RET:=PAKTEST.calc('L2_COM',4,'c4@0000,c6@D1BM06,c30@ ,c10@"+ui->hmpHMNoComboBox->currentText()+",i4@0',10); END;");
    sendReserveIronQuery.exec();

}

void PageHmp::cancelReserve()
{
    ui->hmpReserveEditBtn->setDisabled(false);
    ui->hmpReserveSaveBtn->setDisabled(true);
    ui->hmpReserveCancelBtn->setDisabled(true);
    ui->hmpHMNoComboBox->setDisabled(false);
    ui->hmpReserveTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    reserveModel->database().rollback();

    hmNoSelected(ui->hmpHMNoComboBox->currentText());
}

void PageHmp::refresh()
{
    QStringList hmNoList;
    QSqlQuery hmNoQuery("SELECT HM_NO FROM V_DES_RESULT WHERE STATION_NO='D"+stationNo+"' ORDER BY EVENT_TIME DESC");
    while (hmNoQuery.next())
    {
        QString hmNo = hmNoQuery.value(0).toString();
        hmNoList.append(hmNo);
    }

    if (ui->hmpHMNoComboBox->count() != hmNoList.count())
    {
        ui->hmpHMNoComboBox->clear();
        ui->hmpHMNoComboBox->addItems(hmNoList);
        ui->hmpHMNoComboBox->setDisabled(false);
    }

    desModel->select();
    eventModel->select();
    ironSteelModel->select();
    if(stirInfoModifyFlag==0)
    {
        stirInfoModel->select();
    }
    if(materialModifyFlag==0)
    {
        materialModel->select();
    }

    if (stirInfoModel->rowCount() > 0)
    {
        ui->hmpStirInfoInsertBtn->setDisabled(true);
    }
    else
    {
        ui->hmpStirInfoInsertBtn->setDisabled(false);
    }
}

void PageHmp::sendResult()
{
    QSqlQuery desFinishedQuery("SELECT EVENT_TIME FROM EVENT_TIME WHERE HM_NO = '" + ui->hmpHMNoComboBox->currentText() + "' AND EVENT_NO= 8");
    if (desFinishedQuery.next())
    {
        QSqlQuery sendDesResultQuery;
        sendDesResultQuery.prepare("DECLARE RET NUMBER; BEGIN RET:=PAKTEST.calc('L2_COM',4,'c4@0000,c6@D1BM04,c30@ ,c10@"+ui->hmpHMNoComboBox->currentText()+",i4@0',10); END;");
        sendDesResultQuery.exec();

        QSqlQuery sendSlagResultQuery;
        sendSlagResultQuery.prepare("DECLARE RET NUMBER; BEGIN RET:=PAKTEST.calc('L2_COM',4,'c4@0000,c6@D1BM03,c30@ ,c10@"+ui->hmpHMNoComboBox->currentText()+",i4@0',10); END;");
        sendSlagResultQuery.exec();

        QSqlQuery sendConsumptionQuery;
        sendConsumptionQuery.prepare("DECLARE RET NUMBER; BEGIN RET:=PAKTEST.calc('L2_COM',4,'c4@0000,c6@D1BM07,c30@ ,c10@"+ui->hmpHMNoComboBox->currentText()+",i4@0',10); END;");
        sendConsumptionQuery.exec();
/*
        QSqlQuery sendFinalEleQuery;
        sendFinalEleQuery.prepare("DECLARE RET NUMBER; BEGIN RET:=PAKTEST.calc('L2_COM',4,'c4@0000,c6@D1BM08,c30@ ,c10@"+ui->hmpHMNoComboBox->currentText()+",i4@0',10); END;");
        sendFinalEleQuery.exec();
*/
    }
    else
    {
        QMessageBox::critical(0, qApp->tr("Error"),
            qApp->tr("预处理尚未完成包离开，不能发送实绩")
                     , QMessageBox::Ok);
    }
}

void PageHmp::saveWeight()
{
    QSqlQuery saveDesResultWgtQuery("UPDATE DES_RESULT SET LADLE_TARE_WEIGHT=" + ui->ladleTareWeightLineEdit->text() +
                                    ",INIT_WEIGHT=" + ui->initGrossWeightLineEdit->text() + "-" + ui->ladleTareWeightLineEdit->text() +
                                    ",FINAL_WEIGHT=" + ui->finalGrossWeightLineEdit->text() + "-" + ui->ladleTareWeightLineEdit->text() +
                                    " WHERE HM_NO='" + ui->hmpHMNoComboBox->currentText()+"'");
    QSqlQuery saveLadleResultWgtQuery("UPDATE LADLE_RESULT SET TARE_WGT=" + ui->ladleTareWeightLineEdit->text() +
                                      ",GROSS_WGT=" + ui->finalGrossWeightLineEdit->text() +
                                      ",IRON_WGT=" + ui->finalGrossWeightLineEdit->text() + "-" + ui->ladleTareWeightLineEdit->text() +
                                      " WHERE TREAT_NO='" + ui->hmpHMNoComboBox->currentText()+"'");

    hmNoSelected(ui->hmpHMNoComboBox->currentText());
}
