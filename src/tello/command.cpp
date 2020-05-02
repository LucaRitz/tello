#include <tello/command.hpp>
#include <tello/response/status_response.hpp>

#define DEFAULT_FORECAST 1000

using tello::StatusResponse;

tello::Command::Command(const CommandType& commandType) : _type(commandType) {}

string tello::Command::validate() const {
    return string{};
}

t_forecast tello::Command::forecast() const {
    return DEFAULT_FORECAST;
}

t_forecast tello::Command::forecast(const StatusResponse& status) const {
    return forecast();
}