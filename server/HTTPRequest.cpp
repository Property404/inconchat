#include "HTTPRequest.h"

namespace Goat{
HTTPRequest::HTTPRequest(const std::string& data){
	formFromData(data);
}
std::string HTTPRequest::getMethod() const{
	std::string t = getTopLine();
	return t.substr(0,t.find(" "));
}
std::string HTTPRequest::getResourceName() const{
	std::string t = getTopLine();
	return t.substr(t.find(" ")+1,t.rfind(" ")-t.find(" "));
}

}