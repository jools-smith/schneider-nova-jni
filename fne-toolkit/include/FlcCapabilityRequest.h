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
 *  Provides the interface to the licensing capability request object.
 *
 *  These functions enable the caller to add vendor dictionary data, rights IDs,
 *  and "force response" flag to the licensing capability request object along
 *  with many other request options. Finally the tranmittable binary form of
 *  the capability request may be generated.
 */

#ifndef _INCLUDE_FLC_CAPABILITYREQUEST_H_
#define _INCLUDE_FLC_CAPABILITYREQUEST_H_

#include "FlcTypes.h"
#include "FlcError.h"
#include "FlcCapabilityRequestOperation.h"
#include "FlcExpirationValidationInterval.h"
#include "FlcDesiredFeatureOptions.h"
#include "FlcRightsIdOptions.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup licensingClient
 * @{
 */

 /**
 * @defgroup CapabilityRequest Capability Request
 * @{
 */

/** Opaque data type for the capability request object.
*/
typedef struct flcCapabilityRequest * FlcCapabilityRequestRef;


/**
 *  Creates the capability request object
 *
 *  @param  licensing   Licensing environment object
 *  @param  request     Pointer to the variable to receive the reference to the capability request object to be created
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   Capability request is created with the following default settings:
 *          empty vendor dictionary information, empty list of rights IDs,
 *          "force response", "one-time activation", "include dictionary" and "served buffer"
 *          flags set to FLC_FALSE.
 *  @note   The caller is responsible for deleting the capability request object via the
 *          FlcCapabilityRequestDelete API.
 */
FlcBool FlcCapabilityRequestCreate(FlcLicensingRef            licensing,
                                   FlcCapabilityRequestRef *  request,
                                   FlcErrorRef                error);

/**
 *  Deletes the capability request object
 *
 *  @param  licensing   Licensing environment object
 *  @param  request     Pointer to the variable containing the reference to the capability request object to be deleted
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCapabilityRequestDelete(FlcLicensingRef            licensing,
                                   FlcCapabilityRequestRef *  request,
                                   FlcErrorRef                error);


/**
 *  Sets the force response flag in the capability request object.
 *
 *  @param  licensing       Licensing environment object
 *  @param  request         Capability request object whose response flag is to be set
 *  @param  forceResponse   Flag to be set
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The default value is FLC_FALSE.
 *  @note   Sets the "force response" flag, indicating that the FlexNet Embedded client
 *          always requires a capability response.  The typical behavior of the back office
 *          is to send a capability response only if the client's license rights have changed
 *          since the last capability exchange. However, when the "force response" flag is
 *          included in the capability request, the back office always sends a capability
 *          response even if the client's license rights have not changed. Use this flag with
 *          caution, mostly for exception cases such as to restore current license rights
 *          should client trusted storage be deleted due to corruption or to resolve
 *          synchronization timestamp issues.
 *
 *          The local license server and the CLS license server ignore the "force response"
 *          flag and always send a capability response.
 *
 *          Note that, if a capability request is sent to a CLS license server and client
 *          registration is enabled in the back office, the CLS license server makes an
 *          additional, one-time call to the back office to register the client before
 *          sending the initial capability response to the client. However, if the
 *          "force response" flag is included, a call to the back office is made each time
 *          a capability request is sent to the CLS license server, resulting in increased
 *          response time. Hence, using the flag in this situation is strongly discouraged
 *          and typically unnecessary.
 *  @note   This flag can be used for time synchronization between the client and the server.
 */
FlcBool FlcCapabilityRequestSetForceResponse(FlcLicensingRef         licensing,
                                             FlcCapabilityRequestRef request,
                                             FlcBool                 forceResponse,
                                             FlcErrorRef             error);

/**
 *  Sets the force UUID flag in the capability request object
 *
 *  @param  licensing         Licensing environment object
 *  @param  request           Capability request object whose force-UUID flag is to be set
 *  @param  forceIncludeUuid  Flag to be set
 *  @param  error             Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The default value is FLC_FALSE.
 *  @note     The "force UUID" flag indicates that the client requests the back-office server
 *          to generate a UUID value if one has not already been assigned to the client.
 *          Normally, only some hostid types employ UUID values in capability requests
 *          and responses for the purposes of system identification. This flag indicates
 *          to use a UUID value for any hostid type, which may change the behavior of the
 *          back-office server or a local license server.
 *  @note     Returns FLCERR_UUID_SOURCE_CONFLICT if forceUuid is set to FLC_TRUE and
 *              FlcCapabilityRequestSetUuid was previously called.
 *  @note   A value of FLC_TRUE for the "force UUID" flag is incompatible with a value of
 *          FLC_TRUE for the "Served Buffer" flag.
 *
 *  @note   Possible error codes include
 *          FLCERR_UUID_SOURCE_CONFLICT
 *          FLCERR_CAPABILITY_REQUEST_OPTION_CONFLICT
 */
FlcBool FlcCapabilityRequestSetForceIncludeUuid(FlcLicensingRef         licensing,
                                                FlcCapabilityRequestRef request,
                                                FlcBool                 forceIncludeUuid,
                                                FlcErrorRef             error);

/**
 *  Sets "include vendor dictionary" flag in the capability request
 *
 *  @param  licensing               Licensing environment object
 *  @param  request                 Capability request object to set the flag on
 *  @param  includeVendorDictionary The boolean value of the flag. Setting the flag to true indicates to
 *                                  the license server that the client is interested in receiving vendor dictionary
 *                                  in the capability response. Setting the flag to false indicates that the
 *                                  client is not capable of or interested in processing any vendor dictionary data
 *                                  contained in the capability response.
 *  @param  error                   Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The default value is FLC_FALSE.
 *  @note   The "include vendor dictionary" flag is irrelevant to the capability request itself
 *  @note   The "include vendor dictionary" flag is irrelevant in capability requests sent to the back office
 *  @note   This function is incompatible with the FlcCapabilityRequestAddIncludeVendorDictionaryKey function
 */
FlcBool FlcCapabilityRequestSetIncludeVendorDictionary(FlcLicensingRef         licensing,
                                                       FlcCapabilityRequestRef request,
                                                       FlcBool                 includeVendorDictionary,
                                                       FlcErrorRef             error);

/**
 *  Sets the requested feature borrow interval, in seconds, to be sent to the license server.
 *
 *  @param  licensing        Licensing environment object
 *  @param  request          Capability request object on which to set the borrow interval
 *  @param  borrowInterval   Borrow interval to be set
 *  @param  error            Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   This value works with the borrow granularity rounding value and together they are
 *          considered by the license server to determine the feature expiration value returned
 *          in the capability response. The returned value may be less than the requested value
 *          if the maximum allowed borrow interval defined on the license server is exceeded by
 *          this specification.
 *  @note   The interval defaults to zero if only granularity is set by calling
 *          FlcCapabilityRequestSetBorrowGranularity.
 *  @note   This function is incompatible with the function FlcCapabilityRequestSetOneTimeActivation.
 */
FlcBool FlcCapabilityRequestSetBorrowInterval(FlcLicensingRef         licensing,
                                              FlcCapabilityRequestRef request,
                                              FlcUInt32               borrowInterval,
                                              FlcErrorRef             error);

/**
 *  Sets the requested feature borrow interval expiration granularity to be sent to the license server.
 *
 *  @param  licensing        Licensing environment object
 *  @param  request          Capability request object on which to set the borrow granularity
 *  @param  granularity      Granularity type to be set
 *  @param  error            Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   This value works with the borrow interval and together they are considered by the
 *          license server to determine the feature expiration value returned in the capability
 *          response. The returned value may be less than the requested value rounded to the
 *          specified borrow granularity if the maximum allowed borrow interval defined on the 
 *          server is exceeded by the result of this rounding operation.
 *  @note   The borrow interval will be rounded up to end on an even multiple of the borrow
 *          granularity unit of time. For example, if the borrow interval of 2 seconds is
 *          specified with a borrow granularity of FLC_EVI_HOUR, the license server will return
 *          a feature with an expiration which is 2 seconds in the future rounded to the
 *          end of the specified borrow granularity unit of an hour.
 *  @note   The granularity defaults to FLC_EVI_DAY if only the borrow interval is set
 *          by calling FlcCapabilityRequestSetBorrowInterval.
 *  @note   This function is incompatible with the function FlcCapabilityRequestSetOneTimeActivation.
 */
FlcBool FlcCapabilityRequestSetBorrowGranularity(FlcLicensingRef                   licensing,
                                                 FlcCapabilityRequestRef           request,
                                                 FlcExpirationValidationInterval   granularity,
                                                 FlcErrorRef                       error);

/**
 *  Adds a string vendor dictionary item to the capability request object
 *
 *  @param  licensing        Licensing environment object
 *  @param  request          Capability request object to which to add the vendor dictionary item
 *  @param  key              Vendor dictionary item key
 *  @param  value            Vendor dictionary item value
 *  @param  error            Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCapabilityRequestAddVendorDictionaryStringItem(FlcLicensingRef         licensing,
                                                          FlcCapabilityRequestRef request,
                                                          const FlcChar *         key,
                                                          const FlcChar *         value,
                                                          FlcErrorRef             error);

/**
 *  Adds an integer vendor dictionary item to the capability request object
 *
 *  @param  licensing        Licensing environment object
 *  @param  request          Capability request object to which to add the vendor dictionary item
 *  @param  key              Vendor dictionary item key
 *  @param  value            Vendor dictionary item value
 *  @param  error            Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCapabilityRequestAddVendorDictionaryIntItem(FlcLicensingRef         licensing,
                                                       FlcCapabilityRequestRef request,
                                                       const FlcChar *         key,
                                                       FlcInt32                value,
                                                       FlcErrorRef             error);
/**
 *  Removes a vendor dictionary item from the capability request object
 *
 *  @param  licensing        Licensing environment object
 *  @param  request          Capability request object from which to remove the vendor dictionary item
 *  @param  key              Vendor dictionary item key for item to remove
 *  @param  error            Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCapabilityRequestRemoveVendorDictionaryItem(FlcLicensingRef         licensing,
                                                       FlcCapabilityRequestRef request,
                                                       const FlcChar *         key,
                                                       FlcErrorRef             error);
/**
 *  Removes all items from the vendor dictionary associated with the capability request object
 *
 *  @param  licensing        Licensing environment object
 *  @param  request          Capability request object from which to remove all vendor dictionary items
 *  @param  error            Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCapabilityRequestClearVendorDictionary(FlcLicensingRef         licensing,
                                                  FlcCapabilityRequestRef request,
                                                  FlcErrorRef             error);

/**
 *  Adds a vendor dictionary key that the client is interested in receiving in the capability response
 *
 *  @param  licensing        Licensing environment object
 *  @param  request          Capability request object to which to add the requested vendor dictionary key
 *  @param  key              Capability response vendor dictionary key of interest
 *  @param  error            Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note     Setting the key is irrelevant in capability requests sent to the back office.
 *  @note     This function is incompatible with the FlcCapabilityRequestSetIncludeVendorDictionary function.
 *  @note     This function can be called multiple times to create a set of keys that the client is interested to
 *            receive in the capability response.
*/
FlcBool FlcCapabilityRequestAddIncludeVendorDictionaryKey(FlcLicensingRef         licensing,
                                                          FlcCapabilityRequestRef request,
                                                          const FlcChar *         key,
                                                          FlcErrorRef             error);

/**
 *  Removes a vendor dictionary key from the list of keys that the client is interested in receiving in the
 *  capability response
 *
 *  @param  licensing        Licensing environment object
 *  @param  request          Capability request object to which to add the requested vendor dictionary key
 *  @param  key              Capability response vendor dictionary key to remove
 *  @param  error            Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
*/
FlcBool FlcCapabilityRequestRemoveIncludeVendorDictionaryKey(FlcLicensingRef         licensing,
                                                             FlcCapabilityRequestRef request,
                                                             const FlcChar *         key,
                                                             FlcErrorRef             error);

/**
 *  Clears the list of vendor dictionary keys that the client is interested in receiving in the
 *  capability response
 *
 *  @param  licensing        Licensing environment object
 *  @param  request          Capability request object from which to remove all requested capability
 *                           response vendor dictionary key items
 *  @param  error            Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
*/
FlcBool FlcCapabilityRequestClearIncludeVendorDictionaryKeys(FlcLicensingRef         licensing,
                                                             FlcCapabilityRequestRef request,
                                                             FlcErrorRef             error);

/**
 * Set capability request one-time activation indicator
 *
 *  @param  licensing        Licensing environment object
 *  @param  request          Capability request object whose one-time activation flag is to be set
 *  @param  activation       FLC_TRUE to enable activation, FLC_FALSE to disable
 *  @param  error            Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The default value is FLC_FALSE.
 *  @note   One-time activation is ignored in capability requests sent to a back-office server.
 *  @note   This function is incompatible with the functions FlcCapabilityRequestSetBorrowInterval
 *          and FlcCapabilityRequestSetBorrowGranularity.
 *  @note   This value, if set to true, is incompatible with an operation type of
 *          FLC_CAPABILITY_REQUEST_OPERATION_PREVIEW.
 *
 *  @note   Possible error codes include
 *          FLCERR_CAPABILITY_REQUEST_OPTION_CONFLICT
 */
FlcBool FlcCapabilityRequestSetOneTimeActivation(FlcLicensingRef         licensing,
                                                 FlcCapabilityRequestRef request,
                                                 FlcBool                 activation,
                                                 FlcErrorRef             error);

/**
 * Set capability request served buffer indicator. If set to FLC_TRUE the capability response returned
 * from the local or back-office server should be a served buffer capability response
 *
 *  @param  licensing        Licensing environment object
 *  @param  request          Capability request object whose served buffer flag is to be set
 *  @param  servedBuffer     FLC_TRUE to enable a served buffer response, FLC_FALSE to disable
 *  @param  error            Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The default value is FLC_FALSE.
 *  @note   A value of FLC_TRUE for the "Served Buffer" flag is incompatible with any value other
 *          than FLC_CAPABILITY_REQUEST_OPERATION_REQUEST for the capability request operation.
 *  @note   A value of FLC_TRUE for the "Served Buffer" flag is incompatible with a value of
 *          FLC_TRUE for the "force UUID" flag.
 *
 *  @note   Possible error codes include
 *          FLCERR_CAPABILITY_REQUEST_OPTION_CONFLICT
 */
FlcBool FlcCapabilityRequestSetServedBuffer(FlcLicensingRef         licensing,
                                            FlcCapabilityRequestRef request,
                                            FlcBool                 servedBuffer,
                                            FlcErrorRef             error);

/**
 *  Set capability request incremental indicator. If set to FLC_TRUE indicates that this is an
 *  incremental request for the specified features.
 *
 *  @param  licensing       Licensing environment object
 *  @param  request         Capability request object for which the incremental flag is to be set
 *  @param  incremental     FLX_TRUE to enable incremental flag in request, FLX_FALSE to disable it
 *  @param  error           Error object to be filled with error information in case of failure
 *  @return FLX_TRUE on success, FLX_FALSE on error (see error for additional error info).
 *
 *  @note   The default incremental indicator value is FLC_FALSE.
 *  @note   The incremental property is ignored in capability requests sent to a back-office server.
 *  @note   A value of FLC_TRUE for the "Incremental" flag is incompatible with all operation values
 *          except FLC_CAPABILITY_REQUEST_OPERATION_REQUEST.
 *  @note   This function is used in conjunction with desired feature(s) only.  A desired feature
 *          with a positive count will be added to the client's feature set whereas a desired
 *          feature with a negative count will be subtracted from the clients feature set.
 *  @note   Possible error codes include
 *          FLCERR_CAPABILITY_REQUEST_OPTION_CONFLICT
 */
FlcBool FlcCapabilityRequestSetIncremental(FlcLicensingRef         licensing,
                                           FlcCapabilityRequestRef request,
                                           FlcBool                 incremental,
                                           FlcErrorRef             error);

/**
 * Sets a value which, if set to FLC_TRUE, indicates that the preview capability request is
 * to return information on all available features.
 *
 *  @param  licensing        Licensing environment object
 *  @param  request          Capability request object for which the 'request all features' flag is to be set
 *  @param  requestAll       FLC_TRUE to indicate preview information is to be returned for all available
 *                           reatures, FLC_FALSE otherwise
 *  @param  error            Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   This property, if set to true, is incompatible with all operation values except
 *          FLC_CAPABILITY_REQUEST_OPERATION_PREVIEW.
 *  @note   This property, if set to true, is incompatible with the specification of desired features.
 *
 *  @note   Possible error codes include
 *          FLCERR_CAPABILITY_REQUEST_OPTION_CONFLICT
 */
FlcBool FlcCapabilityRequestSetRequestAllFeatures(FlcLicensingRef         licensing,
                                                  FlcCapabilityRequestRef request,
                                                  FlcBool                 requestAll,
                                                  FlcErrorRef             error);

/**
 *  Adds a feature selector item to the capability request object. Can be called
 *  multiple times
 *
 *  @param  licensing  Licensing environment object
 *  @param  request    Capability request object to which to add the feature selector item
 *  @param  key        Feature selector key
 *  @param  value      Feature selector value
 *  @param  error      Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   This function can be called multiple times to add multiple feature selector items.
 *  @note   This function is incompatible with all operation values except
 *          FLC_CAPABILITY_REQUEST_OPERATION_REQUEST or FLC_CAPABILITY_REQUEST_OPERATION_PREVIEW.
 *
 *  @note   Possible error codes include
 *          FLCERR_CAPABILITY_REQUEST_OPTION_CONFLICT
 */
FlcBool FlcCapabilityRequestAddFeatureSelectorStringItem(FlcLicensingRef         licensing,
                                                         FlcCapabilityRequestRef request,
                                                         const FlcChar *         key,
                                                         const FlcChar *         value,
                                                         FlcErrorRef             error);

/**
 *  Removes a feature selector item from the capability request object
 *
 *  @param  licensing  Licensing environment object
 *  @param  request    Capability request object from which to remove the feature selector item
 *  @param  key        Feature selector item key to remove
 *  @param  error      Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCapabilityRequestRemoveFeatureSelectorItem(FlcLicensingRef         licensing,
                                                      FlcCapabilityRequestRef request,
                                                      const FlcChar *         key,
                                                      FlcErrorRef             error);

/**
 *  Removes all feature selector items associated with the capability request object
 *
 *  @param  licensing  Licensing environment object
 *  @param  request    Capability request object from which to remove all feature selector items
 *  @param  error      Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCapabilityRequestClearFeatureSelectors(FlcLicensingRef         licensing,
                                                  FlcCapabilityRequestRef request,
                                                  FlcErrorRef             error);

/**
 *  Adds a auxiliary host ID to the capability request object.
 *  Can be called multiple times.
 *
 *  @param  licensing     Licensing environment object
 *  @param  request       Capability request object to which the auxiliary host ID is to be added.
 *  @param  type          Host ID type.
 *  @param  id            Host ID value.
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   Host ID types of FLC_HOSTID_TYPE_UNKNOWN, FLC_HOSTID_TYPE_UNSUPPORTED, and
 *          FLC_HOSTID_TYPE_ANY are not supported for auxiliary host IDs.
 */
FlcBool FlcCapabilityRequestAddAuxiliaryHostId(FlcLicensingRef         licensing,
                                               FlcCapabilityRequestRef request,
                                               FlcHostIdType           type,
                                               const FlcChar *         id,
                                               FlcErrorRef             error);

/**
 *  Adds a rights ID to the capability request object. Can be called multiple times.
 *
 *  @param  licensing  Licensing environment object
 *  @param  request    Capability request object to which to add the rights ID
 *  @param  rightsId   Rights ID to be added.
 *  @param  count      Number of copies
 *  @param  error      Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   This function adds an additional rights ID to the capability request
 *          object.
 *  @note   Rights IDs apply only to capability requests sent to a back-office server.
 *  @note   Specification of rights IDs is incompatible with an operation type of
 *          FLC_CAPABILITY_REQUEST_OPERATION_REPORT.
 *
 *  @note   Possible error codes include
 *          FLCERR_CAPABILITY_REQUEST_OPTION_CONFLICT
 */
FlcBool FlcCapabilityRequestAddRightsId(FlcLicensingRef         licensing,
                                        FlcCapabilityRequestRef request,
                                        const FlcChar *         rightsId,
                                        FlcInt32                count,
                                        FlcErrorRef             error);

/**
 *  Adds a rights ID with options to the capability request object. Can be called multiple times.
 *
 *  @param  licensing  Licensing environment object
 *  @param  request    Capability request object to which to add the rights ID
 *  @param  rightsId   Rights ID to be added.
 *  @param  count      Number of copies
 *  @param  options    FlcRightsIdOptionsRef containing the options to apply to this rights ID
 *  @param  error      Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   This function adds an additional rights ID to the capability request object.
 *  @note   Rights IDs apply only to capability requests sent to a back-office server.
 *  @note   Specification of rights IDs with options is incompatible with all operation types
 *          except FLC_CAPABILITY_REQUEST_OPERATION_REQUEST.
 *
 *  @note   Possible error codes include
 *          FLCERR_CAPABILITY_REQUEST_OPTION_CONFLICT
 */
FlcBool FlcCapabilityRequestAddRightsIdWithOptions(FlcLicensingRef          licensing,
                                                   FlcCapabilityRequestRef  request,
                                                   const FlcChar *          rightsId,
                                                   FlcInt32                 count,
                                                   FlcRightsIdOptionsRef    options,
                                                   FlcErrorRef              error);

/**
 *  Adds a desired feature to the capability request object. This API can be called multiple times.
 *
 *  @param  licensing       Licensing environment object
 *  @param  request         Capability request object to add desired feature to
 *  @param  featureName     Desired feature name
 *  @param  featureVersion  Desired feature version
 *  @param  featureCount    Desired feature count
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   Currently there is no validation for the feature information. The same feature name
 *          and version can be added multiple times.
 *  @note   The order in which feature information is added to the capability request is important
 *          because it might affect the generation of capability response by the server object.
 *  @note   An explicit desired feature count of 0 (zero) indicates that none of that feature should
 *          be served to the device, even if the feature appears in the server's reservation list.
 *  @note   When the incremental flag is FLC_TRUE for the request, a negative count for a feature
 *          can be used to indicate a deduction of the client's current count for that feature.
 *  @note   Not compatible with undo (FLC_CAPABILITY_REQUEST_OPERATION_UNDO) operation.
 *  @note   For an operation type of FLC_CAPABILITY_REQUEST_OPERATION_PREVIEW this function
 *          is incompatible with a 'request all features' specification of FLC_TRUE.
 *
 *  @note   Possible error codes include
 *          FLCERR_VERSION_STRING_INVALID
 *          FLCERR_CAPABILITY_REQUEST_OPTION_CONFLICT
 */
FlcBool FlcCapabilityRequestAddDesiredFeature(FlcLicensingRef         licensing,
                                              FlcCapabilityRequestRef request,
                                              const FlcChar *         featureName,
                                              const FlcChar *         featureVersion,
                                              FlcInt32                featureCount,
                                              FlcErrorRef             error);

/**
 *  Adds a desired feature with options to the capability request object. Can be called multiple times.
 *
 *  @param  licensing       Licensing environment object
 *  @param  request         Capability request object to add desired feature to
 *  @param  featureName     Desired feature name
 *  @param  featureVersion  Desired feature version
 *  @param  featureCount    Desired feature count
 *  @param  options         FlcDesiredFeatureOptionsRef containing the options to apply to this desired feature
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   Currently there is no validation for the feature information. The same feature name
 *          and version can be added multiple times.
 *  @note   The order in which feature information is added to the capability request is important
 *          because it might affect the generation of capability response by the server object.
 *  @note   An explicit desired feature count of 0 (zero) indicates that none of that feature should
 *          be served to the device, even if the feature appears in the server's reservation list.
 *  @note   When the incremental flag is FLC_TRUE for the request, a negative count for a feature
 *          can be used to indicate a deduction of the client's current count for that feature.
 *  @note   Not compatible with undo (FLC_CAPABILITY_REQUEST_OPERATION_UNDO) operation.
 *  @note   An options object with partial fulfillment set to FLC_TRUE is only compatible with
 *          a request (FLC_CAPABILITY_REQUEST_OPERATION_REQUEST) operation.
 *  @note   For an operation type of FLC_CAPABILITY_REQUEST_OPERATION_PREVIEW this function
 *          is incompatible with a 'request all features' specification of FLC_TRUE.
 *
 *  @note   Possible error codes include
 *          FLCERR_VERSION_STRING_INVALID
 *          FLCERR_CAPABILITY_REQUEST_OPTION_CONFLICT
 */
FlcBool FlcCapabilityRequestAddDesiredFeatureWithOptions(FlcLicensingRef             licensing,
                                                         FlcCapabilityRequestRef     request,
                                                         const FlcChar *             featureName,
                                                         const FlcChar *             featureVersion,
                                                         FlcInt32                    featureCount,
                                                         FlcDesiredFeatureOptionsRef options,
                                                         FlcErrorRef                 error);

/**
 *  Set the capability request correlation id
 *
 *  @param  licensing       Licensing environment object
 *  @param  request         Capability request object on which to set the correlation id
 *  @param  correlationId   Correlation id or NULL to remove an existing value
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   An undo (FLC_CAPABILITY_REQUEST_OPERATION_UNDO) operation will not succeed without a
 *          correlation ID specification.
 *  @note   Specification of a correlation id is incompatible with an operation type of
 *          FLC_CAPABILITY_REQUEST_OPERATION_PREVIEW.
 *
 *  @note   Possible error codes include
 *          FLCERR_CAPABILITY_REQUEST_OPTION_CONFLICT
 */
FlcBool FlcCapabilityRequestSetCorrelationId(FlcLicensingRef         licensing,
                                             FlcCapabilityRequestRef request,
                                             const FlcChar *         correlationId,
                                             FlcErrorRef             error);

/**
 *  Generates a correlation id and sets the capability request correlation id to the generated value
 *
 *  @param  licensing       Licensing environment object
 *  @param  request         Capability request object on which to set the correlation id
 *  @param  correlationId   Pointer to the variable to receive the address of the generated correlation id
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   A correlationId value is required for an undo (FLC_CAPABILITY_REQUEST_OPERATION_UNDO) operation.
 *  @note   The returned correlation id pointer is valid until the capabilty request object is deleted.
 *  @note   Specification of a correlation id is incompatible with an operation type of
 *          FLC_CAPABILITY_REQUEST_OPERATION_PREVIEW.
 *
 *  @note   Possible error codes include
 *          FLCERR_CAPABILITY_REQUEST_OPTION_CONFLICT
 */
FlcBool FlcCapabilityRequestGenerateCorrelationId(FlcLicensingRef         licensing,
                                                  FlcCapabilityRequestRef request,
                                                  const FlcChar **        correlationId,
                                                  FlcErrorRef             error);


/**
 *  Set the capability request requestor id. Typically this value is used to identify the entity
 *  that requested the acquisition of a resource for reporting purposes.
 *
 *  @param  licensing       Licensing environment object
 *  @param  request         Capability request object on which to set the requestor id
 *  @param  requestorId     Requestor id or NULL to remove an existing value
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCapabilityRequestSetRequestorId(FlcLicensingRef         licensing,
                                           FlcCapabilityRequestRef request,
                                           const FlcChar *         requestorId,
                                           FlcErrorRef             error);

/**
 *  Set the capability request acquisition id. Typically this value is used to identify the
 *  resource that was acquired for reporting purposes.
 *
 *  @param  licensing       Licensing environment object
 *  @param  request         Capability request object on which to set the acquisition id
 *  @param  acquisitionId   Acquisition id or NULL to remove an existing value
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCapabilityRequestSetAcquisitionId(FlcLicensingRef         licensing,
                                             FlcCapabilityRequestRef request,
                                             const FlcChar *         acquisitionId,
                                             FlcErrorRef             error);

/**
 *  Set the capability request enterprise id. Typically this value is used to identify the
 *  account that the acquisition is on behalf of.
 *
 *  @param  licensing       Licensing environment object
 *  @param  request         Capability request object on which to set the acquisition id
 *  @param  enterpriseId    Enterprise id or NULL to remove an existing value
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCapabilityRequestSetEnterpriseId(FlcLicensingRef         licensing,
                                            FlcCapabilityRequestRef request,
                                            const FlcChar *         enterpriseId,
                                            FlcErrorRef             error);
/**
 *  Set the capability request operation
 *
 *  @param  licensing       Licensing environment object
 *  @param  request         Capability request object on which to set the operation
 *  @param  operation       Operation (See the FlcCapabilityRequestOperation enumeration for supported values)
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The default operation is FLC_CAPABILITY_REQUEST_OPERATION_REQUEST.
 *  @note   An undo (FLC_CAPABILITY_REQUEST_OPERATION_UNDO) operation will not succeed without a
 *          correlation ID specification.
 *  @note   An operation type of FLC_CAPABILITY_REQUEST_OPERATION_REPORT is incompatible with specification
 *          of rights IDs.
 *  @note   An operation type of any value other than FLC_CAPABILITY_REQUEST_OPERATION_REQUEST is
 *          incompatible with a "Served Buffer" flag specification of FLC_TRUE.
 *  @note   An operation type of any value other than FLC_CAPABILITY_REQUEST_OPERATION_REQUEST is
 *          incompatible with a capability request "Incremental" flag specification of FLC_TRUE.
 *  @note   An operation type of FLC_CAPABILITY_REQUEST_OPERATION_PREVIEW is incompatible with
 *          specification of any of the following:
 *          - One-time activation
 *          - Served buffer
 *          - Incremental
 *          - Correlation id
 *
 *  @note   Possible error codes include
 *          FLCERR_CAPABILITY_REQUEST_OPTION_CONFLICT
 */
FlcBool FlcCapabilityRequestSetOperation(FlcLicensingRef         licensing,
                                         FlcCapabilityRequestRef request,
                                         FlcInt32                operation,
                                         FlcErrorRef             error);

/**
 *  Set the capability request target license server instance
 *
 *  @param  licensing       Licensing environment object
 *  @param  request         Capability request object on which to set the license server instance value
 *  @param  serverInstance  License server instance value
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The default capability request license server instance is FLC_LICENSE_SERVER_DEFAULT.
 *  @note   You may not set the license server instance to FLC_LICENSE_SERVER_UNKNOWN.
 */
FlcBool FlcCapabilityRequestSetServerInstance(FlcLicensingRef            licensing,
                                              FlcCapabilityRequestRef    request,
                                              FlcLicenseServerInstance   serverInstance,
                                              FlcErrorRef                error);

/**
 *  Generates binary capability request
 *
 *  @param  licensing           Licensing environment object
 *  @param  request             Capability request object for which to generate a binary request
 *  @param  binaryRequest           Pointer to returned binary capability request buffer
 *  @param  binaryRequestSize   Pointer to returned binary capability request size
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   On success the caller will be responsible for deallocating the binary capability request storage
 *          using the FlcMemoryFree API.
 *
 *  @note   Possible error codes include
 *          FLCERR_DICTIONARY_SOURCE_CONFICT
 */
FlcBool FlcCapabilityRequestGenerate(FlcLicensingRef         licensing,
                                     FlcCapabilityRequestRef request,
                                     FlcUInt8 **             binaryRequest,
                                     FlcSize *               binaryRequestSize,
                                     FlcErrorRef             error);




/******************************************************************************
 * DEPRECATED
 *****************************************************************************/

/**
 *  Add existing license info to capability request.
 *
 *  @param  licensing   Licensing environment object
 *  @param  request     Capability request object to add existing features to
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   THIS FUNCTION HAS BEEN DEPRECATED. To renew existing features, use
 *          FlcCapabilityRequestSetIncremental.
 *
 *  @note   This API does not add existing features to the desired features; desired features
 *          have to be constructed independently.
 *
 *  @note   Not currently supported for served buffer capability requests.
 *  @note   Changing the license server instance after a call to FlcCapabilityRequestSetAddExistingFeatures
 *          is not permitted.
 *
 *  @note   Possible error codes include
 *          FLCERR_CAPABILITY_REQUEST_OPTION_CONFLICT
 */
FlcBool FlcCapabilityRequestSetAddExistingFeatures(FlcLicensingRef         licensing,
                                                   FlcCapabilityRequestRef request,
                                                   FlcErrorRef             error);

/**
 *  Add vm info dictionary to capability request.
 *
 *  @param  licensing   Licensing environment object
 *  @param  request     Capability request object to add vm info to
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note By default vm info dictionary is not added
 *
 */
FlcBool FlcCapabilityRequestAddVmInfo(FlcLicensingRef         licensing,
                                      FlcCapabilityRequestRef request,
                                      FlcErrorRef             error);

/** @} */ /* end of CapabilityRequest group */
/** @} */ /* end of addtogroup licensingClient */


#ifdef __cplusplus
}
#endif

#endif /* _INCLUDE_FLC_CAPABILITYREQUEST_H_ */
