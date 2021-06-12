#include "mainwindow.h"
#include <QApplication>

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("messages");
    db.setUserName("messenger");
    db.setPassword("messenger");
    if (!db.open()) {
        QMessageBox::critical(NULL,QObject::tr("Ошибка"), db.lastError().text());
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!createConnection()) {
        return -1;
    }

    //Creating of the data base
    QSqlQuery query;
    QString   str  = "CREATE TABLE messages ("
                         "id SERIAL, "
                         "timestamp timestamp default current_timestamp,"
                         "message  VARCHAR(200), "
                         "read  INTEGER"
                     ");";
    if (!query.exec(str)) {
        QMessageBox::critical(NULL,QObject::tr("Ошибка"), query.lastError().text());
        //return false;
    }

    //Reading of the data
    if (!query.exec("SELECT * FROM messages;")) {
        QMessageBox::critical(NULL,QObject::tr("Ошибка"), query.lastError().text());
        //return false;
    }

    QSqlRecord rec     = query.record();
    int id;
    QString strMessage;

    while (query.next()) {
        id = query.value(rec.indexOf("id")).toInt();
        strMessage  = query.value(rec.indexOf("message")).toString();
        qDebug() << id << strMessage;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
