#pragma once
#include "../response.hpp"

#include <string>

using std::string;

namespace tello {

    class QueryResponse : public Response {
    public:
        explicit QueryResponse(const string& response);
        explicit QueryResponse(const Status& status);

        [[nodiscard]] int value() const;

        static unique_ptr<QueryResponse> error();
        static unique_ptr<QueryResponse> timeout();
        static unique_ptr<QueryResponse> of(const string& arg);
    private:
        int _value;

        static int convert(const string& value);
    };
}