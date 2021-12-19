#include "Connection.h"
/// <summary>
/// Function creates UDP connection automaticaly. Connection object must be created with 
/// UDP::ConnectionType::AUTO parameter befor calling this function. GetLastError() function
/// has to be called after creating Connection object to check if error was set, if 
/// object was created and WSA was initialized without errors GetLastError() function
/// returns UDP::LastError::NONE.
/// </summary>
void Connection::AsyncAutoConnect()
{
	HandleID = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&StartAsyncAutoConnect, (LPVOID)this, 0, NULL);
}
/// <summary>
/// Function creates UDP connection automaticaly. Connection object must be created with 
/// UDP::ConnectionType::SERVER or UDP::ConnectionType::SERVER 
/// parameter befor calling this function. GetLastError() function
/// has to be called after creating Connection object to check if error was set, if 
/// object was created and WSA was initialized without errors GetLastError() function
/// returns UDP::LastError::NONE.
/// </summary>
void Connection::AsyncManualConnect()
{
	HandleID = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&StartAsyncManualConnect, (LPVOID)this, 0, NULL);
}
/// <summary>
/// Function ASYNC disconnecting UDP connection if it's connected or not.
/// Connection object must be created befor calling this function.
/// "Disconnected" is TRUE when UDP connection closed.
/// </summary>
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
/// <summary>
/// Constructor with ConnectionType
/// </summary>
/// <param name="connectionType">Must pass UDP::ConnectionType.
/// Can be UDP::ConnectionType::AUTO, UDP::ConnectionType::SERVER or UDP::ConnectionType::CLIENT</param>
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
/// <summary>
/// Closing WSA UDP connection and setting default object parameters
/// </summary>
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
/// <summary>
/// Function for achieving UDP connection as server.
/// Stops working if connection achieved or "CancelConnecting" is TRUE.
/// </summary>
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
}
/// <summary>
/// Function for achieving UDP connection as server.
/// Stops working if connection achieved or "iter" is 0.
/// </summary>
/// <param name="iter">Counter of connection attempts</param>
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
/// <summary>
/// Function for achieving UDP connection as client.
/// Stops working if connection achieved or "CancelConnecting" is TRUE.
/// </summary>
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
}
/// <summary>
/// Function for achieving UDP connection as client.
/// Stops working if connection achieved or "iter" is 0.
/// </summary>
/// <param name="iter">Counter of connection attempts</param>
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
/// <summary>
/// Function for achieving UDP connection as server or client.
/// Stops working if connection achieved or "CancelConnecting" is TRUE.
/// </summary>
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
/// <summary>
/// Function for achieving UDP connection as server or client dependes on UDP::ConnectionType.
/// Stops working if connection achieved or "CancelConnecting" is TRUE.
/// </summary>
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
/// <summary>
/// Function sets "IsConnected" TRUE if connection confirmed
/// </summary>
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
/// <summary>
/// Function sets "IsConnected" TRUE if connection confirmed
/// </summary>
/// <param name="MaxAttempt">Counter of connection check attempts</param>
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
/// <summary>
/// Function sets IP to connect to for client
/// </summary>
/// <param name="ip">Char* with IP address of SERVER to connect to</param>
void Connection::SetConnectingIP(char* ip)
{
	for (int i = 0; i < IP_LENGTH; i++)
	{
		IP[i] = '\0';
	}

	strcpy_s(IP, ip);
}
/// <summary>
/// Disconnecting and cleaning up. Function must be called after Connection object has been created.
/// </summary>
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
/// <summary>
/// Function returns TRUE if UDP connection is disconnected.
/// </summary>
/// <returns>TRUE if disconnected.</returns>
bool Connection::Disconnected()
{
	return disconnected;
}
/// <summary>
/// Sets UDP::LastError.
/// </summary>
/// <param name="err">UDP::LastError to be set.</param>
void Connection::SetLastError(UDP::LastError err)
{
	ConnectionError = true;
	LAST_ERROR = err;
}

void Connection::StartAsyncManualConnect(Connection* inst)
{
	Connection* Inst = (Connection*)inst;
	Inst->ManualConnect();
}
/// <summary>
/// Function to get UDP::LastError and clean UDP::LastError.
/// </summary>
/// <param name="CleanLastError">Sets UDP::LastError = UDP::LastError::NONE if TRUE.</param>
/// <returns>UDP::LastError value. If no errors returns UDP::LastError::NONE</returns>
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
/// <summary>
/// Function for sending a message that contains SENDER,
/// TYPE, FLAG and message.
/// Must be called after creating Connection oject and calling "AsyncAutoConnect()"
/// or "AsyncManualConnect()" function.
/// </summary>
/// <param name="TYPE">Can be set as TYPE_CONNECTION, TYPE_CHECK,
/// TYPE_SHOOT, TYPE_DATA</param>
/// <param name="FLAG">Can be set as FLAG_ONE, FLAG_TWO, FLAG_THREE, FLAG_DISCONNECT</param>
/// <param name="msg">Has to be a char* to message</param>
/// <returns>TRUE if no error was achieved. If returned FALSE "GetLastError()"
/// function can be called to check error type.</returns>
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
/// <summary>
/// Function trying to receive message by UDP connection "iterOfReceive" times.
/// Must be called after creating Connection oject and calling "AsyncAutoConnect()"
/// or "AsyncManualConnect()" function.
/// </summary>
/// <param name="msg">Reference to UDP::MSG structure which will be filled.</param>
/// <param name="iterOfReceive">Counter of receiving attempts.</param>
/// <returns>TRUE if message was received.</returns>
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
/// <summary>
/// Function to check if UDP connection achieved.
/// </summary>
/// <returns>TRUE if connected.</returns>
bool Connection::Connected()
{
	return IsConnected;
}
/// <summary>
/// Function to check if specified "ConnectionType" connection achieved.
/// </summary>
/// <param name="connectionType">Parameter "ConnectionType" to compare with current connection type.</param>
/// <returns>TRUE if "connectionType" is UDP::ConnectionType::SERVER and
/// currently connected as SERVER. TRUE if "connectionType" is UDP::ConnectionType::CLIENT and
/// currently connected as CLIENT. FALSE if "connectionType" is UDP::ConnectionType::SERVER and
/// currently connected as CLIENT. FALSE if "connectionType" is UDP::ConnectionType::CLIENT and
/// currently connected as SERVER. FALSE if not connected.</returns>
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
}
