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
 *  Provides interfaces to the licensing client short code environment.
 *
 *  These functions provide total support for short code functionality enabling
 *  the implemter to manage short code templates, create short code requests,
 *  examine short code responses, and process short code responses into a short
 *  code license source.
 */

#ifndef _INCLUDE_FLC_SHORTCODE_H_
#define _INCLUDE_FLC_SHORTCODE_H_

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
 * @defgroup shortCode Short Code
 * @{
 */

/** Opaque data type for the short code environment object.
*/
typedef struct flcShortCodeEnvironment * FlcShortCodeEnvironmentRef;

/**
 *  Enumeration describing short code request/response encoding.
 */
typedef enum FlcShortCodeEncoding
{
  FLC_SHORT_CODE_ENCODE_USER         = 0,
  FLC_SHORT_CODE_ENCODE_CUSTOM       = 1,
  FLC_SHORT_CODE_ENCODE_BASE10       = 2,
  FLC_SHORT_CODE_ENCODE_BASE16       = 3,
  FLC_SHORT_CODE_ENCODE_BASE34       = 4,
  FLC_SHORT_CODE_ENCODE_BASE36       = 5
} FlcShortCodeEncoding;

/**
 *  Creates the short code environment object
 *
 *  @param  licensing      Licensing environment object
 *  @param  shortCodeEnv   Pointer to the variable to receive the short code environment object to be created
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note   The caller is responsible for deleting the short code environment object via the 
 *          FlcShortCodeEnvironmentDelete API.
 */
FlcBool FlcShortCodeEnvironmentCreate(FlcLicensingRef              licensing,
                                      FlcShortCodeEnvironmentRef * shortCodeEnv,
                                      FlcErrorRef                  error);

/**
 *  Deletes the short code environment object
 *
 *  @param  shortCodeEnv   Short code environment object to delete
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcShortCodeEnvironmentDelete(FlcShortCodeEnvironmentRef * shortCodeEnv,
                                      FlcErrorRef                  error);

/**
 *  Add the short code template defined by the provided data to the short code environment
 *
 *  @param  shortCodeEnv   Short code environment object
 *  @param  templateId     Pointer to variable to return the short code template id or NULL if
 *                         return of the template id is not required
 *  @param  templateData   Binary template data
 *  @param  templateSize   Binary template data size
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note If a template currently exists in the short code environment with the same template id
 *        as that in the supplied data it will be replaced with the supplied data.
 */
FlcBool FlcShortCodeEnvironmentAddTemplateFromData(FlcShortCodeEnvironmentRef   shortCodeEnv,
                                                   FlcUInt16 *                  templateId,
                                                   const FlcUInt8 *             templateData,
                                                   FlcSize                      templateSize,
                                                   FlcErrorRef                  error);

/**
 *  Add the short code template defined by the data contained within the provided file name
 *
 *  @param  shortCodeEnv   Short code environment object
 *  @param  templateId     Pointer to variable to return the short code template id or NULL if
 *                         return of the template id is not required
 *  @param  templateFile   File containing the binary template data
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note If a template currently exists in the short code environment with the same template id
 *        as that in the supplied template file it will be replaced with the supplied file data.
 *
 *  @note Possible error codes include
 *        FLCSWC_FILE_OPEN  
 *        FLCSWC_EMPTY_FILE 
 *        FLCSWC_FILE_READ  
 */
FlcBool FlcShortCodeEnvironmentAddTemplateFromFile(FlcShortCodeEnvironmentRef   shortCodeEnv,
                                                   FlcUInt16 *                  templateId,
                                                   const FlcChar *              templateFile,
                                                   FlcErrorRef                  error);

/**
 *  Attempt to add all files matching the supplied regular expression as templates to the 
 *  short code environment
 *
 *  @param  shortCodeEnv   Short code environment object
 *  @param  templateCount  Pointer to varable to return the count of files successfully added
 *                         as short code templates to the short code environment
 *  @param  templatePath   Path to template files
 *  @param  pattern        Regular expression pattern to be used in matching file names or 
 *                         NULL if all files are to be processed
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Possible error codes include
 *        FLCSWC_REGEX_EXPRESSION_ERROR
 */
FlcBool FlcShortCodeEnvironmentAddTemplatesFromFiles(FlcShortCodeEnvironmentRef   shortCodeEnv,
                                                     FlcUInt32 *                  templateCount,
                                                     const FlcChar *              templatePath,
                                                     const FlcChar *              pattern,
                                                     FlcErrorRef                  error);
/**
 *  Clear all short code templates from the short code environment
 *
 *  @param  shortCodeEnv   Short code environment object
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcShortCodeEnvironmentClearTemplates(FlcShortCodeEnvironmentRef   shortCodeEnv,
                                              FlcErrorRef                  error);

/**
 *  Gets the short code environment template collection size
 *
 *  @param  shortCodeEnv   Short code environment object
 *  @param  size           Pointer to variable to receive the template collection size
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcShortCodeEnvironmentGetTemplateCollectionSize(FlcShortCodeEnvironmentRef   shortCodeEnv,
                                                         FlcUInt32 *                  size,
                                                         FlcErrorRef                  error);

/**
 *  Gets the template id of the short code template item at the specified index in the short code 
 *  environment template collection
 *
 *  @param  shortCodeEnv   Short code environment object
 *  @param  templateId     Pointer to variable to receive the template id
 *  @param  index          Index of the template item in the short code environment template collection
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcShortCodeEnvironmentGetTemplateCollectionItemId(FlcShortCodeEnvironmentRef   shortCodeEnv,
                                                           FlcUInt16 *                  templateId,
                                                           FlcUInt32                    index,
                                                           FlcErrorRef                  error);
/**
 *  Sets the encoding type to be used in encoding short code requests. If not specified the default is
 *  FLC_SHORT_CODE_ENCODE_BASE34. 
 *
 *  @param  shortCodeEnv        Short code environment object
 *  @param  encoding            Short code request encoding type to use, see FlcShortCodeEncoding for valid values
 *  @param  encodingCharacters  Set of valid encoding characters as a character string. Ignored unless encoding 
 *                              is set to FLC_SHORT_CODE_ENCODE_CUSTOM
 *  @param  segmentSize         Segment size to use when embedding checksum digits
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note If FlcShortCodeSetRequestEncoding is not called the default encoding value of FLC_SHORT_CODE_ENCODE_BASE34
 *        is used.
 *
 *  @note If FLC_SHORT_CODE_ENCODE_USER is specified then no short code request encoding is performed and it is
 *        assumed that the implementer will perform their own encoding on the request data.
 *
 *  @note Encoded data may contain a number of embedded checksum digits, if a segment size is specified.
 *        The same segment size and character set must be used to decode the resultant data.
 */
FlcBool FlcShortCodeSetRequestEncoding(FlcShortCodeEnvironmentRef  shortCodeEnv,
                                       FlcShortCodeEncoding        encoding,
                                       const FlcChar *             encodingCharacters,
                                       FlcSize                     segmentSize,                
                                       FlcErrorRef                 error);

/**
 *  Sets the decoding type to be used in decoding short code responses. If not specified the default is
 *  FLC_SHORT_CODE_ENCODE_BASE34. 
 *
 *  @param  shortCodeEnv        Short code environment object
 *  @param  encoding            Short code response encoding type, see FlcShortCodeEncoding for valid values
 *  @param  encodingCharacters  Set of valid encoding characters as a character string. Ignored unless encoding 
 *                              is set to FLC_SHORT_CODE_ENCODE_CUSTOM
 *  @param  segmentSize         Segment size to use when decoding the short code response
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note If FlcShortCodeSetRequestDecoding is not called the default encoding value of FLC_SHORT_CODE_ENCODE_BASE34
 *        is used.
 *
 *  @note If FLC_SHORT_CODE_ENCODE_USER is specified then no short code response decoding is performed and it is
 *        assumed that the implementer has already decoded the short code response data.
 *
 *  @note Encoded data may contain a number of embedded checksum digits, if a segment size is specified.
 *        The same segment size and character set must be used to decode the resultant data.
 */
FlcBool FlcShortCodeSetResponseDecoding(FlcShortCodeEnvironmentRef  shortCodeEnv,
                                        FlcShortCodeEncoding        encoding,
                                        const FlcChar *             encodingCharacters,
                                        FlcSize                     segmentSize,                
                                        FlcErrorRef                 error);
/**
 *  Gets the feature colleciton associated with the short code template item matching the specified 
 *  template id
 *
 *  @param  shortCodeEnv        Short code environment object
 *  @param  templateId          Template id
 *  @param  featureCollection   Pointer to variable the receive the template feature collection 
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcShortCodeTemplateGetFeatureCollection(FlcShortCodeEnvironmentRef  shortCodeEnv,
                                                 FlcUInt16                   templateId,
                                                 FlcFeatureCollectionRef *   featureCollection,
                                                 FlcErrorRef                 error);

/**
 *  Gets the vendor dictionary associated with the short code template item matching the specified 
 *  template id
 *
 *  @param  shortCodeEnv        Short code environment object
 *  @param  templateId          Template id
 *  @param  vendorDictionary    Pointer to variable the receive the template vendor dictionary
 *  @param  error               Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcShortCodeTemplateGetVendorDictionary(FlcShortCodeEnvironmentRef  shortCodeEnv,
                                                FlcUInt16                   templateId,
                                                FlcDictionaryRef *          vendorDictionary,
                                                FlcErrorRef                 error);

/**
 *  Gets the expiration associated with the short code template item matching the specified 
 *  template id
 *
 *  @param  shortCodeEnv   Short code environment object
 *  @param  templateId     Template id
 *  @param  expiration     Pointer to variable the receive the template expiration
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcShortCodeTemplateGetExpiration(FlcShortCodeEnvironmentRef  shortCodeEnv,
                                          FlcUInt16                   templateId,
                                          struct tm **                expiration,
                                          FlcErrorRef                 error);

/**
 *  Gets the machine type associated with the short code template item matching the specified 
 *  template id
 *
 *  @param  shortCodeEnv   Short code environment object
 *  @param  templateId     Template id
 *  @param  type           Pointer to variable to receive machine type, see FlcMachineType for valid values 
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcShortCodeTemplateGetVirtualMachineType(FlcShortCodeEnvironmentRef  shortCodeEnv,
                                                  FlcUInt16                   templateId,
                                                  FlcMachineType *            type,
                                                  FlcErrorRef                 error);

/**
 *  Sets publisher specific data to use when generating a short code request
 *
 *  @param  shortCodeEnv   Short code environment object
 *  @param  data           Publisher data; maximum size allowed is 255 bytes
 *  @param  size           Size of the publisher data
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcShortCodeRequestSetPublisherData(FlcShortCodeEnvironmentRef  shortCodeEnv,
                                            const FlcUInt8 *            data,
                                            FlcSize                     size,
                                            FlcErrorRef                 error);

/**
 *  Generates the short code request using the current short code environment settings.
 *
 *  @param  shortCodeEnv   Short code environment object
 *  @param  templateId     Template id to use in generating the short code request. The template
 *                         matching this template id must exist in the short code environment 
 *                         template collection
 *  @param  requestData    Pointer to variable to receive the generated short code request data
 *  @param  requestSize    Pointer to variable to receive the generated short code request size
 *  @param  error                Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *
 *  @note If an encoding other than FLC_SHORT_CODE_ENCODE_USER is specified (see FlcShortCodeSetRequestEncoding)
 *        then the requestData returned will be a null terminated character string and can be safely cast to
 *        a FlcChar *.
 *
 *  @note On success the caller will be responsible for deallocating the short code request data 
 *        using the FlcMemoryFree API.
 */
FlcBool FlcShortCodeRequestGenerate(FlcShortCodeEnvironmentRef  shortCodeEnv,
                                    FlcUInt16                   templateId,
                                    FlcUInt8 **                 requestData,
                                    FlcSize *                   requestSize,
                                    FlcErrorRef                 error);

/**
 *  Generates the short code request using the current short code environment request encoding
 *  settings and the supplied template data. 
 *
 *  @param  shortCodeEnv   Short code environment object
 *  @param  templateData   Template data to use in generating the short code request
 *  @param  templateSize   Size of the template data
 *  @param  requestData    Pointer to variable to receive the generated short code request data
 *  @param  requestSize    Pointer to variable to receive the generated short code request size
 *  @param  templateId     Pointer to variable to return the short code template id or NULL if
 *                         return of the template id is not required
 *  @param  error                Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note On success the template generated by the supplied binary data will be added to the short 
 *        code environment template collection.
 *
 *  @note If an encoding other than FLC_SHORT_CODE_ENCODE_USER is specified (see FlcShortCodeSetRequestEncoding)
 *        then the requestData returned will be a null terminated character string and can be safely cast to
 *        a FlcChar *.
 *
 *  @note On success the caller will be responsible for deallocating the short code request data 
 *        using the FlcMemoryFree API.
 */
FlcBool FlcShortCodeRequestGenerateFromData(FlcShortCodeEnvironmentRef  shortCodeEnv,
                                            const FlcUInt8 *            templateData,
                                            FlcSize                     templateSize,
                                            FlcUInt8 **                 requestData,
                                            FlcSize *                   requestSize,
                                            FlcUInt16 *                 templateId,
                                            FlcErrorRef                 error);

/**
 *  Generates the short code request using the current short code environment request encoding
 *  settings and the supplied template data file. 
 *
 *  @param  shortCodeEnv   Short code environment object
 *  @param  templateFile   File containing the binary template data
 *  @param  requestData    Pointer to variable to receive the generated short code request data
 *  @param  requestSize    Pointer to variable to receive the generated short code request size
 *  @param  templateId     Pointer to variable to return the short code template id or NULL if
 *                         return of the template id is not required
 *  @param  error                Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note On success the template generated by the data in the supplied file will be added to the
 *        short code environment template collection.
 *
 *  @note If an encoding other than FLC_SHORT_CODE_ENCODE_USER is specified (see FlcShortCodeSetRequestEncoding)
 *        then the requestData returned will be a null terminated character string and can be safely cast to
 *        a FlcChar *.
 *
 *  @note On success the caller will be responsible for deallocating the short code request data 
 *        using the FlcMemoryFree API.
 *
 *  @note Possible error codes include
 *        FLCSWC_FILE_OPEN  
 *        FLCSWC_EMPTY_FILE 
 *        FLCSWC_FILE_READ  
 */
FlcBool FlcShortCodeRequestGenerateFromFile(FlcShortCodeEnvironmentRef  shortCodeEnv,
                                            const FlcChar *             templateFile,
                                            FlcUInt8 **                 requestData,
                                            FlcSize *                   requestSize,
                                            FlcUInt16 *                 templateId,
                                            FlcErrorRef                 error);

/**
 *  Clears any information associated with the current short code response from the short code
 *  environment.
 *
 *  @param  shortCodeEnv   Short code environment object
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcShortCodeResponseReset(FlcShortCodeEnvironmentRef  shortCodeEnv,
                                  FlcErrorRef                 error);

/**
 *  Adds a segment to the short code environment short code response. After all short code response
 *  segments have been processed by this API the license rights contained in the short code response
 *  may be applied by the FlcProcessShortCodeResponse APIs.
 *
 *  @param  shortCodeEnv   Short code environment object
 *  @param  responseData   Pointer to the short code response data segment string
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Only use this API if the segment size is not 0 (see FlcShortCodeSetResponseDecoding).
 *
 *  @note If no segment size was used to generate the short code response then the FlcShortCodeResponseSet
 *        API should be used to set the short code response in the short code environment.
 *
 *  @note If the short code response is being decoded by the implementer then the 
 *        FlcShortCodeResponseSetWithDecodedData API should be used to set the short code 
 *        response in the short code environment.
 *
 *  @note FlcShortCodeResponseAdd will need to be called 1 to N times with sequential pieces of the 
 *        response data. Each response string passed must be of the same size as the decoding
 *        segment size with the exception of the last call when the data size may be less. If the
 *        short code response data is an integral multiple of the decoding segment size then a final
 *        call with a NULL or 0 length string must be performed.
 *
 *  @note Possible error codes include
 *        FLCSWC_SHORTCODE_INVALID_SEGMENT_SIZE_FOR_API
 *        FLCSWC_SHORTCODE_RESPONSE_DECODING_NOT_SET
 *        FLCSWC_SHORTCODE_RESPONSE_INCOMPLETE
 *        FLCERR_CHECKSUM_SEGMENT_LENGTH_MISMATCH 
 */
FlcBool FlcShortCodeResponseAddSegment(FlcShortCodeEnvironmentRef  shortCodeEnv,
                                       const FlcChar *             responseData,
                                       FlcErrorRef                 error);

/**
 *  Sets the short code environment short code response. After successful completion of this API
 *  the license rights contained in the short code response may be applied by the
 *  FlcProcessShortCodeResponse APIs.
 *
 *  @param  shortCodeEnv   Short code environment object
 *  @param  responseData   Pointer to the short code response data string
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Only use this API if the segment size is 0 (see FlcShortCodeSetResponseDecoding).
 *
 *  @note If a segment size was used to generate the short code response then the FlcShortCodeResponseAdd 
 *        API should be used to set the short code response in the short code environment.
 *
 *  @note If the short code response is being decoded by the implementer then the 
 *        FlcShortCodeResponseSetWithDecodedData API should be used to set the short code 
 *        response in the short code environment.
 *
 *  @note Possible error codes include
 *        FLCSWC_SHORTCODE_INVALID_SEGMENT_SIZE_FOR_API 
 *        FLCSWC_SHORTCODE_RESPONSE_DECODING_NOT_SET
 */
FlcBool FlcShortCodeResponseSet(FlcShortCodeEnvironmentRef  shortCodeEnv,
                                const FlcChar *             responseData,
                                FlcErrorRef                 error);

/**
 *  Sets the short code environment short code response using data already decoded by the implementer.
 *
 *  @param  shortCodeEnv   Short code environment object
 *  @param  decodedData    Pointer to the decoded response data
 *  @param  decodedSize    Decoded response data size
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note The short code environment request decoding value (see FlcShortCodeSetResponseDecoding) 
 *        is ignored during the execution of this API.
 */
FlcBool FlcShortCodeResponseSetWithDecodedData(FlcShortCodeEnvironmentRef  shortCodeEnv,
                                               const FlcUInt8 *            decodedData,
                                               FlcSize                     decodedSize,
                                               FlcErrorRef                 error);

/**
 *  Gets the publisher data from the current short code environment response data
 *
 *  @param  shortCodeEnv   Short code environment object
 *  @param  data           Pointer to variable to receive the short code response publisher data
 *  @param  size           Pointer to variable to receive the short code response publisher data size
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note The returned publisher data address is valid until a new short code response is created by
 *        calling one of the FlcShortCodeResponseAddSegment, FlcShortCodeResponseSet or 
 *        FlcShortCodeResponseSetWithDecodedData APIs.
 *
 *  @note Possible error codes include
 *        FLCSWC_SHORTCODE_RESPONSE_INCOMPLETE
 *        FLCSWC_SHORTCODE_RESPONSE_NOT_FOUND
 */
FlcBool FlcShortCodeResponseGetPublisherData(FlcShortCodeEnvironmentRef  shortCodeEnv,
                                             const FlcUInt8 **           data,
                                             FlcSize *                   size,
                                             FlcErrorRef                 error);

/**
 *  Gets the template id from the current short code environment response data
 *
 *  @param  shortCodeEnv   Short code environment object
 *  @param  templateId     Pointer to variable to receive the short code response template id
 *  @param  error          Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note Possible error codes include
 *        FLCSWC_SHORTCODE_RESPONSE_INCOMPLETE
 *        FLCSWC_SHORTCODE_RESPONSE_NOT_FOUND
 */
FlcBool FlcShortCodeResponseGetTemplateId(FlcShortCodeEnvironmentRef  shortCodeEnv,
                                          FlcUInt16 *                 templateId,
                                          FlcErrorRef                 error);


/** @} */ /* end of shortCode group */
/** @} */ /* end of addtogroup licensingClient */


#ifdef __cplusplus
}
#endif

#endif /* _INCLUDE_FLC_SHORTCODE_H_ */
