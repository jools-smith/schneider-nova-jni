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
 *  Provides the interface to the licensing information message object. An information
 *  message is sent by the client to the back-up license server as part of fail-over support,
 *  or to a local FlexNet Embedded license server if information message-based usage
 *   information is enabled.
 *
 *  These functions enable the caller to create and delete information message objects,
 *  and to add existing features from the specified license source.
 */

#ifndef _INCLUDE_FLC_INFORMATIONMESSAGE_H_
#define _INCLUDE_FLC_INFORMATIONMESSAGE_H_

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
 * @defgroup InformationMessage Information Message
 * @{
 */

/** Opaque data type for the information message object.
*/
typedef struct flcInformationMessage * FlcInformationMessageRef;

/**
 *  Creates the information message object.
 *
 *  @param  licensing   Licensing environment object
 *  @param  infomsg     Pointer to the variable to receive the reference to the information message object to be created
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The caller is responsible for deleting the information message object via the
 *          FlcInformationMessageDelete API.
 */
FlcBool FlcInformationMessageCreate(FlcLicensingRef             licensing,
                                    FlcInformationMessageRef *  infomsg,
                                    FlcErrorRef                 error);

/**
 *  Deletes the information message object
 *
 *  @param  licensing   Licensing environment object
 *  @param  infomsg     Pointer to the variable to containing the reference to the information message object to be deleted
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcInformationMessageDelete(FlcLicensingRef             licensing,
                                    FlcInformationMessageRef *  infomsg,
                                    FlcErrorRef                 error);
/**
 *  Set the information message target license server instance
 *
 *  @param  licensing       Licensing environment object
 *  @param  infomsg         Information message object on which to set the license server instance value
 *  @param  serverInstance  License server instance value 
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The default information message license server instance is FLC_LICENSE_SERVER_DEFAULT.
 *  @note   You may not set the license server instance to FLC_LICENSE_SERVER_UNKNOWN.
 */
FlcBool FlcInformationMessageSetServerInstance(FlcLicensingRef            licensing,
                                               FlcInformationMessageRef   infomsg,
                                               FlcLicenseServerInstance   serverInstance,
                                               FlcErrorRef                error);
/**
 *  Set UUID in information message
 *
 *  @param  licensing   Licensing environment object
 *  @param  infomsg     Information message object to add UUID to
 *  @param  uuidData    UUID to set
 *  @param  uuidSize    Size of UUID in bytes
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success else FLC_FALSE
 *
 *  @note  Sets error to FLCERR_UUID_SOURCE_CONFLICT if the UUID has been already set, 
 *         whether directly or using FlcInformationMessageAddExistingFeatures.
*/
FlcBool FlcInformationMessageSetUuid(FlcLicensingRef           licensing,
                                     FlcInformationMessageRef  infomsg,
                                     FlcUInt8 *                uuidData,
                                     FlcSize                   uuidSize,
                                     FlcErrorRef               error);

/**
 *  Add existing license info to the information message
 *
 *  @param  licensing   Licensing environment object
 *  @param  infomsg     Information message object to add UUID to
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   Currently applicable when trusted storage or served buffer license sources
 *          have been added to the license source collection.
 *
 *  @note Possible error codes include
 *      FLCERR_LICENSE_SOURCE_TYPE_INVALID
 *      FLCERR_INFORMATION_MESSAGE_USAGE_TYPE_CONFLICT
 *      FLCERR_UUID_SOURCE_CONFLICT
 *      FLCERR_HOST_ID_SOURCE_CONFLICT
 *      
 */
FlcBool FlcInformationMessageAddExistingFeatures(FlcLicensingRef           licensing,
                                                 FlcInformationMessageRef  infomsg,
                                                 FlcErrorRef               error);

/**
 *  Adds a string vendor dictionary item to the information message object.
 *
 *  @param  licensing   Licensing environment object
 *  @param  infomsg     Information message object to which to add the vendor dictionary item
 *  @param  key         Vendor dictionary item key
 *  @param  value       Vendor dictionary item value
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcInformationMessageAddVendorDictionaryStringItem(FlcLicensingRef           licensing,
                                                           FlcInformationMessageRef  infomsg,
                                                           const FlcChar *           key,
                                                           const FlcChar *           value,
                                                           FlcErrorRef               error);

/**
 *  Adds an integer vendor dictionary item to the information message object
 *
 *  @param  licensing   Licensing environment object
 *  @param  infomsg     Information message object to which to add the vendor dictionary item
 *  @param  key         Vendor dictionary item key
 *  @param  value       Vendor dictionary item value
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcInformationMessageAddVendorDictionaryIntItem(FlcLicensingRef           licensing,
                                                        FlcInformationMessageRef  infomsg,
                                                        const FlcChar *           key,
                                                        FlcInt32                  value,
                                                        FlcErrorRef               error);

/**
 *  Removes a vendor dictionary item from the information message object
 *
 *  @param  licensing        Licensing environment object
 *  @param  infomsg          Information message object from which to remove the vendor dictionary item
 *  @param  key              Vendor dictionary item key for item to remove
 *  @param  error            Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcInformationMessageRemoveVendorDictionaryItem(FlcLicensingRef           licensing,
                                                        FlcInformationMessageRef  infomsg,
                                                        const FlcChar *           key,
                                                        FlcErrorRef               error);
/**
 *  Add feature usage info to the information message object
 *
 *  @param  licensing       Licensing environment object
 *  @param  infomsg         Information message object to which to add the feature usage 
 *  @param  featureName     Feature name
 *  @param  featureVersion  Feature version
 *  @param  featureCount    Feature count
 *  @param  expiration      Feature expiration
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Can be called more than one time, each time adding additional feature using information.
 *
 *  @note Possible error codes include
 *      FLCERR_INFORMATION_MESSAGE_USAGE_TYPE_CONFLICT
 *      
 */
FlcBool FlcInformationMessageAddFeatureUsage(FlcLicensingRef           licensing,
                                             FlcInformationMessageRef  infomsg,
                                             const FlcChar *           featureName,
                                             const FlcChar *           featureVersion,
                                             FlcUInt32                 featureCount,
                                             struct tm *               expiration,
                                             FlcErrorRef               error);

/**
 *  Generates binary information message
 *
 *  @param  licensing           Licensing environment object
 *  @param  infomsg             Information message object for which to generate a binary message
 *  @param  binaryMsg               Pointer to returned binary information message buffer
 *  @param  binaryMsgSize       Pointer to returned binary information message size
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note On success the caller will be responsible for deallocating the binary information message 
 *        storage using the FlcMemoryFree API.
 */
FlcBool FlcInformationMessageGenerate(FlcLicensingRef           licensing,
                                      FlcInformationMessageRef  infomsg,   
                                      FlcUInt8 **               binaryMsg,
                                      FlcSize *                 binaryMsgSize,
                                      FlcErrorRef               error);

/**
 *  Add vm info dictionary to information message.
 *
 *  @param  licensing   Licensing environment object
 *  @param  infomsg      Information message object for which to add vm info to
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note By default vm info dictionary is not added
 *
 */
FlcBool FlcInformationMessageAddVmInfo(FlcLicensingRef         licensing,
                                      FlcInformationMessageRef infomsg,
                                      FlcErrorRef             error);

/** @} */ /* end of InformationMessage group */
/** @} */ /* end of addtogroup licensingClient */


#ifdef __cplusplus
}
#endif

#endif /* _INCLUDE_FLC_INFORMATIONMESSAGE_H_ */
