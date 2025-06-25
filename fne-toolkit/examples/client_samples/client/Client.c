/****************************************************************************
  Copyright (c) 2008-2020 Flexera Software LLC.
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
#include "FlcLicensing.h"
#include "FlcLicenseManager.h"

#include "util/ErrDisplay.h"
#include "util/FileIO.h"
#include "identity/IdentityClient.h"

/* Use this to direct output to Windows MessageBox
#define UI_OUTPUT
*/

/****************************************************************************
    Client.c

    This example program enables you to:
    1. Process the license file of your choice.
    2. Acquire licenses from a combination of buffer, trusted storage, and
       trial license sources.
*****************************************************************************/

/* identity warning message */
const char * const emptyIdentity = 
    "License-enabled code requires client identity data, " \
    "which you create with pubidutil and printbin -C. " \
    "See the User Guide for more information.";

/*-------------------------------------------------------------------------*/
/*   Utility Functions                                                     */
/*-------------------------------------------------------------------------*/

/**
 *    Validate arguments
 */
static
FlcBool
sValidateArgs(
    int             argc, 
    char *         argv[],
    const char **  fileName);

/**
 *    Obtain number of features in a license source
 */
static FlcBool sCheckNumberOfFeatures(
    FlcLicensingRef       licensing,
    FlcLicenseSourceType  type,
    const FlcChar *       name,
    FlcUInt32 *           size,
    FlcErrorRef           error);

/**
 *    Acquire/Return licenses
 */
static FlcBool sAcquireLicense(
    FlcLicensingRef         licensing,
    const FlcChar * const   featureName,
    const char *   const   tfeatureName,
    FlcInt32                count,
    FlcErrorRef             error);

/**
 *    Display license details
 */
static FlcBool sGetLicenseDetails(
    FlcLicenseRef license,
    FlcErrorRef   error);

/**
 *    Cleanup processing
 */
static FlcBool sCleanup(
    FlcLicensingRef * licensing,
    FlcChar **        bufferFile,
    FlcErrorRef *     error);

/**
 *    Example program usage
 */
static void sUsage(
    const char *       applicationName);


/*==========================================================================*/
int main( int argc, char * argv[] )
/*==========================================================================*/
{
    /* licensing environment object */
    FlcLicensingRef licensing = NULL;

    /* error object */
    FlcErrorRef error = NULL;

    /* fileName */
    const char * inputFile = NULL;

    /* feature names */
    const FlcChar * const survey      = "survey";
    const char *   const tsurvey     = "survey";

    const FlcChar * const highres     = "highres";
    const char *   const thighres    = "highres";

    const FlcChar * const lowres      = "lowres";
    const char *   const tlowres     = "lowres";

    const FlcChar * const download    = "download";
    const char *   const tdownload   = "download";

    const FlcChar * const upload      = "upload";
    const char *   const tupload     = "upload";

    const FlcChar * const updates     = "updates";
    const char *   const tupdates    = "updates";

    const FlcChar * const special     = "special";
    const char *   const tspecial    = "special";

    const FlcChar * const sdchannel   = "sdchannel";
    const char *   const tsdchannel  = "sdchannel";

    const FlcChar * const hdchannel   = "hdchannel";
    const char *   const thdchannel  = "hdchannel";

    /* other */
    char       msg[512]      = {0};
    FlcChar     tsPath[512]   = {0};
    FlcChar *   bufferFile    = NULL;
    FlcUInt32   featureCount  = 0;

    if (!sValidateArgs(argc, argv, &inputFile))
    {
        return -1;
    }

    if (!identity_data || sizeof(identity_data) == 0)
    {
        DisplayErrorString(emptyIdentity, NULL);
        return -1;
    }

    /*----------------------------------------------------------------------*/
    /* create error object                                                  */
    /*----------------------------------------------------------------------*/
    if (!FlcErrorCreate(&error))
    {
        DisplayErrorString("creating error object", NULL);
        return -1;
    }

    /*----------------------------------------------------------------------*/
    /* create licensing environment                                         */
    /*----------------------------------------------------------------------*/
    if (!FlcLicensingCreate(
            &licensing, identity_data, sizeof(identity_data),
            GetStoragePath(tsPath, sizeof(tsPath)) ? tsPath : NULL,
            "1234567890", error))
    {
        DisplayErrorString("creating licensing object", error);
        (void)sCleanup(&licensing, NULL, &error);
        return -1;
    }
    DisplayInfoString("licensing environment established");

    /*----------------------------------------------------------------------*/
    /* note: the order in which license sources are added determines the    */
    /* order in which features are considered for license acquisition       */
    /*----------------------------------------------------------------------*/
    /*----------------------------------------------------------------------*/
    /* if specified, create buffer license source from file                 */
    /*----------------------------------------------------------------------*/
    if (inputFile)
    {
        bufferFile = malloc(512);
        if (!bufferFile) 
        {
            DisplayErrorString("allocating storage for file name", error);
            (void)sCleanup(&licensing, &bufferFile, &error);
            return -1;
        }
        TCharToFlcChar(inputFile, bufferFile, (FlcSize)(512));
        if (!FlcAddBufferLicenseSourceFromFile(licensing, bufferFile, bufferFile, error))
        {
            DisplayErrorString("adding buffer license source", error);
            (void)sCleanup(&licensing, &bufferFile, &error);
            return -1;
        }
    }

    /*----------------------------------------------------------------------*/
    /* add trusted storage license source                                   */
    /*----------------------------------------------------------------------*/
    if (!FlcAddTrustedStorageLicenseSource(licensing, error))
    {
        DisplayErrorString("adding trusted storage license source", error);
        (void)sCleanup(&licensing, &bufferFile, &error);
        return -1;
    }

    /*----------------------------------------------------------------------*/
    /* add trial icense source                                              */
    /*----------------------------------------------------------------------*/
    if (!FlcAddTrialLicenseSource(licensing, error))
    {
        DisplayErrorString("adding trial license source", error);
        (void)sCleanup(&licensing, &bufferFile, &error);
        return -1;
    }

    /*----------------------------------------------------------------------*/
    /* if specified, check number of features loaded from license file      */
    /*----------------------------------------------------------------------*/
    if (bufferFile)
    {
        if (!sCheckNumberOfFeatures(licensing, FLC_LICENSE_SOURCE_BUFFER, bufferFile,
                                    &featureCount, error))
        {
            (void)sCleanup(&licensing, &bufferFile, &error);
            return -1;
        }
        sprintf(msg, "number of features loaded from license file: %u", featureCount);
        DisplayInfoString(msg);
    }
    else
    {
        DisplayInfoString("No license file specified.");
    }

    /*----------------------------------------------------------------------*/
    /* check number of features loaded from trusted storage                 */
    /*----------------------------------------------------------------------*/
    if ( !sCheckNumberOfFeatures(licensing, FLC_LICENSE_SOURCE_TRUSTED_STORAGE, 
                                 NULL, &featureCount, error))
    {
        (void)sCleanup(&licensing, &bufferFile, &error);
        return -1;
    }
    sprintf(msg, "number of features loaded from trusted storage: %u", featureCount);
    DisplayInfoString(msg);

    /*----------------------------------------------------------------------*/
    /* check number of features loaded from trials                          */
    /*----------------------------------------------------------------------*/
    if ( !sCheckNumberOfFeatures(licensing, FLC_LICENSE_SOURCE_TRIAL, NULL,
                                 &featureCount, error))
    {
        (void)sCleanup(&licensing, &bufferFile, &error);
        return -1;
    }
    sprintf(msg, "number of features loaded from trials: %u", featureCount);
    DisplayInfoString(msg);

    /*----------------------------------------------------------------------*/
    /* acquire "survey" license                                             */
    /*----------------------------------------------------------------------*/
    if (sAcquireLicense(licensing, survey, tsurvey, 1, error))
    {
        sprintf(msg, "acquired %s license", tsurvey);
        DisplayInfoString(msg);
    }

    /*----------------------------------------------------------------------*/
    /* acquire "highres" license                                            */
    /*----------------------------------------------------------------------*/
    if (sAcquireLicense(licensing, highres, thighres, 1, error))
    {
        sprintf(msg, "acquired %s license", thighres);
        DisplayInfoString(msg);
    }

    /*----------------------------------------------------------------------*/
    /* acquire "lowres" license                                             */
    /*----------------------------------------------------------------------*/
    if (sAcquireLicense(licensing, lowres, tlowres, 1, error))
    {
        sprintf(msg, "acquired %s license", tlowres);
        DisplayInfoString(msg);
    }

    /*----------------------------------------------------------------------*/
    /* acquire "download" license                                           */
    /*----------------------------------------------------------------------*/
    if (sAcquireLicense(licensing, download, tdownload, 1, error))
    {
        sprintf(msg, "acquired %s license", tdownload);
        DisplayInfoString(msg);
    }

    /*----------------------------------------------------------------------*/
    /* acquire "upload" license                                             */
    /*----------------------------------------------------------------------*/
    if (sAcquireLicense(licensing, upload, tupload, 1, error))
    {
        sprintf(msg, "acquired %s license", tupload);
        DisplayInfoString(msg);
    }

    /*----------------------------------------------------------------------*/
    /* acquire "updates" license                                            */
    /*----------------------------------------------------------------------*/
    if (sAcquireLicense(licensing, updates, tupdates, 1, error))
    {
        sprintf(msg, "acquired %s license", tupdates);
        DisplayInfoString(msg);
    }

    /*----------------------------------------------------------------------*/
    /* acquire "special" license                                            */
    /*----------------------------------------------------------------------*/
    if (sAcquireLicense(licensing, special, tspecial, 1, error))
    {
        sprintf(msg, "acquired %s license", tspecial);
        DisplayInfoString(msg);
    }

    /*----------------------------------------------------------------------*/
    /* acquire 100 "sdchannel" licenses                                     */
    /*----------------------------------------------------------------------*/
    if (sAcquireLicense(licensing, sdchannel, tsdchannel, 100, error))
    {
        sprintf(msg, "acquired %s license", tsdchannel);
        DisplayInfoString(msg);
    }

    /*----------------------------------------------------------------------*/
    /* acquire 10 "hdchannel" licenses                                      */
    /*----------------------------------------------------------------------*/
    if (sAcquireLicense(licensing, hdchannel, thdchannel, 10, error))
    {
        sprintf(msg, "acquired %s license", thdchannel);
        DisplayInfoString(msg);
    }

    /*---------------------------------------------------------------------*/
    /* cleanup                                                             */
    /*---------------------------------------------------------------------*/
    return sCleanup(&licensing, &bufferFile, &error) ? 0 : -1;
}

/*==========================================================================*/
static FlcBool sValidateArgs(int argc, char * argv[], const char ** fileName)
/*==========================================================================*/
{
    if (argc != 1 && argc != 2)
    {
        sUsage(argv[0]);
        return FLC_FALSE;
    }

    if (argc  == 2)
    {
        if (strcmp("-help", argv[1]) == 0  ||
            strcmp("-h",    argv[1]) == 0)
        {
            sUsage(argv[0]);
            return FLC_FALSE;
        }
        else
        {
            *fileName = argv[1];
        }
    }
    return FLC_TRUE;
}

/*=========================================================================*/
static FlcBool sCheckNumberOfFeatures(
    FlcLicensingRef       licensing,
    FlcLicenseSourceType  type,
    const FlcChar *       name,
    FlcUInt32 *           size,
    FlcErrorRef           error)
/*=========================================================================*/
{
    FlcBool bFeaturesOK = FLC_FALSE;
    FlcFeatureCollectionRef features = NULL;

    if (!licensing  ||  !size  ||  !error ||
        (type == FLC_LICENSE_SOURCE_BUFFER && !name))
    {
        DisplayErrorString("checking number of features", NULL);
        return FLC_FALSE;
    }
    *size = 0;

    /*---------------------------------------------------------------------*/
    /* create license source feature collection                            */
    /*---------------------------------------------------------------------*/
    switch (type)
    {
        case FLC_LICENSE_SOURCE_TRUSTED_STORAGE:
            bFeaturesOK = FlcGetTrustedStorageFeatureCollection(
                              licensing, &features, FLC_FALSE, error);
            break;
        case FLC_LICENSE_SOURCE_TRIAL:
            bFeaturesOK = FlcGetTrialFeatureCollection(
                              licensing, &features, FLC_FALSE, error);
            break;
        case FLC_LICENSE_SOURCE_BUFFER:
            bFeaturesOK = FlcGetBufferLicenseSourceFeatureCollection(
                              licensing, &features, name, error);
            break;
        default:
            DisplayErrorString("checking license source type", NULL);
            break;
    }
    if (!bFeaturesOK)
    {
        DisplayErrorString("creating feature collection", error);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* get feature collection size                                         */
    /*---------------------------------------------------------------------*/
    if (!FlcFeatureCollectionSize(features, size, error))
    {
        FlcFeatureCollectionDelete(&features, NULL);
        DisplayErrorString("getting feature collection size", error);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* delete feature collection                                           */
    /*---------------------------------------------------------------------*/
    if (!FlcFeatureCollectionDelete(&features, error))
    {
        DisplayErrorString("deleting feature collection", error);
        return FLC_FALSE;
    }

    return FLC_TRUE;
}

/*==========================================================================*/
static FlcBool sAcquireLicense(
    FlcLicensingRef         licensing,
    const FlcChar * const   featureName,
    const char *   const   tfeatureName,
    FlcInt32                count,
    FlcErrorRef             error)
/*==========================================================================*/
{
    const FlcChar * const   featureVersion  = "1.0";
    FlcLicenseRef           license         = NULL;
    char                   msg[512]        = {0};

    if (!licensing || !featureName || !tfeatureName || !count || !error )
    {
        DisplayErrorString("acquiring license", NULL);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* acquire license                                                     */
    /*---------------------------------------------------------------------*/
    if (!FlcAcquireLicenses(licensing, &license, featureName, featureVersion,
                            count, error))
    {
        sprintf(msg, "acquiring %s license", tfeatureName);
        DisplayErrorString(msg, error);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* get license details                                                 */
    /*---------------------------------------------------------------------*/
    if (!sGetLicenseDetails(license, error))
    {
        sprintf(msg, "getting details for %s license", tfeatureName);
        DisplayErrorString(msg, NULL);
        (void)FlcReturnLicenses(licensing, &license, NULL);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* return license                                                      */
    /*---------------------------------------------------------------------*/
    if (!FlcReturnLicenses(licensing, &license, error) )
    {
        sprintf(msg, "returning %s license", tfeatureName);
        DisplayErrorString(msg, error);
        return FLC_FALSE;
    }

    return FLC_TRUE;
}

/*==========================================================================*/
static FlcBool sGetLicenseDetails(
    FlcLicenseRef    license,
    FlcErrorRef        error)
/*==========================================================================*/
{
    const FlcChar *   name        = NULL;
    const FlcChar *   version     = NULL;
    FlcInt32          count       = 0;
    const struct tm * expiration  = NULL;
    FlcBool           isPerpetual = FLC_FALSE;
    FlcChar *         msg         = NULL;
    char *              tmsg        = NULL;
    FlcSize           length      = 0;

    if (!license)
    {
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* gather license information                                          */
    /*---------------------------------------------------------------------*/
    if (!FlcLicenseGetName(license, &name, error)             ||
        !FlcLicenseGetVersion(license, &version, error)       ||
        !FlcLicenseGetExpiration(license, &expiration, error) ||
        !FlcLicenseGetCount(license, &count, error))
    {
        return FLC_FALSE;
    }
    length = (FlcSize)(strlen(name) + strlen(version));
    /* add enough space for format string */
    length += 100;    /* for name=name, version=version, expiration=mm/dd/yyyy, count=NNNNNNNNNN */

    /*---------------------------------------------------------------------*/
    /* obtain display storage                                              */
    /*---------------------------------------------------------------------*/
    tmsg = (char *)malloc(sizeof(char) * length);
    if (!tmsg)
    {
        return FLC_FALSE;
    }
    msg = (FlcChar *)malloc(sizeof(FlcChar) * length);
    if (!msg)
    {
        free((void *)tmsg);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* format display line                                                 */
    /*---------------------------------------------------------------------*/
    if (FlcDateIsPerpetual(expiration, &isPerpetual, error) && isPerpetual)
    {
        sprintf(msg, "name=%s, version=%s, expiration=permanent, count=%d", name, version, count);
    }
    else
    {
        sprintf(msg, "name=%s, version=%s, expiration=%02d/%02d/%04d, count=%d", name, version,
                expiration->tm_mon + 1, expiration->tm_mday, expiration->tm_year + 1900, count);
    }

    /*---------------------------------------------------------------------*/
    /* convert to char and display                                        */
    /*---------------------------------------------------------------------*/
    FlcCharToTChar(msg, tmsg, length);
    DisplayInfoString(tmsg);

    /*---------------------------------------------------------------------*/
    /* cleanup display storage                                             */
    /*---------------------------------------------------------------------*/
    free((void *)tmsg);
    free(msg);

    return FLC_TRUE;
}

/*==========================================================================*/
static FlcBool sCleanup(
    FlcLicensingRef *  licensing, 
    FlcChar **         bufferFile,
    FlcErrorRef *      error)
/*==========================================================================*/
{
    FlcBool cleanupOK = FLC_TRUE;

    /*---------------------------------------------------------------------*/
    /* delete buffer license file name storage                             */
    /*---------------------------------------------------------------------*/
    if (bufferFile && *bufferFile)
    {
       free(*bufferFile);
       *bufferFile = NULL;
    }

    /*---------------------------------------------------------------------*/
    /* delete licensing environment                                        */
    /*---------------------------------------------------------------------*/
    if (licensing && *licensing && !FlcLicensingDelete(licensing, error ? *error : NULL))
    {
        DisplayErrorString("deleting licensing object", error ? *error : NULL);
        cleanupOK = FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* delete error object                                                 */
    /*---------------------------------------------------------------------*/
    if (error && *error && !FlcErrorDelete(error))
    {
        DisplayErrorString("deleting error object", NULL);
        cleanupOK = FLC_FALSE;
    }

    if (cleanupOK)
    {
       DisplayInfoString("deleted licensing environment");
    }

    return cleanupOK;
}

/*==========================================================================*/
static void sUsage(const char * applicationName)
/*==========================================================================*/
{
    char msg[512] = {0};

    snprintf(msg, 511,
        "\n%s [binary_license_file]"
        "\nAttempts to acquire various features from binary license file, "
        "\ntrusted storage, and trial license sources. ",
        applicationName);

    DisplayMessage(msg, "USAGE");
}
