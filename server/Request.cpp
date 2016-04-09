#include "Request.h"

namespace Goat{
// Getters
int Request::getServerPort() const{
	return server_port;
}
SOCKET Request::getServerSocket() const{
	return server_socket;
}
SOCKET Request::getClientSocket() const{
	return client_socket;
}
std::string Request::getData() const{
	return data;
}

// Setters
void Request::setServerPort(int port){
	server_port = port;
}
void Request::setServerSocket(SOCKET socket){
	server_socket = socket;
}
void Request::setClientSocket(SOCKET socket){
	client_socket = socket;
}
void Request::setData(const std::string &data){
	this->data = data;
}

}