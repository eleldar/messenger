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
> Запуск
```
$ ./Messenger
```

## Монитор:
```
Monitor/
└── Monitor
```
> Запуск
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
$ sudo service postgresql start (запуск сервера БД)
$ createuser -dP messenger (создали пользователя)
Enter password for new role:messenger
Enter it again:messenger
ДАЛЕЕ РАЗОБРАТЬСЯ
...
$ psql (откроем postgres)
...
postgres=# create -E utf8 -U messages messenger (создаем базу данных) ... не работает!
... месторасположения БД может быть связано с проектом!
```
