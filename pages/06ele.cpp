#include <QtSql>
#include <QtGui/QDialog>
#include <QScrollBar>
#include "06ele.h"
#include "ui_mainwindow.h"
#include "util/QQuery2TableModel.h"
#include "util/ToTheMiddleDelegate.h"
#include "util/TimeEditDelegate.h"
#include "util/ReadOnlyDelegate.h"

PageEle::PageEle(Ui::MainWindow *pui,QObject *parent)
    : QObject(parent), ui(pui),ele1Model(0)
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

    ui->eleHmNoComboBox->clear();
    ui->eleHmNoComboBox->addItems(hmNoList);

    if (hmNoList.length() > 0)
    {
        ui->eleHmNoComboBox->setCurrentIndex(0);
        hmNoSelected(ui->eleHmNoComboBox->currentText());
    }

    connect(ui->eleHmNoComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(hmNoSelected(const QString &)));
    connect(ui->eleInternalRadioButton, SIGNAL(toggled(bool)), this, SLOT(InternalToggled(bool)));
    connect(ui->eleExternalRadioButton, SIGNAL(toggled(bool)), this, SLOT(ExternalToggled(bool)));
    connect(ui->ele1TableView->verticalScrollBar(),SIGNAL(valueChanged(int))
            ,this,SLOT(ele2TableViewVerticalScrollBarChangeValue(int)));
    connect(ui->ele2TableView->verticalScrollBar(),SIGNAL(valueChanged(int))
            ,this,SLOT(ele1TableViewVerticalScrollBarChangeValue(int)));
    //connect(ui->eleInsertBtn, SIGNAL(clicked()), this, SLOT(insertSample()));
    connect(ui->eleEditBtn, SIGNAL(clicked()), this, SLOT(setSampleEditable()));
    connect(ui->eleConfirmBtn,SIGNAL(clicked()),this,SLOT(saveSample()));
    connect(ui->eleCancelBtn,SIGNAL(clicked()),this,SLOT(cancelSample()));
    connect(ui->confirmEleButton,SIGNAL(clicked()),this,SLOT(confirmEle()));
    //connect(ui->colorLineEdit,SIGNAL(editingFinished()),this, SLOT(changeColor()));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
    timer->start(1000);
}

bool PageEle::event ( QEvent * )
{
    return true;
}

void PageEle::changeColor()
{
//    bool ok;
//    int value = ui->colorLineEdit->text().toInt(&ok);
//    if ( !ok )
//    {
//        return;
//    }

//    if ( value < 50 )
//    {
//        ui->colorLineEdit->setStyleSheet("background-color:#FF0000"); //red
//    }
//    else
//    {
//        ui->colorLineEdit->setStyleSheet("background-color:#00FF00"); //green
//    }
}

void PageEle::ele1TableViewVerticalScrollBarChangeValue(int value)
{
    int oldValue = ui->ele1TableView->verticalScrollBar()->value();
    if (value == oldValue)
    {
        return;
    }
    else
    {
        ui->ele1TableView->verticalScrollBar()->setValue(value);
    }
}

void PageEle::ele2TableViewVerticalScrollBarChangeValue(int value)
{
    int oldValue = ui->ele2TableView->verticalScrollBar()->value();
    if (value == oldValue)
    {
        return;
    }
    else
    {
        ui->ele2TableView->verticalScrollBar()->setValue(value);
    }
}

void PageEle::hmNoSelected(const QString & selectContent)
{
    if (selectContent == "")
    {
        ui->eleEditBtn->setDisabled(true);
        //ui->eleInsertBtn->setDisabled(true);
        ui->eleConfirmBtn->setDisabled(true);
        ui->eleCancelBtn->setDisabled(true);
    }
    else
    {
        ui->eleEditBtn->setDisabled(false);
        //ui->eleInsertBtn->setDisabled(false);
        ui->eleConfirmBtn->setDisabled(true);
        ui->eleCancelBtn->setDisabled(true);
    }

    QSqlQuery stNoQuery("SELECT ST_NO FROM DES_RESULT WHERE HM_NO = '" + selectContent + "'");
    if (stNoQuery.next())
    {
        ui->stNoLabel->setText(stNoQuery.value(0).toString());
    }
    else
    {
        ui->stNoLabel->setText("");
    }

    // standard
    standard1Model = new QSqlQueryModel(this);

    QString querySql="SELECT ST_NO,to_char(C_MAX_E,'fm990.999999'),to_char(C_MAX_I,'fm990.999999'),to_char(C_AIM_I,'fm990.999999'),to_char(C_MIN_I,'fm990.999999'),to_char(C_MIN_E,'fm990.999999'),to_char(SI_MAX_E,'fm990.999999'),to_char(SI_MAX_I,'fm990.999999'),to_char(SI_AIM_I,'fm990.999999'),to_char(SI_MIN_I,'fm990.999999'),to_char(SI_MIN_E,'fm990.999999'),to_char(MN_MAX_E,'fm990.999999'),to_char(MN_MAX_I,'fm990.999999'),to_char(MN_AIM_I,'fm990.999999'),to_char(MN_MIN_I,'fm990.999999'),to_char(MN_MIN_E,'fm990.999999'),to_char(P_MAX_E,'fm990.999999'),to_char(P_MAX_I,'fm990.999999'),to_char(P_AIM_I,'fm990.999999'),to_char(P_MIN_I,'fm990.999999'),to_char(P_MIN_E,'fm990.999999'),to_char(S_MAX_E,'fm990.999999'),to_char(S_MAX_I,'fm990.999999'),to_char(S_AIM_I,'fm990.999999'),to_char(S_MIN_I,'fm990.999999'),to_char(S_MIN_E,'fm990.999999'),to_char(CU_MAX_E,'fm990.999999'),to_char(CU_MAX_I,'fm990.999999'),to_char(CU_AIM_I,'fm990.999999'),to_char(CU_MIN_I,'fm990.999999'),to_char(CU_MIN_E,'fm990.999999'),to_char(V_MAX_E,'fm990.999999'),to_char(V_MAX_I,'fm990.999999'),to_char(V_AIM_I,'fm990.999999'),to_char(V_MIN_I,'fm990.999999'),to_char(V_MIN_E,'fm990.999999'),to_char(NB_MAX_E,'fm990.999999'),to_char(NB_MAX_I,'fm990.999999'),to_char(NB_AIM_I,'fm990.999999'),to_char(NB_MIN_I,'fm990.999999'),to_char(NB_MIN_E,'fm990.999999'),to_char(CR_MAX_E,'fm990.999999'),to_char(CR_MAX_I,'fm990.999999'),to_char(CR_AIM_I,'fm990.999999'),to_char(CR_MIN_I,'fm990.999999'),to_char(CR_MIN_E,'fm990.999999'),to_char(NI_MAX_E,'fm990.999999'),to_char(NI_MAX_I,'fm990.999999'),to_char(NI_AIM_I,'fm990.999999'),to_char(NI_MIN_I,'fm990.999999'),to_char(NI_MIN_E,'fm990.999999'),to_char(MO_MAX_E,'fm990.999999'),to_char(MO_MAX_I,'fm990.999999'),to_char(MO_AIM_I,'fm990.999999'),to_char(MO_MIN_I,'fm990.999999'),to_char(MO_MIN_E,'fm990.999999'),to_char(SN_MAX_E,'fm990.999999'),to_char(SN_MAX_I,'fm990.999999'),to_char(SN_AIM_I,'fm990.999999'),to_char(SN_MIN_I,'fm990.999999'),to_char(SN_MIN_E,'fm990.999999'),to_char(N_MAX_E,'fm990.999999'),to_char(N_MAX_I,'fm990.999999'),to_char(N_AIM_I,'fm990.999999'),to_char(N_MIN_I,'fm990.999999'),to_char(N_MIN_E,'fm990.999999'),to_char(ALT_MAX_E,'fm990.999999'),to_char(ALT_MAX_I,'fm990.999999'),to_char(ALT_AIM_I,'fm990.999999'),to_char(ALT_MIN_I,'fm990.999999'),to_char(ALT_MIN_E,'fm990.999999'),to_char(ALS_MAX_E,'fm990.999999'),to_char(ALS_MAX_I,'fm990.999999'),to_char(ALS_AIM_I,'fm990.999999'),to_char(ALS_MIN_I,'fm990.999999'),to_char(ALS_MIN_E,'fm990.999999'),to_char(B_MAX_E,'fm990.999999'),to_char(B_MAX_I,'fm990.999999'),to_char(B_AIM_I,'fm990.999999'),to_char(B_MIN_I,'fm990.999999'),to_char(B_MIN_E,'fm990.999999'),to_char(TI_MAX_E,'fm990.999999'),to_char(TI_MAX_I,'fm990.999999'),to_char(TI_AIM_I,'fm990.999999'),to_char(TI_MIN_I,'fm990.999999'),to_char(TI_MIN_E,'fm990.999999'),to_char(CA_MAX_E,'fm990.999999'),to_char(CA_MAX_I,'fm990.999999'),to_char(CA_AIM_I,'fm990.999999'),to_char(CA_MIN_I,'fm990.999999'),to_char(CA_MIN_E,'fm990.999999'),to_char(PB_MAX_E,'fm990.999999'),to_char(PB_MAX_I,'fm990.999999'),to_char(PB_AIM_I,'fm990.999999'),to_char(PB_MIN_I,'fm990.999999'),to_char(PB_MIN_E,'fm990.999999'),to_char(SB_MAX_E,'fm990.999999'),to_char(SB_MAX_I,'fm990.999999'),to_char(SB_AIM_I,'fm990.999999'),to_char(SB_MIN_I,'fm990.999999'),to_char(SB_MIN_E,'fm990.999999'),to_char(ZN_MAX_E,'fm990.999999'),to_char(ZN_MAX_I,'fm990.999999'),to_char(ZN_AIM_I,'fm990.999999'),to_char(ZN_MIN_I,'fm990.999999'),to_char(ZN_MIN_E,'fm990.999999'),to_char(AS_MAX_E,'fm990.999999'),to_char(AS_MAX_I,'fm990.999999'),to_char(AS_AIM_I,'fm990.999999'),to_char(AS_MIN_I,'fm990.999999'),to_char(AS_MIN_E,'fm990.999999'),to_char(CO_MAX_E,'fm990.999999'),to_char(CO_MAX_I,'fm990.999999'),to_char(CO_AIM_I,'fm990.999999'),to_char(CO_MIN_I,'fm990.999999'),to_char(CO_MIN_E,'fm990.999999'),to_char(MG_MAX_E,'fm990.999999'),to_char(MG_MAX_I,'fm990.999999'),to_char(MG_AIM_I,'fm990.999999'),to_char(MG_MIN_I,'fm990.999999'),to_char(MG_MIN_E,'fm990.999999'),to_char(ZR_MAX_E,'fm990.999999'),to_char(ZR_MAX_I,'fm990.999999'),to_char(ZR_AIM_I,'fm990.999999'),to_char(ZR_MIN_I,'fm990.999999'),to_char(ZR_MIN_E,'fm990.999999'),to_char(W_MAX_E,'fm990.999999'),to_char(W_MAX_I,'fm990.999999'),to_char(W_AIM_I,'fm990.999999'),to_char(W_MIN_I,'fm990.999999'),to_char(W_MIN_E,'fm990.999999'),to_char(TA_MAX_E,'fm990.999999'),to_char(TA_MAX_I,'fm990.999999'),to_char(TA_AIM_I,'fm990.999999'),to_char(TA_MIN_I,'fm990.999999'),to_char(TA_MIN_E,'fm990.999999'),to_char(TE_MAX_E,'fm990.999999'),to_char(TE_MAX_I,'fm990.999999'),to_char(TE_AIM_I,'fm990.999999'),to_char(TE_MIN_I,'fm990.999999'),to_char(TE_MIN_E,'fm990.999999'),to_char(RE_MAX_E,'fm990.999999'),to_char(RE_MAX_I,'fm990.999999'),to_char(RE_AIM_I,'fm990.999999'),to_char(RE_MIN_I,'fm990.999999'),to_char(RE_MIN_E,'fm990.999999'),to_char(BI_MAX_E,'fm990.999999'),to_char(BI_MAX_I,'fm990.999999'),to_char(BI_AIM_I,'fm990.999999'),to_char(BI_MIN_I,'fm990.999999'),to_char(BI_MIN_E,'fm990.999999'),to_char(SE_MAX_E,'fm990.999999'),to_char(SE_MAX_I,'fm990.999999'),to_char(SE_AIM_I,'fm990.999999'),to_char(SE_MIN_I,'fm990.999999'),to_char(SE_MIN_E,'fm990.999999'),to_char(BA_MAX_E,'fm990.999999'),to_char(BA_MAX_I,'fm990.999999'),to_char(BA_AIM_I,'fm990.999999'),to_char(BA_MIN_I,'fm990.999999'),to_char(BA_MIN_E,'fm990.999999'),to_char(OT_MAX_E,'fm990.999999'),to_char(OT_MAX_I,'fm990.999999'),to_char(OT_AIM_I,'fm990.999999'),to_char(OT_MIN_I,'fm990.999999'),to_char(OT_MIN_E,'fm990.999999'),to_char(OA_MAX_E,'fm990.999999'),to_char(OA_MAX_I,'fm990.999999'),to_char(OA_AIM_I,'fm990.999999'),to_char(OA_MIN_I,'fm990.999999'),to_char(OA_MIN_E,'fm990.999999'),to_char(H_MAX_E,'fm990.999999'),to_char(H_MAX_I,'fm990.999999'),to_char(H_AIM_I,'fm990.999999'),to_char(H_MIN_I,'fm990.999999'),to_char(H_MIN_E,'fm990.9999') FROM STANDARD_ELE WHERE ST_NO = '" + ui->stNoLabel->text() + "'";

    standard1Model->setQuery(querySql);

    QQuery2TableModel* newStandard1TableModel = new QQuery2TableModel(standard1Model->record(0),1,91,5,this);

    int i=0;
    newStandard1TableModel->setHeaderData(i++, Qt::Horizontal, tr("C"));
    newStandard1TableModel->setHeaderData(i++, Qt::Horizontal, tr("Si"));
    newStandard1TableModel->setHeaderData(i++, Qt::Horizontal, tr("Mn"));
    newStandard1TableModel->setHeaderData(i++, Qt::Horizontal, tr("P"));
    newStandard1TableModel->setHeaderData(i++, Qt::Horizontal, tr("S"));
    newStandard1TableModel->setHeaderData(i++, Qt::Horizontal, tr("Cu"));
    newStandard1TableModel->setHeaderData(i++, Qt::Horizontal, tr("V"));
    newStandard1TableModel->setHeaderData(i++, Qt::Horizontal, tr("Nb"));
    newStandard1TableModel->setHeaderData(i++, Qt::Horizontal, tr("Cr"));
    newStandard1TableModel->setHeaderData(i++, Qt::Horizontal, tr("Ni"));
    newStandard1TableModel->setHeaderData(i++, Qt::Horizontal, tr("Mo"));
    newStandard1TableModel->setHeaderData(i++, Qt::Horizontal, tr("Sn"));
    newStandard1TableModel->setHeaderData(i++, Qt::Horizontal, tr("N"));
    newStandard1TableModel->setHeaderData(i++, Qt::Horizontal, tr("AlT"));
    newStandard1TableModel->setHeaderData(i++, Qt::Horizontal, tr("AlS"));
    newStandard1TableModel->setHeaderData(i++, Qt::Horizontal, tr("B"));
    newStandard1TableModel->setHeaderData(i++, Qt::Horizontal, tr("Ti"));
    newStandard1TableModel->setHeaderData(i++, Qt::Horizontal, tr("Ca"));

    i=0;
    newStandard1TableModel->setHeaderData(i++, Qt::Vertical, tr("MAX_E"));
    newStandard1TableModel->setHeaderData(i++, Qt::Vertical, tr("MAX_I"));
    newStandard1TableModel->setHeaderData(i++, Qt::Vertical, tr("AIM"));
    newStandard1TableModel->setHeaderData(i++, Qt::Vertical, tr("MIN_I"));
    newStandard1TableModel->setHeaderData(i++, Qt::Vertical, tr("MIN_E"));

    ui->eleStandard1TableView->setModel(newStandard1TableModel);

    for(i=0;i<18;i++)
    {
        ui->eleStandard1TableView->setColumnWidth(i,70);
    }
    ui->eleStandard1TableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->eleStandard1TableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);


    ui->eleStandard1TableView->setItemDelegate(new ToTheMiddleDelegate);
    ui->eleStandard1TableView->show();

    QQuery2TableModel* newStandard2TableModel = new QQuery2TableModel(standard1Model->record(0),91,-1,5,this);

    i=0;
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("Pb"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("Sb"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("Zn"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("As"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("Co"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("Mg"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("Zr"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("W"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("Ta"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("Te"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("Re"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("Bi"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("Se"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("Ba"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("OT"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("Oa"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("H"));

    i=0;
    newStandard2TableModel->setHeaderData(i++, Qt::Vertical, tr("MAX_E"));
    newStandard2TableModel->setHeaderData(i++, Qt::Vertical, tr("MAX_I"));
    newStandard2TableModel->setHeaderData(i++, Qt::Vertical, tr("AIM"));
    newStandard2TableModel->setHeaderData(i++, Qt::Vertical, tr("MIN_I"));
    newStandard2TableModel->setHeaderData(i++, Qt::Vertical, tr("MIN_E"));

    ui->eleStandard2TableView->setModel(newStandard2TableModel);

    for(i=0;i<17;i++)
    {
        ui->eleStandard2TableView->setColumnWidth(i,70);
    }
    ui->eleStandard2TableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->eleStandard2TableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    ui->eleStandard2TableView->setItemDelegate(new ToTheMiddleDelegate);
    ui->eleStandard2TableView->show();

    // ele
    ele1Model = new QSqlTableModel(this);
    ele1Model->setTable("IRON_ELE");
    ele1Model->setFilter("HM_NO = '" + selectContent + "'");
    ele1Model->select();

    i=0;
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("HM_NO"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("SAMPLE_ID"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("SAMPLE_TIME"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("SAMPLE_PLACE"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("REP_SAMPLE_FLAG"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("C"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("Si"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("Mn"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("P"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("S"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("CU "));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("V"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("Nb"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("Cr"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("Ni"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("Mo"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("Sn"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("N"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("AlT"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("AlS"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("B"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("Ti"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("Ca"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("Pb"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("Sb"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("Zn"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("As"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("Co"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("Mg"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("Zr"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("W"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("Ta"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("Te"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("Re"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("Bi"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("Se"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("Ba"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("OT"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("Oa"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("H"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("SPARE1"));
    ele1Model->setHeaderData(i++, Qt::Horizontal, tr("SPARE2"));

    //ele1Model->setQuery("SELECT * FROM IRON_ELE WHERE HM_NO = '" + selectContent + "'");
    ui->ele1TableView->setModel(ele1Model);

    ui->ele1TableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->ele1TableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    ui->ele1TableView->setColumnHidden(0,true);
    ui->ele1TableView->setColumnHidden(3,true);
    for(i=23;i<42;i++)
    {
        ui->ele1TableView->setColumnHidden(i,true);
    }

    ui->ele1TableView->setColumnWidth(1,75);
    ui->ele1TableView->setColumnWidth(2,75);
    ui->ele1TableView->setColumnWidth(4,62);
    for(i=5;i<42;i++)
    {
        ui->ele1TableView->setColumnWidth(i,70);
    }

    ui->ele1TableView->setItemDelegateForColumn(1,new ReadOnlyDelegate(this));

    ui->ele1TableView->setItemDelegate(new ToTheMiddleDelegate);

    ui->ele1TableView->setItemDelegateForColumn(2, new TimeEditDelegate("MM-dd hh:mm"));

    ui->ele1TableView->show();

    ui->ele2TableView->setModel(ele1Model);

    ui->ele2TableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->ele2TableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    ui->ele2TableView->setColumnHidden(0,true);
    ui->ele2TableView->setColumnHidden(3,true);
    for(i=5;i<23;i++)
    {
        ui->ele2TableView->setColumnHidden(i,true);
    }
    ui->ele2TableView->setColumnHidden(40,true);
    ui->ele2TableView->setColumnHidden(41,true);

    ui->ele2TableView->setColumnWidth(1,75);
    ui->ele2TableView->setColumnWidth(2,75);
    ui->ele2TableView->setColumnWidth(4,62);
    for(i=5;i<42;i++)
    {
        ui->ele2TableView->setColumnWidth(i,70);
    }

    ui->ele2TableView->setItemDelegateForColumn(1,new ReadOnlyDelegate(this));

    ui->ele2TableView->setItemDelegate(new ToTheMiddleDelegate);

    ui->ele2TableView->setItemDelegateForColumn(2, new TimeEditDelegate("MM-dd hh:mm"));

    ui->ele2TableView->show();

    if(ui->eleInternalRadioButton->isChecked())
    {
        this->InternalToggled(true);
    }

    if(ui->eleExternalRadioButton->isChecked())
    {
        this->ExternalToggled(true);
    }

    QSqlQuery confirmEleQuery("SELECT SAMPLE_ID FROM CONFIRM_ELE WHERE HM_NO = '" + ui->eleHmNoComboBox->currentText() + "'");
    if (confirmEleQuery.next())
    {
        ui->confirmEleLabel->setText(tr("confirmed"));
        ui->confirmEleLabel->setStyleSheet("color:red");
        ui->finalEleSampleIdLabel->setVisible(true);
        ui->confirmSampleIdLabel->setVisible(true);
        ui->confirmSampleIdLabel->setText(confirmEleQuery.value(0).toString());
        ui->confirmSampleIdLabel->setStyleSheet("color:red");
        ui->confirmSampleIdComboBox->clear();
        ui->confirmSampleIdComboBox->setDisabled(true);
        ui->confirmEleButton->setDisabled(true);
    }
    else
    {
        ui->confirmEleLabel->setText(tr("not confirmed"));
        ui->confirmEleLabel->setStyleSheet("color:black");
        ui->finalEleSampleIdLabel->setVisible(false);
        ui->confirmSampleIdLabel->setVisible(false);
        ui->confirmSampleIdComboBox->setDisabled(false);
        ui->confirmEleButton->setDisabled(false);
        QStringList confirmSampleIdList;
        QSqlQuery confirmSampleIdQuery("SELECT SAMPLE_ID FROM IRON_ELE WHERE HM_NO='" + ui->eleHmNoComboBox->currentText() + "' ORDER BY SAMPLE_TIME DESC");
        while (confirmSampleIdQuery.next())
        {
            QString sampleId = confirmSampleIdQuery.value(0).toString();
            confirmSampleIdList.append(sampleId);
        }

        ui->confirmSampleIdComboBox->clear();
        ui->confirmSampleIdComboBox->addItems(confirmSampleIdList);
    }
}

void PageEle::setSampleEditable()
{
    //ui->eleInsertBtn->setDisabled(true);
    ui->eleEditBtn->setDisabled(true);
    ui->eleConfirmBtn->setDisabled(false);
    ui->eleCancelBtn->setDisabled(false);
    ui->eleHmNoComboBox->setDisabled(true);
    ui->ele1TableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->ele2TableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
    timer->stop();
}

void PageEle::saveSample()
{
    //ui->eleInsertBtn->setDisabled(false);
    ui->eleEditBtn->setDisabled(false);
    ui->eleConfirmBtn->setDisabled(true);
    ui->eleCancelBtn->setDisabled(true);
    ui->eleHmNoComboBox->setDisabled(false);
    ui->ele1TableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->ele2TableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    if (ele1Model->submitAll())
    {
        ele1Model->database().commit();
    }

    timer->start();
}

void PageEle::cancelSample()
{
    //ui->eleInsertBtn->setDisabled(false);
    ui->eleEditBtn->setDisabled(false);
    ui->eleConfirmBtn->setDisabled(true);
    ui->eleCancelBtn->setDisabled(true);
    ui->eleHmNoComboBox->setDisabled(false);
    ui->ele1TableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->ele2TableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ele1Model->database().rollback();

    hmNoSelected(ui->eleHmNoComboBox->currentText());

    timer->start();
}

void PageEle::insertSample()
{
    QString maxSampleID;
    QSqlQuery sampleIDQuery("SELECT MAX(to_number(SAMPLE_ID)) FROM IRON_ELE WHERE HM_NO='"+ui->eleHmNoComboBox->currentText()+"'");
    if (sampleIDQuery.next())
    {
        maxSampleID = QVariant(sampleIDQuery.value(0).toInt() + 1).toString();
    }
    else
    {
        maxSampleID = "1";
    }

    QSqlQuery insertSampleQuery("INSERT INTO IRON_ELE(HM_NO,SAMPLE_ID,SAMPLE_TIME) VALUES('"+ui->eleHmNoComboBox->currentText()+"','"+maxSampleID+"',SYSDATE)");

    hmNoSelected(ui->eleHmNoComboBox->currentText());
}

void PageEle::InternalToggled ( bool  )
{
    ui->eleStandard1TableView->setRowHidden(1,false);
    ui->eleStandard1TableView->setRowHidden(3,false);
    ui->eleStandard1TableView->setRowHidden(0,true);
    ui->eleStandard1TableView->setRowHidden(4,true);
    ui->eleStandard2TableView->setRowHidden(1,false);
    ui->eleStandard2TableView->setRowHidden(3,false);
    ui->eleStandard2TableView->setRowHidden(0,true);
    ui->eleStandard2TableView->setRowHidden(4,true);
}

void PageEle::ExternalToggled ( bool  )
{
    ui->eleStandard1TableView->setRowHidden(1,true);
    ui->eleStandard1TableView->setRowHidden(3,true);
    ui->eleStandard1TableView->setRowHidden(0,false);
    ui->eleStandard1TableView->setRowHidden(4,false);
    ui->eleStandard2TableView->setRowHidden(1,true);
    ui->eleStandard2TableView->setRowHidden(3,true);
    ui->eleStandard2TableView->setRowHidden(0,false);
    ui->eleStandard2TableView->setRowHidden(4,false);
}

void PageEle::refresh()
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

    if (ui->eleHmNoComboBox->count() != hmNoList.count())
    {
        ui->eleHmNoComboBox->clear();
        ui->eleHmNoComboBox->addItems(hmNoList);
    }

    if (ele1Model)
    {
        ele1Model->select();
    }

    QSqlQuery confirmEleQuery("SELECT SAMPLE_ID FROM CONFIRM_ELE WHERE HM_NO = '" + ui->eleHmNoComboBox->currentText() + "'");
    if (!confirmEleQuery.next())
    {
        QStringList confirmSampleIdList;
        QSqlQuery confirmSampleIdQuery("SELECT SAMPLE_ID FROM IRON_ELE WHERE HM_NO='" + ui->eleHmNoComboBox->currentText() + "' ORDER BY SAMPLE_TIME DESC");
        while (confirmSampleIdQuery.next())
        {
            QString sampleId = confirmSampleIdQuery.value(0).toString();
            confirmSampleIdList.append(sampleId);
        }
        if (ui->confirmSampleIdComboBox->count() != confirmSampleIdList.count())
        {
            ui->confirmSampleIdComboBox->clear();
            ui->confirmSampleIdComboBox->addItems(confirmSampleIdList);
        }
    }
}

void PageEle::confirmEle()
{
    QSqlQuery insertSampleQuery("INSERT INTO CONFIRM_ELE(HM_NO,SAMPLE_ID) VALUES('"+ui->eleHmNoComboBox->currentText()+"','"+ui->confirmSampleIdComboBox->currentText()+"')");
    /*
    QSqlQuery sendFinalQuery;
    sendFinalQuery.prepare("DECLARE RET NUMBER; BEGIN RET:=PAKTEST.calc('L2_COM',4,'c4@0000,c6@D1BM08,c30@ ,c10@"+ui->eleHmNoComboBox->currentText()+",i4@0',10); END;");
    sendFinalQuery.exec();
    */
    QSqlQuery updateLadleEleQuery;
    updateLadleEleQuery.prepare("CALL UPDATE_LADLE_ELE('" + ui->hmpHMNoComboBox->currentText() + "')");
    updateLadleEleQuery.exec();
    //判断是否被倒入过
    QSqlQuery pourInQuery("select count(*) from ladle_result where start_time>(select start_time from ladle_result where treat_no='" + ui->eleHmNoComboBox->currentText() + "' and treat_type=2) and (ladle_id,ladle_type)=(select ladle_id,ladle_type from ladle_result where treat_no='" + ui->eleHmNoComboBox->currentText() + "' and treat_type=2)");
    if (pourInQuery.next())
    {
        int pourInCount = pourInQuery.value(0).toInt();
        if (pourInCount > 0)
        {
            QSqlQuery recalcElePourInQuery;
            recalcElePourInQuery.prepare("CALL RECALC_ELE_POUR_IN('" + ui->hmpHMNoComboBox->currentText() + "')");
            recalcElePourInQuery.exec();
        }
    }

    //判断是否为高炉罐
    QSqlQuery ladleTypeQuery("select ladle_type from ladle_result where treat_no='" + ui->eleHmNoComboBox->currentText() + "' and treat_type=2");
    if (ladleTypeQuery.next())
    {
        int ladleType = ladleTypeQuery.value(0).toInt();
        if(ladleType == 1 || ladleType == 3)    //高炉罐或钢包
        {
            QSqlQuery pourOutQuery("select count(*) from pour_iron where (ladle_no_out,bf_no_out) = (select ladle_no,bf_no from v_iron_info_current where ladle_no=(select ladle_id from ladle_result where treat_no='" + ui->eleHmNoComboBox->currentText() + "' and treat_type=2))");
            if (pourOutQuery.next())
            {
                int pourOutCount = pourOutQuery.value(0).toInt();
                if (pourOutCount > 0)       //判断是否倒出过
                {
                    QSqlQuery recalcElePourOutQuery;
                    recalcElePourOutQuery.prepare("CALL RECALC_ELE_POUR_OUT('" + ui->hmpHMNoComboBox->currentText() + "')");
                    recalcElePourOutQuery.exec();
                }
            }
        }
    }

    //判断是否已经上水
    QSqlQuery toBofQuery("select count(bof_id) from ladle_result where treat_no='" + ui->eleHmNoComboBox->currentText() + "' and treat_type=2");
    if (toBofQuery.next())
    {
        int toBofCount = toBofQuery.value(0).toInt();
        if (toBofCount > 0)     //已经上水
        {
            QSqlQuery bofIdQuery("select bof_id from ladle_result where treat_no='" + ui->eleHmNoComboBox->currentText() + "' and treat_type=2");
            if (bofIdQuery.next())
            {
                QString bofId = bofIdQuery.value(0).toString();
                QSqlQuery sendToBofEleQuery;
                sendToBofEleQuery.prepare("DECLARE RET NUMBER; BEGIN RET:=PAKTEST.calc('L2_COM',4,'c4@0000,c6@D2BM08,c30@ ,c10@"+bofId+",i4@0',10); END;");
                sendToBofEleQuery.exec();
                QSqlQuery sendToBofInfoQuery;
                sendToBofInfoQuery.prepare("DECLARE RET NUMBER; BEGIN RET:=PAKTEST.calc('L2_COM',4,'c4@0000,c6@D2BM09,c30@ ,c10@"+bofId+",i4@0',10); END;");
                sendToBofInfoQuery.exec();
            }
        }
    }


    hmNoSelected(ui->eleHmNoComboBox->currentText());
}
