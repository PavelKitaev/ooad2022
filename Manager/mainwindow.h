#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <windows.h>

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>          //Сообщения
#include <QInputDialog>         //Диалоговые окна
#include <QPaintEvent>          //Отрисовка фона
#include <QPainter>

#include "popnotify.h"
#include "settingfile.h"
#include "basicform.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_bt_enter_clicked();
    void on_bt_settings_clicked();

private:
    Ui::MainWindow  *ui;
    PopNotify       *popNotify;         //Уведомления
    SettingFile     *settingFile;       //Файл настроек
    SOCKET          Connection;         //Подключение к серверу
    BasicForm       *bf;
    QImage          bg;                 //Изображение фона

private:
    int  ConnectionToServer();          //Подключение к серверу
    void paintEvent(QPaintEvent* e);    //Событие отрисовки фона
};

#endif // MAINWINDOW_H
