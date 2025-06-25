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
    Trials.c

    This example program allows you to:
    1. Process the trial file of your choice.
    2. Acquire licenses from the trial.
*****************************************************************************/

static const char * const fileName  = 
#if defined WIN32 || defined _WIN32_WCE
        ".\trial.bin";
#else
        "trial.bin";
#endif

/* identity warning message */
const char * const emptyIdentity = 
    "License-enabled code requires client identity data, " \
    "which you create with pubidutil and printbin -C. " \
    "See the User Guide for more information.";

/* feature names */
static const FlcChar * survey    = "survey";
static const char *   tsurvey   = "survey";
static const FlcChar * highres   = "highres";
static const char *   thighres  = "highres";
static const FlcChar * lowres    = "lowres";
static const char *   tlowres   = "lowres";
static const FlcChar * download  = "download";
static const char *   tdownload = "download";
static const FlcChar * upload    = "upload";
static const char *   tupload   = "upload";
static const FlcChar * updates   = "updates";
static const char *   tupdates  = "updates";
static const FlcChar * special   = "special";
static const char *   tspecial  = "special";

static const char *   inputFile = NULL;

/* license buffer */
FlcUInt8 * buffer      = NULL;
FlcSize    bufferSize  = 0;


/*-------------------------------------------------------------------------*/
/*   Utility Functions                                                     */
/*-------------------------------------------------------------------------*/

/**
 *    Validate arguments
 */
static FlcBool sValidateArgs(int argc, char * argv[]);

/**
 *    Example program usage
 */
static void sUsage(
    const char * applicationName,
    const char * fileName);

/**
 *    Cleanup processing
 */
static FlcBool sCleanup(
    FlcLicensingRef * licensing,
    FlcErrorRef *     error);

/**
 *    Obtain number of features in trusted storage
 */
static FlcBool sCheckNumberOfFeatures(
    FlcLicensingRef     licensing,
    FlcUInt32 *         size,
    FlcErrorRef         error);

/**
 *    Process trial license data
 */
static FlcBool sProcessTrial(
    FlcLicensingRef     licensing,
    FlcBool *           trialAccepted,
    FlcErrorRef         error);

/**
 *    Acquire a license for the specified feature
 */
static FlcBool sAcquireLicense(
    FlcLicensingRef         licensing,
    const FlcChar *         featureName,
    const char *           tfeatureName,
    FlcErrorRef             error);


/*=========================================================================*/
int main(int argc, char * argv[])
/*=========================================================================*/
{
    /* licensing environment object */
    FlcLicensingRef licensing   = NULL;

    /* error object */
    FlcErrorRef error            = NULL;

    /* other */
    char       msg[512]        = {0};
    FlcChar     tsPath[512]     = {0};
    FlcBool     loaded          = FLC_FALSE;
    FlcUInt32   featureCount    = 0;
    FlcBool     trialAccepted   = 0;
    FlcUInt32   expireSeconds   = 0;

    if (!sValidateArgs(argc, argv))
    {
      return -1;
    }

    if (!identity_data || sizeof(identity_data) == 0)
    {
        DisplayErrorString(emptyIdentity, NULL);
        return -1;
    }

    /*---------------------------------------------------------------------*/
    /* create error object                                                 */
    /*---------------------------------------------------------------------*/
    if (!FlcErrorCreate(&error))
    {
        DisplayErrorString("creating error object", NULL);
        return -1;
    }

    /*---------------------------------------------------------------------*/
    /* read trial license file data                                        */
    /*---------------------------------------------------------------------*/
    if (!ReadFileData(inputFile, &buffer, &bufferSize))
    {
        (void)sCleanup(&licensing, &error);
        return -1;
    }
    sprintf(msg, "read data from %s", inputFile);
    DisplayInfoString(msg);

    /*---------------------------------------------------------------------*/
    /* create licensing environment                                        */
    /*---------------------------------------------------------------------*/
    if (!FlcLicensingCreate(
            &licensing, identity_data, sizeof(identity_data),
            GetStoragePath(tsPath, sizeof(tsPath)) ? tsPath : NULL,
            "1234567890", error))
    {
        DisplayErrorString("creating licensing object", error);
        (void)sCleanup(&licensing, &error);
        return -1;
    }
    DisplayInfoString("licensing environment established");

    /*---------------------------------------------------------------------*/
    /* check number of features loaded from trials storage                 */
    /*---------------------------------------------------------------------*/  
    if (!sCheckNumberOfFeatures(licensing, &featureCount, error))
    {
        (void)sCleanup(&licensing, &error);
        return -1;
    }
    sprintf(msg, "number of features loaded from trials: %u", featureCount);
    DisplayInfoString(msg);

    /*---------------------------------------------------------------------*/
    /* determine if trial has been loaded                                  */
    /*---------------------------------------------------------------------*/  
    if (!FlcTrialDataIsLoaded(licensing, buffer, bufferSize, &loaded, 
                               &expireSeconds, error))
    {
        DisplayErrorString("checking if trial data is loaded", error);
        (void)sCleanup(&licensing, &error);
        return -1;
    }

    if (loaded)
    {
        if (expireSeconds)
        {
            sprintf(msg, "Trial has already been loaded and will expire in %u seconds.", expireSeconds);
            DisplayInfoString(msg);
        }
        else
        {
            DisplayInfoString("Trial has already been loaded and has expired.");
        }
    }

    /*---------------------------------------------------------------------*/
    /* process trial into trials license source                            */
    /*---------------------------------------------------------------------*/  
    if (!sProcessTrial(licensing, &trialAccepted, error))
    {
        (void)sCleanup(&licensing, &error);
        return -1;
    }

    if (trialAccepted)
    {
        DisplayInfoString("processed trial");

        /* check number of features in the trial storage now */
        if (!sCheckNumberOfFeatures(licensing, &featureCount, error))
        {
            (void)sCleanup(&licensing, &error);
            return -1;
        }

        sprintf(msg, "number of features loaded from trials: %u", featureCount);
        DisplayInfoString(msg);
    }

    /*---------------------------------------------------------------------*/
    /* acquire "survey" license                                            */
    /*---------------------------------------------------------------------*/  
    if (sAcquireLicense(licensing, survey, tsurvey, error))
    {
        sprintf(msg, "acquired %s license", tsurvey);
        DisplayInfoString(msg);
    }

    /*---------------------------------------------------------------------*/
    /* acquire "highres" license                                           */
    /*---------------------------------------------------------------------*/  
    if (sAcquireLicense(licensing, highres, thighres, error))
    {
        sprintf(msg, "acquired %s license", thighres);
        DisplayInfoString(msg);
    }

    /*---------------------------------------------------------------------*/
    /* acquire "lowres" license                                            */
    /*---------------------------------------------------------------------*/  
    if (sAcquireLicense(licensing, lowres, tlowres, error))
    {
        sprintf(msg, "acquired %s license", tlowres);
        DisplayInfoString(msg);
    }

    /*---------------------------------------------------------------------*/
    /* acquire "download" license                                          */
    /*---------------------------------------------------------------------*/  
    if (sAcquireLicense(licensing, download, tdownload, error))
    {
        sprintf(msg, "acquired %s license", tdownload);
        DisplayInfoString(msg);
    }

    /*---------------------------------------------------------------------*/
    /* acquire "upload" license                                            */
    /*---------------------------------------------------------------------*/  
    if (sAcquireLicense(licensing, upload, tupload, error))
    {
        sprintf(msg, "acquired %s license", tupload);
        DisplayInfoString(msg);
    }

    /*---------------------------------------------------------------------*/
    /* acquire "updates" license                                           */
    /*---------------------------------------------------------------------*/  
    if (sAcquireLicense(licensing, updates, tupdates, error))
    {
        sprintf(msg, "acquired %s license", tupdates);
        DisplayInfoString(msg);
    }

    /*---------------------------------------------------------------------*/
    /* acquire "special" license                                           */
    /*---------------------------------------------------------------------*/  
    if (sAcquireLicense(licensing, special, tspecial, error))
    {
        sprintf(msg, "acquired %s license", tspecial);
        DisplayInfoString(msg);
    }

    /*---------------------------------------------------------------------*/
    /* cleanup                                                             */
    /*---------------------------------------------------------------------*/
    return sCleanup(&licensing, &error) ? 0 : -1;
}

/*=========================================================================*/
static FlcBool sValidateArgs(int argc, char * argv[])
/*=========================================================================*/
{
    if (argc != 1 && argc != 2)
    {
        sUsage(argv[0], fileName);
        return FLC_FALSE;
    }

    if (argc == 1)
    {
        inputFile = fileName;
        sprintf(msg, "using default trial file %s", inputFile);
        DisplayInfoString(msg);
    }

    if (argc  == 2)
    {
        if (strcmp("-help", argv[1]) == 0  ||
            strcmp("-h",    argv[1]) == 0)
        {
            sUsage(argv[0], fileName);
            return FLC_FALSE;
        }
        else
        {
            inputFile = argv[1];
        }
    }

    return FLC_TRUE;
}

/*=========================================================================*/
static
void
sUsage(
    const char *       applicationName,
    const char *       fileName)
/*=========================================================================*/
{
    char msg[512] = {0};

    snprintf(msg, 511,
        "\n%s [binary_trial_file]"
        "\nActivates a limited-duration trial based on a binary trial file."
        "\n\nIf unset, default binary_trial_file is %s.",
        applicationName, fileName);

    DisplayMessage(msg, "USAGE");
}

/*=========================================================================*/
static
FlcBool
sCleanup(
    FlcLicensingRef * licensing,
    FlcErrorRef *   error)
/*=========================================================================*/
{
    FlcBool cleanupOK = FLC_TRUE;

    /*---------------------------------------------------------------------*/
    /* delete trial data buffer                                            */
    /*---------------------------------------------------------------------*/
    if (buffer)
    {
        free(buffer);
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
static
FlcBool
sCheckNumberOfFeatures(
    FlcLicensingRef     licensing,
    FlcUInt32 *         size,
    FlcErrorRef         error)
/*==========================================================================*/
{
    FlcFeatureCollectionRef features = NULL;

    if (!licensing  ||  !size  ||  !error)
    {
        DisplayErrorString("checking number of features", NULL);
        return FLC_FALSE;
    }
    *size = 0;

    /*---------------------------------------------------------------------*/
    /* create trials storage feature collection                            */
    /*---------------------------------------------------------------------*/
    if (!FlcGetTrialFeatureCollection(licensing,
                                      &features,
                                      FLC_FALSE,
                                      error))
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

/*=========================================================================*/
static
FlcBool
sProcessTrial(
    FlcLicensingRef     licensing,
    FlcBool *           trialAccepted,
    FlcErrorRef         error)
/*=========================================================================*/
{
    if (!licensing  ||  !buffer  ||  !trialAccepted  ||  !error)
    {
        DisplayErrorString("processing trial", NULL);
        return FLC_FALSE;
    }
    *trialAccepted = FLC_FALSE;
     
    /*---------------------------------------------------------------------*/
    /* process trial into trial license source                             */
    /*---------------------------------------------------------------------*/
    if (!FlcProcessTrialLicenseData(licensing, buffer, bufferSize, error))
    {
        switch (FlcErrorGetCode(error))
        {
            case FLCERR_TRIAL_ALREADY_LOADED:
            case FLCERR_TRIAL_EXPIRED:
                DisplayErrorString("processing trial", error);
                return FLC_TRUE;

            default:
                DisplayErrorString("processing trial", error);
                return FLC_FALSE;
        }
    }

    *trialAccepted = FLC_TRUE;
    return FLC_TRUE;
}


/*=========================================================================*/
static
FlcBool
sAcquireLicense(
    FlcLicensingRef         licensing,
    const FlcChar *         featureName,
    const char *           tfeatureName,
    FlcErrorRef             error)
/*=========================================================================*/
{
    const FlcChar * const   featureVersion  = "1.0";
    FlcLicenseRef           license         = NULL;
    char                   msg[512]        = {0};

    if (!licensing  ||  !featureName  ||  !tfeatureName  ||  !error)
    {
        DisplayErrorString("acquiring license", NULL);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* acquire license                                                     */
    /*---------------------------------------------------------------------*/
    if (!FlcAcquireLicense(licensing, &license, featureName, featureVersion, error))
    {
        sprintf(msg, "acquiring %s license", tfeatureName);
        DisplayErrorString(msg, error);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* return license                                                      */
    /*---------------------------------------------------------------------*/
    if (!FlcReturnLicense(licensing, &license, error))
    {
        sprintf(msg, "returning %s license", tfeatureName);
        DisplayErrorString(msg, error);
        return FLC_FALSE;
    }

    return FLC_TRUE;
}

















