#pragma comment(lib,"Ws2_32.lib")
#include "UDP.h"

void UDP::ErroToConsole(int Err)
{
    switch (Err)
    {
    case WSA_INIT_ERROR: { std::cout << "WSA_INIT_ERROR" << std::endl; }
                       break;
    case SOCKET_INIT_ERROR: { std::cout << "SOCKET_INIT_ERROR" << std::endl; }
                          break;
    case BROADCAST_INIT_ERROR: { std::cout << "BROADCAST_INIT_ERROR" << std::endl; }
                             break;
    case SEND_ERROR: { std::cout << "SEND_ERROR" << std::endl; }
                   break;
    case BIND_ERROR: { std::cout << "BIND_ERROR" << std::endl; }
                   break;
    default: std::cout << "NONE" << std::endl;
    };
}

int UDP::GetLastError(bool ClearLastError)
{
    switch (this->LAST_ERROR)
    {
    case WSA_INIT_ERROR:
    {
        if (ClearLastError) LAST_ERROR = NONE;
        return WSA_INIT_ERROR;
    }
    break;
    case SOCKET_INIT_ERROR:
    {
        if (ClearLastError) LAST_ERROR = NONE;
        return SOCKET_INIT_ERROR;
    }
    break;
    case BROADCAST_INIT_ERROR:
    {
        if (ClearLastError) LAST_ERROR = NONE;
        return BROADCAST_INIT_ERROR;
    }
    break;
    case SEND_ERROR:
    {
        if (ClearLastError) LAST_ERROR = NONE;
        return SEND_ERROR;
    }
    break;
    case BIND_ERROR:
    {
        if (ClearLastError) LAST_ERROR = NONE;
        return BIND_ERROR;
    }
    break;
    default: return NONE;
    }

    return NONE;
}

void UDP::MSGReceivedClean()
{
    for (int i = 0; i < strlen(MSGReceived); i++)
    {
        MSGReceived[i] = '\0';
    }
}

char* UDP::FormMsg(int sender, int type, int flag, char* msg)
{
    CleanMsg();

    MESSAGE.SENDER = sender;
    MESSAGE.TYPE = type;
    MESSAGE.FLAG = flag;
    strcpy_s(MESSAGE.msg, MESSAGE.MAX_MSG_LENGTH, msg);

    //char* temp = new char[MESSAGE.MAX_MSG_LENGTH + 3];
    //ZeroMemory(temp, sizeof(temp));

    TempMsg[0] = MESSAGE.SENDER;
    TempMsg[1] = MESSAGE.TYPE;
    TempMsg[2] = MESSAGE.FLAG;

    for (int i = 0; i <= strlen(MESSAGE.msg); i++)
    {
        TempMsg[i + 3] = MESSAGE.msg[i];
    }

    //std::cout << "size of msg: " << strlen(MESSAGE.msg) + 3 << std::endl;
    return TempMsg;
}

void UDP::DecodeMsg(MSG& msg, char* str)
{
    msg.FLAG = NULL;
    msg.SENDER = NULL;
    msg.SIZE = NULL;
    msg.TYPE = NULL;
    for (int i = 0; i < msg.MAX_MSG_LENGTH; i++)
    {
        msg.msg[i] = '\0';
    }
    
    msg.SENDER = str[0];
    msg.TYPE = str[1];
    msg.FLAG = str[2];
    for (int i = 0; i <= strlen(str); i++)
    {
        msg.msg[i] = str[i+3];
    }
    MSGReceivedClean();
}

void UDP::CleanMsg()
{
    for (int i = 0; i < (MML + 3); i++)
    {
        TempMsg[i] = '\0';
    }

    MESSAGE.FLAG = NULL;
    MESSAGE.SENDER = NULL;
    MESSAGE.SIZE = NULL;
    MESSAGE.TYPE = NULL;
    for (int i = 0; i < MESSAGE.MAX_MSG_LENGTH; i++)
    {
        MESSAGE.msg[i] = '\0';
    }
}


UDP::UDP()
{
}

UDP::~UDP()
{
    delete MESSAGE.msg;
    delete TempMsg;
}

void UDP::CleanUp()
{
    closesocket(sock);
    WSACleanup();
}