#include "FneHelpers.h"

using namespace std;

ErrorWrapper::ErrorWrapper() : FneWrapperBase() {
    DEBUG_PRINTLN("created ErrorWrapper");
}

ErrorWrapper::~ErrorWrapper() {
    if (object) {
        FlcErrorDelete(&object);

        DEBUG_PRINTLN("destroyed ErrorWrapper");
    }
}

LicensingWrapper::LicensingWrapper() : FneWrapperBase() {
    DEBUG_PRINTLN("created LicensingWrapper");
}

LicensingWrapper::~LicensingWrapper() {
    if (object) {
        FlcLicensingDelete(&object, nullptr);

        DEBUG_PRINTLN("destroyed LicensingWrapper");
    }
}

HostIdsWrapper::HostIdsWrapper() : FneWrapperBase() {
    DEBUG_PRINTLN("created HostIdsWrapper");
}

HostIdsWrapper::~HostIdsWrapper() {
    if (object) {
    	FlcHostIdsDelete(&object, nullptr);

        DEBUG_PRINTLN("destroyed HostIdsWrapper");
    }
}
