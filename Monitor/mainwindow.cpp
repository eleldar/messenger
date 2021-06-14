#include <QtWidgets>
#include "mainwindow.h"
#include "ui_mainwindow.h"

QMap<int, QListWidgetItem*> idPointer; // словарь указателей
int endId = 0;  // номер последнего id в БД
int dbSize = 0; // размер БД

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

    // при загрузке программы формируем словарь указателей
    if (query.size() > 0){
        while (query.next()) {
            currentId = query.value(rec.indexOf("id")).toInt();
            QListWidgetItem *str = new QListWidgetItem(query.value(1).toString());
            idPointer[currentId] = str;
            endId = currentId;
        }
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
    QSqlQuery query;
    if (!query.exec("SELECT * FROM messages ORDER BY id;")) {
        QMessageBox::critical(NULL, QObject::tr("Ошибка"), "База данных не читается");
    }
    QSqlRecord rec = query.record();
    query.exec("SELECT * FROM messages ORDER BY id;");
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
// отображение и добавление записей в словарь при изменении базы данных
{
    QSqlQuery query;
    if (!query.exec("SELECT * FROM messages ORDER BY id;")) {
        QMessageBox::critical(NULL, QObject::tr("Ошибка"), "База данных не читается");
    }
    QSqlRecord rec = query.record();
    QString strMessage;
    int currentId = query.size();
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
    ui->listWidget->setFocus();
    for(int i=0; i<ui->listWidget->count(); ++i){
        if (ui->listWidget->item(i) == ui->listWidget->currentItem()){
            ui->listWidget->model()->removeRow(i);
            break;
        }
    }
    QSqlQuery query;
    QSqlRecord rec     = query.record();
    query.exec("SELECT id FROM messages WHERE read=0;");
    rec     = query.record();
    while (query.next()) {
        int currId = query.value(rec.indexOf("id")).toInt();
        ui->listWidget->addItem(idPointer[currId]);
    }
}
