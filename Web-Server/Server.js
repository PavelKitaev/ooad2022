const express = require('express');         //Маршрутизация
const multer = require('multer');           //Сохранение файлов от пользователя
const fs = require("fs");                   //Для файловой системы
const archiver  = require('archiver');      //Для создания архива
const db_connect = require("mysql");				        //Для работы с базой данных
var app = express();

app.set('view engine', 'ejs');                  //Движок
//app.use('/public', express.static('public'));   //Директория с публичным содержимым
app.use('/assets', express.static('assets'));   //Директория с публичным содержимым

//--------------------------Подключение к базе данных--------------------------//
const db = db_connect.createConnection({
  host:"127.0.0.1",
  user:"root",
  password:"1111",
  database:"youphoto"
});
db.connect(function(err){
  if (err)
  {
    return console.error("Ошибка: " + err.message);
  }
  else
  {
    console.log("Подключение к серверу MySQL успешно установлено");

    //Проверка наличия таблицы "clienst"
    var query_text = "SHOW TABLES FROM youphoto LIKE 'clients'";
    db.query(query_text, function(err, results){
      if (err) {
        console.log("Ошибка запроса к бд(1-1)", err);
        return;
      }

      if (results[0] == undefined) {
        console.log("Таблица 'clients' отсутствует. Создаем...");
        query_text = "CREATE TABLE clients(id_client int NOT NULL AUTO_INCREMENT, \
          full_name varchar(45) NOT NULL, num_tel varchar(12) NOT NULL, \
          email varchar(128) NOT NULL, PRIMARY KEY(id_client));";
        db.query(query_text, function(err, results){
          if (err) {
            console.log("Ошибка запроса к бд(1-2)", err);
            return;
          }
          console.log("Таблица 'clients' создана.");
        })
      }
    })

    //Проверка наличия таблицы "orders"
    query_text = "SHOW TABLES FROM youphoto LIKE 'orders'";
    db.query(query_text, function(err, results){
      if (err) {
        console.log("Ошибка запроса к бд(2-1)", err);
        return;
      }

      if (results[0] == undefined) {
        console.log("Таблица 'orders' отсутствует. Создаем...");
        query_text = "CREATE TABLE orders(id_order int NOT NULL AUTO_INCREMENT, \
          id_client int NOT NULL, id_branch int, status varchar(80) NOT NULL, \
          address varchar(255) NOT NULL, category varchar(255) NOT NULL, \
          files varchar(255) NOT NULL, description varchar(512) NOT NULL, \
          PRIMARY KEY(id_order), \
          CONSTRAINT fk_client FOREIGN KEY (id_client) \
          REFERENCES clients(id_client) ON DELETE CASCADE ON UPDATE CASCADE);";
        db.query(query_text, function(err, results){
          if (err) {
            console.log("Ошибка запроса к бд(2-2)", err);
            return;
          }
          console.log("Таблица 'orders' создана.");
        })
      }
    })
  }
});
//-----------------------------------------------------------------------------//

//------------------------Директория для хранения файлов-----------------------//
const storage = multer.diskStorage({
   //Директория в которую будут сохранятся файлы
   destination: function(req, file, cb) {cb(null, 'uploads/')},
   //Имя для сохраняемого файла оставляем оригинальное
   filename: function(req, file, cb) {cb(null , file.originalname);}
 });
var upload = multer({ storage: storage }); //Хранилище
//-----------------------------------------------------------------------------//

//-----------------------------------Страницы----------------------------------//
app.get('/', function(req, res) { res.sendFile(__dirname + "/homepage.html"); });
app.get('/createorder', function(req, res) { res.sendFile(__dirname + "/createorder.html"); });
app.get('/changeoreder', function(req, res) { res.sendFile(__dirname + "/changeoreder.html"); });
app.get('/cancelorder', function(req, res) { res.sendFile(__dirname + "/cancelorder.html"); });
app.get('/trackorder', function(req, res) { res.sendFile(__dirname + "/trackorder.html"); });
//-----------------------------------------------------------------------------//

//--------------------------Обработка создания заказа--------------------------//
app.post('/ordercreation', upload.any(), function (req, res, next) {
  var id_client = -1;
  var files_data = "";
  //Если были получены файлы, добавляем их названия в строку
  if (req.files.length > 0)
  {
    for (let i = 0; i < req.files.length; i++)
    {
      files_data += req.files[i].originalname + "|";
    }
  } else {  //Если файлов нет, указываем нулл
    files_data = "NULL";
  }

   //Добавляем данные в базу
  db.query("INSERT INTO clients (full_name, num_tel, email) values(?, ?, ?)",
  [req.body.fio, req.body.number, req.body.email], function(err, rows){
    if (err) {
      console.log("Ошибка вставки данных в БД (3-1)", err);
      return;
    }
    console.log("Вставка данных в БД успешна выполнена");
  });

  db.query("SELECT * FROM clients WHERE id_client = LAST_INSERT_ID();", function(err, results){
    if (err) {
      console.log("Ошибка запроса к бд(3-2)", err);
      res.send("Error!");
      return;
    }
    id_client = results[0].id_client;

    db.query("INSERT INTO orders (id_client, status, address, files, category, description) values(?, ?, ?, ?, ?, ?)",
    [id_client, "Принят, ожидает подтверждения", req.body.address, files_data, req.body.category, req.body.desc],
    function(err, rows){
      if (err) {
        console.log("Ошибка вставки данных в БД (3-3)", err);
        res.send("Error!");
        return;
      }
      console.log("Вставка данных в БД успешна выполнена");

      db.query("SELECT * FROM orders WHERE id_order = LAST_INSERT_ID()", function(err, results){
        if (err) {
          console.log("Ошибка запроса к бд(3-4)", err);
          res.send("error");
          return;
        }
        res.send("Ваш заказ создан под номером - " + results[0].id_order);
      })
    })
  })
});
//-----------------------------------------------------------------------------//

//--------------------------Обработка изменения заказа-------------------------//
app.post('/checkstatusorder', upload.any(), function (req, res, next) {
  var from = req.body.from;
  db.query("SELECT status FROM orders WHERE id_order = ?", [req.body.order], function(err, results){
    if (err) {
      console.log("Ошибка запроса к бд(4-1)", err);
      res.send("error");
      return;
    }
    //Если получили стату, значит такой заказ существует
    if (results[0]) {
      var status = results[0].status;
      if (from === 'changeoreder'){

        if (status == "В работе"){
          res.send("blocked");
        } else if (status == "Отменен заказчиком" || status == "Отменен менеджером"){
          res.send("cancelled");
        } else {
          res.send("unblocked");
        }
      } else if (from === 'trackorder') {
        res.send(status);
      }
    } else {
      res.send("not found");
    }
  })
});
//-----------------------------------------------------------------------------//

//---------------------------Отображение информации----------------------------//
app.get('/info/:data', function(req, res) {
  var msg = req.params.data;
  if (msg == "not found") {
    res.render('info', {
      info: "Заказ с таким номером не найден",});
  } else if (msg == "blocked"){
    res.render('info', {
      info: "Заказ на этапе изготовления. Внесение изменений невозможно",});
  } else if (msg == "error"){
    res.render('info', {
      info: " Сервис временно не доступен, попробуйте позже",});
  } else if (msg == "cancelled") {
    res.render('info', {
      info: "Заказ был отменен, создайте новый",});
  } else {
    res.render('info', {
      info: msg,});
  }
})
//-----------------------------------------------------------------------------//

//---------------------Страница с редактированием заказа-----------------------//
app.get('/changeoreder/:data', function(req, res) {
  var order = req.params.data;
  var files = "null";
  var address = "null";
  var id_client = -1;
  var fio = "null";
  var num_tel = "null";
  var category = "null";
  var email = "null";
  var desc = "null";

  db.query("SELECT * FROM orders LEFT JOIN clients ON orders.id_client = clients.id_client WHERE id_order = ?",
    [order], function(err, results){
    if (err) {
      console.log("Ошибка запроса к бд(5-1)", err);
      res.send("error");
      return;
    }

    files = results[0].files;
    address = results[0].address;
    fio = results[0].full_name;
    num_tel = results[0].num_tel;
    category = results[0].category;
    email = results[0].email;
    desc = results[0].description;

    res.render('changeoreder', {
      fio: fio,
      address: address,
      num_tel: num_tel,
      email: email,
      category: category,
      files: files,
      desc: desc,
      order: order,
      });
  })
})
//-----------------------------------------------------------------------------//

//--------------------------Изменение данных о заказе--------------------------//
app.post('/orderchange', upload.any(), function (req, res, next) {
  var id_order = req.body.order;
  var id_client = -1;
  var files_data = "";
  //Если были получены файлы, добавляем их названия в строку
  console.log("ФАЙЛОВ ПОЛУЧЕНО: " + req.files.length);
  if (req.files.length > 0) {
    console.log("Добавляем имена файлов");
     for (let i = 0; i < req.files.length; i++) {
       files_data += req.files[i].originalname + "|";
     }
  } else {
    files_data = "NULL"
  }

  db.query("SELECT id_client FROM orders WHERE id_order = ?", [id_order], function(err, results){
    if (err) {
      console.log("Ошибка запроса к БД (6-1)", err);
      return;
    }
    id_client = results[0].id_client;
    console.log(id_client);

    db.query("UPDATE clients SET full_name = ?, num_tel = ?, email = ? WHERE id_client = ?",
    [req.body.fio, req.body.number, req.body.email, id_client], function(err, rows){
      if (err) {
        console.log("Ошибка вставки данных в БД (6-2)", err);
        res.send("error");
        return;
      }
      console.log("Вставка данных в БД успешна выполнена");
    });

    if (files_data === "NULL") {
      console.log("Изменяем базу без файлов");
      db.query("UPDATE orders SET address = ?, category = ?, description = ? WHERE id_order = ?",
      [req.body.address, req.body.category, req.body.desc, id_order], function(err, rows){
        if (err) {
          console.log("Ошибка вставки данных в БД (6-2)", err);
          res.send("error");
          return;
        }
        console.log("Вставка данных в БД успешна выполнена");
        res.send("Ваш заказ изменен");
      });
    } else {
      console.log("Изменяем базу с файлами");
      db.query("UPDATE orders SET address = ?, category = ?, files = ?, description = ? WHERE id_order = ?",
      [req.body.address, req.body.category, files_data, req.body.desc, id_order], function(err, rows){
        if (err) {
          console.log("Ошибка вставки данных в БД (6-3)", err);
          res.send("error");
          return;
        }
        console.log("Вставка данных в БД успешна выполнена");
        res.send("Ваш заказ изменен");
      });
    }
  });
});
//-----------------------------------------------------------------------------//

//--------------------------------Отмена заказа--------------------------------//
app.post('/cancelorder', upload.any(), function (req, res, next) {
  var id_order = req.body.order;

  db.query("UPDATE orders SET status = ? WHERE id_order = ?",
  ["Отменен заказчиком", id_order], function(err, rows) {
    if (err) {
      console.log("Ошибка обновления данных в БД (1-1)", err);
      res.send("error");
      return;
    }
    res.send("Ваш заказ отменен");
  });
});
//-----------------------------------------------------------------------------//

app.listen(3000, function() { console.log("Сервер успешно запущен"); });
