#include "JNIHelper.h"


#include <stdexcept>

using namespace std;

const string JNIHelper::separator = " | ";

vector<unsigned char> JNIHelper::get_byte_array(jbyteArray value) const{
    jsize size = environment->GetArrayLength(value);

    vector<unsigned char> buffer;

    buffer.reserve(size);

    environment->GetByteArrayRegion(value, 0, size, reinterpret_cast<jbyte*>(buffer.data()));

    return buffer;
}

string JNIHelper::get_string_value(jstring value) const {
    const char*const cstr = environment->GetStringUTFChars(value, nullptr);

    if (!cstr) {
        throw runtime_error("Cannot decode jstring value");
    }

    string str = cstr;

    environment->ReleaseStringUTFChars(value, cstr);

    return str;
}

void JNIHelper::set_string_field(const std::string&field_name, const std::string& value) const {
    if (!environment) {
        throw runtime_error("No JNI environment reference");
    }

    if (!object) {
        throw runtime_error("No JNI object reference");
    }

    jclass source = environment->GetObjectClass(object);
    if (!source) {
        throw runtime_error("Unable to retrieve class object");
    }

    jfieldID java_field = environment->GetFieldID(source, field_name.c_str(), "Ljava/lang/String;");
    if (!java_field) {
        throw runtime_error("Unable to retrieve member" + separator + field_name);
    }

    jstring java_string = environment->NewStringUTF(value.c_str());
    if (!java_string) {
        throw runtime_error("Unable to create string" + separator + value);
    }

    environment->SetObjectField(object, java_field, java_string);

}
