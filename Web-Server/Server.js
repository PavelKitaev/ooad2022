var response = "-1";                          //Ответ сервера

var _ = require('underscore');

const util = require('util');
const setTimeoutPromise = util.promisify(setTimeout);

const express = require('express');         //Маршрутизация
const multer = require('multer');           //Сохранение файлов от пользователя
const fs = require("fs");                   //Для файловой системы
const archiver  = require('archiver');      //Для создания архива
var app = express();

var net = require('net');
var client = new net.Socket();
client.connect(1337, '192.168.1.50', function() {
	console.log('Connected');
	client.write('WebServer\0');
});

client.on('data', function(data) {
	console.log('Received response: ' + data);
  response = data.toString();
});

app.set('view engine', 'ejs');                    //Движок
app.use('/assets', express.static('assets'));     //Директория с публичным содержимым
//-----------------------------------------------------------------------------//

//------------------------Директория для хранения файлов-----------------------//
const storage = multer.diskStorage({
   //Директория в которую будут сохранятся файлы
   destination: function(req, file, cb) {cb(null, 'C:/YouPhotoFilesClients/')},
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
  var files_data = "NULL";
  //Если были получены файлы, добавляем их названия в строку
  if (req.files.length > 0)
  {
    files_data = req.files.length + ":";

    for (let i = 0; i < req.files.length; i++)
    {
      files_data += req.files[i].originalname;

      if (i != req.files.length - 1) {
        files_data += ":";
      }
    }
  }

  //Отправляем данные на сервер
  let temp = "CreateOrder:" + req.body.fio + ";" + req.body.number + ";" + req.body.email + ";" +
  req.body.address + ";" + files_data + ";" + req.body.category + ";" + req.body.desc + '\0';

  client.write(temp);

  setTimeoutPromise(1000, 'foobar').then((value) => {
	  if (response.includes("-1")){
		  res.send("Ошибка, попробуйте позже");
	  }
	  else {
		res.send("Ваш заказ создан под номером - " + response);
	  }
    
    response = "-1";
  });
});
//-----------------------------------------------------------------------------//


//-------------------------Отслеживание/Изменение заказа-----------------------//
app.post('/checkstatusorder', upload.any(), function (req, res, next) {
  var from = req.body.from;

  //Отправляем данные на сервер
  let temp = "GiveStatus:" + req.body.order + '\0';

  client.write(temp);

  setTimeoutPromise(1000, 'foobar').then((value) => {
    if (!response.includes("-1")) {
      if (from === 'changeoreder'){
        if (response.includes("В работе")){
          res.send("blocked");
        } 
		else if (response.includes("Отменен заказчиком") || response.includes("Отменен менеджером")){
          res.send("cancelled");
        } 
		else {
          res.send("unblocked");
        }
      } 
	  else if (from === 'trackorder') {
		res.send(response);
      }
    }
    else {
      res.send("not found");
    }

    response = "-1";
  });
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

  var address = "null";
  var fio = "null";
  var num_tel = "null";
  var category = "null";
  var email = "null";
  var desc = "null";

  console.log(typeof response);

  //Отправляем данные на сервер
  let temp = "GiveOrderData:" + order + '\0';

  client.write(temp);

  setTimeoutPromise(1000, 'foobar').then((value) => {
    //[0] - fio
    //[1] - number tel
    //[2] email
    //[3] - address
    //[4] - category
    //[5] - description

    let parseData = response.split(";");

    fio = parseData[0];
    num_tel = parseData[1];
    email = parseData[2];
    address = parseData[3];
    category = parseData[4];
    desc = parseData[5];

    res.render('changeoreder', {
      fio: fio,
      address: address,
      num_tel: num_tel,
      email: email,
      category: category,
      desc: desc,
      order: order,
      });


    response = "-1";
  });
})
//-----------------------------------------------------------------------------//

//--------------------------Изменение данных о заказе--------------------------//
app.post('/orderchange', upload.any(), function (req, res, next) {
  var id_order = req.body.order;
  var id_client = -1;
  var files_data = "NULL";

  //Если были получены файлы, добавляем их названия в строку
  if (req.files.length > 0) {
    console.log("Files: " + req.files.length);

    files_data = req.files.length + ":";

    for (let i = 0; i < req.files.length; i++) {
      files_data += req.files[i].originalname;

      if (i != req.files.length - 1) {
        files_data += ":";
      }
    }
  }

  //Отправляем данные на сервер
  let temp = "UpdateOrder:" + id_order + ";" + req.body.fio
              + ";" + req.body.number + ";" + req.body.email + ";" + req.body.address
              + ";" + req.body.category + ";" + req.body.desc + ";";

  if (files_data.includes("NULL")) {
    console.log("files null!");
    temp += "NULL\0";
  }
  else {
    console.log("files is not null!");
    temp += files_data;
    temp += '\0';
  }

  client.write(temp);

  setTimeoutPromise(1000, 'foobar').then((value) => {
    if (response.includes('Failed')) {
      res.send("Ошибка, попробуйте позже");
    }
    else {
      res.send("Ваш заказ изменен");
    }

    response = "-1";
  });

});
//-----------------------------------------------------------------------------//

//--------------------------------Отмена заказа--------------------------------//
app.post('/cancelorder', upload.any(), function (req, res, next) {
  var id_order = req.body.order;

  //Отправляем данные на сервер
  let temp = "CancelOrder:" + req.body.order + '\0';

  client.write(temp);

  setTimeoutPromise(1000, 'foobar').then((value) => {
    if (response != "-1") {
      res.send("Ваш заказ отменен");
    }
    else {
      res.send("Ошибка, попробуйте позже");
    }

    response = "-1";
  });

});
//-----------------------------------------------------------------------------//

app.listen(3000, function() { console.log("Сервер успешно запущен"); });
