# 1. Исходные файлы
## Мессенджер:
```
Messenger/
├── main.cpp
├── mainwindow.cpp
├── mainwindow.h
├── mainwindow.ui
├── Messenger.pro
└── Messenger_ru_RU.ts
```
## Монитор:
...

# 2. Бинарные файлы
## Мессенджер:
```
Messenger/
└── Messenger
```
### Запуск
```
$ ./Messenger
```

## Монитор:
```
Monitor/
└── Monitor
```
### Запуск
```
$ ./Monitor
```

# 3. Компиляция
## Мессенджер:
```
$ cd Messenger
$ qmake Messenger.pro
$ make
```
## Монитор:
...

# 4. Инсталяция
## Установка Qt (на примере ubuntu):
```
$ sudo apt install qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools
$ sudo apt install build-essential
$ sudo apt install qtcreator
```
## Установка драйвера PostgreSQL для Qt
```
$ sudo apt install libqt5sql5-psql
```
## Настройки PostgreSQL
```
$ sudo apt install postgresql (можно postgresql-contrib для доп.функций)
$ sudo su - postgres (вход в учетную запись postgres)
$ createuser -dP messenger (создали пользователя)
Enter password for new role:messenger
Enter it again:messenger
$ createdb messages (создали БД)
```
## Другие команды по работе с БД
### Работа с таблицей
```
$ sudo su - postgres (вход в учетную запись postgres)
$ psql -U messenger -d messages (открытие созданной таблицы)
messages=# SELECT * from messages; (просмотр таблицы)
messages=# SELECT * from messages; (очистка таблицы)
messages=# DROP TABLE messages; (удаление таблицы)
```
### Запуск сервера
```
$ sudo service postgresql start (запуск сервера - на всякий случай!)
```
