#pragma once

#include <tello/response.hpp>

namespace tello {

    class StatusResponse : public Response {
    public:
        explicit StatusResponse(const string& response);
    };
}