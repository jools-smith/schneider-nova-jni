#include "FneHelpers.h"

using namespace std;

const char* FneUtils::get_host_id_type(const FlcHostIdType type) {
	switch (type) {
	case FLC_HOSTID_TYPE_UNKNOWN:
		return "Unknown";
	case FLC_HOSTID_TYPE_UNSUPPORTED:
		return "Unsupported";
	case FLC_HOSTID_TYPE_LONGHOSTID:
		return "Long";
	case FLC_HOSTID_TYPE_ETHERNET:
		return "Ethernet";
	case FLC_HOSTID_TYPE_ANY:
		return "Any";
	case FLC_HOSTID_TYPE_USER:
		return "User";
	case FLC_HOSTID_TYPE_DISPLAY:
		return "Display";
	case FLC_HOSTID_TYPE_HOSTNAME:
		return "Host Name";
	case FLC_HOSTID_TYPE_STRING:
		return "String";
	case FLC_HOSTID_TYPE_FLEXID7:
		return "FlexID 7";
	case FLC_HOSTID_TYPE_VSN:
		return " Volume Serial Number";
	case FLC_HOSTID_TYPE_INTERNET:
		return "Internet";
	case FLC_HOSTID_TYPE_INTERNET6:
		return "Internet IPV6";
	case FLC_HOSTID_TYPE_FLEXID8:
		return "FlexID 8";
	case FLC_HOSTID_TYPE_FLEXID9:
		return "FlexID 9";
	case FLC_HOSTID_TYPE_HOSTDOMAIN:
		return "Host Domain";
	case FLC_HOSTID_TYPE_FLEXID6:
		return "FlexID 6";
	case FLC_HOSTID_TYPE_COMPOSITE:
		return "Composite";
	case FLC_HOSTID_TYPE_VENDOR:
		return "Vendor Defined";
	case FLC_HOSTID_TYPE_FLEXID10:
		return "FlexID 10";
	case FLC_HOSTID_TYPE_VM_UUID:
		return "VM UUID";
	case FLC_HOSTID_TYPE_AMAZON_EIP:
		return "Amazon EIP";
	case FLC_HOSTID_TYPE_AMAZON_AMI:
		return "Amazon AMI ";
	case FLC_HOSTID_TYPE_TOLERANT:
		return "Tolerant";
	case FLC_HOSTID_TYPE_AMAZON_IID:
		return "Amazon IID";
	case FLC_HOSTID_TYPE_EXTENDED:
		return "Extended";
	case FLC_HOSTID_TYPE_PUBLISHER_DEFINED:
		return "Publisher Defined";
	case FLC_HOSTID_TYPE_CONTAINER_ID:
		return "Docker container ID ";
	case FLC_HOSTID_NEXT:
		return "Next";
	default:
		return "????";
	}
}

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
