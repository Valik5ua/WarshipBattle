#include "Connection.h"


void Connection::AsyncAutoConnect()
{
	HandleID = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&StartAsyncAutoConnect, (LPVOID)this, 0, NULL);
}

//void Connection::AsyncServerConnect()
//{
//	HandleID = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&StartAsyncServerConnect, (LPVOID)this, 0, NULL);
//}
//
//void Connection::StartAsyncServerConnect(Connection* inst)
//{
//	Connection* serverInst = (Connection*)inst;
//	serverInst->ServerConnectA();
//}
//
//void Connection::AsyncClientConnect()
//{
//	HandleID = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&StartAsyncClientConnect, (LPVOID)this, 0, NULL);
//
//}
//
//void Connection::StartAsyncClientConnect(Connection* inst)
//{
//	Connection* clientInst = (Connection*)inst;
//	clientInst->ClientConnectA();
//}

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

//Connection::Connection()
//{
//	HandleID = NULL;
//	Server = nullptr;
//	Client = nullptr;
//	IsConnected = false;
//	disconnected = false;
//	IsServer = false;
//	IsClient = false;
//	ConnectionError = false;
//	CancelConnecting = false;
//	strcpy_s(IP, "127.0.0.1");
//	LAST_ERROR = UDP::LastError::NONE;
//	CONNECTION_TYPE = UDP::ConnectionType::AUTO;
//}

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
	CONNECTION_TYPE = connectionType;
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

	//ExitThread(NULL);
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

	//ExitThread(NULL);
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
/////////////////////////////////////////////
}

void Connection::CheckConnection()
{
	int AttemptCounter = 0;
	
	if (IsServer)
	{
		while (true)
		{
			Server->ReceiveMSG(SERVER_CHECKING_ITER_NUM); //20 was good
			if (Server->Received)
			{
				UDP::MSG msg;
				Server->DecodeMsg(msg, Server->MSGReceived);
				if (msg.SENDER == SENDER_CLIENT && msg.TYPE == TYPE_CHECK)
				{
					Server->SendMSG(Server->FormMsg(SENDER_SERVER, TYPE_CHECK, FLAG_ONE, (char*)"check"));
					AttemptCounter = 0;
					IsConnected = true;
					std::cout << "SERVER CONNECTION CHECKED" << std::endl;
				}
			}
			Sleep(SERVER_CHECKING_SLEEP_TIME); // 10 was good
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

			Client->ReceiveMSG(CLIENT_CHECKING_ITER_NUM); // 30 was good
			if (Client->Received)
			{
				UDP::MSG msg;
				Client->DecodeMsg(msg, Client->MSGReceived);
				if (msg.SENDER == SENDER_SERVER && msg.TYPE == TYPE_CHECK)
				{
					AttemptCounter = 0;
					IsConnected = true;
					std::cout << "CLIENT CONNECTION CHECKED" << std::endl;
				}
			}
			Sleep(CLIENT_CHECKING_SLEEP_TIME); // 10 was good
			if (++AttemptCounter == MAX_CHEKING_ATTEMPTS)
			{
				IsConnected = false;
				break;
			}
		}

	}

	IsConnected = false;
	ExitThread(NULL);
}

void Connection::CheckConnection(int MaxAttempt)
{
	int AttemptCounter = 0;

	if (IsServer)
	{
		while (true)
		{
			Server->ReceiveMSG(SERVER_CHECKING_ITER_NUM); //20 was good
			if (Server->Received)
			{
				UDP::MSG msg;
				Server->DecodeMsg(msg, Server->MSGReceived);
				if (msg.SENDER == SENDER_CLIENT && msg.TYPE == TYPE_CHECK)
				{
					Server->SendMSG(Server->FormMsg(SENDER_SERVER, TYPE_CHECK, FLAG_ONE, (char*)"check"));
					
					IsConnected = true;
					std::cout << "SERVER CONNECTION CHECKED" << std::endl;
					break;
				}
			}
			//Sleep(SERVER_CHECKING_SLEEP_TIME); // 10 was good
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

			Client->ReceiveMSG(CLIENT_CHECKING_ITER_NUM); // 30 was good
			if (Client->Received)
			{
				UDP::MSG msg;
				Client->DecodeMsg(msg, Client->MSGReceived);
				if (msg.SENDER == SENDER_SERVER && msg.TYPE == TYPE_CHECK)
				{
					IsConnected = true;
					std::cout << "CLIENT CONNECTION CHECKED" << std::endl;
					break;
				}
			}
			//Sleep(CLIENT_CHECKING_SLEEP_TIME); // 10 was good
			if (++AttemptCounter == MaxAttempt)
			{
				IsConnected = false;
				break;
			}
		}

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

	/*switch (err)
	{
	case 0: LAST_ERROR = UDP::LastError::NONE;
		break;
	case 1: LAST_ERROR = UDP::LastError::WSA_INIT_ERROR;
		break;
	case 2: LAST_ERROR = UDP::LastError::SOCKET_INIT_ERROR;
		break;
	case 3: LAST_ERROR = UDP::LastError::BROADCAST_INIT_ERROR;
		break;
	case 4: LAST_ERROR = UDP::LastError::BIND_ERROR;
		break;
	case 5: LAST_ERROR = UDP::LastError::SEND_ERROR;
		break;
	default: LAST_ERROR = UDP::LastError::NONE;
		break;
	}*/
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
	///////CHECK CONNECTION
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

	return false;
}

bool Connection::Connected()
{
	return IsConnected;
}

