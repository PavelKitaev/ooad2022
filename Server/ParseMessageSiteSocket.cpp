#include "ParseMessageSiteSocket.h"

ParseMessageSiteSocket::ParseMessageSiteSocket(SOCKET _connection, sql::Connection* _con)
{
	connection = _connection;
	con = _con;

	pstmt = 0;
	result = 0;
}

std::string ParseMessageSiteSocket::Processing(std::string msg)
{
	std::cout << "Parse: " << msg << std::endl;
	std::string res = "empty";
	int border = msg.find_first_of(":");

	std::string command = msg.substr(0, border);
	std::string basis = msg.substr(border + 1, msg.length());

	if (command == "CreateOrder") {
		res = CommandCreateOrder(basis);
	}
	else if (command == "GiveStatus") {
		res = CommandGiveStatus(basis);
	}
	else if (command == "GiveOrderData") {
		res = CommandGiveOrderData(basis);
	}
	else if (command == "CancelOrder") {
		res = CommandCancelOrder(basis);
	}
	else if (command == "UpdateOrder") {
		res = CommandUpdateOrder(basis);
	}

	return res;
}

void ParseMessageSiteSocket::split(std::string const& str, const char sep, std::vector<std::string>& res)
{
	size_t start;
	size_t end = 0;

	while ((start = str.find_first_not_of(sep, end)) != std::string::npos)
	{
		end = str.find(sep, start);
		res.push_back(str.substr(start, end - start));
	}
}

std::string ParseMessageSiteSocket::CommandCreateOrder(std::string data)
{
	std::string res = "-1";

	std::vector<std::string> out;
	split(data, ';', out);

	//[0] - FIO
	//[1] - number tel
	//[2] - email
	//[3] - address
	//[4] - files
	//[5] - category
	//[6] - description

	try
	{
		pstmt = con->prepareStatement("INSERT INTO " + (std::string)CLIENTS_TABLE + " (" + (std::string)CLIENTS_FULLNAME_COL + ", "
			+ (std::string)CLIENTS_NUMTEL_COL + ", " + (std::string)CLIENTS_EMAIL_COL + ") VALUES (?, ?, ?)");
		pstmt->setString(1, out[0]);
		pstmt->setString(2, out[1]);
		pstmt->setString(3, out[2]);
		pstmt->executeQuery();

		pstmt = con->prepareStatement("SELECT " + (std::string)CLIENTS_ID_CLIENT_COL + " FROM " + (std::string)CLIENTS_TABLE
			+ " WHERE " + (std::string)CLIENTS_ID_CLIENT_COL + " = LAST_INSERT_ID()");
		result = pstmt->executeQuery();
		result->next();

		std::string id_client = result->getString(1);

		pstmt = con->prepareStatement("INSERT INTO " + (std::string)ORDERS_TABLE + " (" + (std::string)ORDERS_ID_CLIENT_COL + ", "
			+ (std::string)ORDERS_STATUS_COL + ", " + (std::string)ORDERS_ADDRESS_COL + ", "
			+ (std::string)ORDERS_FILES_COL + ", " + (std::string)ORDERS_CATEGORY_COL + ", "
			+ (std::string)ORDERS_DESC_COL + ") VALUES (?, ?, ?, ?, ?, ?)");
		pstmt->setString(1, id_client);
		pstmt->setString(2, STATUS_NEW_ORDERS);
		pstmt->setString(3, out[3]);
		pstmt->setString(4, out[4]);
		pstmt->setString(5, out[5]);
		pstmt->setString(6, out[6]);
		pstmt->executeQuery();

		pstmt = con->prepareStatement("SELECT " + (std::string)ORDERS_ID_ORDER_COL + " FROM " + (std::string)ORDERS_TABLE
			+ " WHERE " + (std::string)ORDERS_ID_ORDER_COL + " = LAST_INSERT_ID()");
		result = pstmt->executeQuery();
		result->next();

		res = result->getString(1);
	}
	catch (sql::SQLException e)
	{
		std::cout << "Error message: " << e.what() << std::endl;
		res = "-1";
	}

	return res;
}

std::string ParseMessageSiteSocket::CommandGiveStatus(std::string data)
{
	pstmt = con->prepareStatement("SELECT " + (std::string)ORDERS_STATUS_COL + " FROM " + (std::string)ORDERS_TABLE
		+ " WHERE " + (std::string)ORDERS_ID_ORDER_COL + " = ?");
	pstmt->setString(1, data);
	result = pstmt->executeQuery();
	if (result->next()) {
		return result->getString(1);
	}

	return "-1";
}

std::string ParseMessageSiteSocket::CommandGiveOrderData(std::string data)
{
	std::string res;
	pstmt = con->prepareStatement("SELECT * FROM " + (std::string)ORDERS_TABLE + " LEFT JOIN " + (std::string)CLIENTS_TABLE
		+ " ON " + (std::string)ORDERS_TABLE + "." + ORDERS_ID_CLIENT_COL + " = "
		+ (std::string)CLIENTS_TABLE + "." + (std::string)CLIENTS_ID_CLIENT_COL + " WHERE "
		+ (std::string)ORDERS_ID_ORDER_COL + " = ?");
	pstmt->setString(1, data);
	result = pstmt->executeQuery();
	result->next();

	res = result->getString("full_name");
	res += ";";

	res += result->getString("num_tel");
	res += ";";

	res += result->getString("email");
	res += ";";

	res += result->getString("address");
	res += ";";

	res += result->getString("category");
	res += ";";

	res += result->getString("description");
	res += ";";

	return res;
}

std::string ParseMessageSiteSocket::CommandCancelOrder(std::string data)
{
	std::string res = "Success";

	pstmt = con->prepareStatement("UPDATE " + (std::string)ORDERS_TABLE + " SET status = ? WHERE id_order = ?");
	pstmt->setString(1, STATUS_CANCELLED_ORDERS_1);
	pstmt->setString(2, data);

	try
	{
		pstmt->executeQuery();
	}
	catch (sql::SQLException e)
	{
		res = "Failed";
	}

	return res;
}

std::string ParseMessageSiteSocket::CommandUpdateOrder(std::string data)
{
	std::string res = "Success";

	std::vector<std::string> out;
	split(data, ';', out);

	//[0] - id_order
	//[1] - fio
	//[2] - number tel
	//[3] - email
	//[4] - address
	//[5] - category
	//[6] - description
	//[7] - files

	try
	{
		pstmt = con->prepareStatement("SELECT " + (std::string)ORDERS_ID_CLIENT_COL + " FROM "
			+ (std::string)ORDERS_TABLE + " WHERE " + (std::string)ORDERS_ID_ORDER_COL + " = ?");
		pstmt->setString(1, out[0]);
		result = pstmt->executeQuery();
		result->next();

		std::string id_client = result->getString(1);

		pstmt = con->prepareStatement("UPDATE " + (std::string)CLIENTS_TABLE + " SET "
			+ (std::string)CLIENTS_FULLNAME_COL + " = ?, "
			+ (std::string)CLIENTS_NUMTEL_COL + " = ?, "
			+ (std::string)CLIENTS_EMAIL_COL + " = ? WHERE "
			+ (std::string)CLIENTS_ID_CLIENT_COL + " = ?");
		pstmt->setString(1, out[1]);
		pstmt->setString(2, out[2]);
		pstmt->setString(3, out[3]);
		pstmt->setString(4, id_client);

		pstmt->executeQuery();

		if (out[7] == "NULL") {
			pstmt = con->prepareStatement("UPDATE " + (std::string)ORDERS_TABLE + " SET "
				+ (std::string)ORDERS_ADDRESS_COL + " = ?, "
				+ (std::string)ORDERS_CATEGORY_COL + " = ?, "
				+ (std::string)ORDERS_DESC_COL + " = ? WHERE "
				+ (std::string)ORDERS_ID_ORDER_COL + " = ?");
			pstmt->setString(1, out[4]);
			pstmt->setString(2, out[5]);
			pstmt->setString(3, out[6]);
			pstmt->setString(4, out[0]);
		}
		else {
			pstmt = con->prepareStatement("UPDATE " + (std::string)ORDERS_TABLE + " SET "
				+ (std::string)ORDERS_ADDRESS_COL + " = ?, "
				+ (std::string)ORDERS_CATEGORY_COL + " = ?, "
				+ (std::string)ORDERS_DESC_COL + " = ?, "
				+ (std::string)ORDERS_FILES_COL + " = ? WHERE "
				+ (std::string)ORDERS_ID_ORDER_COL + " = ?");
			pstmt->setString(1, out[4]);
			pstmt->setString(2, out[5]);
			pstmt->setString(3, out[6]);
			pstmt->setString(4, out[7]);
			pstmt->setString(5, out[0]);
		}

		pstmt->executeQuery();
	}
	catch (sql::SQLException e)
	{
		std::cout << "Error message: " << e.what() << std::endl;
		std::cout << "Error message: " << e.getSQLStateCStr() << std::endl;

		return "Failed";
	}

	return res;
}
