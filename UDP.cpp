#pragma comment(lib,"Ws2_32.lib")
#include "UDP.h"

/// <summary>
/// Default constructor
/// </summary>
UDP::UDP()
{
	CONNECTION_TYPE = ConnectionType::AUTO;
	LAST_ERROR = LastError::NONE;
	BreakThread = false;
	Received = false;
	HandleID = NULL;
	len = NULL;
	broadcast = '1';
	recvbufflen = MAX_MESSAGE_LENGTH;
	TempMsg = new char[MAX_MESSAGE_LENGTH + 3];
}

/// <summary>
/// Static function that returns string value of last set error. Have to be used 
/// with GetLastError() function.
/// </summary>
/// <param name="Err">is a UDP::LastError structure value
///  that can be get by GetLastError() function.</param>
/// <returns>Returns a std::string that represents a string value of UDP::LastError</returns>
std::string UDP::ErroToString(UDP::LastError Err)
{
	switch (Err)
	{
	case UDP::LastError::WSA_INIT_ERROR: { return "WSA_INIT_ERROR"; }
	break;
	case UDP::LastError::SOCKET_INIT_ERROR: { return "SOCKET_INIT_ERROR"; }
	break;
	case UDP::LastError::BROADCAST_INIT_ERROR: { return "BROADCAST_INIT_ERROR"; }
	break;
	case UDP::LastError::SEND_ERROR: { return "SEND_ERROR"; }
	break;
	case UDP::LastError::BIND_ERROR: { return "BIND_ERROR"; }
	break;
	default: return "NONE";
	}
}
/// <summary>
/// Function to get value of UDP::LastError
/// </summary>
/// <param name="ClearLastError">True or False to set UDP::LastError structure to NONE</param>
/// <returns>Returns value of UDP::LastError structure</returns>
UDP::LastError UDP::GetLastError(bool ClearLastError)
{
	switch (this->LAST_ERROR)
	{
	case UDP::LastError::WSA_INIT_ERROR:
	{
		if (ClearLastError) LAST_ERROR = UDP::LastError::NONE;
		return UDP::LastError::WSA_INIT_ERROR;
	}
	break;
	case UDP::LastError::SOCKET_INIT_ERROR:
	{
		if (ClearLastError) LAST_ERROR = UDP::LastError::NONE;
		return UDP::LastError::SOCKET_INIT_ERROR;
	}
	break;
	case UDP::LastError::BROADCAST_INIT_ERROR:
	{
		if (ClearLastError) LAST_ERROR = UDP::LastError::NONE;
		return UDP::LastError::BROADCAST_INIT_ERROR;
	}
	break;
	case UDP::LastError::SEND_ERROR:
	{
		if (ClearLastError) LAST_ERROR = UDP::LastError::NONE;
		return UDP::LastError::SEND_ERROR;
	}
	break;
	case UDP::LastError::BIND_ERROR:
	{
		if (ClearLastError) LAST_ERROR = UDP::LastError::NONE;
		return UDP::LastError::BIND_ERROR;
	}
	break;
	default: return UDP::LastError::NONE;
	}

	return UDP::LastError::NONE;
}
/// <summary>
/// Function sets '\0' to MSGReceived char*
/// </summary>
void UDP::MSGReceivedClean()
{
	for (int i = 0; i < strlen(MSGReceived); i++)
	{
		MSGReceived[i] = '\0';
	}
}
/// <summary>
/// Function used for forming a char* that contains SENDER,
/// TYPE, FLAG and msg
/// </summary>
/// <param name="sender">Can be set as SERVER or CLIENT</param>
/// <param name="type">Can be set as TYPE_CONNECTION, TYPE_CHECK,
/// TYPE_SHOOT, TYPE_DATA</param>
/// <param name="flag">Can be set as FLAG_ONE, FLAG_TWO, FLAG_THREE, FLAG_DISCONNECT</param>
/// <param name="msg">Have to be a char* to message.</param>
/// <returns>Char* to formed message.</returns>
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
/// <summary>
/// Function that filling up all paramiters of UDP::MSG structure from char*
/// </summary>
/// <param name="msg">Reference to UDP::MSG structure</param>
/// <param name="str">Char* to msg suppose to be decoded</param>
void UDP::DecodeMsg(MSG& msg, char* str)
{
	msg.FLAG = NULL;
	msg.SENDER = NULL;
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
/// <summary>
/// Cleans all fields of UDP::MSG structure
/// </summary>
void UDP::CleanMsg()
{
	for (int i = 0; i < (MAX_MESSAGE_LENGTH + 3); i++)
	{
		TempMsg[i] = '\0';
	}

	MESSAGE.FLAG = NULL;
	MESSAGE.SENDER = NULL;
	MESSAGE.TYPE = NULL;
	for (int i = 0; i < MESSAGE.MAX_MSG_LENGTH; i++)
	{
		MESSAGE.msg[i] = '\0';
	}
}

/// <summary>
/// Destructor
/// </summary>
UDP::~UDP()
{
	delete MESSAGE.msg;
	delete TempMsg;
}
/// <summary>
/// Function to close Socket and WSA clean up
/// </summary>
void UDP::CleanUp()
{
	closesocket(sock);
	WSACleanup();
}