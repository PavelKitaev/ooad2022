#include <iostream>
#include <clocale> 

#include "Server.h"

int main()
{
	setlocale(LC_ALL, "");
	Server *s = new Server("192.168.1.50");
	s->Start();

	std::string str;
  
	while (!(str == "exit")) {
		std::cin >> str;
	}

	s->Stop();

	delete s;

	return 0;
}