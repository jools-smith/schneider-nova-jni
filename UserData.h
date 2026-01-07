#pragma once

#include "dump.h"
#include "tra.h"
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

  void reset();
};

struct IdentityMessagePayload final {
  int identity_name;
  int identity_value;
  int message_name;
  int message_value;
};

class UserData final {
  TraWrapper &m_tra;

  int reply;

  FieldWrapper identity;
  FieldWrapper message;

  TFT status;

  UserData(const UserData&source) : m_tra(source.m_tra), reply(0), identity(source.m_tra), message(source.m_tra), status(source.m_tra, TRA_VARIABLE_minus_one_ALIAS_1){

  }

  UserData& operator =(const UserData&) {
    return *this;
  }

public:

  UserData(TraWrapper &value) : m_tra(value), reply(0), identity(value), message(value), status(value, TRA_VARIABLE_minus_one_ALIAS_1) {
  }

  ~UserData() = default;

  void release_all_strings();

  const char* get_identity_name();

  const char* get_identity_value();

  const char* get_message_name();

  const char* get_message_value();

  TFT& get_status() {
    return status;
  }

  int* get_reply_address() {
    return &reply;
  }

  int& get_reply() {
    return reply;
  }

  void set_status(const TRA_VARIABLE_INDEX alias);

  void set_identity_message_values(TRA_STRING_INDEX alias_1, TRA_STRING_INDEX alias_2, TRA_STRING_INDEX alias_3, TRA_STRING_INDEX alias_4);

  void set_identity_message_values(const IdentityMessagePayload &payload);
};

