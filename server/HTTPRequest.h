#pragma once
#include "HTTPMessage.h"

namespace Goat{
/*
	The HTTPRequest derived class 
	derive from HTTPMessage
*/
class HTTPRequest : public HTTPMessage{
public:	
	//Constructors
	// Call HTTPMessage::formFromData
	HTTPRequest(const std::string& data);
	
	// New getters (get from the request line)
	std::string getHTTPVersion() const; //e.g. 1.1
	std::string getResourceName() const; //e.g. /usr/bin/butts
	std::string getMethod() const; //e.g, GET, POST
};
}