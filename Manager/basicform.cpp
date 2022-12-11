#include "basicform.h"
#include "ui_basicform.h"

#define FILE_PATH "C:/YouPhotoFilesManager/"

BasicForm::BasicForm(SOCKET _connection, QString _access, QWidget *parent) : QWidget(parent), ui(new Ui::BasicForm)
{
    ui->setupUi(this);
    Connection = _connection;
    access = _access;

    popNotify = new PopNotify();            //Уведомления
    bg.load(":/images/background_basic");        //Устанавливаем фон

    CreateWidgetMainPage();
    CreateWidgetStat();
    CreateWidgetSelectedOrder();
    CreateWidgetOrderList();
}

BasicForm::~BasicForm()
{
        delete vbl_mp;
        delete top_spacer_mp;
        delete hbl_mp;
        delete left_spacer_mp;
        delete bt_newOrders_mp;
        delete bt_activOrders_mp;
        delete bt_stat_mp;
        delete right_spacer_mp;
    delete widget_mainPage;

        delete vbl_st;
        delete top_spacer_st;
        delete hbl_1_st;
        delete left_spacer_st;
        delete right_spacer_st;
        delete lb_title_st;
        delete hbl_2_st;
        delete lb_doneOrders_st;
        delete lb_doneOrdersCount_st;
        delete hbl_3_st;
        delete lb_activOrders_st;
        delete lb_activOrdersCount_st;
        delete hbl_4_st;
        delete lb_canceledOrders_st;
        delete lb_canceledOrdersCount_st;
        delete hbl_5_st;
        delete bt_mainPage_st;
        delete bottom_spacer_st;
    delete widget_stat;


        delete widget_OrdersScroll;
        delete scrollArea;
        delete vbl_or;
        delete top_spacer_or;
        delete hbl_1_or;
        delete left_spacer_or;
        delete right_spacer_or;
        delete lb_title_or;
        ClearVectors();
        delete vbl_2_or;
        delete hbl_2_or;
        delete bt_mainPage_or;
        delete bottom_spacer_or;
    delete widget_Orders;

        delete lb_title_so;
        delete vbl_so;
        delete top_spacer_so;
        delete hbl_1_so;
        delete left_spacer_so;
        delete right_spacer_so;
        delete bottom_spacer_so;
        delete lb_name_so;
        delete lb_number_so;
        delete lb_email_so;
        delete lb_address_so;
        delete lb_category_so;
        delete te_desc_so;
        delete bt_downlFile_so;
        delete vbl_1_so;
        delete cb_branches;
        delete te_comm_so;
        delete cb_status;
        delete vbl_temp_so;
        delete vbl_2_so;
        delete bt_ok;
        delete bt_mainPage_os;
        delete hbl_so;
    delete widget_selectedOrders;

    delete popNotify;
        delete ui;
}

void BasicForm::CreateWidgetMainPage()
{
    widget_mainPage = new QWidget();

    vbl_mp = new QVBoxLayout();
    top_spacer_mp = new QSpacerItem(1, 500, QSizePolicy::Minimum, QSizePolicy::Minimum);
    vbl_mp->addSpacerItem(top_spacer_mp);

    hbl_mp = new QHBoxLayout();

    left_spacer_mp = new QSpacerItem(2000, 1, QSizePolicy::Minimum, QSizePolicy::Minimum);
    hbl_mp->addSpacerItem(left_spacer_mp);

    bt_newOrders_mp = new QPushButton("Новые заказы");
    bt_newOrders_mp->setMinimumSize(200, 50);
    bt_newOrders_mp->setMaximumSize(200, 50);
    bt_newOrders_mp->setStyleSheet(GetStyleSheet("button"));
    connect(bt_newOrders_mp, SIGNAL(clicked()), this, SLOT(slotNewOrdersButtonClicked()));
    hbl_mp->addWidget(bt_newOrders_mp);

    hbl_mp->addSpacing(30);

    bt_activOrders_mp = new QPushButton("Активные заказы");
    bt_activOrders_mp->setMinimumSize(200, 50);
    bt_activOrders_mp->setMaximumSize(200, 50);
    bt_activOrders_mp->setStyleSheet(GetStyleSheet("button"));
    connect(bt_activOrders_mp, SIGNAL(clicked()), this, SLOT(slotActivOrdersButtonClicked()));
    hbl_mp->addWidget(bt_activOrders_mp);

    hbl_mp->addSpacing(30);

    bt_stat_mp = new QPushButton("Статистика");
    bt_stat_mp->setMinimumSize(200, 50);
    bt_stat_mp->setMaximumSize(200, 50);
    bt_stat_mp->setStyleSheet(GetStyleSheet("button"));
    connect(bt_stat_mp, SIGNAL(clicked()), this, SLOT(slotStatButtonClicked()));
    hbl_mp->addWidget(bt_stat_mp);

    right_spacer_mp = new QSpacerItem(1000, 1, QSizePolicy::Minimum, QSizePolicy::Minimum);
    hbl_mp->addSpacerItem(right_spacer_mp);

    vbl_mp->addLayout(hbl_mp);
    widget_mainPage->setLayout(vbl_mp);
    this->layout()->addWidget(widget_mainPage);
}

void BasicForm::CreateWidgetStat()
{
    widget_stat = new QWidget();
    vbl_st = new QVBoxLayout();
    top_spacer_st = new QSpacerItem(1, 40, QSizePolicy::Fixed, QSizePolicy::Fixed);
    vbl_st->addSpacerItem(top_spacer_st);


    hbl_1_st = new QHBoxLayout();
    left_spacer_st = new QSpacerItem(100, 1, QSizePolicy::Minimum, QSizePolicy::Minimum);
    right_spacer_st = new QSpacerItem(2000, 1, QSizePolicy::Minimum, QSizePolicy::Minimum);
    lb_title_st = new QLabel("Статистика заказов");
    lb_title_st->setStyleSheet(GetStyleSheet("label_title"));

    hbl_1_st->addSpacerItem(left_spacer_st);
    hbl_1_st->addWidget(lb_title_st);
    hbl_1_st->addSpacerItem(right_spacer_st);
    vbl_st->addLayout(hbl_1_st);
    vbl_st->addSpacing(15);


    hbl_2_st = new QHBoxLayout();
    lb_doneOrders_st = new QLabel("  Выполнено заказов");
    lb_doneOrders_st->setStyleSheet(GetStyleSheet("label_big"));

    lb_doneOrdersCount_st = new QLabel("-100");
    lb_doneOrdersCount_st->setStyleSheet(GetStyleSheet("label_small"));
    lb_doneOrdersCount_st->setAlignment(Qt::AlignCenter);

    hbl_2_st->addSpacerItem(left_spacer_st);
    hbl_2_st->addWidget(lb_doneOrders_st);
    hbl_2_st->addSpacing(15);
    hbl_2_st->addWidget(lb_doneOrdersCount_st);
    hbl_2_st->addSpacerItem(right_spacer_st);
    vbl_st->addLayout(hbl_2_st);
    vbl_st->addSpacing(5);

    hbl_3_st = new QHBoxLayout();
    lb_activOrders_st = new QLabel("  Активных заказов (в т.ч новых)");
    lb_activOrders_st->setStyleSheet(GetStyleSheet("label_big"));

    lb_activOrdersCount_st = new QLabel("-100");
    lb_activOrdersCount_st->setStyleSheet(GetStyleSheet("label_small"));
    lb_activOrdersCount_st->setAlignment(Qt::AlignCenter);

    hbl_3_st->addSpacerItem(left_spacer_st);
    hbl_3_st->addWidget(lb_activOrders_st);
    hbl_3_st->addSpacing(15);
    hbl_3_st->addWidget(lb_activOrdersCount_st);
    hbl_3_st->addSpacerItem(right_spacer_st);
    vbl_st->addLayout(hbl_3_st);
    vbl_st->addSpacing(5);

    hbl_4_st = new QHBoxLayout();
    lb_canceledOrders_st = new QLabel("  Отменено заказов");
    lb_canceledOrders_st->setStyleSheet(GetStyleSheet("label_big"));

    lb_canceledOrdersCount_st = new QLabel("-100");
    lb_canceledOrdersCount_st->setStyleSheet(GetStyleSheet("label_small"));
    lb_canceledOrdersCount_st->setAlignment(Qt::AlignCenter);

    hbl_4_st->addSpacerItem(left_spacer_st);
    hbl_4_st->addWidget(lb_canceledOrders_st);
    hbl_4_st->addSpacing(15);
    hbl_4_st->addWidget(lb_canceledOrdersCount_st);
    hbl_4_st->addSpacerItem(right_spacer_st);
    vbl_st->addLayout(hbl_4_st);
    vbl_st->addSpacing(50);


    hbl_5_st = new QHBoxLayout();

    bt_mainPage_st = new QPushButton("На главную");
    bt_mainPage_st->setMinimumSize(200, 50);
    bt_mainPage_st->setMaximumSize(200, 50);
    bt_mainPage_st->setStyleSheet(GetStyleSheet("button"));
    connect(bt_mainPage_st, SIGNAL(clicked()), this, SLOT(slotMainPageButtonClicked()));

    hbl_5_st->addSpacerItem(left_spacer_st);
    hbl_5_st->addWidget(bt_mainPage_st);
    hbl_5_st->addSpacerItem(right_spacer_st);
    vbl_st->addLayout(hbl_5_st);


    bottom_spacer_st = new QSpacerItem(1, 1000, QSizePolicy::Minimum, QSizePolicy::Minimum);
    vbl_st->addSpacerItem(bottom_spacer_st);

    widget_stat->setLayout(vbl_st);
    this->layout()->addWidget(widget_stat);
    widget_stat->setHidden(true);
}

void BasicForm::CreateWidgetSelectedOrder()
{
    widget_selectedOrders = new QWidget();
    vbl_so = new QVBoxLayout();
    top_spacer_so = new QSpacerItem(1, 20, QSizePolicy::Fixed, QSizePolicy::Fixed);
    vbl_so->addSpacerItem(top_spacer_so);


    hbl_1_so = new QHBoxLayout();
    left_spacer_so = new QSpacerItem(100, 1, QSizePolicy::Minimum, QSizePolicy::Minimum);
    right_spacer_so = new QSpacerItem(2000, 1, QSizePolicy::Maximum, QSizePolicy::Maximum);
    bottom_spacer_so = new QSpacerItem(1, 1000, QSizePolicy::Maximum, QSizePolicy::Maximum);


    lb_title_so = new QLabel("Заказ #00000");
    lb_title_so->setStyleSheet(GetStyleSheet("label_title"));

    hbl_1_so->addSpacerItem(left_spacer_so);
    hbl_1_so->addWidget(lb_title_so);
    hbl_1_so->addSpacerItem(right_spacer_so);

    vbl_so->addLayout(hbl_1_so);
    vbl_so->addSpacing(15);


    lb_name_so = new QLabel("ФИО");
    lb_name_so->setStyleSheet(GetStyleSheet("label_big"));
    lb_name_so->setMaximumSize(450, 50);
    lb_name_so->setIndent(20);

    lb_number_so = new QLabel("Номер телефона");
    lb_number_so->setStyleSheet(GetStyleSheet("label_big"));
    lb_number_so->setMaximumSize(450, 50);
    lb_number_so->setIndent(20);

    lb_email_so = new QLabel("Электронная почта");
    lb_email_so->setStyleSheet(GetStyleSheet("label_big"));
    lb_email_so->setMaximumSize(450, 50);
    lb_email_so->setIndent(20);

    lb_address_so = new QLabel("Адрес/Самовывоз");
    lb_address_so->setStyleSheet(GetStyleSheet("label_big"));
    lb_address_so->setMaximumSize(450, 50);
    lb_address_so->setIndent(20);

    lb_category_so = new QLabel("Категория");
    lb_category_so->setStyleSheet(GetStyleSheet("label_big"));
    lb_category_so->setMaximumSize(450, 50);
    lb_category_so->setIndent(20);

    te_desc_so = new QTextEdit("Описание заказа");
    te_desc_so->setReadOnly(true);
    te_desc_so->setStyleSheet(GetStyleSheet("textedit_desc"));
    te_desc_so->setMaximumSize(450, 150);


    bt_downlFile_so = new QPushButton("Скачать файлы");
    bt_downlFile_so->setStyleSheet(GetStyleSheet("button"));
    connect(bt_downlFile_so, SIGNAL(clicked()), this, SLOT(slotDownloadFilesButtonClicked()));
    bt_downlFile_so->setMinimumSize(200, 50);
    bt_downlFile_so->setMaximumSize(200, 50);

    vbl_1_so = new QVBoxLayout();
    vbl_1_so->addWidget(lb_name_so);
    vbl_1_so->addSpacing(15);
    vbl_1_so->addWidget(lb_number_so);
    vbl_1_so->addSpacing(15);
    vbl_1_so->addWidget(lb_email_so);
    vbl_1_so->addSpacing(15);
    vbl_1_so->addWidget(lb_address_so);
    vbl_1_so->addSpacing(15);
    vbl_1_so->addWidget(lb_category_so);
    vbl_1_so->addSpacing(15);
    vbl_1_so->addWidget(te_desc_so);
    vbl_1_so->addSpacing(15);
    vbl_1_so->addWidget(bt_downlFile_so, 0, Qt::AlignRight);
    vbl_1_so->addSpacerItem(bottom_spacer_so);

    cb_branches = new QComboBox();
    cb_branches->setPlaceholderText("Выбор филиала");
    cb_branches->setStyleSheet(GetStyleSheet("combobox"));

    te_comm_so = new QTextEdit();
    te_comm_so->setPlaceholderText("   Комментарий");
    te_comm_so->setStyleSheet(GetStyleSheet("textedit_desc"));
    te_comm_so->setMaximumSize(450, 150);

    cb_status = new QComboBox();
    cb_status->setPlaceholderText("Выбор статуса");
    cb_status->setStyleSheet(GetStyleSheet("combobox"));

    bt_ok = new QPushButton("Применить");
    bt_ok->setStyleSheet(GetStyleSheet("button"));
    bt_ok->setMinimumSize(200, 50);
    bt_ok->setMaximumSize(200, 50);
    connect(bt_ok, SIGNAL(clicked()), this, SLOT(slotSendDataButtonClicked()));

    bt_mainPage_os = new QPushButton("На главную");
    bt_mainPage_os->setStyleSheet(GetStyleSheet("button"));
    bt_mainPage_os->setMinimumSize(200, 50);
    bt_mainPage_os->setMaximumSize(200, 50);
    connect(bt_mainPage_os, SIGNAL(clicked()), this, SLOT(slotMainPageButtonClicked()));

    vbl_temp_so = new QVBoxLayout();
    vbl_temp_so->addWidget(cb_branches);
    vbl_temp_so->addSpacing(160);
    vbl_temp_so->addWidget(te_comm_so);
    vbl_temp_so->addSpacing(15);
    vbl_temp_so->addWidget(cb_status);
    vbl_temp_so->addSpacing(15);
    vbl_temp_so->addWidget(bt_ok, 0, Qt::AlignRight);
    vbl_temp_so->addSpacing(15);
    vbl_temp_so->addWidget(bt_mainPage_os, 0, Qt::AlignRight);
    vbl_temp_so->addSpacerItem(bottom_spacer_so);

    hbl_so = new QHBoxLayout();
    hbl_so->addSpacerItem(left_spacer_so);
    hbl_so->addLayout(vbl_1_so);
    hbl_so->addSpacing(50);
    hbl_so->addLayout(vbl_temp_so);
    hbl_so->addSpacerItem(right_spacer_so);

    vbl_so->addLayout(hbl_so);

    vbl_so->addSpacerItem(bottom_spacer_so);

    widget_selectedOrders->setLayout(vbl_so);
    this->layout()->addWidget(widget_selectedOrders);
    widget_selectedOrders->setHidden(true);
}

void BasicForm::CreateWidgetOrderList()
{
    widget_Orders = new QWidget();
    widget_Orders->setStyleSheet("border: none");
    widget_OrdersScroll = new QWidget();
    widget_OrdersScroll->setGeometry(0,0, this->maximumWidth(), this->maximumHeight());

    scrollArea = new QScrollArea(widget_Orders);
    scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    scrollArea->setWidgetResizable(true);
    scrollArea->setGeometry(0, 0, this->width(), this->height());
    scrollArea->setStyleSheet("background: transparent");
    scrollArea->setWidget( widget_OrdersScroll );

    vbl_or = new QVBoxLayout();
    top_spacer_or = new QSpacerItem(1, 40, QSizePolicy::Fixed, QSizePolicy::Fixed);
    vbl_or->addSpacerItem(top_spacer_or);

    hbl_1_or = new QHBoxLayout();
    left_spacer_or = new QSpacerItem(100, 1, QSizePolicy::Minimum, QSizePolicy::Minimum);
    right_spacer_or = new QSpacerItem(2000, 1, QSizePolicy::Maximum, QSizePolicy::Maximum);
    lb_title_or = new QLabel("Новые заказы"); //Меняется на Активные заказы
    lb_title_or->setStyleSheet(GetStyleSheet("label_title"));

    hbl_1_or->addSpacerItem(left_spacer_or);
    hbl_1_or->addWidget(lb_title_or);
    hbl_1_or->addSpacerItem(right_spacer_or);
    vbl_or->addLayout(hbl_1_or);
    vbl_or->addSpacing(15);

    vbl_2_or = new QVBoxLayout();
    vbl_or->addLayout(vbl_2_or);

    vbl_or->addSpacing(50);
    hbl_2_or = new QHBoxLayout();

    bt_mainPage_or = new QPushButton("На главную");
    bt_mainPage_or->setMinimumSize(200, 50);
    bt_mainPage_or->setMaximumSize(200, 50);
    bt_mainPage_or->setStyleSheet(GetStyleSheet("button"));
    connect(bt_mainPage_or, SIGNAL(clicked()), this, SLOT(slotMainPageButtonClicked()));

    hbl_2_or->addSpacerItem(left_spacer_or);
    hbl_2_or->addWidget(bt_mainPage_or);
    hbl_2_or->addSpacerItem(right_spacer_or);
    vbl_or->addLayout(hbl_2_or);

    bottom_spacer_or = new QSpacerItem(1, 400, QSizePolicy::Minimum, QSizePolicy::Minimum);
    vbl_or->addSpacerItem(bottom_spacer_or);

    widget_OrdersScroll->setLayout(vbl_or);
    this->layout()->addWidget(widget_Orders);
    widget_Orders->setHidden(true);
}

void BasicForm::paintEvent(QPaintEvent *e)
{
    if (scrollArea != nullptr) {
        scrollArea->setGeometry(0, 0, this->width(), this->height());
    }

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

QString BasicForm::GetStyleSheet(QString type)
{
    QString style;

    if (type == "button"){
        style = "QPushButton:enabled { background-color: white;\
                                      font: Montserrat Medium;\
                                      border-style: solid;\
                                      border-width: 0px;\
                                      border-radius: 25px;\
                                      border-color: black;\
                                      font: 14pt;\
                                      padding: 6px;\
                                      color: black; }\
                QPushButton:default { border-color: navy; }\
                QPushButton:hover { background-color: rgb(246, 246, 246); }\
                QPushButton:pressed { background-color: rgb(200, 200, 200); }";
    }
    if (type == "label_title") {
        style = "QLabel { color: white; \
                          font: bold Montserrat ExtraBold; \
                          font-style: italic; \
                          font: 37.5pt; }";
    }

    if (type == "label_big") {
        style = "QLabel { color: black; \
                          font: Montserrat Medium; \
                          font: 16pt; \
                          border-radius: 25px;\
                          background-color: white; \
                          min-width: 450px; \
                          max-width: 450px; \
                          min-height: 50px; }";
    }

    if (type == "textedit_desc") {
        style = "QTextEdit { color: black; \
                             font: Montserrat Medium; \
                             font: 16pt; \
                             border-width: 10px;\
                             border-radius: 25px;\
                             background-color: white; \
                             min-width: 450px; \
                             max-width: 450px; \
                             min-height: 115px; } \
                 QScrollBar:vertical { border: 1px solid white; \
                                       background:white; \
                                       width:10px; \
                                       margin: 20px 0px 20px 0px; } \
                QScrollBar::handle:vertical { background: black; \
                                              min-height: 0px; } \
                QScrollBar::add-line:vertical { background: white;\
                                                height: 0px;\
                                                subcontrol-position: bottom;\
                                                subcontrol-origin: margin; } \
                QScrollBar::sub-line:vertical { background: white; \
                                                height: 0 px; \
                                                subcontrol-position: top; \
                                                subcontrol-origin: margin; }";
    }

    if (type == "label_small") {
        style = "QLabel { color: black; \
                font: Montserrat Medium; \
                font: 16pt; \
                text-align: center; \
                border-radius: 25px;\
                background-color: white; \
                min-width: 100px; \
                max-width: 100px; \
                min-height: 50px; }";
    }

    if (type == "label_orders") {
        style = "QLabel { color: black; \
                        font: Montserrat Medium; \
                        font: 16pt; \
                        border-radius: 25px;\
                        background-color: white; \
                        min-width: 750px; \
                        max-width: 750px; \
                        min-height: 50px; }";
    }

    if (type == "button_order") {
        style = "QPushButton { color: black; \
                               font: Montserrat Medium; \
                               font: 16pt; \
                               border-radius: 25px;\
                               background-color: white; \
                               min-width: 200px; \
                               max-width: 200px; \
                               min-height: 50px; }\
                 QPushButton:default { border-color: navy; }\
                 QPushButton:hover { background-color: rgb(246, 246, 246); }\
                 QPushButton:pressed { background-color: rgb(200, 200, 200); }";
    }

    if (type == "combobox") {
        style = "QComboBox {color: black;\
                            font: Montserrat Medium;\
                            font: 16pt;\
                            border-radius: 25px;\
                            background-color: white;\
                            min-width: 425px;\
                            max-width: 425px;\
                            min-height: 50px; \
                            padding-left: 25px; } \
                 QComboBox:editable { background: white; } \
                 QComboBox:!editable, QComboBox::drop-down:editable { background: white; } \
                 QComboBox:!editable:on, QComboBox::drop-down:editable:on { background: white; \
                                                                            border-bottom-right-radius: 0px; \
                                                                            border-bottom-left-radius: 0px;} \
                 QComboBox::drop-down { subcontrol-origin: padding; \
                                        subcontrol-position: top right; \
                                        width: 80px; \
                                        border-left-width: 2px;\
                                        border-left-color: black;\
                                        border-left-style: none;\
                                        border-top-right-radius: 25px;\
                                        border-bottom-right-radius: 25px; \
                                        image: url(:/images/icon_arrow_new);}\
                QComboBox QAbstractItemView { border: none; selection-background-color: lightgray; padding-left: 25px;}";
    }

    return style;
}

void BasicForm::CreateOrdersList(QString request)
{
    QString response;

    char* response_temp;
    int requestSize = request.size() + 1;

    send(Connection, (char*)&requestSize, sizeof(int), 0);
    send(Connection, request.toUtf8(), requestSize, 0);

    int response_size = 0;
    if (recv(Connection, (char*)&response_size, sizeof(int), NULL) > 0) {
        response_temp = new char[response_size];

        if (recv(Connection, response_temp, response_size, NULL) > 0) {
            response = response_temp;
        }
    }
    else {
        popNotify->setPopupText("Ошибка");
        popNotify->show(this);
        return;
    }

    if (response_size > 1) {
        QStringList ordersList = response.split(";");
        QString res = "";

        for (int i = 0; i < ordersList.length(); i++) {
            QStringList temp = ordersList[i].split(":");
            qDebug() << temp << temp.isEmpty();
            //qDebug() << (temp == "");
            if (temp.isEmpty()) {
                continue;
            }

            res = "#" + temp[0] + ", Категория - " + temp[1];

            QHBoxLayout *hbl = new QHBoxLayout();
            QLabel *lb_order = new QLabel(res);
            lb_order->setStyleSheet(GetStyleSheet("label_orders"));
            lb_order->setIndent(20);

            QPushButton *bt = new QPushButton("Выбрать");
            bt->setStyleSheet(GetStyleSheet("button_order"));
            bt->setAccessibleName(temp[0]);
            connect(bt, SIGNAL(clicked()), this, SLOT(slotOpenOrderButtonClicked()));

            hbl->addSpacerItem(left_spacer_or);
            hbl->addWidget(lb_order);
            hbl->addSpacing(15);
            hbl->addWidget(bt);
            hbl->addSpacerItem(right_spacer_or);

            vbl_2_or->addLayout(hbl);
            vbl_2_or->addSpacing(5);

            hbls.push_back(hbl);
            lbs_order.push_back(lb_order);
            btns_orderSelected.push_back(bt);
        }
    }


    widget_Orders->setHidden(false);

    if (response_temp != NULL) {
        delete[] response_temp;
    }
}

void BasicForm::ClearVectors()
{
    if (!btns_orderSelected.isEmpty()) {
        for (int i = 0; i < btns_orderSelected.size(); i++) {
            btns_orderSelected[i]->disconnect(SIGNAL(clicked()));
            delete btns_orderSelected[i];
        }
        btns_orderSelected.clear();
    }

    if (!lbs_order.isEmpty()) {
        for (int i = 0; i < lbs_order.size(); i++) {
            delete lbs_order[i];
        }
        lbs_order.clear();
    }

    if (!hbls.isEmpty()) {
        for (int i = 0; i < hbls.size(); i++) {
            //Удаляем спайсенги
            while (hbls[i]->takeAt(0) != nullptr) {
                delete hbls[i]->takeAt(0);
            }
            //Удаляем элемент
            delete hbls[i];
        }
        hbls.clear();
    }
}

void BasicForm::slotStatButtonClicked()
{
    QString request = "GiveStat:" + access;
    int requestSize = request.size() + 1;

    QString responseq;
    char *response_temp;

    send(Connection, (char*)&requestSize, sizeof(int), 0);
    send(Connection, request.toUtf8(), requestSize, 0);

    int response_size = 0;
    if (recv(Connection, (char*)&response_size, sizeof(int), NULL) > 0) {
        response_temp = new char[response_size];
        //response_temp[response_size] = '\0';

        if (recv(Connection, response_temp, response_size, NULL) > 0) {
            responseq = response_temp;
        }
    }
    else {
        popNotify->setPopupText("Ошибка");
        popNotify->show(this);
        return;
    }

    QStringList stat = responseq.split(":");
    lb_doneOrdersCount_st->setText(stat[0]);
    lb_activOrdersCount_st->setText(stat[1]);
    lb_canceledOrdersCount_st->setText(stat[2]);

    bg.load(":/images/background");         //Устанавливаем фон

    widget_mainPage->setHidden(true);
    widget_stat->setHidden(false);

    if (response_temp != NULL) {
        qDebug() << "delete...";
        delete[] response_temp;
    }
}

void BasicForm::slotNewOrdersButtonClicked()
{
    bg.load(":/images/background");        //Устанавливаем фон
    widget_mainPage->setHidden(true);
    lb_title_or->setText("Новые заказы");

    QString request = "GiveNewOrders:" + access;
    CreateOrdersList(request);
}

void BasicForm::slotActivOrdersButtonClicked()
{
    bg.load(":/images/background");        //Устанавливаем фон
    widget_mainPage->setHidden(true);
    lb_title_or->setText("Активные заказы");

    QString request = "GiveActivOrders:" + access;
    CreateOrdersList(request);
}

void BasicForm::slotMainPageButtonClicked()
{
    bg.load(":/images/background_basic");        //Устанавливаем фон

    if (!widget_stat->isHidden())
        widget_stat->setHidden(true);

    if (!widget_Orders->isHidden() || !widget_selectedOrders->isHidden()) {

        ClearVectors();

        //Удаляем спайсенги
        while (vbl_2_or->takeAt(0) != nullptr) {
            delete vbl_2_or->takeAt(0);
        }

        cb_branches->clear();
        cb_status->clear();

        widget_Orders->setHidden(true);
        widget_selectedOrders->setHidden(true);
    }


    widget_mainPage->setHidden(false);
}

void BasicForm::slotOpenOrderButtonClicked()
{
    tempNumberOrder = ((QPushButton*) sender())->accessibleName();
    lb_title_so->setText("Заказ #" + tempNumberOrder);

    QString request = "GiveDataOrder:" + ((QPushButton*) sender())->accessibleName();
    QString response;

    char* response_temp;
    int requestSize = request.size() + 1;

    send(Connection, (char*)&requestSize, sizeof(int), 0);
    send(Connection, request.toUtf8(), requestSize, 0);

    int response_size = 0;
    if (recv(Connection, (char*)&response_size, sizeof(int), NULL) > 0) {
        response_temp = new char[response_size];

        if (recv(Connection, response_temp, response_size, NULL) > 0) {
            qDebug() << response_temp;
            response = response_temp;
        }
    }
    else {
        popNotify->setPopupText("Ошибка");
        popNotify->show(this);
        return;
    }

    // [0] - ФИО
    // [1] - Номер
    // [2] - Почта
    // [3] - Адрес
    // [4] - Категория
    // [5] - Описание заказа
    // [6] - Файлы
    // [7] - Статус
    // [8] - Коммент
    // [9] - филиалы, которые работают с данной категорией заказа, вычисляет сервер/ Либо выбранный ранее филиал

    QStringList dataOrder = response.split(";");
    qDebug() << dataOrder;

    lb_name_so->setText(dataOrder[0]);
    lb_number_so->setText(dataOrder[1]);
    lb_email_so->setText(dataOrder[2]);
    lb_address_so->setText(dataOrder[3]);
    lb_category_so->setText(dataOrder[4]);

    te_desc_so->setText(dataOrder[5]);
    QTextBlockFormat format;
    format.setLeftMargin(19);
    format.setRightMargin(10);
    format.setAlignment(Qt::AlignLeft);
    te_desc_so->textCursor().setBlockFormat(format);

    if (dataOrder[9].length() != 0)
        te_comm_so->setText(dataOrder[8]);

    te_comm_so->textCursor().setBlockFormat(format);

    cb_branches->addItems(dataOrder[9].split(":"));

    cb_status->addItem("В работе");
    cb_status->addItem("Подтвержден");
    cb_status->addItem("Отменен менеджером");

    if (dataOrder[7] != "Принят, ожидает подтверждения"){
        qDebug() << "Активный заказ";
        cb_status->addItem("Требуется согласование");

        if (dataOrder[3] == "Самовывоз") {
            cb_status->addItem("Готов к выдаче");
        }
        else {
            cb_status->addItem("В доставке");
        }

        cb_status->addItem("Завершен");

        cb_status->setCurrentText(dataOrder[7]);
        cb_branches->setCurrentIndex(0);
    }

    widget_Orders->setHidden(true);
    widget_selectedOrders->setHidden(false);

    if (response_temp != NULL) {
        delete[] response_temp;
    }
}

void BasicForm::slotSendDataButtonClicked()
{
    qDebug() << "Отправляем данные";

    QString branch = cb_branches->currentText();
    QString status = cb_status->currentText();
    QString comment = te_comm_so->toPlainText();

    if (branch.length() == 0 || status.length() == 0 || comment.length() == 0) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка заполнения");
        msgBox.setText("Филиал, статус и комментарий должны быть заполнены");
        msgBox.exec();
    }
    else {
        branch = branch.right(branch.length() - branch.indexOf(")") - 2);

        QString request = "UpdateOrderData:" + tempNumberOrder + ";" + branch + ";" + status + ";" + comment;
        qDebug() << "request: " << request;

        int requestSize = request.toUtf8().size() + 1;

        send(Connection, (char*)&requestSize, sizeof(int), 0);
        send(Connection, request.toUtf8(), requestSize, 0);

        char* response_temp;
        int response_size = 0;
        if (recv(Connection, (char*)&response_size, sizeof(int), NULL) > 0) {
            response_temp = new char[response_size];

            if (recv(Connection, response_temp, response_size, NULL) > 0) {
                if (strcmp(response_temp, "Success") == 0) {
                    popNotify->setPopupText("Данные успешно обновлены");
                    popNotify->show(this);
                }
                else {
                    popNotify->setPopupText("Произошла ошибка");
                    popNotify->show(this);
                }
            }
        }
        else {
            popNotify->setPopupText("Ошибка");
            popNotify->show(this);
            return;
        }

        if (response_temp != NULL) {
            delete[] response_temp;
        }

        bt_mainPage_os->click();
    }
}

void BasicForm::slotDownloadFilesButtonClicked()
{
    char *response_temp;
    int response_size = 0;

    qDebug() << "Download " << tempNumberOrder;
    QString request = "GiveFiles:" + tempNumberOrder;

    int requestSize = request.size() + 1;

    send(Connection, (char*)&requestSize, sizeof(int), 0);
    send(Connection, request.toUtf8(), requestSize, 0);

    QString filesData;

    if (recv(Connection, (char*)&response_size, sizeof(int), NULL) > 0) {
        response_temp = new char[response_size];

        if (recv(Connection, response_temp, response_size, NULL) > 0) {
            filesData = response_temp;
        }
    }
    else {
        popNotify->setPopupText("Ошибка");
        popNotify->show(this);
        return;
    }

    if (response_temp != NULL){
        delete[] response_temp;
    }

    if (filesData != "NoFiles") {
        qDebug() << filesData;

        QString path = FILE_PATH + tempNumberOrder + "/";

        std::filesystem::path sandbox{ path.toStdString() };

        if (std::filesystem::exists(sandbox) == 0) {
            std::filesystem::create_directory(sandbox);
        }

        QStringList files = filesData.split(":"); //[0] - count files, [1] - file name, [2] - file name ...

        for (int i = 1; i <= files[0].toInt(); i++)
        {
            qDebug() << "Iter: " << i << " File: " << files[i];

            int part_size = 1024;

            FILE *f;
            fopen_s(&f, (path + files[i]).toUtf8(), "wb");

            long file_size = 0;
            recv(Connection, (char *)&file_size, sizeof(file_size), 0);

            char *buffer = new char[part_size];

            while (file_size) {

                int n = recv(Connection, buffer, part_size, 0);
                file_size -= n;
                fwrite(buffer, 1, n, f);
            }
            delete [] buffer;
            fclose(f);
        }

        if (recv(Connection, (char*)&response_size, sizeof(int), NULL) > 0) {
            response_temp = new char[response_size];
            if (recv(Connection, response_temp, response_size, NULL) > 0) {
                if (strcmp(response_temp, "Success") == 0) {
                    popNotify->setPopupText("Файлы скачаны в " + path);
                    popNotify->show(this);
                }
            }
        }
        else {
            popNotify->setPopupText("Ошибка");
            popNotify->show(this);
            return;
        }

        if (response_temp != NULL) {
            delete[] response_temp;
        }
    }
    else {
        popNotify->setPopupText("Данный заказ не содержит файлов");
        popNotify->show(this);
    }
}
