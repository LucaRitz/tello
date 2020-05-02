#pragma once

#include <string>
#include <vector>
#include <numeric>

using std::string;
using std::vector;

namespace tello::validator {

    template<class type>
    string between(const type& argument, type lowerBound, type upperBound, const string& argName) {
        try {
            if (argument < lowerBound || argument > upperBound) {
                return string("Value of '") + argName + string("' has to be between 20 and 500\n\r");
            }

            return string("");
        } catch (const std::invalid_argument& exc) {
            return string("Value of '") + argName + string("' has to be between '") + std::to_string(lowerBound) +
                   string("' and '") + std::to_string(upperBound) + string("'\n\r");
        } catch (const std::out_of_range& exc) {
            return string("Value of '") + argName + string("' has to be between '") + std::to_string(lowerBound) +
                   string("' and '") + std::to_string(upperBound) + string("'\n\r");
        }
    }

    string concat(vector<string> errors) {
        return std::accumulate(std::begin(errors), std::end(errors), string(),
                                   [](string &ss, string &s)
                                   {
                                       return ss.empty() ? s : ss + "\n\r" + s;
                                   });
    }
}