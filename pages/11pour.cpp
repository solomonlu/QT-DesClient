#include <QtSql>
#include <QMessageBox>
#include "11pour.h"
#include "ui_mainwindow.h"
#include "util/ToTheMiddleDelegate.h"
#include "util/TimeEditDelegate.h"
#include "util/ReadOnlyDelegate.h"

PagePour::PagePour(Ui::MainWindow *pui,QObject *parent)
    : QObject(parent), ui(pui)
{
    ironInfoModel = new QSqlTableModel(this);
    ironInfoModel->setTable("V_IRON_INFO_CURRENT");
    ironInfoModel->setSort(ironInfoModel->fieldIndex("SAMPLE_TIME"),Qt::DescendingOrder);
    ironInfoModel->select();
    ui->ironInfoTableView->setModel(ironInfoModel);
    int i=0;
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("BF_NO"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("LADLE_NO"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("TARE_WEIGHT"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("GROSS_WEIGHT"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("IRON_WEIGHT"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("SAMPLE_ID"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("SAMPLE_TIME"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("SAMPLE_PLACE"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("REP_SAMPLE_FLAG"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_C"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_SI"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_MN"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_P"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_S"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_CU"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_V"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_NB"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_CR"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_NI"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_MO"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_SN"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_N"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_ALT"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_ALS"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_B"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_TI"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_CA"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_PB"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_SB"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_ZN"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_AS"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_CO"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_MG"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_ZR"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_W"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_TA"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_TE"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_RE"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_BI"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_SE"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_BA"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_OT"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_OA"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_H"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("SPARE1"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("SPARE2"));
    ironInfoModel->setHeaderData(i++, Qt::Horizontal, tr("POUR_IN_COUNT"));

    ui->ironInfoTableView->setColumnWidth(0,70);
    ui->ironInfoTableView->setColumnWidth(1,70);
    ui->ironInfoTableView->setColumnWidth(2,70);
    ui->ironInfoTableView->setColumnWidth(3,70);
    ui->ironInfoTableView->setColumnWidth(4,70);
    ui->ironInfoTableView->setColumnWidth(5,100);
    ui->ironInfoTableView->setColumnWidth(6,100);
    ui->ironInfoTableView->setColumnWidth(7,55);
    ui->ironInfoTableView->setColumnWidth(8,55);
    for(i=9;i<47;i++)
    {
        ui->ironInfoTableView->setColumnWidth(i,70);
    }

    ui->ironInfoTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->ironInfoTableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    ui->ironInfoTableView->setItemDelegateForColumn(ironInfoModel->fieldIndex("SAMPLE_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));

    ui->ironInfoTableView->setItemDelegateForColumn(0,new ReadOnlyDelegate(this));
    ui->ironInfoTableView->setItemDelegateForColumn(1,new ReadOnlyDelegate(this));

    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_CU"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_V"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_NB"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_CR"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_NI"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_MO"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_SN"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_N"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_ALT"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_ALS"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_B"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_TI"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_CA"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_PB"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_SB"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_ZN"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_AS"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_CO"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_MG"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_ZR"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_W"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_TA"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_TE"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_RE"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_BI"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_SE"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_BA"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_OT"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_OA"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("ELE_H"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("SPARE1"),true);
    ui->ironInfoTableView->setColumnHidden(ironInfoModel->fieldIndex("SPARE2"),true);

    ui->ironInfoTableView->setItemDelegate(new ToTheMiddleDelegate);
    //----------------------------------------------
    stdIronInfoModel = new QSqlTableModel(this);
    stdIronInfoModel->setTable("V_STD_IRON_INFO_CURRENT");
    stdIronInfoModel->setSort(stdIronInfoModel->fieldIndex("END_TIME"),Qt::DescendingOrder);
    stdIronInfoModel->select();
    ui->stdIronInfoTableView->setModel(stdIronInfoModel);
    i=0;
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("LADLE_NO"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("TARE_WEIGHT"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("GROSS_WEIGHT"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("IRON_WEIGHT"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("SAMPLE_ID"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("SAMPLE_TIME"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("SAMPLE_PLACE"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("REP_SAMPLE_FLAG"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_C"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_SI"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_MN"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_P"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_S"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_CU"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_V"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_NB"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_CR"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_NI"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_MO"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_SN"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_N"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_ALT"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_ALS"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_B"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_TI"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_CA"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_PB"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_SB"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_ZN"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_AS"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_CO"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_MG"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_ZR"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_W"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_TA"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_TE"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_RE"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_BI"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_SE"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_BA"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_OT"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_OA"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_H"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("SPARE1"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("SPARE2"));
    stdIronInfoModel->setHeaderData(i++, Qt::Horizontal, tr("POUR_IN_COUNT"));

    ui->stdIronInfoTableView->setColumnWidth(0,70);
    ui->stdIronInfoTableView->setColumnWidth(1,70);
    ui->stdIronInfoTableView->setColumnWidth(2,70);
    ui->stdIronInfoTableView->setColumnWidth(3,70);
    ui->stdIronInfoTableView->setColumnWidth(4,100);
    ui->stdIronInfoTableView->setColumnWidth(5,100);
    ui->stdIronInfoTableView->setColumnWidth(6,55);
    ui->stdIronInfoTableView->setColumnWidth(7,55);
    for(i=8;i<47;i++)
    {
        ui->stdIronInfoTableView->setColumnWidth(i,70);
    }

    ui->stdIronInfoTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->stdIronInfoTableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    ui->stdIronInfoTableView->setItemDelegateForColumn(stdIronInfoModel->fieldIndex("SAMPLE_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));

    ui->stdIronInfoTableView->setItemDelegateForColumn(0,new ReadOnlyDelegate(this));

    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("SAMPLE_ID"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("SAMPLE_TIME"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("SAMPLE_PLACE"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("REP_SAMPLE_FLAG"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_CU"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_V"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_NB"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_CR"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_NI"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_MO"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_SN"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_N"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_ALT"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_ALS"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_B"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_TI"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_CA"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_PB"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_SB"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_ZN"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_AS"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_CO"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_MG"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_ZR"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_W"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_TA"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_TE"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_RE"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_BI"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_SE"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_BA"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_OT"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_OA"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("ELE_H"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("SPARE1"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("SPARE2"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("END_TIME"),true);
    ui->stdIronInfoTableView->setColumnHidden(stdIronInfoModel->fieldIndex("FIRST_BF_NO_OUT"),true);

    ui->stdIronInfoTableView->setItemDelegate(new ToTheMiddleDelegate);
    //----------------------------------------------
    stdIronInfoHisModel = new QSqlTableModel(this);
    stdIronInfoHisModel->setTable("V_STD_IRON_INFO_FINAL");
    stdIronInfoHisModel->setFilter("END_FLAG=1");
    stdIronInfoHisModel->setSort(stdIronInfoHisModel->fieldIndex("END_TIME"),Qt::DescendingOrder);
    stdIronInfoHisModel->select();
    ui->stdIronInfoHisTableView->setModel(stdIronInfoHisModel);
    i=0;
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("LADLE_NO"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("TARE_WEIGHT"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("GROSS_WEIGHT"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("IRON_WEIGHT"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("SAMPLE_ID"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("SAMPLE_TIME"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("SAMPLE_PLACE"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("REP_SAMPLE_FLAG"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_C"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_SI"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_MN"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_P"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_S"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_CU"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_V"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_NB"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_CR"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_NI"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_MO"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_SN"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_N"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_ALT"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_ALS"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_B"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_TI"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_CA"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_PB"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_SB"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_ZN"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_AS"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_CO"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_MG"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_ZR"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_W"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_TA"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_TE"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_RE"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_BI"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_SE"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_BA"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_OT"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_OA"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_H"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("SPARE1"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("SPARE2"));
    stdIronInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("POUR_IN_COUNT"));

    ui->stdIronInfoHisTableView->setColumnWidth(0,70);
    ui->stdIronInfoHisTableView->setColumnWidth(1,70);
    ui->stdIronInfoHisTableView->setColumnWidth(2,70);
    ui->stdIronInfoHisTableView->setColumnWidth(3,70);
    ui->stdIronInfoHisTableView->setColumnWidth(4,100);
    ui->stdIronInfoHisTableView->setColumnWidth(5,100);
    ui->stdIronInfoHisTableView->setColumnWidth(6,55);
    ui->stdIronInfoHisTableView->setColumnWidth(7,55);
    for(i=8;i<48;i++)
    {
        ui->stdIronInfoHisTableView->setColumnWidth(i,70);
    }

    ui->stdIronInfoHisTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->stdIronInfoHisTableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    ui->stdIronInfoHisTableView->setItemDelegateForColumn(stdIronInfoHisModel->fieldIndex("SAMPLE_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));

    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("SAMPLE_ID"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("SAMPLE_TIME"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("SAMPLE_PLACE"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("REP_SAMPLE_FLAG"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_CU"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_V"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_NB"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_CR"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_NI"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_MO"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_SN"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_N"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_ALT"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_ALS"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_B"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_TI"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_CA"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_PB"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_SB"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_ZN"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_AS"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_CO"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_MG"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_ZR"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_W"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_TA"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_TE"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_RE"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_BI"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_SE"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_BA"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_OT"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_OA"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("ELE_H"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("SPARE1"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("SPARE2"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("END_FLAG"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("END_TIME"),true);
    ui->stdIronInfoHisTableView->setColumnHidden(stdIronInfoHisModel->fieldIndex("FIRST_BF_NO_OUT"),true);

    ui->stdIronInfoHisTableView->setItemDelegate(new ToTheMiddleDelegate);
    //----------------------------------------------
    ui->bfNoComboBox->clear();
    ui->bfNoInComboBox->clear();
    ui->bfNoOutComboBox->clear();
    ui->bfNoOutStdComboBox->clear();
    QSqlQuery bfNoQuery("SELECT BF_NO FROM V_IRON_INFO_CURRENT");
    QStringList bfNoList;
    bfNoList.append("");
    while (bfNoQuery.next())
    {
        QString bfNo = bfNoQuery.value(0).toString();
        bfNoList.append(bfNo);
    }
    ui->bfNoComboBox->addItems(bfNoList);
    ui->bfNoInComboBox->addItems(bfNoList);
    ui->bfNoOutComboBox->addItems(bfNoList);
    ui->bfNoOutStdComboBox->addItems(bfNoList);

    QStringList ladleNoStdList;
    QSqlQuery ladleNoStdQuery("SELECT LADLE_NO FROM V_STD_IRON_INFO_CURRENT ORDER BY END_TIME DESC");
    ladleNoStdList.append("");
    while (ladleNoStdQuery.next())
    {
        QString ladleNoStd = ladleNoStdQuery.value(0).toString();
        ladleNoStdList.append(ladleNoStd);
    }
    ui->ladleNoStdComboBox->clear();
    ui->ladleNoStdComboBox->addItems(ladleNoStdList);

    pourInfoModel = new QSqlTableModel(this);
    pourInfoModel->setTable("POUR_IRON");
    pourInfoModel->setSort(pourInfoModel->fieldIndex("POUR_TIME"),Qt::DescendingOrder);
    pourInfoModel->select();
    ui->pourInfoTableView->setModel(pourInfoModel);

    i=0;
    pourInfoModel->setHeaderData(i++, Qt::Horizontal, tr("POUR_NO"));
    pourInfoModel->setHeaderData(i++, Qt::Horizontal, tr("LADLE_NO_OUT"));
    pourInfoModel->setHeaderData(i++, Qt::Horizontal, tr("BF_NO_OUT"));
    pourInfoModel->setHeaderData(i++, Qt::Horizontal, tr("LADLE_NO_IN"));
    pourInfoModel->setHeaderData(i++, Qt::Horizontal, tr("BF_NO_IN"));
    pourInfoModel->setHeaderData(i++, Qt::Horizontal, tr("POUR_TIME"));
    pourInfoModel->setHeaderData(i++, Qt::Horizontal, tr("POUR_WEIGHT"));
    pourInfoModel->setHeaderData(i++, Qt::Horizontal, tr("POUR_IN_COUNT"));

    i=0;
    ui->pourInfoTableView->setColumnWidth(i++,100);
    ui->pourInfoTableView->setColumnWidth(i++,100);
    ui->pourInfoTableView->setColumnWidth(i++,100);
    ui->pourInfoTableView->setColumnWidth(i++,100);
    ui->pourInfoTableView->setColumnWidth(i++,100);
    ui->pourInfoTableView->setColumnWidth(i++,100);

    ui->pourInfoTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->pourInfoTableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    ui->pourInfoTableView->setItemDelegateForColumn(pourInfoModel->fieldIndex("POUR_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));

    ui->pourInfoTableView->setItemDelegateForColumn(0,new ReadOnlyDelegate(this));
    ui->pourInfoTableView->setItemDelegateForColumn(1,new ReadOnlyDelegate(this));
    ui->pourInfoTableView->setItemDelegateForColumn(2,new ReadOnlyDelegate(this));
    ui->pourInfoTableView->setItemDelegateForColumn(3,new ReadOnlyDelegate(this));
    ui->pourInfoTableView->setItemDelegateForColumn(4,new ReadOnlyDelegate(this));

    ui->pourInfoTableView->setItemDelegate(new ToTheMiddleDelegate);
    //---------------------------------------------------------
    ironInfoHisModel = new QSqlTableModel(this);
    ironInfoHisModel->setTable("V_IRON_INFO_FINAL");
    ironInfoHisModel->setFilter("(BF_NO,LADLE_NO) NOT IN (SELECT BF_NO,LADLE_NO FROM V_IRON_INFO_CURRENT)");
    ironInfoHisModel->setSort(ironInfoHisModel->fieldIndex("SAMPLE_TIME"),Qt::DescendingOrder);
    ironInfoHisModel->select();
    ui->ironInfoHisTableView->setModel(ironInfoHisModel);
    i=0;
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("BF_NO"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("LADLE_NO"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("TARE_WEIGHT"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("GROSS_WEIGHT"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("IRON_WEIGHT"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("SAMPLE_ID"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("SAMPLE_TIME"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("SAMPLE_PLACE"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("REP_SAMPLE_FLAG"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_C"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_SI"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_MN"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_P"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_S"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_CU"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_V"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_NB"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_CR"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_NI"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_MO"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_SN"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_N"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_ALT"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_ALS"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_B"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_TI"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_CA"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_PB"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_SB"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_ZN"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_AS"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_CO"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_MG"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_ZR"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_W"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_TA"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_TE"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_RE"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_BI"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_SE"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_BA"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_OT"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_OA"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("ELE_H"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("SPARE1"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("SPARE2"));
    ironInfoHisModel->setHeaderData(i++, Qt::Horizontal, tr("POUR_IN_COUNT"));

    ui->ironInfoHisTableView->setColumnWidth(0,70);
    ui->ironInfoHisTableView->setColumnWidth(1,70);
    ui->ironInfoHisTableView->setColumnWidth(2,70);
    ui->ironInfoHisTableView->setColumnWidth(3,70);
    ui->ironInfoHisTableView->setColumnWidth(4,70);
    ui->ironInfoHisTableView->setColumnWidth(5,100);
    ui->ironInfoHisTableView->setColumnWidth(6,100);
    ui->ironInfoHisTableView->setColumnWidth(7,55);
    ui->ironInfoHisTableView->setColumnWidth(8,55);
    for(i=9;i<47;i++)
    {
        ui->ironInfoHisTableView->setColumnWidth(i,70);
    }

    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_CU"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_V"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_NB"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_CR"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_NI"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_MO"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_SN"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_N"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_ALT"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_ALS"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_B"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_TI"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_CA"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_PB"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_SB"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_ZN"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_AS"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_CO"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_MG"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_ZR"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_W"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_TA"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_TE"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_RE"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_BI"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_SE"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_BA"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_OT"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_OA"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("ELE_H"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("SPARE1"),true);
    ui->ironInfoHisTableView->setColumnHidden(ironInfoHisModel->fieldIndex("SPARE2"),true);


    ui->ironInfoHisTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->ironInfoHisTableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    ui->ironInfoHisTableView->setItemDelegateForColumn(ironInfoModel->fieldIndex("SAMPLE_TIME"), new TimeEditDelegate("MM-dd hh:mm:ss"));

    ui->ironInfoHisTableView->setItemDelegate(new ToTheMiddleDelegate);

    QValidator *validator = new QIntValidator(this);
    ui->pourWeightLineEdit->setValidator(validator);
    ui->pourWeightStdLineEdit->setValidator(validator);

    connect(ui->bfNoComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(bfNoSelected(const QString &)));
    connect(ui->bfNoInComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(bfNoInSelected(const QString &)));
    connect(ui->bfNoOutComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(bfNoOutSelected(const QString &)));
    connect(ui->bfNoOutStdComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(bfNoOutStdSelected(const QString &)));
    connect(ui->pourBtn, SIGNAL(clicked()), this, SLOT(pour()));
    connect(ui->pourStdBtn, SIGNAL(clicked()), this, SLOT(pourStd()));
    connect(ui->ladleLeaveBtn, SIGNAL(clicked()), this, SLOT(ladleLeave()));
    connect(ui->ladleLeaveStdBtn, SIGNAL(clicked()), this, SLOT(ladleLeaveStd()));
    connect(ui->ironInfoEditBtn, SIGNAL(clicked()), this, SLOT(editIronInfo()));
    connect(ui->ironInfoSaveBtn,SIGNAL(clicked()),this,SLOT(saveIronInfo()));
    connect(ui->ironInfoCancelBtn,SIGNAL(clicked()),this,SLOT(cancelIronInfo()));
    connect(ui->stdIronInfoEditBtn, SIGNAL(clicked()), this, SLOT(editIronInfoStd()));
    connect(ui->stdIronInfoSaveBtn,SIGNAL(clicked()),this,SLOT(saveIronInfoStd()));
    connect(ui->stdIronInfoCancelBtn,SIGNAL(clicked()),this,SLOT(cancelIronInfoStd()));
    connect(ui->pourInfoEditBtn, SIGNAL(clicked()), this, SLOT(editPourInfo()));
    connect(ui->pourInfoSaveBtn,SIGNAL(clicked()),this,SLOT(savePourInfo()));
    connect(ui->pourInfoCancelBtn,SIGNAL(clicked()),this,SLOT(cancelPourInfo()));
    connect(ui->sendPourResultButton,SIGNAL(clicked()),this,SLOT(sendPourResult()));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
    timer->start(1000);
}

bool PagePour::event ( QEvent * )
{
    return true;
}

void PagePour::bfNoSelected(const QString &selectContent)
{
    ui->ladleNoComboBox->clear();
    QSqlQuery ladleNoQuery("SELECT LADLE_NO FROM V_IRON_INFO_CURRENT WHERE BF_NO = '" + selectContent + "'");
    QStringList ladleNoList;
    while (ladleNoQuery.next())
    {
        QString ladleNo = ladleNoQuery.value(0).toString();
        ladleNoList.append(ladleNo);
    }
    ui->ladleNoComboBox->addItems(ladleNoList);
}

void PagePour::bfNoInSelected(const QString &selectContent)
{
    ui->ladleNoInComboBox->clear();
    QSqlQuery ladleNoQuery("SELECT LADLE_NO FROM V_IRON_INFO_CURRENT WHERE BF_NO = '" + selectContent + "'");
    QStringList ladleNoList;
    while (ladleNoQuery.next())
    {
        QString ladleNo = ladleNoQuery.value(0).toString();
        ladleNoList.append(ladleNo);
    }
    ui->ladleNoInComboBox->addItems(ladleNoList);
}

void PagePour::bfNoOutSelected(const QString &selectContent)
{
    ui->ladleNoOutComboBox->clear();
    QSqlQuery ladleNoQuery("SELECT LADLE_NO FROM V_IRON_INFO_CURRENT WHERE BF_NO = '" + selectContent + "'");
    QStringList ladleNoList;
    while (ladleNoQuery.next())
    {
        QString ladleNo = ladleNoQuery.value(0).toString();
        ladleNoList.append(ladleNo);
    }
    ui->ladleNoOutComboBox->addItems(ladleNoList);
}

void PagePour::bfNoOutStdSelected(const QString &selectContent)
{
    ui->ladleNoOutStdComboBox->clear();
    QSqlQuery ladleNoQuery("SELECT LADLE_NO FROM V_IRON_INFO_CURRENT WHERE BF_NO = '" + selectContent + "'");
    QStringList ladleNoList;
    while (ladleNoQuery.next())
    {
        QString ladleNo = ladleNoQuery.value(0).toString();
        ladleNoList.append(ladleNo);
    }
    ui->ladleNoOutStdComboBox->addItems(ladleNoList);
}


void PagePour::pour()
{
    QString bfNoOut = ui->bfNoOutComboBox->currentText();
    QString bfNoIn = ui->bfNoInComboBox->currentText();
    QString ladleNoOut = ui->ladleNoOutComboBox->currentText();
    QString ladleNoIn = ui->ladleNoInComboBox->currentText();
    QString pourWeight = ui->pourWeightLineEdit->text();

    if (bfNoOut == "" || bfNoIn == "" || ladleNoOut == "" || ladleNoIn == "" || pourWeight == "")
    {
        QMessageBox::critical(0, qApp->tr("Error"),
            qApp->tr("倒罐相关项不能有空值")
                     , QMessageBox::Ok);
        return;
    }

    if (bfNoOut == bfNoIn && ladleNoOut == ladleNoIn)
    {
        QMessageBox::critical(0, qApp->tr("Error"),
            qApp->tr("倒出高炉炉号和铁水包号不能与倒入相同")
                     , QMessageBox::Ok);
        return;
    }

    QSqlQuery pourIronQuery("SELECT * FROM POUR_IRON WHERE BF_NO_OUT = '" + bfNoOut
                          + "' AND BF_NO_IN = '" + bfNoIn
                          + "' AND LADLE_NO_OUT = '" + ladleNoOut
                          + "' AND LADLE_NO_IN = '" + ladleNoIn
                          + "'");
    if (pourIronQuery.next())
    {
        QMessageBox::critical(0, qApp->tr("Error"),
            qApp->tr("不能重复向同一罐铁水倒罐")
                     , QMessageBox::Ok);
        return;
    }

    QSqlQuery pourWeightQuery("SELECT IRON_WEIGHT FROM V_IRON_INFO_CURRENT WHERE BF_NO = '" + bfNoOut
                          + "' AND LADLE_NO = '" + ladleNoOut
                          + "'");
    if (pourWeightQuery.next())
    {
        QString pourWeightMax = pourWeightQuery.value(0).toString();
        if (pourWeightMax == "")
        {
            QMessageBox::critical(0, qApp->tr("Error"),
                qApp->tr("倒出的铁水信息重量不能为空")
                         , QMessageBox::Ok);
            return;
        }
        /*
        if(QVariant(pourWeightMax).toInt() < QVariant(pourWeight).toInt())
        {
            QMessageBox::critical(0, qApp->tr("Error"),
                qApp->tr("倒罐铁水重量不能大于铁水罐内剩余铁水量")
                         , QMessageBox::Ok);
            return;
        }
        */

        QSqlQuery pourQuery;
        pourQuery.prepare("CALL CALC_POUR('"
                         + bfNoIn + "','"
                         + ladleNoIn + "','"
                         + bfNoOut + "','"
                         + ladleNoOut + "',"
                         + pourWeight + ")");
        pourQuery.exec();

/*
        QSqlQuery updateLadleInfoQuery;
        updateLadleInfoQuery.prepare("CALL UPDATE_LADLE_INFO('"+ladleNoIn+"',1)");
        updateLadleInfoQuery.exec();
*/
        QSqlQuery pourNoQuery("SELECT MAX(to_number(POUR_NO)) FROM POUR_IRON");
        pourNoQuery.next();
        QString pourNo = pourNoQuery.value(0).toString();

        QSqlQuery sendPourResultQuery;
        sendPourResultQuery.prepare("DECLARE RET NUMBER; BEGIN RET:=PAKTEST.calc('L2_COM',4,'c4@0000,c6@D1BM02,c30@ ,c10@"+pourNo+",i4@0',10); END;");
        sendPourResultQuery.exec();

        ironInfoModel->select();
        pourInfoModel->select();
        stdIronInfoModel->select();
    }
}

void PagePour::pourStd()
{
    QString bfNoOutStd = ui->bfNoOutStdComboBox->currentText();
    QString ladleNoOutStd = ui->ladleNoOutStdComboBox->currentText();
    QString ladleNoInStd = ui->ladleNoInStdLineEdit->text();
    QString pourWeightStd = ui->pourWeightStdLineEdit->text();
    QString tareWeightInStd = ui->tareWeightInStdLineEdit->text();

    if (bfNoOutStd == "" || ladleNoOutStd == "" || ladleNoInStd == "" || pourWeightStd == "")
    {
        QMessageBox::critical(0, qApp->tr("Error"),
            qApp->tr("倒罐相关项不能有空值")
                     , QMessageBox::Ok);
        return;
    }

    QSqlQuery existLadleQuery("SELECT COUNT(*) FROM IRON_INFO WHERE LADLE_NO='"+ladleNoInStd+"'");      //判断是否已存在高炉罐
    if (existLadleQuery.next())
    {
        int existLadleCount=existLadleQuery.value(0).toInt();
        if (existLadleCount>0)
        {
            QSqlQuery ladleUsingQuery("SELECT COUNT(*) FROM LADLE_STATUS WHERE LADLE_ID='"+ladleNoInStd+"'");       //判断高炉罐是否尚未离站
            if (ladleUsingQuery.next())
            {
                int ladleUsingCount=ladleUsingQuery.value(0).toInt();
                if (ladleUsingCount>0)
                {
                    QMessageBox::critical(0, qApp->tr("Error"),
                                          qApp->tr(("高炉罐"+ladleNoInStd+"尚未离站，必须离站后才能作为标准罐倒罐").toStdString().data())
                                 , QMessageBox::Ok);
                    return;
                }
                else
                {
                    QMessageBox::StandardButton buttonClicked = QMessageBox::critical(NULL, qApp->tr("警告"),
                                          qApp->tr(("已经存在罐号为"+ladleNoInStd+"的高炉罐，是否确定将此罐作为标准罐使用？").toStdString().data()),
                                                                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                    if (buttonClicked==QMessageBox::Yes)
                    {
                        QSqlQuery ironInfoUpdtQuery("UPDATE IRON_INFO SET LADLE_NO=substr('"+ladleNoInStd+"',0,8)||'_1' WHERE LADLE_NO='"+ladleNoInStd+"'");
                        QSqlQuery ironTimeUpdtQuery("UPDATE IRON_TIME SET LADLE_NO=substr('"+ladleNoInStd+"',0,8)||'_1' WHERE LADLE_NO='"+ladleNoInStd+"'");
                        QSqlQuery ladleResultUpdtQuery("UPDATE LADLE_RESULT SET LADLE_ID=substr('"+ladleNoInStd+"',0,8)||'_1' WHERE LADLE_ID='"+ladleNoInStd+"'");
                    }
                    else
                    {
                        return;
                    }
                }
            }

        }
    }

    QSqlQuery pourIronQuery("SELECT POUR_NO FROM POUR_IRON WHERE BF_NO_OUT = '" + bfNoOutStd
                          + "' AND BF_NO_IN IS NULL "
                          + " AND LADLE_NO_OUT = '" + ladleNoOutStd
                          + "' AND LADLE_NO_IN = '" + ladleNoInStd
                          + "'");
    if (pourIronQuery.next())
    {
        QSqlQuery stdIronInfoQuery("SELECT * FROM V_STD_IRON_INFO_CURRENT WHERE LADLE_NO = '" + ladleNoInStd + "'");
        if(stdIronInfoQuery.next())
        {
            QString pourNo=pourIronQuery.value(0).toString();
            QSqlQuery ladleResultQuery("SELECT * FROM LADLE_RESULT WHERE TREAT_NO='"+pourNo+"' AND TREAT_TYPE=1 AND BOF_ID IS NULL");
            if(ladleResultQuery.next())
            {
                QMessageBox::critical(0, qApp->tr("Error"),
                    qApp->tr("不能重复向同一罐铁水倒罐")
                             , QMessageBox::Ok);
                return;
            }
        }
    }

    QSqlQuery pourWeightQuery("SELECT IRON_WEIGHT FROM V_IRON_INFO_CURRENT WHERE BF_NO = '" + bfNoOutStd
                          + "' AND LADLE_NO = '" + ladleNoOutStd
                          + "'");
    if (pourWeightQuery.next())
    {
        QString pourWeightMax = pourWeightQuery.value(0).toString();
        if (pourWeightMax == "")
        {
            QMessageBox::critical(0, qApp->tr("Error"),
                qApp->tr("倒出的铁水信息重量不能为空")
                         , QMessageBox::Ok);
            return;
        }
        /*
        if(QVariant(pourWeightMax).toInt() < QVariant(pourWeightStd).toInt())
        {
            QMessageBox::critical(0, qApp->tr("Error"),
                qApp->tr("倒罐铁水重量不能大于铁水罐内剩余铁水量")
                         , QMessageBox::Ok);
            return;
        }
        */
        QSqlQuery pourQuery;
        pourQuery.prepare("CALL CALC_POUR_STD('"
                         + tareWeightInStd + "','"
                         + ladleNoInStd + "','"
                         + bfNoOutStd + "','"
                         + ladleNoOutStd + "',"
                         + pourWeightStd + ")");
        pourQuery.exec();
/*
        QSqlQuery updateLadleInfoQuery;
        updateLadleInfoQuery.prepare("CALL UPDATE_LADLE_INFO('"+ladleNoInStd+"',2)");
        updateLadleInfoQuery.exec();
*/
        QSqlQuery pourNoQuery("SELECT MAX(to_number(POUR_NO)) FROM POUR_IRON");
        pourNoQuery.next();
        QString pourNo = pourNoQuery.value(0).toString();

        QSqlQuery sendPourResultQuery;
        sendPourResultQuery.prepare("DECLARE RET NUMBER; BEGIN RET:=PAKTEST.calc('L2_COM',4,'c4@0000,c6@D1BM02,c30@ ,c10@"+pourNo+",i4@0',10); END;");
        sendPourResultQuery.exec();

        QStringList ladleNoStdList;
        QSqlQuery ladleNoStdQuery("SELECT LADLE_NO FROM V_STD_IRON_INFO_CURRENT ORDER BY END_TIME DESC");
        ladleNoStdList.append("");
        while (ladleNoStdQuery.next())
        {
            QString ladleNoStd = ladleNoStdQuery.value(0).toString();
            ladleNoStdList.append(ladleNoStd);
        }
        ui->ladleNoStdComboBox->clear();
        ui->ladleNoStdComboBox->addItems(ladleNoStdList);

        ironInfoModel->select();
        pourInfoModel->select();
        stdIronInfoModel->select();
    }
}

void PagePour::ladleLeave()
{
    QString bfNo = ui->bfNoComboBox->currentText();
    QString ladleNo = ui->ladleNoComboBox->currentText();
    QSqlQuery ladleLeaveQuery("UPDATE IRON_TIME SET END_TIME=SYSDATE WHERE BF_NO='"+bfNo+"' AND LADLE_NO='"+ladleNo+"'");
    QSqlQuery ladleStatusLeaveQuery("DELETE FROM LADLE_STATUS WHERE LADLE_ID='"+ladleNo+"'");
    ironInfoModel->select();
    pourInfoModel->select();
    ironInfoHisModel->select();
    stdIronInfoModel->select();

    ui->bfNoComboBox->clear();
    ui->bfNoInComboBox->clear();
    ui->bfNoOutComboBox->clear();
    QSqlQuery bfNoQuery("SELECT BF_NO FROM V_IRON_INFO_CURRENT");
    QStringList bfNoList;
    bfNoList.append("");
    while (bfNoQuery.next())
    {
        QString bfNo = bfNoQuery.value(0).toString();
        bfNoList.append(bfNo);
    }
    ui->bfNoComboBox->addItems(bfNoList);
    ui->bfNoInComboBox->addItems(bfNoList);
    ui->bfNoOutComboBox->addItems(bfNoList);
}

void PagePour::ladleLeaveStd()
{
    QString ladleNoStd = ui->ladleNoStdComboBox->currentText();
    QSqlQuery ladleLeaveStdQuery("UPDATE STD_IRON_INFO SET END_FLAG=1,END_TIME=SYSDATE WHERE LADLE_NO='"+ladleNoStd+"' AND END_FLAG=0");
    ironInfoModel->select();
    pourInfoModel->select();
    ironInfoHisModel->select();
    stdIronInfoModel->select();
    stdIronInfoHisModel->select();

    QStringList ladleNoStdList;
    QSqlQuery ladleNoStdQuery("SELECT LADLE_NO FROM V_STD_IRON_INFO_CURRENT ORDER BY END_TIME DESC");
    ladleNoStdList.append("");
    while (ladleNoStdQuery.next())
    {
        QString ladleNoStd = ladleNoStdQuery.value(0).toString();
        ladleNoStdList.append(ladleNoStd);
    }
    ui->ladleNoStdComboBox->clear();
    ui->ladleNoStdComboBox->addItems(ladleNoStdList);
}

void PagePour::editIronInfo()
{
    ui->ironInfoEditBtn->setDisabled(true);
    ui->ironInfoSaveBtn->setDisabled(false);
    ui->ironInfoCancelBtn->setDisabled(false);
    ui->ironInfoTableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
    timer->stop();
}
void PagePour::saveIronInfo()
{
    ui->ironInfoEditBtn->setDisabled(false);
    ui->ironInfoSaveBtn->setDisabled(true);
    ui->ironInfoCancelBtn->setDisabled(true);
    ui->ironInfoTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    if (ironInfoModel->submitAll())
    {
        ironInfoModel->database().commit();
    }
    timer->start();
}
void PagePour::cancelIronInfo()
{
    ui->ironInfoEditBtn->setDisabled(false);
    ui->ironInfoSaveBtn->setDisabled(true);
    ui->ironInfoCancelBtn->setDisabled(true);
    ui->ironInfoTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ironInfoModel->database().rollback();
    ironInfoModel->select();
    timer->start();
}

void PagePour::editIronInfoStd()
{
    ui->stdIronInfoEditBtn->setDisabled(true);
    ui->stdIronInfoSaveBtn->setDisabled(false);
    ui->stdIronInfoCancelBtn->setDisabled(false);
    ui->stdIronInfoTableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
    timer->stop();
}
void PagePour::saveIronInfoStd()
{
    ui->stdIronInfoEditBtn->setDisabled(false);
    ui->stdIronInfoSaveBtn->setDisabled(true);
    ui->stdIronInfoCancelBtn->setDisabled(true);
    ui->stdIronInfoTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    if (stdIronInfoModel->submitAll())
    {
        stdIronInfoModel->database().commit();
    }
    timer->start();
}
void PagePour::cancelIronInfoStd()
{
    ui->stdIronInfoEditBtn->setDisabled(false);
    ui->stdIronInfoSaveBtn->setDisabled(true);
    ui->stdIronInfoCancelBtn->setDisabled(true);
    ui->stdIronInfoTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    stdIronInfoModel->database().rollback();
    stdIronInfoModel->select();
    timer->start();
}

void PagePour::editPourInfo()
{
    ui->pourInfoEditBtn->setDisabled(true);
    ui->pourInfoSaveBtn->setDisabled(false);
    ui->pourInfoCancelBtn->setDisabled(false);
    ui->pourInfoTableView->setEditTriggers(QAbstractItemView::AllEditTriggers);

    timer->stop();
}
void PagePour::savePourInfo()
{
    ui->pourInfoEditBtn->setDisabled(false);
    ui->pourInfoSaveBtn->setDisabled(true);
    ui->pourInfoCancelBtn->setDisabled(true);
    ui->pourInfoTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    if (pourInfoModel->submitAll())
    {
        pourInfoModel->database().commit();
    }
    timer->start();
}
void PagePour::cancelPourInfo()
{
    ui->pourInfoEditBtn->setDisabled(false);
    ui->pourInfoSaveBtn->setDisabled(true);
    ui->pourInfoCancelBtn->setDisabled(true);
    ui->pourInfoTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pourInfoModel->database().rollback();
    pourInfoModel->select();
    timer->start();
}

void PagePour::refresh()
{

    QStringList bfNoList;
    QSqlQuery bfNoQuery("SELECT BF_NO FROM V_IRON_INFO_CURRENT");
    bfNoList.append("");
    while (bfNoQuery.next())
    {
        QString bfNo = bfNoQuery.value(0).toString();
        bfNoList.append(bfNo);
    }
    if(ui->bfNoComboBox->count() != bfNoList.count())
    {
        ui->bfNoComboBox->clear();
        ui->bfNoComboBox->addItems(bfNoList);
    }
    if(ui->bfNoInComboBox->count() != bfNoList.count())
    {
        ui->bfNoInComboBox->clear();
        ui->bfNoInComboBox->addItems(bfNoList);
    }
    if(ui->bfNoOutComboBox->count() != bfNoList.count())
    {
        ui->bfNoOutComboBox->clear();
        ui->bfNoOutComboBox->addItems(bfNoList);
    }
    if(ui->bfNoOutStdComboBox->count() != bfNoList.count())
    {
        ui->bfNoOutStdComboBox->clear();
        ui->bfNoOutStdComboBox->addItems(bfNoList);
    }
    QStringList pourNoList;
    QSqlQuery pourNoQuery("SELECT POUR_NO FROM POUR_IRON ORDER BY POUR_TIME DESC");
    pourNoList.append("");
    while (pourNoQuery.next())
    {
        QString bfNo = pourNoQuery.value(0).toString();
        pourNoList.append(bfNo);
    }

    QStringList ladleNoStdList;
    QSqlQuery ladleNoStdQuery("SELECT LADLE_NO FROM V_STD_IRON_INFO_CURRENT ORDER BY END_TIME DESC");
    ladleNoStdList.append("");
    while (ladleNoStdQuery.next())
    {
        QString ladleNoStd = ladleNoStdQuery.value(0).toString();
        ladleNoStdList.append(ladleNoStd);
    }
    if(ui->ladleNoStdComboBox->count() != ladleNoStdList.count())
    {
        ui->ladleNoStdComboBox->clear();
        ui->ladleNoStdComboBox->addItems(ladleNoStdList);
    }

    ironInfoModel->select();
    ironInfoHisModel->select();
    stdIronInfoModel->select();
    stdIronInfoHisModel->select();
    pourInfoModel->select();


}

void PagePour::sendPourResult()
{
    QString pourNo=ui->pourNoLineEdit->text();
    if(pourNo!="")
    {
        QSqlQuery sendPourResultQuery;
        sendPourResultQuery.prepare("DECLARE RET NUMBER; BEGIN RET:=PAKTEST.calc('L2_COM',4,'c4@0000,c6@D2BM02,c30@ ,c10@"+pourNo+",i4@0',10); END;");
        sendPourResultQuery.exec();
    }
    else
    {
        QMessageBox::critical(0, qApp->tr("Error"),
            qApp->tr("倒罐号不能为空")
                     , QMessageBox::Ok);
        return;
    }

}
