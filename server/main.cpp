#include "Server.h"
#include "InputFile.h"
#include "HTTPRequest.h"
#include "FileFetcher.h"
#include <iostream>
#include "chat.h"
using namespace std;
#define CHAT_STATUS_OK "0"
#define CHAT_STATUS_USER_EXISTS "1"
#define CHAT_STATUS_USER_DNE "1"
#define CHAT_STATUS_TIMEOUT "2"
#define CHAT_STATUS_FUCKYOU "3"
#define CHAT_STATUS_WHAT "4"
#define CHAT_STATUS_NOMSG "5"

Room room("BF Chat");


class ChatServer: public Goat::Server{
	
	static std::string parse(std::string data){
		if(data.substr(0,4)=="NUSR"){
			User nu = User(data.substr(5,data.length()-1));
			if(!room.userExists(nu.getName())){
				room.addUser(nu);
				cout<<"New User <"<<nu.getName()<<"> added\n";
				return CHAT_STATUS_OK;
			}else{
				return CHAT_STATUS_USER_EXISTS;
			}	
		}
		if(data.substr(0,4)=="RECV"){
			return room.dump();
		}
		if(data.substr(0,4)=="NMSG"){
			data = data.substr(5,data.length()-1);
			size_t a=data.find(" ");
			if(a!=std::string::npos){
				
				std::string username = data.substr(0,a);
				std::string message = data.substr(a+1,data.length()-1);
				cout<<"Username: <"<<username<<">\nMessage: <"<<message<<">\n";
				
				if(room.userExists(username)){
					room.newMessage(Message(User(username),message));
					cout<<"Message added\n";
				}else{
					cout<<"User does not exists :("<<endl;
					return CHAT_STATUS_USER_DNE;
				}
			}else{
				return CHAT_STATUS_NOMSG;
			}
		}
		return CHAT_STATUS_WHAT;
	}
	static string reply_n(const Goat::Request& request){
		std::string data = request.getData();
		std::string response="idk";
		std::string status="200";
		if(data.length()>7){
			parse(data);
		}
		response = room.dump();
		return "HTTP/1.1 "+status+" OK\r\nAccess-Control-Allow-Origin: *\r\nAccept-Ranges: bytes\r\n\
Connection: close\r\nDate: Mon, 23 May 2005 22:38:34 GMT\r\nContent-Type: text/html; charset=UTF-8\r\n\
Content-Length: "+std::to_string(response.length())+"\r\n\r\n"+response;
	}
public:
	ChatServer(){
		setTracing(false);
		addPortFunction(2048,reply_n);
	}
};
int main()
{
	ChatServer server;
    server.run();
    return 0;
}
