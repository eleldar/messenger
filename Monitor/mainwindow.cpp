#include <QtWidgets>
#include "mainwindow.h"
#include "ui_mainwindow.h"

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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if (createConnection()) {
        QSqlQuery query;
        //Reading of the data
        if (!query.exec("SELECT * FROM messages;")) {
            QMessageBox::critical(NULL,QObject::tr("Ошибка"), query.lastError().text());
            //return false;
        }
        QSqlRecord rec     = query.record();
        QString strMessage;
        while (query.next()) {
            strMessage  = query.value(rec.indexOf("message")).toString();
            ui->listWidget->addItem(strMessage);
        }
        // нужен цикл опроса БД
        // с фильтрацией: флаг чтения и времени создания
    }
    else {
        QMessageBox::critical(NULL,QObject::tr("Ошибка"), "Проблемы с БД");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //Adding some information
    ui->listWidget->addItem("нужно убирать прочитанные");
    ui->listWidget->clear();
}
