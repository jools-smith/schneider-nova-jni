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

class FieldWrapper final {

public:
    TFT name;
    TFT value;
	FieldWrapper(TraWrapper &value) : name(value, TRA_VARIABLE_minus_one_ALIAS_2), value(value, TRA_VARIABLE_minus_one_ALIAS_3) {

	}

	~FieldWrapper() = default;

	void reset() {
		name = TRA_VARIABLE_minus_one_ALIAS_4;
		value = TRA_VARIABLE_minus_one_ALIAS_5;
	}
};

class UserData final {
    TraWrapper &m_tra;

    int reply;

    FieldWrapper identity;
    FieldWrapper message;

    TFT status;
public:



    UserData(TraWrapper &value) : m_tra(value), reply(0), identity(value), message(value), status(value, TRA_VARIABLE_minus_one_ALIAS_1) {
    }

    void release_all_strings() {
    	tra_release_string(m_tra, identity.name);
    	tra_release_string(m_tra, identity.value);
    	identity.reset();

    	tra_release_string(m_tra, message.name);
    	tra_release_string(m_tra, message.value);
    	message.reset();
    }

    const char* get_identity_name() {
    	return tra_get_string(m_tra, identity.name);
    }

    const char* get_identity_value() {
    	return tra_get_string(m_tra, identity.value);
    }

    const char* get_message_name() {
    	return tra_get_string(m_tra, message.name);
    }

    const char* get_message_value() {
    	return tra_get_string(m_tra, message.value);
    }

    TFT& get_status() {
    	return status;
    }

    int* get_reply_address() {
    	return &reply;
    }

    void set_status(const TRA_VARIABLE_INDEX alias) {
    	status = TFT(m_tra, alias);
    }

    void set_identity_message_values(const TRA_STRING_INDEX alias_1, const TRA_STRING_INDEX alias_2, const TRA_STRING_INDEX alias_3, const TRA_STRING_INDEX alias_4) {
    	identity.name = alias_1;
    	identity.value = alias_2;
    	message.name = alias_3;
    	message.value = alias_4;
    }

    ~UserData() = default;
};

#endif
