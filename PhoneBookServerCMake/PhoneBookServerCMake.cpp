// PhoneBookServerCMake.cpp: определяет точку входа для приложения.
//

#include "PhoneBookServerCMake.h"





int main()
{


	SERVER MyServer("127.0.0.1", 1234);

	MyServer.startServer();



}
