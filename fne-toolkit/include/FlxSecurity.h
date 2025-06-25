/****************************************************************************
 Copyright (c) 2009-2020 Flexera Software LLC.
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

#if !defined(FLX_SECURITY_H_INCLUDED)
#define FLX_SECURITY_H_INCLUDED

#ifdef _MSC_VER
#pragma warning( push, 3 )
#include <windows.h>
#pragma warning( pop )
#endif

#if defined(__cplusplus)
extern "C" {
#endif

/** @file
 *  Provides the interface to available security features.
 *
 *  The functions provided enable the caller to validate the integrity
 *  of the processed executable linked with the core library.
 */

/**
 * @defgroup security Security
 * @{
 */

/**
 *  Error codes returned by the security functions.
 */
typedef enum FlxSecureError
{
    FLXSEC_SUCCESS = 0,             /** The file was loaded and passed integrity validation */
    FLXSEC_INTEGRITY_FAILURE = -1,  /** The file integrity does not match the value when the file was processed */
    FLXSEC_FILE_NOT_PROCESSED = -2, /** The file has not been processed by the integrity tool */
    FLXSEC_FILE_OPEN_FAILED = -3,   /** Failed to open this file */
    FLXSEC_VERSION_MISMATCH = -4    /** The tool used to process this file does not match the version of the library linked */
} FlxSecureError;

/**
 *  Checks the integrity of this executable
 *
 *  @param  filename    The file path and name of this executable.
 *  @return see FlxSecureError for specific failure codes.
 *
 *  @note In order for this validation check to succeed the executable
 *   which is making this call must have first been processed by
 *   the securityprocessor utility.
 */
int FlxSecureCheckIntegrity( char const * filename );


/**
 *  Wide character version of FlxSecureCheckIntegrity
 *
 *  See description of FlxSecureCheckIntegrity for more details.
 */
#ifdef _MSC_VER
int FlxSecureCheckIntegrityWide( wchar_t const * filename );
#endif

/** @} */ /* end of security group */

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* !defined(FLX_SECURITY_H_INCLUDED) */
