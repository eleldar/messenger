#include <QtWidgets>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), SLOT(on_pushButton_clicked()));
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
            qDebug() << text;
        }
    }
}
