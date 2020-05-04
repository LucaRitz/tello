#pragma once
#include "../response.hpp"

#include <string>

using std::string;

namespace tello {

    class QueryResponse : public Response {
    public:
        QueryResponse() : Response(Status::UNKNOWN), _value(-1) {}
        explicit QueryResponse(const string& value);
        explicit QueryResponse(const Status& status);

        [[nodiscard]] int value() const;

    private:
        int _value;

        static int convert(const string& value);
    };
}