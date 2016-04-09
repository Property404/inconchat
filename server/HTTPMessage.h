#pragma once
#include <string>

namespace Goat{
/*
	HTTPMessage "abstract"(hidden constructor) class
	Meant to partially form the classes
	HTTPRequest and HTTPResponse along with Goat::Request
	and Goat::Response, respectively
*/
class HTTPMessage{
private:
	std::string topline; //"GET /index.html HTTP/1.1" or "HTTP/1.1 200 OK"
	std::string header;
	std::string body;
protected:
	// Make data into topline, header, and body
	void formFromData(const std::string&);
	
	// Request line or status line
	std::string getTopLine() const;
	HTTPMessage();
public:
	// Getters
	std::string getHeader() const;
	std::string getBody() const;
	std::string getFromHeader(const std::string&) const;// Get attribute from header
	std::string toString() const;//Output request as string
	// Setters
	/* Write this shit later*/
	
};
}