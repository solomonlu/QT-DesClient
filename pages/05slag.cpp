#include <QtSql>
#include "util/TimeEditDelegate.h"
#include "05slag.h"
#include "ui_mainwindow.h"
#include "util/ToTheMiddleDelegate.h"

PageSlag::PageSlag(Ui::MainWindow *pui,QObject *parent)
    : QObject(parent), ui(pui)
{
    stirInfo1Model = new QSqlRelationalTableModel(this);
    stirInfo1Model->setTable("V_STIR_INFO");
    stirInfo1Model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    stirInfo1Model->setSort(stirInfo1Model->fieldIndex("HM_NO"),Qt::DescendingOrder);
    stirInfo1Model->select();
    stirInfo1Model->setFilter("STATION_NO = 'D4'");
    ui->stirInfo1TableView->setModel(stirInfo1Model);

    int i=0;
    stirInfo1Model->setHeaderData(i++, Qt::Horizontal, tr("HM_NO"));
    stirInfo1Model->setHeaderData(i++, Qt::Horizontal, tr("STATION_NO"));
    stirInfo1Model->setHeaderData(i++, Qt::Horizontal, tr("STIR_NO"));
    stirInfo1Model->setHeaderData(i++, Qt::Horizontal, tr("STIR_TIMES"));
    stirInfo1Model->setHeaderData(i++, Qt::Horizontal, tr("LIQUID_LEVEL"));
    stirInfo1Model->setHeaderData(i++, Qt::Horizontal, tr("STIR_SPEED_LOW"));
    stirInfo1Model->setHeaderData(i++, Qt::Horizontal, tr("STIR_SPEED_MEDIUM"));
    stirInfo1Model->setHeaderData(i++, Qt::Horizontal, tr("STIR_SPEED_HIGH"));
    stirInfo1Model->setHeaderData(i++, Qt::Horizontal, tr("HIGH_SPEED_TIME"));

    i=0;
    ui->stirInfo1TableView->setColumnWidth(i++,100);
    ui->stirInfo1TableView->setColumnWidth(i++,80);
    ui->stirInfo1TableView->setColumnWidth(i++,80);
    ui->stirInfo1TableView->setColumnWidth(i++,80);
    ui->stirInfo1TableView->setColumnWidth(i++,100);
    ui->stirInfo1TableView->setColumnWidth(i++,80);
    ui->stirInfo1TableView->setColumnWidth(i++,80);
    ui->stirInfo1TableView->setColumnWidth(i++,80);
    ui->stirInfo1TableView->setColumnWidth(i++,80);


    ui->stirInfo1TableView->setItemDelegate(new ToTheMiddleDelegate);

    ui->stirInfo1TableView->setColumnHidden(1,true);

    ui->stirInfo1TableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->stirInfo1TableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    stirInfo1Model->select();

    stirInfo2Model = new QSqlRelationalTableModel(this);
    stirInfo2Model->setTable("V_STIR_INFO");
    stirInfo2Model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    stirInfo2Model->setSort(stirInfo2Model->fieldIndex("HM_NO"),Qt::DescendingOrder);
    stirInfo1Model->select();
    stirInfo2Model->setFilter("STATION_NO = 'D5'");
    ui->stirInfo2TableView->setModel(stirInfo2Model);

    i=0;
    stirInfo2Model->setHeaderData(i++, Qt::Horizontal, tr("HM_NO"));
    stirInfo2Model->setHeaderData(i++, Qt::Horizontal, tr("STATION_NO"));
    stirInfo2Model->setHeaderData(i++, Qt::Horizontal, tr("STIR_NO"));
    stirInfo2Model->setHeaderData(i++, Qt::Horizontal, tr("STIR_TIMES"));
    stirInfo2Model->setHeaderData(i++, Qt::Horizontal, tr("LIQUID_LEVEL"));
    stirInfo2Model->setHeaderData(i++, Qt::Horizontal, tr("STIR_SPEED_LOW"));
    stirInfo2Model->setHeaderData(i++, Qt::Horizontal, tr("STIR_SPEED_MEDIUM"));
    stirInfo2Model->setHeaderData(i++, Qt::Horizontal, tr("STIR_SPEED_HIGH"));
    stirInfo2Model->setHeaderData(i++, Qt::Horizontal, tr("HIGH_SPEED_TIME"));

    i=0;
    ui->stirInfo2TableView->setColumnWidth(i++,100);
    ui->stirInfo2TableView->setColumnWidth(i++,80);
    ui->stirInfo2TableView->setColumnWidth(i++,80);
    ui->stirInfo2TableView->setColumnWidth(i++,80);
    ui->stirInfo2TableView->setColumnWidth(i++,100);
    ui->stirInfo2TableView->setColumnWidth(i++,80);
    ui->stirInfo2TableView->setColumnWidth(i++,80);
    ui->stirInfo2TableView->setColumnWidth(i++,80);
    ui->stirInfo2TableView->setColumnWidth(i++,80);

    ui->stirInfo2TableView->setItemDelegate(new ToTheMiddleDelegate);

    ui->stirInfo2TableView->setColumnHidden(1,true);

    ui->stirInfo2TableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->stirInfo2TableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    stirInfo2Model->select();

    connect(ui->stirInfo1TableView,SIGNAL(clicked(const QModelIndex &)),this,SLOT(selectSlag1(const QModelIndex &)));
    connect(ui->stirInfo2TableView,SIGNAL(clicked(const QModelIndex &)),this,SLOT(selectSlag2(const QModelIndex &)));
}

bool PageSlag::event ( QEvent * )
{
    return true;
}

void PageSlag::selectSlag1(const QModelIndex &index)
{
    QSqlRecord record = stirInfo1Model->record(index.row());
    slag1Model = new QSqlRelationalTableModel(this);
    slag1Model->setTable("V_SLAG_RESULT");
    slag1Model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    slag1Model->select();
    slag1Model->setFilter("STATION_NO = 'D4'");
    slag1Model->setFilter("HM_NO = " + record.value("HM_NO").toString());
    ui->slag1TableView->setModel(slag1Model);

    slag1Model->setHeaderData(slag1Model->fieldIndex("HM_NO"), Qt::Horizontal, tr("HM_NO"));
    slag1Model->setHeaderData(slag1Model->fieldIndex("STATION_NO"), Qt::Horizontal, tr("STATION_NO"));
    slag1Model->setHeaderData(slag1Model->fieldIndex("SLAG_TYPE"), Qt::Horizontal, tr("SLAG_TYPE"));
    slag1Model->setHeaderData(slag1Model->fieldIndex("SLAG_START_TIME"), Qt::Horizontal, tr("SLAG_START_TIME"));
    slag1Model->setHeaderData(slag1Model->fieldIndex("SLAG_END_TIME"), Qt::Horizontal, tr("SLAG_END_TIME"));
    slag1Model->setHeaderData(slag1Model->fieldIndex("SLAG_TIME"), Qt::Horizontal, tr("SLAG_TIME"));
    slag1Model->setHeaderData(slag1Model->fieldIndex("FINAL_TEMP_TIME"), Qt::Horizontal, tr("FINAL_TEMP_TIME"));
    slag1Model->setHeaderData(slag1Model->fieldIndex("FINAL_TEMP"), Qt::Horizontal, tr("FINAL_TEMP"));
    slag1Model->setHeaderData(slag1Model->fieldIndex("SLAG_WEIGHT"), Qt::Horizontal, tr("SLAG_WEIGHT"));

    ui->slag1TableView->setColumnWidth(slag1Model->fieldIndex("HM_NO"),80);
    ui->slag1TableView->setColumnWidth(slag1Model->fieldIndex("STATION_NO"),80);
    ui->slag1TableView->setColumnWidth(slag1Model->fieldIndex("SLAG_TYPE"),80);
    ui->slag1TableView->setColumnWidth(slag1Model->fieldIndex("SLAG_START_TIME"),100);
    ui->slag1TableView->setColumnWidth(slag1Model->fieldIndex("SLAG_END_TIME"),100);
    ui->slag1TableView->setColumnWidth(slag1Model->fieldIndex("SLAG_TIME"),80);
    ui->slag1TableView->setColumnWidth(slag1Model->fieldIndex("FINAL_TEMP_TIME"),100);
    ui->slag1TableView->setColumnWidth(slag1Model->fieldIndex("FINAL_TEMP"),80);
    ui->slag1TableView->setColumnWidth(slag1Model->fieldIndex("SLAG_WEIGHT"),80);

    ui->slag1TableView->setItemDelegateForColumn(slag1Model->fieldIndex("SLAG_START_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));
    ui->slag1TableView->setItemDelegateForColumn(slag1Model->fieldIndex("SLAG_END_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));
    ui->slag1TableView->setItemDelegateForColumn(slag1Model->fieldIndex("FINAL_TEMP_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));

    ui->slag1TableView->setColumnHidden(slag1Model->fieldIndex("STATION_NO"),true);

    ui->slag1TableView->setItemDelegate(new ToTheMiddleDelegate);

    ui->slag1TableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->slag1TableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    slag1Model->setRelation(slag1Model->fieldIndex("SLAG_TYPE"), QSqlRelation("SLAG_MAP", "SLAG_TYPE", "SLAG_DESC"));

    slag1Model->select();
}

void PageSlag::selectSlag2(const QModelIndex &index)
{
    QSqlRecord record = stirInfo2Model->record(index.row());

    slag2Model = new QSqlRelationalTableModel(this);
    slag2Model->setTable("V_SLAG_RESULT");
    slag2Model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    slag2Model->select();
    slag2Model->setFilter("STATION_NO = 'D5'");
    slag2Model->setFilter("HM_NO = " + record.value("HM_NO").toString());
    ui->slag2TableView->setModel(slag2Model);

    slag2Model->setHeaderData(slag2Model->fieldIndex("HM_NO"), Qt::Horizontal, tr("HM_NO"));
    slag2Model->setHeaderData(slag2Model->fieldIndex("STATION_NO"), Qt::Horizontal, tr("STATION_NO"));
    slag2Model->setHeaderData(slag2Model->fieldIndex("SLAG_TYPE"), Qt::Horizontal, tr("SLAG_TYPE"));
    slag2Model->setHeaderData(slag2Model->fieldIndex("SLAG_START_TIME"), Qt::Horizontal, tr("SLAG_START_TIME"));
    slag2Model->setHeaderData(slag2Model->fieldIndex("SLAG_END_TIME"), Qt::Horizontal, tr("SLAG_END_TIME"));
    slag2Model->setHeaderData(slag2Model->fieldIndex("SLAG_TIME"), Qt::Horizontal, tr("SLAG_TIME"));
    slag2Model->setHeaderData(slag2Model->fieldIndex("FINAL_TEMP_TIME"), Qt::Horizontal, tr("FINAL_TEMP_TIME"));
    slag2Model->setHeaderData(slag2Model->fieldIndex("FINAL_TEMP"), Qt::Horizontal, tr("FINAL_TEMP"));
    slag2Model->setHeaderData(slag2Model->fieldIndex("SLAG_WEIGHT"), Qt::Horizontal, tr("SLAG_WEIGHT"));

    ui->slag2TableView->setColumnWidth(slag2Model->fieldIndex("HM_NO"),80);
    ui->slag2TableView->setColumnWidth(slag2Model->fieldIndex("STATION_NO"),80);
    ui->slag2TableView->setColumnWidth(slag2Model->fieldIndex("SLAG_TYPE"),80);
    ui->slag2TableView->setColumnWidth(slag2Model->fieldIndex("SLAG_START_TIME"),100);
    ui->slag2TableView->setColumnWidth(slag2Model->fieldIndex("SLAG_END_TIME"),100);
    ui->slag2TableView->setColumnWidth(slag2Model->fieldIndex("SLAG_TIME"),80);
    ui->slag2TableView->setColumnWidth(slag2Model->fieldIndex("FINAL_TEMP_TIME"),100);
    ui->slag2TableView->setColumnWidth(slag2Model->fieldIndex("FINAL_TEMP"),80);
    ui->slag2TableView->setColumnWidth(slag2Model->fieldIndex("SLAG_WEIGHT"),80);

    ui->slag2TableView->setItemDelegateForColumn(slag2Model->fieldIndex("SLAG_START_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));
    ui->slag2TableView->setItemDelegateForColumn(slag2Model->fieldIndex("SLAG_END_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));
    ui->slag2TableView->setItemDelegateForColumn(slag2Model->fieldIndex("FINAL_TEMP_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));

    ui->slag2TableView->setColumnHidden(slag2Model->fieldIndex("STATION_NO"),true);

    ui->slag2TableView->setItemDelegate(new ToTheMiddleDelegate);

    ui->slag2TableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->slag2TableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    slag2Model->setRelation(slag2Model->fieldIndex("SLAG_TYPE"), QSqlRelation("SLAG_MAP", "SLAG_TYPE", "SLAG_DESC"));

    slag2Model->select();
}

void PageSlag::refresh()
{
    stirInfo1Model->select();
    stirInfo2Model->select();
}
