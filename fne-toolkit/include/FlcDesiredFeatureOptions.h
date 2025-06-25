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
 *  Provides the interface to the licensing capability request desired feature
 *  options object.
 *
 *  These functions enable the caller to specify options which can then be 
 *  supplied when adding a desired feature to a capability request object. 
 */

#ifndef _INCLUDE_FLC_DESIRED_FEATURE_OPTIONS_H_
#define _INCLUDE_FLC_DESIRED_FEATURE_OPTIONS_H_

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
 * @addtogroup CapabilityRequest
 * @{
 */

/** Opaque data type for the desired feature options object.
*/
typedef struct flcDesiredFeatureOptions * FlcDesiredFeatureOptionsRef;

/**
 *  Creates the desired feature options object.
 *
 *  @param  licensing   Licensing environment object
 *  @param  options     Pointer to the variable to receive the reference to the desired feature options object to be created
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The caller is responsible for deleting the desired feature options object via the
 *          FlcDesiredFeatureOptionsDelete API.
 */
FlcBool FlcDesiredFeatureOptionsCreate(FlcLicensingRef               licensing,
                                       FlcDesiredFeatureOptionsRef * options,
                                       FlcErrorRef                   error);

/**
 *  Deletes the desired feature options object.
 *
 *  @param  licensing   Licensing environment object
 *  @param  options     Pointer to the variable containing the reference to the desired feature options object to be deleted
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcDesiredFeatureOptionsDelete(FlcLicensingRef               licensing,
                                       FlcDesiredFeatureOptionsRef * options,
                                       FlcErrorRef                   error);

/**
 *  Sets the partial fulfillment desired feature option. When this option is set to true the client
 *  indicates to the license server that partial fulfillment of the feature count is desired
 *  when the full requested count is not available.
 *
 *  @param  licensing          Licensing environment object
 *  @param  options            Desired feature options object
 *  @param  partialFulfillment Partial fulfillment option value to set. 
 *  @param  error              Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   This option, if set to FLC_TRUE, is incompatible with desired features added to 
 *          capability request objects with an operation value of anything other than
 *          FLC_CAPABILITY_REQUEST_OPERATION_REQUEST.
 */
FlcBool FlcDesiredFeatureOptionsSetPartialFulfillment(FlcLicensingRef             licensing,
                                                      FlcDesiredFeatureOptionsRef options,
                                                      FlcBool                     partialFulfillment,
                                                      FlcErrorRef                 error);

/**
 *  Gets the value of the partial fulfillment desired feature option in the provided desired feature 
 *  options object.
 *
 *  @param  licensing          Licensing environment object
 *  @param  options            Desired feature options object
 *  @param  partialFulfillment Pointer to the variable to receive the partial fulfillment option value
 *  @param  error              Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcDesiredFeatureOptionsGetPartialFulfillment(FlcLicensingRef             licensing,
                                                      FlcDesiredFeatureOptionsRef options,
                                                      FlcBool *                   partialFulfillment,
                                                      FlcErrorRef                 error);

  

/** @} */ /* end of addtogroup CapabilityRequest */
/** @} */ /* end of addtogroup licensingClient */


#ifdef __cplusplus
}
#endif

#endif /* _INCLUDE_FLC_DESIRED_FEATURE_OPTIONS_H_ */
