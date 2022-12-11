#ifndef PARSE_MESSAGE_MANAGER_APP_H_
#define PARSE_MESSAGE_MANAGER_APP_H_

#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <fstream>

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <windows.h>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

#include "constants.h"

class ParseMessageManagerApp
{
public:
	ParseMessageManagerApp(SOCKET _connection, sql::Connection* _con);
	std::string Processing(std::string msg);

	~ParseMessageManagerApp() {
		if ( result != NULL)
			delete result;
		if (pstmt != NULL)
			delete pstmt;
	};

private:
	bool auth = false;
	SOCKET connection;

	sql::Connection* con;
	sql::PreparedStatement* pstmt;	//Insert, select
	sql::ResultSet* result;

	void split(std::string const& str, const char sep, std::vector<std::string>& res);

	std::string CommandAuthentication(std::string msg);
	std::string CommandStat();
	std::string CommandNewOrders();
	std::string CommandActivOrders();
	std::string CommangGiveDataOrder(std::string number_order);
	std::string CommandUpdateOrderData(std::string data);
	std::string CommandDownFiles(std::string number_order);
};

#endif   // PARSE_MESSAGE_MANAGER_APP_H_
