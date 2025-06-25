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
#include "FlcLicensing.h"
#include "FlcLicenseManager.h"

#include "util/ErrDisplay.h"
#include "identity/IdentityClient.h"

/* Use this to direct output to Windows MessageBox
#define UI_OUTPUT
*/

/****************************************************************************
  Lmflex.c

  This example program illustrates the general sequence of functions
    required to acquire "f1" and "f2" features from a legacy
    certificate license source.  For compilation and usage instructions,
    see the FlexNet Embedded SDK User Guide.

*****************************************************************************/

#define DEFAULT_FILE "legacy.lic"


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
 *    Cleanup processing
 */
static
FlcBool
sCleanup(
    FlcLicensingRef * licensing,
    FlcErrorRef *     error);

/**
 *    Example program usage
 */
static
void
sUsage(
    const char *    applicationName,
    const char *    fileName);

/*==========================================================================*/
int main(int argc, char * argv[])
/*==========================================================================*/
{
    /* licensing environment object */
    FlcLicensingRef licensing = NULL;

    /* error object */
    FlcErrorRef error = NULL;

    /* license object */
    FlcLicenseRef license = NULL;

    /* fileName */
    const char * inputFile = DEFAULT_FILE;

    /* feature names */
    const FlcChar * const f1 = "f1";
    const FlcChar * const f2 = "f2";

    /* other */
    FlcChar * bufferFile = NULL;
    char     msg[512] = {0};

    if (!sValidateArgs(argc, argv, &inputFile))
    {
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
    /*                                                                      */
    /* The hostid set for the main licensing object is used for FlexNet     */
    /* Embedded license acquisition. For legacy certificates, features can  */
    /* use any supported hostid valid on the target system.                 */
    /*----------------------------------------------------------------------*/
    if (!FlcLicensingCreate(
            &licensing, identity_data, sizeof(identity_data),
            NULL, "1234567890", error))
    {
        DisplayErrorString("creating licensing object", error);
        (void)sCleanup(&licensing, &error);
        return -1;
    }
    DisplayInfoString("licensing environment established");

    /*----------------------------------------------------------------------*/
    /* create certificate license source from file                          */
    /*----------------------------------------------------------------------*/
    bufferFile = malloc(strlen(inputFile) + 64);
    if (!bufferFile) 
    {
        DisplayErrorString("allocating storage for file name", error);
        (void)sCleanup(&licensing, &error);
        return -1;
    }
    TCharToFlcChar(inputFile, bufferFile, (FlcSize)(strlen(inputFile) + 64));
    if (!FlcAddCertificateLicenseSourceFromFile(licensing, bufferFile, NULL, error))
    {
        DisplayErrorString("adding certificate license source", error);
        (void)sCleanup(&licensing, &error);
        free(bufferFile);
        return -1;
    }
    free(bufferFile);

    /*----------------------------------------------------------------------*/
    /* acquire "f1" license                                                 */
    /*----------------------------------------------------------------------*/
    if (FlcAcquireLicense(licensing, &license, f1, "1.0", error))
    {
        DisplayInfoString("acquired f1 license");
        /*------------------------------------------------------------------*/
        /* return "f1" license                                              */
        /*------------------------------------------------------------------*/
        if (!FlcReturnLicense(licensing, &license, error))
        {
            DisplayErrorString("returning f1", error);
            (void)sCleanup(&licensing, &error);
            return -1;
        }
    }
    else
    {
        DisplayErrorString("acquiring f1", error);
    }

    /*----------------------------------------------------------------------*/
    /* acquire "f2" license                                                 */
    /*----------------------------------------------------------------------*/
    if (FlcAcquireLicense(licensing, &license, f2, "1.0", error))
    {
        DisplayInfoString("acquired f2 license");
        /*------------------------------------------------------------------*/
        /* return "f2" license                                              */
        /*------------------------------------------------------------------*/
        if (!FlcReturnLicense(licensing, &license, error))
        {
            DisplayErrorString("returning f2", error);
            (void)sCleanup(&licensing, &error);
            return -1;
        }
    }
    else
    {
        DisplayErrorString("acquiring f2", error);
    }

    /*----------------------------------------------------------------------*/
    /* cleanup                                                              */
    /*----------------------------------------------------------------------*/
    return sCleanup(&licensing, &error) ? 0 : -1;

}

/*==========================================================================*/
static FlcBool sValidateArgs(int argc, char * argv[], const char ** fileName)
/*==========================================================================*/
{
    if (argc != 1 && argc != 2)
    {
        sUsage(argv[0], *fileName);
        return FLC_FALSE;
    }

    if (argc == 2)
    {
        if (strcmp("-help", argv[1]) == 0  ||
            strcmp("-h",    argv[1]) == 0)
        {
            sUsage(argv[0], *fileName);
            return FLC_FALSE;
        }
        else
        {
            *fileName = argv[1];
        }
    }
    return FLC_TRUE;
}

/*==========================================================================*/
static void sUsage(const char * applicationName, const char* fileName)
/*==========================================================================*/
{
    char msg[1024] = {0};

    snprintf(msg, 1023,
      "\n%s [certificate_file]"
      "\nAttempts to acquire 'f1' and 'f2' features from a"
      "\nlegacy certificate license file."
      "\n\nIf unset, default legacy certificate_file is %s.",
      applicationName, DEFAULT_FILE);

    DisplayMessage(msg, "USAGE");
}

/*==========================================================================*/
static FlcBool sCleanup(FlcLicensingRef * licensing, FlcErrorRef * error)
/*==========================================================================*/
{
    FlcBool cleanupOK = FLC_TRUE;

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
