#include "InputFile.h"
#include "path.h"
#include <cstdio>
namespace Goat{
InputFile::InputFile(std::string fname){
	fullpath = "";
	contents = "";
	status=load(fname);
}
InputFile::InputFile(int status){
	this->status = status;
}

int InputFile::load(const std::string& fpath){
	// Get full path
	fullpath = fpath;
	if(!makeFullPath(fullpath))
		return InputFile::INVALID_PATH;
	
	// Open file
	FILE* fp=fopen(fpath.c_str(),"rb");
	
	// Check is file opened properly
	if(fp==NULL)
		return InputFile::FAILED_TO_OPEN;
	
	// Check size
	fseek(fp, 0, SEEK_END);
	long fp_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	if(fp_size == -1L)
		return InputFile::FAILED_TO_READ;
	
	// Read
	char* data = new char[fp_size];
	fread(data, 1, fp_size, fp);
	if (data == NULL)
		return InputFile::FAILED_TO_READ;
	
	// Close
	fclose(fp);
	
	// Copy to contents
	this->contents = std::string(data);
	delete[] data;
	
	// Success!
	return SUCCESS;
}

// Obvious getters
std::string InputFile::getFullPath() const{
	return fullpath;
}
std::string InputFile::getContents() const{
	return contents;
}
int InputFile::getStatus() const{
	return status;
}

// Other getters
std::string InputFile::getPath() const{
	return fullpath.substr(0,fullpath.rfind("/"));
}
std::string InputFile::getName() const{
	int slash = fullpath.rfind("/")+1;
	return fullpath.substr(slash, fullpath.length()-slash);
}
std::string InputFile::getExtension() const{
	std::string name = getName();
	int dot = name.find(".");
	return name.substr(dot, name.length()-dot);
}

}