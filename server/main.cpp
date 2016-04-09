#include "Server.h"
#include "InputFile.h"
#include "HTTPRequest.h"
#include "FileFetcher.h"
#include <iostream>
#include "chat.h"
using namespace std;


Room room("BF Chat");


class ChatServer: public Goat::Server{
	static string parse(const Goat::Request& request){
		if(request.substr(0,4)=="NUSR"){
			User nu = User(request.substr());
			room.addUser(nu);
			cout<<"New User <"<<nu.getName()<<"> added\n";
		}
	}
	static string reply_n(const Goat::Request& request){
		cout<<"Echo: "+request.getData();
		return "Echo: "+request.getData();
	}
public:
	ChatServer(){
		addPortFunction(2048,reply_n);
	}
};
int main()
{
	ChatServer server;
    server.run();
    return 0;
}
