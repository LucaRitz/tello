#pragma once
#include "../response.hpp"

#include <string>

using std::string;

namespace tello {

    class QueryResponse : public Response {
    public:
        QueryResponse() : _value(-1) {}
        explicit QueryResponse(const Status& status);

        [[nodiscard]] int value() const;

        static QueryResponse error();
        static QueryResponse empty();

        void update(const string& value) override;
        void update(const Status& status);

    private:
        int _value;

        static int convert(const string& value);
    };
}