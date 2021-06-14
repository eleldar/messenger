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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    if (createConnection()) {
        //Создание таблицы базы данных
        QSqlQuery query;
        QString   str  = "CREATE TABLE IF NOT EXISTS messages (id SERIAL, message  VARCHAR(200), read  INTEGER);";
        query.exec(str);
    }
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), SLOT(on_pushButton_clicked()));
    QSqlQuery query;
    // Чтение базы данных
    if (!query.exec("SELECT * FROM messages ORDER BY id;")) {
        QMessageBox::critical(NULL,QObject::tr("Ошибка"), "База данных недоступна");
    }
    QSqlRecord rec     = query.record();
    QString strMessage;

    // Добавление сообщений в список
    while (query.next()) {
        strMessage  = query.value(rec.indexOf("message")).toString();
        ui->listWidget->addItem(strMessage);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString text = ui->lineEdit->text();
    ui->lineEdit->clear();
    // Добавление сообщений в список
    QSqlQuery query;
    QString strF = "INSERT INTO  messages (message, read) VALUES('%2', %3);";
    if (text!=""){
        QString str = strF.arg(text).arg("0");
        if (!query.exec(str)) {
            QMessageBox::critical(NULL,QObject::tr("Ошибка"), query.lastError().text());
        }
        else {
            ui->listWidget->addItem(text);
        }
    }
}
