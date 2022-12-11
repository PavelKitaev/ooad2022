#ifndef SERVER_H_
#define SERVER_H_

#include <thread>
#include <iostream>
#include <vector>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

#include "ParseMessageManagerApp.h"
#include "ParseMessageSiteSocket.h"
#include "constants.h"

template<typename T>
struct tThrd
{
  typedef DWORD(__thiscall T::* PMethod)();
  static DWORD WINAPI Function(PVOID pParam) {
    return (((tThrd*)pParam)->pThis->*((tThrd*)pParam)->pMethod)();
  };

  T*          pThis;
  PMethod     pMethod;
  HANDLE      Handle;
};

class Server
{
public:
  Server(std::string address);
  int Start();
  int Stop();
  ~Server();

private:
  static const int max_clients = 10;

  DWORD GlobalHandle();
  DWORD ClientHandle();
  HANDLE  hThreadArray[max_clients];

  bool waitClients;
  bool processingClients;

  tThrd<Server> GlobalThread;
  tThrd<Server> ClientThread;

  sql::Connection* con;
  sql::PreparedStatement* pstmt;
  sql::Statement* stmt;
  sql::ResultSet* result;

  SOCKADDR_IN addr;
  SOCKET sListen;
  SOCKET newConnection;
  SOCKET Connections[max_clients]; //Массив всех сокетов

  int sizeofaddr = 0;
  int count = 0;
  int temp_index = -1;

  void CheckDataBase();
  int CheckTable(std::string nameTable);
  std::string CreateString(std::string nameTable);
  void DisconnectClient(int _index);
};

#endif   // SERVER_H_