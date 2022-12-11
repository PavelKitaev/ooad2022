#include "settingfile.h"

SettingFile::SettingFile()
{
    file_name = "setting.ini";
    file.setFileName(file_name);
    param_count = 2;

    address = "IP_Address:";
    port = "Port:";
    default_address = "127.0.0.1";
    default_port = "1111";

    CheckFile();
}

QString SettingFile::GetIP()
{
    return GetData(address);
}

QString SettingFile::GetPort()
{
    return GetData(port);
}

int SettingFile::ChangeIP(QString newAddress)
{
    return ChangeData(address, newAddress);
}

int SettingFile::ChangePort(QString newPort)
{
    return ChangeData(port, newPort);
}

QString SettingFile::GetData(QString param)
{
    QString res, temp;

    if (file.open(QIODevice::ReadOnly |QIODevice::Text)) {                //Открываем файл на чтение
        while(!file.atEnd()) {                                            //Идем до строкам
            temp = file.readLine();                                       //Получаем строку

            if (temp.indexOf(param) == 0) {                               //Если в полученной строке есть нужное включение
                for (int i = 0; i < temp.length(); i++){                  //Достаем данные
                    if (temp[i] == '[') {                                 //Идем до открывающейся скобки
                        for (i = i + 1; i < temp.length(); i++) {
                            if (temp[i] == ']') {                         //Дошли до закр. скобки - выходим

                                break;
                            }

                            res += temp[i];                               //Записываем данные
                        }
                    }
                }

                break;
            }
        }
    }

    file.close(); //Закрываем файл

    return res;
}

int SettingFile::ChangeData(QString param, QString data)
{
    QStringList dataFile;

    // Открываем файл на чтение
    if (file.open(QIODevice::ReadOnly |QIODevice::Text)) {
        while(!file.atEnd()) {
            dataFile += file.readLine();
        }

         // Ищем индекс строки, в который находится параметр, который нужно изменить
         int index = -1;
         for (int i = 0; i < dataFile.length(); i++) {
             if (dataFile[i].indexOf(param) == 0)
             index = i;
         }

         // Изменяем
         if (index != -1) {
             dataFile[index] = param + "[" + data + "]\n";
         }
         // Очищаем файл
         file.remove();
    }

    // Записываем в файл новые данные
    if (file.open(QIODevice::WriteOnly |QIODevice::Text)) {
        for (int i = 0; i < dataFile.length(); i++) {
            file.write(dataFile[i].toUtf8());
        }
        file.close(); //Закрываем файл
    }

    return 0;
}

void SettingFile::CheckFile()
{
    int count = 0;

    QStringList data;

    if (file.open(QIODevice::ReadOnly |QIODevice::Text)) {
        while(!file.atEnd()) {
            data += file.readLine();
        }

        file.close();
    }

    if (data.length() > 0) {
        if (data[0].indexOf(address) == 0) {
            count++;
        }

        if (data[1].indexOf(port) == 0) {
            count ++;
        }
    }

    // Если файл содержит меньше параметров, чем должно быть - перезаписываем содержимое
    if (count != param_count) {
        file.remove(); // Очищаем файл

        //Открываем файл на запись
        if (file.open(QIODevice::WriteOnly |QIODevice::Text)) {
            file.write((address + "[" + default_address + "]\n").toUtf8());
            file.write((port + "[" + default_port + "]\n").toUtf8());

            file.close(); //Закрываем файл
        }
    }
}
