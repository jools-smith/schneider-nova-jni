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
#ifndef _ERR_DISLAY_H_
#define _ERR_DISLAY_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "FlcTypes.h"
#include "FlcError.h"

/* Global Buffers -- used for display only */
extern char msg[1024];


/**
 *    Convert FlcChar string to char string
 *
 *    @param    data    FlcChar string to convert
 *    @param    result  char buffer to receive converted string
 *    @param    size    Size of result buffer in chars
 *    @return   none
 */
void
FlcCharToTChar(
    const FlcChar *    data,
    char *            result,
    size_t             size);


/**
 *    Convert char string to FlcChar string
 *
 *    @param    data     char string to convert
 *    @param    result   FlcChar buffer to receive converted string
 *    @param    size     Size of result buffer in FlcChar's
 *    @return   none
 */
void
TCharToFlcChar(
    const char *    data,
    FlcChar *        result,
    size_t           size);


/**
 *    Output message to stdout
 *
 *    @param    msg        Message to output
 *    @param    caption    Optional caption
 */
void
DisplayMessage(
    const char * msg,
    const char * caption);


/**
 *    Output error string to stdout
 *
 *    @param    msg      Message to output
 *    @param    error    Detailed error info
 */
void
DisplayErrorString(
    const char *   msg,
    FlcErrorRef     error);


/**
 *    Output info string to stdout
 *
 *    @param    msg        Message to output
 */
void
DisplayInfoString(const char * msg);

/**
 *    Display status
 *
 *    @param    code      Status code indicating information about device Id or license rights
 *    @param    detail    Details from FlcCapabilityResponseGetStatusItem
 *    @param    category  Category such as internal or external
 */
void
DisplayStatus(
    FlcInt32 code,
    const FlcChar* detail,
    FlcInt32 category);

char * GetMonth(int month);

#endif


