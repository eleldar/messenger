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
                         "message  VARCHAR(100), "
                         "read  INTEGER"
                     ");";
    if (!query.exec(str)) {
        QMessageBox::critical(NULL,QObject::tr("Ошибка"), query.lastError().text());
        //return false;
    }

    //Adding some information
    QString strF =
          "INSERT INTO  messages (message, read) "
          "VALUES('%2', %3);";

    str = strF.arg("Hello 1")
              .arg("1");

    if (!query.exec(str)) {
        QMessageBox::critical(NULL,QObject::tr("Ошибка"), query.lastError().text());
        return false;
    }

    str = strF.arg("Hello 2")
              .arg("2");
    if (!query.exec(str)) {
        QMessageBox::critical(NULL,QObject::tr("Ошибка"), query.lastError().text());
        return false;
    }

    if (!query.exec("SELECT * FROM messages;")) {
        QMessageBox::critical(NULL,QObject::tr("Ошибка"), query.lastError().text());
        return false;
    }

    //Reading of the data
    QSqlRecord rec     = query.record();
    QString strMessage;

    while (query.next()) {
        strMessage  = query.value(rec.indexOf("message")).toString();
        QMessageBox::critical(NULL,QObject::tr("Ok"), strMessage);
    }

    MainWindow w;
    w.show();
    return a.exec();
}
