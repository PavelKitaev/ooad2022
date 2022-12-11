#ifndef CONSTATN_H_
#define CONSTATN_H_

#define NAME_DATABASE             "youphoto"        //Имя базы данных

#define CLIENTS_TABLE             "clients"         //Имя таблицы с клиентами
#define CLIENTS_ID_CLIENT_COL     "id_client"       //Название столбца с id клиента
#define CLIENTS_FULLNAME_COL      "full_name"       //Название столбца с ФИО клиента
#define CLIENTS_NUMTEL_COL        "num_tel"         //Название столбца с номером телефона клиента
#define CLIENTS_EMAIL_COL         "email"           //Название столбца с почтой клиента


#define ORDERS_TABLE              "orders"          //Имя таблицы с заказами
#define ORDERS_ID_ORDER_COL       "id_order"        //Название столбца с id заказа
#define ORDERS_ID_CLIENT_COL      "id_client"       //Название столбца с id клиента
#define ORDERS_ID_BRANCH_COL      "id_branch"       //Название столбца с id филиала
#define ORDERS_ADDRESS_COL        "address"         //Название столбца с адресом
#define ORDERS_CATEGORY_COL       "category"        //Название столбца с категорией
#define ORDERS_DESC_COL           "description"     //Название столбца с описанием заказа
#define ORDERS_FILES_COL          "files"           //Название столбца с файлами заказа
#define ORDERS_STATUS_COL         "status"          //Название столбца со статусом
#define ORDERS_COMMENT_COL        "comment"         //Название столбца с комментарием


#define MANAGERS_TABLE            "managers"        //Имя таблицы с менеджерами
#define MANAGERS_ID_MANAGER_COL   "id_manager"      //Название столбца с id менеджера
#define MANAGERS_FULLNAME_COL     "full_name"       //Название столбца с ФИО
#define MANAGERS_NUMTEL_COL       "num_tel"         //Название столбца с номером телефона
#define MANAGERS_LOGIN_COL        "login"           //Название столбца с логином
#define MANAGERS_PASSWORD_COL     "password"        //Название столбца с паролем
#define MANAGERS_ACCESS_COL       "access"          //Название столбца с уровнем доступа


#define BRANCHES_TABLE            "branches"        //Имя таблицы с филиалами
#define BRANCHES_ID_BRANCH_COL    "id_branch"       //Название столбца с id филиала
#define BRANCHES_DIRECTOR_COL     "director"        //Название столбца с ФИО директора
#define BRANCHES_NUMTEL_COL       "num_tel"         //Название столбца с номером телефона
#define BRANCHES_ADDRESS_COL      "address"         //Название столбца с адресом филиала
#define BRANCHES_ORDERSPROG_COL       "ordersprogress"  //Название столбца с количеством заказов в работе


#define HARDWARE_TABLE            "hardware"        //Имя таблицы с оборудованием
#define HARDWARE_ID_HARDWARE_COL  "id_hardware"     //Название столбца с id оборудования
#define HARDWARE_CATEGORY_COL     "category"        //Название столбца с категорией
#define HARDWARE_DESC_COL         "description"     //Название столбца с описанием ободурования
#define HARDWARE_IDS_BRANCHES_COL "ids_branches"    //Название столбца с филиалами


#define STATUS_COMPLITED_ORDERS   "Завершен"
#define STATUS_CANCELLED_ORDERS_1 "Отменен заказчиком"
#define STATUS_CANCELLED_ORDERS_2 "Отменен менеджером"
#define STATUS_NEW_ORDERS         "Принят, ожидает подтверждения"

#define FILES_PATH "C:/YouPhotoFilesClients/"

#endif   // CONSTATN_H_