#include <QtSql>
#include "02standard.h"
#include "ui_mainwindow.h"
#include "util/QQuery2TableModel.h"
#include "util/ToTheMiddleDelegate.h"

PageStandard::PageStandard(Ui::MainWindow *pui,QObject *parent)
    : QObject(parent), ui(pui)
{
    QStringList stNoList;
    stNoList.append("");
    QSqlQuery stNoQuery("SELECT ST_NO FROM STANDARD_ELE");
    while (stNoQuery.next())
    {
        QString stNo = stNoQuery.value(0).toString();
        stNoList.append(stNo);
    }

    ui->stNoComboBox->clear();
    ui->stNoComboBox->addItems(stNoList);

    connect(ui->stNoComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(stNoSelected(const QString &)));
    connect(ui->standardInternalRadioButton, SIGNAL(toggled(bool)), this, SLOT(InternalToggled(bool)));
    connect(ui->standardExternalRadioButton, SIGNAL(toggled(bool)), this, SLOT(ExternalToggled(bool)));
}

bool PageStandard::event ( QEvent * )
{
    return true;
}

void PageStandard::stNoSelected(const QString & selectContent)
{
    standard1Model = new QSqlQueryModel(this);
    QString querySql="SELECT ST_NO,to_char(C_MAX_E,'fm990.999999'),to_char(C_MAX_I,'fm990.999999'),to_char(C_AIM_I,'fm990.999999'),to_char(C_MIN_I,'fm990.999999'),to_char(C_MIN_E,'fm990.999999'),to_char(SI_MAX_E,'fm990.999999'),to_char(SI_MAX_I,'fm990.999999'),to_char(SI_AIM_I,'fm990.999999'),to_char(SI_MIN_I,'fm990.999999'),to_char(SI_MIN_E,'fm990.999999'),to_char(MN_MAX_E,'fm990.999999'),to_char(MN_MAX_I,'fm990.999999'),to_char(MN_AIM_I,'fm990.999999'),to_char(MN_MIN_I,'fm990.999999'),to_char(MN_MIN_E,'fm990.999999'),to_char(P_MAX_E,'fm990.999999'),to_char(P_MAX_I,'fm990.999999'),to_char(P_AIM_I,'fm990.999999'),to_char(P_MIN_I,'fm990.999999'),to_char(P_MIN_E,'fm990.999999'),to_char(S_MAX_E,'fm990.999999'),to_char(S_MAX_I,'fm990.999999'),to_char(S_AIM_I,'fm990.999999'),to_char(S_MIN_I,'fm990.999999'),to_char(S_MIN_E,'fm990.999999'),to_char(CU_MAX_E,'fm990.999999'),to_char(CU_MAX_I,'fm990.999999'),to_char(CU_AIM_I,'fm990.999999'),to_char(CU_MIN_I,'fm990.999999'),to_char(CU_MIN_E,'fm990.999999'),to_char(V_MAX_E,'fm990.999999'),to_char(V_MAX_I,'fm990.999999'),to_char(V_AIM_I,'fm990.999999'),to_char(V_MIN_I,'fm990.999999'),to_char(V_MIN_E,'fm990.999999'),to_char(NB_MAX_E,'fm990.999999'),to_char(NB_MAX_I,'fm990.999999'),to_char(NB_AIM_I,'fm990.999999'),to_char(NB_MIN_I,'fm990.999999'),to_char(NB_MIN_E,'fm990.999999'),to_char(CR_MAX_E,'fm990.999999'),to_char(CR_MAX_I,'fm990.999999'),to_char(CR_AIM_I,'fm990.999999'),to_char(CR_MIN_I,'fm990.999999'),to_char(CR_MIN_E,'fm990.999999'),to_char(NI_MAX_E,'fm990.999999'),to_char(NI_MAX_I,'fm990.999999'),to_char(NI_AIM_I,'fm990.999999'),to_char(NI_MIN_I,'fm990.999999'),to_char(NI_MIN_E,'fm990.999999'),to_char(MO_MAX_E,'fm990.999999'),to_char(MO_MAX_I,'fm990.999999'),to_char(MO_AIM_I,'fm990.999999'),to_char(MO_MIN_I,'fm990.999999'),to_char(MO_MIN_E,'fm990.999999'),to_char(SN_MAX_E,'fm990.999999'),to_char(SN_MAX_I,'fm990.999999'),to_char(SN_AIM_I,'fm990.999999'),to_char(SN_MIN_I,'fm990.999999'),to_char(SN_MIN_E,'fm990.999999'),to_char(N_MAX_E,'fm990.999999'),to_char(N_MAX_I,'fm990.999999'),to_char(N_AIM_I,'fm990.999999'),to_char(N_MIN_I,'fm990.999999'),to_char(N_MIN_E,'fm990.999999'),to_char(ALT_MAX_E,'fm990.999999'),to_char(ALT_MAX_I,'fm990.999999'),to_char(ALT_AIM_I,'fm990.999999'),to_char(ALT_MIN_I,'fm990.999999'),to_char(ALT_MIN_E,'fm990.999999'),to_char(ALS_MAX_E,'fm990.999999'),to_char(ALS_MAX_I,'fm990.999999'),to_char(ALS_AIM_I,'fm990.999999'),to_char(ALS_MIN_I,'fm990.999999'),to_char(ALS_MIN_E,'fm990.999999'),to_char(B_MAX_E,'fm990.999999'),to_char(B_MAX_I,'fm990.999999'),to_char(B_AIM_I,'fm990.999999'),to_char(B_MIN_I,'fm990.999999'),to_char(B_MIN_E,'fm990.999999'),to_char(TI_MAX_E,'fm990.999999'),to_char(TI_MAX_I,'fm990.999999'),to_char(TI_AIM_I,'fm990.999999'),to_char(TI_MIN_I,'fm990.999999'),to_char(TI_MIN_E,'fm990.999999'),to_char(CA_MAX_E,'fm990.999999'),to_char(CA_MAX_I,'fm990.999999'),to_char(CA_AIM_I,'fm990.999999'),to_char(CA_MIN_I,'fm990.999999'),to_char(CA_MIN_E,'fm990.999999'),to_char(PB_MAX_E,'fm990.999999'),to_char(PB_MAX_I,'fm990.999999'),to_char(PB_AIM_I,'fm990.999999'),to_char(PB_MIN_I,'fm990.999999'),to_char(PB_MIN_E,'fm990.999999'),to_char(SB_MAX_E,'fm990.999999'),to_char(SB_MAX_I,'fm990.999999'),to_char(SB_AIM_I,'fm990.999999'),to_char(SB_MIN_I,'fm990.999999'),to_char(SB_MIN_E,'fm990.999999'),to_char(ZN_MAX_E,'fm990.999999'),to_char(ZN_MAX_I,'fm990.999999'),to_char(ZN_AIM_I,'fm990.999999'),to_char(ZN_MIN_I,'fm990.999999'),to_char(ZN_MIN_E,'fm990.999999'),to_char(AS_MAX_E,'fm990.999999'),to_char(AS_MAX_I,'fm990.999999'),to_char(AS_AIM_I,'fm990.999999'),to_char(AS_MIN_I,'fm990.999999'),to_char(AS_MIN_E,'fm990.999999'),to_char(CO_MAX_E,'fm990.999999'),to_char(CO_MAX_I,'fm990.999999'),to_char(CO_AIM_I,'fm990.999999'),to_char(CO_MIN_I,'fm990.999999'),to_char(CO_MIN_E,'fm990.999999'),to_char(MG_MAX_E,'fm990.999999'),to_char(MG_MAX_I,'fm990.999999'),to_char(MG_AIM_I,'fm990.999999'),to_char(MG_MIN_I,'fm990.999999'),to_char(MG_MIN_E,'fm990.999999'),to_char(ZR_MAX_E,'fm990.999999'),to_char(ZR_MAX_I,'fm990.999999'),to_char(ZR_AIM_I,'fm990.999999'),to_char(ZR_MIN_I,'fm990.999999'),to_char(ZR_MIN_E,'fm990.999999'),to_char(W_MAX_E,'fm990.999999'),to_char(W_MAX_I,'fm990.999999'),to_char(W_AIM_I,'fm990.999999'),to_char(W_MIN_I,'fm990.999999'),to_char(W_MIN_E,'fm990.999999'),to_char(TA_MAX_E,'fm990.999999'),to_char(TA_MAX_I,'fm990.999999'),to_char(TA_AIM_I,'fm990.999999'),to_char(TA_MIN_I,'fm990.999999'),to_char(TA_MIN_E,'fm990.999999'),to_char(TE_MAX_E,'fm990.999999'),to_char(TE_MAX_I,'fm990.999999'),to_char(TE_AIM_I,'fm990.999999'),to_char(TE_MIN_I,'fm990.999999'),to_char(TE_MIN_E,'fm990.999999'),to_char(RE_MAX_E,'fm990.999999'),to_char(RE_MAX_I,'fm990.999999'),to_char(RE_AIM_I,'fm990.999999'),to_char(RE_MIN_I,'fm990.999999'),to_char(RE_MIN_E,'fm990.999999'),to_char(BI_MAX_E,'fm990.999999'),to_char(BI_MAX_I,'fm990.999999'),to_char(BI_AIM_I,'fm990.999999'),to_char(BI_MIN_I,'fm990.999999'),to_char(BI_MIN_E,'fm990.999999'),to_char(SE_MAX_E,'fm990.999999'),to_char(SE_MAX_I,'fm990.999999'),to_char(SE_AIM_I,'fm990.999999'),to_char(SE_MIN_I,'fm990.999999'),to_char(SE_MIN_E,'fm990.999999'),to_char(BA_MAX_E,'fm990.999999'),to_char(BA_MAX_I,'fm990.999999'),to_char(BA_AIM_I,'fm990.999999'),to_char(BA_MIN_I,'fm990.999999'),to_char(BA_MIN_E,'fm990.999999'),to_char(OT_MAX_E,'fm990.999999'),to_char(OT_MAX_I,'fm990.999999'),to_char(OT_AIM_I,'fm990.999999'),to_char(OT_MIN_I,'fm990.999999'),to_char(OT_MIN_E,'fm990.999999'),to_char(OA_MAX_E,'fm990.999999'),to_char(OA_MAX_I,'fm990.999999'),to_char(OA_AIM_I,'fm990.999999'),to_char(OA_MIN_I,'fm990.999999'),to_char(OA_MIN_E,'fm990.999999'),to_char(H_MAX_E,'fm990.999999'),to_char(H_MAX_I,'fm990.999999'),to_char(H_AIM_I,'fm990.999999'),to_char(H_MIN_I,'fm990.999999'),to_char(H_MIN_E,'fm990.9999') FROM STANDARD_ELE WHERE ST_NO = '" + selectContent + "'";
    standard1Model->setQuery(querySql);
    QQuery2TableModel* newStandard1TableModel = new QQuery2TableModel(standard1Model->record(0),1,61,5,this);

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

    i=0;
    newStandard1TableModel->setHeaderData(i++, Qt::Vertical, tr("MAX_E"));
    newStandard1TableModel->setHeaderData(i++, Qt::Vertical, tr("MAX_I"));
    newStandard1TableModel->setHeaderData(i++, Qt::Vertical, tr("AIM"));
    newStandard1TableModel->setHeaderData(i++, Qt::Vertical, tr("MIN_I"));
    newStandard1TableModel->setHeaderData(i++, Qt::Vertical, tr("MIN_E"));

    ui->standard1TableView->setModel(newStandard1TableModel);

    for(i=0;i<12;i++)
    {
        ui->standard1TableView->setColumnWidth(i,118);
    }

    ui->standard1TableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->standard1TableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);


    ui->standard1TableView->setItemDelegate(new ToTheMiddleDelegate);
    ui->standard1TableView->show();

    QQuery2TableModel* newStandard2TableModel = new QQuery2TableModel(standard1Model->record(0),61,121,5,this);

    i=0;
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("N"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("AlT"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("AlS"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("B"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("Ti"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("Ca"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("Pb"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("Sb"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("Zn"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("As"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("Co"));
    newStandard2TableModel->setHeaderData(i++, Qt::Horizontal, tr("Mg"));


    i=0;
    newStandard2TableModel->setHeaderData(i++, Qt::Vertical, tr("MAX_E"));
    newStandard2TableModel->setHeaderData(i++, Qt::Vertical, tr("MAX_I"));
    newStandard2TableModel->setHeaderData(i++, Qt::Vertical, tr("AIM"));
    newStandard2TableModel->setHeaderData(i++, Qt::Vertical, tr("MIN_I"));
    newStandard2TableModel->setHeaderData(i++, Qt::Vertical, tr("MIN_E"));

    ui->standard2TableView->setModel(newStandard2TableModel);

    for(i=0;i<12;i++)
    {
        ui->standard2TableView->setColumnWidth(i,118);
    }

    ui->standard2TableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->standard2TableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    ui->standard2TableView->setItemDelegate(new ToTheMiddleDelegate);
    ui->standard2TableView->show();

    QQuery2TableModel* newStandard3TableModel = new QQuery2TableModel(standard1Model->record(0),121,-1,5,this);

    i=0;
    newStandard3TableModel->setHeaderData(i++, Qt::Horizontal, tr("Zr"));
    newStandard3TableModel->setHeaderData(i++, Qt::Horizontal, tr("W"));
    newStandard3TableModel->setHeaderData(i++, Qt::Horizontal, tr("Ta"));
    newStandard3TableModel->setHeaderData(i++, Qt::Horizontal, tr("Te"));
    newStandard3TableModel->setHeaderData(i++, Qt::Horizontal, tr("Re"));
    newStandard3TableModel->setHeaderData(i++, Qt::Horizontal, tr("Bi"));
    newStandard3TableModel->setHeaderData(i++, Qt::Horizontal, tr("Se"));
    newStandard3TableModel->setHeaderData(i++, Qt::Horizontal, tr("Ba"));
    newStandard3TableModel->setHeaderData(i++, Qt::Horizontal, tr("OT"));
    newStandard3TableModel->setHeaderData(i++, Qt::Horizontal, tr("Oa"));
    newStandard3TableModel->setHeaderData(i++, Qt::Horizontal, tr("H"));

    i=0;
    newStandard3TableModel->setHeaderData(i++, Qt::Vertical, tr("MAX_E"));
    newStandard3TableModel->setHeaderData(i++, Qt::Vertical, tr("MAX_I"));
    newStandard3TableModel->setHeaderData(i++, Qt::Vertical, tr("AIM"));
    newStandard3TableModel->setHeaderData(i++, Qt::Vertical, tr("MIN_I"));
    newStandard3TableModel->setHeaderData(i++, Qt::Vertical, tr("MIN_E"));

    ui->standard3TableView->setModel(newStandard3TableModel);

    for(i=0;i<11;i++)
    {
        ui->standard3TableView->setColumnWidth(i,118);
    }


    ui->standard3TableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->standard3TableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    ui->standard3TableView->setItemDelegate(new ToTheMiddleDelegate);
    ui->standard3TableView->show();

    if(ui->standardInternalRadioButton->isChecked())
    {
        this->InternalToggled(true);
    }

    if(ui->standardExternalRadioButton->isChecked())
    {
        this->ExternalToggled(true);
    }
}

void PageStandard::InternalToggled ( bool  )
{
    ui->standard1TableView->setRowHidden(1,false);
    ui->standard1TableView->setRowHidden(3,false);
    ui->standard1TableView->setRowHidden(0,true);
    ui->standard1TableView->setRowHidden(4,true);
    ui->standard2TableView->setRowHidden(1,false);
    ui->standard2TableView->setRowHidden(3,false);
    ui->standard2TableView->setRowHidden(0,true);
    ui->standard2TableView->setRowHidden(4,true);
    ui->standard3TableView->setRowHidden(1,false);
    ui->standard3TableView->setRowHidden(3,false);
    ui->standard3TableView->setRowHidden(0,true);
    ui->standard3TableView->setRowHidden(4,true);
}

void PageStandard::ExternalToggled ( bool  )
{
    ui->standard1TableView->setRowHidden(1,true);
    ui->standard1TableView->setRowHidden(3,true);
    ui->standard1TableView->setRowHidden(0,false);
    ui->standard1TableView->setRowHidden(4,false);
    ui->standard2TableView->setRowHidden(1,true);
    ui->standard2TableView->setRowHidden(3,true);
    ui->standard2TableView->setRowHidden(0,false);
    ui->standard2TableView->setRowHidden(4,false);
    ui->standard3TableView->setRowHidden(1,true);
    ui->standard3TableView->setRowHidden(3,true);
    ui->standard3TableView->setRowHidden(0,false);
    ui->standard3TableView->setRowHidden(4,false);
}
