#include "InputFile.h"
#include <vector>

namespace Goat{
/*
	Fetches files on a particular folder in the server
	Part of making a file server
*/
class FileFetcher{
	std::string path; //Path where files can be accesseed
	std::vector<std::string> blacklist; //Paths/files that cannot be accessed
	
public: 
	FileFetcher(const std::string& path);
	
	// Setters
	void setPath(const std::string&	path);
	void pushBlackList(const std::string& path);
	
	//Getters
	std::string getPath() const;
	
	InputFile fetch(std::string path) const;
	
};
}