#include <QtSQL>
#include <QMessageBox>
#include <QScrollBar>
#include "util/TimeEditDelegate.h"
#include "01plan.h"
#include "ui_mainwindow.h"
#include "ui_plandialog.h"
#include "util/ToTheMiddleDelegate.h"

PagePlan::PagePlan(Ui::MainWindow *pui,QObject *parent)
    : QObject(parent), ui(pui),planDialog(new Ui::PlanDialog)
{
    planTableModel = new QSqlRelationalTableModel(this);
    planTableModel->setTable("PLAN");
    planTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    planTableModel->setSort(planTableModel->fieldIndex("DES_END_TIME"),Qt::DescendingOrder);
    planTableModel->setRelation(planTableModel->fieldIndex("PLAN_STATUS"), QSqlRelation("PLAN_MAP", "PLAN_STATUS", "PLAN_STATUS_DESC"));
    planTableModel->select();

    ui->planTableView->setModel(planTableModel);
    ui->planTableView_2->setModel(planTableModel);

    // change header data
    planTableModel->setHeaderData(planTableModel->fieldIndex("PLAN_NO"), Qt::Horizontal, tr("PLAN_NO"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("PRODUCTION_ID"), Qt::Horizontal, tr("PRODUCTION_ID"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("ST_NO"), Qt::Horizontal, tr("ST_NO"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("ROUTE_ID"), Qt::Horizontal, tr("ROUTE_ID"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("SMELT_TYPE"), Qt::Horizontal, tr("SMELT_TYPE"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("BOF_NO"), Qt::Horizontal, tr("BOF_NO"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("CCM_NO"), Qt::Horizontal, tr("CCM_NO"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("CAST_NO"), Qt::Horizontal, tr("CAST_NO"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("CAST_TOTAL_HEATS"), Qt::Horizontal, tr("CAST_TOTAL_HEATS"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("CAST_DIV_NO"), Qt::Horizontal, tr("CAST_DIV_NO"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("DES_END_TIME"), Qt::Horizontal, tr("DES_END_TIME"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("MAT_LOAD_START_TIME"), Qt::Horizontal, tr("MAT_LOAD_START_TIME"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("BLOW_START_TIME"), Qt::Horizontal, tr("BLOW_START_TIME"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("BLOW_END_TIME"), Qt::Horizontal, tr("BLOW_END_TIME"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("TAP_START_TIME"), Qt::Horizontal, tr("TAP_START_TIME"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("TAP_END_TIME"), Qt::Horizontal, tr("TAP_END_TIME"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("REFINE_ST1"), Qt::Horizontal, tr("REFINE_ST1"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("REFINE_ST2"), Qt::Horizontal, tr("REFINE_ST2"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("REFINE_ST3"), Qt::Horizontal, tr("REFINE_ST3"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("REFINE_ST4"), Qt::Horizontal, tr("REFINE_ST4"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("LADLE_ARRIVE_TIME"), Qt::Horizontal, tr("LADLE_ARRIVE_TIME"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("LADLE_START_TIME"), Qt::Horizontal, tr("LADLE_START_TIME"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("LADLE_END_TIME"), Qt::Horizontal, tr("LADLE_END_TIME"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("LADLE_EARLY_TIME"), Qt::Horizontal, tr("LADLE_EARLY_TIME"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("PLAN_STEEL_WEIGHT"), Qt::Horizontal, tr("PLAN_STEEL_WEIGHT"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("PLAN_STATUS_DESC"), Qt::Horizontal, tr("PLAN_STATUS_DESC"));
    planTableModel->setHeaderData(planTableModel->fieldIndex("DS_CODE"), Qt::Horizontal, tr("DS_CODE"));
    // order by field index planTableView
    int i=0;
    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("PLAN_NO")),i++);
    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("PRODUCTION_ID")),i++);
    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("ST_NO")),i++);

    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("PLAN_STEEL_WEIGHT")),i++);
    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("DS_CODE")),i++);

    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("DES_END_TIME")),i++);

    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("BOF_NO")),i++);
    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("SMELT_TYPE")),i++);
    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("MAT_LOAD_START_TIME")),i++);
    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("BLOW_START_TIME")),i++);
    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("BLOW_END_TIME")),i++);
    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("TAP_START_TIME")),i++);
    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("TAP_END_TIME")),i++);

    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("ROUTE_ID")),i++);
    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("REFINE_ST1")),i++);
    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("REFINE_ST2")),i++);
    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("REFINE_ST3")),i++);
    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("REFINE_ST4")),i++);

    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("CCM_NO")),i++);
    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("CAST_NO")),i++);
    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("CAST_TOTAL_HEATS")),i++);
    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("CAST_DIV_NO")),i++);
    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("LADLE_ARRIVE_TIME")),i++);
    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("LADLE_START_TIME")),i++);
    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("LADLE_END_TIME")),i++);
    ui->planTableView->horizontalHeader()->moveSection(ui->planTableView->horizontalHeader()->visualIndex(planTableModel->fieldIndex("LADLE_EARLY_TIME")),i++);

    // order by field index planTableView_2
    i=0;
    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("PLAN_NO")),i++);
    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("PRODUCTION_ID")),i++);
    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("ST_NO")),i++);

    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("PLAN_STEEL_WEIGHT")),i++);

    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("DES_END_TIME")),i++);

    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("SMELT_TYPE")),i++);
    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("BOF_NO")),i++);
    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("MAT_LOAD_START_TIME")),i++);
    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("BLOW_START_TIME")),i++);
    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("BLOW_END_TIME")),i++);
    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("TAP_START_TIME")),i++);
    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("TAP_END_TIME")),i++);

    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("ROUTE_ID")),i++);
    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("REFINE_ST1")),i++);
    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("REFINE_ST2")),i++);
    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("REFINE_ST3")),i++);
    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("REFINE_ST4")),i++);

    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("CCM_NO")),i++);
    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("CAST_NO")),i++);
    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("CAST_TOTAL_HEATS")),i++);
    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("CAST_DIV_NO")),i++);
    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("LADLE_ARRIVE_TIME")),i++);
    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("LADLE_START_TIME")),i++);
    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("LADLE_END_TIME")),i++);
    ui->planTableView_2->horizontalHeader()->moveSection(ui->planTableView_2->horizontalHeader()->visualIndex(planTableModel->fieldIndex("LADLE_EARLY_TIME")),i++);

    //setColumnWidth planTableView
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("PLAN_NO"),80);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("PRODUCTION_ID"),80);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("ST_NO"),100);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("ROUTE_ID"),60);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("SMELT_TYPE"),80);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("BOF_NO"),70);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("CCM_NO"),60);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("CAST_NO"),60);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("CAST_TOTAL_HEATS"),60);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("CAST_DIV_NO"),60);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("DES_END_TIME"),100);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("MAT_LOAD_START_TIME"),100);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("BLOW_START_TIME"),100);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("BLOW_END_TIME"),100);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("TAP_START_TIME"),100);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("TAP_END_TIME"),100);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("REFINE_ST1"),80);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("REFINE_ST2"),80);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("REFINE_ST3"),80);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("REFINE_ST4"),80);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("LADLE_ARRIVE_TIME"),80);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("LADLE_START_TIME"),80);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("LADLE_END_TIME"),80);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("LADLE_EARLY_TIME"),70);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("PLAN_STEEL_WEIGHT"),90);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("PLAN_STATUS"),80);
    ui->planTableView->setColumnWidth(planTableModel->fieldIndex("DS_CODE"),80);

    //setColumnWidth planTableView_2
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("PLAN_NO"),80);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("PRODUCTION_ID"),80);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("ST_NO"),100);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("ROUTE_ID"),60);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("SMELT_TYPE"),60);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("BOF_NO"),60);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("CCM_NO"),60);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("CAST_NO"),60);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("CAST_TOTAL_HEATS"),60);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("CAST_DIV_NO"),60);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("DES_END_TIME"),80);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("MAT_LOAD_START_TIME"),80);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("BLOW_START_TIME"),80);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("BLOW_END_TIME"),80);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("TAP_START_TIME"),80);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("TAP_END_TIME"),80);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("REFINE_ST1"),80);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("REFINE_ST2"),80);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("REFINE_ST3"),80);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("REFINE_ST4"),80);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("LADLE_ARRIVE_TIME"),80);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("LADLE_START_TIME"),80);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("LADLE_END_TIME"),80);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("LADLE_EARLY_TIME"),70);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("PLAN_STEEL_WEIGHT"),90);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("PLAN_STATUS"),80);
    ui->planTableView_2->setColumnWidth(planTableModel->fieldIndex("DS_CODE"),80);

    //setItemDelegateForColumn planTableView
    ui->planTableView->setItemDelegateForColumn(planTableModel->fieldIndex("DES_END_TIME"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->planTableView->setItemDelegateForColumn(planTableModel->fieldIndex("MAT_LOAD_START_TIME"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->planTableView->setItemDelegateForColumn(planTableModel->fieldIndex("BLOW_START_TIME"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->planTableView->setItemDelegateForColumn(planTableModel->fieldIndex("BLOW_END_TIME"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->planTableView->setItemDelegateForColumn(planTableModel->fieldIndex("TAP_START_TIME"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->planTableView->setItemDelegateForColumn(planTableModel->fieldIndex("TAP_END_TIME"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->planTableView->setItemDelegateForColumn(planTableModel->fieldIndex("REFINE_ST1"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->planTableView->setItemDelegateForColumn(planTableModel->fieldIndex("REFINE_ST2"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->planTableView->setItemDelegateForColumn(planTableModel->fieldIndex("REFINE_ST3"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->planTableView->setItemDelegateForColumn(planTableModel->fieldIndex("REFINE_ST4"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->planTableView->setItemDelegateForColumn(planTableModel->fieldIndex("LADLE_ARRIVE_TIME"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->planTableView->setItemDelegateForColumn(planTableModel->fieldIndex("LADLE_START_TIME"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->planTableView->setItemDelegateForColumn(planTableModel->fieldIndex("LADLE_END_TIME"), new TimeEditDelegate("MM-dd hh:mm"));

    //setItemDelegateForColumn planTableView_2
    ui->planTableView_2->setItemDelegateForColumn(planTableModel->fieldIndex("DES_END_TIME"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->planTableView_2->setItemDelegateForColumn(planTableModel->fieldIndex("MAT_LOAD_START_TIME"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->planTableView_2->setItemDelegateForColumn(planTableModel->fieldIndex("BLOW_START_TIME"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->planTableView_2->setItemDelegateForColumn(planTableModel->fieldIndex("BLOW_END_TIME"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->planTableView_2->setItemDelegateForColumn(planTableModel->fieldIndex("TAP_START_TIME"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->planTableView_2->setItemDelegateForColumn(planTableModel->fieldIndex("TAP_END_TIME"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->planTableView_2->setItemDelegateForColumn(planTableModel->fieldIndex("REFINE_ST1"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->planTableView_2->setItemDelegateForColumn(planTableModel->fieldIndex("REFINE_ST2"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->planTableView_2->setItemDelegateForColumn(planTableModel->fieldIndex("REFINE_ST3"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->planTableView_2->setItemDelegateForColumn(planTableModel->fieldIndex("REFINE_ST4"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->planTableView_2->setItemDelegateForColumn(planTableModel->fieldIndex("LADLE_ARRIVE_TIME"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->planTableView_2->setItemDelegateForColumn(planTableModel->fieldIndex("LADLE_START_TIME"), new TimeEditDelegate("MM-dd hh:mm"));
    ui->planTableView_2->setItemDelegateForColumn(planTableModel->fieldIndex("LADLE_END_TIME"), new TimeEditDelegate("MM-dd hh:mm"));

    ui->planTableView->setItemDelegate(new ToTheMiddleDelegate);
    ui->planTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->planTableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    ui->planTableView_2->setItemDelegate(new ToTheMiddleDelegate);
    ui->planTableView_2->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->planTableView_2->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    //set column hidden planTableView
    ui->planTableView->setColumnHidden(planTableModel->fieldIndex("ROUTE_ID"),true);
    ui->planTableView->setColumnHidden(planTableModel->fieldIndex("CCM_NO"),true);
    ui->planTableView->setColumnHidden(planTableModel->fieldIndex("CAST_NO"),true);
    ui->planTableView->setColumnHidden(planTableModel->fieldIndex("CAST_TOTAL_HEATS"),true);
    ui->planTableView->setColumnHidden(planTableModel->fieldIndex("CAST_DIV_NO"),true);
    ui->planTableView->setColumnHidden(planTableModel->fieldIndex("REFINE_ST1"),true);
    ui->planTableView->setColumnHidden(planTableModel->fieldIndex("REFINE_ST2"),true);
    ui->planTableView->setColumnHidden(planTableModel->fieldIndex("REFINE_ST3"),true);
    ui->planTableView->setColumnHidden(planTableModel->fieldIndex("REFINE_ST4"),true);
    ui->planTableView->setColumnHidden(planTableModel->fieldIndex("LADLE_ARRIVE_TIME"),true);
    ui->planTableView->setColumnHidden(planTableModel->fieldIndex("LADLE_START_TIME"),true);
    ui->planTableView->setColumnHidden(planTableModel->fieldIndex("LADLE_END_TIME"),true);
    ui->planTableView->setColumnHidden(planTableModel->fieldIndex("LADLE_EARLY_TIME"),true);

    //set column hidden planTableView_2
    ui->planTableView_2->setColumnHidden(planTableModel->fieldIndex("DES_END_TIME"),true);
    ui->planTableView_2->setColumnHidden(planTableModel->fieldIndex("SMELT_TYPE"),true);
    ui->planTableView_2->setColumnHidden(planTableModel->fieldIndex("BOF_NO"),true);
    ui->planTableView_2->setColumnHidden(planTableModel->fieldIndex("MAT_LOAD_START_TIME"),true);
    ui->planTableView_2->setColumnHidden(planTableModel->fieldIndex("BLOW_START_TIME"),true);
    ui->planTableView_2->setColumnHidden(planTableModel->fieldIndex("BLOW_END_TIME"),true);
    ui->planTableView_2->setColumnHidden(planTableModel->fieldIndex("TAP_START_TIME"),true);
    ui->planTableView_2->setColumnHidden(planTableModel->fieldIndex("TAP_END_TIME"),true);
    ui->planTableView_2->setColumnHidden(planTableModel->fieldIndex("DS_CODE"),true);


    //
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
    timer->start(10000);

    connect(ui->planTableView,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(showDialog(const QModelIndex &)));
    connect(ui->planTableView_2,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(showDialog(const QModelIndex &)));
    connect(ui->planTableView->verticalScrollBar(),SIGNAL(valueChanged(int))
            ,this,SLOT(plan2TableViewVerticalScrollBarChangeValue(int)));
    connect(ui->planTableView_2->verticalScrollBar(),SIGNAL(valueChanged(int))
            ,this,SLOT(plan1TableViewVerticalScrollBarChangeValue(int)));
    connect(ui->planTableView->horizontalScrollBar(),SIGNAL(valueChanged(int))
            ,this,SLOT(plan2TableViewHorizontalScrollBarChangeValue(int)));
    connect(ui->planTableView_2->horizontalScrollBar(),SIGNAL(valueChanged(int))
            ,this,SLOT(plan1TableViewHorizontalScrollBarChangeValue(int)));

    showPlanDialog = new QDialog();
    planDialog->setupUi(showPlanDialog);
    connect(planDialog->buttonBox,SIGNAL(clicked(QAbstractButton *)),this,SLOT(closeDialog(QAbstractButton *)));
//    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveShop()));
//    connect(ui->revertButton, SIGNAL(clicked()), shopTableModel, SLOT(revertAll()));
//    connect(ui->insertButton, SIGNAL(clicked()), this, SLOT(insertShop()));
}

bool PagePlan::event ( QEvent * )
{
    return true;
}

void PagePlan::refresh()
{
    if(planTableModel)
    {
        planTableModel->select();
    }
}

void PagePlan::showDialog(const QModelIndex &index)
{
    QSqlRecord record = planTableModel->record(index.row());

    //showPlanDialog->setWindowTitle(QVariant(index.row()).toString());
    pushPlanIndex = QVariant(index.row()).toInt();
    planDialog->planNoLable->setText(record.value("PLAN_NO").toString());
    planDialog->productionIdLineEdit->setText(record.value("PRODUCTION_ID").toString());
    planDialog->stNoLineEdit->setText(record.value("ST_NO").toString());
    planDialog->routeIdLineEdit->setText(record.value("ROUTE_ID").toString());
    planDialog->smeltTypeLineEdit->setText(record.value("SMELT_TYPE").toString());
    planDialog->bofNoLineEdit->setText(record.value("BOF_NO").toString());
    planDialog->ccmNoLineEdit->setText(record.value("CCM_NO").toString());
    planDialog->castNoLineEdit->setText(record.value("CAST_NO").toString());
    planDialog->castTotalHeatsLineEdit->setText(record.value("CAST_TOTAL_HEATS").toString());
    planDialog->castDivNoLineEdit->setText(record.value("CAST_DIV_NO").toString());
    planDialog->planSteelWeightLineEdit->setText(record.value("PLAN_STEEL_WEIGHT").toString());
    planDialog->planStatusComboBox->setCurrentIndex(record.value("PLAN_STATUS").toInt());
    /*
    planDialog->LineEdit->setText(record.value("").toString());
    planDialog->LineEdit->setText(record.value("").toString());
    planDialog->LineEdit->setText(record.value("").toString());
    */

    showPlanDialog->exec();
}

void PagePlan::closeDialog(QAbstractButton *button)
{
    if (button)
    {
        if (button->text() == "OK")
        {
            int rowIndex = pushPlanIndex;
            planTableModel->setData(planTableModel->index(rowIndex,planTableModel->fieldIndex("PRODUCTION_ID")),planDialog->productionIdLineEdit->text());
            planTableModel->setData(planTableModel->index(rowIndex,planTableModel->fieldIndex("ST_NO")),planDialog->stNoLineEdit->text());
            planTableModel->setData(planTableModel->index(rowIndex,planTableModel->fieldIndex("ROUTE_ID")),planDialog->routeIdLineEdit->text());
            planTableModel->setData(planTableModel->index(rowIndex,planTableModel->fieldIndex("SMELT_TYPE")),planDialog->smeltTypeLineEdit->text());
            planTableModel->setData(planTableModel->index(rowIndex,planTableModel->fieldIndex("BOF_NO")),planDialog->bofNoLineEdit->text());
            planTableModel->setData(planTableModel->index(rowIndex,planTableModel->fieldIndex("CCM_NO")),planDialog->ccmNoLineEdit->text());
            planTableModel->setData(planTableModel->index(rowIndex,planTableModel->fieldIndex("CAST_NO")),planDialog->castNoLineEdit->text());
            planTableModel->setData(planTableModel->index(rowIndex,planTableModel->fieldIndex("CAST_TOTAL_HEATS")),planDialog->castTotalHeatsLineEdit->text());
            planTableModel->setData(planTableModel->index(rowIndex,planTableModel->fieldIndex("CAST_DIV_NO")),planDialog->castDivNoLineEdit->text());
            planTableModel->setData(planTableModel->index(rowIndex,planTableModel->fieldIndex("PLAN_STEEL_WEIGHT")),planDialog->planSteelWeightLineEdit->text());
            planTableModel->setData(planTableModel->index(rowIndex,planTableModel->fieldIndex("PLAN_STATUS")),planDialog->planStatusComboBox->currentIndex());

            if (planTableModel->submitAll())
            {
                planTableModel->database().commit();
            }
        }

        if (button->text() == "Cancel")
        {

        }
    }
}

void PagePlan::plan1TableViewVerticalScrollBarChangeValue(int value)
{
    int oldValue = ui->planTableView->verticalScrollBar()->value();
    if (value == oldValue)
    {
        return;
    }
    else
    {
        ui->planTableView->verticalScrollBar()->setValue(value);
    }
}

void PagePlan::plan2TableViewVerticalScrollBarChangeValue(int value)
{
    int oldValue = ui->planTableView_2->verticalScrollBar()->value();
    if (value == oldValue)
    {
        return;
    }
    else
    {
        ui->planTableView_2->verticalScrollBar()->setValue(value);
    }
}

void PagePlan::plan1TableViewHorizontalScrollBarChangeValue(int value)
{
    int oldValue = ui->planTableView->horizontalScrollBar()->value();
    if (value == oldValue)
    {
        return;
    }
    else
    {
        ui->planTableView->horizontalScrollBar()->setValue(value);
    }
}

void PagePlan::plan2TableViewHorizontalScrollBarChangeValue(int value)
{
    int oldValue = ui->planTableView_2->horizontalScrollBar()->value();
    if (value == oldValue)
    {
        return;
    }
    else
    {
        ui->planTableView_2->horizontalScrollBar()->setValue(value);
    }
}

//void PagePlan::saveShop()
//{
//    shopTableModel->database().transaction();
//    if (shopTableModel->submitAll()) {
//        shopTableModel->database().commit();
//    } else {
//        shopTableModel->database().rollback();
//        //TODO this warning compile error
//        QMessageBox::warning(0, tr("Table shop"),
//                             tr("The database reported an error: %1")
//                             .arg(shopTableModel->lastError().text()),QMessageBox::Ok,QMessageBox::Cancel);
//    }
//}

//void PagePlan::insertShop()
//{
//    int rowNum = shopTableModel->rowCount();
//    shopTableModel->insertRow(rowNum);
//}
