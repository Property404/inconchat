#pragma once
#include <string>
#include <winsock2.h>

namespace Goat{
/*
	Request Class (non protocol-specific)
	Contains info that the listener thread
	passes onto the port function
*/
class Request{
private:
	int server_port;//Port server was listening on
	SOCKET server_socket;//Listening socket
	SOCKET client_socket;
	//std::string server_ip;//IP of the server
	//std::string client_ip;//IP of the client
	std::string data;//Contents of request
public:
	// Getters
	int getServerPort() const;
	SOCKET getServerSocket() const;
	SOCKET getClientSocket() const;
	std::string getData() const;//Expect to be overridden
	
	// Setters
	void setServerPort(int);
	void setServerSocket(SOCKET);
	void setClientSocket(SOCKET);
	void setData(const std::string&);//Expect to be overridden

};
}