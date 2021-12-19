#pragma once
#include "UDP.h"
#include "UDPServer.h"
#include "UDPClient.h"

#define MSG_RECEIVE_ITER_NUM				100
#define SERVER_CONNECTION_SLEEP_TIME		400
#define CLIENT_CONNECTION_SLEEP_TIME		200
#define SERVER_CHECKING_SLEEP_TIME			20
#define CLIENT_CHECKING_SLEEP_TIME			20
#define MAX_CHEKING_ATTEMPTS				20
#define SERVER_CHECKING_ITER_NUM			20
#define CLIENT_CHECKING_ITER_NUM			30
#define	AUTO_CONNECT_CHECKING_ITER_NUM		10
#define AUTO_CONNECT_SERVER_ATTEMPTS		2
#define AUTO_CONNECT_CLIENT_ATTEMPTS		1

class Connection
{
public:
	UDP::ConnectionType CONNECTION_TYPE;
	Connection() = delete;
	Connection(UDP::ConnectionType connectionType);
	void AsyncAutoConnect();
	void AsyncManualConnect();
	void AsyncDisconnect();
	bool Connected();
	bool Connected(UDP::ConnectionType connectionType);
	bool Disconnected();
	UDP::LastError GetLastError(bool CleanLastError);
	bool SendMSG(int TYPE, int FLAG, char* msg);
	bool ReceiveMSG(UDP::MSG& msg, int iterOfReceive);
	void SetConnectingIP(char* ip);

private:
	void ManualConnect();
	void Disconnect();
	void AsyncCheckConnection();
	void ServerConnect();
	void ServerConnect(int iter);
	void ClientConnect();
	void ClientConnect(int iter);
	void CheckConnection();
	void CheckConnection(int MaxAttempt);

private:
	UDP::LastError LAST_ERROR;
	HANDLE HandleID;
	UDPServer* Server;
	UDPClient* Client;
	char IP[IP_LENGTH];
	bool IsConnected;
	bool IsServer;
	bool IsClient;
	bool CancelConnecting;
	bool ConnectionError;
	bool disconnected;

private:
	void CleanUP();
	void SetLastError(UDP::LastError err);
	static void StartAsyncManualConnect(Connection* inst);
	static void StartAsyncCheckConnection(Connection* inst);
	static void StartAsyncAutoConnect(Connection* inst);
	static void StartAsyncDisconnect(Connection* inst);
	void AutoConnect();
};

