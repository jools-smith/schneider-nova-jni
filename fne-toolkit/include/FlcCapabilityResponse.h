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
 *  Provides the interface to the licensing client capability response object.
 *
 *  These functions enable the caller to extract feature, status, and
 *  vendor dictionary information from the capability response object.
 */

#ifndef _INCLUDE_FLC_CAPABILITYRESPONSE_H_
#define _INCLUDE_FLC_CAPABILITYRESPONSE_H_

#include "FlcTypes.h"
#include "FlcError.h"
#include "FlcDictionary.h"
#include "FlcFeature.h"
#include "FlcStatus.h"
#include "FlcMachineType.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup licensingClient
 * @{
 */

/**
 * @defgroup CapabilityResponse Capability Response
 * @{
 */

/** Opaque data type for the capability response object.
*/
typedef struct flcCapabilityResponse * FlcCapabilityResponseRef;

/**
 *  Creates the capability response object.
 *
 *  @param  licensing            Licensing environment object
 *  @param  response             Pointer to the variable to receive the reference to the capability response object to be created
 *  @param  binaryResponse       Buffer containing binary response
 *  @param  binaryResponseSize   Size of binary response buffer
 *  @param  error                Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note The caller is responsible for deleting the capability response object via the
 *        FlcCapabilityResponseDelete API.
 *
 *  @note Possible error codes include
 *        FLCERR_DATA_INSUFFICIENT,
 *        FLCERR_DATA_CORRUPTED,
 *        FLCERR_SIGNATURE_INVALID
 *        FLCERR_INPUT_TYPE_MISMATCH
 */
FlcBool FlcCapabilityResponseCreate(FlcLicensingRef             licensing,
                                    FlcCapabilityResponseRef *  response,
                                    const FlcUInt8 *            binaryResponse,
                                    FlcSize                     binaryResponseSize,
                                    FlcErrorRef                 error);
/**
 *  Creates the capability response object from the data contained in the supplied file
 *  
 *  @param  licensing            Licensing environment object
 *  @param  response             Pointer to variable to receive the reference to the capability response object to be created
 *  @param  fileName             Name of the file containing the binary response data
 *  @param  error                Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note The caller is responsible for deleting the capability response object via the
 *        FlcCapabilityResponseDelete API.
 *
 *  @note Possible error codes include
 *        FLCSWC_FILE_OPEN  
 *        FLCSWC_EMPTY_FILE 
 *        FLCSWC_FILE_READ  
 *        FLCERR_DATA_INSUFFICIENT,
 *        FLCERR_DATA_CORRUPTED,
 *        FLCERR_SIGNATURE_INVALID
 */
FlcBool FlcCapabilityResponseCreateFromFile(FlcLicensingRef             licensing,
                                            FlcCapabilityResponseRef *  response,
                                            const FlcChar *             fileName,
                                            FlcErrorRef                 error);

/**
 *  Deletes the capability response object.
 *
 *  @param  licensing   Licensing environment object
 *  @param  response    Pointer to variable containing the reference to the capability response object to be deleted
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCapabilityResponseDelete(FlcLicensingRef             licensing,
                                    FlcCapabilityResponseRef *  response,
                                    FlcErrorRef                 error);

/**
 *  Get capability response type.
 *
 *  @param  response      Capability response object of interest
 *  @param  messageType   Pointer to variable to receive capability response message type
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCapabilityResponseGetMessageType(FlcCapabilityResponseRef response,
                                            FlcMessageType *         messageType, 
                                            FlcErrorRef              error);

/**
 *  Get renew interval from response.
 *
 *  @param  response      Capability response object of interest
 *  @param  renewInterval Pointer to variable to receive renew interval set into the capability response
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCapabilityResponseGetRenewInterval(FlcCapabilityResponseRef response,
                                              FlcUInt32 *              renewInterval,
                                              FlcErrorRef              error);

/**
 *  Get server name from response
 *
 *  @param  response      Capability response object of interest
 *  @param  serverName    Pointer to variable to receive server name set into the capability response.
 *                        Returns NULL if the server name has not been set
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCapabilityResponseGetServerName(FlcCapabilityResponseRef response,
                                           const FlcChar **         serverName,
                                           FlcErrorRef              error);

/**
 *  Get server URI from response
 *
 *  @param  response      Capability response object of interest
 *  @param  serverURI     Pointer to variable to receive URI of the main server. Returns NULL if not
 *                        set by the server
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCapabilityResponseGetServerURI(FlcCapabilityResponseRef response,
                                          const FlcChar **         serverURI,
                                          FlcErrorRef              error);
/**
 *  Get backup server URI from response
 *
 *  @param  response          Capability response object of interest
 *  @param  backupServerURI   Pointer to variable to receive URI of the backup server. Returns NULL if not
 *                            set by the server
 *  @param  error             Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCapabilityResponseGetBackupServerURI(FlcCapabilityResponseRef response,
                                                const FlcChar **         backupServerURI,
                                                FlcErrorRef              error);
/**
 *  Get server hostid from response
 *
 *  @param  response          Capability response object of interest
 *  @param  serverIdType      Pointer to variable to receive the capability response server hostid type
 *                            or FLC_HOSTID_TYPE_UNKNOWN if not available
 *  @param  serverIdValue     Pointer to variable to recieive the capability response server host id value
 *                            or NULL if not available
 *  @param  error             Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 * 
 *  @note Currently the server hostid is only available from a served buffer capability response
 *
 *  @note   The returned server id value string address is valid until the cabability response object is 
 *            deleted.
 */
FlcBool FlcCapabilityResponseGetServerId(FlcCapabilityResponseRef  response,
                                         FlcUInt32 *               serverIdType,
                                         const FlcChar **          serverIdValue,
                                         FlcErrorRef               error);

/**
 *  Get served time from response
 *
 *  @param  response          Capability response object of interest
 *  @param  servedTime        Pointer to variable to receive the capability response served time or
 *                            NULL if not available
 *  @param  error             Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Currently the served time is only available from a served buffer capability response
 *
 *  @note   The returned served time address is valid until the cabability response object is 
 *            deleted.
 */
FlcBool FlcCapabilityResponseGetServedTime(FlcCapabilityResponseRef  response,
                                           const time_t **           servedTime,
                                           FlcErrorRef               error);

/**
 *  Get correlation ID from response
 *
 *  @param  response          Capability response object of interest
 *  @param  correlationId     Pointer to variable to receive the correlation ID of the capability response.
 *                            Returns NULL if not set by the server
 *  @param  error             Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCapabilityResponseGetCorrelationId(FlcCapabilityResponseRef response,
                                              const FlcChar **         correlationId,
                                              FlcErrorRef              error);

/**
 *  Get confirmation request needed indicator from response
 *
 *  @param  response                      Capability response object of interest
 *  @param  confirmationRequestNeeded     Pointer to variable to receive the confirmation request needed
 *                                        indicator from the capability response.  Returns FLC_FALSE if not
 *                                        set by the server.
 *  @param  error                         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCapabilityResponseGetConfirmationRequestNeeded(FlcCapabilityResponseRef response,
                                                          FlcBool *                confirmationRequestNeeded,
                                                          FlcErrorRef              error);

/**
 *  Get clone suspect indicator from response
 *
 *  @param  response                      Capability response object of interest
 *  @param  cloneSuspect                  Pointer to variable to receive the clone suspect indicator from
 *                                        the capability response.  Returns FLC_FALSE if not set by the
 *                                        server.
 *  @param  error                         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCapabilityResponseGetCloneSuspect(FlcCapabilityResponseRef response,
                                             FlcBool *                cloneSuspect,
                                             FlcErrorRef              error);

/**
 *  Gets an indicator specifying whether or not this capability response was generated in response to
 *  a capability request with an operation type of FLC_CAPABILITY_REQUEST_OPERATION_PREVIEW.
 *
 *  @param  response                      Capability response object of interest
 *  @param  isPreview                     Pointer to variable to receive the 'is preview' indicator from
 *                                        the capability response.  Returns FLC_FALSE if not set by the
 *                                        server.
 *  @param  error                         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCapabilityResponseIsPreview(FlcCapabilityResponseRef response,
                                       FlcBool *                isPreview,
                                       FlcErrorRef              error);

/**
 *  Get trusted storage license server instance from response
 *
 *  @param  response                      Capability response object of interest
 *  @param  serverInstance                Pointer to variable to receive the license server instance value
 *                                        from the capability response.  Returns FLC_LICENSE_SERVER_UNKNOWN
 *                                        if not set by the server.
 *  @param  error                         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCapabilityResponseGetServerInstance(FlcCapabilityResponseRef    response,
                                               FlcLicenseServerInstance *  serverInstance,
                                               FlcErrorRef                 error);

/**
 *  Get UUID from response if present
 *
 *  @param  response    Capability response object of interest
 *  @param  uuid        Pointer to variable to receive UUID if present. Returns NULL if not set by the server
 *  @param  uuidSize    Pointer to variable to receive UUID size if present. Returns 0 if not set by the server
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCapabilityResponseGetUuid(FlcCapabilityResponseRef response,
                                     const FlcUInt8 **        uuid,
                                     FlcSize *                uuidSize,
                                     FlcErrorRef              error);

/**
 *  Get machine type from the capability response
 *
 *  @param  response    Capability response object of interest
 *  @param  type        Pointer to variable to receive machine type, see FlcMachineType for valid values 
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   FLC_MACHINE_TYPE_UNKNOWN is the default if a value is not present
 */
FlcBool FlcCapabilityResponseGetVirtualMachineType(FlcCapabilityResponseRef   response,
                                                   FlcMachineType *           type,
                                                   FlcErrorRef                error);

/**
 *  Gets a reference to virtual information dictionary in the capability response
 *
 *  @param  response    Capability response object of interest
 *  @param  dictionary  Pointer to variable to receive virtual machine information dictionary if present.
 *                      Returns NULL if not set by the server
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   If machine type is not FLC_MACHINE_TYPE_VIRTUAL, NULL will be returned.
 *
 *  @note   Lifetime of the returned dictionary depends on the lifetime of the
 *          corresponding response object.
 */
FlcBool FlcCapabilityResponseGetVirtualMachineInfo(FlcCapabilityResponseRef   response,
                                                   FlcDictionaryRef *         dictionary,
                                                   FlcErrorRef                error);

/**
 *  Gets a reference to the vendor dictionary in the capability response
 *
 *  @param  response    Capability response object of interest
 *  @param  dictionary  Pointer to variable to receive vendor dictionary if present. Returns NULL if not
 *                      set by the server
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   If machine type is not FLC_MACHINE_TYPE_VIRTUAL, NULL will be returned.
 *
 *  @note   Lifetime of the returned dictionary depends on the lifetime of the
 *            corresponding response object.
 */
FlcBool FlcCapabilityResponseGetVendorDictionary(FlcCapabilityResponseRef   response,
                                                 FlcDictionaryRef *         dictionary,
                                                 FlcErrorRef                error);

/**
 *  Gets a collection of features from the target capability response
 *
 *  @param  response           Capability response object of interest
 *  @param  featureCollection  Pointer to variable to receive feature collection. Returns NULL if not
 *                             set by the server
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info).
 *
 *  @note   Lifetime of the returned feature collection depends on the lifetime of the 
 *          corresponding response object. 
 *
 *  @note   While FlcFeatureCollectionDelete may be called on the returned feature collection 
 *          it will neither delete the feature collection nor raise an error. The feature collection
 *          storage will persist until the capability response object is deleted.
 */
FlcBool FlcCapabilityResponseGetFeatureCollection(FlcCapabilityResponseRef  response,
                                                  FlcFeatureCollectionRef * featureCollection,
                                                  FlcErrorRef               error);

/**
 *  Gets a collection of status information from the target capability response
 *
 *  @param  response             Capability response object of interest
 *  @param  statusCollection     Pointer to variable to receive status collection. Returns NULL if not
 *                               set by the server
 *  @param  error                Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info).
 *
 *  @note   Lifetime of the returned status collection depends on the lifetime of the 
 *          corresponding response object. 
 */
FlcBool FlcCapabilityResponseGetStatusCollection(FlcCapabilityResponseRef  response,
                                                 FlcStatusCollectionRef *  statusCollection,
                                                 FlcErrorRef               error);

/**
 *  Gets a collection of hostids from the target capability response
 *
 *  @param  response             Capability response object of interest
 *  @param  hostIds              Pointer to variable to receive hostIds collection. Returns NULL if not
 *                               set by the server
 *  @param  error                Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info).
 *
 *  @note   Lifetime of the returned hostIds collection depends on the lifetime of the 
 *          corresponding response object. 
 */
FlcBool FlcCapabilityResponseGetHostIds(FlcCapabilityResponseRef  response,
                                        FlcHostIdsRef *           hostIds, 
                                        FlcErrorRef               error);

/** @} */ /* end of CapabilityResponse group */
/** @} */ /* end of addtogroup licensingClient */


#ifdef __cplusplus
}
#endif

#endif /* _INCLUDE_FLC_CAPABILITYRESPONSE_H_ */
