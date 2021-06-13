#include <QtWidgets>
#include "mainwindow.h"
#include "ui_mainwindow.h"

QMap<int, QListWidgetItem*> idPointer;
int endId = 0;
int dbSize = 0;

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
    if (!createConnection()) {QMessageBox::critical(NULL,QObject::tr("Ошибка"), "База данных недоступна");}
    // Чтение базы данных
    QSqlQuery query;
    if (!query.exec("SELECT * FROM messages ORDER BY id;")) {
        QMessageBox::critical(NULL, QObject::tr("Ошибка"), "База данных не читается");
    }
    QSqlRecord rec     = query.record();
    QString strMessage;
    int currentId;

    // формируем базу указателей при загрузке программы, если не пустая
    if (query.size() > 0){
        while (query.next()) {
            currentId = query.value(rec.indexOf("id")).toInt();
            QListWidgetItem *str = new QListWidgetItem(query.value(1).toString());
            idPointer[currentId] = str;
        }
        endId = currentId; // номер последнего id в БД
    }

    ui->setupUi(this);
    // асинхронный цикл опроса БД
    QTimer* ptimer = new QTimer(this);
    connect(ptimer, SIGNAL(timeout()), SLOT(database_pull()));
    ptimer->start(500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
// удаление выделенных (прочитанных) сообщений
{
    // Чтение базы данных
    QSqlQuery query;
    if (!query.exec("SELECT * FROM messages ORDER BY id;")) {
        QMessageBox::critical(NULL, QObject::tr("Ошибка"), "База данных не читается");
    }
    QSqlRecord rec = query.record();
    query.exec("SELECT * FROM messages ORDER BY id;");
    // меняем флаг чтения
    QListWidgetItem* itemPtr = ui->listWidget->currentItem();
    QString idStr; // искомый ID for SQL request
    QMap<int, QListWidgetItem*>::iterator it = idPointer.begin();
    for (;it !=idPointer.end(); ++it){
        if (itemPtr == it.value()){
            idStr = QString::number(it.key());
            QString request = "UPDATE messages SET read = 1 WHERE id = " + idStr + ";";
            query.exec(request);
            break;
            }
    }    
    monitor_update();
}

void MainWindow::database_pull()
{
    QSqlQuery query;
    // Чтение базы данных
    if (!query.exec("SELECT * FROM messages ORDER BY id;")) {
        QMessageBox::critical(NULL, QObject::tr("Ошибка"), "База данных не читается");
    }
    QSqlRecord rec = query.record();
    QString strMessage;
    int currentId = query.size();
    // добавляем в базу указателей при изменении базы данных
    if (currentId > endId){
        for (; currentId > endId; ++endId) {
            query.seek(endId);
            QListWidgetItem *str = new QListWidgetItem(query.value(1).toString());
            idPointer[currentId] = str;
        }
    }
    query.exec("SELECT id FROM messages WHERE read=0;");
    rec = query.record();
    if(dbSize != currentId){
        dbSize = currentId;
        while (query.next()) {
            int currId = query.value(rec.indexOf("id")).toInt();
            ui->listWidget->addItem(idPointer[currId]);
        }
     }
}

void MainWindow::monitor_update()
{
    // ошибка после clear
    ui->listWidget->blockSignals(true);
    ui->listWidget->clear();
    ui->listWidget->blockSignals(false);
    QSqlQuery query;
    QSqlRecord rec     = query.record();
    ui->listWidget->removeItemWidget(ui->listWidget->currentItem());
    query.exec("SELECT id FROM messages WHERE read=0;");
    rec     = query.record();
    while (query.next()) {
        int currId = query.value(rec.indexOf("id")).toInt();
        ui->listWidget->addItem(idPointer[currId]);
    }
}
