#pragma once
#include"UDP.h"

class UDPServer : public UDP
{
public:
    UDPServer();
    void AsyncReceiveMSG();
    void ReceiveMSG(int iter);
    void SendMSG(char* msg);
    ~UDPServer();

private:
    bool Init();
    void ReceiveMSG();
    static void StartAsyncReceiveMSG(UDPServer* inst);
};
