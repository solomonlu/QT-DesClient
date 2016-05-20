#include "database.h"
#include <QMessageBox>
#include <QtSql>

//#define WITH_SQLITE

void Database::createConnection()
{
    qApp->addLibraryPath(qApp->applicationDirPath () + "/plugins");
#ifdef WITH_SQLITE
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("DatabaseFile.db");
#else
    QSqlDatabase db = QSqlDatabase::addDatabase("QOCI");
    db.setHostName("192.168.0.201");
    db.setDatabaseName("desdb");
    //db.setHostName("127.0.0.1");
    //db.setDatabaseName("orcl");
    db.setUserName("desuser");
    db.setPassword("desuser");
    db.setPort(1521);
#endif
    if (!db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            qApp->tr("�޷��������ݿ����ӣ�"
                     "���ݿ����ӱ���: %1").arg(db.lastError().text())
                     , QMessageBox::Ok);
    }
}
