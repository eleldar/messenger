#include <QtWidgets>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), SLOT(on_pushButton_clicked()));
    QSqlQuery query;
    //Reading of the data
    if (!query.exec("SELECT * FROM messages;")) {
        QMessageBox::critical(NULL,QObject::tr("Ошибка"), query.lastError().text());
        //return false;
    }
    QSqlRecord rec     = query.record();
    QString strMessage;

    QStringList messageList;
    while (query.next()) {
        strMessage  = query.value(rec.indexOf("message")).toString();
        messageList << strMessage;
        qDebug() << strMessage;
    }

    QStringListModel* model = new QStringListModel();
    model->setStringList(messageList);
    ui->listView->setModel(model);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString text = ui->lineEdit->text();
    ui->lineEdit->clear();
    //Adding some information
    QSqlQuery query;
    QString strF = "INSERT INTO  messages (message, read) VALUES('%2', %3);";
    if (text!=""){
        QString str = strF.arg(text).arg("0");

        if (!query.exec(str)) {
            QMessageBox::critical(NULL,QObject::tr("Ошибка"), query.lastError().text());
        }
        else {

            //Reading of the data
            if (!query.exec("SELECT * FROM messages;")) {
                QMessageBox::critical(NULL,QObject::tr("Ошибка"), query.lastError().text());
                //return false;
            }
            QSqlRecord rec     = query.record();
            QString strMessage;

            QStringList messageList;
            while (query.next()) {
                strMessage  = query.value(rec.indexOf("message")).toString();
                messageList << strMessage;
            }

            QStringListModel* model = new QStringListModel();
            model->setStringList(messageList);
            ui->listView->setModel(model);
        }
    }

}
