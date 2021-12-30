#include "UDPServer.h"
/// <summary>
/// Default Constructor
/// </summary>
UDPServer::UDPServer()
{
	len = sizeof(struct sockaddr_in);
	//Received = false;
	//BreakThread = false;

	Recv_addr.sin_family = AF_INET;
	Recv_addr.sin_port = htons(MYPORT);
	Recv_addr.sin_addr.s_addr = INADDR_ANY;
	
	CONNECTION_TYPE = UDP::ConnectionType::AUTO;
	LAST_ERROR = UDP::LastError::NONE;
	
	if (!Init())
	{
		closesocket(sock);
		WSACleanup();
	}
}
/// <summary>
/// Constructor with ConnectionType
/// </summary>
/// <param name="connectionType">Value of UDP::ConnectioType structure must be
/// UDP::ConnectionType::SERVER</param>
UDPServer::UDPServer(UDP::ConnectionType connectionType)
{
	len = sizeof(struct sockaddr_in);
	//Received = false;
	//BreakThread = false;

	Recv_addr.sin_family = AF_INET;
	Recv_addr.sin_port = htons(MYPORT);
	Recv_addr.sin_addr.s_addr = INADDR_ANY;

	CONNECTION_TYPE = connectionType;
	LAST_ERROR = UDP::LastError::NONE;

	if (!Init())
	{
		closesocket(sock);
		WSACleanup();
	}
}
/// <summary>
/// Function for initialize all WSA parameters and create Socket
/// </summary>
/// <returns>True if all initializations didn't set Error. False if initializations 
/// was finished with Error, UDP::LAST_ERROR holds Error code</returns>
bool UDPServer::Init()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		LAST_ERROR = UDP::LastError::WSA_INIT_ERROR;
		return false;
	}
	
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		LAST_ERROR = UDP::LastError::SOCKET_INIT_ERROR;
		return false;
	}

	//     This option is needed on the socket in order to be able to receive broadcast messages
	//   If not set the receiver will not receive broadcast messages in the local network.
	if (CONNECTION_TYPE == UDP::ConnectionType::AUTO)
	{
		if (setsockopt(this->sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
		{
			std::cout << "Error in setting Broadcast option" << std::endl;
			LAST_ERROR = UDP::LastError::BROADCAST_INIT_ERROR;
			return false;
		}
	}
	
	char enable = '1';
	if (setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0)
	{
		std::cout << "Error in setting Reuseaddr option" << std::endl;
		LAST_ERROR = UDP::LastError::BROADCAST_INIT_ERROR;
		return false;
	}

	if (bind(sock, (sockaddr*)&Recv_addr, sizeof(Recv_addr)) < 0)
	{
		std::cout << "Error in BINDING SERVER" << WSAGetLastError() << std::endl;

		std::system("pause");
		LAST_ERROR = UDP::LastError::BIND_ERROR;
		return false;
	}

	u_long iMode = 1;
	ioctlsocket(sock, FIONBIO, &iMode);

	return true;
}
/// <summary>
/// Function creates THREAD for StartAsyncReceiveMSG() function and passing current instance as parameter
/// </summary>
void UDPServer::AsyncReceiveMSG()
{
	HandleID = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&StartAsyncReceiveMSG, (LPVOID)this, 0, NULL);
}
/// <summary>
/// Function for receive message by "recvfrom" of WSA UDP connection.
/// Stops receiving if "BreakThread" is TRUE or message is received
/// </summary>
void UDPServer::ReceiveMSG()
{
	Received = false;
  
	ZeroMemory(recvbuff, sizeof(recvbuff));

	while (true)
	{
		recvfrom(sock, recvbuff, recvbufflen, 0, (sockaddr*)&Sender_addr, &len);
		if (strlen(recvbuff) != 0)
		{
			MSGReceived = (char*)recvbuff;
			Received = true;
			break;
		}
		if (BreakThread) break;
	}
}
/// <summary>
/// Function starts ReceiveMSG() function from received instance
/// </summary>
/// <param name="inst">Instance of UDPServer Class</param>
void UDPServer::StartAsyncReceiveMSG(UDPServer* inst)
{
	UDPServer* serverInst = (UDPServer*)inst;
	serverInst->ReceiveMSG();
}
/// <summary>
/// Function for receive message by "recvfrom" of WSA UDP connection.
/// Stops receiving if "--iter" is 0 or message is received
/// </summary>
/// <param name="iter">Counter of receiving iterations</param>
void UDPServer::ReceiveMSG(int iter)
{
	Received = false;
	
	ZeroMemory(recvbuff, sizeof(recvbuff));

	while (true)
	{
		recvfrom(sock, recvbuff, recvbufflen, 0, (sockaddr*)&Sender_addr, &len);
		Sleep(10);
		if (strlen(recvbuff) != 0)
		{
			MSGReceived = (char*)recvbuff;
			Received = true;
			break;
		}

		if (--iter == 0) break;
	}
}
/// <summary>
/// Sending message by "sendto" of WSA UDP connection
/// </summary>
/// <param name="msg">Char* to message has to send</param>
void UDPServer::SendMSG(char* msg)
{
	if (sendto(sock, msg, strlen(msg) + 1, 0, (sockaddr*)&Sender_addr, sizeof(Sender_addr)) < 0)
	{
		std::cout << "Error in Sending." << WSAGetLastError() << std::endl;
		LAST_ERROR = UDP::LastError::SEND_ERROR;
		std::system("pause");
		closesocket(sock);
	}
	Sleep(10);
}
/// <summary>
/// Destructor
/// </summary>
UDPServer::~UDPServer()
{
}
