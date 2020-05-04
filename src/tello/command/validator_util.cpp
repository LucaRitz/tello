#include "validator_util.hpp"

#include <numeric>

string tello::validator::concat(vector<string> errors) {
    return std::accumulate(std::begin(errors), std::end(errors), string(),
        [](string& ss, string& s)
        {
            return ss.empty() ? s : ss + "\n\r" + s;
        });
}

string tello::validator::isHorizontalDirection(const char& direction, const string& argName)
{
    switch (direction)
    {
	    case 'l':
	    case 'r':
	    case 'f':
	    case 'b':
	        return string("");

	    default:
	        return string("Value of '") + argName + string("' has to be one of them ['l', 'r', 'f', 'b']\n\r");
    }
}
