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
 *  Provides an interface to get license feature information.

    The functions provided enable the caller to access a feature collection 
  and get   information about the individual feature items (INCREMENT line in license rights), 
    including name, version, expiration date, vendor string, etc.
 */

#ifndef _INCLUDE_FLC_FEATURE_H_
#define _INCLUDE_FLC_FEATURE_H_

#include <time.h>
#include "FlcTypes.h"
#include "FlcError.h"
#include "FlcLicenseSourceType.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup licensingClient
 * @{
 */

/**
 * @defgroup feature Feature
 * @{
 */

/** Uncounted feature count value.
*/
#define FLC_FEATURE_UNCOUNTED_VALUE 0x7fffffff

/** Opaque data type for the feature collection object.
*/
typedef struct flcFeatureCollection * FlcFeatureCollectionRef;

/** Opaque data type for the feature object.
*/
typedef struct flcFeature *           FlcFeatureRef;

/**
 *  Deletes the feature collection object
 *
 *  @param  featureCollection   Pointer to the variable to containing the reference to the feature collection object to be deleted
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcFeatureCollectionDelete(FlcFeatureCollectionRef  * featureCollection,
                                   FlcErrorRef                error);

/**
 *  Gets the size of the feature collection object
 *
 *  @param  featureCollection   Feature collection object of interest
 *  @param  size                Pointer to variable to receive returned size
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcFeatureCollectionSize(FlcFeatureCollectionRef featureCollection,
                                 FlcSize *               size,
                                 FlcErrorRef             error);

/**
 *  Gets the feature at a given index
 *
 *  @param  featureCollection   Feature collection object of interest
 *  @param  feature             Pointer to variable to receive returned feature reference
 *  @param  index               Index, ranging from 0 to (feature collection size - 1)
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Possible error codes include
 *      FLCERR_INDEX_OUT_OF_BOUND
 */
FlcBool FlcFeatureCollectionGet(FlcFeatureCollectionRef featureCollection,
                                FlcFeatureRef *         feature, 
                                FlcUInt32               index,
                                FlcErrorRef             error);

/**
 *  Find the feature matching the provided name starting at the provided index in the feature collection
 *
 *  @param  featureCollection   Feature collection object of interest
 *  @param  feature             Pointer to variable to receive returned feature reference or NULL if no
 *                              matching feature is found and on error
 *  @param  index               Pointer to variable to receive index of matching feature if found
 *  @param  featureName         Name of the feature to find
 *  @param  startingIndex       Starting search index
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info) or if no 
 *          matching feature is found
 *
 *  @note Possible error codes include
 *      FLCERR_FEATURE_NOT_FOUND 
 *      FLCERR_INDEX_OUT_OF_BOUND
 */
FlcBool FlcFeatureCollectionFind(FlcFeatureCollectionRef featureCollection,
                                 FlcFeatureRef *         feature,
                                 FlcUInt32 *             index,
                                 const FlcChar *         featureName,
                                 FlcUInt32               startingIndex,
                                 FlcErrorRef             error);

/**
 *  Gets the license source type for this feature.
 *
 *  @param  feature   Feature object of interest
 *  @param  source    Pointer to variable to receive returned feature source defined by FlcLicenseSourceType
 *  @param  error     Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcFeatureGetSource(FlcFeatureRef          feature,
                            FlcLicenseSourceType * source,
                            FlcErrorRef            error);

/**
 *  Gets the trusted storage license server instance for this feature
 *
 *  @param  feature         Feature object of interest
 *  @param  serverInstance  Pointer to variable to receive returned trusted storage license server instance
 *                          defined by FlcLicenseServerInstance.
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   This function will return FLC_LICENSE_SERVER_UNKNOWN for all features that do not 
 *          have a license source type of FLC_LICENSE_SOURCE_TRUSTED_STORAGE.
 */
FlcBool FlcFeatureGetServerInstance(FlcFeatureRef                feature,
                                    FlcLicenseServerInstance *   serverInstance,
                                    FlcErrorRef                  error);

/**
 *  Gets name of the feature
 *
 *  @param  feature   Feature object of interest
 *  @param  name      Pointer to variable to receive returned feature name
 *  @param  error     Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcFeatureGetName(FlcFeatureRef    feature, 
                          const FlcChar ** name,
                          FlcErrorRef      error);

/**
 *  Gets version of the feature
 *
 *  @param  feature   Feature object of interest
 *  @param  version   Pointer to variable to receive returned feature version
 *  @param  error     Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcFeatureGetVersion(FlcFeatureRef    feature, 
                             const FlcChar ** version,
                             FlcErrorRef      error);

/**
 *  Gets count from the feature line. For a preview feature this value reflects the current 
 *  count of this feature on the server available to the requesting client.
 *
 *  @param  feature   Feature object of interest
 *  @param  count     Pointer to variable to receive returned feature count
 *  @param  error     Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note For a preview feature this value is valid for the point in time the license
 *        server processed the client preview capability request. There is no guarantee
 *        that the full count will be available to the client if requested in a subsequent
 *        capability request as some of the available count may have been consumned by 
 *        another client in the time between the preview capability request and a 
 *        subsequent request for this feature.
 *  @note Sets count to FLC_FEATURE_UNCOUNTED_VALUE for an uncounted feature.
 */
FlcBool FlcFeatureGetCount(FlcFeatureRef    feature, 
                           FlcInt32 *       count,
                           FlcErrorRef      error);


/**
 *  Gets the count available for acquisition for a specific feature at the time the 
 *  feature collection is created.
 *
 *  @param  feature   Feature object of interest
 *  @param  count     Pointer to variable to receive returned feature available count
 *  @param  error     Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Sets count to FLC_FEATURE_UNCOUNTED_VALUE for an uncounted feature.
 */
FlcBool FlcFeatureGetAvailableAcquisitionCount(FlcFeatureRef    feature, 
                                               FlcInt32 *       count,
                                               FlcErrorRef      error);

/**
 *  Gets maximum count from the feature line
 *
 *  @param  feature   Feature object of interest
 *  @param  maxCount  Pointer to variable to receive the maximum count of this feature on the server
 *                    potentially available to the requesting client
 *  @param  error     Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note This value only has meaning for preview features.
 *  @note Sets count to FLC_FEATURE_UNCOUNTED_VALUE for an uncounted feature.
 */
FlcBool FlcFeatureGetMaxCount(FlcFeatureRef    feature, 
                              FlcInt32 *       maxCount,
                              FlcErrorRef      error);

/**
 *  Detects whether the feature uses a metered license model
 *
 *  @param  feature     Feature object of interest
 *  @param  isMetered   Pointer to variable to receive returned metered indicator. This indicator
 *                      will be set to FLC_TRUE for a metered feature (MODEL=metered); FLC_FALSE
 *                      otherwise and on error
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcFeatureIsMetered(FlcFeatureRef    feature, 
                            FlcBool *        isMetered,
                            FlcErrorRef      error);

/**
 *  Detects whether the feature that uses a metered license model is reusable
 *
 *  @param  feature             Feature object of interest
 *  @param  isMeteredReusable   Pointer to variable to receive returned reusable metered indicator.
 *                              This indicator will be set to FLC_TRUE for a reusable metered feature
 *                              (MODEL=metered REUSABLE); FLC_FALSE otherwise and on error 
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcFeatureIsMeteredReusable(FlcFeatureRef    feature, 
                                    FlcBool *        isMeteredReusable,
                                    FlcErrorRef      error);

/**
 *  Returns whether or not the feature is uncounted
 *
 *  @param  feature      Feature object of interest
 *  @param  isUncounted  Pointer to variable to receive returned uncounted indicator which
 *                       is set to FLC_TRUE if the feature is uncounted, and FLC_FALSE if 
 *                       the feature has a count value
 *  @param  error        Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   Uncounted features contain a count value of FLC_FEATURE_UNCOUNTED_VALUE or 0x7fffffff.
 */
FlcBool FlcFeatureIsUncounted(FlcFeatureRef feature,
                              FlcBool *     isUncounted,
                              FlcErrorRef   error);


/**
 *  Returns whether or not the feature is a preview feature. A preview feature is returned as part
 *  of the response to a preview capability request.
 *
 *  @param  feature      Feature object of interest
 *  @param  isPreview    Pointer to variable to receive returned preview feature indicator which
 *                       is set to FLC_TRUE if the feature has been returned in response to a 
 *                       preview capability request, and FLC_FALSE otherwise
 *  @param  error        Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcFeatureIsPreview(FlcFeatureRef feature,
                            FlcBool *     isPreview,
                            FlcErrorRef   error);

/**
 *  Returns whether or not the feature is a served feature. A served feature is a feature
 *  that has been served from a local license server.
 *
 *  @param  feature      Feature object of interest
 *  @param  isServed     Pointer to variable to receive returned served feature indicator which
 *                       is set to FLC_TRUE if the feature is a served feature and FLC_FALSE otherwise
 *  @param  error        Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcFeatureIsServed(FlcFeatureRef feature,
                            FlcBool *     isServed,
                            FlcErrorRef   error);

/**
 *  Checks whether feature is valid for serving
 *
 *  @param  feature   Feature object of interest
 *  @param  status    Pointer to variable to receive returned status which is set to 
 *                    FLCERR_SUCCESS for a valid feature, and a specific error code 
 *                    for an invalid feature
 *  @param  error     Error object to be filled with error information in the
 *                            case of failure
 *  @return FLC_TRUE on success; FLC_FALSE otherwise
 *
 *  @note   Feature is considered valid for serving if it passes signature and format
 *          validation, the current date is between start and expiration dates,
 *          and there is a match for the server hostid.
 */
FlcBool FlcFeatureGetValidStatusForServing(FlcFeatureRef    feature, 
                                           FlcInt32 *       status,
                                           FlcErrorRef      error);

/**
 *  Checks whether feature is valid for acquisition
 *
 *  @param  feature  Feature object of interest
 *  @param  status   Pointer to variable to receive returned status which is set to 
 *                   FLCERR_SUCCESS for a valid feature, and a specific error code 
 *                   for an invalid feature
 *  @param  error    Error object to be filled with error information in the
 *                   case of failure
 *  @return FLC_TRUE on success; FLC_FALSE otherwise
 *
 *  @note   Feature is considered valid for acquisition if it passes signature and format
 *          validation, the current date is between the start and expiration dates,
 *          the device id matches the device, and it is not a preview feature.
 *
 *  @note   This function does not indicate whether the clock windback is detected. You can use
 *          the FlcClockWindbackDetected function for that purpose.
 *
 */
FlcBool FlcFeatureGetValidStatusForAcquisition(FlcFeatureRef    feature, 
                                               FlcInt32 *       status,
                                               FlcErrorRef      error);

/**
 *  Gets serial number string of the feature
 *
 *  @param  feature       Feature object of interest
 *  @param  serialNumber  Pointer to variable to receive returned feature serial number if present,
 *                        NULL otherwise
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcFeatureGetSerialNumber(FlcFeatureRef     feature, 
                                  const FlcChar **  serialNumber,
                                  FlcErrorRef       error);

/**
 *  Gets notice string of the feature
 *
 *  @param  feature   Feature object of interest
 *  @param  notice    Pointer to variable to receive returned feature notice string if present,
 *                    NULL otherwise
 *  @param  error     Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcFeatureGetNotice(FlcFeatureRef     feature, 
                            const FlcChar **  notice,
                            FlcErrorRef       error);

/**
 *  Gets issuer string of the feature
 *
 *  @param  feature   Feature object of interest
 *  @param  issuer    Pointer to variable to receive returned feature issuer string (ISSUER value)
 *                    if present, NULL otherwise
 *  @param  error     Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcFeatureGetIssuer(FlcFeatureRef     feature, 
                            const FlcChar **  issuer,
                            FlcErrorRef       error);

/**
 *  Gets vendor string of the feature
 *
 *  @param  feature       Feature object of interest
 *  @param  vendorString  Pointer to variable to receive returned feature vendor string
 *                        (VENDOR_STRING value) if present, NULL otherwise
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcFeatureGetVendorString(FlcFeatureRef     feature, 
                                  const FlcChar **  vendorString,
                                  FlcErrorRef       error);

/**
 *  Gets a reference to the issued date of the feature
 *
 *  @param  feature   Feature object of interest
 *  @param  date      Pointer to variable to receive returned feature isseued date
 *                    (ISSUED value) if present, NULL otherwise
 *  @param  error     Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcFeatureGetIssued(FlcFeatureRef       feature, 
                            const struct tm **  date,
                            FlcErrorRef         error);

/**
 *  Gets a reference to the expiration date of the feature
 *
 *  @param  feature   Feature object of interest
 *  @param  date      Pointer to variable to receive returned feature expriation date or
 *                    NULL in case of error
 *  @param  error     Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcFeatureGetExpiration(FlcFeatureRef       feature, 
                                const struct tm **  date,
                                FlcErrorRef         error);

/**
 *  Gets the definitive expiration of a feature as defined by the back-office. This value
 *  could potentially be later than the entitlement expiration thus defining a grace period 
 *  in which a feature from an expired entitlement can continue to be used. This value might
 *  or might not be the same as the value returned by FlcFeatureGetExpiration, which is based
 *  on how the feature was obtained.
 *
 *  @param  feature   Feature object of interest
 *  @param  date      Pointer to variable to receive the final feature expiration date, or NULL
 *                    if an error is encountered.
 *  @param  error     Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   Use the FlxDateIsPerpetual function on the returned final expiration date value to
 *          determine whether or not this value represents a perpetual feature.
 */
FlcBool FlcFeatureGetFinalExpiration(FlcFeatureRef       feature, 
                                     const struct tm **  date,
                                     FlcErrorRef         error);

/**
 *  Gets the expiration of the back office entitlement associated with this feature.
 *
 *  @param  feature   Feature object of interest
 *  @param  date      Pointer to variable to receive the entitlement expiration date, or NULL
 *                    if an error is encountered.
 *  @param  error     Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   Use the FlxDateIsPerpetual function on the returned entitlement expiration date
 *          value to determine whether or not this value represents a perpetual entitlement.
 */
FlcBool FlcFeatureGetEntitlementExpiration(FlcFeatureRef       feature, 
                                           const struct tm **  date,
                                           FlcErrorRef         error);

/**
 *  Gets a reference to the start date of the feature
 *
 *  @param  feature   Feature object of interest
 *  @param  date      Pointer to variable to receive returned feature start date
 *                    (START value) if present, NULL otherwise
 *  @param  error     Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcFeatureGetStartDate(FlcFeatureRef       feature, 
                               const struct tm **  date,
                               FlcErrorRef         error);

/**
 *  Returns whether or not the feature is currently within the specified entitlement grace period.
 *
 *  @param  feature         Feature object of interest
 *  @param  isInGracePeriod Pointer to variable to receive returned grace period indicator which
 *                          is set to FLC_TRUE if the feature is expired but within the  
 *                          entitlement grace period and FLC_FALSE otherwise.
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcFeatureIsInGracePeriod(FlcFeatureRef       feature,
                                  FlcBool *           isInGracePeriod,
                                  FlcErrorRef         error);

/**
 *  Returns whether or not the feature is permanent.
 *
 *  @param  feature      Feature object of interest
 *  @param  isPerpetual  Pointer to variable to receive returned perpetual indicator which
 *                       is set to FLC_TRUE if the feature is permanent, and FLC_FALSE if 
 *                       the feature contains a real expiration date
 *  @param  error        Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   This is a convenience function. The same determination can be made by calling
 *          FlcDateIsPerpetual on the value returned from FlcFeatureGetExpiration.
 */
FlcBool FlcFeatureIsPerpetual(FlcFeatureRef       feature,
                              FlcBool *           isPerpetual,
                              FlcErrorRef         error);

/**
 *  Gets a value representing the non-reusable metered feature undo interval in seconds.
 *
 *  @param  feature      Feature object of interest
 *  @param  undoSeconds  Pointer to variable to receive the feature undo interval
 *                       (MODEL=metered UNDO_INTERVAL=value) in seconds or 0 if the feature
 *                       is not metered or is metered reusable and on error.
 *  @param  error        Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info).
 */
FlcBool FlcFeatureGetMeteredUndoInterval(FlcFeatureRef     feature, 
                                         FlcUInt32 *       undoSeconds,
                                         FlcErrorRef       error);

/**
 *  Gets a value representing the number of seconds remaining in the non-reusable metered feature
 *  undo interval.
 *
 *  @param  feature               Feature object of interest
 *  @param  undoSecondsRemaining  Pointer to variable to receive the number of seconds remaining
 *                                in the feature undo interval (MODEL=metered UNDO_INTERVAL=value).
 *                                Returns 0 if the feature has no undo interval or if the undo 
 *                                interval has expired and on error.
 *  @param  error                 Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info).
 */
FlcBool FlcFeatureGetSecondsLeftInUndoInterval(FlcFeatureRef   feature,
                                               FlcUInt32 *     undoSecondsRemaining,
                                               FlcErrorRef     error);

/**
 *  Gets a collection of hostids from the target feature
 *
 *  @param  feature      Feature object of interest
 *  @param  hostIds      Pointer to variable to receive hostIds collection. Returns NULL if not
 *                       set and on error
 *  @param  error        Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info).
 *  @note   Lifetime of the returned hostIds collection depends on the lifetime of the 
 *          corresponding feature object. 
 */
FlcBool FlcFeatureGetHostIds(FlcFeatureRef       feature,
                             FlcHostIdsRef *     hostIds, 
                             FlcErrorRef         error);

/** @} */ /* end of feature group */
/** @} */ /* end of addtogroup licensingClient */

#ifdef __cplusplus
}
#endif

#endif /* _INCLUDE_FLC_FEATURE_H_ */
