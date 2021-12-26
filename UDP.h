#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#include "winsock2.h"
#include <ws2tcpip.h>
#include <iostream>

#define IP_LENGTH           15

#define SENDER_SERVER       11
#define SENDER_CLIENT       12

#define TYPE_CONNECTION     21
#define TYPE_CHECK          22
#define TYPE_SHOOT          23
#define TYPE_DATA           24
#define TYPE_DEPLOYING      25
#define TYPE_MAINGAME       26

#define FLAG_ONE            31
#define FLAG_TWO            32
#define FLAG_THREE          33
#define FLAG_FOUR           34
#define FLAG_DISCONNECT     35

#define MYPORT              54000
#define MAX_MESSAGE_LENGTH  200

class UDP
{
public:
    /// <summary>
    /// CONNECTION_TYPE is a ConnectionType enum instance could be set to AUTO, SERVER or CLIENT
    /// </summary>
    enum class ConnectionType
    {
        AUTO = 0,
        SERVER,
        CLIENT
    }CONNECTION_TYPE;

    /// <summary>
    /// LAST_ERROR is a LastError enum instance could be set to NONE, WSA_INIT_ERROR,
    /// SOCKET_INIT_ERROR, BROADCAST_INIT_ERROR, BIND_ERROR, SEND_ERROR
    /// </summary>
    enum class LastError
    {
        NONE = 0,
        WSA_INIT_ERROR,
        SOCKET_INIT_ERROR,
        BROADCAST_INIT_ERROR,
        BIND_ERROR,
        SEND_ERROR
    }LAST_ERROR;

    /// <summary>
    /// Structure incapsulates message with fields SENDER, TYPE, FLAG, msg. Maximum length of msg
    /// can be checked in MAX_MSG_LENGTH field
    /// </summary>
    struct MSG
    {
        int SENDER;
        int TYPE;
        int FLAG;
        const int MAX_MSG_LENGTH = MAX_MESSAGE_LENGTH;
        char* msg = new char[MAX_MSG_LENGTH];
    }MESSAGE;

    static std::string ErroToString(UDP::LastError Err);
    UDP::LastError GetLastError(bool ClearLastError);
    void CleanUp();
    bool Received;
    void DecodeMsg(MSG& msg, char* str);
    char* FormMsg(int sender, int type, int flag, char* msg);
    char* MSGReceived = new char[MAX_MESSAGE_LENGTH];

protected:
    UDP();
    ~UDP();

protected:
    SOCKET sock;
    WSADATA wsaData;
    HANDLE HandleID;
    struct sockaddr_in Recv_addr;
    struct sockaddr_in Sender_addr;
    bool BreakThread;
    int len;
    int recvbufflen;
    char recvbuff[MAX_MESSAGE_LENGTH];
    char broadcast;
    //char* TempMsg = new char[MAX_MESSAGE_LENGTH + 3];
    char* TempMsg;

private:
    void MSGReceivedClean();
    void CleanMsg();    
};

