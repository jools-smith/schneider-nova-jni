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
#include "FlcMachineType.h"
#include "FlcCapabilityRequest.h"
#include "FlcCapabilityResponse.h"
#include "FlcComm.h"

#include "util/ErrDisplay.h"
#include "util/FileIO.h"
#include "identity/IdentityClient.h"

/* Use this to direct output to Windows MessageBox
#define UI_OUTPUT
*/

/****************************************************************************
    ServedBufferRequest.c

    This example program allows you to:
    1. Send a capability request with the served-buffer property enabled,
       over http to the server and process the response, optionally saving
       served buffer response data to a file.
    2. Save a served buffer capability request into a file, for offline or
       indirect processing.
    3. Process one or two served buffer responses contained in files
       conveyed offline or indirectly.
    4. Given two served buffer response files report a stale response, if 
       one exists.
*****************************************************************************/

/* identity warning message */
const char * const emptyIdentity = 
    "License-enabled code requires client identity data, " \
    "which you create with pubidutil and printbin -C. " \
    "See the User Guide for more information.";

/* server parameters */
static char *         serverUrl  = NULL;

/* file names */
const static char *   inputFile  = NULL;
const static char *   inputFile1 = NULL;
const static char *   outputFile = NULL;

/* functionality */
static FlcBool         generateRequest     = FLC_FALSE;
static FlcBool         storeRequest        = FLC_FALSE;
static FlcBool         readResponse        = FLC_FALSE;
static FlcBool         talkToServer        = FLC_FALSE;
static FlcBool         acquireLicense      = FLC_FALSE;
static FlcBool         reportStale         = FLC_FALSE;

/* served buffer capability request and response binary buffers */
static FlcUInt8 *      binaryRequest       = NULL;
static FlcSize         binaryRequestSize   = 0;
static FlcUInt8 *      binaryResponse      = NULL;
static FlcSize         binaryResponseSize  = 0;
static FlcUInt8 *      binaryResponse1     = NULL;
static FlcSize         binaryResponseSize1 = 0;

/*-------------------------------------------------------------------------*/
/*   Utility Functions                                                     */
/*-------------------------------------------------------------------------*/

/**
 *    Validate arguments
 */
static FlcBool sValidateArgs(
    int argc,
    char * argv[]);

/**
 *    Communicate with a license server
 */
static FlcBool TalkToServer(
    const char *       targetUrl,
    const FlcUInt8 *    request,
    FlcSize             requestSize,
    FlcUInt8 **         response,
    FlcSize *           responseSize,
    FlcErrorRef         error);

/**
 *    Add host data
 */
static FlcBool sAddHostData(
    FlcLicensingRef licensing,
    FlcErrorRef     error);

/**
 *    Generate a served buffer capability request
 */
static FlcBool sGenerateCapabilityRequest(
    FlcLicensingRef  licensing,
    FlcBool          forceResponse,
    FlcUInt8 **      request,
    FlcSize *        requestSize,
    FlcErrorRef      error);

/**
 *    Process a served buffer response file
 */
static FlcBool sProcessServedBufferResponseFile(
    FlcLicensingRef licensing,
    const char *   name,
    FlcUInt8 **     response,
    FlcSize *       responseSize,
    FlcBool         responseForced,
    FlcBool *       responseAccepted,
    FlcErrorRef     error);

/**
 *    Process the served buffer response
 */
static FlcBool sProcessServedBufferResponse(
    FlcLicensingRef licensing,
    FlcUInt8 *      response,
    FlcSize         responseSize,
    FlcBool         responseForced,
    FlcBool *       responseAccepted,
    FlcErrorRef     error);

/**
 *    Display error status collection
 */
static void sDisplayErrorStatusInfo(
    FlcStatusCollectionRef    errorStatusCollection,
    FlcErrorRef               error);

/**
 *    Get served buffer response details
 */
static FlcBool sGetResponseDetails(
    FlcCapabilityResponseRef    capabilityResponse,
    FlcErrorRef                 error);

/**
 *    Obtain number of features in served buffer response
 */
static FlcBool sCheckNumberOfFeatures(
    FlcLicensingRef licensing,
    FlcUInt32 *     size,
    FlcUInt32       idType,
    const FlcChar * idValue,
    FlcErrorRef     error);

/**
 *    Acquire/Return licenses
 */
static FlcBool sAcquireLicense(
    FlcLicensingRef    licensing,
    FlcErrorRef        error);

/**
 *    Detect stale license file
 */
static FlcBool sReportStale(
    FlcLicensingRef    licensing,
    FlcErrorRef        error);

/**
 *    Cleanup processing
 */
static FlcBool sCleanup(
    FlcLicensingRef * licensing,
    FlcErrorRef *     error);

/**
 *    Example program usage
 */
static void sUsage(
    const char * applicationName);

/*=========================================================================*/
int main(int argc, char * argv[])
/*=========================================================================*/
{
    /* licensing environment object */
    FlcLicensingRef licensing    = NULL;

    /* error object */
    FlcErrorRef error            = NULL;

    /* other */
    char       msg[512]         = {0};
    FlcChar     tsPath[512]      = {0};
    FlcBool     forceResponse    = FLC_FALSE;
    FlcBool     responseAccepted = FLC_FALSE;


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
    /* set host data                                                       */
    /*---------------------------------------------------------------------*/
    if (!sAddHostData(licensing, error))
    {
        DisplayErrorString("adding host data", error);
        (void)sCleanup(&licensing, &error);
        return -1;
    }

    /*---------------------------------------------------------------------*/
    /* generate served buffer capability request                           */
    /*---------------------------------------------------------------------*/
    if (generateRequest)
    {
        if (!sGenerateCapabilityRequest(
                 licensing, forceResponse, 
                 &binaryRequest, &binaryRequestSize,
                 error))
        {
            (void)sCleanup(&licensing, &error);
            return -1;
        }
        DisplayInfoString("generated served buffer capability request");
    }

    /*---------------------------------------------------------------------*/
    /* store served buffer capability request                              */
    /*---------------------------------------------------------------------*/
    if (storeRequest)
    {
        if (!WriteFileData(outputFile, binaryRequest, binaryRequestSize))
        {
            (void)sCleanup(&licensing, &error);
            return -1;
        }

        sprintf(msg, "wrote served buffer capability request to %s", outputFile);
        DisplayInfoString(msg);
    }

    /*---------------------------------------------------------------------*/
    /* send served buffer capability request to the license server,        */
    /* receive served buffer response from the server                      */
    /*---------------------------------------------------------------------*/
    if (talkToServer)
    {
        if (!TalkToServer(
                serverUrl, binaryRequest, binaryRequestSize,
                &binaryResponse, &binaryResponseSize, error))
        {
            DisplayErrorString("talking to server", error);
            (void)sCleanup(&licensing, &error);
            return -1;
        }
        sprintf(msg, "talked to %s - sent request and received response",
                       serverUrl);
        DisplayInfoString(msg);
        if (outputFile)
        {
            sprintf(msg, "saving served buffer response to: %s", outputFile);
            DisplayInfoString(msg);
            if (!WriteFileData(outputFile, binaryResponse, binaryResponseSize))
            {
                (void)sCleanup(&licensing, &error);
                return -1;
            }
        }
        if (!sProcessServedBufferResponse(
                licensing, binaryResponse, binaryResponseSize,
                forceResponse, &responseAccepted, error))
        {
            (void)sCleanup(&licensing, &error);
            return -1;
        }
    }

    /*---------------------------------------------------------------------*/
    /* read served buffer response(s) from the provided file(s)            */
    /*---------------------------------------------------------------------*/
    if (readResponse)
    {
        if (!sProcessServedBufferResponseFile(licensing, inputFile, 
               &binaryResponse, &binaryResponseSize, forceResponse, 
               &responseAccepted, error))
        {
            (void)sCleanup(&licensing, &error);
            return -1;
        }
        if (inputFile1 &&
            !sProcessServedBufferResponseFile(licensing, inputFile1, 
               &binaryResponse1, &binaryResponseSize1, forceResponse, 
               &responseAccepted, error))
        {
            (void)sCleanup(&licensing, &error);
            return -1;
        }
    }

    /*---------------------------------------------------------------------*/
    /* acquire license                                                     */
    /*---------------------------------------------------------------------*/
    if (acquireLicense)
    {
        if (!sAcquireLicense(licensing, error))
        {
            (void)sCleanup(&licensing, &error);
            return -1;
        }
        DisplayInfoString("acquired license");
    }

    /*---------------------------------------------------------------------*/
    /* report stale license file                                           */
    /*---------------------------------------------------------------------*/
    if (reportStale)
    {
        if (!ReadFileData(inputFile, &binaryResponse, &binaryResponseSize) ||
            !ReadFileData(inputFile1, &binaryResponse1, &binaryResponseSize1))
        {
            (void)sCleanup(&licensing, &error);
            return -1;
        }

        if (!sReportStale(licensing, error))
        {
            (void)sCleanup(&licensing, &error);
            return -1;
        }
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
    FlcBool     invalidSpec = FLC_FALSE;
    FlcInt32    ii = 0, actions = 0;
    char *     arg = NULL;
    char *     command = NULL; 
    char       msg[512]    = {0};

    if (argc <= 1)
    {
        sUsage(argv[0]);
        return FLC_FALSE;
    }

      for (ii = 1; !invalidSpec && ii < argc; ii++)
      {
        arg = argv[ii];
        if (!strcmp("-help", arg)|| !strcmp("-h", arg))
        {
            sUsage(argv[0]);
            return FLC_FALSE;
        }
        else if (!strcmp("-generate", arg))
        { /* should be followed by 1 parameter for output filename */
            invalidSpec = (argc <= ii + 1 || actions) ? FLC_TRUE : FLC_FALSE;
            if (!invalidSpec)
            {
                command         = arg;
                outputFile      = argv[++ii];
                generateRequest = FLC_TRUE;
                storeRequest    = FLC_TRUE;
            }
            actions++;
        }
        else if (!strcmp("-process", arg))
        { /* should be followed by 1 or 2 parameters for input filename(s) */
            invalidSpec = (argc <= ii + 1 || actions) ? FLC_TRUE : FLC_FALSE;
            if (!invalidSpec)
            {
                command         = arg;
                inputFile       = argv[++ii];
                readResponse    = FLC_TRUE;
                acquireLicense  = FLC_TRUE;
                if (argc > ii + 1 && strncmp("-", argv[ii + 1], 1))
                {
                    inputFile1  = argv[++ii];   
                }
            }
            actions++;
        }
        else if (!strcmp("-server", arg))
        { /* should be followed by 3 parameters for server host, port and directory */
            invalidSpec = (argc <= ii + 1 || actions) ? FLC_TRUE : FLC_FALSE;
            if (!invalidSpec)
            {
                command         = arg;
                serverUrl       = argv[++ii];
                generateRequest = FLC_TRUE;
                talkToServer    = FLC_TRUE;
                acquireLicense  = FLC_TRUE;
            }
            actions++;
        }
        else if (!strcmp("-reportstale", arg))
        { /* should be followed by 2 file names */
            invalidSpec = (argc <= ii + 2 || actions) ? FLC_TRUE : FLC_FALSE;
            if (!invalidSpec)
            {
                command         = arg;
                inputFile       = argv[++ii];
                inputFile1      = argv[++ii];
                reportStale     = FLC_TRUE;
            }
            actions++;
        }
        else if (!strcmp("-save", arg))
        { /* should be followed by a file name */
            invalidSpec = (argc <= ii + 1) ? FLC_TRUE : FLC_FALSE;
            if (!invalidSpec)
            {
                if  (!outputFile)
                {
                    outputFile  = argv[ii + 1];
                }
                ii++;
            }
        }
        else
        {
            sprintf(msg, "unknown option: %s", arg);
            DisplayErrorString(msg, NULL);
            sUsage(argv[0]);
            return FLC_FALSE;
        }
    }

    if (invalidSpec)
    {
        if (actions > 1)
        {
            sprintf(msg, "multiple operations specified: %s, %s", command, arg);
        }
        else
        {
            sprintf(msg, "invalid specification for option: %s", arg);
        }
        DisplayErrorString(msg, NULL);
        sUsage(argv[0]);
        return FLC_FALSE;
    }
    else if (!actions)
    {
        sUsage(argv[0]);
        return FLC_FALSE;
    }

    return FLC_TRUE;
}

/*=========================================================================*/
static FlcBool sCleanup(
    FlcLicensingRef * licensing,
    FlcErrorRef *     error)
/*=========================================================================*/
{  
    FlcBool cleanupOK = FLC_TRUE;

    /*---------------------------------------------------------------------*/
    /* delete binary buffers                                               */
    /*---------------------------------------------------------------------*/
    if (binaryRequest)
    {
        FlcMemoryFree(binaryRequest);
    }
    if (binaryResponse)
    {
      (readResponse || reportStale) ? free(binaryResponse) : FlcMemoryFree(binaryResponse);
    }
    if (binaryResponse1)
    {
      (readResponse || reportStale) ? free(binaryResponse1) : FlcMemoryFree(binaryResponse1);
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

/*=========================================================================*/
static FlcBool sAddHostData(
    FlcLicensingRef licensing,
    FlcErrorRef     error)
/*=========================================================================*/
{
    if (!licensing || !error)
    {
        DisplayErrorString("adding host data", NULL);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* The optional host name is typically set by a user as a friendly     */
    /* name for the host.                                                  */
    /* The host name is not used for license enforcement.                  */
    /*---------------------------------------------------------------------*/
    if (!FlcSetHostName(licensing, "Sample Device", error))
    {
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* The host type is typically a name set by the implementer, and is    */
    /* not modifiable by the user.                                         */
    /* While optional, the host type may be used in certain scenarios by   */
    /* some back-office systems such as FlexNet Operations.                */
    /*---------------------------------------------------------------------*/
    if (!FlcSetHostType(licensing, "FLX_CLIENT", error))
    {
        return FLC_FALSE;
    }

    return FLC_TRUE;
}

/*=========================================================================*/
static
FlcBool
sGenerateCapabilityRequest(
    FlcLicensingRef  licensing,
    FlcBool          forceResponse,
    FlcUInt8 **      request,
    FlcSize *        requestSize,
    FlcErrorRef      error)
/*=========================================================================*/
{
    FlcCapabilityRequestRef capabilityRequest = NULL;

    if (!licensing || !request || !requestSize || !error)
    {
        DisplayErrorString("generating served buffer capability request", NULL);
        return FLC_FALSE;
    }
    *request     = NULL;
    *requestSize = 0;

    /*---------------------------------------------------------------------*/
    /* create capability request object                                    */
    /*---------------------------------------------------------------------*/
    if (!FlcCapabilityRequestCreate(licensing,
                                    &capabilityRequest,
                                    error))
    {
        DisplayErrorString("creating capability request object", error);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* Set force response flag - optional                                  */
    /*---------------------------------------------------------------------*/
    if (!FlcCapabilityRequestSetForceResponse(licensing,
                                              capabilityRequest,
                                              forceResponse,
                                              error))
    {
        (void)FlcCapabilityRequestDelete(licensing, &capabilityRequest, NULL);
        DisplayErrorString("setting capability request force response", error);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* Set served buffer flag                                              */
    /*---------------------------------------------------------------------*/
    if (!FlcCapabilityRequestSetServedBuffer(licensing,
                                             capabilityRequest,
                                             FLC_TRUE,
                                             error))
    {
        (void)FlcCapabilityRequestDelete(licensing, &capabilityRequest, NULL);
        DisplayErrorString("setting capability request served buffer indicator", error);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------------*/
    /* Other optional information can be added to the request.  For example,     */
    /* Before sending the request to a back-office server such as FlexNet        */
    /* Operations, you can add one or more optional rights IDs and counts to the */
    /* capability request using FlcCapabilityRequestAddRightsId, as in:          */
    /*                                                                           */
    /* if (!FlcCapabilityRequestAddRightsId(licensing,                           */
    /*                                      capabilityRequest,                   */
    /*                                      "ACT-TEST",                          */
    /*                                      1,                                   */
    /*                                      error))                              */
    /* {                                                                         */
    /*     (void)FlcCapabilityRequestDelete(licensing, &capabilityRequest, NULL);*/
    /*     DisplayErrorString("adding rights id", error);                    */
    /*     return FLC_FALSE;                                                     */
    /* }                                                                         */
    /*                                                                           */
    /* Similarly, before sending a request to a local license server, you can    */
    /* add one or more desired features and counts to the request, or instruct   */
    /* the local server to include vendor dictionary data in its response.       */
    /*                                                                           */
    /* if (!FlcCapabilityRequestAddDesiredFeature(licensing,                     */
    /*                                            capabilityRequest,             */
    /*                                            "feature-name",                */
    /*                                            "1.0",                         */
    /*                                            1,                             */
    /*                                            error))                        */
    /* {                                                                         */
    /*     (void)FlcCapabilityRequestDelete(licensing, &capabilityRequest, NULL);*/
    /*     DisplayErrorString("adding desired features", error);             */
    /*     return FLC_FALSE;                                                     */
    /* }                                                                         */
    /*---------------------------------------------------------------------------*/

    /*---------------------------------------------------------------------------*/
    /* Moreover, when FNE code is acting as a proxy, the hostid and type on      */
    /* behalf of which the request is being generated should be explicitly       */
    /* set in the licensing environment.  In direct communications between a     */
    /* client and server, this is not necessary:                                 */
    /*                                                                           */
    /* FlcSetDefaultHostId(licensing,                                            */
    /*                     hostIdType,                                           */
    /*                     "hostid value",                                       */
    /*                     error)                                                */
    /*---------------------------------------------------------------------------*/

    /*---------------------------------------------------------------------*/
    /* generate binary capability request                                  */
    /*---------------------------------------------------------------------*/
    if (!FlcCapabilityRequestGenerate(licensing,
                                      capabilityRequest, 
                                      request,
                                      requestSize,
                                      error))
    {
        (void)FlcCapabilityRequestDelete(licensing, &capabilityRequest, NULL);
        DisplayErrorString("generating binary capability request", error);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* delete capability request object                                    */
    /*---------------------------------------------------------------------*/
    if (!FlcCapabilityRequestDelete(licensing, &capabilityRequest, error))
    {
        DisplayErrorString("deleting capability request object", error);
        return FLC_FALSE;
    }

    return FLC_TRUE;
}

/*=========================================================================*/
static FlcBool sProcessServedBufferResponseFile(
    FlcLicensingRef licensing,
    const char *   name,
    FlcUInt8 **     response,
    FlcSize *       responseSize,
    FlcBool         responseForced,
    FlcBool *       responseAccepted,
    FlcErrorRef     error)
/*=========================================================================*/
{
    *responseAccepted = FLC_FALSE;
    if (!ReadFileData(name, response, responseSize))
    {
        return FLC_FALSE;
    }
    if (!(*response) || !(*responseSize))
    {
        sprintf(msg, "file %s is empty", name);
        DisplayInfoString(msg);
    }
    else
    {
        sprintf(msg, "read data from %s", name);
        DisplayInfoString(msg);
        if (!sProcessServedBufferResponse(
                licensing, *response, *responseSize,
                responseForced, responseAccepted, error))
        {
            return FLC_FALSE;
        }
    }
    return FLC_TRUE;
}

/*=========================================================================*/
static
FlcBool
sProcessServedBufferResponse(
    FlcLicensingRef licensing,
    FlcUInt8 *      response,
    FlcSize         responseSize,
    FlcBool         responseForced,
    FlcBool *       responseAccepted,
    FlcErrorRef     error)
/*=========================================================================*/
{
    FlcCapabilityResponseRef capabilityResponse = NULL;
    FlcStatusCollectionRef errorStatusCollection = NULL;
    FlcBool licenseSourceExists   = FLC_FALSE;
    FlcBool responseIsStale       = FLC_FALSE;
    FlcUInt32 serverIdType        = (FlcUInt32)FLC_HOSTID_TYPE_UNKNOWN;
    const FlcChar * serverIdValue = NULL;
    FlcUInt32 featureCount        = 0;

    if (!licensing || !responseAccepted || !error ||
        (responseSize && !response))
    {
        DisplayErrorString("processing served buffer response", NULL);
        return FLC_FALSE;
    }
    *responseAccepted = FLC_FALSE;

    if (responseSize == 0)
    {
        if (responseForced)
        {
            DisplayErrorString("missing forced response", NULL);
            return FLC_FALSE;
        }
        else
        {
            /*----------------------------------------------------------------------*/
            /* By convention, a server sends a response of size zero if there are   */
            /* no changes to report since the last confirmed processed response and */
            /* the "force response" flag was not set on the request.  (Different    */
            /* server implementations may use different conventions.)               */
            /*----------------------------------------------------------------------*/
            DisplayInfoString("server indicates that there were no changes relative to last response");
            *responseAccepted = FLC_TRUE;
            return FLC_TRUE;
        }
    }

    /*---------------------------------------------------------------------*/
    /* determine whether or not this served buffer response data is stale  */ 
    /*---------------------------------------------------------------------*/
    if (!FlcServedBufferLicenseSourceExists(licensing,
                                            response,
                                            responseSize,
                                            &licenseSourceExists,
                                            &responseIsStale,
                                            error))
    {
        DisplayErrorString("checking for stale served buffer response data", error);
        return FLC_FALSE;
    }

    if (licenseSourceExists && responseIsStale)
    {
        /* capability response data is out of date and will return FLCERR_RESPONSE_STALE if processed */
        DisplayInfoString("served buffer response data is stale and will not be processed");
        return FLC_TRUE;
    }

    /*---------------------------------------------------------------------*/
    /* process served buffer response into a served buffer license source  */ 
    /* and create capability response object from binary response          */
    /*---------------------------------------------------------------------*/
    if (!FlcAddServedBufferLicenseSourceFromData(licensing,
                                                 &capabilityResponse,
                                                 response, 
                                                 responseSize, 
                                                 error))
    {
        DisplayErrorString("processing served buffer response", error);
        switch (FlcErrorGetCode(error))
        {
            case FLCERR_RESPONSE_STALE:
            case FLCERR_RESPONSE_EXPIRED:
            case FLCERR_CAPABILITY_RESPONSE_TYPE_INVALID:
                {
                    return FLC_TRUE;
                }
            default:
                {
                    if (FlcGetLastErrorStatusCollection(licensing, &errorStatusCollection, error) &&
                        errorStatusCollection)
                    {
                        sDisplayErrorStatusInfo(errorStatusCollection, error);
                    }
                    return FLC_FALSE;
                }
        }
    }

    /*--------------------------------------------------------------------*/
    /* get served buffer response server id                               */
    /*--------------------------------------------------------------------*/
    if (!FlcCapabilityResponseGetServerId(capabilityResponse,
                                          &serverIdType,
                                          &serverIdValue, 
                                          error))
    {
        DisplayErrorString("getting served buffer response server hostid", error);
        (void)FlcCapabilityResponseDelete(licensing, &capabilityResponse, NULL);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* get served buffer response details                                  */
    /*---------------------------------------------------------------------*/
    if (!sGetResponseDetails(capabilityResponse, error))
    {
        (void)FlcCapabilityResponseDelete(licensing, &capabilityResponse, NULL);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* check number of features loaded from served buffer response         */
    /*---------------------------------------------------------------------*/
    if (!sCheckNumberOfFeatures(licensing,
                                &featureCount,
                                serverIdType,
                                serverIdValue, 
                                error))
    {
        (void)FlcCapabilityResponseDelete(licensing, &capabilityResponse, NULL);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* delete capability response object                                   */
    /*---------------------------------------------------------------------*/
    if (!FlcCapabilityResponseDelete(licensing, &capabilityResponse, error))
    {
        DisplayErrorString("deleting capability response object", error);
        return FLC_FALSE;
    }
    DisplayInfoString("processed served buffer response");
    sprintf(msg, "number of features loaded from served buffer response: %u", featureCount);
    DisplayInfoString(msg);

    *responseAccepted = FLC_TRUE;
    return FLC_TRUE;
}

/*=========================================================================*/
static FlcBool sAcquireLicense(
    FlcLicensingRef licensing,
    FlcErrorRef     error)
/*=========================================================================*/
{
    FlcLicenseRef license = NULL;

    const FlcChar* const survey  = "survey";
    const FlcChar* const version = "1.0";

    if (!licensing  ||  !error)
    {
        DisplayErrorString("acquiring license", NULL);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* acquire license                                                     */
    /*---------------------------------------------------------------------*/
    if (!FlcAcquireLicense(licensing, &license, survey, version, error))
    {
        DisplayErrorString("acquiring survey license", error);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* return license                                                      */
    /*---------------------------------------------------------------------*/
    if (!FlcReturnLicense(licensing, &license, error))
    {
        DisplayErrorString("returning survey license", error);
        return FLC_FALSE;
    }

    return FLC_TRUE;
}

/*=========================================================================*/
static
FlcBool
sGetResponseDetails(
    FlcCapabilityResponseRef    capabilityResponse,
    FlcErrorRef                 error)
/*=========================================================================*/
{

    FlcDictionaryRef        vmInfo          = 0;
    FlcDictionaryRef        dictionary      = 0;
    FlcStatusCollectionRef  status          = 0;    
    FlcUInt32               size            = 0;
    FlcDictionaryValueType  valueType       = FLC_DICTIONARY_UNKNOWN_VALUE;
    FlcMachineType          machineType     = FLC_MACHINE_TYPE_UNKNOWN;
    const FlcChar *         key             = 0;
    const FlcChar *         stringValue     = 0;
    FlcInt32                intValue        = 0;
    FlcUInt32               iii             = 0;
    char                   msg[1024]       = {0};
    const FlcChar *         formattedStatus = 0;


    if (!capabilityResponse  ||  !error)
    {
        DisplayErrorString("getting served buffer response details", 0);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* get machine type from response                                      */
    /*---------------------------------------------------------------------*/
    FlcCapabilityResponseGetVirtualMachineType(capabilityResponse, &machineType, 0);
    switch (machineType)
    {
        case FLC_MACHINE_TYPE_PHYSICAL:
            DisplayInfoString("machine type PHYSICAL");
            break;
        case FLC_MACHINE_TYPE_VIRTUAL:
            DisplayInfoString("machine type VIRTUAL");
            break;
        case FLC_MACHINE_TYPE_UNKNOWN:
        default:
            DisplayInfoString("machine type UNKNOWN");
            break;
    }

    if (FLC_MACHINE_TYPE_VIRTUAL == machineType)
    {
        FlcCapabilityResponseGetVirtualMachineInfo(capabilityResponse, &vmInfo, 0);
        if (vmInfo)
        {
            if (!FlcDictionaryGetSize(vmInfo, &size, error))
            {
                DisplayErrorString("getting virtual machine dictionary size", error);
                return FLC_FALSE;
            }
            sprintf(msg, "number of items in the response virtual machine dictionary: %d", size);
            DisplayInfoString(msg);
        }
    }

    /*---------------------------------------------------------------------*/
    /* get vendor dictionary from response                                 */
    /*---------------------------------------------------------------------*/
    FlcCapabilityResponseGetVendorDictionary(capabilityResponse, &dictionary, 0);

    if (!dictionary)
    {
        DisplayInfoString("no vendor dictionary in served buffer response");
    }
    else
    {
        /*-----------------------------------------------------------------*/
        /* get vendor dictionary size                                      */
        /*-----------------------------------------------------------------*/
        if (!FlcDictionaryGetSize(dictionary, &size, error))
        {
            DisplayErrorString("getting vendor dictionary size", error);
            return FLC_FALSE;
        }

        sprintf(msg, "number of items in the response vendor dictionary: %d", size);
        DisplayInfoString(msg);

        /*-----------------------------------------------------------------*/
        /* get vendor dictionary items                                     */
        /*-----------------------------------------------------------------*/
        for (iii = 0; iii < size; iii++)
        {
            /* get item type */
            if (!FlcDictionaryGetValueType(dictionary, iii, &valueType, error))
            {
                DisplayErrorString("getting item value type from vendor dictionary", error);
                return FLC_FALSE;
            }

            /* get key and value according to the type */
            switch (valueType)
            {
                case FLC_DICTIONARY_STR_VALUE:
                    if (!FlcDictionaryGetStringItem(dictionary, iii, &key, &stringValue, error))
                    {
                        DisplayErrorString("getting string item from vendor dictionary", error);
                        return FLC_FALSE;
                    }
                    break;

                case FLC_DICTIONARY_INT_VALUE:
                    if (!FlcDictionaryGetIntItem(dictionary, iii, &key, &intValue, error))
                    {
                        DisplayErrorString("getting integer item from vendor dictionary", error);
                        return FLC_FALSE;
                    }
                    break;

                default:
                    DisplayErrorString("unknown item value type from vendor dictionary", error);
                    return FLC_FALSE;
            }
        }
    }

    /*---------------------------------------------------------------------*/
    /* get status collection from response                                 */
    /*---------------------------------------------------------------------*/
    size = 0;
    FlcCapabilityResponseGetStatusCollection(capabilityResponse, &status, 0);
    if (status && !FlcStatusCollectionSize(status, &size, error))
    {
      DisplayErrorString("getting number of status items", error);
      return FLC_FALSE;
    }

    if (size)
    {
      sprintf(msg, "number of status items in the served buffer response: %d", size);
      DisplayInfoString(msg);
    }

    /*---------------------------------------------------------------------*/
    /* get status information                                              */
    /*---------------------------------------------------------------------*/
    for (iii = 0; iii < size; iii++)
    {
        /* get item formatted description */
        if (!FlcStatusCollectionFormatItem(status, iii, &formattedStatus, error))
        {
            DisplayErrorString("getting status information from response", error);
            return FLC_FALSE;
        }
        FlcCharToTChar(formattedStatus, msg, sizeof(msg)/sizeof(char));
        DisplayInfoString(msg);
    }

    return FLC_TRUE;
}

/*=========================================================================*/
static
void
sDisplayErrorStatusInfo(
    FlcStatusCollectionRef    errorStatusCollection,
    FlcErrorRef               error)
/*=========================================================================*/
{

    FlcUInt32               size            = 0;
    FlcUInt32               iii             = 0;
    const FlcChar *         formattedStatus = NULL;
    char                   msg[1024]       = {0};

    /*---------------------------------------------------------------------*/
    /* process error status collection                                     */
    /*---------------------------------------------------------------------*/
    if (!FlcStatusCollectionSize(errorStatusCollection, &size, error))
    {
        DisplayErrorString("getting number of error status items", error);
        return;
    }

    if (size)
    {
        sprintf(msg, "number of error status items returned: %d", size);
        DisplayInfoString(msg);
    }

    /*---------------------------------------------------------------------*/
    /* get status information                                              */
    /*---------------------------------------------------------------------*/
    for (iii = 0; iii < size; iii++)
    {
        /* get item formatted description */
        if (!FlcStatusCollectionFormatItem(errorStatusCollection, iii, &formattedStatus, error))
        {
            DisplayErrorString("getting status information from response", error);
            return;
        }
        FlcCharToTChar(formattedStatus, msg, sizeof(msg)/sizeof(char));
        DisplayInfoString(msg);
    }
}

/*=========================================================================*/
static
FlcBool
sCheckNumberOfFeatures(
    FlcLicensingRef licensing,
    FlcUInt32 *     size,
    FlcUInt32       idType,
    const FlcChar * idValue,
    FlcErrorRef     error)
/*=========================================================================*/
{
    FlcFeatureCollectionRef features = NULL;

    if (!licensing  ||  !size  ||  !error)
    {
        DisplayErrorString("checking number of features", NULL);
        return FLC_FALSE;
    }
    *size = 0;

    /*---------------------------------------------------------------------*/
    /* create served buffer feature collection for the specified server    */
    /*---------------------------------------------------------------------*/
    if (!FlcGetServedBufferLicenseSourceFeatureCollection(licensing,
                                                          &features,
                                                          idType,
                                                          idValue,
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
static FlcBool sReportStale(
    FlcLicensingRef licensing,
    FlcErrorRef     error)
/*=========================================================================*/
{
    FlcCapabilityResponseRef capResp  = NULL;
    FlcCapabilityResponseRef capResp1 = NULL;
    FlcMessageType respType  = FLC_MESSAGE_TYPE_UNKNOWN;
    FlcMessageType respType1 = FLC_MESSAGE_TYPE_UNKNOWN; 
    FlcUInt32 serverIdType  = 0;
    FlcUInt32 serverIdType1 = 0;
    const FlcChar * serverIdValue  = NULL;
    const FlcChar * serverIdValue1 = NULL;
    const time_t *  servedTime  = NULL;
    const time_t *  servedTime1 = NULL;

    if (!strcmp(inputFile, inputFile1))
    {
        DisplayErrorString("-reportstale option file names cannot be the same", NULL);
        return FLC_FALSE;
    }
    sprintf(msg, "checking files %s, %s for a stale served buffer response", inputFile, inputFile1);
    DisplayInfoString(msg);

    if (!FlcCapabilityResponseCreate(licensing, &capResp,
                                     binaryResponse, binaryResponseSize, error))
    {
        sprintf(msg, "creating capability response object from file: %s", inputFile);
        DisplayErrorString(msg, error);
        return FLC_FALSE;
    }
    (void)FlcCapabilityResponseGetMessageType(capResp, &respType, NULL);
    if (respType != FLC_MESSAGE_TYPE_BUFFER_CAPABILITY_RESPONSE)
    {
        (void)FlcCapabilityResponseDelete(licensing, &capResp, NULL);
        sprintf(msg, "file '%s' is not a served buffer response", inputFile);
        DisplayErrorString(msg, NULL);
        return FLC_FALSE;
    }

    if (!FlcCapabilityResponseCreate(licensing, &capResp1,
                                     binaryResponse1, binaryResponseSize1, error))
    {
        (void)FlcCapabilityResponseDelete(licensing, &capResp, NULL);
        sprintf(msg, "creating capability response object from file: %s", inputFile1);
        DisplayErrorString(msg, error);
        return FLC_FALSE;
    }
    (void)FlcCapabilityResponseGetMessageType(capResp1, &respType1, NULL);
    if (respType1 != FLC_MESSAGE_TYPE_BUFFER_CAPABILITY_RESPONSE)
    {
        (void)FlcCapabilityResponseDelete(licensing, &capResp, NULL);
        (void)FlcCapabilityResponseDelete(licensing, &capResp1, NULL);
        sprintf(msg, "file '%s' is not a served buffer response", inputFile1);
        DisplayErrorString(msg, NULL);
        return FLC_FALSE;
    }

    if (!FlcCapabilityResponseGetServerId(capResp, &serverIdType, &serverIdValue, error)    ||
        !FlcCapabilityResponseGetServedTime(capResp, &servedTime, error)                    ||
        !FlcCapabilityResponseGetServerId(capResp1, &serverIdType1, &serverIdValue1, error) ||
        !FlcCapabilityResponseGetServedTime(capResp1, &servedTime1, error))
    {
        (void)FlcCapabilityResponseDelete(licensing, &capResp, NULL);
        (void)FlcCapabilityResponseDelete(licensing, &capResp1, NULL);
        DisplayErrorString("obtaining server id and served time information", error);
        return FLC_FALSE;
    }

    if (!serverIdValue || !serverIdValue1 || ! servedTime || !servedTime1)
    {
        (void)FlcCapabilityResponseDelete(licensing, &capResp, NULL);
        (void)FlcCapabilityResponseDelete(licensing, &capResp1, NULL);
        DisplayErrorString("server id and served time information not available", NULL);
        return FLC_FALSE;
    }

    if (serverIdType != serverIdType1 || strcmp(serverIdValue, serverIdValue1))
    {
        DisplayInfoString("files are from different servers");
    }
    else 
    {
        sprintf(msg, "stale file: %s", 
             (difftime(*servedTime1, *servedTime) >= 0 ? inputFile : inputFile1)); 
        DisplayInfoString(msg);
    }

    (void)FlcCapabilityResponseDelete(licensing, &capResp, NULL);
    (void)FlcCapabilityResponseDelete(licensing, &capResp1, NULL);
    return FLC_TRUE;
}


/*=========================================================================*/
static void sUsage(
    const char * applicationName)
/*=========================================================================*/
{
    char msg[2048] = {0};

    snprintf(msg, 2047,
        "\n%s [-generate outputfile]"
        "\n%s [-reportstale inputfile inputfile1]"
        "\n%s [-process inputfile [inputfile1]]"
        "\n%s [-server url [-save outputfile]]"
        "\n\nwhere:"
        "\n-generate Generates served buffer capability request into a file."
        "\n-reportstale If 'inputfile' and 'inputfile1' originate from the same"
        "\n          server a determination will be made as to which file is older."
        "\n-process  Processes served buffer response(s) from file(s)."
        "\n-server   Sends served buffer capability request to a server and processes"
        "\n          the served buffer response."
        "\n          For the test back-office server, use"
        "\n          http://hostname:8080/request."
        "\n          For FlexNet Operations, use"
        "\n          http://hostname:8888/flexnet/deviceservices."
        "\n          For FlexNet Embedded License Server, use"
        "\n          http://hostname:7070/fne/bin/capability."
        "\n          For Cloud License Server, use"
        "\n          https://<tenant>.compliance.flexnetoperations.com/instances/<instance-id>/request."
        "\n          For FNO Cloud, use"
        "\n          https://<tenant>.compliance.flexnetoperations.com/deviceservices."
        "\n-save     Saved served buffer response file name.",
        applicationName, applicationName, applicationName, applicationName);

    DisplayMessage(msg, "USAGE");
}

/*=========================================================================*/
FlcBool
TalkToServer(
    const char *       targetUrl,
    const FlcUInt8 *    request,
    FlcSize             requestSize,
    FlcUInt8 **         response,
    FlcSize *           responseSize,
    FlcErrorRef         error)
/*=========================================================================*/
{
    FlcBool rc = FLC_TRUE;
    FlcCommRef commInterface = 0;
    FlcChar url[512]     = {0};

    if (!FlcCommCreate(&commInterface, error))
    {
        DisplayErrorString("creating communications interface", error);
        return FLC_FALSE;
    }
    TCharToFlcChar(targetUrl, url, sizeof(url));
    rc = FlcCommSetServer(commInterface, url, error) &&
         FlcCommSendBinaryMessage(commInterface, (void *)request, requestSize, (void **)response, responseSize, error);
    (void)FlcCommDelete(&commInterface, 0);
    return rc;
}

