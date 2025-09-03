#ifndef USER_DATA_H
#define USER_DATA_H

#include "dump.h"

#include "TraWrapper.h"

#include "FlcLicensing.h"
#include "FlcLicenseManager.h"
#include "FlcFeature.h"

#include "fnedemo.RSA512.IdentityClient.h"

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
//
//	int&name() {
//		return m_name;
//	}
//	int&value() {
//		return m_value;
//	}
};

class UserData {
    TraWrapper &m_tra;

    FlcErrorRef m_error;
    FlcLicensingRef m_licensing;
    FlcLicenseRef m_license;

public:
    std::string error;
    std::string path;

    FieldWrapper identity;
    FieldWrapper message;
    TFT status;

    UserData(TraWrapper &value) : m_tra(value), m_error(0), m_licensing(0), m_license(0), identity(value), message(value), status(value, TRA_VARIABLE_minus_one_ALIAS_1){
    }

    virtual ~UserData();

    /**
     * for external testing only - not part of JNI functionality
     */
    int test_initialize();

    /**
     * for external testing only - not part of JNI functionality
     */
    int test_dump(std::stringstream &stream);

};

#endif
