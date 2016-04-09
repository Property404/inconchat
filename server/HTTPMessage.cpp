#include "HTTPMessage.h"

namespace Goat {
HTTPMessage::HTTPMessage(){
	topline = "";
	header = "";
	body = "";
}
void HTTPMessage::formFromData(const std::string& data){
	// Empty field
	topline = data.substr(0,data.find("\r\n"));
	header = data.substr(data.find("\r\n")+2,data.find("\r\n\r\n")-data.find("\r\n")-2);
	body = data.substr(data.find("\r\n\r\n"), data.length()-1-data.find("\r\n\r\n"));
}

std::string HTTPMessage::toString() const{
	return topline+"\r\n"+header+"\r\n\r\n"+body;
}
std::string HTTPMessage::getBody() const{return body;}
std::string HTTPMessage::getHeader() const{return header;}
std::string HTTPMessage::getTopLine() const{return topline;}


	
}