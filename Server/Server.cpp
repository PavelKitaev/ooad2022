#include "Server.h"

Server::Server(std::string address)
{
	sql::Driver* driver;

	//Подключаемся к базе данных
	try {
		driver = get_driver_instance();
		Server::con = driver->connect("tcp://192.168.1.49:3306", "admin", "M2e3D5f2!");
		Server::con->setSchema(NAME_DATABASE); //Выбор базы данных
		std::cout << "Successful database connection" << std::endl;
		CheckDataBase();
		std::cout << "Successful database checked" << std::endl;
	}
	catch (sql::SQLException e) {
		std::cout << "Error message: " << e.what() << std::endl;
		system("pause");
		exit(1);
	}

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);

	//Инициализируем библиотеку сокетов
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	sizeofaddr = sizeof(addr);
	char addressS[] = "192.168.1.50";
	inet_pton(AF_INET, addressS, &(addr.sin_addr));
	addr.sin_port = htons(1337);
	addr.sin_family = AF_INET;

	sListen = socket(AF_INET, SOCK_STREAM, NULL);
	if (sListen == INVALID_SOCKET) {
		std::cout << "Init socket: Error. " << WSAGetLastError() << std::endl;
	}
	else {
		std::cout << "Init socket: OK" << std::endl;
	}
	
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	waitClients = true;
	processingClients = true;

	std::cout << "Start server: OK" << std::endl;
}

int Server::Start()
{
	int res = 0;
	try {
		GlobalThread.pThis = this;
		GlobalThread.pMethod = &Server::GlobalHandle;
		GlobalThread.Handle = CreateThread(NULL, 0, GlobalThread.Function, &GlobalThread, 0, NULL);
	}
	catch (const char* msg) {
		res = 1;
	}


	return res;
}

int Server::Stop()
{
	int res = 0;

	try {
		waitClients = false;
		processingClients = false;

		closesocket(sListen);
		closesocket(newConnection);

		for (int i = 0; i < count; i++) {
			closesocket(Connections[i]);
		}

		WSACleanup();

		WaitForMultipleObjects(count, hThreadArray, TRUE, INFINITE);

		for (int i = 0; i < count; i++)
		{
			CloseHandle(hThreadArray[i]);
		}

		WaitForSingleObject(GlobalThread.Handle, INFINITE);
		CloseHandle(GlobalThread.Handle);

		std::cout << "Stop server: OK" << std::endl;
	}
	catch (const char* msg) {
		res = 1;
	}

	return res;
}

Server::~Server()
{
	if (con != NULL) {
		delete con;
	}
}

DWORD Server::GlobalHandle()
{
	std::cout << "Waiting for clients..." << std::endl;

	DWORD dwThreadId, dwThrdParam;

	while (waitClients) //Цикл на ожидание и обработку подключений
	{
		//Ожидание подключения
		if ((newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr)) != 0) {
			//Если места есть - обрабатываем
			if (count != max_clients) {
				//Ищем доступное место в массиве сокетов
				for (int i = 0; i < max_clients; i++) {
					if (Connections[i] == 0) {
						Connections[i] = newConnection; //Записываем
						count++; //Увеличиваем число активных клиентов
						dwThrdParam = newConnection;

						tThrd<Server> ClientThread;

						temp_index = i;
						ClientThread.pThis = this;
						ClientThread.pMethod = &Server::ClientHandle;
						hThreadArray[i] = CreateThread(NULL, 0, ClientThread.Function, &ClientThread, 0, NULL);
						ClientThread.Handle = hThreadArray[i];
						Sleep(100);

						break; //Выходим и вновь ожидаем подключение
					}
				}
			}
			else //Переполнение
			{
				std::cout << "Overflow";
				break;
			}
		}
	}

	return 0;
}

DWORD Server::ClientHandle()
{
	const int first_msg_size = 10;
	int ind = temp_index;

	char first_msg[first_msg_size];

	if (recv(Connections[ind], first_msg, first_msg_size, MSG_WAITALL) > 0) {
		std::cout << "New client connected. FirstMSG: " << first_msg << std::endl;

		if (strcmp(first_msg, "ManagerApp") == 0) {
			ParseMessageManagerApp* p = new ParseMessageManagerApp(Connections[ind], con);

			int msg_size = 0;
			while (processingClients)
			{
				if (recv(Connections[ind], (char*)&msg_size, sizeof(int), NULL) > 0)
				{
					std::cout << "msg_size: " << msg_size << std::endl;

					char* msg = new char[msg_size];

					if (recv(Connections[ind], msg, msg_size, NULL) > 0)
					{
						std::cout << "MSG: " << msg << std::endl;

						std::string respond = p->Processing(msg);
						int respondSize = respond.size() + 1;

						send(Connections[ind], (char*)&respondSize, sizeof(int), 0);
						send(Connections[ind], respond.c_str(), respondSize, 0);

						if (respond == "Failed Auth") {
							DisconnectClient(ind);
							delete p;
							return 0;
						}

						delete[] msg;
					}
				}
				else
				{
					DisconnectClient(ind);
					delete p;
					return 0;
				}
			}
		}
		else if (strcmp(first_msg, "WebServer") == 0) {
			std::cout << "WebServer connected" << std::endl;

			ParseMessageSiteSocket* p = new ParseMessageSiteSocket(Connections[ind], con);

			int msg_size = 1300; //	Сумма максимальной длинны полей таблиц для создания заказа

			while (true)
			{
				char* msg = new char[msg_size];
				if (recv(Connections[ind], msg, msg_size, NULL) > 0)
				{
					std::cout << "MSG: " << msg << std::endl;

					std::string respond = p->Processing(msg);
					respond += '\0';

					int respondSize = respond.size() + 1;
					std::cout << "Response: " << respond << std::endl;

					send(Connections[ind], respond.c_str(), respondSize, 0);
				}
				else
				{
					DisconnectClient(ind);
					delete p;
					return 0;
				}
				delete[] msg;
				std::cout << "tyt\n";
			}
		}
	}
	return 0;
}

void Server::CheckDataBase()
{
	stmt = con->createStatement();
	CheckTable(CLIENTS_TABLE); //Проверяем наличие таблицы CLIENTS
	CheckTable(ORDERS_TABLE); //Проверяем наличие таблицы ORDERS
	CheckTable(BRANCHES_TABLE);//Проверяем наличие таблицы BRANCHES
	CheckTable(HARDWARE_TABLE); //Проверяем наличие таблицы HARDWARE

	//Проверяем наличие таблицы ORDERS
	if (CheckTable(MANAGERS_TABLE) == 1) {
		//Добавление стандартной учетки
		std::cout << "Default accout created" << std::endl;
		pstmt = con->prepareStatement(CreateString("CreateAccount"));
		pstmt->execute();
	}

	delete stmt;
	delete pstmt;
}

int Server::CheckTable(std::string nameTable)
{
	int res = 0;
	try {
		pstmt = con->prepareStatement("SHOW TABLES FROM " + (std::string)NAME_DATABASE + " LIKE '" + nameTable + "'");
		pstmt->execute();
		result = pstmt->executeQuery();
		result->next();

		if (result->getString(1) == nameTable) {
			std::cout << "Table " << nameTable << ": OK" << std::endl;
		}

	}
	catch (sql::SQLException e)
	{
		std::cout << "Table " << nameTable << ": CREATING...";
		stmt->execute(CreateString(nameTable));
		std::cout << "     CREATED" << std::endl;
		res = 1;
	}

	return res;
}

std::string Server::CreateString(std::string nameTable)
{
	std::string res = "CREATE TABLE ";

	if (nameTable == CLIENTS_TABLE) {
		res += CLIENTS_TABLE;
		res += " (" + (std::string)CLIENTS_ID_CLIENT_COL + " int NOT NULL AUTO_INCREMENT, ";
		res += (std::string)CLIENTS_FULLNAME_COL + " varchar(45) NOT NULL, ";
		res += (std::string)CLIENTS_NUMTEL_COL + " varchar(12) NOT NULL, ";
		res += (std::string)CLIENTS_EMAIL_COL + " varchar(255) NOT NULL, PRIMARY KEY(" + (std::string)CLIENTS_ID_CLIENT_COL + "))";
	}

	if (nameTable == ORDERS_TABLE) {
		res += ORDERS_TABLE;
		res += " (" + (std::string)ORDERS_ID_ORDER_COL + " int NOT NULL AUTO_INCREMENT, ";
		res += (std::string)ORDERS_ID_CLIENT_COL + " int NOT NULL, ";
		res += (std::string)ORDERS_ID_BRANCH_COL + " int, ";
		res += (std::string)ORDERS_ADDRESS_COL + " varchar(255) NOT NULL, ";
		res += (std::string)ORDERS_CATEGORY_COL + " varchar(80) NOT NULL, ";
		res += (std::string)ORDERS_DESC_COL + " varchar(255) NOT NULL, ";
		res += (std::string)ORDERS_FILES_COL + " varchar(255) NOT NULL, ";
		res += (std::string)ORDERS_STATUS_COL + " varchar(80) NOT NULL, ";
		res += (std::string)ORDERS_COMMENT_COL + " varchar(255), PRIMARY KEY(";
		res += (std::string)ORDERS_ID_ORDER_COL + "), CONSTRAINT fk_client FOREIGN KEY(";
		res += (std::string)ORDERS_ID_CLIENT_COL + ") REFERENCES " + (std::string)CLIENTS_TABLE + " (";
		res += (std::string)CLIENTS_ID_CLIENT_COL + ") ON DELETE CASCADE ON UPDATE CASCADE)";
	}

	if (nameTable == MANAGERS_TABLE) {
		res += MANAGERS_TABLE;
		res += " (" + (std::string)MANAGERS_ID_MANAGER_COL + " int NOT NULL AUTO_INCREMENT, ";
		res += (std::string)MANAGERS_FULLNAME_COL + " varchar(45) NOT NULL, ";
		res += (std::string)MANAGERS_NUMTEL_COL + " varchar(12) NOT NULL, ";
		res += (std::string)MANAGERS_LOGIN_COL + " varchar(45) NOT NULL, ";
		res += (std::string)MANAGERS_PASSWORD_COL + " varchar(64) NOT NULL, ";
		res += (std::string)MANAGERS_ACCESS_COL + " varchar(45) NOT NULL, PRIMARY KEY(";
		res += (std::string)MANAGERS_ID_MANAGER_COL + "))";
	}

	if (nameTable == BRANCHES_TABLE) {
		res += BRANCHES_TABLE;
		res += " (" + (std::string)BRANCHES_ID_BRANCH_COL + " int NOT NULL AUTO_INCREMENT, ";
		res += (std::string)BRANCHES_DIRECTOR_COL + " varchar(45) NOT NULL, ";
		res += (std::string)BRANCHES_NUMTEL_COL + " varchar(12) NOT NULL, ";
		res += (std::string)BRANCHES_ADDRESS_COL + " varchar(255) NOT NULL, ";
		res += (std::string)BRANCHES_ORDERSPROG_COL + " int NOT NULL, PRIMARY KEY(";
		res += (std::string)BRANCHES_ID_BRANCH_COL + "))";
	}

	if (nameTable == HARDWARE_TABLE) {
		res += HARDWARE_TABLE;
		res += " (" + (std::string)HARDWARE_ID_HARDWARE_COL + " int NOT NULL AUTO_INCREMENT, ";
		res += (std::string)HARDWARE_CATEGORY_COL + " varchar(255) NOT NULL, ";
		res += (std::string)HARDWARE_DESC_COL + " varchar(255) NOT NULL, ";
		res += (std::string)HARDWARE_IDS_BRANCHES_COL + " varchar(255) NOT NULL, PRIMARY KEY(";
		res += (std::string)HARDWARE_ID_HARDWARE_COL + "))";
	}

	if (nameTable == "CreateAccount") {
		res = "INSERT INTO " + (std::string)MANAGERS_TABLE + "(";
		res += (std::string)MANAGERS_FULLNAME_COL + ", ";
		res += (std::string)MANAGERS_NUMTEL_COL + ", ";
		res += (std::string)MANAGERS_LOGIN_COL + ", ";
		res += (std::string)MANAGERS_PASSWORD_COL + ", ";
		res += (std::string)MANAGERS_ACCESS_COL + ") VALUES ('Страндартная учетная запись', '+79000000000', 'admin','1111','root')";
	}

	return res;
}


void Server::DisconnectClient(int _index)
{
	count--; //Уменьшаем количество текущих пользователей
	closesocket(_index); //Закрываем сокет
	Connections[_index] = 0; //Зануляем подключение для повторного использования
	std::cout << "Client disabled" << std::endl;
}
