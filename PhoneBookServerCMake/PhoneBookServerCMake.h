#pragma once
#include <iostream>
#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <cstring>
#endif
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <thread>




struct connection
{
	int ClientConnestion;
};


class SERVER
{
public:
	SERVER(std::string serverIp, int serverPort)
	{
		int erStat;
#ifdef _WIN32

		WSADATA wsData;
		erStat = WSAStartup(MAKEWORD(2, 2), &wsData);
		if (erStat != 0) {
			std::cout << "Error WinSock version initializaion #";
			std::cout << WSAGetLastError();
		}
		else
			std::cout << "WinSock initialization is OK" << std::endl;
#endif




		ServSock = socket(AF_INET, SOCK_STREAM, 0);
#ifdef _WIN32
		if (ServSock == INVALID_SOCKET) {
			std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
			closesocket(ServSock);
#else
		if (ServSock == -1) {
			std::cout << "Error initialization socket # " << std::endl;
			close(ServSock);
#endif
#ifdef _WIN32
			WSACleanup();
#endif
		}
		else
			std::cout << "Server socket initialization is OK" << std::endl;

		sockaddr_in servInfo;


		in_addr ip_to_num;
		erStat = inet_pton(AF_INET, serverIp.c_str(), &ip_to_num);

		if (erStat != 1) {

			std::cout << "Error in IP translation to special numeric format" << std::endl;

		}

#ifdef _WIN32
		ZeroMemory(&servInfo, sizeof(servInfo));
#else
		memset(&servInfo, 0, sizeof(servInfo));
#endif


		servInfo.sin_family = AF_INET;
		servInfo.sin_addr = ip_to_num;
		servInfo.sin_port = htons(serverPort);

		erStat = bind(ServSock, reinterpret_cast<sockaddr*>(&servInfo), sizeof(servInfo));

#ifdef _WIN32
		if (erStat != 0) {
			std::cout << "Error Socket binding to server info. Error # " << WSAGetLastError() << std::endl;
			closesocket(ServSock);
#else
		if (erStat != 0) {
			std::cout << "Error Socket binding to server info. Error # " << std::endl;
			close(ServSock);
#endif
#ifdef _WIN32
			WSACleanup();
#endif
		}
		else
			std::cout << "Binding socket to Server info is OK" << std::endl;

		erStat = listen(ServSock, SOMAXCONN);


#ifdef _WIN32
		if (erStat != 0) {
			std::cout << "Can't start to listen to. Error # " << WSAGetLastError() << std::endl;
			closesocket(ServSock);
#else
		if (erStat != 0) {
			std::cout << "Can't start to listen to. Error # " << std::endl;
			close(ServSock);
#endif
#ifdef _WIN32
			WSACleanup();
#endif
		}
		else {
			std::cout << "Listening..." << std::endl;
		}


		}





	std::vector<std::string> parseLine(std::string line)
	{
		std::vector<std::string> result;

		std::string word;
		for (int i = 0; i < line.size(); i++)
		{

			if (line[i] == ';' && result.size() <= 5)
			{
				if (word == "")
				{
					word = "-1";
				}
				result.push_back(word);
				word.clear();
			}
			else
			{
				word += line[i];
			}



		}
		result.push_back(word);

		return result;
	}





	int getLastLineID()
	{

		std::ifstream in("bd.bin", std::ios::binary);
		std::string line;
		int lastRowId = 0;

		if (in.is_open())
		{
			in.seekg(-3, std::ios::end);
			int fileSize = in.tellg();
			char c;
			in.get(c);
			for (int i = -3; i >= -3 - fileSize; i--)
			{
				in.seekg(i, std::ios::end);
				in.get(c);
				if (c == '\n')
				{
					std::getline(in, line);
					in.close();
					return(stoi(parseLine(line)[0]));
				}
			}
			in.seekg(0);
			std::getline(in, line);
			in.close();
			std::cout << line << std::endl;
			if (line.size() == 0)
			{
				return(-1);

			}
			else
			{
				return(stoi(parseLine(line)[0]));
			}

		}
		else
		{
			return -1;
		}





	}

	int addDBLine(std::vector<std::string> line)
	{
		int lineCount = 0;
		std::ofstream out;
		std::string firstLine;
		int rowId = 1;

		rowId = getLastLineID() + 1;
		out.open("bd.bin", std::ios::app);
		if (out.is_open())
		{
			out << std::to_string(rowId) << ";";
			for (int i = 0; i < line.size() - 1; i++)
			{
				out << line[i] << ";";
			}
			out << line[line.size() - 1];
			out << std::endl;
		}
		out.close();
		return 0;
	}


	std::string findLinesByArgument(std::string argument, int type)
	{
		std::ifstream in("bd.bin", std::ios::binary);
		std::string line;
		std::string result;
		bool success = false;
		if (in.is_open())
		{
			while (std::getline(in, line))
			{
				if (type == 1)
				{
					if (parseLine(line)[1].find(argument) != std::string::npos)
					{
						result += line + "\n";
						success = true;
					}
				}
				if (type == 2)
				{
					if (parseLine(line)[2].find(argument) != std::string::npos)
					{
						result += line + "\n";
						success = true;
					}
				}
				if (type == 3)
				{
					if (parseLine(line)[3].find(argument) != std::string::npos)
					{
						result += line + "\n";
						success = true;
					}
				}
				if (type == 4)
				{
					if (parseLine(line)[4].find(argument) != std::string::npos)
					{
						result += line + "\n";
						success = true;
					}
				}
				if (type == 5)
				{
					if (parseLine(line)[5].find(argument) != std::string::npos)
					{
						result += line + "\n";
						success = true;
					}
				}


			}


		}
		in.close();
		if (success == true)
		{
			return result;
		}
		else
		{
			return "Error";
		}

	}


	std::string getLineByNumber(int lineNumber)
	{
		std::ifstream in("bd.bin", std::ios::binary);
		std::string line;
		std::vector<std::string> result;
		int i = 0;
		if (in.is_open())
		{
			while (std::getline(in, line))
			{
				if (i == lineNumber)
				{
					in.close();
					return(line);
				}
				i++;

			}
		}
		in.close();
		return("Error");


	}

	std::string getLineById(int lineId)
	{
		std::ifstream in("bd.bin", std::ios::binary);
		std::string line;
		std::vector<std::string> result;
		int lastRowId = 0;

		if (in.is_open())
		{
			while (std::getline(in, line))
			{
				result = parseLine(line);
				if (stoi(result[0]) == lineId)
				{
					in.close();
					return line;
				}

			}
		}
		return("Error");



	}


	int deleteLine(std::string lineToRemove)
	{
		std::ifstream in("bd.bin", std::ios::binary);



		std::ofstream outFile("bdtemp.bin", std::ios::binary);
		bool lineRemoved = false;
		if (in.is_open())
		{
			if (outFile.is_open())
			{
				std::string line;
				while (std::getline(in, line)) {
					if (line != lineToRemove) {
						outFile << line << std::endl;
					}
					else {
						lineRemoved = true;
					}
				}


			}

		}
		in.close();
		outFile.close();
		if (lineRemoved == true)
		{
			std::remove("bd.bin");
		}
		if (lineRemoved && std::rename("bdtemp.bin", "bd.bin") == 0) {
			std::cout << "Строка успешно удалена из файла." << std::endl;
			return 0;
		}
		else {
			std::cerr << "Ошибка удаления строки из файла." << std::endl;
			return -1;
		}
	}







	int sendMessage(int ClientConn, std::string message)
	{
		short packet_size = 0;

		packet_size = send(ClientConn, message.data(), message.size(), 0);
		std::cout << "Server message : " << message << std::endl;
#ifdef _WIN32
		if (packet_size == SOCKET_ERROR) {
			return 1;
		}
#else
		if (packet_size == SO_ERROR) {
			return 1;
		}
#endif

		return 0;

	}


	std::string handleMessage(std::string message)
	{
		if (message.find("ADD_LINE ") == 0)
		{
			std::vector<std::string> words;
			std::string word;

			int bufFirstPos = message.find('"') + 1;
			int bufSecondPos = message.find('"', bufFirstPos + 1);
			for (int i = 0; i < 4; i++)
			{

				while (bufFirstPos < bufSecondPos)
				{
					word += message[bufFirstPos];
					bufFirstPos++;

				}
				if (word == " ")
				{
					word.clear();
				}
				words.push_back(word);
				word.clear();
				bufFirstPos = message.find('"', bufFirstPos + 1) + 1;
				bufSecondPos = message.find('"', bufFirstPos + 1);



			}
			for (int i = bufFirstPos; i < message.size() - 1; i++)
			{
				word += message[i];
			}
			words.push_back(word);
			word.clear();

			if (addDBLine(words) == 0)
			{
				return("Success!");
			}
			else
			{
				return("Line write error");
			}

		}
		if (message.find("DELETE_LINE ") == 0)
		{
			std::string argument;
			int lineId;
			argument = std::string(message.begin() + 12, message.end());
			try
			{
				lineId = stoi(argument);
			}
			catch (const std::invalid_argument& e)
			{
				return("Wrong number!");
			}
			std::string result = getLineById(lineId);

			if (deleteLine(result) == 0)
			{
				return("Success!");
			}
			else
			{
				return("Error!");
			}

		}
		if (message.find("SEARCH_BY ") == 0)
		{
			if (message.find("FIRST_NAME ") == 10)
			{
				std::string argument;
				int lineId;
				argument = std::string(message.begin() + 21, message.end());
				return(findLinesByArgument(argument, 1));

			}
			if (message.find("SECOND_NAME ") == 10)
			{
				std::string argument;
				int lineId;
				argument = std::string(message.begin() + 22, message.end());
				return(findLinesByArgument(argument, 2));
			}
			if (message.find("LAST_NAME ") == 10)
			{
				std::string argument;
				int lineId;
				argument = std::string(message.begin() + 20, message.end());
				return(findLinesByArgument(argument, 3));
			}
			if (message.find("PHONE_NUMBER ") == 10)
			{
				std::string argument;
				int lineId;
				argument = std::string(message.begin() + 23, message.end());
				return(findLinesByArgument(argument, 4));
			}
			if (message.find("COMMENT ") == 10)
			{
				std::string argument;
				int lineId;
				argument = std::string(message.begin() + 19, message.end());
				return(findLinesByArgument(argument, 5));
			}
		}
		if (message.find("VIEW_LINE ") == 0)
		{
			std::string argument;
			int lineId;
			argument = std::string(message.begin() + 10, message.end());
			try
			{
				lineId = stoi(argument);
			}
			catch (const std::invalid_argument& e)
			{
				return("Wrong number!");
			}

			std::string result = getLineById(lineId);

			return(result);


		}
		if (message.find("GET_LINE ") == 0)
		{
			std::string argument;
			int lineNumber;
			argument = std::string(message.begin() + 9, message.end());
			try
			{
				lineNumber = stoi(argument);
			}
			catch (const std::invalid_argument& e)
			{
				return("Wrong number!");
			}

			std::string result = getLineByNumber(lineNumber);


			return(result);

		}
		if (message.find("GET_LASTLINE_ID") == 0)
		{
			return(std::to_string(getLastLineID()));

		}
		return("Error occured!");


	}







	void handleConnection(int connId, std::vector<char> message)
	{
		message.erase(std::remove(message.begin(), message.end(), '\0'), message.end());

		std::string serverAnsver;
		std::string clientMessage;
		clientMessage = std::string(message.begin(), message.end());
		std::cout << "Client message : " << clientMessage << std::endl;
		serverAnsver = handleMessage(clientMessage);

		int code = sendMessage(connections[connId].ClientConnestion, serverAnsver);

	}

	int setNonBlocking(int sockfd) {
#ifdef _WIN32
		unsigned long mode = 1;
		return ioctlsocket(sockfd, FIONBIO, &mode);
#else
		int flags = fcntl(sockfd, F_GETFL, 0);
		if (flags == -1) {
			return -1;
		}
		return fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
#endif
	}

	void handleClientConnections()
	{

		while (true)
		{
			std::vector<char> message(100);
			for (int i = 0; i < connections.size(); i++)
			{
				int messageCode = recv(connections[i].ClientConnestion, message.data(), message.size(), 0);
				if (messageCode == -1)
				{
					continue;
				}
				else
				{
					if (messageCode != 0)
					{
						threads.emplace_back(&SERVER::handleConnection, this, i, message);

					}

				}


			}
		}


	}



	void startServer()
	{
		std::thread th(&SERVER::handleClientConnections, this);

		while (true)
		{

			sockaddr_in clientInfo;

#ifdef _WIN32
			ZeroMemory(&clientInfo, sizeof(clientInfo));
#else
			memset(&clientInfo, 0, sizeof(clientInfo));
#endif


#ifdef _WIN32	
			int clientInfo_size = sizeof(clientInfo);
#else
			unsigned int clientInfo_size = sizeof(clientInfo);
#endif





			int ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfo_size);


#ifdef _WIN32
			if (ClientConn == INVALID_SOCKET)
			{
				std::cout << "Client detected, but can't connect to a client. Error # " << WSAGetLastError() << std::endl;
				closesocket(ServSock);
				closesocket(ClientConn);
				WSACleanup();
			}
#else
			if (ClientConn == -1)
			{
				if (errno == EAGAIN || errno == EWOULDBLOCK)
				{
					continue;
				}
				else
				{
					std::cout << "Client detected, but can't connect to a client. Error # " << std::endl;
					close(ServSock);
					close(ClientConn);
				}

			}
#endif

			else
			{
#ifdef _WIN32
				if (setNonBlocking(ClientConn) == SOCKET_ERROR) {
					closesocket(ServSock);
					closesocket(ClientConn);
					WSACleanup();
				}
#else
				if (setNonBlocking(ClientConn) == -1) {
					close(ServSock);
					close(ClientConn);
				}
#endif

				int code = sendMessage(ClientConn, "Welcome!Connection established!");

				connection ClientSonnection;
				ClientSonnection.ClientConnestion = ClientConn;
				connections.push_back(ClientSonnection);

			}



		}

	}





private:
	std::vector<std::thread> threads;
	std::vector<connection> connections;
	int ServSock;

		};


