#include "UseThatHelpingFunction.h"
#include <string>
#include "Console.h"

std::string UseThat::getNameWithSpaces(std::istringstream& iss)
{
    std::string word;
    iss >> word;

    if (word.empty())
        return "";

    std::string result = word;

    // If there is a quoted string, read until the closing quote
    if (!word.empty() && word[0] == '"') 
    {
        result = word.substr(1);  // It remove the opening quote
        char nextChar;

        // Read char by char until we find the closing quote
        while (iss.get(nextChar)) 
        {
            if (nextChar == '"') 
                break;

            result += nextChar;
        }
    }

    return removeExtraSpaces(result);
}

std::string UseThat::removeExtraSpaces(std::string str)
{
    str.erase(0, str.find_first_not_of(" "));
    str.erase(str.find_last_not_of(" ") + 1);

	// Put unique values at the beginning of the string
	// And erase the duplicates placed after those unique values
    auto new_end = std::unique(str.begin(), str.end(), [](char left, char right) 
        {
        return left == ' ' && right == ' ';
        });
    str.erase(new_end, str.end());

    return str;
}