#ifndef USER_DATA_H
#define USER_DATA_H

#include "dump.h"

#include "TraWrapper.h"

#include "FlcLicensing.h"
#include "FlcLicenseManager.h"
#include "FlcFeature.h"

#include <string>
#include <vector>
#include <sstream>

class FieldWrapper {
//	int m_name;
//	int m_value;
public:
    TFT name;
    TFT value;
	FieldWrapper(TraWrapper &value) : name(value, TRA_VARIABLE_minus_one_ALIAS_2), value(value, TRA_VARIABLE_minus_one_ALIAS_3) {

	}
};

class UserData final {
    TraWrapper &m_tra;

public:
    FieldWrapper identity;
    FieldWrapper message;
    TFT status;

    UserData(TraWrapper &value) : m_tra(value), identity(value), message(value), status(value, TRA_VARIABLE_minus_one_ALIAS_1){
    }

    ~UserData() = default;
};

#endif
