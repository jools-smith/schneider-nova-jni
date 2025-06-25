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
 *  Provides the interface to the licensing capability request rights id
 *  options object.
 *
 *  These functions enable the caller to specify options which can then be 
 *  supplied when adding a rights id to a capability request object. 
 */

#ifndef _INCLUDE_FLC_RIGHTS_ID_OPTIONS_H_
#define _INCLUDE_FLC_RIGHTS_ID_OPTIONS_H_

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

/** Opaque data type for the rights id options object.
*/
typedef struct flcRightsIdOptions * FlcRightsIdOptionsRef;

/**
 *  Creates the rights id options object.
 *
 *  @param  licensing   Licensing environment object
 *  @param  options     Pointer to the variable that receives the reference to the rights ID object being created
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The caller is responsible for deleting the rights id options object via the
 *          FlcRightsIdOptionsDelete API.
 */
FlcBool FlcRightsIdOptionsCreate(FlcLicensingRef           licensing,
                                 FlcRightsIdOptionsRef *   options,
                                 FlcErrorRef               error);

/**
 *  Deletes the rights id options object.
 *
 *  @param  licensing   Licensing environment object
 *  @param  options     Pointer to the variable containing the reference to the rights id options object to be deleted
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcRightsIdOptionsDelete(FlcLicensingRef           licensing,
                                 FlcRightsIdOptionsRef *   options,
                                 FlcErrorRef               error);

/**
 *  Sets the partial fulfillment rights id option. When this option is set to true, the client
 *  indicates to the license server that partial fulfillment of the rights id count is desired
 *  when the full requested count is not available.
 *
 *  @param  licensing          Licensing environment object
 *  @param  options            Rights id options object
 *  @param  partialFulfillment Partial fulfillment option value to set. 
 *  @param  error              Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   This option, if set to FLC_TRUE, is incompatible with rights ids added to 
 *          capability request objects with an operation value of anything other than
 *          FLC_CAPABILITY_REQUEST_OPERATION_REQUEST.
 */
FlcBool FlcRightsIdOptionsSetPartialFulfillment(FlcLicensingRef           licensing,
                                                FlcRightsIdOptionsRef     options,
                                                FlcBool                   partialFulfillment,
                                                FlcErrorRef               error);

/**
 *  Gets the value of the partial fulfillment rights id option in the provided rights id 
 *  options object.
 *
 *  @param  licensing          Licensing environment object
 *  @param  options            Rights id options object
 *  @param  partialFulfillment Pointer to the variable to receive the partial fulfillment option value
 *  @param  error              Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcRightsIdOptionsGetPartialFulfillment(FlcLicensingRef           licensing,
                                                FlcRightsIdOptionsRef     options,
                                                FlcBool *                 partialFulfillment,
                                                FlcErrorRef               error);

  

/** @} */ /* end of addtogroup CapabilityRequest */
/** @} */ /* end of addtogroup licensingClient */


#ifdef __cplusplus
}
#endif

#endif /* _INCLUDE_FLC_RIGHTS_ID_OPTIONS_H_ */
