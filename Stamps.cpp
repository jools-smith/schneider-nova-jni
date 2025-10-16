/*
 * Stamps.cpp
 *
 *  Created on: Oct 16, 2025
 *      Author: jools
 */

#include "Stamps.h"

//TODO: not sure its good practice to include all this tra stuff here
#include "tra.h"
#include "tra_gen/nova_declarative_data.h"

#include <ctime>
#include <time.h>
#include <string>
#include <sstream>
#include <set>
#include <vector>
#include <locale>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

std::string Stamps::get_timestamp() const {

  stringstream zulu;
  zulu << datestamp << " " << timestamp;


  std::tm tmb;
  if (zulu >> std::get_time(&tmb, "%b %e %Y %H:%M:%S")) {

    const auto epoch = std::mktime(&tmb);

    char bfr[STAMP_BUFFER_SIZE];
    struct tm time;

    strptime(datestamp.c_str(), "%b %e %Y", &time);

    // bfr = yyyy-mm-dd
    strftime(bfr, sizeof bfr, "%Y.%m.%d", &time);

    stringstream version_string;

    version_string << bfr << "." << uppercase << hex << epoch << " [" << release << "]";

    return version_string.str();
  }
  else {
    return "unable to calulate version";
  }
}

std::string Stamps::get_gnu_version() const {
  char bfr[STAMP_BUFFER_SIZE];

  snprintf(bfr, sizeof bfr, "%d.%d.%d", gnuc, gnuc_minor, gnuc_patch);

  return bfr;
}

std::string Stamps::get_tra_version() const {
  char bfr[STAMP_BUFFER_SIZE];

  snprintf(bfr, sizeof bfr, "%s.%s.%s.%s", TRA_VERSION_MAJOR, TRA_VERSION_MINOR, TRA_VERSION_MAINT, TRA_VERSION_BUILD);

  return bfr;
}
