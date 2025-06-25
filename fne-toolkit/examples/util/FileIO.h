/****************************************************************************
  Copyright (c) 2008-2020 Flexera Software LLC.
  All Rights Reserved.
  This software has been provided pursuant to a License Agreement
  containing restrictions on its use.  This software contains
  valuable trade secrets and proprietary information of
  Flexera Software LLC and is protected by law.
  It may not be copied or distributed in any form or medium, disclosed
  to third parties, reverse engineered or used in any manner not
  provided for in said License Agreement except with the prior
  written authorization from Flexera Software LLC.
*****************************************************************************/
#ifndef _FILE_IO_H_
#define _FILE_IO_H_

#ifdef _MSC_VER
#include <tchar.h>
#endif
#include "FlcTypes.h"

FlcBool
GetStoragePath(
    FlcChar *   pathName,
    FlcSize     pathLength);

FlcBool
ReadFileData(
    const char *   fileName,
    FlcUInt8 **     buffer,
    FlcSize *       bufferSize);

FlcBool
ReadFileText(
    const char *   fileName,
    FlcChar **      text);

FlcBool
WriteFileData(
    const char *       fileName,
    const FlcUInt8 *    buffer,
    FlcSize             bufferSize);

#endif




