#include "FileFetcher.h"
#include "path.h"

namespace Goat{
	FileFetcher::FileFetcher(const std::string& path){
		setPath(path);
	}
	void FileFetcher::setPath(const std::string& path){
		this->path=path;
	}
	std::string FileFetcher::getPath() const{
		return path;
	}
	
	InputFile FileFetcher::fetch(std::string ipath) const{
		// Get full path
		if(!makeFullPath(ipath))
			return InputFile(InputFile::INVALID_PATH);
		
		// Compare paths
		for(unsigned int i=0;i<path.length();i++)
			if (path[i]!=ipath[i])
				return InputFile(InputFile::FORBIDDEN);
		
		// Return file
		return InputFile(ipath);
	}
	
}