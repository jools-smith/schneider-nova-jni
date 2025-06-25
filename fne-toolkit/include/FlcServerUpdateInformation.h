/****************************************************************************
  Copyright (c) 2012-2020 Flexera Software LLC.
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

/** @file
 *  Provides the interface to license server update information.
 *
 *  These functions enable the caller to determine the interval since the last time
 *  trusted storage was updated and the interval until the next required update.
 */

#ifndef _INCLUDE_FLC_SERVERUPDATEINFORMATION_H_
#define _INCLUDE_FLC_SERVERUPDATEINFORMATION_H_

#include "FlcTypes.h"
#include "FlcError.h"
#include "FlcLicensing.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup licensingClient
 * @{
 */

 /**
 * @defgroup serverUpdate Server Update
 * @{
 */

/**
 *  Gets number of seconds since the last processed capability response from the default 
 *  license server instance.
 *
 *  @param  licensing    Licensing environment object
 *  @param  seconds      Pointer to variable to return seconds
 *  @param  error        Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Possible error codes include
 *        FLCERR_NO_SERVER_DATA_FOUND,
 *        FLCERR_WINDBACK_DETECTED
 */
FlcBool FlcServerUpdateGetLastTime(FlcLicensingRef licensing,
                                   FlcUInt32*      seconds,
                                   FlcErrorRef     error);

/**
 *  Gets number of seconds since the last processed capability response from the specified 
 *  license server instance.
 *
 *  @param  licensing      Licensing environment object
 *  @param  serverInstance License server instance of interest
 *  @param  seconds        Pointer to variable to return seconds
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Possible error codes include
 *        FLCERR_NO_SERVER_DATA_FOUND,
 *        FLCERR_WINDBACK_DETECTED
 */
FlcBool FlcServerInstanceGetSecondsSinceLastUpdate(FlcLicensingRef            licensing,
                                                   FlcLicenseServerInstance   serverInstance,
                                                   FlcUInt32 *                seconds,
                                                   FlcErrorRef                error);

/**
 *  Gets number of seconds left until the next update of trusted storage for the default
 *  license server instance is required according to the renew interval
 *
 *  @param  licensing    Licensing environment object
 *  @param  seconds      Pointer to variable to return seconds
 *  @param  error        Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Negative number of seconds indicates that the time for required update has already passed
 *
 *  @note Possible error codes include
 *        FLCERR_NO_SERVER_DATA_FOUND,
 *        FLCERR_NO_SERVER_UPDATE_NEEDED,
 *        FLCERR_WINDBACK_DETECTED
 */
FlcBool FlcServerUpdateGetNextTime(FlcLicensingRef licensing,
                                   FlcInt32*       seconds,
                                   FlcErrorRef     error);

/**
 *  Gets number of seconds left until the next update of trusted storage for the specified
 *  license server instance is required according to the renew interval
 *
 *  @param  licensing      Licensing environment object
 *  @param  serverInstance License server instance of interest
 *  @param  seconds        Pointer to variable to return seconds
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Negative number of seconds indicates that the time for required update has already passed
 *
 *  @note Possible error codes include
 *        FLCERR_NO_SERVER_DATA_FOUND,
 *        FLCERR_NO_SERVER_UPDATE_NEEDED,
 *        FLCERR_WINDBACK_DETECTED
 */
FlcBool FlcServerInstanceGetSecondsUntilNextUpdate(FlcLicensingRef            licensing,
                                                   FlcLicenseServerInstance   serverInstance,
                                                   FlcInt32*                  seconds,
                                                   FlcErrorRef                error);

/** @} */ /* end of serverUpdate group */
/** @} */ /* end of addtogroup licensingClient */


#ifdef __cplusplus
}
#endif

#endif /* _INCLUDE_FLC_SERVERUPDATEINFORMATION_H_ */
