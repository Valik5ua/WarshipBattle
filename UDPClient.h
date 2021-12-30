#pragma once
#include"UDP.h"

class UDPClient : public UDP
{
public:
	UDPClient();
	UDPClient(UDP::ConnectionType connectionType, char* ip);
	void AsyncReceiveMSG();
	void ReceiveMSG(int iter);
	void SendMSG(char* msg);
	~UDPClient();

private:
	bool Init();
	void ReceiveMSG();
	static void StartAsyncReceiveMSG(UDPClient* inst);
};
