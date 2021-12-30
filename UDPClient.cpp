#include "UDPClient.h"
/// <summary>
/// Default Constructor
/// </summary>
UDPClient::UDPClient()
{
	len = sizeof(struct sockaddr_in);
	//Received = false;
	//BreakThread = false;

	Recv_addr.sin_family = AF_INET;
	Recv_addr.sin_port = htons(MYPORT);
	Recv_addr.sin_addr.s_addr = INADDR_BROADCAST;
	
	LAST_ERROR = UDP::LastError::NONE;
	CONNECTION_TYPE = UDP::ConnectionType::AUTO;

	if (!Init())
	{
		closesocket(sock);
		WSACleanup();
	}
}
/// <summary>
/// Constructor with ConnectionType and IP address
/// </summary>
/// <param name="connectionType">Value of UDP::ConnectioType structure must be
/// UDP::ConnectionType::CLIENT</param>
/// <param name="ip">Char* to IP address to which has to be connected default is "127.0.0.1" 
/// local host</param>
UDPClient::UDPClient(UDP::ConnectionType connectionType, char* ip)
{
	len = sizeof(struct sockaddr_in);
	//Received = false;
	//BreakThread = false;

	Recv_addr.sin_family = AF_INET;
	Recv_addr.sin_port = htons(MYPORT);
	
	if (connectionType == UDP::ConnectionType::CLIENT)
	{
		Recv_addr.sin_addr.s_addr = inet_addr(ip);
	}
	else
	{
		Recv_addr.sin_addr.s_addr = INADDR_BROADCAST;
	}

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
bool UDPClient::Init()
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
		if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
		{
			std::cout << "Error in setting Broadcast option" << std::endl;
			LAST_ERROR = UDP::LastError::BROADCAST_INIT_ERROR;
			return false;
		}
	}

	u_long iMode = 1;
	ioctlsocket(sock, FIONBIO, &iMode);

	return true;
}
/// <summary>
/// Function creates THREAD for StartAsyncReceiveMSG() function and passing current instance as parameter
/// </summary>
void UDPClient::AsyncReceiveMSG()
{
	HandleID = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&StartAsyncReceiveMSG, (LPVOID)this, 0, NULL);
}
/// <summary>
/// Function starts ReceiveMSG() function from received instance
/// </summary>
/// <param name="inst">Instance of UDPClient Class</param>
void UDPClient::StartAsyncReceiveMSG(UDPClient* inst)
{
	UDPClient* clientInst = (UDPClient*)inst;
	clientInst->ReceiveMSG();
}
/// <summary>
/// Sending message by "sendto" of WSA UDP connection
/// </summary>
/// <param name="msg">Char* to message has to send</param>
void UDPClient::SendMSG(char* msg)
{
	if (sendto(sock, msg, strlen(msg) + 1, 0, (sockaddr*)&Recv_addr, sizeof(Recv_addr)) < 0)
	{
		std::cout << "Error in Sending." << WSAGetLastError() << std::endl;
		LAST_ERROR = UDP::LastError::SEND_ERROR;
		std::system("pause");
		closesocket(sock);
	}
	Sleep(10);
}
/// <summary>
/// Function for receive message by "recvfrom" of WSA UDP connection.
/// Stops receiving if "BreakThread" is TRUE or message is received
/// </summary>
void UDPClient::ReceiveMSG()
{
	Received = false;
	ZeroMemory(recvbuff, sizeof(recvbuff));

	while (true)
	{
		recvfrom(sock, recvbuff, recvbufflen, 0, (sockaddr*)&Recv_addr, &len);
		Sleep(10);
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
/// Function for receive message by "recvfrom" of WSA UDP connection.
/// Stops receiving if "--iter" is 0 or message is received
/// </summary>
/// <param name="iter">Counter of receiving iterations</param>
void UDPClient::ReceiveMSG(int iter)
{
	Received = false;
	
	ZeroMemory(recvbuff, sizeof(recvbuff));

	while (true)
	{
		recvfrom(sock, recvbuff, recvbufflen, 0, (sockaddr*)&Recv_addr, &len);
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
/// Destructor
/// </summary>
UDPClient::~UDPClient()
{
}
