/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

// Nova.cpp : Defines the exported functions for the DLL application.
//
#define LIB_EXPORT __attribute__ ((visibility ("default")))

#include <ctime>
#include <string>
#include <sstream>
#include <utility>

using namespace std;

#include "dump.h"
#include "FneHelpers.h"
#include "Status.h"
#include "Nova.h"
#include "JNIHelper.h"
#include "IdentityClient.h"
#include "TraWrapper.h"
#include "UserData.h"
#include "Stamps.h"

#include "jni.h"
#include "com_flexera_schneider_fnesigner_Nova.h"

static TraWrapper tra;

static const Stamps stamps(
#ifdef ENABLE_DEBUG_MACROS
    "DEBUG"
#else
    "RELEASE"
#endif
);

extern "C" int cf_save_jni_field_aliases_good(tra_Data *const p) {
	DEBUG_PRINT("cf_save_jni_field_aliases_good %p", static_cast<void*>(p));
	try {
		auto*const pud = static_cast<UserData*>(tra_get_user_data(p));
        DEBUG_PRINT("user data %p", static_cast<void*>(pud));

        IdentityMessagePayload payload{};

        tra_call(tra, TRA_FUNCTION_GET_ALIAS_FROM_SET_ALIAS_1, pud, TRA_STRING_identity_field_name_ALIAS_5, TRA_VARIABLE_ax_ALIAS_97, &payload.identity_name);
        tra_call(tra, TRA_FUNCTION_GET_ALIAS_FROM_SET_ALIAS_2, pud, TRA_STRING_identity_ALIAS_5, TRA_VARIABLE_ax_ALIAS_96, &payload.identity_value);
        tra_call(tra, TRA_FUNCTION_GET_ALIAS_FROM_SET_ALIAS_3, pud, TRA_STRING_message_field_name_ALIAS_5, TRA_VARIABLE_ax_ALIAS_95, &payload.message_name);
        tra_call(tra, TRA_FUNCTION_GET_ALIAS_FROM_SET_ALIAS_4, pud, TRA_STRING_ok_ALIAS_5, TRA_VARIABLE_ax_ALIAS_94, &payload.message_value);

        pud->set_identity_message_values(payload);

        pud->set_status(TRA_VARIABLE_one_ALIAS_1);

		return 1;
	}
	catch (...) {
	    DEBUG_PRINTLN("exception");
	    return -1;
	}
}

extern "C" int cf_save_jni_field_aliases_bad(tra_Data *const p) {
	DEBUG_PRINT("cf_save_jni_field_aliases_bad %p", static_cast<void*>(p));
	try {
		auto*const pud = static_cast<UserData*>(tra_get_user_data(p));
        DEBUG_PRINT("user data %p", static_cast<void*>(pud));

        IdentityMessagePayload payload{};

        tra_call(tra, TRA_FUNCTION_GET_ALIAS_FROM_SET_ALIAS_5, pud, TRA_STRING_identity_field_name_ALIAS_4, TRA_VARIABLE_ax_ALIAS_87, &payload.identity_name);
        tra_call(tra, TRA_FUNCTION_GET_ALIAS_FROM_SET_ALIAS_6, pud, TRA_STRING_identity_bad_ALIAS_4, TRA_VARIABLE_ax_ALIAS_86, &payload.identity_value);
        tra_call(tra, TRA_FUNCTION_GET_ALIAS_FROM_SET_ALIAS_7, pud, TRA_STRING_message_field_name_ALIAS_4, TRA_VARIABLE_ax_ALIAS_85, &payload.message_name);
        tra_call(tra, TRA_FUNCTION_GET_ALIAS_FROM_SET_ALIAS_8, pud, TRA_STRING_tamper_detected_ALIAS_4, TRA_VARIABLE_ax_ALIAS_84, &payload.message_value);

        pud->set_identity_message_values(payload);

        pud->set_status(TRA_VARIABLE_zero_ALIAS_1);

		return 1;
	}
	catch (...) {
	    DEBUG_PRINTLN("exception");
	    return -1;
	}
}

extern "C" int do_initialize(tra_Data *const p) {
	DEBUG_PRINT("do_initialize %p", static_cast<void*>(p));

	try {
		auto*const pud = static_cast<UserData*>(tra_get_user_data(p));
        DEBUG_PRINT("user data %p", static_cast<void*>(pud));

        // invoke cf_save_jni_field_aliases_bad */
        tra_call(tra, TRA_FUNCTION_SAVE_FIELD_ALIASES_BAD_ALIAS_1, pud, TRA_VARIABLE_ax_ALIAS_3, TRA_VARIABLE_ax_ALIAS_4, pud->get_reply_address());

	    // license check would go here
	    return  TFT(tra, TRA_VARIABLE_zero_ALIAS_5) + TFT(tra, TRA_VARIABLE_one_ALIAS_5);
	}
	catch (...) {
	    DEBUG_PRINTLN("exception");
	    return -1;
	}
}

extern "C" int do_initialize_success(tra_Data*const p) {
	DEBUG_PRINT("do_initialize_success %p", static_cast<void*>(p));
	try {
		auto*const pud = static_cast<UserData*>(tra_get_user_data(p));
        DEBUG_PRINT("user data %p", static_cast<void*>(pud));

        // invoke cf_save_jni_field_aliases_success */
        tra_call(tra, TRA_FUNCTION_SAVE_FIELD_ALIASES_GOOD_ALIAS_1, pud, TRA_VARIABLE_ax_ALIAS_1, TRA_VARIABLE_ax_ALIAS_2, pud->get_reply_address());

	    return TFT(tra, TRA_VARIABLE_zero_ALIAS_4) + TFT(tra, TRA_VARIABLE_one_ALIAS_4);
	}
	catch (...) {
	    DEBUG_PRINTLN("exception");
	    return -1;
	}
}

extern "C" int do_initialize_fail(tra_Data*const p) {
	DEBUG_PRINT("do_initialize_fail %p", static_cast<void*>(p));
	try {
		auto*const pud = static_cast<UserData*>(tra_get_user_data(p));
        DEBUG_PRINT("user data %p", static_cast<void*>(pud));

        tra_call(tra, TRA_FUNCTION_SAVE_FIELD_ALIASES_BAD_ALIAS_2, pud, TRA_VARIABLE_ax_ALIAS_5, TRA_VARIABLE_ax_ALIAS_6, pud->get_reply_address());

	    return TFT(tra, TRA_VARIABLE_minus_one_ALIAS_3) + TFT(tra, TRA_VARIABLE_one_ALIAS_3);
	}
	catch (...) {
	    DEBUG_PRINTLN("exception");
	    return -1;
	}
}

extern "C" int cf_tamper_detected(tra_Data*const p) {
	DEBUG_PRINT("cf_tamper_detected %p", static_cast<void*>(p));
	try {
		auto*const pud = static_cast<UserData*>(tra_get_user_data(p));
        DEBUG_PRINT("user data %p", static_cast<void*>(pud));

	    return  1;
	}
	catch (...) {
	    DEBUG_PRINTLN("exception");
	    return -1;
	}
}

extern "C" LIB_EXPORT jboolean JNICALL Java_com_flexera_schneider_fnesigner_Nova_initialize(JNIEnv *env, jobject object) {

	DEBUG_PRINTLN("Java_com_flexera_schneider_fnesigner_Nova_initialize")

    const JNIHelper jvm(env, object);

    try {
    	UserData userdata(tra);
        DEBUG_PRINT("user data %p", static_cast<void*>(&userdata));

        const auto reply = userdata.get_reply_address();

        tra_call(tra, TRA_FUNCTION_CLEAR_ALIASES_ALIAS_1, &userdata, TRA_VARIABLE_ax_ALIAS_99, TRA_VARIABLE_ax_ALIAS_98, reply);

        tra_call(tra, TRA_FUNCTION_ADD_ALIAS_TO_SET_ALIAS_1, &userdata, TRA_STRING_identity_field_name_ALIAS_1, TRA_VARIABLE_ax_ALIAS_1, reply);
        tra_call(tra, TRA_FUNCTION_ADD_ALIAS_TO_SET_ALIAS_2, &userdata, TRA_STRING_identity_field_name_ALIAS_2, TRA_VARIABLE_ax_ALIAS_2, reply);
        tra_call(tra, TRA_FUNCTION_ADD_ALIAS_TO_SET_ALIAS_3, &userdata, TRA_STRING_identity_field_name_ALIAS_3, TRA_VARIABLE_ax_ALIAS_3, reply);

        tra_call(tra, TRA_FUNCTION_ADD_ALIAS_TO_SET_ALIAS_11, &userdata, TRA_STRING_identity_ALIAS_1, TRA_VARIABLE_ax_ALIAS_11, reply);
        tra_call(tra, TRA_FUNCTION_ADD_ALIAS_TO_SET_ALIAS_12, &userdata, TRA_STRING_identity_ALIAS_2, TRA_VARIABLE_ax_ALIAS_12, reply);
        tra_call(tra, TRA_FUNCTION_ADD_ALIAS_TO_SET_ALIAS_13, &userdata, TRA_STRING_identity_ALIAS_3, TRA_VARIABLE_ax_ALIAS_13, reply);

        tra_call(tra, TRA_FUNCTION_ADD_ALIAS_TO_SET_ALIAS_21, &userdata, TRA_STRING_identity_bad_ALIAS_1, TRA_VARIABLE_ax_ALIAS_21, reply);
        tra_call(tra, TRA_FUNCTION_ADD_ALIAS_TO_SET_ALIAS_22, &userdata, TRA_STRING_identity_bad_ALIAS_2, TRA_VARIABLE_ax_ALIAS_22, reply);
        tra_call(tra, TRA_FUNCTION_ADD_ALIAS_TO_SET_ALIAS_23, &userdata, TRA_STRING_identity_bad_ALIAS_3, TRA_VARIABLE_ax_ALIAS_23, reply);

        tra_call(tra, TRA_FUNCTION_ADD_ALIAS_TO_SET_ALIAS_31, &userdata, TRA_STRING_message_field_name_ALIAS_1, TRA_VARIABLE_ax_ALIAS_31, reply);
        tra_call(tra, TRA_FUNCTION_ADD_ALIAS_TO_SET_ALIAS_32, &userdata, TRA_STRING_message_field_name_ALIAS_2, TRA_VARIABLE_ax_ALIAS_32, reply);
        tra_call(tra, TRA_FUNCTION_ADD_ALIAS_TO_SET_ALIAS_33, &userdata, TRA_STRING_message_field_name_ALIAS_3, TRA_VARIABLE_ax_ALIAS_33, reply);

        tra_call(tra, TRA_FUNCTION_ADD_ALIAS_TO_SET_ALIAS_41, &userdata, TRA_STRING_ok_ALIAS_1, TRA_VARIABLE_ax_ALIAS_41, reply);
        tra_call(tra, TRA_FUNCTION_ADD_ALIAS_TO_SET_ALIAS_42, &userdata, TRA_STRING_ok_ALIAS_2, TRA_VARIABLE_ax_ALIAS_42, reply);
        tra_call(tra, TRA_FUNCTION_ADD_ALIAS_TO_SET_ALIAS_43, &userdata, TRA_STRING_ok_ALIAS_3, TRA_VARIABLE_ax_ALIAS_43, reply);

        tra_call(tra, TRA_FUNCTION_ADD_ALIAS_TO_SET_ALIAS_51, &userdata, TRA_STRING_tamper_detected_ALIAS_1, TRA_VARIABLE_ax_ALIAS_51, reply);
        tra_call(tra, TRA_FUNCTION_ADD_ALIAS_TO_SET_ALIAS_52, &userdata, TRA_STRING_tamper_detected_ALIAS_2, TRA_VARIABLE_ax_ALIAS_52, reply);
        tra_call(tra, TRA_FUNCTION_ADD_ALIAS_TO_SET_ALIAS_53, &userdata, TRA_STRING_tamper_detected_ALIAS_3, TRA_VARIABLE_ax_ALIAS_53, reply);

        tra_call(tra, TRA_FUNCTION_SHOW_ALIASES_ALIAS_1, &userdata, TRA_VARIABLE_ax_ALIAS_60, TRA_VARIABLE_ax_ALIAS_61, reply);
		return JNI_TRUE;
    }
    catch (const runtime_error&  err) {

    	DEBUG_PRINT("exception | %s", err.what())

        jvm.set_string_field(tra_get_string(tra, TRA_STRING_message_field_name_ALIAS_10), err.what());

        return JNI_FALSE;
    }
    catch (...) {

//        jvm.set_string_field("message", "exception");

        jvm.set_string_field(tra_get_string(tra, TRA_STRING_message_field_name_ALIAS_11), tra_get_string(tra, TRA_STRING_exception_ALIAS_11));
        return JNI_FALSE;
    }
}

extern "C" LIB_EXPORT jboolean JNICALL Java_com_flexera_schneider_fnesigner_Nova_version(JNIEnv *env, jobject object) {

	DEBUG_PRINTLN("Java_com_flexera_schneider_fnesigner_Nova_version")

    const JNIHelper jvm(env, object);

    try {
    	ErrorWrapper error;

    	Status status(error);
    	status["FlcErrorCreate"] = FlcErrorCreate(error);

    	LicensingWrapper licensing;
    	status["FlcLicensingCreate"] = FlcLicensingCreate(licensing, identity_data, sizeof identity_data, nullptr, nullptr, error);

    	status["FlcSetVmDetectionEnabled"] =  FlcSetVmDetectionEnabled(licensing, FLC_TRUE, error);

    	const FlcChar*fneVersion;
    	status["FlcGetLicensingVersion"] = FlcGetLicensingVersion(licensing, &fneVersion, error);

    	const FlcChar*fneClientVersion;
    	status["FlcGetClientVersion"] = FlcGetClientVersion(licensing, &fneClientVersion, error);

    	// get host IDs
    	HostIdsWrapper hostids;
    	status["FlcGetHostIds"] = FlcGetHostIds(licensing, hostids, error);

    	FlcUInt32 size;
    	status["FlcHostIdsGetIdCount"] = FlcHostIdsGetIdCount(hostids, &size, error);

    	DEBUG_PRINTLN("Host Ids");
    	for (FlcUInt32 i = 0; i < size; i++) {
    		FlcInt32 type;
    		const FlcChar* value;
    		status["FlcHostIdsGetId"] =  FlcHostIdsGetId(hostids, i, &type, &value, error);

            DEBUG_PRINT("%2.2d | %s | %s", type, FneUtils::get_host_id_type(static_cast<FlcHostIdType>(type)), value);
    	}


//    	status["force fail"] = FLC_FALSE;

        jvm.set_string_field(tra_get_string(tra, TRA_STRING_fne_toolkit_version_ALIAS_1), fneVersion);

        jvm.set_string_field(tra_get_string(tra, TRA_STRING_native_library_version_ALIAS_1), stamps.get_timestamp());

        jvm.set_string_field(tra_get_string(tra, TRA_STRING_compiler_version_ALIAS_1), stamps.get_gnu_version());

        jvm.set_string_field(tra_get_string(tra, TRA_STRING_tra_version_ALIAS_1), stamps.get_tra_version());

        jvm.set_string_field(tra_get_string(tra, TRA_STRING_identity_secret_field_name_ALIAS_2), tra_get_string(tra, TRA_STRING_identity_secret_ALIAS_2));

		return JNI_TRUE;
    }
    catch (const runtime_error&  err) {

    	DEBUG_PRINT("exception | %s", err.what())

        jvm.set_string_field(tra_get_string(tra, TRA_STRING_message_field_name_ALIAS_13), err.what());

        return JNI_FALSE;
    }
    catch (...) {

//        jvm.set_string_field("message", "exception");

        jvm.set_string_field(tra_get_string(tra, TRA_STRING_message_field_name_ALIAS_13), tra_get_string(tra, TRA_STRING_exception_ALIAS_13));
        return JNI_FALSE;
    }
}

extern "C" LIB_EXPORT jboolean JNICALL Java_com_flexera_schneider_fnesigner_Nova_process(JNIEnv *env, jobject object) {

	DEBUG_PRINTLN("Java_com_flexera_schneider_fnesigner_Nova_process")

    const JNIHelper jvm(env, object);

    try {
    	UserData userdata(tra);

        DEBUG_PRINT("user data %p", static_cast<void*>(&userdata));

    	tra_if(tra, TRA_SNIF_initialize_ALIAS_1, &userdata);

    	/** set up the JNI fields **/

        jvm.set_string_field(userdata.get_identity_name(),userdata.get_identity_value());

        jvm.set_string_field(userdata.get_message_name(),userdata.get_message_value());

        userdata.release_all_strings();

        return TFT(tra, TRA_VARIABLE_zero_ALIAS_1) + userdata.get_status();
    }
    catch (const runtime_error&  err) {

    	DEBUG_PRINT("exception | %s", err.what())

        jvm.set_string_field(tra_get_string(tra, TRA_STRING_message_field_name_ALIAS_14), err.what());

        return JNI_FALSE;
    }
    catch (...) {

//        jvm.set_string_field("message", "exception");

        jvm.set_string_field(tra_get_string(tra, TRA_STRING_message_field_name_ALIAS_15), tra_get_string(tra, TRA_STRING_exception_ALIAS_15));
        return JNI_FALSE;
    }
}

class Success final {
    const std::string message;
public:
    explicit Success(std::string message) : message(std::move(message)) {

    }
    ~Success() = default;

    const char* get_message() const {
        return message.c_str();
    }
};

/** TEST HARNESS **/
static void caption(const string& message, const bool underline=true) {
	cout << endl;
	cout << message << endl;
	if (underline) {
		cout << string(message.length(), '-') << endl;
	}
}

static void prints(const string& message, const string&data) {
	cout << message << " | "  << data << endl;
}

static void printl(const string& message, const long data) {
  cout << message << " | " << dec << data << endl;
}

static void printp(const string& message, const void* data) {
  cout << message << " | " << hex << data << endl;
}

static void prints(const string& message, const string&data1, const string&data2) {
	cout << message << " | "  << data1 << " | "  << data2 << endl;
}

extern "C" LIB_EXPORT int schneider_nova_jni_test() {

  caption(__FUNCTION__);
  printp("Address", reinterpret_cast<void*>(&schneider_nova_jni_test));
  prints("File   ", __FILE__);
  printl("Line   ", __LINE__);

  tra.enable();

//  cout << __FUNCTION__ << "(" << reinterpret_cast<void*>(&schneider_nova_jni_test) << ") " << __FILE__ << " @ " << __LINE__ << endl;

  try {
    caption("NovaJni");
    prints("Version    ", stamps.get_timestamp());
    prints("TRA version", stamps.get_tra_version());
    prints("GNU version", stamps.get_gnu_version());

    caption("TRA load test");
    UserData userdata(tra);
    cout << "User data " << &userdata << endl;

    int value;
    caption("TRA integer tests");
    tra_get_value(tra, TRA_VARIABLE_zero_ALIAS_1, &value);
    printl("a", value);
    tra_get_value(tra, TRA_VARIABLE_one_ALIAS_1, &value);
    printl("b", value);
    tra_get_value(tra, TRA_VARIABLE_minus_one_ALIAS_1, &value);
    printl("c", value);
    tra_get_value(tra, TRA_VARIABLE_ax_ALIAS_1, &value);
    printl("d", value);

    caption("TRA string tests");
    prints("a", tra_get_string(tra, TRA_STRING_ok_ALIAS_1));
    prints("b", tra_get_string(tra, TRA_STRING_identity_field_name_ALIAS_1));
    prints("c", tra_get_string(tra, TRA_STRING_compiler_version_ALIAS_1));
    prints("d", tra_get_string(tra, TRA_STRING_message_field_name_ALIAS_1));
    prints("e", tra_get_string(tra, TRA_STRING_exception_ALIAS_1));
    prints("f", tra_get_string(tra, TRA_STRING_native_library_version_ALIAS_1));
    prints("g", tra_get_string(tra, TRA_STRING_tra_version_ALIAS_1));
    prints("h", tra_get_string(tra, TRA_STRING_fne_toolkit_version_ALIAS_1));
    prints("i", tra_get_string(tra, TRA_STRING_tamper_detected_ALIAS_1));

    caption("FNE tests");

    ErrorWrapper error;

    Status status(error);
    status["FlcErrorCreate"] = FlcErrorCreate(error);

    LicensingWrapper licensing;
    status["FlcLicensingCreate"] = FlcLicensingCreate(licensing, identity_data, sizeof identity_data, nullptr, nullptr,
        error);

    status["FlcSetVmDetectionEnabled"] = FlcSetVmDetectionEnabled(licensing, FLC_TRUE, error);

    const FlcChar *fneVersion;
    status["FlcGetLicensingVersion"] = FlcGetLicensingVersion(licensing, &fneVersion, error);
    prints("FNE version", fneVersion);

    const FlcChar *fneClientVersion;
    status["FlcGetClientVersion"] = FlcGetClientVersion(licensing, &fneClientVersion, error);
    prints("FNE client version", fneVersion);

    // get host IDs
    HostIdsWrapper hostids;
    status["FlcGetHostIds"] = FlcGetHostIds(licensing, hostids, error);

    FlcUInt32 size;
    status["FlcHostIdsGetIdCount"] = FlcHostIdsGetIdCount(hostids, &size, error);

    caption("Available Hosts");
    for (FlcUInt32 i = 0; i < size; i++) {
      FlcInt32 type;
      const FlcChar *value;
      status["FlcHostIdsGetId"] = FlcHostIdsGetId(hostids, i, &type, &value, error);

      prints(to_string(i), FneUtils::get_host_id_type(static_cast<FlcHostIdType>(type)), value);
    }

    cout << endl;

    throw Success("All tests passed successfully");

    return 0;
  }
  catch (const Success &err) {

    caption("Success handler", false);
    cout << err.get_message() << endl;

    return 0;
  }
  catch (const runtime_error &err) {

    caption("Error handler", false);
    cout << "exception | " << err.what() << endl;

    return -1;
  }
  catch (...) {

    caption("Exception handler", false);
    cout << "unexpected exception..." << endl;

    return -2;
  }
}

