#ifndef PARSE_MESSAGE_SITE_SOCKET_H_
#define PARSE_MESSAGE_SITE_SOCKET_H_

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

class ParseMessageSiteSocket
{
public:
	ParseMessageSiteSocket(SOCKET _connection, sql::Connection* _con);

	std::string Processing(std::string msg);

	~ParseMessageSiteSocket() {
		if (result != NULL)
			delete result;
		if (pstmt != NULL)
			delete pstmt;
	};

private:
	SOCKET connection;

	sql::Connection* con;
	sql::PreparedStatement* pstmt;	//Insert, select
	sql::ResultSet* result;

	void split(std::string const& str, const char sep, std::vector<std::string>& res);
	std::string CommandCreateOrder(std::string data);
	std::string CommandGiveStatus(std::string data);
	std::string CommandGiveOrderData(std::string data);
	std::string CommandCancelOrder(std::string data);
	std::string CommandUpdateOrder(std::string data);
};

#endif   // PARSE_MESSAGE_SITE_SOCKET_H_
