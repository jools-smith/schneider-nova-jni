#pragma once

#include "FneHelpers.h"

#include <string>


class Status {
	ErrorWrapper&error;
    std::string method;
    FlcBool status;

public:
    explicit Status(ErrorWrapper& err) : error(err), status(FLC_TRUE) {
    }

    Status& operator[] (const std::string& where);

    Status& operator= (const FlcBool value);

    Status& operator<< (const std::string& where) {
        return operator[](where);
    }

    Status& operator<< (FlcBool value) {
        return operator=(value);
    }
};

