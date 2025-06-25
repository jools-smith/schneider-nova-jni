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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#if defined(_MSC_VER) && _MSC_VER >= 1900
#pragma comment(lib, "legacy_stdio_definitions.lib")
#endif

#include "FlcError.h"
#include "FlcLicensing.h"
#include "FlcLicenseManager.h"

#include "identity/IdentityClient.h"


/* Use this to direct output to Windows MessageBox
#define UI_OUTPUT
*/

#define DEFAULT_FILE "license.bin"

/* identity warning message */
const char * const emptyIdentity = 
    "License-enabled code requires client identity data, " \
    "which you create with pubidutil and printbin -C. " \
    "See the User Guide for more information.";

/* forward function declarations */
static
FlcBool
sValidateArgs(
    int        argc, 
    char *    argv[],
    char **   fileName);

static
void
sUsage(
    const char *    applicationName,
    const char *    fileName);

static
FlcBool
sCleanup(
    FlcLicensingRef * licensing,
    FlcErrorRef *     error);

static
FlcBool
sTCharToFlcChar(
    const char * src,
    FlcChar *     dest,
    FlcSize       destSize);

static
void
sDisplayErrorString(
    const char *  msg,
    FlcErrorRef    error);

static
void
sDisplayInfoString(
    const char *  msg);

static
void
sDisplayMessage(
    const char *  msg,
    const char *  caption);

static
void
FlcCharToTChar(
     const FlcChar * data,
     char *         result,
     size_t          size);

/****************************************************************************
    BasicClient.c

    This example program illustrates the general sequence of functions
    required to acquire "survey" and "highres" features from a buffer
    license source.  For compilation and usage instructions, see the
    FlexNet Embedded Getting Started Guide.

    Note that this example is intended ONLY as example code to verify that
    the basic system will compile and run in a particular environment.  For
    more comprehensive examples that illustrate additional license sources,
    cleanup during error conditions, etc., see the "Client" and other samples
    provided in the "examples" directory of the  FlexNet Embedded toolkit.

*****************************************************************************/


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
    char * inputFile = DEFAULT_FILE;

    /* feature names */
    const FlcChar * const survey = "survey";
    const FlcChar * const highres = "highres";

    /* other */
    FlcChar bufferFile[256] = {0};

    if (!sValidateArgs(argc, argv, &inputFile))
    {
        return -1;
    }

    if (!identity_data || sizeof(identity_data) == 0)
    {
        sDisplayErrorString(emptyIdentity, NULL);
        return -1;
    }

    /*----------------------------------------------------------------------*/
    /* create error object                                                  */
    /*----------------------------------------------------------------------*/
    if (!FlcErrorCreate(&error))
    {
        sDisplayErrorString("creating error object", NULL);
        return -1;
    }

    /*----------------------------------------------------------------------*/
    /* create licensing environment                                         */
    /*----------------------------------------------------------------------*/
    if (!FlcLicensingCreate(
            &licensing, identity_data, sizeof(identity_data),
            NULL, "1234567890", error))
    {
        sDisplayErrorString("creating licensing object", error);
        (void)sCleanup(0, &error);
        return -1;
    }
    sDisplayInfoString("licensing environment established");

    /*----------------------------------------------------------------------*/
    /* create buffer license source from file                               */
    /*----------------------------------------------------------------------*/
    sTCharToFlcChar(inputFile, bufferFile, sizeof(bufferFile));
    if (!FlcAddBufferLicenseSourceFromFile(licensing, bufferFile, NULL, error))
    {
        sDisplayErrorString("adding buffer license source", error);
        (void)sCleanup(&licensing, &error);
        return -1;
    }

    /*----------------------------------------------------------------------*/
    /* acquire "survey" license                                             */
    /*----------------------------------------------------------------------*/
    if (!FlcAcquireLicense(licensing, &license, survey, "1.0", error))
    {
        sDisplayErrorString("acquiring survey", error);
        (void)sCleanup(&licensing, &error);
        return -1;
    }
    sDisplayInfoString("acquired survey license");

    /*----------------------------------------------------------------------*/
    /* return "survey" license                                              */
    /*----------------------------------------------------------------------*/
    if (!FlcReturnLicense(licensing, &license, error))
    {
        sDisplayErrorString("returning survey", error);
        (void)sCleanup(&licensing, &error);
        return -1;
    }

    /*----------------------------------------------------------------------*/
    /* acquire "highres" license                                            */
    /*----------------------------------------------------------------------*/
    if (!FlcAcquireLicense(licensing, &license, highres, "1.0", error))
    {
        sDisplayErrorString("acquiring highres", error);
        (void)sCleanup(&licensing, &error);
        return -1;
    }
    sDisplayInfoString("acquired highres license");

    /*----------------------------------------------------------------------*/
    /* return "highres" license                                             */
    /*----------------------------------------------------------------------*/
    if (!FlcReturnLicense(licensing, &license, error))
    {
        sDisplayErrorString("returning highres", error);
        (void)sCleanup(&licensing, &error);
        return -1;
    }

    /*----------------------------------------------------------------------*/
    /* cleanup                                                              */
    /*----------------------------------------------------------------------*/
    return sCleanup(&licensing, &error) ? 0 : -1;

}

/*==========================================================================*/
static FlcBool sValidateArgs(int argc, char * argv[], char ** fileName)
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
    char msg[512] = {0};

    snprintf(msg, 511,
        "\n%s [binary_license_file]"
        "\nAttempts to acquire 'survey' and 'highres' features from a signed "
        "\nbinary license file."
        "\n\nIf unset, default binary_license_file is %s.",
        applicationName, fileName);

    sDisplayMessage(msg, "USAGE");
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
        sDisplayErrorString("deleting licensing object", error ? *error : NULL);
        cleanupOK = FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* delete error object                                                 */
    /*---------------------------------------------------------------------*/
    if (error && *error && !FlcErrorDelete(error))
    {
        sDisplayErrorString("deleting error object", NULL);
        cleanupOK = FLC_FALSE;
    }

    if (cleanupOK)
    {
       sDisplayInfoString("deleted licensing environment");
    }

    return cleanupOK;
}

/*==========================================================================*/
static FlcBool sTCharToFlcChar(const char * src, FlcChar * dest, FlcSize destSize)
/*==========================================================================*/
{
    if (!src || !dest || !destSize)
    {
        return FLC_FALSE;
    }
#ifdef _UNICODE
    WideCharToMultiByte(CP_UTF8,
                        0,
                        src,
                        -1,
                        dest,
                        (int)destSize,
                        0,
                        0);
#else
    strncpy(dest, src, destSize);
#endif
    return FLC_TRUE;
}

/*==========================================================================*/
void sDisplayErrorString(const char * msg, FlcErrorRef error)
/*==========================================================================*/
{
    char msg1[1024] = {0};

    /*----------------------------------------------------------------------*/
    if (!msg)
    {
        return;
    }

    /*----------------------------------------------------------------------*/
    if (error)
    {
        char errorMessage[512] = {0};
        FlcCharToTChar(FlcErrorGetMessage(error), errorMessage, 512);
        sprintf(msg1, "%s: %s", msg, errorMessage);
    }
    else
    {
        sprintf(msg1, "%s", msg);
    }

    sDisplayMessage(msg1, "ERROR");
}

/*==========================================================================*/
static void sDisplayInfoString(const char * msg)
/*==========================================================================*/
{
    if (msg)
    {
        sDisplayMessage(msg, "INFO");
    }
}

/*==========================================================================*/
static  void sDisplayMessage(const char * msg, const char * caption)
/*==========================================================================*/
{
    if (msg  && caption)
    {
#ifdef UI_OUTPUT
        MessageBox(NULL, msg, caption, 0);
#else /* !UI_OUTPUT */
        printf("%s: %s\n", caption, msg);
#endif /* UI_OUTPUT */
    }
}

/*==========================================================================*/
static void FlcCharToTChar(const FlcChar * data, char * result, size_t size)
/*==========================================================================*/
{
    if (data && result && size)
    {
#ifdef _UNICODE
        mbstowcs(result, data, size - 1);
#else
        strncpy(result, data, size);
#endif
    }
}
