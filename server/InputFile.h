#pragma once
#include <string>

namespace Goat{
/*
	A hopefully thread safe input file class
*/
class InputFile{
private:
	// File definition
	std::string fullpath;
	std::string contents;
	int status;
	
	// Load a file information into the object
	int load(const std::string& fname);
public:
	// Constructor (set status to load return value)
	InputFile(std::string fname);
	InputFile(int status);
	
	// Getters
	int getStatus() const;
	std::string getContents() const;
	std::string getFullPath() const;
	std::string getPath() const;
	std::string getExtension() const;
	std::string getName() const;
	
	// Status/error Codes
	const static int SUCCESS = 0;
	const static int FAILED_TO_OPEN = 10;
	const static int FAILED_TO_READ = 20;
	const static int INVALID_PATH = 40;
	const static int FORBIDDEN = 50;
};
}