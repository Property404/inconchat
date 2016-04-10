#include "Server.h"
#include "InputFile.h"
#include "HTTPRequest.h"
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

std::string bfi(const std::string& source){
	//ORIGINALLY IN C, THAT'S WHY ALL THE MALLOCS! STFU!
	std::string output="";
	/* Set up tape*/
	unsigned int tapesize = 1;
	char* tape = (char*)malloc(sizeof(char)*tapesize);
	tape[0] = 0;
	
	/* Set up while list*/
	int loopdepth=0;int lqsize=1;int skip=0;
	int* loopqueue=(int*)malloc(sizeof(int)*lqsize);
	
	//Interpret
	unsigned int i = 0; unsigned int pointer=0;
	for(i=0;i<source.length();i++){
		//if(pointer<0)printf("\nERROR POINTER SUBZERO\n");
		//Skipping loop
		
		
		//Beginning of loop
		if (source[i]=='['){
			loopdepth++;
			if (skip)skip++;
			else if(tape[pointer]==0)
				skip = 1;
			if(loopdepth>=lqsize){
				lqsize++;
				int* ptemp = (int*)realloc(loopqueue, sizeof(int)*lqsize);//Reallocate loop queue
				if(ptemp==NULL)printf("\nERROR _ REALLOC\n");
				loopqueue=ptemp;
				}
			if(loopdepth==0)printf("\nERROR _ LOOPDEPTH SUBONE\n");
			loopqueue[loopdepth-1] = i;
			continue;
		}
		
		//End of loop
		else if(source[i]==']'){
			loopdepth--;
			if (skip){skip--;continue;}
			
			//Return
			i = loopqueue[loopdepth]-1;continue;
			
		}
		else if(skip){
			//printf("%c",'a'+skip);
			continue;
		}
		
		//Output
		else if(source[i]=='.')
			output+=tape[pointer];//putchar(tape[pointer]);
		else if(source[i]=='+')
			tape[pointer]++;
		else if(source[i]=='-')
			tape[pointer]--;
		else if(source[i]=='>'){
			pointer++;
			if(pointer>=tapesize){
				tapesize=pointer+1;
				tape = (char*)realloc(tape, sizeof(char)*tapesize);
				tape[pointer]=0;
			}
		}
		else if(source[i]=='<' && pointer>0){
			pointer--;
			if(pointer>100)printf("Error: Negative pointer");
		}
		//putchar(source[i]);
	}
	
	free(tape);
	return output;
}
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
				std::string raw =data.substr(a+1,data.length()-1);
				size_t f=raw.find("&lt;");
				while((f=raw.find("&lt;"))!=std::string::npos){
					raw=raw.replace(f,4, "<");
				}
				while((f=raw.find("&gt;"))!=std::string::npos){
					raw=raw.replace(f,4, ">");
				}
				cout<<raw<<endl;
				std::string message = bfi(raw);
				cout<<message<<endl;
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
Connection: close\r\nContent-Type: text/html; charset=UTF-8\r\n\
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
