#include "ParseMessageManagerApp.h"

ParseMessageManagerApp::ParseMessageManagerApp(SOCKET _connection, sql::Connection* _con)
{
	connection = _connection;
	con = _con;

	pstmt = 0;
	result = 0;
}

std::string ParseMessageManagerApp::Processing(std::string msg)
{
	std::string res = "empty";
	int border = msg.find_first_of(":");

	std::string command = msg.substr(0, border);
	std::string basis = msg.substr(border + 1, msg.length());

	if (command == "Authentication") {												// Authentication:Login|Password
		res = CommandAuthentication(basis);
	}
	else if (command == "GiveDataOrder" && auth) {					//GiveDataOrder:NumberOrder
		res = CommangGiveDataOrder(basis);
	}
	else if (command == "GiveStat" && auth) {								//GiveStat:access
		res = CommandStat();
	}
	else if (command == "GiveNewOrders" && auth) {								//GiveNewOrders:access
		res = CommandNewOrders();
	}
	else if (command == "GiveActivOrders" && auth) {							//GiveActivOrders:access
		res = CommandActivOrders();
	}
	else if (command == "UpdateOrderData" && auth) {					//UpdateOrderData:OrderNumber;branch;status;comment
		res = CommandUpdateOrderData(basis);
	}
	else if (command == "GiveFiles" && auth) {								//GiveFiles:OrderNumber
		res = CommandDownFiles(basis);
	}

	return res;
}

void ParseMessageManagerApp::split(std::string const& str, const char sep, std::vector<std::string>& res)
{
	size_t start;
	size_t end = 0;

	while ((start = str.find_first_not_of(sep, end)) != std::string::npos)
	{
		end = str.find(sep, start);
		res.push_back(str.substr(start, end - start));
	}
}

std::string ParseMessageManagerApp::CommandAuthentication(std::string msg)
{
	std::string res = "Failed Auth";
	std::string login;
	std::string pass;

	int border = msg.find_first_of("|");

	login = msg.substr(0, border);
	pass = msg.substr(border + 1, msg.length());


	pstmt = con->prepareStatement("SELECT " + (std::string)MANAGERS_PASSWORD_COL + " FROM " + (std::string)MANAGERS_TABLE + " WHERE " + (std::string)MANAGERS_LOGIN_COL + " = ? ");
	pstmt->setString(1, login);
	result = pstmt->executeQuery();
	result->next();

	try
	{
		if (result->getString(1).c_str() == pass) {
			auth = true;
			res = "Success";
		}
	}
	catch (sql::SQLException e)
	{
		res = "Error: ";
		res += e.what();
	}

	return res;
}

std::string ParseMessageManagerApp::CommandStat()
{
	setlocale(LC_ALL, "");
	std::string res;

	pstmt = con->prepareStatement("SELECT COUNT(*) FROM " + (std::string)ORDERS_TABLE + " WHERE " + (std::string)ORDERS_STATUS_COL + " = ?");
	pstmt->setString(1, STATUS_COMPLITED_ORDERS);

	result = pstmt->executeQuery();
	result->next();
	res = result->getString(1).c_str();
	res += ":";

	pstmt = con->prepareStatement("SELECT COUNT(*) FROM " + (std::string)ORDERS_TABLE + " WHERE NOT " + (std::string)ORDERS_STATUS_COL
		+ " = ? AND NOT " + (std::string)ORDERS_STATUS_COL + " = ? AND NOT " + (std::string)ORDERS_STATUS_COL + " = ? ");
	pstmt->setString(1, STATUS_COMPLITED_ORDERS);
	pstmt->setString(2, STATUS_CANCELLED_ORDERS_1);
	pstmt->setString(3, STATUS_CANCELLED_ORDERS_2);

	result = pstmt->executeQuery();
	result->next();
	res += result->getString(1).c_str();
	res += ":";

	pstmt = con->prepareStatement("SELECT COUNT(*) FROM " + (std::string)ORDERS_TABLE + " WHERE " + (std::string)ORDERS_STATUS_COL + " = ? OR "
		+ (std::string)ORDERS_STATUS_COL + " = ?");
	pstmt->setString(1, STATUS_CANCELLED_ORDERS_1);
	pstmt->setString(2, STATUS_CANCELLED_ORDERS_2);

	result = pstmt->executeQuery();
	result->next();
	res += result->getString(1).c_str();

	return res;
}

std::string ParseMessageManagerApp::CommandNewOrders()
{
	std::string res = "";

	pstmt = con->prepareStatement("SELECT " + (std::string)ORDERS_ID_ORDER_COL + ", " + (std::string)ORDERS_CATEGORY_COL
		+ " FROM " + (std::string)ORDERS_TABLE + " WHERE " + (std::string)ORDERS_STATUS_COL + " = ?");
	pstmt->setString(1, STATUS_NEW_ORDERS);
	result = pstmt->executeQuery();

	while (result->next()) {
		res += result->getString(1).c_str();
		res += ":";

		res += result->getString(2).c_str();
		res += ";";
	}

	res = res.substr(0, res.size() - 1);
	return res;
}

std::string ParseMessageManagerApp::CommandActivOrders()
{
	std::string res = "";

	pstmt = con->prepareStatement("SELECT " + (std::string)ORDERS_ID_ORDER_COL + ", " + (std::string)ORDERS_CATEGORY_COL
		+ " FROM " + (std::string)ORDERS_TABLE + " WHERE NOT " + (std::string)ORDERS_STATUS_COL + " = ? AND NOT "
		+ (std::string)ORDERS_STATUS_COL + " = ? AND NOT " + (std::string)ORDERS_STATUS_COL + " = ? AND NOT "
		+ (std::string)ORDERS_STATUS_COL + " = ? ");
	pstmt->setString(1, STATUS_NEW_ORDERS);
	pstmt->setString(2, STATUS_CANCELLED_ORDERS_1);
	pstmt->setString(3, STATUS_CANCELLED_ORDERS_2);
	pstmt->setString(4, STATUS_COMPLITED_ORDERS);
	result = pstmt->executeQuery();

	while (result->next()) {
		res += result->getString(1).c_str();
		res += ":";

		res += result->getString(2).c_str();
		res += ";";
	}

	res = res.substr(0, res.size() - 1);

	return res;
}

std::string ParseMessageManagerApp::CommangGiveDataOrder(std::string number_order)
{
	std::string res = "";
	pstmt = con->prepareStatement("SELECT " + (std::string)ORDERS_ID_CLIENT_COL + " FROM " + (std::string)ORDERS_TABLE
		+ " WHERE " + (std::string)ORDERS_ID_ORDER_COL + " = ? ");
	pstmt->setString(1, number_order);
	result = pstmt->executeQuery();
	result->next();

	std::string id_client = result->getString(1).c_str();

	pstmt = con->prepareStatement("SELECT " + (std::string)CLIENTS_FULLNAME_COL + ", " + (std::string)CLIENTS_NUMTEL_COL
		+ ", " + (std::string)CLIENTS_EMAIL_COL + " FROM " + (std::string)CLIENTS_TABLE + " WHERE "
		+ (std::string)CLIENTS_ID_CLIENT_COL + " = ? ");
	pstmt->setString(1, id_client);
	result = pstmt->executeQuery();
	result->next();

	res += result->getString(1).c_str(); //ФИО
	res += ";";

	res += result->getString(2).c_str(); //Номер
	res += ";";

	res += result->getString(3).c_str();	//Почта
	res += ";";

	pstmt = con->prepareStatement("SELECT " + (std::string)ORDERS_ID_BRANCH_COL + ", "
		+ (std::string)ORDERS_ADDRESS_COL + ", "
		+ (std::string)ORDERS_CATEGORY_COL + ", "
		+ (std::string)ORDERS_DESC_COL + ", "
		+ (std::string)ORDERS_FILES_COL + ", "
		+ (std::string)ORDERS_STATUS_COL + ", "
		+ (std::string)ORDERS_COMMENT_COL + " FROM " + (std::string)ORDERS_TABLE + " WHERE "
		+ (std::string)ORDERS_ID_ORDER_COL + " = ? ");
	pstmt->setString(1, number_order);
	result = pstmt->executeQuery();
	result->next();

	std::string id_branch = result->getString(1).c_str();

	res += result->getString(2).c_str(); //Адрес
	res += ";";

	std::string category = result->getString(3).c_str(); //Категория
	res += category;
	res += ";";

	res += result->getString(4).c_str(); //Описание
	res += ";";

	res += result->getString(5).c_str(); //Файлы
	res += ";";

	res += result->getString(6).c_str(); //Статус
	res += ";";

	res += result->getString(7).c_str(); //Коммент
	res += ";";

	if (strlen(result->getString(1).c_str()) != 0) { //Назначенный филиал
		pstmt = con->prepareStatement("SELECT " + (std::string)BRANCHES_ORDERSPROG_COL + ", " + (std::string)BRANCHES_ADDRESS_COL + " FROM "
			+ (std::string)BRANCHES_TABLE + " WHERE " + (std::string)BRANCHES_ID_BRANCH_COL + " = ? ");
		pstmt->setString(1, id_branch);
		result = pstmt->executeQuery();
		result->next();

		res += "(" + result->getString(1) + ") " + result->getString(2);
	}
	else { //Вычисляем филиалы, где возможно выполнить заказ. Основываясь на наличии оборудования

		pstmt = con->prepareStatement("SELECT " + (std::string)HARDWARE_IDS_BRANCHES_COL + " FROM " + (std::string)HARDWARE_TABLE
			+ " WHERE " + (std::string)HARDWARE_CATEGORY_COL + " = ? ");
		pstmt->setString(1, category);
		result = pstmt->executeQuery();
		result->next();

		std::string branches = result->getString(1).c_str();

		std::vector<std::string> out;
		split(branches, ';', out);

		for (int i = 0; i < out.size(); i++) {
			pstmt = con->prepareStatement("SELECT " + (std::string)BRANCHES_ORDERSPROG_COL + ", " + (std::string)BRANCHES_ADDRESS_COL + " FROM "
				+ (std::string)BRANCHES_TABLE + " WHERE " + (std::string)BRANCHES_ID_BRANCH_COL + " = ?");
			pstmt->setString(1, out[i]);
			result = pstmt->executeQuery();
			result->next();

			res += "(" + result->getString(1) + ") " + result->getString(2);
			if (i != out.size() - 1) {
				res += ":";
			}
		}
	}

	return res;
}

std::string ParseMessageManagerApp::CommandUpdateOrderData(std::string data)
{
	std::string res = "Success";
	// out[0] - номер заказа
	// out[1] - адрес филиала
	// out[2] - статус
	// out[3] - комментарий

	std::vector<std::string> out;
	split(data, ';', out);

	pstmt = con->prepareStatement("SELECT " + (std::string)BRANCHES_ID_BRANCH_COL + " FROM "
		+ (std::string)BRANCHES_TABLE + " WHERE " + (std::string)BRANCHES_ADDRESS_COL + " = ?");
	pstmt->setString(1, out[1]);
	result = pstmt->executeQuery();
	result->next();

	std::string id_branch = result->getString(1).c_str();

	pstmt = con->prepareStatement("UPDATE " + (std::string)ORDERS_TABLE + " SET " + (std::string)ORDERS_ID_BRANCH_COL + " = ?, "
		+ (std::string)ORDERS_STATUS_COL + " = ?, " + (std::string)ORDERS_COMMENT_COL + " = ? WHERE "
		+ (std::string)ORDERS_ID_ORDER_COL + " = ?");
	pstmt->setString(1, id_branch);
	pstmt->setString(2, out[2]);
	pstmt->setString(3, out[3]);
	pstmt->setString(4, out[0]);

	try
	{
		pstmt->executeQuery();
	}
	catch (sql::SQLException e)
	{
		res = "Failed. ";
		res += e.what();
	}

	std::string count_orders;
	try
	{
		pstmt = con->prepareStatement("SELECT COUNT(*) FROM " + (std::string)ORDERS_TABLE + " WHERE "
			+ (std::string)ORDERS_ID_BRANCH_COL + " = ? AND NOT " + (std::string)ORDERS_STATUS_COL + " = ? AND NOT "
			+ (std::string)ORDERS_STATUS_COL + " = ? AND NOT " + (std::string)ORDERS_STATUS_COL + " = ? AND NOT " + (std::string)ORDERS_STATUS_COL + " = ?");
		pstmt->setString(1, id_branch);
		pstmt->setString(2, STATUS_COMPLITED_ORDERS);
		pstmt->setString(3, STATUS_CANCELLED_ORDERS_1);
		pstmt->setString(4, STATUS_CANCELLED_ORDERS_2);
		pstmt->setString(5, STATUS_NEW_ORDERS);
		result = pstmt->executeQuery();
		result->next();

		count_orders = result->getString(1).c_str();
		std::cout << "COUNT ORDERS: " << count_orders << std::endl;
	}
	catch (sql::SQLException e)
	{
		std::cout << "Error message: " << e.what() << std::endl;
	}

	try
	{
		pstmt = con->prepareStatement("UPDATE " + (std::string)BRANCHES_TABLE + " SET " + (std::string)BRANCHES_ORDERSPROG_COL
			+ " = ? WHERE " + (std::string)BRANCHES_ID_BRANCH_COL + " = ?");
		pstmt->setString(1, count_orders);
		pstmt->setString(2, id_branch);
		pstmt->executeQuery();
	}
	catch (sql::SQLException e)
	{
		std::cout << "Error message: " << e.what() << std::endl;
	}

	return res;
}

std::string ParseMessageManagerApp::CommandDownFiles(std::string number_order)
{
	std::string res = "Success";

	pstmt = con->prepareStatement("SELECT " + (std::string)ORDERS_FILES_COL + " FROM " + (std::string)ORDERS_TABLE
		+ " WHERE " + (std::string)ORDERS_ID_ORDER_COL + " = ?");
	pstmt->setString(1, number_order);
	result = pstmt->executeQuery();
	result->next();

	std::string files = result->getString(1).c_str();
	if (files != "NULL") {
		std::vector<std::string> out;
		split(files, ':', out);

		int respondSize = files.size() + 1;

		send(connection, (char*)&respondSize, sizeof(int), 0);
		send(connection, files.c_str(), respondSize, 0);

		int part_size = 1024;
		for (int i = 1; i <= stoi(out[0]); i++)
		{
			std::string temp = FILES_PATH + out[i];
			FILE* f;
			fopen_s(&f, temp.c_str(), "rb");

			struct stat si;
			if (stat(temp.c_str(), &si)) {
				return "Failed";
			}

			if (send(connection, (char*)&si.st_size, sizeof(si.st_size), 0) == SOCKET_ERROR)
			{
				printf("send error\n");
				return "Failed";
			}

			int parts_count = si.st_size / part_size;
			int last_part_size = si.st_size % part_size;
			//std::cout << "PARTS_COUNT: " << parts_count << "   LAST_PART_SIZE: " << last_part_size << std::endl;

			if (!f)
			{
				printf("fopen error\n");
				system("pause");
				return "Failed";
			}

			char* buffer = new char[part_size];

			for (int i = 0; i < parts_count; i++) {
				if (fread(buffer, 1, part_size, f) != part_size)
				{
					printf("fread error\n");
					system("pause");
					return "Failed";
				}
				if (send(connection, buffer, part_size, 0) == SOCKET_ERROR)
				{
					printf("send error\n");
					system("pause");
					return "Failed";
				}
			}

			if (last_part_size)
			{
				if (fread(buffer, 1, last_part_size, f) != last_part_size)
				{
					printf("fread error\n");
					system("pause");
					return "Failed";
				}
				if (send(connection, buffer, last_part_size, 0) == SOCKET_ERROR)
				{
					printf("send error\n");
					system("pause");
					return "Failed";
				}
			}

			delete[] buffer;

			fclose(f);

			Sleep(100);
		}
	}
	else {
		res = "NoFiles";
	}

	return res;
}