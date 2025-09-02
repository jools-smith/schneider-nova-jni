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
#include <set>
#include <vector>

using namespace std;

#include "dump.h"

#include "Nova.h"

#include "TraWrapper.h"

#include "UserData.h"

#include "jni.h"
#include "com_flexera_schneider_fnesigner_Nova.h"

/*!
 * Wrap tra_State pointer for lazy initialization
 */

static TraWrapper tra;

extern "C" {

// TODO - need to include license check at some point
int do_initialize(tra_Data *ptr) {
    DEBUG_PRINTLN("do_initialize")

    TFT result(tra, TRA_VARIABLE_zero_ALIAS_12);

    // license check would go here
    result += TFT(tra, TRA_VARIABLE_one_ALIAS_12);

    return result;
}

int do_checkout(tra_Data*) {
    DEBUG_PRINTLN("do_checkout")

    tra_copy(tra, TRA_VARIABLE_status_ALIAS_1, TRA_VARIABLE_ax_ALIAS_2);
    tra_call(tra, TRA_SF_MULTIPLY_ALIAS_1, NULL, TRA_VARIABLE_status_ALIAS_2, TRA_VARIABLE_zero_ALIAS_1, NULL);
    tra_call(tra, TRA_SF_MULTIPLY_ALIAS_2, NULL, TRA_VARIABLE_status_ALIAS_3, TRA_VARIABLE_zero_ALIAS_2, NULL);
    tra_call(tra, TRA_SF_MULTIPLY_ALIAS_3, NULL, TRA_VARIABLE_status_ALIAS_4, TRA_VARIABLE_zero_ALIAS_3, NULL);

    tra_copy(tra, TRA_VARIABLE_status_ALIAS_5, TRA_VARIABLE_ax_ALIAS_3);
    tra_copy(tra, TRA_VARIABLE_status_ALIAS_5, TRA_VARIABLE_ax_ALIAS_4);

    tra_call(tra, TRA_SF_ADD_ALIAS_1, NULL, TRA_VARIABLE_status_ALIAS_6, TRA_VARIABLE_one_ALIAS_4, NULL);
    tra_call(tra, TRA_SF_ADD_ALIAS_2, NULL, TRA_VARIABLE_status_ALIAS_7, TRA_VARIABLE_zero_ALIAS_5, NULL);
    tra_call(tra, TRA_SF_SUBTRACT_ALIAS_3, NULL, TRA_VARIABLE_status_ALIAS_8, TRA_VARIABLE_one_ALIAS_6, NULL);

    int status;

    tra_call(tra, TRA_SF_LESS_THAN_ALIAS_1, NULL, TRA_VARIABLE_one_ALIAS_1, TRA_VARIABLE_status_ALIAS_9, &status);

    return status;
}

int do_initialize_fail(tra_Data*) {
    DEBUG_PRINTLN("do_initialize_fail")

    tra_copy(tra, TRA_VARIABLE_status_ALIAS_10, TRA_VARIABLE_bx_ALIAS_2);
    tra_call(tra, TRA_SF_MULTIPLY_ALIAS_1, NULL, TRA_VARIABLE_status_ALIAS_11, TRA_VARIABLE_one_ALIAS_1, NULL);
    tra_call(tra, TRA_SF_MULTIPLY_ALIAS_2, NULL, TRA_VARIABLE_status_ALIAS_12, TRA_VARIABLE_one_ALIAS_2, NULL);
    tra_call(tra, TRA_SF_MULTIPLY_ALIAS_3, NULL, TRA_VARIABLE_status_ALIAS_13, TRA_VARIABLE_one_ALIAS_3, NULL);

    // this is an invalid
    tra_copy(tra, TRA_VARIABLE_status_ALIAS_14, TRA_VARIABLE_bx_ALIAS_3);
    tra_copy(tra, TRA_VARIABLE_status_ALIAS_14, TRA_VARIABLE_bx_ALIAS_4);

    tra_call(tra, TRA_SF_SUBTRACT_ALIAS_1, NULL, TRA_VARIABLE_status_ALIAS_15, TRA_VARIABLE_one_ALIAS_7, NULL);
    tra_call(tra, TRA_SF_SUBTRACT_ALIAS_2, NULL, TRA_VARIABLE_status_ALIAS_16, TRA_VARIABLE_zero_ALIAS_8, NULL);
    tra_call(tra, TRA_SF_ADD_ALIAS_3, NULL, TRA_VARIABLE_status_ALIAS_17, TRA_VARIABLE_one_ALIAS_9, NULL);

    int status;

    tra_call(tra, TRA_SF_LESS_THAN_ALIAS_1, NULL, TRA_VARIABLE_one_ALIAS_2, TRA_VARIABLE_status_ALIAS_17, &status);

    return status;
}

LIB_EXPORT jboolean JNICALL Java_com_flexera_schneider_fnesigner_Nova_process(JNIEnv *env, jobject object) {
    TFT status(tra, TRA_VARIABLE_minus_one_ALIAS_1); //-2
    TFT one(tra, TRA_VARIABLE_one_ALIAS_3);

    status -= one;
    if (env) {
        UserData userdata(tra);

//            userdata.path = convert(env, licenseFilePath);

        // AX = 0
        tra_set_value(tra, TRA_VARIABLE_ax_ALIAS_1, TRA_STRING_identity_ALIAS_1);
        tra_set_value(tra, TRA_VARIABLE_bx_ALIAS_1, TRA_STRING_identity_bad_ALIAS_1);

        /**
         * For now ther is no license check
         *
         * tra_if(tra, TRA_SNIF_initialize_ALIAS_1, &userdata);
         *
         */
        tra_if(tra, TRA_SNIF_initialize_ALIAS_1, &userdata);
        status += TFT(tra, TRA_VARIABLE_one_ALIAS_4); // -1
        jclass source = env->GetObjectClass(object);
        if (source) {
            DEBUG_PRINT("### class %p\n", (void* )source)

            jfieldID field_m = env->GetFieldID(source, "message", "Ljava/lang/String;");

            DEBUG_PRINT("### field_m %p\n", (void* )field_m)

            if (field_m) {
                jstring string = env->NewStringUTF(userdata.error.c_str());
                if (string) {
                    DEBUG_PRINT("### message %p\n", (void* )string)

                    env->SetObjectField(object, field_m, string);
                    status += TFT(tra, TRA_VARIABLE_one_ALIAS_5); // 0
                }
            }

            jfieldID field_i = env->GetFieldID(source, "identity", "Ljava/lang/String;");

            DEBUG_PRINT("### field_i %p\n", (void* )field_i)

            if (field_i) {
                int alias = 0;
                tra_get_value(tra, TRA_VARIABLE_status_ALIAS_17, &alias);

                string id = tra_get_string(tra, alias);

                DEBUG_PRINT("### %s\n", id.c_str())

                jstring identity = env->NewStringUTF(id.c_str());
                if (identity) {
                    DEBUG_PRINT("### identity %p\n", (void* )identity);

                    env->SetObjectField(object, field_i, identity);

                    status += TFT(tra, TRA_VARIABLE_one_ALIAS_6); // + 1
                }
            }
        }
    }

    DEBUG_PRINT("RETURN %i %i %s\n", status.get(), one.get(), status == one ? "T" : "F");

    return status == one;
}

bool LIB_EXPORT test_tra(stringstream &stream) {
    TFT status(tra);

    tra_copy(tra, TRA_VARIABLE_status_ALIAS_1, TRA_VARIABLE_zero_ALIAS_1);

    // = 0
    int count = 144;

    tra_call(tra, TRA_SF_ADD_ALIAS_1, NULL, TRA_VARIABLE_status_ALIAS_2, TRA_VARIABLE_one_ALIAS_1, NULL);
    tra_call(tra, TRA_SF_ADD_ALIAS_2, NULL, TRA_VARIABLE_status_ALIAS_3, TRA_VARIABLE_one_ALIAS_2, NULL);
    tra_call(tra, TRA_SF_ADD_ALIAS_3, NULL, TRA_VARIABLE_status_ALIAS_4, TRA_VARIABLE_one_ALIAS_3, NULL);
    tra_call(tra, TRA_SF_ADD_ALIAS_1, NULL, TRA_VARIABLE_status_ALIAS_5, TRA_VARIABLE_one_ALIAS_4, NULL);
    tra_call(tra, TRA_SF_ADD_ALIAS_2, NULL, TRA_VARIABLE_status_ALIAS_6, TRA_VARIABLE_one_ALIAS_5, NULL);
    tra_call(tra, TRA_SF_ADD_ALIAS_3, NULL, TRA_VARIABLE_status_ALIAS_7, TRA_VARIABLE_one_ALIAS_6, NULL);
    tra_call(tra, TRA_SF_ADD_ALIAS_1, NULL, TRA_VARIABLE_status_ALIAS_8, TRA_VARIABLE_one_ALIAS_7, NULL);
    tra_call(tra, TRA_SF_ADD_ALIAS_2, NULL, TRA_VARIABLE_status_ALIAS_9, TRA_VARIABLE_one_ALIAS_8, NULL);
    tra_call(tra, TRA_SF_ADD_ALIAS_3, NULL, TRA_VARIABLE_status_ALIAS_10, TRA_VARIABLE_one_ALIAS_9, NULL);
    tra_call(tra, TRA_SF_ADD_ALIAS_1, NULL, TRA_VARIABLE_status_ALIAS_11, TRA_VARIABLE_one_ALIAS_10, NULL);
    tra_call(tra, TRA_SF_ADD_ALIAS_2, NULL, TRA_VARIABLE_status_ALIAS_12, TRA_VARIABLE_one_ALIAS_11, NULL);
    tra_call(tra, TRA_SF_ADD_ALIAS_3, NULL, TRA_VARIABLE_status_ALIAS_13, TRA_VARIABLE_one_ALIAS_12, NULL);
    tra_call(tra, TRA_SF_ADD_ALIAS_1, NULL, TRA_VARIABLE_status_ALIAS_14, TRA_VARIABLE_one_ALIAS_13, NULL);
    tra_call(tra, TRA_SF_ADD_ALIAS_2, NULL, TRA_VARIABLE_status_ALIAS_15, TRA_VARIABLE_one_ALIAS_14, NULL);
    tra_call(tra, TRA_SF_ADD_ALIAS_3, NULL, TRA_VARIABLE_status_ALIAS_16, TRA_VARIABLE_one_ALIAS_15, NULL);
    // 15

    tra_call(tra, TRA_SF_SUBTRACT_ALIAS_1, NULL, TRA_VARIABLE_status_ALIAS_2, TRA_VARIABLE_one_ALIAS_1, NULL);
    tra_call(tra, TRA_SF_SUBTRACT_ALIAS_2, NULL, TRA_VARIABLE_status_ALIAS_3, TRA_VARIABLE_one_ALIAS_2, NULL);
    tra_call(tra, TRA_SF_SUBTRACT_ALIAS_3, NULL, TRA_VARIABLE_status_ALIAS_4, TRA_VARIABLE_one_ALIAS_3, NULL);
    // 12

    tra_call(tra, TRA_SF_MULTIPLY_ALIAS_3, NULL, TRA_VARIABLE_status_ALIAS_1, TRA_VARIABLE_status_ALIAS_2, NULL);
    // 144

    TFT result(tra, TRA_VARIABLE_status_ALIAS_3);

    stream << "Expected count:" << count << " - actual count:" << result.get();

    return count == result;
}

bool LIB_EXPORT test_fne(const string &licenseFilePath, stringstream &stream) {
    UserData data(tra);

    data.path = licenseFilePath;

    TFT status(tra);
    const TFT istrue(tra, TRA_VARIABLE_one_ALIAS_19);
    status = data.test_initialize();
    if (status == istrue) {
        status = data.test_dump(stream);
    }

    return status == istrue;
}

//
bool LIB_EXPORT quick_test_tra() {
    DEBUG_PRINTLN("TFT status(tra)")
    TFT status(tra);

    DEBUG_PRINTLN("const TFT istrue(tra, TRA_VARIABLE_one_ALIAS_19)")
    const TFT istrue(tra, TRA_VARIABLE_one_ALIAS_19);

    DEBUG_PRINTLN("return status == istrue")
    return status == istrue;
}
}
/* extern c */
