#include "UserData.h"

using namespace std;


void FieldWrapper::reset() {
  name = TRA_VARIABLE_minus_one_ALIAS_4;
  value = TRA_VARIABLE_minus_one_ALIAS_5;
}

void UserData::release_all_strings() {
	tra_release_string(m_tra, identity.name);
	tra_release_string(m_tra, identity.value);
	identity.reset();

	tra_release_string(m_tra, message.name);
	tra_release_string(m_tra, message.value);
	message.reset();
}


void UserData::set_identity_message_values(const TRA_STRING_INDEX alias_1, const TRA_STRING_INDEX alias_2, const TRA_STRING_INDEX alias_3, const TRA_STRING_INDEX alias_4) {
	identity.name = alias_1;
	identity.value = alias_2;
	message.name = alias_3;
	message.value = alias_4;
}

void UserData::set_status(const TRA_VARIABLE_INDEX alias) {
  status = TFT(m_tra, alias);
}

const char* UserData::get_identity_name() {
   return tra_get_string(m_tra, identity.name);
 }

 const char* UserData::get_identity_value() {
   return tra_get_string(m_tra, identity.value);
 }

 const char* UserData::get_message_name() {
   return tra_get_string(m_tra, message.name);
 }

 const char* UserData::get_message_value() {
   return tra_get_string(m_tra, message.value);
 }

 void UserData::set_identity_message_values(const IdentityMessagePayload &payload) {
   set_identity_message_values(static_cast<TRA_STRING_INDEX>(payload.identity_name), static_cast<TRA_STRING_INDEX>(payload.identity_value), static_cast<TRA_STRING_INDEX>(payload.message_name),
       static_cast<TRA_STRING_INDEX>(payload.message_value));
 }
