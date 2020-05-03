#pragma once
#include "../response.hpp"

#include <string>

using std::string;

namespace tello {

    class QueryResponse : public Response {
    public:
        explicit QueryResponse(const Status& status);

        [[nodiscard]] int value() const;

        static shared_ptr<QueryResponse> error();
        static shared_ptr<QueryResponse> timeout();
        static shared_ptr<QueryResponse> empty();

        void update(const string& value) override;
    private:
        int _value;

        static int convert(const string& value);
    };
}