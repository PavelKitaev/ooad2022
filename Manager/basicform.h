#ifndef BASICFORM_H
#define BASICFORM_H

#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <windows.h>
#include <fstream>
#include <filesystem>

#include <QWidget>
#include <QPaintEvent>          //Отрисовка фона
#include <QPainter>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QTextEdit>
#include <QComboBox>
#include <QMessageBox>

#include "popnotify.h"


namespace Ui {
class BasicForm;
}

class BasicForm : public QWidget
{
    Q_OBJECT

public:
    explicit BasicForm(SOCKET _connection = 0, QString _access = "", QWidget *parent = nullptr);
    ~BasicForm();

private:
    Ui::BasicForm   *ui;
    PopNotify       *popNotify;
    SOCKET          Connection;
    QString         access;
    QString         tempNumberOrder;
    QImage          bg;                 //Изображение фона

    QWidget *widget_mainPage;
        QVBoxLayout *vbl_mp;
        QSpacerItem *top_spacer_mp;
        QHBoxLayout *hbl_mp;
        QSpacerItem *left_spacer_mp;
        QPushButton *bt_newOrders_mp;
        QPushButton *bt_activOrders_mp;
        QPushButton *bt_stat_mp;
        QSpacerItem *right_spacer_mp;

    QWidget *widget_stat;
        QVBoxLayout *vbl_st;
        QSpacerItem *top_spacer_st;
        QHBoxLayout *hbl_1_st;
        QSpacerItem *left_spacer_st;
        QSpacerItem *right_spacer_st;
        QLabel      *lb_title_st;
        QHBoxLayout *hbl_2_st;
        QLabel      *lb_doneOrders_st;
        QLabel      *lb_doneOrdersCount_st;
        QHBoxLayout *hbl_3_st;
        QLabel      *lb_activOrders_st;
        QLabel      *lb_activOrdersCount_st;
        QHBoxLayout *hbl_4_st;
        QLabel      *lb_canceledOrders_st;
        QLabel      *lb_canceledOrdersCount_st;
        QHBoxLayout *hbl_5_st;
        QPushButton *bt_mainPage_st;
        QSpacerItem *bottom_spacer_st;

    QWidget *widget_Orders;
        QWidget     *widget_OrdersScroll;
        QScrollArea *scrollArea;
        QVBoxLayout *vbl_or;
        QSpacerItem *top_spacer_or;
        QHBoxLayout *hbl_1_or;
        QSpacerItem *left_spacer_or;
        QSpacerItem *right_spacer_or;
        QLabel      *lb_title_or;

        QVBoxLayout *vbl_2_or;

        QVector<QHBoxLayout*>   hbls;
        QVector<QLabel*>        lbs_order;
        QVector<QPushButton*>   btns_orderSelected;

        QHBoxLayout *hbl_2_or;
        QPushButton *bt_mainPage_or;
        QSpacerItem *bottom_spacer_or;

    QWidget *widget_selectedOrders;
        QLabel      *lb_title_so;
        QVBoxLayout *vbl_so;
        QSpacerItem *top_spacer_so;
        QHBoxLayout *hbl_1_so;
        QSpacerItem *left_spacer_so;
        QSpacerItem *right_spacer_so;
        QSpacerItem *bottom_spacer_so;
        QLabel      *lb_name_so;
        QLabel      *lb_number_so;
        QLabel      *lb_email_so;
        QLabel      *lb_address_so;
        QLabel      *lb_category_so;
        QTextEdit   *te_desc_so;
        QPushButton *bt_downlFile_so;
        QVBoxLayout *vbl_1_so;
        QComboBox   *cb_branches;
        QTextEdit   *te_comm_so;
        QComboBox   *cb_status;
        QVBoxLayout *vbl_temp_so;
        QVBoxLayout *vbl_2_so;
        QPushButton *bt_ok;
        QPushButton *bt_mainPage_os;
        QHBoxLayout *hbl_so;


private:
    void CreateWidgetMainPage();
    void CreateWidgetStat();
    void CreateWidgetSelectedOrder();
    void CreateWidgetOrderList();
    void paintEvent(QPaintEvent* e);    //Событие отрисовки фона
    QString GetStyleSheet(QString type);
    void CreateOrdersList(QString request);
    void ClearVectors();

private slots:
    void slotStatButtonClicked();
    void slotNewOrdersButtonClicked();
    void slotActivOrdersButtonClicked();
    void slotMainPageButtonClicked();
    void slotOpenOrderButtonClicked();
    void slotSendDataButtonClicked();
    void slotDownloadFilesButtonClicked();
};

#endif // BASICFORM_H
