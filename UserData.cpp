#include "UserData.h"

using namespace std;


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
