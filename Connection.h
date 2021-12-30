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

/// <summary>
/// Class mean for creating UDP connection in the locan network. To creat UDP connection
/// create pointer for class "Connection", allocate memory in heap with
/// one of "UDP::ConnectionType" enum specifier. To achive connection automaticaly pass to 
/// constructor "UDP::ConnectionType::AUTO" as parameter. To start connection as server
/// use "UDP::ConnectionType::SERVER". To start connection as client use
/// "UDP::ConnectionType::CLIENT". Use "GetLastError" to chech if initialization of connection
/// was successfull. Use "AsyncAutoConnect" to start auto connection, or "AsyncManualConnect"
/// to start connection as server or client manualy. Connection achieved if "Connected" returns
/// true. To get your PC's IP address use "GetMyIP". If you are trying to connect as client use
/// "SetConnectingIP" to set IP address of server side PC. Use "SendMSG" and "ReceiveMSG"
/// to communicate between sides. To stop connection use "AsyncDisconnect", connection
/// terminated if "Disconnected" returns true.
/// </summary>
class Connection
{
public:
	/// <summary>
	/// Deleted default constructor.
	/// </summary>
	Connection() = delete;
	/// <summary>
	/// Constructor with ConnectionType
	/// </summary>
	/// <param name="connectionType">Must pass UDP::ConnectionType.
	/// Can be UDP::ConnectionType::AUTO, UDP::ConnectionType::SERVER or UDP::ConnectionType::CLIENT</param>
	Connection(UDP::ConnectionType connectionType);
	/// <summary>
	/// Function creates UDP connection automaticaly. Connection object must be created with 
	/// UDP::ConnectionType::AUTO parameter befor calling this function. GetLastError() function
	/// has to be called after creating Connection object to check if error was set, if 
	/// object was created and WSA was initialized without errors GetLastError() function
	/// returns UDP::LastError::NONE.
	/// </summary>
	void AsyncAutoConnect();
	/// <summary>
	/// Function creates UDP connection automaticaly. Connection object must be created with 
	/// UDP::ConnectionType::SERVER or UDP::ConnectionType::SERVER 
	/// parameter befor calling this function. GetLastError() function
	/// has to be called after creating Connection object to check if error was set, if 
	/// object was created and WSA was initialized without errors GetLastError() function
	/// returns UDP::LastError::NONE.
	/// </summary>
	void AsyncManualConnect();
	/// <summary>
	/// Function ASYNC disconnecting UDP connection if it's connected or not.
	/// Connection object must be created befor calling this function.
	/// "Disconnected" is TRUE when UDP connection closed.
	/// </summary>
	void AsyncDisconnect();
	/// <summary>
	/// Function to check if UDP connection achieved.
	/// </summary>
	/// <returns>TRUE if connected.</returns>
	bool Connected();
	/// <summary>
	/// Function to check if specified "UDP::ConnectionType" connection achieved.
	/// </summary>
	/// <param name="connectionType">Parameter "UDP::ConnectionType" to compare with current connection type.</param>
	/// <returns>TRUE if "connectionType" is UDP::ConnectionType::SERVER and
	/// currently connected as SERVER. TRUE if "connectionType" is UDP::ConnectionType::CLIENT and
	/// currently connected as CLIENT. FALSE if "connectionType" is UDP::ConnectionType::SERVER and
	/// currently connected as CLIENT. FALSE if "connectionType" is UDP::ConnectionType::CLIENT and
	/// currently connected as SERVER. FALSE if not connected.</returns>
	bool Connected(UDP::ConnectionType connectionType);
	/// <summary>
	/// Function returns TRUE if UDP connection is disconnected.
	/// </summary>
	/// <returns>TRUE if disconnected.</returns>
	bool Disconnected();
	/// <summary>
	/// Function trying to receive message by UDP connection "iterOfReceive" times.
	/// Must be called after creating Connection oject and calling "AsyncAutoConnect()"
	/// or "AsyncManualConnect()" function.
	/// </summary>
	/// <param name="msg">Reference to UDP::MSG structure which will be filled.</param>
	/// <param name="iterOfReceive">Counter of receiving attempts.</param>
	/// <returns>TRUE if message was received.</returns>
	bool ReceiveMSG(UDP::MSG& msg, int iterOfReceive);
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
	bool SendMSG(int TYPE, int FLAG, char* msg);
	/// <summary>
	/// Function sets IP to connect to for client
	/// </summary>
	/// <param name="ip">Char* with IP address of SERVER to connect to</param>
	void SetConnectingIP(char* ip);
	/// <summary>
	/// Use to get IP address of current PC.
	/// </summary>
	/// <returns>Char* to char array with IP address in local network.</returns>
	char* GetMyIP();
	/// <summary>
	/// Function to get UDP::LastError and clean UDP::LastError.
	/// </summary>
	/// <param name="CleanLastError">Sets UDP::LastError = UDP::LastError::NONE if TRUE.</param>
	/// <returns>UDP::LastError value. If no errors returns UDP::LastError::NONE</returns>
	UDP::LastError GetLastError(bool CleanLastError);

private:
	UDP::ConnectionType CONNECTION_TYPE;
	UDP::LastError LAST_ERROR;
	HANDLE HandleID;
	UDPServer* Server;
	UDPClient* Client;
	char IP[IP_LENGTH];
	char MyIP[IP_LENGTH];
	bool IsConnected;
	bool IsServer;
	bool IsClient;
	bool IsIPSet;
	bool CancelConnecting;
	bool ConnectionError;
	bool disconnected;

private:
	
	void AsyncCheckConnection();
	/// <summary>
	/// Function for achieving UDP connection as server or client.
	/// Stops working if connection achieved or "CancelConnecting" is TRUE.
	/// </summary>
	void AutoConnect();
	/// <summary>
	/// Function sets "IsConnected" TRUE if connection confirmed
	/// </summary>
	void CheckConnection();
	/// <summary>
	/// Function sets "IsConnected" TRUE if connection confirmed
	/// </summary>
	/// <param name="MaxAttempt">Counter of connection check attempts</param>
	void CheckConnection(int MaxAttempt);
	/// <summary>
	/// Function for achieving UDP connection as client.
	/// Stops working if connection achieved or "CancelConnecting" is TRUE.
	/// </summary>
	void ClientConnect();
	/// <summary>
	/// Function for achieving UDP connection as client.
	/// Stops working if connection achieved or "iter" is 0.
	/// </summary>
	/// <param name="iter">Counter of connection attempts</param>
	void ClientConnect(int iter);
	/// <summary>
	/// Closing WSA UDP connection and setting default object parameters
	/// </summary>
	void CleanUP();
	/// <summary>
	/// Disconnecting and cleaning up. Function must be called after Connection object has been created.
	/// </summary>
	void Disconnect();
	/// <summary>
	/// Function for achieving UDP connection as server or client dependes on UDP::ConnectionType.
	/// Stops working if connection achieved or "CancelConnecting" is TRUE.
	/// </summary>
	void ManualConnect();
	/// <summary>
	/// Function for achieving UDP connection as server.
	/// Stops working if connection achieved or "CancelConnecting" is TRUE.
	/// </summary>
	void ServerConnect();
	/// <summary>
	/// Function for achieving UDP connection as server.
	/// Stops working if connection achieved or "iter" is 0.
	/// </summary>
	/// <param name="iter">Counter of connection attempts</param>
	void ServerConnect(int iter);
	/// <summary>
	/// Sets UDP::LastError.
	/// </summary>
	/// <param name="err">UDP::LastError to be set.</param>
	void SetLastError(UDP::LastError err);

	void SetMyIP();
	static void StartAsyncManualConnect(Connection* inst);
	static void StartAsyncCheckConnection(Connection* inst);
	static void StartAsyncAutoConnect(Connection* inst);
	static void StartAsyncDisconnect(Connection* inst);
};

