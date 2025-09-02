#ifndef USER_DATA_H
#define USER_DATA_H

#include "dump.h"

#include "TraWrapper.h"

#include "FlcLicensing.h"
#include "FlcLicenseManager.h"
#include "FlcFeature.h"
//#include "Nova.IdentityClient.h"
#include "fnedemo.RSA512.IdentityClient.h"

#include <string>
#include <vector>
#include <sstream>

class UserData {
    TraWrapper &m_tra;

    FlcErrorRef m_error;
    FlcLicensingRef m_licensing;
    FlcLicenseRef m_license;

public:
    std::string error;
    std::string path;

    UserData(TraWrapper &value) : m_tra(value), m_error(0), m_licensing(0), m_license(0) {
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
