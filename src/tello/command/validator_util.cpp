#include "validator_util.hpp"

#include <numeric>

string tello::validator::concat(vector<string> errors) {
    return std::accumulate(std::begin(errors), std::end(errors), string(),
        [](string& ss, string& s)
        {
            return ss.empty() ? s : ss + "\n\r" + s;
        });
}