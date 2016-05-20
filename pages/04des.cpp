#include <QtSql>
#include "util/TimeEditDelegate.h"
#include <QtSql>
#include "04des.h"
#include "ui_mainwindow.h"
#include "util/ToTheMiddleDelegate.h"

PageDes::PageDes(Ui::MainWindow *pui,QObject *parent)
    : QObject(parent), ui(pui)
{
    //ui->page3PasswordLineEdit->setStyleSheet("lineedit-password-character: 42");
    //ui->page3PasswordLineEdit->setEchoMode (QLineEdit::Password);
    des1Model = new QSqlRelationalTableModel(this);
    des1Model->setTable("DES_RESULT");
    des1Model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    des1Model->setSort(des1Model->fieldIndex("HM_NO"),Qt::DescendingOrder);
    des1Model->setRelation(des1Model->fieldIndex("SHIFT_ID"), QSqlRelation("SHIFT_MAP", "SHIFT", "SHIFT_DESC"));
    des1Model->setRelation(des1Model->fieldIndex("CREW_ID"), QSqlRelation("CREW_MAP", "CREW", "CREW_DESC"));
    des1Model->setRelation(des1Model->fieldIndex("DS_CODE"), QSqlRelation("DES_MAP", "DS_CODE", "DS_DESC"));
    des1Model->select();
    des1Model->setFilter("STATION_NO = 'D4'");
    ui->des1TableView->setModel(des1Model);

    des1Model->setHeaderData(des1Model->fieldIndex("HM_NO"), Qt::Horizontal, tr("HM_NO"));
    des1Model->setHeaderData(des1Model->fieldIndex("PLAN_NO"), Qt::Horizontal, tr("PLAN_NO"));
    des1Model->setHeaderData(des1Model->fieldIndex("PRODUCTION_ID"), Qt::Horizontal, tr("PRODUCTION_ID"));
    des1Model->setHeaderData(des1Model->fieldIndex("ST_NO"), Qt::Horizontal, tr("ST_NO"));
    des1Model->setHeaderData(des1Model->fieldIndex("STATION_NO"), Qt::Horizontal, tr("STATION_NO"));
    des1Model->setHeaderData(des1Model->fieldIndex("SHIFT_ID"), Qt::Horizontal, tr("SHIFT_ID"));
    des1Model->setHeaderData(des1Model->fieldIndex("CREW_ID"), Qt::Horizontal, tr("CREW_ID"));
    des1Model->setHeaderData(des1Model->fieldIndex("SHIFT_DESC"), Qt::Horizontal, tr("SHIFT_DESC"));
    des1Model->setHeaderData(des1Model->fieldIndex("CREW_DESC"), Qt::Horizontal, tr("CREW_DESC"));
    des1Model->setHeaderData(des1Model->fieldIndex("TPD_NO"), Qt::Horizontal, tr("TPD_NO"));
    des1Model->setHeaderData(des1Model->fieldIndex("DS_CODE"), Qt::Horizontal, tr("DS_CODE"));
    des1Model->setHeaderData(des1Model->fieldIndex("DS_DESC"), Qt::Horizontal, tr("DS_DESC"));
    des1Model->setHeaderData(des1Model->fieldIndex("IRON_LADLE_ID"), Qt::Horizontal, tr("IRON_LADLE_ID"));
    des1Model->setHeaderData(des1Model->fieldIndex("INIT_TEMP_TIME"), Qt::Horizontal, tr("INIT_TEMP_TIME"));
    des1Model->setHeaderData(des1Model->fieldIndex("INIT_TEMP"), Qt::Horizontal, tr("INIT_TEMP"));
    des1Model->setHeaderData(des1Model->fieldIndex("FINAL_TEMP_TIME"), Qt::Horizontal, tr("FINAL_TEMP_TIME"));
    des1Model->setHeaderData(des1Model->fieldIndex("FINAL_TEMP"), Qt::Horizontal, tr("FINAL_TEMP"));
    des1Model->setHeaderData(des1Model->fieldIndex("INIT_S"), Qt::Horizontal, tr("INIT_S"));
    des1Model->setHeaderData(des1Model->fieldIndex("FINAL_S"), Qt::Horizontal, tr("FINAL_S"));
    des1Model->setHeaderData(des1Model->fieldIndex("INIT_WEIGHT"), Qt::Horizontal, tr("INIT_WEIGHT"));
    des1Model->setHeaderData(des1Model->fieldIndex("FINAL_WEIGHT"), Qt::Horizontal, tr("FINAL_WEIGHT"));
    des1Model->setHeaderData(des1Model->fieldIndex("LADLE_TARE_WEIGHT"), Qt::Horizontal, tr("LADLE_TARE_WEIGHT"));
    des1Model->setHeaderData(des1Model->fieldIndex("DES_TIME"), Qt::Horizontal, tr("DES_TIME"));
    des1Model->setHeaderData(des1Model->fieldIndex("HM_TIME"), Qt::Horizontal, tr("HM_TIME"));

    ui->des1TableView->setColumnWidth(des1Model->fieldIndex("HM_NO"),80);
    ui->des1TableView->setColumnWidth(des1Model->fieldIndex("PLAN_NO"),80);
    ui->des1TableView->setColumnWidth(des1Model->fieldIndex("PRODUCTION_ID"),80);
    ui->des1TableView->setColumnWidth(des1Model->fieldIndex("ST_NO"),120);
    ui->des1TableView->setColumnWidth(des1Model->fieldIndex("STATION_NO"),80);
    ui->des1TableView->setColumnWidth(des1Model->fieldIndex("SHIFT_ID"),40);
    ui->des1TableView->setColumnWidth(des1Model->fieldIndex("CREW_ID"),40);
    ui->des1TableView->setColumnWidth(des1Model->fieldIndex("SHIFT_DESC"),55);
    ui->des1TableView->setColumnWidth(des1Model->fieldIndex("CREW_DESC"),40);
    ui->des1TableView->setColumnWidth(des1Model->fieldIndex("TPD_NO"),80);
    ui->des1TableView->setColumnWidth(des1Model->fieldIndex("DS_CODE"),55);
    ui->des1TableView->setColumnWidth(des1Model->fieldIndex("DS_DESC"),55);
    ui->des1TableView->setColumnWidth(des1Model->fieldIndex("IRON_LADLE_ID"),70);
    ui->des1TableView->setColumnWidth(des1Model->fieldIndex("INIT_TEMP_TIME"),95);
    ui->des1TableView->setColumnWidth(des1Model->fieldIndex("INIT_TEMP"),55);
    ui->des1TableView->setColumnWidth(des1Model->fieldIndex("FINAL_TEMP_TIME"),95);
    ui->des1TableView->setColumnWidth(des1Model->fieldIndex("FINAL_TEMP"),55);
    ui->des1TableView->setColumnWidth(des1Model->fieldIndex("INIT_S"),80);
    ui->des1TableView->setColumnWidth(des1Model->fieldIndex("FINAL_S"),80);
    ui->des1TableView->setColumnWidth(des1Model->fieldIndex("INIT_WEIGHT"),70);
    ui->des1TableView->setColumnWidth(des1Model->fieldIndex("FINAL_WEIGHT"),70);
    ui->des1TableView->setColumnWidth(des1Model->fieldIndex("LADLE_TARE_WEIGHT"),70);
    ui->des1TableView->setColumnWidth(des1Model->fieldIndex("DES_TIME"),70);
    ui->des1TableView->setColumnWidth(des1Model->fieldIndex("HM_TIME"),70);

    ui->des1TableView->setItemDelegateForColumn(des1Model->fieldIndex("INIT_TEMP_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));
    ui->des1TableView->setItemDelegateForColumn(des1Model->fieldIndex("FINAL_TEMP_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));
    ui->des1TableView->setItemDelegate(new ToTheMiddleDelegate);

    ui->des1TableView->setColumnHidden(des1Model->fieldIndex("STATION_NO"),true);
    ui->des1TableView->setColumnHidden(des1Model->fieldIndex("DS_DESC"),true);

    ui->des1TableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->des1TableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    ui->des1TableView->show();



    des2Model = new QSqlRelationalTableModel(this);
    des2Model->setTable("DES_RESULT");
    des2Model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    des2Model->setSort(des2Model->fieldIndex("HM_NO"),Qt::DescendingOrder);
    des2Model->setRelation(des2Model->fieldIndex("SHIFT_ID"), QSqlRelation("SHIFT_MAP", "SHIFT", "SHIFT_DESC"));
    des2Model->setRelation(des2Model->fieldIndex("CREW_ID"), QSqlRelation("CREW_MAP", "CREW", "CREW_DESC"));
    des2Model->setRelation(des2Model->fieldIndex("DS_CODE"), QSqlRelation("DES_MAP", "DS_CODE", "DS_DESC"));
    des2Model->select();
    des2Model->setFilter("STATION_NO = 'D5'");
    ui->des2TableView->setModel(des2Model);

    des2Model->setHeaderData(des2Model->fieldIndex("HM_NO"), Qt::Horizontal, tr("HM_NO"));
    des2Model->setHeaderData(des2Model->fieldIndex("PLAN_NO"), Qt::Horizontal, tr("PLAN_NO"));
    des2Model->setHeaderData(des2Model->fieldIndex("PRODUCTION_ID"), Qt::Horizontal, tr("PRODUCTION_ID"));
    des2Model->setHeaderData(des2Model->fieldIndex("ST_NO"), Qt::Horizontal, tr("ST_NO"));
    des2Model->setHeaderData(des2Model->fieldIndex("STATION_NO"), Qt::Horizontal, tr("STATION_NO"));
    des2Model->setHeaderData(des2Model->fieldIndex("SHIFT_ID"), Qt::Horizontal, tr("SHIFT_ID"));
    des2Model->setHeaderData(des2Model->fieldIndex("CREW_ID"), Qt::Horizontal, tr("CREW_ID"));
    des2Model->setHeaderData(des2Model->fieldIndex("SHIFT_DESC"), Qt::Horizontal, tr("SHIFT_DESC"));
    des2Model->setHeaderData(des2Model->fieldIndex("CREW_DESC"), Qt::Horizontal, tr("CREW_DESC"));
    des2Model->setHeaderData(des2Model->fieldIndex("TPD_NO"), Qt::Horizontal, tr("TPD_NO"));
    des2Model->setHeaderData(des2Model->fieldIndex("DS_CODE"), Qt::Horizontal, tr("DS_CODE"));
    des2Model->setHeaderData(des2Model->fieldIndex("DS_DESC"), Qt::Horizontal, tr("DS_DESC"));
    des2Model->setHeaderData(des2Model->fieldIndex("IRON_LADLE_ID"), Qt::Horizontal, tr("IRON_LADLE_ID"));
    des2Model->setHeaderData(des2Model->fieldIndex("INIT_TEMP_TIME"), Qt::Horizontal, tr("INIT_TEMP_TIME"));
    des2Model->setHeaderData(des2Model->fieldIndex("INIT_TEMP"), Qt::Horizontal, tr("INIT_TEMP"));
    des2Model->setHeaderData(des2Model->fieldIndex("FINAL_TEMP_TIME"), Qt::Horizontal, tr("FINAL_TEMP_TIME"));
    des2Model->setHeaderData(des2Model->fieldIndex("FINAL_TEMP"), Qt::Horizontal, tr("FINAL_TEMP"));
    des2Model->setHeaderData(des2Model->fieldIndex("INIT_S"), Qt::Horizontal, tr("INIT_S"));
    des2Model->setHeaderData(des2Model->fieldIndex("FINAL_S"), Qt::Horizontal, tr("FINAL_S"));
    des2Model->setHeaderData(des2Model->fieldIndex("INIT_WEIGHT"), Qt::Horizontal, tr("INIT_WEIGHT"));
    des2Model->setHeaderData(des2Model->fieldIndex("FINAL_WEIGHT"), Qt::Horizontal, tr("FINAL_WEIGHT"));
    des2Model->setHeaderData(des2Model->fieldIndex("LADLE_TARE_WEIGHT"), Qt::Horizontal, tr("LADLE_TARE_WEIGHT"));
    des2Model->setHeaderData(des2Model->fieldIndex("DES_TIME"), Qt::Horizontal, tr("DES_TIME"));
    des2Model->setHeaderData(des2Model->fieldIndex("HM_TIME"), Qt::Horizontal, tr("HM_TIME"));

    ui->des2TableView->setColumnWidth(des2Model->fieldIndex("HM_NO"),80);
    ui->des2TableView->setColumnWidth(des2Model->fieldIndex("PLAN_NO"),80);
    ui->des2TableView->setColumnWidth(des2Model->fieldIndex("PRODUCTION_ID"),80);
    ui->des2TableView->setColumnWidth(des2Model->fieldIndex("ST_NO"),120);
    ui->des2TableView->setColumnWidth(des2Model->fieldIndex("STATION_NO"),80);
    ui->des2TableView->setColumnWidth(des2Model->fieldIndex("SHIFT_ID"),40);
    ui->des2TableView->setColumnWidth(des2Model->fieldIndex("CREW_ID"),40);
    ui->des2TableView->setColumnWidth(des2Model->fieldIndex("SHIFT_DESC"),55);
    ui->des2TableView->setColumnWidth(des2Model->fieldIndex("CREW_DESC"),40);
    ui->des2TableView->setColumnWidth(des2Model->fieldIndex("TPD_NO"),80);
    ui->des2TableView->setColumnWidth(des2Model->fieldIndex("DS_CODE"),55);
    ui->des2TableView->setColumnWidth(des2Model->fieldIndex("DS_DESC"),55);
    ui->des2TableView->setColumnWidth(des2Model->fieldIndex("IRON_LADLE_ID"),70);
    ui->des2TableView->setColumnWidth(des2Model->fieldIndex("INIT_TEMP_TIME"),95);
    ui->des2TableView->setColumnWidth(des2Model->fieldIndex("INIT_TEMP"),55);
    ui->des2TableView->setColumnWidth(des2Model->fieldIndex("FINAL_TEMP_TIME"),95);
    ui->des2TableView->setColumnWidth(des2Model->fieldIndex("FINAL_TEMP"),55);
    ui->des2TableView->setColumnWidth(des2Model->fieldIndex("INIT_S"),80);
    ui->des2TableView->setColumnWidth(des2Model->fieldIndex("FINAL_S"),80);
    ui->des2TableView->setColumnWidth(des2Model->fieldIndex("INIT_WEIGHT"),70);
    ui->des2TableView->setColumnWidth(des2Model->fieldIndex("FINAL_WEIGHT"),70);
    ui->des2TableView->setColumnWidth(des2Model->fieldIndex("LADLE_TARE_WEIGHT"),70);
    ui->des2TableView->setColumnWidth(des2Model->fieldIndex("DES_TIME"),70);
    ui->des2TableView->setColumnWidth(des2Model->fieldIndex("HM_TIME"),70);

    ui->des2TableView->setItemDelegateForColumn(des2Model->fieldIndex("INIT_TEMP_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));
    ui->des2TableView->setItemDelegateForColumn(des2Model->fieldIndex("FINAL_TEMP_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));
    ui->des2TableView->setItemDelegate(new ToTheMiddleDelegate);

    ui->des2TableView->setColumnHidden(des2Model->fieldIndex("STATION_NO"),true);
    ui->des2TableView->setColumnHidden(des2Model->fieldIndex("DS_DESC"),true);

    ui->des2TableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->des2TableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    ui->des2TableView->show();
}

bool PageDes::event ( QEvent * )
{
    return true;
}

void PageDes::refresh()
{
    des1Model->select();
    des2Model->select();
}

