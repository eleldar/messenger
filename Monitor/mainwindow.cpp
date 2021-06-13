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
    //ui->listWidget->setSelectionMode(QAbstractItemView::MultiSelection);

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
    //ui->listWidget->addItem("нужно убирать прочитанные");
    //ui->listWidget->clear();

    QSqlQuery query;
    // Чтение базы данных
    if (!query.exec("SELECT * from messages ORDER BY id;")) {
        QMessageBox::critical(NULL, QObject::tr("Ошибка"), "База данных не читается");
    }
    /*
    // получаем указатели на элементы списка
    for(int i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem* item = ui->listWidget->item(i);
        if (ui->listWidget->currentItem() == item){

            qDebug() << ui->listWidget->currentItem();
        }
    }
    // получаем id сообщений из БД
    QSqlRecord rec     = query.record();
    int idMessage;
    while (query.next()) {
        idMessage  = query.value(rec.indexOf("id")).toInt();
        qDebug() << idMessage;
    }
    //соотвествие между id сообщения и указателем
    QSqlRecord rec     = query.record();
    int i = 0;
    int idMessage;
    while (query.next()) {
        QListWidgetItem* item = ui->listWidget->item(i++);
        idMessage  = query.value(rec.indexOf("id")).toInt();
        qDebug() << idMessage << item;
    }

    // исправить !!!
    QSqlRecord rec     = query.record();
    int i = 0;
    int idMessage;
    QString idStr;
    QListWidgetItem* itemPtr;
    //query.exec("SELECT * from messages ORDER BY id;");
    while (query.next()) {
        itemPtr = ui->listWidget->item(i++);
        idMessage  = query.value(rec.indexOf("id")).toInt();
        idStr = QString::number(idMessage);
        qDebug() << idStr;
        if (ui->listWidget->currentItem() == itemPtr){
            QString request = "UPDATE messages SET read = 1 WHERE id = " + idStr + ";";
            query.exec(request);
            qDebug() << ui->listWidget->currentItem() << idMessage << itemPtr;
        }
    }
    */
    ui->listWidget->clear();
    endId = 0;
}

void MainWindow::database_pull()
{
    QSqlQuery query;
    // Чтение базы данных
    if (!query.exec("SELECT * from messages ORDER BY id;")) {
        QMessageBox::critical(NULL, QObject::tr("Ошибка"), "База данных не читается");
    }
    QSqlRecord rec     = query.record();
    QString strMessage;
    int currentId;
    while (query.next()) {
        currentId = query.value(rec.indexOf("id")).toInt();
        int read = query.value(rec.indexOf("read")).toInt();
        strMessage  = query.value(rec.indexOf("message")).toString();
        if (currentId > endId & read==0) {
            endId = currentId;
            ui->listWidget->addItem(strMessage);
        }
    }
}
