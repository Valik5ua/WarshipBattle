#include "UDPClient.h"

UDPClient::UDPClient()
{
	len = sizeof(struct sockaddr_in);
	Received = false;
	BreakThread = false;

	Recv_addr.sin_family = AF_INET;
	Recv_addr.sin_port = htons(MYPORT);
	Recv_addr.sin_addr.s_addr = INADDR_BROADCAST;
	
	LAST_ERROR = NONE;
	if (!Init())
	{
		closesocket(sock);
		WSACleanup();
	}
}

bool UDPClient::Init()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		LAST_ERROR = WSA_INIT_ERROR;
		return false;
	}

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		LAST_ERROR = SOCKET_INIT_ERROR;
		return false;
	}

	//     This option is needed on the socket in order to be able to receive broadcast messages
	//   If not set the receiver will not receive broadcast messages in the local network.
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
	{
		std::cout << "Error in setting Broadcast option" << std::endl;
		LAST_ERROR = BROADCAST_INIT_ERROR;
		return false;
	}

	u_long iMode = 1;
	ioctlsocket(sock, FIONBIO, &iMode);

	return true;
}

void UDPClient::AsyncReceiveMSG()
{
	HandleID = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&StartAsyncReceiveMSG, (LPVOID)this, 0, NULL);
}

void UDPClient::StartAsyncReceiveMSG(UDPClient* inst)
{
	UDPClient* clientInst = (UDPClient*)inst;
	clientInst->ReceiveMSG();
}

void UDPClient::SendMSG(char* msg)
{
	if (sendto(sock, msg, strlen(msg) + 1, 0, (sockaddr*)&Recv_addr, sizeof(Recv_addr)) < 0)
	{
		std::cout << "Error in Sending." << WSAGetLastError() << std::endl;
		LAST_ERROR = SEND_ERROR;
		std::system("pause");
		closesocket(sock);
	}
	Sleep(10);
	//else
		//std::cout << "CLIENT send message Successfully" << std::endl;
}

void UDPClient::ReceiveMSG()
{
	Received = false;
	/*u_long iMode = 1;
	ioctlsocket(sock, FIONBIO, &iMode);*/
	ZeroMemory(recvbuff, sizeof(recvbuff));

	while (true)
	{
		recvfrom(sock, recvbuff, recvbufflen, 0, (sockaddr*)&Recv_addr, &len);
		//std::cout << "Received Message is : " << recvbuff << std::endl;
		//std::cout << "Received Message is : " << strlen(recvbuff) << std::endl;
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

void UDPClient::ReceiveMSG(int iter)
{
	Received = false;
	
	ZeroMemory(recvbuff, sizeof(recvbuff));

	while (true)
	{
		recvfrom(sock, recvbuff, recvbufflen, 0, (sockaddr*)&Recv_addr, &len);
		//std::cout << "Received Message is : " << recvbuff << std::endl;
		//std::cout << "Received Message is : " << strlen(recvbuff) << std::endl;
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

UDPClient::~UDPClient()
{
}
