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
 *  Provides an interface to license management functionality.
 */

#ifndef _INCLUDE_FLC_LICENSEMANAGER_H_
#define _INCLUDE_FLC_LICENSEMANAGER_H_


#include "FlcTypes.h"
#include "FlcError.h"
#include "FlcLicensing.h"
#include "FlcLicense.h"
#include "FlcCapabilityResponse.h"
#include "FlcShortCode.h"
#include "FlcHostIdType.h"
#include "FlcLicenseSourceType.h"
#include "FlcMachineType.h"
#include "FlcDictionary.h"
#include "FlcFeature.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup licensingClient
 * @{
 */

/**
 * @defgroup licenseManager License Manager
 * @{
 */

/** Opaque data type for the license collection object.
*/
typedef struct flcLicenseCollection * FlcLicenseCollectionRef;

/**
 *  Gets the default host id to be used with all capability request, information message,
 *  and short code requests.
 *
 *  @param  licensing     Licensing environment object
 *  @param  hostIdType    Pointer to variable to receive the default host id type as
 *                        defined in FlcHostIdType. If no default host id is set then
 *                        FLC_HOSTID_TYPE_UNKNOWN is returned.
 *  @param  hostIdValue   Pointer to variable to recieive the default host id value or
 *                        NULL if no default host id is set
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcGetDefaultHostId(FlcLicensingRef   licensing,
                            FlcHostIdType *   hostIdType,
                            const FlcChar **  hostIdValue,
                            FlcErrorRef       error);

/**
 *  Sets the default host id to be used with all capability request, information message,
 *  and short code requests.
 *
 *  @param  licensing     Licensing environment object
 *  @param  hostIdType    Pointer to variable containing the default host id type.
 *  @param  hostIdValue   Pointer to variable containing the default host id value.
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note If set then the license manager default host id will be used as the client host id when generating requests.
 *        If not set then the request client host id will be chosen in the following order.
 *        - For capability requests only, if a capability response is currently stored in the target trusted
 *          storage license server instance location then the client host ID from that capability response will
 *          be used when generating the request.
 *        - The custom host id, if specified.
 *        - The first Ethernet address present on the client.
 */
FlcBool FlcSetDefaultHostId(FlcLicensingRef  licensing,
                            FlcHostIdType    hostIdType,
                            const FlcChar *  hostIdValue,
                            FlcErrorRef      error);

/**
 *  Gets an value indicating whether the current environment is virtual, physical, or of an
 *  unknown type
 *
 *  @param  licensing   Licensing environment object
 *  @param  type        Pointer to variable to receive machine type, see FlcMachineType for valid values
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   FLC_MACHINE_TYPE_UNKNOWN is the default if a value is not present
 */
FlcBool FlcGetVirtualMachineType(FlcLicensingRef   licensing,
                                 FlcMachineType *  type,
                                 FlcErrorRef       error);

/**
 *  Gets a reference to a dictionary containing virtual information
 *
 *  @param  licensing   Licensing environment object
 *  @param  dictionary  Pointer to variable to receive virtual machine information dictionary if present.
 *                      Returns NULL if not a virtual machine environment or no information is available
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   If machine type is not FLC_MACHINE_TYPE_VIRTUAL, NULL will be returned.
 *  @note   Lifetime of the returned dictionary depends on the lifetime of the licensing environment object.
 */
FlcBool FlcGetVirtualMachineInfo(FlcLicensingRef     licensing,
                                 FlcDictionaryRef *  dictionary,
                                 FlcErrorRef         error);

/**
 *  Adds the default trusted storage license source to the licensing environment license source collection
 *
 *  @param  licensing   Licensing environment object
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcAddTrustedStorageLicenseSource(FlcLicensingRef  licensing,
                                          FlcErrorRef      error);

/**
 *  Adds the specified trusted storage license source to the licensing environment license source collection
 *
 *  @param  licensing      Licensing environment object
 *  @param  serverInstance Trusted storage license server instance for the license source to be added
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcAddTrustedStorageServerInstanceLicenseSource(FlcLicensingRef            licensing,
                                                        FlcLicenseServerInstance   serverInstance,
                                                        FlcErrorRef                error);

/**
 *  Process capability response data received from the license server. If the response is a served
 *  buffer response then the data will be processed into either a new served buffer license source
 *  or an existing served buffer license source with a matching server host id. If the response is
 *  not a served buffer response then the the data will be populated into the trusted storage
 *  server instance location indicated in the capability response or the default location if no
 *  server instance location is present in the response. A trusted storage license source for the
 *  appropriate server instance will be added to the license source collection if not already present.
 *
 *  @param  licensing   Licensing environment object
 *  @param  response    Pointer to the capability response object
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Possible error codes include
 *        FLCERR_RESPONSE_HOST_ID_MISMATCH,
 *        FLCERR_RESPONSE_SERVER_ERROR,
 *        FLCERR_RESPONSE_STALE,
 *        FLCERR_RESPONSE_EXPIRED,
 *        FLCERR_DATA_CORRUPTED,
 *        FLCERR_INPUT_TYPE_MISMATCH,
 *        FLCERR_MACHINE_TYPE_MISMATCH,
 *        FLCERR_UUID_MISMATCH,
 *        FLCERR_LICENSE_MODEL_CONFLICT
 *
 *  @note Upon success, this function might invalidate some acquired licenses. Invalid licenses will be
 *        marked as stale and must be re-acquired. In general, it is recommended that you return all licenses
 *        prior to any updates to the license sources used for acquisition, and then re-acquire all the licenses
 *        after the update.
 *
 *  @note License return and re-acquisition is not necessary when processing a served buffer response into a
 *        new served buffer license source.
 *  @see FlcServedBufferLicenseSourceExists
 */
FlcBool FlcProcessCapabilityResponse(FlcLicensingRef            licensing,
                                     FlcCapabilityResponseRef   response,
                                     FlcErrorRef                error);

/**
 *  Process capability response data received from the license server. If the response is a served
 *  buffer response then the data will be processed into either a new served buffer license source
 *  or an existing served buffer license source with a matching server host id. If the response is
 *  not a served buffer response then the the data will be populated into the specified trusted
 *  storage server instance. A trusted storage license source for the specified server instance will
 *  be added to the license source collection if not already present.
 *
 *  @param  licensing       Licensing environment object
 *  @param  response        Pointer to the capability response object
 *  @param  serverInstance  Target trusted storage server instance
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Possible error codes include
 *        FLCERR_RESPONSE_HOST_ID_MISMATCH,
 *        FLCERR_RESPONSE_SERVER_ERROR,
 *        FLCERR_RESPONSE_STALE,
 *        FLCERR_RESPONSE_EXPIRED,
 *        FLCERR_DATA_CORRUPTED,
 *        FLCERR_INPUT_TYPE_MISMATCH,
 *        FLCERR_MACHINE_TYPE_MISMATCH,
 *        FLCERR_UUID_MISMATCH,
 *        FLCERR_LICENSE_MODEL_CONFLICT
 *        FLXERR_CAPABILITY_SERVER_INSTANCE_MISMATCH
 *
 *  @note A server instance parameter value of FLC_LICENSE_SERVER_UNKNOWN is invalid.
 *
 *  @note If the response is not a served buffer response and the response contains a server instance
 *        location different than the provided server instance parameter value an error of type
 *        FLCERR_CAPABILITY_SERVER_INSTANCE_MISMATCH will be returned.
 *
 *  @note Upon success, this function might invalidate some acquired licenses. Invalid licenses will be
 *        marked as stale and must be re-acquired. In general, it is recommended that you return all licenses
 *        prior to any updates to the license sources used for acquisition, and then re-acquire all the licenses
 *        after the update.
 *
 *  @note License return and re-acquisition is not necessary when processing a served buffer response into a
 *        new served buffer license source.
 *  @see FlcServedBufferLicenseSourceExists
 */
FlcBool FlcProcessCapabilityResponseToServerInstance(FlcLicensingRef            licensing,
                                                     FlcCapabilityResponseRef   response,
                                                     FlcLicenseServerInstance   serverInstance,
                                                     FlcErrorRef                error);

/**
 *  Process capability response data received from the license server. If the response is a served
 *  buffer response then the data will be processed into either a new served buffer license source
 *  or an existing served buffer license source with a matching server host id. If the response is
 *  not a served buffer response then the the data will be populated into the trusted storage
 *  server instance location indicated in the capability response or the default location if no
 *  server instance location is present in the response. A trusted storage license source for the
 *  appropriate server instance will be added to the license source collection if not already present.
 *
 *  @param  licensing   Licensing environment object
 *  @param  response    Pointer to variable to receive the capability response object reference
 *                      created from the supplied data. If NULL no capability response object is
 *                      returned.
 *  @param  data        Pointer to the binary capability response data
 *  @param  size        Binary capability response data size
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Possible error codes include
 *        FLCERR_RESPONSE_HOST_ID_MISMATCH,
 *        FLCERR_RESPONSE_SERVER_ERROR,
 *        FLCERR_RESPONSE_STALE,
 *        FLCERR_RESPONSE_EXPIRED,
 *        FLCERR_DATA_CORRUPTED,
 *        FLCERR_INPUT_TYPE_MISMATCH,
 *        FLCERR_MACHINE_TYPE_MISMATCH,
 *        FLCERR_UUID_MISMATCH,
 *        FLCERR_LICENSE_MODEL_CONFLICT
 *
 *  @note Upon success, this function might invalidate some acquired licenses. Invalid licenses will be
 *        marked as stale and must be re-acquired. In general, it is recommended that you return all licenses
 *        prior to any updates to the license sources used for acquisition, and then re-acquire all the licenses
 *        after the update.
 *
 *  @note License return and re-acquisition is not necessary when processing a served buffer response into a
 *        new served buffer license source.
 *  @see FlcServedBufferLicenseSourceExists
 */
FlcBool FlcProcessCapabilityResponseData(FlcLicensingRef             licensing,
                                         FlcCapabilityResponseRef *  response,
                                         const FlcUInt8 *            data,
                                         FlcSize                     size,
                                         FlcErrorRef                 error);

/**
 *  Process capability response data received from the license server. If the response is a served
 *  buffer response then the data will be processed into either a new served buffer license source
 *  or an existing served buffer license source with a matching server host id. If the response is
 *  not a served buffer response then the the data will be populated into the specified trusted
 *  storage server instance. A trusted storage license source for the specified server instance will
 *  be added to the license source collection if not already present.
 *
 *  @param  licensing       Licensing environment object
 *  @param  response        Pointer to variable to receive the capability response object reference
 *                          created from the supplied data. If NULL no capability response object is
 *                          returned.
 *  @param  data            Pointer to the binary capability response data
 *  @param  size            Binary capability response data size
 *  @param  serverInstance  Target trusted storage server instance
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Possible error codes include
 *        FLCERR_RESPONSE_HOST_ID_MISMATCH,
 *        FLCERR_RESPONSE_SERVER_ERROR,
 *        FLCERR_RESPONSE_STALE,
 *        FLCERR_RESPONSE_EXPIRED,
 *        FLCERR_DATA_CORRUPTED,
 *        FLCERR_INPUT_TYPE_MISMATCH,
 *        FLCERR_MACHINE_TYPE_MISMATCH,
 *        FLCERR_UUID_MISMATCH,
 *        FLCERR_LICENSE_MODEL_CONFLICT
 *        FLXERR_CAPABILITY_SERVER_INSTANCE_MISMATCH
 *
 *  @note A server instance parameter value of FLC_LICENSE_SERVER_UNKNOWN is invalid.
 *
 *  @note If the response is not a served buffer response and the response contains a server instance
 *        location different than the provided server instance parameter value an error of type
 *        FLCERR_CAPABILITY_SERVER_INSTANCE_MISMATCH will be returned.
 *
 *  @note Upon success, this function might invalidate some acquired licenses. Invalid licenses will be
 *        marked as stale and must be re-acquired. In general, it is recommended that you return all licenses
 *        prior to any updates to the license sources used for acquisition, and then re-acquire all the licenses
 *        after the update.
 *
 *  @note License return and re-acquisition is not necessary when processing a served buffer response into a
 *        new served buffer license source.
 *  @see FlcServedBufferLicenseSourceExists
 */
FlcBool FlcProcessCapabilityResponseDataToServerInstance(FlcLicensingRef             licensing,
                                                         FlcCapabilityResponseRef *  response,
                                                         const FlcUInt8 *            data,
                                                         FlcSize                     size,
                                                         FlcLicenseServerInstance    serverInstance,
                                                         FlcErrorRef                 error);

/**
 *  Process capability response data contained in the supplied binary capability response data
 *  file. If the response is a served buffer response then the data will be processed into either a
 *  new served buffer license source or an existing served buffer license source with a matching server
 *  host id. If the response is not a served buffer response then the the data will be populated into
 *  the trusted storage server instance location indicated in the capability response or the default
 *  location if no server instance location is present in the response. A trusted storage license source
 *  for the appropriate server instance will be added to the license source collection if not already present.
 *
 *  @param  licensing       Licensing environment object
 *  @param  response        Pointer to variable to receive the capability response object reference
 *                          created from the supplied data. If NULL no capability response object is
 *                          returned.
 *  @param  fileName        File name of the file containing the binary capability response data
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Possible error codes include
 *        FLCSWC_FILE_OPEN,
 *        FLCSWC_EMPTY_FILE,
 *        FLCSWC_FILE_READ
 *        FLCERR_RESPONSE_HOST_ID_MISMATCH,
 *        FLCERR_RESPONSE_SERVER_ERROR,
 *        FLCERR_RESPONSE_STALE,
 *        FLCERR_RESPONSE_EXPIRED,
 *        FLCERR_DATA_CORRUPTED,
 *        FLCERR_INPUT_TYPE_MISMATCH,
 *        FLCERR_MACHINE_TYPE_MISMATCH,
 *        FLCERR_UUID_MISMATCH,
 *        FLCERR_LICENSE_MODEL_CONFLICT
 *
 *  @note A server instance parameter value of FLC_LICENSE_SERVER_UNKNOWN is invalid.
 *
 *  @note Upon success, this function might invalidate some acquired licenses. Invalid licenses will be
 *        marked as stale and must be re-acquired. In general, it is recommended that you return all licenses
 *        prior to any updates to the license sources used for acquisition, and then re-acquire all the licenses
 *        after the update.
 *
 *  @note License return and re-acquisition is not necessary when processing a served buffer response into a
 *        new served buffer license source.
 *  @see FlcServedBufferLicenseSourceExists
 */
FlcBool FlcProcessCapabilityResponseFile(FlcLicensingRef             licensing,
                                         FlcCapabilityResponseRef *  response,
                                         const FlcChar *             fileName,
                                         FlcErrorRef                 error);

/**
 *  Process capability response data contained in the supplied binary capability response data
 *  file. If the file contains a served buffer response then the data will be processed into either
 *  a new served buffer license source or an existing served buffer license source with a matching
 *  server host id. If the response is not a served buffer response then the the data will be
 *  populated into the specified trusted storage server instance. A trusted storage license source for
 *  the specified server instance will be added to the license source collection if not already present.
 *
 *  @param  licensing       Licensing environment object
 *  @param  response        Pointer to variable to receive the capability response object reference
 *                          created from the supplied data. If NULL no capability response object is
 *                          returned.
 *  @param  fileName        File name of the file containing the binary capability response data
 *  @param  serverInstance  Target trusted storage server instance
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Possible error codes include
 *        FLCSWC_FILE_OPEN,
 *        FLCSWC_EMPTY_FILE,
 *        FLCSWC_FILE_READ
 *        FLCERR_RESPONSE_HOST_ID_MISMATCH,
 *        FLCERR_RESPONSE_SERVER_ERROR,
 *        FLCERR_RESPONSE_STALE,
 *        FLCERR_RESPONSE_EXPIRED,
 *        FLCERR_DATA_CORRUPTED,
 *        FLCERR_INPUT_TYPE_MISMATCH,
 *        FLCERR_MACHINE_TYPE_MISMATCH,
 *        FLCERR_UUID_MISMATCH,
 *        FLCERR_LICENSE_MODEL_CONFLICT
 *        FLXERR_CAPABILITY_SERVER_INSTANCE_MISMATCH
 *
 *  @note A server instance parameter value of FLC_LICENSE_SERVER_UNKNOWN is invalid.
 *
 *  @note If the response is not a served buffer response and the response contains a server instance
 *        location different than the provided server instance parameter value an error of type
 *        FLCERR_CAPABILITY_SERVER_INSTANCE_MISMATCH will be returned.
 *
 *  @note Upon success, this function might invalidate some acquired licenses. Invalid licenses will be
 *        marked as stale and must be re-acquired. In general, it is recommended that you return all licenses
 *        prior to any updates to the license sources used for acquisition, and then re-acquire all the licenses
 *        after the update.
 *
 *  @note License return and re-acquisition is not necessary when processing a served buffer response into a
 *        new served buffer license source.
 *  @see FlcServedBufferLicenseSourceExists
 */
FlcBool FlcProcessCapabilityResponseFileToServerInstance(FlcLicensingRef             licensing,
                                                         FlcCapabilityResponseRef *  response,
                                                         const FlcChar *             fileName,
                                                         FlcLicenseServerInstance    serverInstance,
                                                         FlcErrorRef                 error);

/**
 *  Adds a short code license source to the licensing environment license source collection
 *
 *  @param  licensing   Licensing environment object
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcAddShortCodeLicenseSource(FlcLicensingRef  licensing,
                                     FlcErrorRef      error);

/**
 *  Processes the short code environment response. If a short code license source does not
 *  currently exist in the licensing environment it will be added.
 *
 *  @param  shortCodeEnv   Short code environment object
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note In order to succeed a short code template with an id matching that of the short code
 *        response must exist in the short code environment template collection.
 *
 *  @note Possible error codes include
 *        FLCSWC_SHORTCODE_RESPONSE_NOT_FOUND
 */
FlcBool FlcProcessShortCodeResponse(FlcShortCodeEnvironmentRef  shortCodeEnv,
                                    FlcErrorRef                 error);

/**
 *  Processes the short code environment response using the supplied template data. If a short code
 *  license source does not currently exist in the licensing environment it will be added.
 *
 *  @param  shortCodeEnv   Short code environment object
 *  @param  templateData   Binary template data
 *  @param  templateSize   Binary template data size
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note On success the template generated by the supplied data will be added to the
 *        short code environment template collection.
 *
 *  @note Possible error codes include
 *        FLCSWC_SHORTCODE_RESPONSE_NOT_FOUND
 */
FlcBool FlcProcessShortCodeResponseWithData(FlcShortCodeEnvironmentRef shortCodeEnv,
                                            const FlcUInt8 *           templateData,
                                            FlcSize                    templateSize,
                                            FlcErrorRef                error);

/**
 *  Processes the short code environment response using the supplied template data file. If a short
 *  license source does not currently exist in the licensing environment it will be added.
 *
 *  @param  shortCodeEnv   Short code environment object
 *  @param  templateFile   File containing the binary template data
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note On success the template generated by the data in the supplied file will be added to the
 *        short code environment template collection.
 *
 *  @note Possible error codes include
 *        FLCSWC_FILE_OPEN,
 *        FLCSWC_EMPTY_FILE,
 *        FLCSWC_FILE_READ,
 *        FLCSWC_SHORTCODE_RESPONSE_NOT_FOUND
 */
FlcBool FlcProcessShortCodeResponseWithFile(FlcShortCodeEnvironmentRef shortCodeEnv,
                                            const FlcChar *            templateFile,
                                            FlcErrorRef                error);

/**
 *  Adds a trial license source to the licensing environment license source collection
 *
 *  @param  licensing   Licensing environment object
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcAddTrialLicenseSource(FlcLicensingRef  licensing,
                                 FlcErrorRef      error);

/**
 *  Populates trial license storage with the license information contained in the supplied binary
 *  trial license data. If a trial license source does not currently exist in the licensing
 *  environment it will be added
 *
 *  @param  licensing   Licensing environment object
 *  @param  data        Pointer to the binary trial license data
 *  @param  size        Binary trial license data size
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcProcessTrialLicenseData(FlcLicensingRef  licensing,
                                   const FlcUInt8 * data,
                                   FlcSize          size,
                                   FlcErrorRef      error);

/**
 *  Populates trial license storage with the license information contained in the supplied binary
 *  trial license data file. If a trial license source does not currently exist in the licensing
 *  environment it will be added
 *
 *  @param  licensing   Licensing environment object
 *  @param  fileName    File name of the file containing the binary trial license data
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Possible error codes include
 *        FLCSWC_FILE_OPEN,
 *        FLCSWC_EMPTY_FILE,
 *        FLCSWC_FILE_READ
 */
FlcBool FlcProcessTrialLicenseFile(FlcLicensingRef  licensing,
                                   const FlcChar *  fileName,
                                   FlcErrorRef      error);

/**
 *  Indicates whether or not trial license storage had already been populated with the license
 *  information contained in the supplied binary trial license data
 *
 *  @param  licensing             Licensing environment object
 *  @param  data                  Pointer to the binary trial license data
 *  @param  size                  Binary trial license data size
 *  @param  isLoaded              Pointer to variable to receive the trial license data loaded indicator,
 *                                FLC_TRUE if the trial has been loaded
 *  @param  secondsToExpiration   Pointer to variable to receive the number of seconds until trial
 *                                expiration, 0 if expired. Only valid if trial has been loaded.
 *  @param  error                 Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Possible error codes include
 *      FLCERR_DATA_CORRUPTED,
 *      FLCERR_DATA_INSUFFICIENT,
 *      FLCERR_TRIAL_INVALID_ID,
 *      FLCERR_TRIAL_EXPIRED,
 *      FLCERR_SIGNATURE_INVALID,
 *      FLCERR_WINDBACK_DETECTED,
 *      FLCERR_INPUT_TYPE_MISMATCH
 */
FlcBool FlcTrialDataIsLoaded(FlcLicensingRef    licensing,
                             const FlcUInt8 *   data,
                             FlcSize            size,
                             FlcBool *          isLoaded,
                             FlcUInt32 *        secondsToExpiration,
                             FlcErrorRef        error);

/**
 *  Indicates whether or not trial license storage had already been populated with the license
 *  information contained in the supplied binary trial license data file
 *
 *  @param  licensing             Licensing environment object
 *  @param  fileName              File name of the file containing the binary trial license data
 *  @param  isLoaded              Pointer to variable to receive the trial license data loaded indicator,
 *                                FLC_TRUE if the trial has been loaded
 *  @param  secondsToExpiration   Pointer to variable to receive the number of seconds until trial
 *                                expiration, 0 if expired. Only valid if trial has been loaded.
 *  @param  error                 Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Possible error codes include
 *      FLCERR_DATA_CORRUPTED,
 *      FLCERR_DATA_INSUFFICIENT,
 *      FLCERR_TRIAL_INVALID_ID,
 *      FLCERR_TRIAL_EXPIRED,
 *      FLCERR_SIGNATURE_INVALID,
 *      FLCERR_WINDBACK_DETECTED,
 *      FLCERR_INPUT_TYPE_MISMATCH
 */
FlcBool FlcTrialFileIsLoaded(FlcLicensingRef   licensing,
                             const FlcChar *   fileName,
                             FlcBool *         isLoaded,
                             FlcUInt32 *       secondsToExpiration,
                             FlcErrorRef       error);

/**
 *  Adds served buffer license data to the licensing environment license source collection. If no
 *  current served buffer license source exists for the license server then one will be added. If a
 *  current served buffer license source does exist for the license server then the existing license
 *  source features will be replaced with served buffer license features in the provided data.
 *
 *  @param  licensing   Licensing environment object
 *  @param  response    Pointer to variable to receive the capability response object reference
 *                      created from the supplied data. If NULL no capability response object is
 *                      returned.
 *  @param  data        Pointer to the binary capability response containing the served buffer data
 *  @param  size        Binary capability response size
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note In the case where the contents of a current served buffer license source are replaced,
 *        successful execution of this function might invalidate some acquired licenses. Invalid
 *        licenses will be marked as stale and must be re-acquired. In general, it is recommended
 *        that you return all licenses prior to any updates to the license sources used for acquisition,
 *        and then re-acquire all the licenses after the update.
 *  @see FlcServedBufferLicenseSourceExists
 *
 *  @note Possible error codes include
 *        FLCERR_DATA_INSUFFICIENT,
 *        FLCERR_DATA_CORRUPTED,
 *        FLCERR_SIGNATURE_INVALID
 *        FLCERR_CAPABILITY_RESPONSE_TYPE_INVALID
 */
FlcBool FlcAddServedBufferLicenseSourceFromData(FlcLicensingRef             licensing,
                                                FlcCapabilityResponseRef *  response,
                                                const FlcUInt8 *            data,
                                                FlcSize                     size,
                                                FlcErrorRef                 error);

/**
 *  Adds served buffer license data to the licensing environment license source collection. If no
 *  current served buffer license source exists for the license server then one will be added. If a
 *  current served buffer license source does exist for the license server then the existing license
 *  source features will be replaced with served buffer license features in the provided served
 *  buffer capability response file.
 *
 *  @param  licensing   Licensing environment object
 *  @param  response    Pointer to variable to receive the capability response object reference
 *                      created from the supplied file. If NULL no capability response object is
 *                      returned.
 *  @param  fileName    File name of the served buffer capability response file
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note In the case where the contents of a current served buffer license source are replaced,
 *        successful execution of this function might invalidate some acquired licenses. Invalid
 *        licenses will be marked as stale and must be re-acquired. In general, it is recommended
 *        that you return all licenses prior to any updates to the license sources used for acquisition,
 *        and then re-acquire all the licenses after the update.
 *  @see FlcServedBufferLicenseSourceExists
 *
 *  @note Possible error codes include
 *        FLCSWC_FILE_OPEN,
 *        FLCSWC_EMPTY_FILE,
 *        FLCSWC_FILE_READ,
 *        FLCERR_DATA_INSUFFICIENT,
 *        FLCERR_DATA_CORRUPTED,
 *        FLCERR_SIGNATURE_INVALID
 *        FLCERR_CAPABILITY_RESPONSE_TYPE_INVALID
 */
FlcBool FlcAddServedBufferLicenseSourceFromFile(FlcLicensingRef             licensing,
                                                FlcCapabilityResponseRef *  response,
                                                const FlcChar *             fileName,
                                                FlcErrorRef                 error);

/**
 *  Returns a feature collection of all features contained in all current license
 *  sources that have been added to the licensing environment license source collection
 *
 *  @param  licensing           Licensing environment object
 *  @param  featureCollection   Pointer to variable to receive the feature collection
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcGetFeatureCollection(FlcLicensingRef            licensing,
                                FlcFeatureCollectionRef *  featureCollection,
                                FlcErrorRef                error);

/**
 *  Returns a feature collection of all trusted storage features for the default license server
 *  instance.
 *
 *  @param  licensing           Licensing environment object
 *  @param  featureCollection   Pointer to variable to receive the feature collection
 *  @param  diagnostic          Diagnostic indicator. If set to FLC_TRUE all features are loaded
 *                              even if they are considered invalid for one reason or another
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcGetTrustedStorageFeatureCollection(FlcLicensingRef            licensing,
                                              FlcFeatureCollectionRef *  featureCollection,
                                              FlcBool                    diagnostic,
                                              FlcErrorRef                error);

/**
 *  Returns a feature collection of all trusted storage features for the specified license server
 *  instance.
 *
 *  @param  licensing           Licensing environment object
 *  @param  featureCollection   Pointer to variable to receive the feature collection
 *  @param  serverInstance      Trusted storage license server instance
 *  @param  diagnostic          Diagnostic indicator. If set to FLC_TRUE all features are loaded
 *                              even if they are considered invalid for one reason or another
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcGetTrustedStorageServerInstanceFeatureCollection(FlcLicensingRef            licensing,
                                                            FlcFeatureCollectionRef *  featureCollection,
                                                            FlcLicenseServerInstance   serverInstance,
                                                            FlcBool                    diagnostic,
                                                            FlcErrorRef                error);

/**
 *  Gets a reference to the vendor dictionary information in trusted storage for the default
 *  license server instance.
 *
 *  @param  licensing          Licensing environment object
 *  @param  vendorDictionary   Pointer to variable to receive the trusted storage vendor dictionary object
 *  @param  error              Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The returned vendor dictionary address is valid until the next FlcGetTrustedStorageVendorDictionary or
 *            FlcGetTrustedStorageServerInstanceVendorDictionary request for the default server instance or until the
 *            licensing environment is deleted.
 */
FlcBool FlcGetTrustedStorageVendorDictionary(FlcLicensingRef       licensing,
                                             FlcDictionaryRef *    vendorDictionary,
                                             FlcErrorRef           error);

/**
 *  Gets a reference to the vendor dictionary information in trusted storage for the specified
 *  license server instance.
 *
 *  @param  licensing          Licensing environment object
 *  @param  vendorDictionary   Pointer to variable to receive the trusted storage vendor dictionary object
 *  @param  serverInstance     Target trusted storage license server instance
 *  @param  error              Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The returned vendor dictionary address is valid until the next FlcGetTrustedStorageServerInstanceVendorDictionary
 *            request for the same target instance or until the licensing environment is deleted.
 */
FlcBool FlcGetTrustedStorageServerInstanceVendorDictionary(FlcLicensingRef            licensing,
                                                           FlcDictionaryRef *         vendorDictionary,
                                                           FlcLicenseServerInstance   serverInstance,
                                                           FlcErrorRef                error);

/**
 *  Gets the correlation ID stored in trusted storage for the default license server instance.
 *
 *  @param  licensing          Licensing environment object
 *  @param  correlationId      Pointer to variable to receive the trusted storage correlation ID if present else NULL
 *  @param  error              Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The returned correlation id string address is valid until the next FlcGetTrustedStorageCorrelationId or
 *            FlcGetTrustedStorageServerInstanceCorrelationId request for the default server instance or until the
 *            licensing environment is deleted.
 */
FlcBool FlcGetTrustedStorageCorrelationId(FlcLicensingRef       licensing,
                                          const FlcChar **      correlationId,
                                          FlcErrorRef           error);


/**
 *  Gets the correlation ID stored in trusted storage for the specified license server instance.
 *
 *  @param  licensing          Licensing environment object
 *  @param  correlationId      Pointer to variable to receive the trusted storage correlation ID if present else NULL
 *  @param  serverInstance     Target trusted storage license server instance
 *  @param  error              Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The returned correlation id string address is valid until the next FlcGetTrustedStorageServerInstanceCorrelationId
 *            request for the same target instance or until the licensing environment is deleted.
 */
FlcBool FlcGetTrustedStorageServerInstanceCorrelationId(FlcLicensingRef            licensing,
                                                        const FlcChar **           correlationId,
                                                        FlcLicenseServerInstance   serverInstance,
                                                        FlcErrorRef                error);

/**
 *  Get server hostid stored in trusted storage for the default license server instance.
 *
 *  @param  licensing          Licensing environment object
 *  @param  serverIdType       Pointer to variable to receive the server hostid type associated with the
 *                             last capability response processed into trusted storage or
 *                             FLC_HOSTID_TYPE_UNKNOWN if not available
 *  @param  serverIdValue      Pointer to variable to recieive the server hostid value associated with the
 *                             last capability response processed into trusted storage or
 *                             FLC_HOSTID_TYPE_UNKNOWN if not available
 *  @param  error              Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The returned server id value string address is valid until the next FlcGetTrustedStorageServerId
 *            or FlcGetTrustedStorageServerInstanceServerId request for the default server instance or until
 *            the licensing environment is deleted.
 */
FlcBool FlcGetTrustedStorageServerId(FlcLicensingRef       licensing,
                                     FlcUInt32 *           serverIdType,
                                     const FlcChar **      serverIdValue,
                                     FlcErrorRef           error);

/**
 *  Get server hostid stored in trusted storage for the specified license server instance.
 *
 *  @param  licensing          Licensing environment object
 *  @param  serverIdType       Pointer to variable to receive the server hostid type associated with the
 *                             last capability response processed into trusted storage or
 *                             FLC_HOSTID_TYPE_UNKNOWN if not available
 *  @param  serverIdValue      Pointer to variable to recieive the server hostid value associated with the
 *                             last capability response processed into trusted storage or
 *                             FLC_HOSTID_TYPE_UNKNOWN if not available
 *  @param  serverInstance     Target trusted storage license server instance
 *  @param  error              Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The returned server id value string address is valid until the next FlcGetTrustedStorageServerInstanceServerId
 *            request for the same target server instance or until the licensing environment is deleted.
 */
FlcBool FlcGetTrustedStorageServerInstanceServerId(FlcLicensingRef            licensing,
                                                   FlcUInt32 *                serverIdType,
                                                   const FlcChar **           serverIdValue,
                                                   FlcLicenseServerInstance   serverInstance,
                                                   FlcErrorRef                error);

/**
 *  Get served time from trusted storage for the default license server instance
 *
 *  @param  licensing          Licensing environment object
 *  @param  servedTime         Pointer to variable to receive trusted storage served time or
 *                             NULL if not available
 *  @param  error              Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The returned served time address is valid until the next FlcGetTrustedStorageServedTime
 *            request or FlcGetTrustedStorageServerInstanceServedTime request for the default server
 *            instance or until the licensing environment is deleted.
 */
FlcBool FlcGetTrustedStorageServedTime(FlcLicensingRef     licensing,
                                       const time_t **     servedTime,
                                       FlcErrorRef         error);


/**
 *  Get served time from trusted storage for the specified license server instance
 *
 *  @param  licensing          Licensing environment object
 *  @param  servedTime         Pointer to variable to receive trusted storage served time or
 *                             NULL if not available
 *  @param  serverInstance     Target trusted storage license server instance
 *  @param  error              Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The returned served time address is valid until the next FlcGetTrustedStorageServerInstanceServedTime
 *          request for the same target server instance or until the licensing environment is deleted.
 */
FlcBool FlcGetTrustedStorageServerInstanceServedTime(FlcLicensingRef            licensing,
                                                     const time_t **            servedTime,
                                                     FlcLicenseServerInstance   serverInstance,
                                                     FlcErrorRef                error);


/**
 *  Get uuid value from trusted storage for the specified license server instance.
 *
 *  @param  licensing          Licensing environment object
 *  @param  uuid               Variable to receive a pointer to the trusted storage uuid value
 *                             or NULL if not available
 *  @param  uuidSize           Returned uuid size
 *  @param  serverInstance     Target trusted storage license server instance
 *  @param  error              Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The returned uuid address is valid until the next FlcGetTrustedStorageServerInstanceUuid
 *          request for the same target server instance or until the licensing environment is deleted.
 */
FlcBool FlcGetTrustedStorageServerInstanceUuid(FlcLicensingRef            licensing,
                                               const FlcUInt8 **          uuid,
                                               FlcSize *                  uuidSize,
                                               FlcLicenseServerInstance   serverInstance,
                                               FlcErrorRef                error);

/**
 *  Returns a feature collection of all features contained in trial license storage.
 *
 *  @param  licensing           Licensing environment object
 *  @param  featureCollection   Pointer to variable to receive the feature collection
 *  @param  diagnostic          Diagnostic indicator. If set to FLC_TRUE all features are loaded
 *                              even if they are considered invalid for one reason or another
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcGetTrialFeatureCollection(FlcLicensingRef            licensing,
                                     FlcFeatureCollectionRef *  featureCollection,
                                     FlcBool                    diagnostic,
                                     FlcErrorRef                error);

/**
 *  Returns a feature collection of all features contained in short code license storage.
 *
 *  @param  licensing           Licensing environment object
 *  @param  featureCollection   Pointer to variable to receive the feature collection
 *  @param  diagnostic          Diagnostic indicator. If set to FLC_TRUE all features are loaded
 *                              even if they are considered invalid for one reason or another
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcGetShortCodeFeatureCollection(FlcLicensingRef            licensing,
                                         FlcFeatureCollectionRef *  featureCollection,
                                         FlcBool                    diagnostic,
                                         FlcErrorRef                error);

/**
 *  Returns a feature collection of all features contained in the supplied served buffer capability
 *  response data.
 *
 *  @param  licensing           Licensing environment object
 *  @param  featureCollection   Pointer to variable to receive the feature collection
 *  @param  diagnostic          Diagnostic indicator. If set to FLC_TRUE all features are loaded
 *                              even if they are considered invalid for one reason or another
 *  @param  data                Pointer to the binary served buffer capability response data
 *  @param  size                Binary served buffer capability response data size
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcGetServedBufferFeatureCollection(FlcLicensingRef            licensing,
                                            FlcFeatureCollectionRef *  featureCollection,
                                            FlcBool                    diagnostic,
                                            const FlcUInt8 *           data,
                                            FlcSize                    size,
                                            FlcErrorRef                error);

/**
 *  Returns a feature collection of all features contained in the specified served buffer license source.
 *
 *  @param  licensing           Licensing environment object
 *  @param  featureCollection   Pointer to variable to receive the feature collection
 *  @param  serverIdType        Pointer to variable containing the server hostid type associated with the
 *                              served buffer license source from which the feature collection is to be returned
 *  @param  serverIdValue       Pointer to variable containing the server hostid value associated with the
 *                              served buffer license source from which the feature collection is to be returned
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Possible error codes include
 *        FLCSWC_SERVED_BUFFER_LICENSE_SOURCE_NOT_FOUND
 */
FlcBool FlcGetServedBufferLicenseSourceFeatureCollection(FlcLicensingRef            licensing,
                                                         FlcFeatureCollectionRef *  featureCollection,
                                                         FlcUInt32                  serverIdType,
                                                         const FlcChar *            serverIdValue,
                                                         FlcErrorRef                error);

/**
 *  Adds a buffer license source to the licensing environment license source collection from the
 *  supplied data.
 *
 *  @param  licensing   Licensing environment object
 *  @param  data        Pointer to the binary buffer license data
 *  @param  size        Binary buffer license data size
 *  @param  name        Optional unique buffer license source name
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Buffer license sources do not need to be named but FlcGetBufferLicenseSourceFeatureCollection
 *        will require a name to access the feature collection for any but the first unnamed buffer
 *        license source.
 *  @note Possible error codes include
 *        FLCERR_DATA_INSUFFICIENT,
 *        FLCERR_DATA_CORRUPTED,
 *        FLCERR_SIGNATURE_INVALID
 */
FlcBool FlcAddBufferLicenseSourceFromData(FlcLicensingRef   licensing,
                                          const FlcUInt8 *  data,
                                          FlcSize           size,
                                          const FlcChar *   name,
                                          FlcErrorRef       error);

/**
 *  Adds a buffer license source to the licensing environment license source collection from the supplied
 *  buffer license file.
 *
 *  @param  licensing   Licensing environment object
 *  @param  fileName    File name of the buffer license file
 *  @param  name        Optional unique buffer license source name
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Buffer license sources do not need to be named but FlcGetBufferLicenseSourceFeatureCollection
 *        will require a name to access the feature collection for any but the first unnamed buffer
 *        license source.
 *  @note Possible error codes include
 *        FLCSWC_FILE_OPEN,
 *        FLCSWC_EMPTY_FILE,
 *        FLCSWC_FILE_READ,
 *        FLCERR_DATA_INSUFFICIENT,
 *        FLCERR_DATA_CORRUPTED,
 *        FLCERR_SIGNATURE_INVALID
 */
FlcBool FlcAddBufferLicenseSourceFromFile(FlcLicensingRef  licensing,
                                          const FlcChar *  fileName,
                                          const FlcChar *  name,
                                          FlcErrorRef      error);

/**
 *  Returns a feature collection of all features contained in the supplied buffer license data
 *
 *  @param  licensing           Licensing environment object
 *  @param  featureCollection   Pointer to variable to receive the feature collection
 *  @param  diagnostic          Diagnostic indicator. If set to FLC_TRUE all features are loaded
 *                              even if they are considered invalid for one reason or another
 *  @param  data                Pointer to the binary buffer license data
 *  @param  size                Binary buffer license data size
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcGetBufferLicenseFeatureCollection(FlcLicensingRef            licensing,
                                             FlcFeatureCollectionRef *  featureCollection,
                                             FlcBool                    diagnostic,
                                             const FlcUInt8 *           data,
                                             FlcSize                    size,
                                             FlcErrorRef                error);
/**
 *  Returns a feature collection of all features contained in the specified buffer license source.
 *
 *  @param  licensing           Licensing environment object
 *  @param  featureCollection   Pointer to variable to receive the feature collection
 *  @param  name                Name used in FlcAddBufferLicenseSourceFromData/FlcAddBufferLicenseSourceFromFile
 *                              or NULL if no name was assigned to the buffer license source.
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Buffer license sources do not need to be named however only features for named or the first unnamed buffer
 *        license source can be returned by this function.
 *
 *  @note Possible error codes include
 *        FLCSWC_BUFFER_LICENSE_SOURCE_NOT_FOUND
 */
FlcBool FlcGetBufferLicenseSourceFeatureCollection(FlcLicensingRef            licensing,
                                                   FlcFeatureCollectionRef *  featureCollection,
                                                   const FlcChar *            name,
                                                   FlcErrorRef                error);

/**
 *  Adds a certificate license source to the licensing environment license source collection from the
 *  supplied data.
 *
 *  @param  licensing         Licensing environment object
 *  @param  certificateData   Pointer to the FNP certificate data string
 *  @param  name              Optional unique certificate license source name
 *  @param  error             Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Certificate license sources do not need to be named but FlcGetCertificateLicenseSourceFeatureCollection
 *        will require a name to access the feature collection for any but the first unnamed certificate
 *        license source.
 *  @note Possible error codes include
 *        FLCERR_UNSUPPORTED_CERTIFICATE_KEYWORD,
 *        FLCERR_UNKNOWN_CERTIFICATE_KEYWORD
 */
FlcBool FlcAddCertificateLicenseSourceFromData(FlcLicensingRef   licensing,
                                               const FlcChar *   certificateData,
                                               const FlcChar *   name,
                                               FlcErrorRef       error);

/**
 *  Adds a certificate license source to the licensing environment license source collection from the supplied
 *  certificate license file.
 *
 *  @param  licensing   Licensing environment object
 *  @param  fileName    File name of the FNP certificate license file
 *  @param  name        Optional unique certificate license source name
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Certificate license sources do not need to be named but FlcGetCertificateLicenseSourceFeatureCollection
 *        will require a name to access the feature collection for any but the first unnamed buffer
 *        license source.
 *  @note Possible error codes include
 *        FLCSWC_FILE_OPEN,
 *        FLCSWC_EMPTY_FILE,
 *        FLCSWC_FILE_READ,
 *        FLCERR_UNSUPPORTED_CERTIFICATE_KEYWORD,
 *        FLCERR_UNKNOWN_CERTIFICATE_KEYWORD
 */
FlcBool FlcAddCertificateLicenseSourceFromFile(FlcLicensingRef  licensing,
                                               const FlcChar *  fileName,
                                               const FlcChar *  name,
                                               FlcErrorRef      error);

/**
 *  Returns a feature collection of all features contained in the supplied cetificate license data
 *
 *  @param  licensing           Licensing environment object
 *  @param  featureCollection   Pointer to variable to receive the feature collection
 *  @param  diagnostic          Diagnostic indicator. If set to FLC_TRUE all features are loaded
 *                              even if they are considered invalid for one reason or another
 *  @param  certificateData     Pointer to the certificate license data
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcGetCertificateLicenseFeatureCollection(FlcLicensingRef            licensing,
                                                  FlcFeatureCollectionRef *  featureCollection,
                                                  FlcBool                    diagnostic,
                                                  const FlcChar *            certificateData,
                                                  FlcErrorRef                error);

/**
 *  Returns a feature collection of all features contained in the specified certificate license source
 *
 *  @param  licensing           Licensing environment object
 *  @param  featureCollection   Pointer to variable to receive the feature collection
 *  @param  name                Name used in FlcAddCertificateLicenseSourceFromData/FlcAddCertificateLicenseSourceFromFile
 *                              or NULL if no name was assigned to the certificate license source.
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Certificate license sources do not need to be named however only features for named or the first unnamed
 *        certificate license source can be returned by this function.
 *
 *  @note Possible error codes include
 *        FLCSWC_CERTIFICATE_LICENSE_SOURCE_NOT_FOUND
 */
FlcBool FlcGetCertificateLicenseSourceFeatureCollection(FlcLicensingRef            licensing,
                                                        FlcFeatureCollectionRef *  featureCollection,
                                                        const FlcChar *            name,
                                                        FlcErrorRef                error);

/**
 *  Indicates whether or not a named license source with a name matching the supplied name currently exists in the
 *  license source collection.
 *
 *  @param  licensing           Licensing environment object
 *  @param  name                License source name to match
 *  @param  exists              Pointer to variable to receive indicator which is set to FLC_TRUE if a license source
 *                              matching the supplied name currently exists in the license source collection and
 *                              FLC_FALSE otherwise
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Currently only buffer and certificate license sources may be named.
 */
FlcBool FlcNamedLicenseSourceExists(FlcLicensingRef            licensing,
                                    const FlcChar *            name,
                                    FlcBool *                  exists,
                                    FlcErrorRef                error);

/**
 *  Indicates whether or not a served buffer license source with server id information matching that contained in the
 *  supplied served buffer capability response data currently exists in the license source collection. If a match is
 *  found a determination is made as to whether or not the supplied capability response data is stale in comparison to
 *  the matching served buffer license source.
 *
 *  @param  licensing           Licensing environment object
 *  @param  responseData        Served buffer binary capability response data
 *  @param  responseSize        Served buffer binary capability response size
 *  @param  licenseSourceExists Pointer to variable to receive indicator which is set to FLC_TRUE if a served buffer license
 *                              source matching the server id information contained in the supplied served buffer capability
 *                              response exists in the license source collection and FLC_FALSE otherwise.
 *  @param  responseIsStale     Pointer to variable to receive indicator which is set to FLC_TRUE if a matching served buffer
 *                              license source if found (exists set to FLC_TRUE) and the provided served buffer capability
 *                              response data served time is earlier than the served time in the matching license source,
 *                              otherwise this value is set to FLC_FALSE.
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note This function can be used to detect stale served buffer license data without processing the data
 */
FlcBool FlcServedBufferLicenseSourceExists(FlcLicensingRef            licensing,
                                           const FlcUInt8 *           responseData,
                                           FlcSize                    responseSize,
                                           FlcBool *                  licenseSourceExists,
                                           FlcBool *                  responseIsStale,
                                           FlcErrorRef                error);

/**
 *  Returns a feature collection of all features contained in the supplied buffer license or capability
 *  response data.
 *
 *  @param  licensing           Licensing environment object
 *  @param  featureCollection   Pointer to variable to receive the feature collection
 *  @param  diagnostic          Diagnostic indicator. If set to FLC_TRUE all features are loaded even
 *                              if they are considered invalid for one reason or another. Ignored
 *                              for trusted storage capability response data which always shows all
 *                              features contained in the capability response.
 *  @param  data                Pointer to the binary buffer license or capability response data
 *  @param  size                Data size
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcGetFeatureCollectionFromData(FlcLicensingRef            licensing,
                                        FlcFeatureCollectionRef *  featureCollection,
                                        FlcBool                    diagnostic,
                                        const FlcUInt8 *           data,
                                        FlcSize                    size,
                                        FlcErrorRef                error);


/**
 *  Returns a feature collection of all features contained in the supplied buffer license or capability
 *  response data file.
 *
 *  @param  licensing           Licensing environment object
 *  @param  featureCollection   Pointer to variable to receive the feature collection
 *  @param  diagnostic          Diagnostic indicator. If set to FLC_TRUE all features are loaded
 *                              even if they are considered invalid for one reason or another.  Ignored
 *                              for trusted storage capability response data which always shows all
 *                              features contained in the capability response.
 *  @param  fileName            File name of the buffer license or capability response data file
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Possible error codes include
 *        FLCSWC_FILE_OPEN,
 *        FLCSWC_EMPTY_FILE,
 *        FLCSWC_FILE_READ
 */
FlcBool FlcGetFeatureCollectionFromFile(FlcLicensingRef            licensing,
                                        FlcFeatureCollectionRef *  featureCollection,
                                        const FlcChar *            fileName,
                                        FlcBool                    diagnostic,
                                        FlcErrorRef                error);

/**
 *  Adds all buffer, served buffer and trial license files contained in the supplied path as license
 *  sources to the specified licensing environment. Only files with a ".bin" extension will be considered.
 *  Files that do not contain buffer, served buffer or trial license data are ignored.
 *  This function is effectively the same as calling the FlcAddFilesToLicenseSourcesEx(..)
 *  function with ".bin" as the value for the 'fileExtension' argument.
 *
 *  @param  licensing   Licensing environment object
 *  @param  dirPath     Directory path to inspect for buffer and trial license files
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcAddFilesToLicenseSources(FlcLicensingRef  licensing,
                                    const FlcChar *  dirPath,
                                    FlcErrorRef      error);

/**
 *  Adds all buffer, served buffer and trial license files contained in the supplied path as license
 *  sources to the specified licensing environment. Only files with the extension indicated by 
 *  the 'fileExtension' parameter will be considered.
 *  Files that do not contain buffer, served buffer or trial license data are ignored.
 *
 *  @param  licensing        Licensing environment object
 *  @param  dirPath          Directory path to inspect for buffer and trial license files
 *  @param  fileExtension    File extension of the files to be considered (including the starting ".") - e.g. : ".alx"
 *  @param  error            Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcAddFilesToLicenseSourcesEx(FlcLicensingRef  licensing,
                                      const FlcChar *  dirPath,
                                      const FlcChar *  fileExtension,
                                      FlcErrorRef      error);

/**
 *  Return all currently acquired non-metered licenses and release all metered licenses
 *
 *  @param  licensing   Licensing environment object
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcReturnAllLicenses(FlcLicensingRef  licensing,
                             FlcErrorRef      error);

/**
 *  Return all currently acquired licenses and delete all current license sources
 *
 *  @param  licensing   Licensing environment object
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcLicensingReset(FlcLicensingRef  licensing,
                          FlcErrorRef      error);

/**
 *  Acquires a license
 *
 *  @param  licensing       Licensing environment object
 *  @param  license         Pointer to variable to receive a reference to the acquired license object
 *  @param  featureName     Name of license to acquire
 *  @param  featureVersion  Lowest acceptable string version of the license
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note If the acquired license is non-metered then it should be returned using FlcReturnLicense or
 *        FlcReturnLicenses. FlcReturnLicense(s) may also be used to return metered reusable or
 *        undoable license counts. Consumed metered licenses are properly destroyed by calling
 *        FlcReleaseLicense when no longer needed.
 *
 *  @note Calling FlcLicenseDelete will release the license storage associated with a reusable or
 *        non-reusable license however it will not fundamentally return or release the license.
 *        An acquired license object can be regenerated through FlcLicenseCollectionCreate and
 *        FlcLicenseCollectionGet
 *
 *  @note The license reference obtained from this function is valid until the license is returned,
 *        released, or deleted or the licensing environment is deleted
 *
 *  @note The lowest acceptable string version for parameter "featureVersion" is "0.0". This case
 *        is equivalent to saying "any version"
 *
 *  @note This API internally acts as FlcAcquireLicensesExtended with acquisitionId and
 *        requestorId set to NULL, and count set to 1.
 *
 *  @note Possible error codes include
 *      FLCERR_VERSION_STRING_INVALID,
 *      FLCERR_FEATURE_NOT_FOUND,
 *      FLCERR_FEATURE_EXPIRED,
 *      FLCERR_FEATURE_NOT_STARTED,
 *      FLCERR_FEATURE_VERSION_NOT_FOUND,
 *      FLCERR_FEATURE_HOST_ID_MISMATCH,
 *      FLCERR_TS_HOST_ID_MISMATCH,
 *      FLCERR_FEATURE_INSUFFICIENT_COUNT,
 *      FLCERR_WINDBACK_DETECTED,
 *      FLCERR_VERSION_STRING_INVALID,
 *      FLCERR_LICENSE_MODEL_CONFLICT,
 *      FLCERR_FEATURE_METERED_ATTRIBUTE_CONFLICT,
 *      FLCERR_FEATURE_OVERDRAFT_NOT_SUPPORTED,
 *      FLCERR_METERED_FEATURES_NOT_ENABLED
 */
FlcBool FlcAcquireLicense(FlcLicensingRef   licensing,
                          FlcLicenseRef *   license,
                          const FlcChar *   featureName,
                          const FlcChar *   featureVersion,
                          FlcErrorRef       error);

/**
 *  Acquires licenses
 *
 *  @param  licensing       Licensing environment object
 *  @param  license         Pointer to variable to receive a reference to the acquired license object
 *  @param  featureName     Name of license to acquire
 *  @param  featureVersion  Lowest acceptable string version of the license
 *  @param  featureCount    Number of licenses to acquire
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Acquires counted licenses across multiple features and license sources. License sources
 *        are searched in the order of their addition to the licensing environment. Features within
 *        a license source are searched in ascending version order.
 *
 *  @note There is no partial fulfillment; if the entire count is not available, this API returns an error
 *
 *  @note If the acquired license is non-metered then it should be returned using FlcReturnLicense or
 *        FlcReturnLicenses. FlcReturnLicense(s) may also be used to return metered reusable or
 *        undoable license counts. Consumed metered licenses are properly destroyed by calling
 *        FlcReleaseLicense when no longer needed.
 *
 *  @note Calling FlcLicenseDelete will release the license storage associated with a reusable or
 *        non-reusable license however it will not fundamentally return or release the license.
 *        An acquired license object can be regenerated through FlcLicenseCollectionCreate and
 *        FlcLicenseCollectionGet
 *
 *  @note The license reference obtained from this function is valid until the license is returned,
 *        released, or deleted or the licensing environment is deleted
 *
 *  @note The lowest acceptable string version for parameter "featureVersion" is "0.0". This case
 *        is equivalent to saying "any version"
 *
 *  @note This API internally acts as FlcAcquireLicensesExtended with acquisitionId and
 *        requestorId set to NULL.
 *
 *  @note Possible error codes include
 *      FLCERR_VERSION_STRING_INVALID,
 *      FLCERR_FEATURE_NOT_FOUND,
 *      FLCERR_FEATURE_EXPIRED,
 *      FLCERR_FEATURE_NOT_STARTED,
 *      FLCERR_FEATURE_VERSION_NOT_FOUND,
 *      FLCERR_FEATURE_HOST_ID_MISMATCH,
 *      FLCERR_TS_HOST_ID_MISMATCH,
 *      FLCERR_FEATURE_INSUFFICIENT_COUNT,
 *      FLCERR_WINDBACK_DETECTED,
 *      FLCERR_VERSION_STRING_INVALID,
 *      FLCERR_LICENSE_MODEL_CONFLICT,
 *      FLCERR_FEATURE_METERED_ATTRIBUTE_CONFLICT,
 *      FLCERR_FEATURE_OVERDRAFT_NOT_SUPPORTED,
 *      FLCERR_METERED_FEATURES_NOT_ENABLED
 */
FlcBool FlcAcquireLicenses(FlcLicensingRef  licensing,
                           FlcLicenseRef *  license,
                           const FlcChar *  featureName,
                           const FlcChar *  featureVersion,
                           FlcUInt32        featureCount,
                           FlcErrorRef      error);

/**
 *  Acquires licenses
 *
 *  @param  licensing       Licensing environment object
 *  @param  license         Pointer to variable to receive a reference to the acquired license object
 *  @param  featureName     Name of license to acquire
 *  @param  featureVersion  Lowest acceptable string version of the license
 *  @param  featureCount    Number of licenses to acquire
 *  @param  requestorId     Optional ID of requestor, used for reporting purposes
 *  @param  acquisitionId   Optional ID of acquisition, used for reporting purposes
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Requestor Id is for reporting only and can be set to NULL. In this case, the system host id
 *        will be used in its place.
 *
 *  @note Acquisition Id is reserved for future use. It also can be set to NULL so the system host id is
 *        used in its place
 *
 *  @note Acquires counted licenses across multiple features and license sources. License sources
 *        are searched in the order of their addition to the licensing environment. Features within
 *        a license source are searched in ascending version order.
 *
 *  @note There is no partial fulfillment; if the entire count is not available, this API returns an error
 *
 *  @note If the acquired license is non-metered then it should be returned using FlcReturnLicense or
 *        FlcReturnLicenses. FlcReturnLicense(s) may also be used to return metered reusable or
 *        undoable license counts. Consumed metered licenses are properly destroyed by calling
 *        FlcReleaseLicense when no longer needed.
 *
 *  @note Calling FlcLicenseDelete will release the license storage associated with a reusable or
 *        non-reusable license however it will not fundamentally return or release the license.
 *        An acquired license object can be regenerated through FlcLicenseCollectionCreate and
 *        FlcLicenseCollectionGet
 *
 *  @note The license reference obtained from this function is valid until the license is returned,
 *        released, or deleted or the licensing environment is deleted
 *
 *  @note The lowest acceptable string version for parameter "version" is "0.0". This case
 *        is equivalent to saying "any version"
 *
 *
 *  @note Possible error codes include
 *      FLCERR_VERSION_STRING_INVALID,
 *      FLCERR_FEATURE_NOT_FOUND,
 *      FLCERR_FEATURE_EXPIRED,
 *      FLCERR_FEATURE_NOT_STARTED,
 *      FLCERR_FEATURE_VERSION_NOT_FOUND,
 *      FLCERR_FEATURE_HOST_ID_MISMATCH,
 *      FLCERR_TS_HOST_ID_MISMATCH,
 *      FLCERR_FEATURE_INSUFFICIENT_COUNT,
 *      FLCERR_WINDBACK_DETECTED,
 *      FLCERR_VERSION_STRING_INVALID,
 *      FLCERR_LICENSE_MODEL_CONFLICT,
 *      FLCERR_FEATURE_METERED_ATTRIBUTE_CONFLICT,
 *      FLCERR_FEATURE_OVERDRAFT_NOT_SUPPORTED,
 *      FLCERR_METERED_FEATURES_NOT_ENABLED
 */
FlcBool FlcAcquireLicensesExtended(FlcLicensingRef   licensing,
                                   FlcLicenseRef *   license,
                                   const FlcChar *   featureName,
                                   const FlcChar *   featureVersion,
                                   FlcUInt32         featureCount,
                                   const FlcChar *   requestorId,
                                   const FlcChar *   acquisitionId,
                                   FlcErrorRef       error);

/**
 *  Returns an acquired non-metered license or a metered license which is reusable or for which the
 *  undo interval has not expired.
 *
 *  @param  licensing       Licensing environment object
 *  @param  license         Pointer to variable to containing reference to the license object
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   Functionally the same as FlcReturnLicenses.
 *
 *  @note   Supports counted licenses. The returned count is restored to the pool of
 *          available licenses.
 *
 *  @note   Successful return of the license invalidates its reference.
 *
 *  @note Possible error codes include
 *      FLCERR_LICENSE_NOT_FOUND,
 *      FLCERR_METERED_LICENSE_NOT_REUSABLE,
 *      FLCERR_METERED_LICENSE_UNDO_INTERVAL_EXPIRED
 */
FlcBool FlcReturnLicense(FlcLicensingRef  licensing,
                         FlcLicenseRef *  license,
                         FlcErrorRef      error);

/**
 *  Returns an acquired non-metered license or a metered license which is reusable or for which the
 *  undo interval has not expired.
 *
 *  @param  licensing       Licensing environment object
 *  @param  license         Pointer to variable to containing reference to the license object
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   Supports counted licenses. The returned count is restored to the pool of
 *          available licenses.
 *
 *  @note   Successful return of the license invalidates its reference.
 *
 *  @note Possible error codes include
 *      FLCERR_LICENSE_NOT_FOUND,
 *      FLCERR_METERED_LICENSE_NOT_REUSABLE,
 *      FLCERR_METERED_LICENSE_UNDO_INTERVAL_EXPIRED
 */
FlcBool FlcReturnLicenses(FlcLicensingRef  licensing,
                          FlcLicenseRef *  license,
                          FlcErrorRef      error);

/**
 *  Releases a metered or stale license
 *
 *  @param  licensing       Licensing environment object
 *  @param  license         Pointer to variable to containing reference to the license object
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   Functionally the same as FlcReleaseLicenses.
 *
 *  @note   Successful release of the license invalidates its reference.
 *
 *  @note   If invoked on a non-metered license this method will first attempt to return
 *          the license counts however it is recommended to always use the FlcReturnLicense function to
 *          return non-metered licenses. You must use the FlcReturnLicense function to return counts
 *          associated with a reusable or undoable metered license.
 *
 *  @note Possible error codes include
 *      FLCERR_LICENSE_NOT_FOUND,
 */
FlcBool FlcReleaseLicense(FlcLicensingRef  licensing,
                          FlcLicenseRef *  license,
                          FlcErrorRef      error);

/**
 *  Releases a metered or stale license
 *
 *  @param  licensing       Licensing environment object
 *  @param  license         Pointer to variable to containing reference to the license object
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   Successful return of the license invalidates its reference.
 *
 *  @note   If invoked on a non-metered license this method will first attempt to return
 *          the license counts however it is recommended to always use the FlcReturnLicense function to
 *          return non-metered licenses. You must use the FlcReturnLicense function to return counts
 *          associated with a reusable or undoable metered license.
 *
 *  @note Possible error codes include
 *      FLCERR_LICENSE_NOT_FOUND
 */
FlcBool FlcReleaseLicenses(FlcLicensingRef  licensing,
                           FlcLicenseRef *  license,
                           FlcErrorRef      error);

/**
 *  Checks to see whether the counts associated with the target license object are returnable.
 *
 *  @param  licensing       Licensing environment object
 *  @param  license         Reference to the license object to check
 *  @param  isReturnable    Pointer to variable to receive the returnable indicator,
 *                          FLC_TRUE if the license can be returned
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   Stale licenses are not considered returnable. Also metered (MODEL=metered) non-reusable
 *          licenses will not be considered returnable if no undo interval is specified or if the undo
 *          interval has expired.
 *
 *  @note   If the license is not returnable FlcLicenseRelease should be issued to clean up the
 *          license object storage when no longer needed.
 */
FlcBool FlcLicenseIsReturnable(FlcLicensingRef licensing,
                               FlcLicenseRef   license,
                               FlcBool *       isReturnable,
                               FlcErrorRef     error);

/**
 *  Checks to see whether a license is valid. A license is valid if it has not already been returned or released
 *
 *  @param  licensing       Licensing environment object
 *  @param  license         Reference to the license object to check
 *  @param  isValid         Pointer to variable to receive the license valid indicator set to FLC_TRUE
 *                          if the license is valid, and FLC_FALSE if the license has been returned or released
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   This function would not typically be used and can only return an isValid value of FLC_FALSE if
 *          the license object represents a license that has already been returned or released via
 *          FlcReturnAllLicenses, FlcClearAllLicenses, FlcLicensingReset, or through a duplicate license object
 *          obtained from an acquired license collection (see FlcLicenseCollectionCreate).
 *
 *  @note   If the license has been returned FlcLicenseDelete should be issued to clean up the
 *          license object storage.
 *
 */
FlcBool FlcLicenseIsValid(FlcLicensingRef licensing,
                          FlcLicenseRef   license,
                          FlcBool *       isValid,
                          FlcErrorRef     error);

/**
 *  Gets the count available for acquisition for a specific feature
 *
 *  @param  licensing       Licensing environment object
 *  @param  featureName     Feature name of interest
 *  @param  featureVersion  Feature version of interest
 *  @param  count           Pointer to variable to receive the available feature count
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Sets count to FLC_FEATURE_UNCOUNTED_VALUE for an uncounted feature.
 *
 *  @note Possible error codes include
 *      FLCERR_VERSION_STRING_INVALID,
 *      FLCERR_LICENSE_MODEL_CONFLICT
 */
FlcBool FlcGetAvailableAcquisitionCount(FlcLicensingRef   licensing,
                                        const FlcChar *   featureName,
                                        const FlcChar *   featureVersion,
                                        FlcInt32 *        count,
                                        FlcErrorRef       error);

/**
 *  Creates a license collection containing a license object for every currently acquired
 *  license.
 *
 *  @param  licensing           Licensing environment object
 *  @param  licenseCollection   Pointer to the variable to receive the reference to the license
 *                              collection to be created
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The caller is responsible for deleting the license collection object via the
 *          FlcLicenseCollectionDelete API.
 */
FlcBool FlcLicenseCollectionCreate(FlcLicensingRef           licensing,
                                   FlcLicenseCollectionRef * licenseCollection,
                                   FlcErrorRef               error);

/**
 *  Deletes the license collection object
 *
 *  @param  licenseCollection   Pointer to the variable containing the reference to the license
 *                              collection to be deleted
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcLicenseCollectionDelete(FlcLicenseCollectionRef * licenseCollection,
                                   FlcErrorRef               error);

/**
 *  Gets the size of the license collection object
 *
 *  @param  licenseCollection   License collection object of interest
 *  @param  size                  Pointer to variable to receive returned size
 *  @param  error                 Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcLicenseCollectionSize(FlcLicenseCollectionRef licenseCollection,
                                 FlcSize *               size,
                                 FlcErrorRef             error);

/**
 *  Gets the license at a given index
 *
 *  @param  licenseCollection   License collection object of interest
 *  @param  license             Pointer to variable to receive returned license reference
 *  @param  index               Index, ranging from 0 to (license collection size - 1)
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note If the license is returnable it may be returned using FlcReturnLicense or FlcReturnLicenses.
 *        The storage associated with the license may also be released via FlcLicenseDelete
 *
 *  @note Calling FlcLicenseDelete will release the license storage but will not return the license
 *
 *  @note The license reference obtained from this function is valid until the license is returned,
 *        deleted or the licensing environment is deleted
 *
 *  @note Possible error codes include
 *      FLCERR_INDEX_OUT_OF_BOUND,
 *      FLCERR_LICENSE_NOT_FOUND (if attempting to access an index which maps to a previously
 *                                returned license)
 */
FlcBool FlcLicenseCollectionGet(FlcLicenseCollectionRef licenseCollection,
                                FlcLicenseRef *         license,
                                FlcUInt32               index,
                                FlcErrorRef             error);

/**
 *  Fetches the error status collection associated with the last unsuccessful request that supports
 *  error status information from the current thread.
 *
 *  @param  licensing               Licensing environment object
 *  @param  errorStatusCollection   Pointer to the variable to receive the reference to the error status
 *                                  collection associated with the last unsuccessful request from the
 *                                  current thread. NULL if no errors codes were reported.
 *  @param  error                   Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Currently only FlcCapabilityResponseCreate or FlcCapabilityResponseCreateFromFile functions
 *      support the return of an error status collection on unsuccessful completion.
 */
FlcBool FlcGetLastErrorStatusCollection(FlcLicensingRef           licensing,
                                        FlcStatusCollectionRef *  errorStatusCollection,
                                        FlcErrorRef               error);
/**
 *  Indicates whether the application is currently running in a container.
 *
 *  @param  licensing         Licensing environment object
 *  @param  isContainerized   Pointer to variable to notify that the application is running in a container,
 *                            FLC_TRUE if the application is containerized.
 *  @param  error             Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcIsContainerized(FlcLicensingRef   licensing,
                                     FlcBool *         isContainerized,
                                     FlcErrorRef       error);

/** @} */ /* end of licenseManager group */
/** @} */ /* end of addtogroup licensingClient */

#ifdef __cplusplus
}
#endif

#endif /* _INCLUDE_FLC_LICENSEMANAGER_H_ */
