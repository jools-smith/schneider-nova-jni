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
 *  Provides the interface to communications functionality.
 *
 *  The functions provided enable the caller to create and delete a 
 *  communications object which can be used to communicate with server,
 */

/**
 * @addtogroup clientCommon
 * @{
 */

/**
 * @defgroup comm Comm
 * @{
 * @brief The functions provided enable the caller to create, delete, and
 * modify a communication object used to communicate information to a server.
 */


#ifndef _INCLUDE_FLC_COMM_H_
#define _INCLUDE_FLC_COMM_H_

#include "FlcTypes.h"
#include "FlcErrorCodes.h"
#include "FlcError.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Opaque data type for the communications object.
*/
typedef struct flcComm * FlcCommRef;

/** Function pointer for memory allocation.
*/
typedef void * (* FlcCommMemoryAllocFnPtr)(const void * newdata,
                                           size_t newdataSize,
                                           void * userData);
/**
 *  Creates a communications object
 *
 *  @param  comm        Communications object
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 */
FlcBool FlcCommCreate(FlcCommRef *     comm,
                      FlcErrorRef      error);

/**
 *  Deletes a communications object
 *
 *  @param  comm        Communications object
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCommDelete(FlcCommRef *     comm,
                      FlcErrorRef      error);

/**
 *  Sets the target server URL.
 *
 *  @param  comm        Communications object
 *  @param  server      Target server URL
 *  @param  error       Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCommSetServer(FlcCommRef       comm,
                         const FlcChar *  server,
                         FlcErrorRef      error);

/**
 *  Sets the communication connection timeout value.
 *
 *  @param  comm          Communications object
 *  @param  timeout       Connection timeout value in seconds. A value of 0 disables the connection timeout
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCommSetConnectTimeout(FlcCommRef     comm,
                                 FlcUInt32      timeout,
                                 FlcErrorRef    error);

/**
 *  Sets the maximum amount of time allowed for the communication transfer operation.
 *
 *  @param  comm          Communications object
 *  @param  timeout       Transfer timeout value in seconds. A value of 0 disables the transfer timeout
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCommSetTransferTimeout(FlcCommRef     comm,
                                  FlcUInt32      timeout,
                                  FlcErrorRef    error);

/**
 *  Sets communications proxy information
 *
 *  @param  comm          Communications object
 *  @param  proxyServer   Proxy server hostname or IP address. Any specified protocol prefix (such as http://) is ignored.
 *  @param  port          Proxy server port
 *  @param  userid        Proxy server userid credentials (if needed, otherwise NULL)
 *  @param  password      Proxy server password credentials (if needed, otherwise NULL)
 *  @param  error         Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCommSetProxyDetails(FlcCommRef       comm,
                               const FlcChar *  proxyServer,
                               FlcUInt32        port,
                               const FlcChar *  userid,
                               const FlcChar *  password,
                               FlcErrorRef      error);

/**
 *  Sets communications proxy auto-detect flag value
 *
 *  @param  comm              Communications object
 *  @param  proxyAutoDetect   Proxy auto-detect flag value
 *  @param  error             Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 */
FlcBool FlcCommSetProxyAutoDetect(FlcCommRef       comm,
                                  FlcBool          proxyAutoDetect,
                                  FlcErrorRef      error);
                          
/**
 *  Sets SSL communications information
 *
 *  @param  comm                            Communications object
 *  @param  sslCertificateFilePath          Fully qualified path to the CA (certificate authority) certificate file, which must be in PEM format
 *  @param  error                           Error object to be filled with error information in the case of failure
 * 
 */
FlcBool FlcCommSetSSLCertificatePath(FlcCommRef        comm,
                                     const FlcChar *   sslCertificateFilePath,
                                     FlcErrorRef       error);


/**
 *  Sends a binary message to the target server URL and optionally receives a response.
 *
 *  @param  comm            Communications object
 *  @param  sendData        Data to send
 *  @param  sendDataLength  Length of data to send
 *  @param  recvData        Received data (NULL if no response expected)
 *  @param  recvDataLength  Received data length (NULL if no response expected)
 *  @param  error           Error object to be filled with error information in the case of failure
 *  @return FLC_TRUE on success, FLC_FALSE on error (see error for additional error info)
 *
 *  @note If the recvData parameter is specified then the recvDataLength parameter must also be
 *        specified.
 *  @note On success the caller will be responsible for deallocating the received data storage
 *        using the FlcMemoryFree API.
 */
FlcBool FlcCommSendBinaryMessage(FlcCommRef    comm,
                                 const void *  sendData,
                                 FlcSize       sendDataLength,
                                 void **       recvData,
                                 FlcSize *     recvDataLength,
                                 FlcErrorRef   error);



/**
 *  Returns the HTTP status code for any communication done.
 *
 *  @param  comm          Communications object
 *  @return non-zero on success, zero on error
 */
FlcUInt32 FlcCommGetHttpResponseCode(FlcCommRef comm);



/** @} */ /* end of comm group */
/** @} */ /* end of addtogroup clientCommon */

#ifdef __cplusplus
}
#endif

#endif /* _INCLUDE_FLC_COMM_H_ */
