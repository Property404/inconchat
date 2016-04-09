#pragma once
#include <string>
namespace Goat{
// Make short path into full path
// Returns 1 on success, 0 on fail
bool makeFullPath(std::string& path);
}