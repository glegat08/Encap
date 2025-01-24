#pragma once
#include <string>
#include <sstream>
#include <algorithm>

class Console;

namespace UseThat
{
    std::string removeExtraSpaces(std::string str);
    std::string getNameWithSpaces(std::istringstream& iss);
}