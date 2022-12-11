#ifndef SETTINGFILE_H
#define SETTINGFILE_H

#include <QFile>
#include <QDebug>
//#include <QRegExp>
#include <QRegularExpression>
class SettingFile
{
public:
    SettingFile();

    QString GetIP();
    QString GetPort();

    int ChangeIP(QString newAddress);
    int ChangePort(QString newPort);

private:
    QFile file;
    QString address;
    QString port;
    QString default_address;
    QString default_port;
    int param_count;

    QString file_name;;

    QString GetData(QString param);
    int ChangeData(QString param, QString data);
    void CheckFile();
};

#endif // SETTINGFILE_H
