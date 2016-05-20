#include <QtSQL>
#include <QMessageBox>
#include "util/QReserveTableModel.h"
#include "util/TimeEditDelegate.h"
#include "03status.h"
#include "ui_mainwindow.h"
#include "util/ToTheMiddleDelegate.h"

PageStatus::PageStatus(Ui::MainWindow *pui,QObject *parent)
    : QObject(parent), ui(pui)
{
    statusTableModel = new QSqlRelationalTableModel(this);
    statusTableModel->setTable("STATUS");
    statusTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    statusTableModel->setSort(statusTableModel->fieldIndex("STATION_NO"),Qt::AscendingOrder);
    statusTableModel->select();

    reserveTableModel = new QReserveTableModel(statusTableModel,this);
    ui->statusTableView->setModel(reserveTableModel);


    int i=0;
    reserveTableModel->setHeaderData(i++, Qt::Horizontal, tr("D1"));
    reserveTableModel->setHeaderData(i++, Qt::Horizontal, tr("D2"));
    reserveTableModel->setHeaderData(i++, Qt::Horizontal, tr("D3"));
    reserveTableModel->setHeaderData(i++, Qt::Horizontal, tr("BOF2"));
    reserveTableModel->setHeaderData(i++, Qt::Horizontal, tr("BOF3"));
    reserveTableModel->setHeaderData(i++, Qt::Horizontal, tr("BOF4"));

    i=1;
    reserveTableModel->setHeaderData(statusTableModel->fieldIndex("PLAN_NO"), Qt::Vertical, tr("PLAN_NO"));
    reserveTableModel->setHeaderData(statusTableModel->fieldIndex("PRODUCTION_ID"), Qt::Vertical, tr("PRODUCTION_ID"));
    reserveTableModel->setHeaderData(statusTableModel->fieldIndex("TREAT_ID"), Qt::Vertical, tr("TREAT_ID"));
    reserveTableModel->setHeaderData(statusTableModel->fieldIndex("IRON_LADLE_ID"), Qt::Vertical, tr("IRON_LADLE_ID"));
    reserveTableModel->setHeaderData(statusTableModel->fieldIndex("STEEL_GRADE"), Qt::Vertical, tr("STEEL_GRADE"));
    reserveTableModel->setHeaderData(statusTableModel->fieldIndex("STATUS_CODE"), Qt::Vertical, tr("STATUS_CODE"));
    reserveTableModel->setHeaderData(statusTableModel->fieldIndex("TIME_TO_END"), Qt::Vertical, tr("TIME_TO_END"));
    reserveTableModel->setHeaderData(statusTableModel->fieldIndex("UPDATE_TIME"), Qt::Vertical, tr("UPDATE_TIME"));

    //ui->statusTableView->setItemDelegateForColumn(statusTableModel->fieldIndex("UPDATE_TIME"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->statusTableView->setItemDelegateForRow(8, new TimeEditDelegate("MM-dd hh:mm"));

    ui->statusTableView->setRowHidden(statusTableModel->fieldIndex("STATION_NO"),true); // hide station no

    ui->statusTableView->verticalHeader()->setVisible(true);
    ui->statusTableView->horizontalHeader()->setVisible(true);

    statusTableModel->setRelation(statusTableModel->fieldIndex("STATUS_CODE"), QSqlRelation("STATUS_MAP", "STATUS_CODE", "STATUS_DESC"));

    ui->statusTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->statusTableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    ui->statusTableView->setItemDelegate(new ToTheMiddleDelegate);

    for(i=0;i<6;i++)
    {
        ui->statusTableView->setColumnWidth(i,100);
    }

    ladleStatusTableModel = new QSqlRelationalTableModel(this);
    ladleStatusTableModel->setTable("LADLE_STATUS");
    ladleStatusTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ladleStatusTableModel->setSort(ladleStatusTableModel->fieldIndex("START_TIME"),Qt::DescendingOrder);
    ladleStatusTableModel->select();

    ui->ladleStatusTableView->setModel(ladleStatusTableModel);

    ladleStatusTableModel->setHeaderData(ladleStatusTableModel->fieldIndex("LADLE_ID"), Qt::Horizontal, tr("LADLE_ID"));
    ladleStatusTableModel->setHeaderData(ladleStatusTableModel->fieldIndex("LADLE_TYPE"), Qt::Horizontal, tr("LADLE_TYPE"));
    ladleStatusTableModel->setHeaderData(ladleStatusTableModel->fieldIndex("CURRENT_TREAT"), Qt::Horizontal, tr("CURRENT_TREAT"));
    ladleStatusTableModel->setHeaderData(ladleStatusTableModel->fieldIndex("TREAT_NO"), Qt::Horizontal, tr("TREAT_NO"));
    ladleStatusTableModel->setHeaderData(ladleStatusTableModel->fieldIndex("START_TIME"), Qt::Horizontal, tr("START_TIME"));
    ladleStatusTableModel->setHeaderData(ladleStatusTableModel->fieldIndex("CURRENT_STATUS"), Qt::Horizontal, tr("CURRENT_STATUS"));

    ladleStatusTableModel->setRelation(ladleStatusTableModel->fieldIndex("CURRENT_STATUS"), QSqlRelation("LADLE_STATUS_MAP", "CURRENT_STATUS", "STATUS_DESC"));
    ladleStatusTableModel->setRelation(ladleStatusTableModel->fieldIndex("LADLE_TYPE"), QSqlRelation("LADLE_TYPE_MAP", "LADLE_TYPE", "TYPE_DESC"));
    ladleStatusTableModel->setRelation(ladleStatusTableModel->fieldIndex("CURRENT_TREAT"), QSqlRelation("TREAT_TYPE_MAP", "TREAT_TYPE", "TYPE_DESC"));


    ui->ladleStatusTableView->setItemDelegateForColumn(ladleStatusTableModel->fieldIndex("START_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));

    ui->ladleStatusTableView->setItemDelegate(new ToTheMiddleDelegate);
    ui->ladleStatusTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->ladleStatusTableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    /*
    QStringList bofIdList;
    QSqlQuery bofIdQuery("SELECT BOF_ID FROM LADLE_RESULT WHERE BOF_ID IS NOT NULL GROUP BY BOF_ID ORDER BY BOF_ID DESC");
    bofIdList.append("");
    while (bofIdQuery.next())
    {
        QString bofId = bofIdQuery.value(0).toString();
        bofIdList.append(bofId);
    }
    ui->bofIdComboBox->clear();
    ui->bofIdComboBox->addItems(bofIdList);
    */
    QString currentBofId;
    QSqlQuery currentBofIdQuery("SELECT MAX(BOF_ID) FROM V_BOF_ID");
    while (currentBofIdQuery.next())
    {
        currentBofId = currentBofIdQuery.value(0).toString();
    }
    ui->bofIdLineEdit->setText(currentBofId);
    bofIdSelected();
    QStringList ladleNoToBofList;
    QSqlQuery ladleNoToBofQuery("SELECT LADLE_ID FROM LADLE_STATUS ORDER BY START_TIME ASC");
    ladleNoToBofList.append("");
    while (ladleNoToBofQuery.next())
    {
        QString ladleNoToBof = ladleNoToBofQuery.value(0).toString();
        ladleNoToBofList.append(ladleNoToBof);
    }

    ui->forecastToBofComboBox->clear();
    ui->forecastToBofComboBox->addItems(ladleNoToBofList);

    ui->confirmToBofComboBox->clear();
    ui->confirmToBofComboBox->addItems(ladleNoToBofList);

//    connect(ui->bofIdComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(bofIdSelected(const QString &)));
//    connect(ui->hmIntoBofButton, SIGNAL(clicked()), this, SLOT(hmIntoBof()));
    connect(ui->bofIdBtn, SIGNAL(clicked()), this, SLOT(bofIdSelected()));
    connect(ui->forecastToBofButton, SIGNAL(clicked()), this, SLOT(forecastToBof()));
    connect(ui->confirmToBofButton, SIGNAL(clicked()), this, SLOT(confirmToBof()));
    connect(ui->forecastEditButton, SIGNAL(clicked()), this, SLOT(forecastEdit()));
    connect(ui->cancelForecastBtn, SIGNAL(clicked()), this, SLOT(cancelForecast()));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
    timer->start(1000);
}

bool PageStatus::event ( QEvent * e)
{
    if(e &&  e->type() == QEvent::ChildAdded)
    {
        return true;
    }

    if(statusTableModel)
    {
       statusTableModel->select();
    }
    return true;
}

void PageStatus::bofIdSelected()
{
    ladleResultTableModel = new QSqlRelationalTableModel(this);
    ladleResultTableModel->setTable("LADLE_RESULT");
    ladleResultTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ladleResultTableModel->setSort(ladleResultTableModel->fieldIndex("START_TIME"),Qt::DescendingOrder);
    ladleResultTableModel->setRelation(ladleResultTableModel->fieldIndex("LADLE_TYPE"), QSqlRelation("LADLE_TYPE_MAP", "LADLE_TYPE", "TYPE_DESC"));
    ladleResultTableModel->setRelation(ladleResultTableModel->fieldIndex("TREAT_TYPE"), QSqlRelation("TREAT_TYPE_MAP", "TREAT_TYPE", "TYPE_DESC"));
    ladleResultTableModel->setFilter("BOF_ID='" + ui->bofIdLineEdit->text() + "'");
    ladleResultTableModel->select();

    ui->ladleResultTableView->setModel(ladleResultTableModel);

    int i=0;
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("LADLE_ID"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("LADLE_TYPE"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("TREAT_NO"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("TREAT_TYPE"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("START_TIME"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("TARE_WGT"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("GROSS_WGT"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("IRON_WGT"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_C"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_SI"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_MN"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_P"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_S"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_CU"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_V"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_NB"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_CR"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_NI"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_MO"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_SN"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_N"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_ALT"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_ALS"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_B"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_TI"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_CA"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_PB"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_SB"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_ZN"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_AS"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_CO"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_MG"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_ZR"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_W"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_TA"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_TE"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_RE"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_BI"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_SE"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_BA"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_OT"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_OA"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_H"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("SPARE1"));
    ladleResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("SPARE2"));

    ui->ladleResultTableView->setColumnWidth(0,70);
    ui->ladleResultTableView->setColumnWidth(1,70);
    ui->ladleResultTableView->setColumnWidth(2,70);
    ui->ladleResultTableView->setColumnWidth(3,70);
    ui->ladleResultTableView->setColumnWidth(4,100);
    ui->ladleResultTableView->setColumnWidth(5,70);
    ui->ladleResultTableView->setColumnWidth(6,70);
    ui->ladleResultTableView->setColumnWidth(7,70);
    for(i=8;i<45;i++)
    {
        ui->ladleResultTableView->setColumnWidth(i,70);
    }

    ui->ladleResultTableView->setColumnHidden(ladleResultTableModel->fieldIndex("BOF_ID"),true);

    ui->ladleResultTableView->setItemDelegateForColumn(ladleResultTableModel->fieldIndex("START_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));

    ui->ladleResultTableView->setItemDelegate(new ToTheMiddleDelegate);
    ui->ladleResultTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->ladleStatusTableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    //-----------------------------------------------------------------------
    ladlePourResultTableModel = new QSqlRelationalTableModel(this);
    ladlePourResultTableModel->setTable("POUR_IRON");
    ladlePourResultTableModel->setSort(ladlePourResultTableModel->fieldIndex("POUR_TIME"),Qt::DescendingOrder);
    ladlePourResultTableModel->setFilter("POUR_NO IN (SELECT TREAT_NO FROM LADLE_RESULT WHERE TREAT_TYPE=1 AND BOF_ID='" + ui->bofIdLineEdit->text() + "')");
    ladlePourResultTableModel->select();
    ui->ladlePourResultTableView->setModel(ladlePourResultTableModel);

    i=0;
    ladlePourResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("POUR_NO"));
    ladlePourResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("LADLE_NO_OUT"));
    ladlePourResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("BF_NO_OUT"));
    ladlePourResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("LADLE_NO_IN"));
    ladlePourResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("BF_NO_IN"));
    ladlePourResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("POUR_TIME"));
    ladlePourResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("POUR_WEIGHT"));
    ladlePourResultTableModel->setHeaderData(i++, Qt::Horizontal, tr("POUR_IN_COUNT"));

    i=0;
    ui->ladlePourResultTableView->setColumnWidth(i++,100);
    ui->ladlePourResultTableView->setColumnWidth(i++,100);
    ui->ladlePourResultTableView->setColumnWidth(i++,100);
    ui->ladlePourResultTableView->setColumnWidth(i++,100);
    ui->ladlePourResultTableView->setColumnWidth(i++,100);
    ui->ladlePourResultTableView->setColumnWidth(i++,100);

    ui->ladlePourResultTableView->setItemDelegateForColumn(ladlePourResultTableModel->fieldIndex("POUR_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));

    ui->ladlePourResultTableView->setItemDelegate(new ToTheMiddleDelegate);
    ui->ladlePourResultTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->ladlePourResultTableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    //-----------------------------------------------------------------------
    ladleDesResultTableModel = new QSqlRelationalTableModel(this);
    ladleDesResultTableModel->setTable("DES_RESULT");
    ladleDesResultTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ladleDesResultTableModel->setSort(ladleDesResultTableModel->fieldIndex("HM_NO"),Qt::DescendingOrder);
    ladleDesResultTableModel->setRelation(ladleDesResultTableModel->fieldIndex("SHIFT_ID"), QSqlRelation("SHIFT_MAP", "SHIFT", "SHIFT_DESC"));
    ladleDesResultTableModel->setRelation(ladleDesResultTableModel->fieldIndex("CREW_ID"), QSqlRelation("CREW_MAP", "CREW", "CREW_DESC"));
    ladleDesResultTableModel->setRelation(ladleDesResultTableModel->fieldIndex("DS_CODE"), QSqlRelation("DES_MAP", "DS_CODE", "DS_DESC"));
    ladleDesResultTableModel->select();
    ladleDesResultTableModel->setFilter("HM_NO IN (SELECT TREAT_NO FROM LADLE_RESULT WHERE TREAT_TYPE=2 AND BOF_ID='" + ui->bofIdLineEdit->text() + "')");
    ui->ladleDesResultTableView->setModel(ladleDesResultTableModel);

    ladleDesResultTableModel->setHeaderData(ladleDesResultTableModel->fieldIndex("HM_NO"), Qt::Horizontal, tr("HM_NO"));
    ladleDesResultTableModel->setHeaderData(ladleDesResultTableModel->fieldIndex("PLAN_NO"), Qt::Horizontal, tr("PLAN_NO"));
    ladleDesResultTableModel->setHeaderData(ladleDesResultTableModel->fieldIndex("PRODUCTION_ID"), Qt::Horizontal, tr("PRODUCTION_ID"));
    ladleDesResultTableModel->setHeaderData(ladleDesResultTableModel->fieldIndex("ST_NO"), Qt::Horizontal, tr("ST_NO"));
    ladleDesResultTableModel->setHeaderData(ladleDesResultTableModel->fieldIndex("STATION_NO"), Qt::Horizontal, tr("STATION_NO"));
    ladleDesResultTableModel->setHeaderData(ladleDesResultTableModel->fieldIndex("SHIFT_ID"), Qt::Horizontal, tr("SHIFT_ID"));
    ladleDesResultTableModel->setHeaderData(ladleDesResultTableModel->fieldIndex("CREW_ID"), Qt::Horizontal, tr("CREW_ID"));
    ladleDesResultTableModel->setHeaderData(ladleDesResultTableModel->fieldIndex("SHIFT_DESC"), Qt::Horizontal, tr("SHIFT_DESC"));
    ladleDesResultTableModel->setHeaderData(ladleDesResultTableModel->fieldIndex("CREW_DESC"), Qt::Horizontal, tr("CREW_DESC"));
    ladleDesResultTableModel->setHeaderData(ladleDesResultTableModel->fieldIndex("TPD_NO"), Qt::Horizontal, tr("TPD_NO"));
    ladleDesResultTableModel->setHeaderData(ladleDesResultTableModel->fieldIndex("DS_CODE"), Qt::Horizontal, tr("DS_CODE"));
    ladleDesResultTableModel->setHeaderData(ladleDesResultTableModel->fieldIndex("DS_DESC"), Qt::Horizontal, tr("DS_DESC"));
    ladleDesResultTableModel->setHeaderData(ladleDesResultTableModel->fieldIndex("IRON_LADLE_ID"), Qt::Horizontal, tr("IRON_LADLE_ID"));
    ladleDesResultTableModel->setHeaderData(ladleDesResultTableModel->fieldIndex("INIT_TEMP_TIME"), Qt::Horizontal, tr("INIT_TEMP_TIME"));
    ladleDesResultTableModel->setHeaderData(ladleDesResultTableModel->fieldIndex("INIT_TEMP"), Qt::Horizontal, tr("INIT_TEMP"));
    ladleDesResultTableModel->setHeaderData(ladleDesResultTableModel->fieldIndex("FINAL_TEMP_TIME"), Qt::Horizontal, tr("FINAL_TEMP_TIME"));
    ladleDesResultTableModel->setHeaderData(ladleDesResultTableModel->fieldIndex("FINAL_TEMP"), Qt::Horizontal, tr("FINAL_TEMP"));
    ladleDesResultTableModel->setHeaderData(ladleDesResultTableModel->fieldIndex("INIT_S"), Qt::Horizontal, tr("INIT_S"));
    ladleDesResultTableModel->setHeaderData(ladleDesResultTableModel->fieldIndex("FINAL_S"), Qt::Horizontal, tr("FINAL_S"));
    ladleDesResultTableModel->setHeaderData(ladleDesResultTableModel->fieldIndex("INIT_WEIGHT"), Qt::Horizontal, tr("INIT_WEIGHT"));
    ladleDesResultTableModel->setHeaderData(ladleDesResultTableModel->fieldIndex("FINAL_WEIGHT"), Qt::Horizontal, tr("FINAL_WEIGHT"));
    ladleDesResultTableModel->setHeaderData(ladleDesResultTableModel->fieldIndex("LADLE_TARE_WEIGHT"), Qt::Horizontal, tr("LADLE_TARE_WEIGHT"));
    ladleDesResultTableModel->setHeaderData(ladleDesResultTableModel->fieldIndex("DES_TIME"), Qt::Horizontal, tr("DES_TIME"));
    ladleDesResultTableModel->setHeaderData(ladleDesResultTableModel->fieldIndex("HM_TIME"), Qt::Horizontal, tr("HM_TIME"));

    ui->ladleDesResultTableView->setColumnWidth(ladleDesResultTableModel->fieldIndex("HM_NO"),80);
    ui->ladleDesResultTableView->setColumnWidth(ladleDesResultTableModel->fieldIndex("PLAN_NO"),80);
    ui->ladleDesResultTableView->setColumnWidth(ladleDesResultTableModel->fieldIndex("PRODUCTION_ID"),80);
    ui->ladleDesResultTableView->setColumnWidth(ladleDesResultTableModel->fieldIndex("ST_NO"),120);
    ui->ladleDesResultTableView->setColumnWidth(ladleDesResultTableModel->fieldIndex("STATION_NO"),80);
    ui->ladleDesResultTableView->setColumnWidth(ladleDesResultTableModel->fieldIndex("SHIFT_ID"),40);
    ui->ladleDesResultTableView->setColumnWidth(ladleDesResultTableModel->fieldIndex("CREW_ID"),40);
    ui->ladleDesResultTableView->setColumnWidth(ladleDesResultTableModel->fieldIndex("SHIFT_DESC"),55);
    ui->ladleDesResultTableView->setColumnWidth(ladleDesResultTableModel->fieldIndex("CREW_DESC"),40);
    ui->ladleDesResultTableView->setColumnWidth(ladleDesResultTableModel->fieldIndex("TPD_NO"),80);
    ui->ladleDesResultTableView->setColumnWidth(ladleDesResultTableModel->fieldIndex("DS_CODE"),55);
    ui->ladleDesResultTableView->setColumnWidth(ladleDesResultTableModel->fieldIndex("DS_DESC"),55);
    ui->ladleDesResultTableView->setColumnWidth(ladleDesResultTableModel->fieldIndex("IRON_LADLE_ID"),70);
    ui->ladleDesResultTableView->setColumnWidth(ladleDesResultTableModel->fieldIndex("INIT_TEMP_TIME"),95);
    ui->ladleDesResultTableView->setColumnWidth(ladleDesResultTableModel->fieldIndex("INIT_TEMP"),55);
    ui->ladleDesResultTableView->setColumnWidth(ladleDesResultTableModel->fieldIndex("FINAL_TEMP_TIME"),95);
    ui->ladleDesResultTableView->setColumnWidth(ladleDesResultTableModel->fieldIndex("FINAL_TEMP"),55);
    ui->ladleDesResultTableView->setColumnWidth(ladleDesResultTableModel->fieldIndex("INIT_S"),80);
    ui->ladleDesResultTableView->setColumnWidth(ladleDesResultTableModel->fieldIndex("FINAL_S"),80);
    ui->ladleDesResultTableView->setColumnWidth(ladleDesResultTableModel->fieldIndex("INIT_WEIGHT"),70);
    ui->ladleDesResultTableView->setColumnWidth(ladleDesResultTableModel->fieldIndex("FINAL_WEIGHT"),70);
    ui->ladleDesResultTableView->setColumnWidth(ladleDesResultTableModel->fieldIndex("LADLE_TARE_WEIGHT"),70);
    ui->ladleDesResultTableView->setColumnWidth(ladleDesResultTableModel->fieldIndex("DES_TIME"),70);
    ui->ladleDesResultTableView->setColumnWidth(ladleDesResultTableModel->fieldIndex("HM_TIME"),70);

    ui->ladleDesResultTableView->setItemDelegateForColumn(ladleDesResultTableModel->fieldIndex("INIT_TEMP_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));
    ui->ladleDesResultTableView->setItemDelegateForColumn(ladleDesResultTableModel->fieldIndex("FINAL_TEMP_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));

    ui->ladleDesResultTableView->setColumnHidden(ladleDesResultTableModel->fieldIndex("STATION_NO"),true);
    ui->ladleDesResultTableView->setColumnHidden(ladleDesResultTableModel->fieldIndex("DS_DESC"),true);

    ui->ladleDesResultTableView->setItemDelegate(new ToTheMiddleDelegate);
    ui->ladleDesResultTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->ladleDesResultTableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
}

void PageStatus::refresh()
{
    QStringList bofIdList;
    QSqlQuery bofIdQuery("SELECT BOF_ID FROM V_BOF_ID");
    bofIdList.append("");
    while (bofIdQuery.next())
    {
        QString bofId = bofIdQuery.value(0).toString();
        bofIdList.append(bofId);
    }
    /*
    if (ui->bofIdComboBox->count() != bofIdList.count())
    {
        ui->bofIdComboBox->clear();
        ui->bofIdComboBox->addItems(bofIdList);
        ui->bofIdComboBox->setDisabled(false);
    }
    */
    QStringList ladleNoToBofList;
    QSqlQuery ladleNoToBofQuery("SELECT LADLE_ID FROM LADLE_STATUS ORDER BY START_TIME ASC");
    ladleNoToBofList.append("");
    while (ladleNoToBofQuery.next())
    {
        QString ladleNoToBof = ladleNoToBofQuery.value(0).toString();
        ladleNoToBofList.append(ladleNoToBof);
    }

    if (ui->forecastToBofComboBox->count() != ladleNoToBofList.count())
    {
        ui->forecastToBofComboBox->clear();
        ui->forecastToBofComboBox->addItems(ladleNoToBofList);
        ui->confirmToBofComboBox->clear();
        ui->confirmToBofComboBox->addItems(ladleNoToBofList);
    }

    if(statusTableModel)
    {
        statusTableModel->select();
    }

    if(ladleStatusTableModel)
    {
        ladleStatusTableModel->select();
    }
    /*
    if(ui->bofIdComboBox->currentText()!="")
    {
        if(ladleResultTableModel)
        {
            ladleResultTableModel->select();
        }
        if(ladlePourResultTableModel)
        {
            ladlePourResultTableModel->select();
        }
        if(ladleDesResultTableModel)
        {
            ladleDesResultTableModel->select();
        }
    }
    */



    QSqlQuery forecastWgtQuery("SELECT IRON_WGT FROM IRON_IN_BOF");
    if(forecastWgtQuery.next())
    {
        ui->forecastWgtLabel->setText(forecastWgtQuery.value(0).toString());

        QSqlQuery forecastLadleIdQuery("SELECT LADLE_ID FROM LADLE_RESULT WHERE BOF_ID=(SELECT CURRENT_BOF_ID FROM CURRENT_INFO)");
        if(forecastLadleIdQuery.next())
        {
            ui->forecastLadleIdLabel->setText(forecastLadleIdQuery.value(0).toString());
        }
        else
        {
            ui->forecastLadleIdLabel->setText("未预判上水");
        }

        QSqlQuery forecastTempQuery("SELECT TEMP FROM TEMP_TO_BOF WHERE BOF_ID=(SELECT CURRENT_BOF_ID FROM CURRENT_INFO)");
        if(forecastTempQuery.next())
        {
            ui->forecastTempLabel->setText(forecastTempQuery.value(0).toString());
        }
        else
        {
            ui->forecastTempLabel->setText("未预判上水");
        }
    }
    else
    {
        ui->forecastLadleIdLabel->setText("未预判上水");
        ui->forecastWgtLabel->setText("未预判上水");
        ui->forecastTempLabel->setText("未预判上水");
    }

}

/*
void PageStatus::hmIntoBof()
{
    QSqlQuery updateLadleWeightQuery;
    updateLadleWeightQuery.prepare("CALL UPDATE_LADLE_WEIGHT('" + ui->ladleNoToBofComboBox->currentText() + "')");
    updateLadleWeightQuery.exec();

    QSqlQuery ladleTypeQuery("SELECT LADLE_TYPE FROM LADLE_STATUS WHERE LADLE_ID='"+ui->ladleNoToBofComboBox->currentText()+"'");
    while (ladleTypeQuery.next())
    {
        int ladleType = ladleTypeQuery.value(0).toInt();
        if(ladleType==1||ladleType==3)
        {
            QSqlQuery ladleLeaveQuery("UPDATE IRON_TIME SET END_TIME=SYSDATE WHERE END_TIME IS NULL AND LADLE_NO='"+ui->ladleNoToBofComboBox->currentText()+"'");
        }
        else
        {
            QSqlQuery ladleLeaveStdQuery("UPDATE STD_IRON_INFO SET END_FLAG=1,END_TIME=SYSDATE WHERE LADLE_NO='"+ui->ladleNoToBofComboBox->currentText()+"' AND END_FLAG=0");
        }
    }


    QSqlQuery ironToBofQuery;
    ironToBofQuery.prepare("DECLARE RET NUMBER; BEGIN RET:=PAKTEST.calc('L2_COM',4,'c4@0000,c6@000000,c30@ ,c10@"+ui->ladleNoToBofComboBox->currentText()+",i4@0',10); END;");
    ironToBofQuery.exec();

    QSqlQuery currentBofIdQuery("SELECT CURRENT_BOF_ID FROM CURRENT_INFO");
    while (currentBofIdQuery.next())
    {
        QString currentBofId = currentBofIdQuery.value(0).toInt();
        QStringList bofIdList;
        QSqlQuery bofIdQuery("SELECT BOF_ID FROM LADLE_RESULT WHERE BOF_ID IS NOT NULL GROUP BY BOF_ID ORDER BY BOF_ID DESC");
        bofIdList.append("");
        while (bofIdQuery.next())
        {
            QString bofId = bofIdQuery.value(0).toString();
            bofIdList.append(bofId);
        }
        ui->bofIdComboBox->clear();
        ui->bofIdComboBox->addItems(bofIdList);
        ui->bofIdComboBox->setCurrentIndex(1);
    }
}
*/

void PageStatus::forecastToBof()
{
    //判断是否为空
    QString forecastLadleId = ui->forecastToBofComboBox->currentText();
    QString forecastWeight = ui->forecastToBofWgtLineEdit->text();
    QString forecastTemp = ui->forecastToBofTempLineEdit->text();
    if (forecastLadleId == "" || forecastWeight == "" || forecastTemp == "")
    {
        QMessageBox::critical(0, qApp->tr("Error"),
            qApp->tr("上水相关项不能有空值")
                     , QMessageBox::Ok);
        return;
    }
    //判断是否已经有过预判上水
    QSqlQuery ironInBofQuery("SELECT Count(*) FROM IRON_IN_BOF ");
    if (ironInBofQuery.next())
    {
        int count = ironInBofQuery.value(0).toInt();
        if(count==1)
        {
            QMessageBox::critical(0, qApp->tr("Error"),
                qApp->tr("已经存在预判上水内容，无法再次预判")
                         , QMessageBox::Ok);
            return;
        }
    }
    //执行预判上水
    QSqlQuery forecastToBofQuery;
    forecastToBofQuery.prepare("CALL FORECAST_TO_BOF('"
                                + forecastLadleId + "',"
                                + forecastWeight + ","
                                + forecastTemp + ")");
    forecastToBofQuery.exec();

    //读取预判的上水号
    QString bofId="";
    QSqlQuery bofIdQuery("select current_bof_id from current_info");
    if (bofIdQuery.next())
    {
        bofId = bofIdQuery.value(0).toString();
    }
    //发送上水电文
    QSqlQuery sendToBofEleQuery;
    sendToBofEleQuery.prepare("DECLARE RET NUMBER; BEGIN RET:=PAKTEST.calc('L2_COM',4,'c4@0000,c6@D2BM08,c30@ ,c10@"+bofId+",i4@0',10); END;");
    sendToBofEleQuery.exec();
    QSqlQuery sendToBofInfoQuery;
    sendToBofInfoQuery.prepare("DECLARE RET NUMBER; BEGIN RET:=PAKTEST.calc('L2_COM',4,'c4@0000,c6@D2BM09,c30@ ,c10@"+bofId+",i4@0',10); END;");
    sendToBofInfoQuery.exec();
    //发送预判上水电文
    //QSqlQuery sendForecastToBofQuery;
    //sendForecastToBofQuery.prepare("DECLARE RET NUMBER; BEGIN RET:=PAKTEST.calc('L2_COM',4,'c4@0000,c6@000000,c30@ ,c10@"+ui->forecastToBofComboBox->currentText()+",i4@0',10); END;");
    //sendForecastToBofQuery.exec();

    //标准罐离站  ----已经加入到FORECAST_TO_BOF存储过程中
    //QSqlQuery stdLeaveQuery;
    //stdLeaveQuery.prepare("CALL STD_LEAVE('" + forecastLadleId + "')");
    //stdLeaveQuery.exec();
    /*
    QSqlQuery ladleTypeQuery("SELECT LADLE_TYPE FROM LADLE_STATUS WHERE LADLE_ID='"+ui->forecastToBofComboBox->currentText()+"'");
    while (ladleTypeQuery.next())
    {
        int ladleType = ladleTypeQuery.value(0).toInt();
        if(ladleType==1||ladleType==3)
        {
            //QSqlQuery ladleLeaveQuery("UPDATE IRON_TIME SET END_TIME=SYSDATE WHERE END_TIME IS NULL AND LADLE_NO='"+ui->ladleNoToBofComboBox->currentText()+"'");
        }
        else
        {
            QSqlQuery ladleLeaveStdQuery("UPDATE STD_IRON_INFO SET END_FLAG=1,END_TIME=SYSDATE WHERE LADLE_NO='"+ui->forecastToBofComboBox->currentText()+"' AND END_FLAG=0");
            //QSqlQuery ladleStatusLeaveStdQuery("DELETE FROM LADLE_STATUS WHERE LADLE_ID='"+ui->forecastToBofComboBox->currentText()+"'");
        }
    }
    */

    QString currentBofId;
    QSqlQuery currentBofIdQuery("SELECT MAX(BOF_ID) FROM V_BOF_ID");
    while (currentBofIdQuery.next())
    {
        currentBofId = currentBofIdQuery.value(0).toString();
    }
    ui->bofIdLineEdit->setText(currentBofId);
    bofIdSelected();

    /*
    QString partLadleId = ui->partToBofComboBox->currentText();
    QString partWeight = ui->partToBofWgtLineEdit->text();

    if (partLadleId == "" || partWeight == "")
    {
        QMessageBox::critical(0, qApp->tr("Error"),
            qApp->tr("入炉相关项不能有空值")
                     , QMessageBox::Ok);
        return;
    }
    QSqlQuery partToBofQuery;
    partToBofQuery.prepare("CALL PART_TO_BOF('"
                     + partLadleId + "',"
                     + partWeight + ")");
    partToBofQuery.exec();
    */
}
void PageStatus::confirmToBof()
{
    //判断是否为空
    QString confirmLadleId = ui->confirmToBofComboBox->currentText();
    QString confirmWeight = ui->confirmToBofWgtLineEdit->text();
    QString confirmTemp = ui->confirmToBofTempLineEdit->text();
    if (confirmLadleId == "" || confirmWeight == "" || confirmTemp == "")
    {
        QMessageBox::critical(0, qApp->tr("Error"),
            qApp->tr("上水相关项不能有空值")
                     , QMessageBox::Ok);
        return;
    }
    //判断是否已经有过预判上水
    QSqlQuery ironInBofQuery("SELECT Count(*) FROM IRON_IN_BOF ");
    if (ironInBofQuery.next())
    {
        int count = ironInBofQuery.value(0).toInt();
        if(count==0)
        {
            QMessageBox::critical(0, qApp->tr("Error"),
                qApp->tr("尚未进行预判上水操作，无法直接确定上水")
                         , QMessageBox::Ok);
            return;
        }
    }


    //执行确定上水
    QSqlQuery confirmToBofQuery;
    confirmToBofQuery.prepare("CALL CONFIRM_TO_BOF('"
                                + confirmLadleId + "',"
                                + confirmWeight + ","
                                + confirmTemp + ")");
    confirmToBofQuery.exec();

    //读取之前的上水号
    QString bofId="";
    QSqlQuery bofIdQuery("select current_bof_id from current_info");
    if (bofIdQuery.next())
    {
        bofId = bofIdQuery.value(0).toString();
    }
    //发送上水电文
    QSqlQuery sendToBofEleQuery;
    sendToBofEleQuery.prepare("DECLARE RET NUMBER; BEGIN RET:=PAKTEST.calc('L2_COM',4,'c4@0000,c6@D2BM08,c30@ ,c10@"+bofId+",i4@0',10); END;");
    sendToBofEleQuery.exec();
    QSqlQuery sendToBofInfoQuery;
    sendToBofInfoQuery.prepare("DECLARE RET NUMBER; BEGIN RET:=PAKTEST.calc('L2_COM',4,'c4@0000,c6@D2BM09,c30@ ,c10@"+bofId+",i4@0',10); END;");
    sendToBofInfoQuery.exec();

    //标准罐离站
    QSqlQuery ladleTypeQuery("SELECT LADLE_TYPE FROM LADLE_STATUS WHERE LADLE_ID='"+ui->confirmToBofComboBox->currentText()+"'");
    while (ladleTypeQuery.next())
    {
        int ladleType = ladleTypeQuery.value(0).toInt();
        if(ladleType==1||ladleType==3)
        {
            //QSqlQuery ladleLeaveQuery("UPDATE IRON_TIME SET END_TIME=SYSDATE WHERE END_TIME IS NULL AND LADLE_NO='"+ui->ladleNoToBofComboBox->currentText()+"'");
        }
        else
        {
            QSqlQuery ladleLeaveStdQuery("UPDATE STD_IRON_INFO SET END_FLAG=1,END_TIME=SYSDATE WHERE LADLE_NO='"+ui->confirmToBofComboBox->currentText()+"' AND END_FLAG=0");
            QSqlQuery ladleStatusLeaveStdQuery("DELETE FROM LADLE_STATUS WHERE LADLE_ID='"+ui->confirmToBofComboBox->currentText()+"'");
        }
    }

    QString currentBofId;
    QSqlQuery currentBofIdQuery("SELECT MAX(BOF_ID) FROM V_BOF_ID");
    while (currentBofIdQuery.next())
    {
        currentBofId = currentBofIdQuery.value(0).toString();
    }
    ui->bofIdLineEdit->setText(currentBofId);
    bofIdSelected();
    /*
    QString finishLadleId = ui->finishToBofComboBox->currentText();
    QString finishWeight = ui->finishToBofWgtLineEdit->text();

    if (finishLadleId == "" || finishWeight == "")
    {
        QMessageBox::critical(0, qApp->tr("Error"),
            qApp->tr("入炉相关项不能有空值")
                     , QMessageBox::Ok);
        return;
    }
    QSqlQuery partToBofQuery;
    partToBofQuery.prepare("CALL PART_TO_BOF('"
                     + finishLadleId + "',"
                     + finishWeight + ")");
    partToBofQuery.exec();

    QSqlQuery finishToBofQuery;
    finishToBofQuery.prepare("CALL finish_to_bof()");
    finishToBofQuery.exec();


    QSqlQuery currentBofIdQuery("SELECT CURRENT_BOF_ID FROM CURRENT_INFO");
    while (currentBofIdQuery.next())
    {
        QString currentBofId = currentBofIdQuery.value(0).toString();
        qDebug()<<currentBofId;
        QSqlQuery sendFinalEleQuery;
        sendFinalEleQuery.prepare("DECLARE RET NUMBER; BEGIN RET:=PAKTEST.calc('L2_COM',4,'c4@0000,c6@D1BM08,c30@ ,c10@"+currentBofId+",i4@0',10); END;");
        sendFinalEleQuery.exec();

        QSqlQuery sendIronToBofQuery;
        sendIronToBofQuery.prepare("DECLARE RET NUMBER; BEGIN RET:=PAKTEST.calc('L2_COM',4,'c4@0000,c6@D1BM09,c30@ ,c10@"+currentBofId+",i4@0',10); END;");
        sendIronToBofQuery.exec();

        QStringList bofIdList;
        QSqlQuery bofIdQuery("SELECT BOF_ID FROM LADLE_RESULT WHERE BOF_ID IS NOT NULL GROUP BY BOF_ID ORDER BY BOF_ID DESC");
        bofIdList.append("");
        while (bofIdQuery.next())
        {
            QString bofId = bofIdQuery.value(0).toString();
            bofIdList.append(bofId);
        }
        ui->bofIdComboBox->clear();
        ui->bofIdComboBox->addItems(bofIdList);
        ui->bofIdComboBox->setCurrentIndex(1);
    }
    */
}

void PageStatus::forecastEdit()
{
    QSqlQuery forecastEditLadleResultQuery("UPDATE LADLE_RESULT SET GROSS_WGT=" + ui->forecastEditWgtLineEdit->text() + "+TARE_WGT,IRON_WGT=" + ui->forecastEditWgtLineEdit->text() + " WHERE BOF_ID=(SELECT CURRENT_BOF_ID FROM CURRENT_INFO) AND START_TIME=(SELECT MAX(START_TIME) FROM LADLE_RESULT WHERE BOF_ID=(SELECT CURRENT_BOF_ID FROM CURRENT_INFO))");
    QSqlQuery forecastEditIronInBofQuery("UPDATE IRON_IN_BOF SET IRON_WGT=" + ui->forecastEditWgtLineEdit->text());
    QSqlQuery forecastEditTempToBofQuery("UPDATE TEMP_TO_BOF SET TEMP=" + ui->forecastEditTempLineEdit->text() + " WHERE BOF_ID=(SELECT CURRENT_BOF_ID FROM CURRENT_INFO)");
    //读取当前的上水号
    QString bofId="";
    QSqlQuery bofIdQuery("select current_bof_id from current_info");
    if (bofIdQuery.next())
    {
        bofId = bofIdQuery.value(0).toString();
    }
    //发送二次上水修改电文
    QSqlQuery sendToBofEleQuery;
    sendToBofEleQuery.prepare("DECLARE RET NUMBER; BEGIN RET:=PAKTEST.calc('L2_COM',4,'c4@0000,c6@D2BM08,c30@ ,c10@"+bofId+",i4@0',10); END;");
    sendToBofEleQuery.exec();
    QSqlQuery sendToBofInfoQuery;
    sendToBofInfoQuery.prepare("DECLARE RET NUMBER; BEGIN RET:=PAKTEST.calc('L2_COM',4,'c4@0000,c6@D2BM09,c30@ ,c10@"+bofId+",i4@0',10); END;");
    sendToBofInfoQuery.exec();
}

void PageStatus::cancelForecast()
{
    //判断是否已经有过预判上水
    QSqlQuery ironInBofQuery("SELECT Count(*) FROM IRON_IN_BOF ");
    if (ironInBofQuery.next())
    {
        int count = ironInBofQuery.value(0).toInt();
        if(count==0)
        {
            QMessageBox::critical(0, qApp->tr("Error"),
                qApp->tr("尚未进行预判上水，无法取消")
                         , QMessageBox::Ok);
            return;
        }
    }


    //取消预判上水
    QSqlQuery cancelForecastQuery;
    cancelForecastQuery.prepare("CALL CANCEL_FORECAST()");
    cancelForecastQuery.exec();
    QSqlQuery currentBofIdQuery("SELECT MAX(BOF_ID) FROM V_BOF_ID");

    QString currentBofId;
    while (currentBofIdQuery.next())
    {
        currentBofId = currentBofIdQuery.value(0).toString();
    }
    ui->bofIdLineEdit->setText(currentBofId);
    bofIdSelected();
}
