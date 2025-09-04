/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

// Nova.cpp : Defines the exported functions for the DLL application.
//
#define LIB_EXPORT __attribute__ ((visibility ("default")))

#include <ctime>
#include <time.h>
#include <string>
#include <sstream>
#include <set>
#include <vector>

using namespace std;

#include "dump.h"
#include "FneHelpers.h"
#include "Nova.h"
#include "JNIHelper.h"
#include "IdentityClient.h"
#include "TraWrapper.h"

#include "UserData.h"

#include "jni.h"
#include "com_flexera_schneider_fnesigner_Nova.h"

/*!
 * Wrap tra_State pointer for lazy initialization
 */

static TraWrapper tra;

static const string datestamp = __DATE__;
static const string timestamp = __TIME__;

static int gnuc = __GNUC__;
static int gnuc_minor = __GNUC_MINOR__;
static int gnuc_patch = __GNUC_PATCHLEVEL__;

extern "C" {

int cf_save_jni_field_aliases_good(tra_Data *p) {
	DEBUG_PRINT("cf_save_jni_field_aliases_good %p", (void*)p);
	try {
		UserData*const pud = static_cast<UserData*>(tra_get_user_data(p));
        DEBUG_PRINT("user data %p", (void*)pud);

		pud->identity.name = TRA_STRING_identity_field_name_ALIAS_2;
		pud->identity.value = TRA_STRING_identity_ALIAS_1;
		pud->message.name = TRA_STRING_message_field_name_ALIAS_2;
		pud->message.value = TRA_STRING_ok_ALIAS_1;
		pud->status = TFT(tra, TRA_VARIABLE_one_ALIAS_1);

		return 1;
	}
	catch (...) {
	    DEBUG_PRINTLN("exception");
	    return -1;
	}
}

int cf_save_jni_field_aliases_bad(tra_Data *p) {
	DEBUG_PRINT("cf_save_jni_field_aliases_bad %p", (void*)p);
	try {
		UserData*const pud = static_cast<UserData*>(tra_get_user_data(p));
        DEBUG_PRINT("user data %p", (void*)pud);

		pud->identity.name = TRA_STRING_identity_field_name_ALIAS_1;
		pud->identity.value = TRA_STRING_identity_bad_ALIAS_1;
		pud->message.name = TRA_STRING_message_field_name_ALIAS_1;
		pud->message.value = TRA_STRING_tamper_detected_ALIAS_1;
		pud->status = TFT(tra, TRA_VARIABLE_zero_ALIAS_1);

		return 1;
	}
	catch (...) {
	    DEBUG_PRINTLN("exception");
	    return -1;
	}
}
// TODO - need to include license check at some point
int do_initialize(tra_Data *p) {
	DEBUG_PRINT("do_initialize %p", (void*)p);

	try {
		UserData*const pud = static_cast<UserData*>(tra_get_user_data(p));
        DEBUG_PRINT("user data %p", (void*)pud);

        tra_call(tra, TRA_FUNCTION_SAVE_FIELD_ALIASES_BAD_ALIAS_1, pud, TRA_VARIABLE_ax_ALIAS_3, TRA_VARIABLE_ax_ALIAS_4, nullptr);

//		pud->identity.name = TRA_STRING_identity_field_name_ALIAS_1;
//		pud->identity.value = TRA_STRING_identity_bad_ALIAS_1;
//		pud->message.name = TRA_STRING_message_field_name_ALIAS_1;
//		pud->message.value = TRA_STRING_tamper_detected_ALIAS_1;
//		pud->status = TFT(tra, TRA_VARIABLE_zero_ALIAS_1);

	    // license check would go here
	    return  TFT(tra, TRA_VARIABLE_zero_ALIAS_12) + TFT(tra, TRA_VARIABLE_one_ALIAS_12);
	}
	catch (...) {
	    DEBUG_PRINTLN("exception");
	    return -1;
	}
}

int do_initialize_success(tra_Data*const p) {
	DEBUG_PRINT("do_initialize_success %p", (void*)p);
	try {
		UserData*const pud = static_cast<UserData*>(tra_get_user_data(p));
        DEBUG_PRINT("user data %p", (void*)pud);

        tra_call(tra, TRA_FUNCTION_SAVE_FIELD_ALIASES_GOOD_ALIAS_1, pud, TRA_VARIABLE_ax_ALIAS_1, TRA_VARIABLE_ax_ALIAS_2, nullptr);

//		pud->identity.name = TRA_STRING_identity_field_name_ALIAS_2;
//		pud->identity.value = TRA_STRING_identity_ALIAS_1;
//		pud->message.name = TRA_STRING_message_field_name_ALIAS_2;
//		pud->message.value = TRA_STRING_ok_ALIAS_1;
//		pud->status = TFT(tra, TRA_VARIABLE_one_ALIAS_1);

	    return  TFT(tra, TRA_VARIABLE_zero_ALIAS_13) + TFT(tra, TRA_VARIABLE_one_ALIAS_9);
	}
	catch (...) {
	    DEBUG_PRINTLN("exception");
	    return -1;
	}
}

int do_initialize_fail(tra_Data*p) {
	DEBUG_PRINT("do_initialize_fail %p", (void*)p);
	try {
		UserData*const pud = static_cast<UserData*>(tra_get_user_data(p));
        DEBUG_PRINT("user data %p", (void*)pud);

        tra_call(tra, TRA_FUNCTION_SAVE_FIELD_ALIASES_BAD_ALIAS_1, pud, TRA_VARIABLE_ax_ALIAS_5, TRA_VARIABLE_ax_ALIAS_6, nullptr);

//		pud->identity.name = TRA_STRING_identity_field_name_ALIAS_3;
//		pud->identity.value = TRA_STRING_identity_bad_ALIAS_2;
//		pud->message.name = TRA_STRING_message_field_name_ALIAS_3;
//		pud->message.value = TRA_STRING_tamper_detected_ALIAS_2;
//		pud->status = TFT(tra, TRA_VARIABLE_zero_ALIAS_2);

	    return  TFT(tra, TRA_VARIABLE_minus_one_ALIAS_13) + TFT(tra, TRA_VARIABLE_one_ALIAS_7);
	}
	catch (...) {
	    DEBUG_PRINTLN("exception");
	    return -1;
	}
}

int cf_tamper_detected(tra_Data*p) {
	DEBUG_PRINT("cf_tamper_detected %p", (void*)p);
	try {
		UserData*const pud = static_cast<UserData*>(tra_get_user_data(p));
        DEBUG_PRINT("user data %p", (void*)pud);

	    return  1;
	}
	catch (...) {
	    DEBUG_PRINTLN("exception");
	    return -1;
	}
}

LIB_EXPORT jboolean JNICALL Java_com_flexera_schneider_fnesigner_Nova_version(JNIEnv *env, jobject object) {

	DEBUG_PRINTLN("Java_com_flexera_schneider_fnesigner_Nova_version")

    const JNIHelper jvm(env, object);

    try {
    	ErrorWrapper error;

    	Status status(error);
    	status << "FlcErrorCreate" << FlcErrorCreate(error);

    	LicensingWrapper licensing;
    	status["FlcLicensingCreate"] = FlcLicensingCreate(licensing, identity_data, sizeof identity_data, nullptr, nullptr, error);

    	const FlcChar*fneVersion;
    	status = FlcGetLicensingVersion(licensing, &fneVersion, error);

        jvm.set_string_field("fneToolkitVersion", fneVersion);

    	struct tm time;
    	strptime(datestamp.c_str(), "%b %e %Y", &time);

    	char bfr[90];

    	strftime(bfr, sizeof bfr, "%Y-%m-%d", &time);
        jvm.set_string_field("nativeLibraryVersion", string(bfr) + " " + timestamp);

        snprintf(bfr, sizeof bfr, "%d.%d.%d", gnuc, gnuc_minor, gnuc_patch);
        jvm.set_string_field("compilerVersion", bfr);

		return JNI_TRUE;
    }
    catch (const runtime_error& err) {

        jvm.set_string_field("message", err.what());

        cout << "exception | " << err.what() << endl;

        return JNI_FALSE;
    }
}


LIB_EXPORT jboolean JNICALL Java_com_flexera_schneider_fnesigner_Nova_process(JNIEnv *env, jobject object) {

	DEBUG_PRINTLN("Java_com_flexera_schneider_fnesigner_Nova_process")

    const JNIHelper jvm(env, object);

    try {
    	UserData userdata(tra);

        DEBUG_PRINT("user data %p", (void*)&userdata);

    	tra_if(tra, TRA_SNIF_initialize_ALIAS_1, &userdata);

    	/** set up the JNI fields **/

        jvm.set_string_field(
        		tra_get_string(tra, userdata.identity.name),
				tra_get_string(tra, userdata.identity.value));

        tra_release_string(tra, userdata.identity.value);

        jvm.set_string_field(
        		tra_get_string(tra, userdata.message.name),
				tra_get_string(tra, userdata.message.value));

        return TFT(tra, TRA_VARIABLE_zero_ALIAS_1) + userdata.status;
    }
    catch (const runtime_error& err) {

        jvm.set_string_field("message", err.what());

        cout << "exception | " << err.what() << endl;

        return JNI_FALSE;
    }
}

}
/* extern c */
