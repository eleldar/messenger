#include <QtWidgets>
#include "mainwindow.h"
#include "ui_mainwindow.h"
int endId = 0;

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
        // асинхронный цикл опроса БД
        QTimer* ptimer = new QTimer(this);
        connect(ptimer, SIGNAL(timeout()), SLOT(database_pull()));
        ptimer->start(500);
        database_pull();
    }
    else {
        QMessageBox::critical(NULL,QObject::tr("Ошибка"), "База данных недоступна");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    // удаление выделенных (прочитанных) сообщений
    ui->listWidget->addItem("нужно убирать прочитанные");
    //ui->listWidget->clear();
}

void MainWindow::database_pull()
{
    QSqlQuery query;
    // Чтение базы данных
    if (!query.exec("SELECT * FROM messages;")) {
        QMessageBox::critical(NULL, QObject::tr("Ошибка"), "База данных не читается");
    }
    QSqlRecord rec     = query.record();
    QString strMessage;
    int currentId;
    // нужна фильтрация: флаг чтения
    while (query.next()) {
        currentId = query.value(rec.indexOf("id")).toInt();
        strMessage  = query.value(rec.indexOf("message")).toString();
        if (currentId > endId) {
            endId = currentId;
            ui->listWidget->addItem(strMessage);
        }
    }
}
