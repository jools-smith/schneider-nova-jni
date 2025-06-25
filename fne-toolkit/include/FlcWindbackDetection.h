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
 *  Provides the interface to the licensing client clock-windback detection
 *  functionality.
 *
 *  Allows for enabling and disabling clock-windback detection with the
 *  specified tolerance and frequency values.
 */

#ifndef _INCLUDE_FLC_WINDBACKDETECTION_H_
#define _INCLUDE_FLC_WINDBACKDETECTION_H_

#include "FlcTypes.h"
#include "FlcError.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup licensingClient
 * @{
 */

/**
 * @defgroup clockWindback Clock Windback
 * @{
 */

/**
 *  Sets the licensing environment clock-windback detection tolerance parameter
 *
 *  @param  licensing   Licensing environment object
 *  @param  tolerance     Tolerance (in seconds) for windback detection. The system is considered
 *                      to be in a wound-back state if the current time is less than
 *                      (anchor time - tolerance).
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info).
 *
 *  @note   The default licensing environment tolerance parameter is 86400 seconds or one day.
 *
 *  @note   The tolerance set in this call is a target tolerance for the system. The true tolerance
 *          depends on the frequency of anchor updates and response window. If tolerance is set to
 *          zero (0), no tolerance window is granted.
 *
 *  @note   If clock-windback detection is already enabled setting this value will not change the  
 *          effective tolerance value until FlcClockWindbackDetectionSetEnabled is called with the
 *          "enable" parameter set to FLC_TRUE.
 */
FlcBool FlcClockWindbackDetectionSetTolerance(FlcLicensingRef licensing,
                                              FlcUInt32       tolerance,
                                              FlcErrorRef     error);
/**
 *  Gets the licensing environment clock-windback detection tolerance parameter
 *
 *  @param  licensing   Licensing environment object
 *  @param  tolerance     Pointer to variable to receive the current licensing environment clock-windback 
 *                      detection tolerance value (in seconds). The system is considered to be in a 
 *                      wound-back state if the current time is less than (anchor time - tolerance).
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info).
 */
FlcBool FlcClockWindbackDetectionGetTolerance(FlcLicensingRef licensing,
                                              FlcUInt32 *     tolerance,
                                              FlcErrorRef     error);

/**
 *  Sets the licensing environment clock-windback detection frequency parameter
 *
 *  @param  licensing   Licensing environment object
 *  @param  frequency   Frequency (in seconds) for anchor time updates
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info).
 *
 *  @note   The default licensing environment frequency parameter is 86400 seconds or one day.
 *
 *  @note   The frequency set in this call is the target frequency of anchor updates on the system.
 *          The actual frequency might be lower if not many time-sensitive events are happening; or
 *          it may be higher if the user keeps winding the clock back or if many responses are received.
 *          If frequency is set to zero (0), anchor will be updated every time clock-windback detection
 *          happens (explicitly or implicitly)
 *
 *  @note   If windback detection is already enabled setting this value will not change the effective
 *          clock-windback anchor time update frequency value until FlcClockWindbackDetectionSetEnabled
 *          is called with the "enable" parameter set to FLC_TRUE.
 */
FlcBool FlcClockWindbackDetectionSetFrequency(FlcLicensingRef licensing,
                                              FlcUInt32       frequency,
                                              FlcErrorRef     error);

/**
 *  Gets the licensing environment clock-windback detection frequency parameter
 *
 *  @param  licensing   Licensing environment object
 *  @param  frequency   Pointer to variable to receive the current licensing environment clock-windback 
 *                      detection frequency value (in seconds)
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info).
 */
FlcBool FlcClockWindbackDetectionGetFrequency(FlcLicensingRef licensing,
                                              FlcUInt32 *     frequency,
                                              FlcErrorRef     error);

/**
 *  Gets a value indicating whether or not clock-windback detection is currently enabled
 *
 *  @param  licensing   Licensing environment object
 *  @param  enabled       Pointer to variable which is set to FLC_TRUE if clock-windback detection is
 *                      currently enabled or FLC_FALSE if clock-windback detection is currently disabled
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info).
 */
FlcBool FlcClockWindbackDetectionIsEnabled(FlcLicensingRef licensing,
                                           FlcBool *       enabled,
                                           FlcErrorRef     error);

/**
 *  Enables or disables clock-windback detection. If enabling clock-windback detection the current
 *  licensing environment tolerance and frequency values are used.
 *
 *  @param  licensing   Licensing environment object
 *  @param  enable        Enables clock-windback detection if set to FLC_TRUE, otherwise disables
 *                      clock-windback detection
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info).
 *
 *  @note   By default the licensing environment is created with windback detection disabled.
 *
 *  @note   Clock-windback detection requires that the licensing environment be created with a 
 *          storage path, even when using buffer license sources.
 *
 *  @note   If clock-windback detection is already enabled, calling this function updates the tolerance
 *          and frequency values with the current licensing environment clock-windback parameter values.
 *
 *  @note Possible error codes include
 *        FLCERR_STORAGE_CLASS_INCOMPLETE
 */
FlcBool FlcClockWindbackDetectionSetEnabled(FlcLicensingRef licensing, 
                                            FlcBool         enable, 
                                            FlcErrorRef     error);

/**
 *  Sets the licensing environment clock-windback parameters to the specified parameters and 
 *  enables clock-windback detection
 *
 *  @param  licensing   Licensing environment object
 *  @param  tolerance     Tolerance (in seconds) for clock-windback detection. The system is considered
 *                      to be in a wound-back state if the current time is less than
 *                      (anchor time - tolerance).
 *  @param  frequency   Frequency (in seconds) for anchor time updates.
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info).
 *
 *  @note   By default the licensing environment is created with clock-windback detection disabled.
 *
 *  @note   Clock-windback detection requires that the licensing environment be created with a 
 *          storage path, even when using buffer license sources.
 *
 *  @note   The tolerance set in this call is a target tolerance for the system. The true tolerance
 *          depends on the frequency of anchor updates and response window. If tolerance is set to
 *          zero (0), no tolerance window is granted.
 *
 *  @note   The frequency set in this call is the target frequency of anchor updates on the system.
 *          The actual frequency might be lower if not many time-sensitive events are happening; or
 *          it may be higher if the user keeps winding the clock back or if many responses are received.
 *          If frequency is set to zero (0), anchor will be updated every time clock-windback detection
 *          happens (explicitly or implicitly)
 *
 *  @note   If clock-windback detection is already enabled, calling this function updates the tolerance
 *          and frequency values.
 *
 *  @note Possible error codes include
 *        FLCERR_STORAGE_CLASS_INCOMPLETE
 */
FlcBool FlcClockWindbackDetectionEnable(FlcLicensingRef licensing, 
                                        FlcUInt32       tolerance,  
                                        FlcUInt32       frequency, 
                                        FlcErrorRef     error);

/**
 *  Checks whether the clock-windback state is detected on the system
 *
 *  @param  licensing   Licensing environment object
 *  @param  detected    Pointer to variable to receive clock-windback status, FLC_TRUE if
 *                      clock-windback detected otherwise FLC_FALSE
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   This function is intended to be used in conjunction with the trusted storage 
 *          private data source facility in cases similar to when a private data source is
 *          being used to store a product metric such as a launch counter.
 *  @note   Possible error codes include
 *          FLCERR_WINDBACK_NOT_ENABLED
 */
FlcBool FlcClockWindbackDetected(FlcLicensingRef licensing, 
                                 FlcBool *       detected,
                                 FlcErrorRef     error);

/** @} */ /* end of clockWindback group */
/** @} */ /* end of addtogroup licensingClient */


#ifdef __cplusplus
}
#endif

#endif /* _INCLUDE_FLC_WINDBACKDETECTION_H_ */
