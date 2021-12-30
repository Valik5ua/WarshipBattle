#include "Connection.h"

void Connection::AsyncAutoConnect()
{
	HandleID = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&StartAsyncAutoConnect, (LPVOID)this, 0, NULL);
}

void Connection::AsyncManualConnect()
{
	HandleID = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&StartAsyncManualConnect, (LPVOID)this, 0, NULL);
}

void Connection::AsyncDisconnect()
{
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&StartAsyncDisconnect, (LPVOID)this, 0, NULL);
}

void Connection::AsyncCheckConnection()
{
	HandleID = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&StartAsyncCheckConnection, (LPVOID)this, 0, NULL);
}

void Connection::StartAsyncCheckConnection(Connection* inst)
{
	Connection* Inst = (Connection*)inst;
	Inst->CheckConnection();
}

void Connection::StartAsyncAutoConnect(Connection* inst)
{
	Connection* Inst = (Connection*)inst;
	Inst->AutoConnect();
}

void Connection::StartAsyncDisconnect(Connection* inst)
{
	Connection* Inst = (Connection*)inst;
	Inst->Disconnect();
}

Connection::Connection(UDP::ConnectionType connectionType)
{
	HandleID = NULL;
	IsConnected = false;
	ConnectionError = false;
	CancelConnecting = false;
	IsServer = false;
	IsClient = false;
	disconnected = false;
	Client = nullptr;
	Server = nullptr;
	LAST_ERROR = UDP::LastError::NONE;
	strcpy_s(IP,"127.0.0.1");
	strcpy_s(MyIP,"127.0.0.1");
	CONNECTION_TYPE = connectionType;
	IsIPSet = false;
}

void Connection::CleanUP()
{
	if (IsServer)
	{
		Server->CleanUp();
	}

	if (IsClient)
	{
		Client->CleanUp();
	}

	//HandleID = NULL;
	Server = nullptr;
	Client = nullptr;
	IsConnected = false;
	IsServer = false;
	IsClient = false;
	IsIPSet = false;
	ConnectionError = false;
	CancelConnecting = false;
	LAST_ERROR = UDP::LastError::NONE;
	disconnected = true;
}

void Connection::ServerConnect()
{
	while (true)
	{
		/// Creating Temp object
		UDPServer* server = new UDPServer(UDP::ConnectionType::SERVER);
		/// Checking if object created without errors
		if (server->GetLastError(false) != UDP::LastError::NONE)
		{
			SetLastError(server->GetLastError(true));
			delete server;
			server = nullptr;
			break;
		}

		if (!IsIPSet)
		{
			SetMyIP();
			IsIPSet = true;
		}
		/// Receiving messages X times. Receiving breaks if received and Received == true
		server->ReceiveMSG(MSG_RECEIVE_ITER_NUM);

		/// Checking if message received
		/// if received asigning memory of temp object to empty UDPServer pointer
		/// if not, cleaning up and releasing temp object memory  
		if (server->Received)
		{
			UDP::MSG msg;
			server->DecodeMsg(msg, server->MSGReceived);
			if (msg.SENDER == SENDER_CLIENT && msg.TYPE == TYPE_CONNECTION && msg.FLAG == FLAG_ONE)
			{
				char* tempMsg = nullptr;
				tempMsg = server->FormMsg(SENDER_SERVER, TYPE_CONNECTION, FLAG_TWO, (char*)"code");
				server->SendMSG(tempMsg);

				server->ReceiveMSG(MSG_RECEIVE_ITER_NUM);

				if (server->Received)
				{
					server->DecodeMsg(msg, server->MSGReceived);
					if (msg.SENDER == SENDER_CLIENT && msg.TYPE == TYPE_CONNECTION && msg.FLAG == FLAG_THREE)
					{
						IsServer = true;
						Server = server;
						server = nullptr;
						break;
					}
					else
					{
						server->CleanUp();
						delete server;
						server = nullptr;
					}
				}
			}
			else
			{
				server->CleanUp();
				delete server;
				server = nullptr;
			}
		}
		else
		{
			server->CleanUp();
			delete server;
			server = nullptr;
		}

		if (CancelConnecting)
		{
			break;
		}
	}
}

void Connection::ServerConnect(int iter)
{
	for (int i = 0; i < iter; i++)
	{
		/// Creating Temp object
		UDPServer* server = new UDPServer;
		/// Checking if object created without errors
		if (server->GetLastError(false) != UDP::LastError::NONE)
		{
			SetLastError(server->GetLastError(true));
			delete server;
			server = nullptr;
			break;
		}
		if (!IsIPSet)
		{
			SetMyIP();
			IsIPSet = true;
		}
		/// Receiving messages X times. Receiving breaks if received and Received == true
		server->ReceiveMSG(MSG_RECEIVE_ITER_NUM);

		/// Checking if message received
		/// if received asigning memory of temp object to empty UDPServer pointer
		/// if not, cleaning up and releasing temp object memory  
		if (server->Received)
		{
			UDP::MSG msg;
			server->DecodeMsg(msg, server->MSGReceived);
			if (msg.SENDER == SENDER_CLIENT && msg.TYPE == TYPE_CONNECTION && msg.FLAG == FLAG_ONE)
			{
				char* tempMsg = nullptr;
				tempMsg = server->FormMsg(SENDER_SERVER, TYPE_CONNECTION, FLAG_TWO, (char*)"code");
				server->SendMSG(tempMsg);

				server->ReceiveMSG(MSG_RECEIVE_ITER_NUM);

				if (server->Received)
				{
					server->DecodeMsg(msg, server->MSGReceived);
					if (msg.SENDER == SENDER_CLIENT && msg.TYPE == TYPE_CONNECTION && msg.FLAG == FLAG_THREE)
					{
						IsServer = true;
						Server = server;
						server = nullptr;
						break;
					}
					else
					{
						server->CleanUp();
						delete server;
						server = nullptr;
					}
				}
			}
			else
			{
				server->CleanUp();
				delete server;
				server = nullptr;
			}
		}
		else
		{
			server->CleanUp();
			delete server;
			server = nullptr;
		}
	}
}

void Connection::ClientConnect()
{
	while (true)
	{
		/// Creating Temp object
		UDPClient* client = new UDPClient(UDP::ConnectionType::CLIENT, IP);
		/// Checking if object created without errors
		if (client->GetLastError(false) != UDP::LastError::NONE)
		{
			SetLastError(client->GetLastError(true));
			delete client;
			client = nullptr;
			break;
		}
		if (!IsIPSet)
		{
			SetMyIP();
			IsIPSet = true;
		}
		char* tempMsg = nullptr;
		tempMsg = client->FormMsg(SENDER_CLIENT, TYPE_CONNECTION, FLAG_ONE, (char*)"code");
		client->SendMSG(tempMsg);

		/// Receiving messages X times. Receiving breaks if received and Received == true
		client->ReceiveMSG(MSG_RECEIVE_ITER_NUM);

		/// Checking if message received
		/// if received asigning memory of temp object to empty UDPServer pointer
		/// if not, cleaning up and releasing temp object memory  
		if (client->Received)
		{
			UDP::MSG msg;
			client->DecodeMsg(msg, client->MSGReceived);
			if (msg.SENDER == SENDER_SERVER && msg.TYPE == TYPE_CONNECTION && msg.FLAG == FLAG_TWO)
			{
				char* tempMsg = nullptr;
				tempMsg = client->FormMsg(SENDER_CLIENT, TYPE_CONNECTION, FLAG_THREE, (char*)"code");
				client->SendMSG(tempMsg);

				IsClient = true;
				Client = client;
				client = nullptr;
				break;
			}
			else
			{
				client->CleanUp();
				delete client;
				client = nullptr;
			}
		}
		else
		{
			client->CleanUp();
			delete client;
			client = nullptr;
		}

		if (CancelConnecting)
		{
			break;
		}
	}
}

void Connection::ClientConnect(int iter)
{
	for (int i = 0; i < iter; i++)
	{
		/// Creating Temp object
		UDPClient* client = new UDPClient;
		/// Checking if object created without errors
		if (client->GetLastError(false) != UDP::LastError::NONE)
		{
			SetLastError(client->GetLastError(true));
			delete client;
			client = nullptr;
			break;
		}
		if (!IsIPSet)
		{
			SetMyIP();
			IsIPSet = true;
		}
		char* tempMsg = nullptr;
		tempMsg = client->FormMsg(SENDER_CLIENT, TYPE_CONNECTION, FLAG_ONE, (char*)"code");
		client->SendMSG(tempMsg);

		/// Receiving messages X times. Receiving breaks if received and Received == true
		client->ReceiveMSG(MSG_RECEIVE_ITER_NUM);

		/// Checking if message received
		/// if received asigning memory of temp object to empty UDPServer pointer
		/// if not, cleaning up and releasing temp object memory  
		if (client->Received)
		{
			UDP::MSG msg;
			client->DecodeMsg(msg, client->MSGReceived);
			if (msg.SENDER == SENDER_SERVER && msg.TYPE == TYPE_CONNECTION && msg.FLAG == FLAG_TWO)
			{
				char* tempMsg = nullptr;
				tempMsg = client->FormMsg(SENDER_CLIENT, TYPE_CONNECTION, FLAG_THREE, (char*)"code");
				client->SendMSG(tempMsg);

				IsClient = true;
				Client = client;
				client = nullptr;
				break;
			}
			else
			{
				client->CleanUp();
				delete client;
				client = nullptr;
			}
		}
		else
		{
			client->CleanUp();
			delete client;
			client = nullptr;
		}
	}
}

void Connection::AutoConnect()
{
	while (true)
	{
		ServerConnect(AUTO_CONNECT_SERVER_ATTEMPTS);
		if (IsServer)
		{
			if (CancelConnecting)
			{
				CleanUP();
				break;
			}

			this->CheckConnection(AUTO_CONNECT_CHECKING_ITER_NUM);
			if (IsConnected)
			{
				break;
			}
			else
			{
				CleanUP();
			}
		}
		else if (CancelConnecting)
		{
			break;
		}
		
		ClientConnect(AUTO_CONNECT_CLIENT_ATTEMPTS);
		if (IsClient)
		{
			if (CancelConnecting)
			{
				CleanUP();
				break;
			}

			this->CheckConnection(AUTO_CONNECT_CHECKING_ITER_NUM);
			if (IsConnected)
			{
				break;
			}
			else
			{
				CleanUP();
			}
		}
		else if (CancelConnecting)
		{
			break;
		}
	}

	ExitThread(NULL);
}

void Connection::ManualConnect()
{
	if (CONNECTION_TYPE == UDP::ConnectionType::AUTO)
	{
		std::cout << "WRONG CONNECTION TYPE: USE SERVER OR CLIENT ONLY WITH \"ManualConnect\" FUNCTION" << std::endl;
	}

	switch (CONNECTION_TYPE)
	{
	case UDP::ConnectionType::SERVER:
	{
		while (true)
		{
			this->ServerConnect();
			if (IsServer)
			{
				if (CancelConnecting)
				{
					CleanUP();
					break;
				}
				else
				{
					this->CheckConnection(AUTO_CONNECT_CHECKING_ITER_NUM);
				}

				if (IsConnected)
				{
					break;
				}
				else
				{
					CleanUP();
				}
			}
			else if (CancelConnecting)
			{
				break;
			}
		}
	}
	break;
	case UDP::ConnectionType::CLIENT:
	{
		while (true)
		{
			this->ClientConnect();
			if (IsClient)
			{
				if (CancelConnecting)
				{
					CleanUP();
					break;
				}
				else
				{
					this->CheckConnection(AUTO_CONNECT_CHECKING_ITER_NUM);
				}

				if (IsConnected)
				{
					break;
				}
				else
				{
					CleanUP();
				}
			}
			else if (CancelConnecting)
			{
				break;
			}
		}
	}
	break;
	}
	
	ExitThread(NULL);
}

void Connection::CheckConnection()
{
	int AttemptCounter = 0;
	if (IsServer || IsClient)
	{
		if (IsServer)
		{
			while (true)
			{
				Server->ReceiveMSG(SERVER_CHECKING_ITER_NUM);
				if (Server->Received)
				{
					UDP::MSG msg;
					Server->DecodeMsg(msg, Server->MSGReceived);
					if (msg.SENDER == SENDER_CLIENT && msg.TYPE == TYPE_CHECK)
					{
						Server->SendMSG(Server->FormMsg(SENDER_SERVER, TYPE_CHECK, FLAG_ONE, (char*)"check"));
						AttemptCounter = 0;

						IsConnected = true;
					}
				}
				Sleep(SERVER_CHECKING_SLEEP_TIME);
				if (++AttemptCounter == MAX_CHEKING_ATTEMPTS)
				{
					IsConnected = false;
					break;
				}
			}
		}

		if (IsClient)
		{
			while (true)
			{
				Client->SendMSG(Client->FormMsg(SENDER_CLIENT, TYPE_CHECK, FLAG_ONE, (char*)"check"));

				Client->ReceiveMSG(CLIENT_CHECKING_ITER_NUM);
				if (Client->Received)
				{
					UDP::MSG msg;
					Client->DecodeMsg(msg, Client->MSGReceived);
					if (msg.SENDER == SENDER_SERVER && msg.TYPE == TYPE_CHECK)
					{
						AttemptCounter = 0;
						IsConnected = true;
					}
				}
				Sleep(CLIENT_CHECKING_SLEEP_TIME);
				if (++AttemptCounter == MAX_CHEKING_ATTEMPTS)
				{
					IsConnected = false;
					break;
				}
			}

		}
	}
	else
	{
		std::cout << "YOU ARE NOT CONNECTED. USE AUTO/MANUAL CONNECT BEFORE SEND OR RECEIVE." << std::endl;
	}

	ExitThread(NULL);
}

void Connection::CheckConnection(int MaxAttempt)
{
	int AttemptCounter = 0;
	
	if (IsServer || IsClient)
	{
		if (IsServer)
		{
			while (true)
			{
				Server->ReceiveMSG(SERVER_CHECKING_ITER_NUM);
				if (Server->Received)
				{
					UDP::MSG msg;
					Server->DecodeMsg(msg, Server->MSGReceived);
					if (msg.SENDER == SENDER_CLIENT && msg.TYPE == TYPE_CHECK)
					{
						Server->SendMSG(Server->FormMsg(SENDER_SERVER, TYPE_CHECK, FLAG_ONE, (char*)"check"));
						IsConnected = true;
						break;
					}
				}

				if (++AttemptCounter == MaxAttempt)
				{
					IsConnected = false;
					break;
				}
			}
		}

		if (IsClient)
		{
			while (true)
			{
				Client->SendMSG(Client->FormMsg(SENDER_CLIENT, TYPE_CHECK, FLAG_ONE, (char*)"check"));

				Client->ReceiveMSG(CLIENT_CHECKING_ITER_NUM);
				if (Client->Received)
				{
					UDP::MSG msg;
					Client->DecodeMsg(msg, Client->MSGReceived);
					if (msg.SENDER == SENDER_SERVER && msg.TYPE == TYPE_CHECK)
					{
						IsConnected = true;
						break;
					}
				}

				if (++AttemptCounter == MaxAttempt)
				{
					IsConnected = false;
					break;
				}
			}
		}
	}
	else
	{
		std::cout << "YOU ARE NOT CONNECTED. USE AUTO/MANUAL CONNECT BEFORE SEND OR RECEIVE." << std::endl;
	}
}

void Connection::SetConnectingIP(char* ip)
{
	for (int i = 0; i < IP_LENGTH; i++)
	{
		IP[i] = '\0';
	}

	strcpy_s(IP, ip);
}

char* Connection::GetMyIP()
{
	return MyIP;
}

void Connection::Disconnect()
{
	if (!IsConnected)
	{
		CancelConnecting = true;
		WaitForSingleObject(HandleID, INFINITE);
		CloseHandle(HandleID);
	}

	CleanUP();
	ExitThread(NULL);
}

bool Connection::Disconnected()
{
	return disconnected;
}

void Connection::SetLastError(UDP::LastError err)
{
	ConnectionError = true;
	LAST_ERROR = err;
}

void Connection::SetMyIP()
{
	for (int i = 0; i < IP_LENGTH; i++)
	{
		MyIP[i] = '\0';
	}

	struct hostent* hn = gethostbyname(NULL);
	char* ip = inet_ntoa(*((struct in_addr*)hn->h_addr_list[0]));
	strcpy_s(MyIP, ip);
}

void Connection::StartAsyncManualConnect(Connection* inst)
{
	Connection* Inst = (Connection*)inst;
	Inst->ManualConnect();
}

UDP::LastError Connection::GetLastError(bool CleanLastError)
{
	UDP::LastError temp_lastError = LAST_ERROR;
	if (CleanLastError)
	{
		ConnectionError = false;
		LAST_ERROR = UDP::LastError::NONE;
	}

	return temp_lastError;
}

bool Connection::SendMSG(int TYPE, int FLAG, char* msg)
{
	if (IsConnected)
	{
		if (IsServer)
		{
			Server->SendMSG(Server->FormMsg(SENDER_SERVER, TYPE, FLAG, msg));
			if (Server->GetLastError(false) != UDP::LastError::NONE)
			{
				SetLastError(Server->GetLastError(true));
				return false;
			}
			return true;
		}

		if (IsClient)
		{
			Client->SendMSG(Client->FormMsg(SENDER_CLIENT, TYPE, FLAG, msg));
			if (Client->GetLastError(false) != UDP::LastError::NONE)
			{
				SetLastError(Client->GetLastError(true));
				return false;
			}
			return true;
		}
	}
	else
	{
		std::cout << "YOU ARE NOT CONNECTED. USE AUTO/MANUAL CONNECT BEFORE SEND OR RECEIVE." << std::endl;
		return false;
	}

	return false;
}

bool Connection::ReceiveMSG(UDP::MSG& msg, int iterOfReceive)
{
	if (IsConnected)
	{
		if (IsServer)
		{
			Server->ReceiveMSG(iterOfReceive);
			if (Server->Received)
			{
				Server->DecodeMsg(msg, Server->MSGReceived);
				return true;
			}
		}

		if (IsClient)
		{
			Client->ReceiveMSG(iterOfReceive);
			if (Client->Received)
			{
				Client->DecodeMsg(msg, Client->MSGReceived);
				return true;
			}
		}
	}
	else
	{
		std::cout << "YOU ARE NOT CONNECTED. USE AUTO/MANUAL CONNECT BEFORE SEND OR RECEIVE." << std::endl;
		return false;
	}

	return false;
}

bool Connection::Connected()
{
	return IsConnected;
}

bool Connection::Connected(UDP::ConnectionType connectionType)
{
	if (IsConnected)
	{
		if (connectionType == UDP::ConnectionType::SERVER)
		{
			if (IsServer) return true;
			else return false;
		}

		if (connectionType == UDP::ConnectionType::CLIENT)
		{
			if (IsClient) return true;
			else return false;
		}
	}
	else
	{
		return false;
	}
	return false;
}
