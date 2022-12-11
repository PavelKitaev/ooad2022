#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    popNotify = new PopNotify();            //Уведомления
    settingFile = new SettingFile();        //Файл настроек
    bg.load(":/images/background");              //Устанавливаем фон

    qDebug() << QDir::toNativeSeparators(QApplication::applicationDirPath());
}

MainWindow::~MainWindow()
{
    delete popNotify;
    delete settingFile;
    delete bf;
    delete ui;

    WSACleanup();
}

void MainWindow::on_bt_enter_clicked()
{
    QString login = ui->le_login->text();
    QString pass = ui->le_pass->text();

    //Поверхностная проверка корректности заполнения полей
    if ((login.length() < 3) || (pass.length() < 3)) {
        popNotify->setPopupText("Ошибка заполнения");
        popNotify->show(this);
    }
    else {
        if (ConnectionToServer() == 0) {

            QString firsRequest = "ManagerApp\0";
            int firsRequestSize = firsRequest.size();
            send(Connection, firsRequest.toUtf8(), firsRequestSize, 0);

            QString request = "Authentication:" + login + "|" + pass;
            int requestSize = request.size() + 1;

            qDebug() << "Send";
            send(Connection, (char*)&requestSize, sizeof(int), 0);
            send(Connection, request.toUtf8(), requestSize, 0);

            char *response;
            int response_size = 0;
            if (recv(Connection, (char*)&response_size, sizeof(int), NULL) > 0) {
                response = new char[response_size];

                if (recv(Connection, response, response_size, NULL) > 0) {
                    if (strcmp(response, "Success") == 0) {
                        this->hide(); //Скрываем форму входа

                        bf = new BasicForm(Connection, "access");             //Открываем основную форму
                        bf->setMinimumSize(1200, 700);                      //Минимальные размеры
                        bf->showNormal();                                   //Показать развернуто
                        bf->setWindowTitle("YouPhoto");                     //Заголовок
                    }
                    else {
                        popNotify->setPopupText("Неверный логин или пароль");
                        popNotify->show(this);
                    }
                }
            }
            else {
                popNotify->setPopupText("Ошибка");
                popNotify->show(this);
                return;
            }

            if (response != NULL) {
                delete[] response;
            }
        }
        else {
            popNotify->setPopupText("Ошибка подключения к серверу");
            popNotify->show(this);
        }
    }
}

int MainWindow::ConnectionToServer()
{
    int res = -1;

    QString address = settingFile->GetIP();
    QString port = settingFile->GetPort();

    //Инициализация Библиотеки Сокетов
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);

    if (WSAStartup(DLLVersion, &wsaData) != 0)
    {
        //std::cout << ("Несоответствие сетевых библиотек.");
        exit(1);
    }

    //Создание сокета
    SOCKADDR_IN addr;
    inet_pton(AF_INET, address.toUtf8(), &(addr.sin_addr));        //Адрес сервера
    addr.sin_port = htons(port.toShort());              //Порт сервера
    addr.sin_family = AF_INET;                          //Сокет интернета

    Connection = socket(AF_INET, SOCK_STREAM, 0);

    res = ::connect(Connection, (SOCKADDR*)&addr, sizeof(addr)); //Подключение

    if (res != 0)
    {
        WSACleanup();
    }

    return res;
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    if (bg.isNull()) {
        QWidget::paintEvent(e);
        return;
    }

    QPainter p(this);

    //Масштабирование
    qreal xs = qreal(width()) / qreal(bg.width());
    qreal ys = qreal(height()) / qreal(bg.height());

    if (xs != 1 || ys != 1) {
        QImage img;
        int cx = qRound(e->rect().x() / xs);
        int cy = qRound(e->rect().y() / ys);
        int cw = qRound(e->rect().width() / xs);
        int ch = qRound(e->rect().height() / ys);
        cw = qMin(cw, bg.width() - cx);
        ch = qMin(ch, bg.height() - cy);
        img = bg.copy(cx, cy, cw, ch);
        cw = qRound(cw * xs);
        ch = qRound(ch * ys);
        img = img.scaled(cw, ch, Qt::IgnoreAspectRatio, Qt::FastTransformation);
        p.drawImage(e->rect().topLeft(), img);
    } else {
        p.drawImage(e->rect().topLeft(), bg, QRect(e->rect().x(), e->rect().y(),
                    qMin(e->rect().width(), bg.width()), qMin(e->rect().height(), bg.height())));
    }
}

void MainWindow::on_bt_settings_clicked()
{
    bool ok;
    QString address;
    address = QInputDialog::getText(0,"Укажите адрес сервера", "IP:",
                                            QLineEdit::Normal, settingFile->GetIP(), &ok);
    if (ok)
    {
        qDebug() << address;
        settingFile->ChangeIP(address);
    }

    QString port;
    port = QInputDialog::getText(0, "Укажите порт сервера", "Port:",
                                         QLineEdit::Normal, settingFile->GetPort(), &ok);
    if (ok)
    {
        qDebug() << port;
        settingFile->ChangePort(port);
    }
}


