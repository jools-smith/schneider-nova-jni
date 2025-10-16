/*
 * Stamps.h
 *
 *  Created on: Oct 16, 2025
 *      Author: jools
 */

#pragma once

#include "dump.h"

#include <string>

#define STAMP_BUFFER_SIZE 64
class Stamps {
  const std::string datestamp;
  const std::string timestamp;

//  const std::string version;

  const int gnuc;
  const int gnuc_minor;
  const int gnuc_patch;
public:

  }


  std::string get_timestamp() const ;

  std::string get_gnu_version() const ;

  std::string get_tra_version() const ;
};
