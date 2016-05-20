#include <QtSQL>
#include "09alarm.h"
#include "ui_mainwindow.h"

PageAlarm::PageAlarm(Ui::MainWindow *pui,QObject *parent)
    : QObject(parent), ui(pui)
{
    ui->startTimeDateTimeEdit->setDateTime(QVariant(ui->systemTimeDateTimeLabel->text()).toDateTime());
    ui->endTimeDateTimeEdit->setDateTime(QVariant(ui->systemTimeDateTimeLabel->text()).toDateTime());

    ui->infoTypeComboBox->clear();
    QSqlQuery msgTypeQuery("SELECT MSGID,MSG_TYPE FROM MESSAGE_MAP");
    int index = 0;
    ui->infoTypeComboBox->insertItem(index++,"","");
    while (msgTypeQuery.next())
    {
        QString msgID = msgTypeQuery.value(0).toString();
        QString msgType = msgTypeQuery.value(1).toString();
        ui->infoTypeComboBox->insertItem(index++,msgType,msgID);
    }

    alarmTableModel = new QSqlRelationalTableModel(this);
    alarmTableModel->setTable("MESSAGESHOW");
    alarmTableModel->setRelation(alarmTableModel->fieldIndex("MSGID"), QSqlRelation("MESSAGE_MAP", "MSGID", "MSG_TYPE"));
    alarmTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    alarmTableModel->setSort(3,Qt::DescendingOrder);
    alarmTableModel->select();

    ui->alarmTableView->setModel(alarmTableModel);

    alarmTableModel->setHeaderData(alarmTableModel->fieldIndex("ID"), Qt::Horizontal, tr("ID"));
    alarmTableModel->setHeaderData(alarmTableModel->fieldIndex("MSGID"), Qt::Horizontal, tr("MSGID"));
    alarmTableModel->setHeaderData(alarmTableModel->fieldIndex("MSGDATE"), Qt::Horizontal, tr("MSGDATE"));
    alarmTableModel->setHeaderData(alarmTableModel->fieldIndex("MSG"), Qt::Horizontal, tr("MSG"));
    alarmTableModel->setHeaderData(alarmTableModel->fieldIndex("MSG_TYPE"), Qt::Horizontal, tr("MSG_TYPE"));

    int i=0;
    ui->alarmTableView->horizontalHeader()->moveSection(ui->alarmTableView->horizontalHeader()->visualIndex(alarmTableModel->fieldIndex("ID")),i++);
    ui->alarmTableView->horizontalHeader()->moveSection(ui->alarmTableView->horizontalHeader()->visualIndex(alarmTableModel->fieldIndex("MSG_TYPE")),i++);
    ui->alarmTableView->horizontalHeader()->moveSection(ui->alarmTableView->horizontalHeader()->visualIndex(alarmTableModel->fieldIndex("MSGDATE")),i++);
    ui->alarmTableView->horizontalHeader()->moveSection(ui->alarmTableView->horizontalHeader()->visualIndex(alarmTableModel->fieldIndex("MSG")),i++);

    ui->alarmTableView->verticalHeader()->setVisible(false);

    ui->alarmTableView->setColumnWidth(alarmTableModel->fieldIndex("ID"),70);
    ui->alarmTableView->setColumnWidth(alarmTableModel->fieldIndex("MSGID"),70);
    ui->alarmTableView->setColumnWidth(alarmTableModel->fieldIndex("MSGDATE"),150);
    ui->alarmTableView->setColumnWidth(alarmTableModel->fieldIndex("MSG"),800);

    connect(ui->selectAlarmButton, SIGNAL(clicked()), this, SLOT(select()));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
    timer->start(1000);
}

bool PageAlarm::event ( QEvent * )
{
    return true;
}

void PageAlarm::select()
{
    if(alarmTableModel)
    {
        QString filterString;
        if (ui->infoTypeComboBox->currentText() != "")
        {
            filterString = QString("relTblAl_1.MSG_TYPE = '") + ui->infoTypeComboBox->currentText() + "'";
            if ( ui->startTimeDateTimeEdit->text() != ui->endTimeDateTimeEdit->text() )
            {
                filterString += QString(" AND MSGDATE >= to_date('" + ui->startTimeDateTimeEdit->text() + "','YYYY/MM/DD HH24:MI:SS')");
                filterString += QString(" AND MSGDATE <= to_date('" + ui->endTimeDateTimeEdit->text() + "','YYYY/MM/DD HH24:MI:SS')");
            }
        }
        else
        {
            if ( ui->startTimeDateTimeEdit->text() != ui->endTimeDateTimeEdit->text() )
            {
                filterString += QString("MSGDATE >= to_date('" + ui->startTimeDateTimeEdit->text() + "','YYYY/MM/DD HH24:MI:SS')");
                filterString += QString(" AND MSGDATE <= to_date('" + ui->endTimeDateTimeEdit->text() + "','YYYY/MM/DD HH24:MI:SS')");
            }
        }
        alarmTableModel->setFilter(filterString);
        alarmTableModel->select();
    }
}

void PageAlarm::refresh()
{
    select();
}
