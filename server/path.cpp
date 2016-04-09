#include "path.h"

namespace Goat{
bool makeFullPath(std::string& path){
	// Declare cpath temporary variable
	char cpath[_MAX_PATH];
	
	// Get full path
	if(_fullpath(cpath,path.c_str(),_MAX_PATH)==NULL)
		return 0;
	
	// Copy to path and replace backslashes with slashes
	path = "";
	for (int i=0;cpath[i]!='\0';i++)
		path+=(cpath[i]=='\\')?'/':cpath[i];
	path+='\0';
	return 1;
}
}