#pragma once

#include <jni.h>

#include <string>
#include <vector>

class JNIHelper {
    JNIEnv *const environment;
    jobject object;

    static const std::string separator;

public:
    JNIHelper(JNIEnv *const env, jobject object) : environment(env), object(object ){
    }

    void set_string_field(const std::string&field_name, const std::string&value) const;

    std::string get_string_value(jstring value) const;

    std::vector<unsigned char> get_byte_array(jbyteArray value) const;
};


