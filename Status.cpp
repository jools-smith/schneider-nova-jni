#include "Status.h"
#include "dump.h"

#include <stdexcept>
#include <iostream>

using namespace std;

Status& Status::operator[](const std::string& where) {
    method = where;

    return *this;
}

Status& Status::operator=(const FlcBool value) {
    status = value;

    if (status != FLC_TRUE) {
    	const auto message = FlcErrorGetMessage(error);

    	DEBUG_PRINTLN(message);

        throw runtime_error(method + " | " + message);
    }
    return *this;
}
