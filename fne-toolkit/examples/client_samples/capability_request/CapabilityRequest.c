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
    CapabilityRequest.c

    This example program allows you to:
    1. Send a capability request via http to the server and process response,
       saving data into trusted storage.
    2. Write capability request to a file. This request can be fed into server
       to generate response.
    3. Read capability response from a file and process accordingly.
*****************************************************************************/

const static char * months[12] = { "jan", "feb", "mar", "apr",
                                   "may", "jun", "jul", "aug",
                                   "sep", "oct", "nov", "dec" };

/* identity warning message */
const char * const emptyIdentity = 
    "License-enabled code requires client identity data, " \
    "which you create with pubidutil and printbin -C. " \
    "See the User Guide for more information.";

/* server parameters */
static char *     serverUrl       = NULL;

/* file names */
static char *     inputFile       = NULL;
static char *     outputFile      = NULL;

/* functionality */
static FlcBool     generateRequest = FLC_FALSE;
static FlcBool     storeRequest    = FLC_FALSE;
static FlcBool     processResponse = FLC_FALSE;
static FlcBool     showFeatures    = FLC_FALSE;
static FlcBool     readResponse    = FLC_FALSE;
static FlcBool     talkToServer    = FLC_FALSE;
static FlcBool     acquireLicense  = FLC_FALSE;

/* capability request and response binary buffers */
static FlcUInt8 *  binaryRequest         = NULL;
static FlcSize     binaryRequestSize     = 0;
static FlcUInt8 *  binaryResponse        = NULL;
static FlcSize     binaryResponseSize    = 0;


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
 *    Obtain number of features in trusted storage
 */
static FlcBool sCheckNumberOfFeatures(
    FlcLicensingRef licensing,
    FlcUInt32 *     size,
    FlcErrorRef     error);

/**
 *    Generate a capability request
 */
static FlcBool sGenerateCapabilityRequest(
    FlcLicensingRef  licensing,
    FlcBool          forceResponse,
    FlcUInt8 **      request,
    FlcSize *        requestSize,
    FlcErrorRef      error);

/**
 *    Process the capability response
 */
static FlcBool sProcessCapabilityResponse(
    FlcLicensingRef licensing,
    FlcUInt8 *      response,
    FlcSize         responseSize,
    FlcBool         responseForced,
    FlcBool *       responseAccepted,
    FlcErrorRef     error);

/**
 *    Display the capability response features
 */
static FlcBool sDisplayCapabilityResponseFeatures(
    FlcLicensingRef licensing,
    FlcUInt8 *      response,
    FlcSize         responseSize,
    FlcErrorRef     error);

static FlcBool sPrintFeatureAttributes(
    FlcFeatureRef feature,
    FlcUInt32     index,
    FlcErrorRef   error);

static void sDateToString(const struct tm * date, char * string);

static void sAddFeatureInfo(char * buff, size_t size, const char *info);


/**
 *    Display error status collection
 */
static void sDisplayErrorStatusInfo(
    FlcStatusCollectionRef    errorStatusCollection,
    FlcErrorRef               error);

/**
 *    Get capability response details
 */
static FlcBool sGetResponseDetails(
    FlcCapabilityResponseRef    capabilityResponse,
    FlcErrorRef                 error);

/**
 *    Acquire/Return licenses
 */
static FlcBool sAcquireLicense(
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
    FlcUInt32   featureCount     = 0;
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
    /* add trusted storage license source                                  */
    /*---------------------------------------------------------------------*/
    if (!FlcAddTrustedStorageLicenseSource(licensing, error))
    {
        DisplayErrorString("adding trusted storage license source", error);
        (void)sCleanup(&licensing, &error);
        return -1;
    }

    if (!sCheckNumberOfFeatures(licensing, &featureCount, error))
    {
        (void)sCleanup(&licensing, &error);
        return -1;
    }

    sprintf(msg, "number of features loaded from trusted storage: %u", featureCount);
    DisplayInfoString(msg);

    /*---------------------------------------------------------------------*/
    /* generate capability request                                         */
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
        DisplayInfoString("generated capability request");
    }

    /*---------------------------------------------------------------------*/
    /* store capability request                                            */
    /*---------------------------------------------------------------------*/
    if (storeRequest)
    {
        if (!WriteFileData(outputFile, binaryRequest, binaryRequestSize))
        {
            (void)sCleanup(&licensing, &error);
            return -1;
        }

        sprintf(msg, "wrote capability request to %s", outputFile);
        DisplayInfoString(msg);
    }

    /*---------------------------------------------------------------------*/
    /* send capability request to the license server,                      */
    /* receive capability response from the server                         */
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
    }

    /*---------------------------------------------------------------------*/
    /* read capability response from the file                              */
    /*---------------------------------------------------------------------*/
    if (readResponse)
    {
        if (!ReadFileData(inputFile, &binaryResponse, &binaryResponseSize))
        {
            (void)sCleanup(&licensing, &error);
            return -1;
        }
        sprintf(msg, "read capability response from %s", inputFile);
        DisplayInfoString(msg);
        responseAccepted = FLC_TRUE;
    }

    /*---------------------------------------------------------------------*/
    /* show capability response features                                   */
    /*---------------------------------------------------------------------*/
    if (showFeatures)
    {
        if (!sDisplayCapabilityResponseFeatures(
                licensing, binaryResponse, binaryResponseSize, error))
        {
            (void)sCleanup(&licensing, &error);
            return -1;
        }
    }

    /*---------------------------------------------------------------------*/
    /* process capability response                                         */
    /*---------------------------------------------------------------------*/
    if (processResponse)
    {
        if (!sProcessCapabilityResponse(
                licensing, binaryResponse, binaryResponseSize,
                forceResponse, &responseAccepted, error))
        {
            (void)sCleanup(&licensing, &error);
            return -1;
        }
    }

    /*---------------------------------------------------------------------*/
    /* check number of features loaded from capability response            */
    /*---------------------------------------------------------------------*/

    if (responseAccepted)
    {
        if (!sCheckNumberOfFeatures(licensing, &featureCount, error))
        {
            (void)sCleanup(&licensing, &error);
            return -1;
        }

        sprintf(msg, "number of features loaded from capability response: %u", featureCount);
        DisplayInfoString(msg);
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
    /* cleanup                                                             */
    /*---------------------------------------------------------------------*/
    return sCleanup(&licensing, &error) ? 0 : -1;
}

/*=========================================================================*/
static FlcBool sValidateArgs(int argc, char * argv[])
/*=========================================================================*/
{
    FlcBool     invalidSpec = FLC_FALSE;
    char       msg[512]    = {0};

    if (argc <= 1 || strcmp("-help", argv[1]) == 0 || strcmp("-h", argv[1]) == 0)
    {
        sUsage(argv[0]);
        return FLC_FALSE;
    }

    if (strcmp("-generate", argv[1]) != 0 &&
        strcmp("-process",  argv[1]) != 0 &&
        strcmp("-server",   argv[1]) != 0)
    {
        sprintf(msg, "unknown option: %s", argv[1]);
        DisplayErrorString(msg, NULL);
        sUsage(argv[0]);
        return FLC_FALSE;
    }

    if (argc != 3)
    {
        invalidSpec = FLC_TRUE;
    }

    if (strcmp("-server", argv[1]) == 0)
    {
        serverUrl      = argv[2];
        generateRequest = FLC_TRUE;
        talkToServer    = FLC_TRUE;
        processResponse = FLC_TRUE;
        acquireLicense  = FLC_TRUE;
    }
    else if (strcmp("-generate", argv[1]) == 0)
    {
        outputFile      = argv[2];
        generateRequest = FLC_TRUE;
        storeRequest    = FLC_TRUE;
    }
    else if (strcmp("-process", argv[1]) == 0)
    {
        inputFile       = argv[2];
        readResponse    = FLC_TRUE;
        processResponse = FLC_TRUE;
        acquireLicense  = FLC_TRUE;
    }
    else
    {   /* should not be hit but left here in case options evolve. */
        invalidSpec = FLC_TRUE;
    }

    if (invalidSpec)
    {
        sprintf(msg, "invalid specification for option: %s", argv[1]);
        DisplayErrorString(msg, NULL);
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
      readResponse ? free(binaryResponse) : FlcMemoryFree(binaryResponse);
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
sCheckNumberOfFeatures(
    FlcLicensingRef licensing,
    FlcUInt32 *     size,
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
    /* create trusted storage feature collection                           */
    /*---------------------------------------------------------------------*/
    if (!FlcGetTrustedStorageFeatureCollection(licensing,
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
        DisplayErrorString("generating capability request", NULL);
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
    /* Set one or more optional rights ID and count values in the          */
    /* capability request. This is often used with FlexNet Operations.     */ 
    /*---------------------------------------------------------------------*/
    /*
    if (!FlcCapabilityRequestAddRightsId(licensing, 
                                          capabilityRequest, 
                                          "ACT-TEST",
                                          1,
                                          error))
    {
        (void)FlcCapabilityRequestDelete(licensing, &capabilityRequest, NULL);
        DisplayErrorString("adding rights id", error);
        return FLC_FALSE;
    }
    */

    /*---------------------------------------------------------------------*/
    /* Set one or more optional desired features (name, version, and       */
    /* count) in the capability request. (Desired features are often used  */
    /* with a local license server.) When used in conjunction with an      */
    /* incremental capability request (see the API documentation for       */
    /* FlcCapabilityRequestSetIncremental), the desired feature count      */
    /* specified is a request for either an addition to (positive count    */
    /* value) or a substraction from (negative count value) the current    */
    /* served count for the specified feature information.                 */
    /*---------------------------------------------------------------------*/
    /*
    if (!FlcCapabilityRequestAddDesiredFeature(licensing,
                                                capabilityRequest,
                                                "survey",
                                                "1.0",
                                                1,
                                                error) ||
        !FlcCapabilityRequestAddDesiredFeature(licensing,
                                               capabilityRequest,
                                               "highres",
                                               "1.0",
                                               1,
                                               error))
    {
        (void)FlcCapabilityRequestDelete(licensing, &capabilityRequest, NULL);
        DisplayErrorString("adding desired features", error);
        return FLC_FALSE;
    }
    */



    /*---------------------------------------------------------------------*/
    /* Optional operation to preview a capability request.                 */
    /* Use FlcCapabilityRequestAddDesiredFeature to preview details for a  */
    /* specific feature name, version and count.                           */
    /* Use FlcCapabilityRequestSetRequestAllFeatures to request a preview  */
    /* of all features potentially served by the target server.            */
    /*                                                                     */
    /* Caution: You may not specify desired features and also set          */
    /* 'request all features' to FLC_TRUE on the same preview capability   */
    /* request.                                                            */
    /*---------------------------------------------------------------------*/
    /*
    if (!FlcCapabilityRequestSetOperation(licensing, 
                                          capabilityRequest,
                                          FLC_CAPABILITY_REQUEST_OPERATION_PREVIEW,
                                          error))
    {
        (void)FlcCapabilityRequestDelete(licensing, &capabilityRequest, NULL);
        DisplayErrorString("setting operation to preview", error);
        return FLC_FALSE;
    }

    if (!FlcCapabilityRequestAddDesiredFeature(licensing, 
                                               capabilityRequest,
                                               "f1", "1.0", 5, error))
    {
        (void)FlcCapabilityRequestDelete(licensing, &capabilityRequest, NULL);
        DisplayErrorString("adding desired preview feature", error);
        return FLC_FALSE;
    }

    if (!FlcCapabilityRequestSetRequestAllFeatures(licensing,
                                                   capabilityRequest,
                                                   FLC_TRUE, error))
    {
        (void)FlcCapabilityRequestDelete(licensing, &capabilityRequest, NULL);
        DisplayErrorString("setting preview features to all", error);
        return FLC_FALSE;
    }

    processResponse = FLC_FALSE;
    showFeatures = FLC_TRUE;
    acquireLicense = FLC_FALSE;
    /*

    /*---------------------------------------------------------------------*/
    /* Set one or more optional vendor dictionary name and value pairs.    */
    /*---------------------------------------------------------------------*/

    /*
    if (!FlcCapabilityRequestAddVendorDictionaryIntItem(licensing,
                                                        capabilityRequest,
                                                        "SomeInteger",
                                                        99,
                                                        error) ||
        !FlcCapabilityRequestAddVendorDictionaryStringItem(licensing,
                                                          capabilityRequest,
                                                          "SomeString",
                                                          "Anything",
                                                          error))
    {
        (void)FlcCapabilityRequestDelete(licensing, &capabilityRequest, NULL);
        DisplayErrorString("adding vendor dictionary items", error);
        return FLC_FALSE;
    }
    */


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
static
FlcBool
sProcessCapabilityResponse(
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

    if (!licensing || !responseAccepted || !error ||
        (responseSize && !response))
    {
        DisplayErrorString("processing capability response", NULL);
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
    /* process capability response into the trusted storage license source */ 
    /* and create capability response object from binary response          */
    /*---------------------------------------------------------------------*/
    if (!FlcProcessCapabilityResponseData(
             licensing, &capabilityResponse, response, responseSize, error))
    {
        DisplayErrorString("processing capability response", error);
        switch (FlcErrorGetCode(error))
        {
            case FLCERR_RESPONSE_STALE:
            case FLCERR_RESPONSE_EXPIRED:
            case FLCERR_PREVIEW_RESPONSE_NOT_PROCESSED:
                return FLC_TRUE;
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
    else
    {
        DisplayInfoString("processed capability response");
    }

    /*---------------------------------------------------------------------*/
    /* get capability response details                                     */
    /*---------------------------------------------------------------------*/
    if (!sGetResponseDetails(capabilityResponse, error))
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
sDisplayCapabilityResponseFeatures(
    FlcLicensingRef licensing,
    FlcUInt8 *      response,
    FlcSize         responseSize,
    FlcErrorRef     error)
/*=========================================================================*/
{
    FlcCapabilityResponseRef capabilityResponse = NULL;
    FlcStatusCollectionRef   errorStatusCollection = NULL;
    FlcFeatureCollectionRef  features  = NULL;
    FlcFeatureRef            feature   = NULL;
    FlcUInt32                size      = 0;
    FlcUInt32                index     = 0;
    FlcBool                  isPreview = FLC_FALSE;
    char                    msg[128]  = {0};


    if (!licensing || !error || (responseSize && !response))
    {
        DisplayErrorString("displaying capability response features", NULL);
        return FLC_FALSE;
    }

    if (responseSize == 0)
    {
        DisplayErrorString("missing capability response", NULL);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* create a capability response object                                 */
    /*---------------------------------------------------------------------*/
    if (!FlcCapabilityResponseCreate(
             licensing, &capabilityResponse, response, responseSize, error))
    {
        DisplayErrorString("creating capability response", error);
        if (FlcGetLastErrorStatusCollection(licensing, &errorStatusCollection, error) &&
            errorStatusCollection)
        {
            sDisplayErrorStatusInfo(errorStatusCollection, error);
        }
        return FLC_FALSE;
    }

    if (!FlcCapabilityResponseIsPreview(capabilityResponse, &isPreview, error))
    {
        DisplayErrorString("obtaining capability response type", error);
        (void)FlcCapabilityResponseDelete(licensing, &capabilityResponse, NULL);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* get capability response details                                     */
    /*---------------------------------------------------------------------*/
    if (!sGetResponseDetails(capabilityResponse, error))
    {
        (void)FlcCapabilityResponseDelete(licensing, &capabilityResponse, NULL);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* display the capability response feature information                 */
    /*---------------------------------------------------------------------*/
    if (!FlcCapabilityResponseGetFeatureCollection(capabilityResponse, &features, error))
    {
        DisplayErrorString("obtaining capability response feature collection", error);
        (void)FlcCapabilityResponseDelete(licensing, &capabilityResponse, NULL);
        return FLC_FALSE;
    }

    DisplayInfoString("==============================================");
    sprintf(msg, "Features found in%s capability response:\n", isPreview ? " preview" : ""); 
    DisplayInfoString(msg);

    if (!FlcFeatureCollectionSize(features, &size, error))
    {
        DisplayErrorString("getting feature collection size", error);
        return FLC_FALSE;
    }

    for (index = 0; index < size; index++)
    {
        if (!FlcFeatureCollectionGet(features, &feature, index, error))
        {
            DisplayErrorString("getting feature from collection", error);
            return FLC_FALSE;
        }

        if (!sPrintFeatureAttributes(feature, index, error))
        {
            DisplayErrorString("getting feature info", error);
            return FLC_FALSE;
        }
    }

    /*---------------------------------------------------------------------*/
    /* delete capability response object                                   */
    /*---------------------------------------------------------------------*/
    if (!FlcCapabilityResponseDelete(licensing, &capabilityResponse, error))
    {
        DisplayErrorString("deleting capability response object", error);
        return FLC_FALSE;
    }

    return FLC_TRUE;
}

/*==========================================================================*/
static FlcBool sPrintFeatureAttributes(FlcFeatureRef feature,
                                       FlcUInt32     index,
                                       FlcErrorRef   error)
/*==========================================================================*/
{
    char cmsg[1024]                = {0};

    const char * charPtr           = NULL;
    const char * namePtr           = NULL;
    const char * versionPtr        = NULL;
    int          count             = 0;
    int          maxCount          = 0;

    const struct tm * expiration   = NULL;
    const struct tm * issued       = NULL;
    const struct tm * startDate    = NULL;

    FlcBool   isPreview                = FLC_FALSE;
    FlcBool   isPerpetual              = FLC_FALSE;
    FlcBool   isMetered                = FLC_FALSE;
    FlcBool   isMeteredReusable        = FLC_FALSE;
    FlcUInt32 meteredUndoInterval      = 0;
    FlcInt32  meteredAvailableCount    = 0;

    char   countString[16]             = {0};
    char   maxCountString[16]          = {0};
    char   issuedDateString[32]        = {0};
    char   startDateString[32]         = {0};
    char   issuedString[64]            = {0};
    char   startString[64]             = {0};
    char   countInfoString[64]         = {0};
    char   expirationDateString[32]    = {0};
    char   expirationString[64]        = {0};   
    char   meteredString[64]           = {0};
    char   undoString[64]              = {0};
    char   vendorString[512]           = {0};
    char   issuerString[512]           = {0};
    char   noticeString[512]           = {0};
    char   serialNumberString[512]     = {0};

    char * featureInfo                = NULL;

    if (!feature)
    {
        DisplayErrorString("printing feature attributes", NULL);
        return FLC_FALSE;
    }

    /* get feature name and version */
    FlcFeatureGetName(feature, &namePtr, NULL);
    FlcFeatureGetVersion(feature, &versionPtr, NULL);

    /* translate count to string */
    FlcFeatureGetCount(feature, &count, NULL);
    count == FLC_FEATURE_UNCOUNTED_VALUE ? 
        sprintf(countString, "uncounted") : sprintf(countString, "%d", count);

    /* translate preview attributes to strings */
    FlcFeatureIsPreview(feature, &isPreview, NULL);
    if (isPreview)
    {
        FlcFeatureGetMaxCount(feature, &maxCount, NULL);
        maxCount == FLC_FEATURE_UNCOUNTED_VALUE ?
            sprintf(maxCountString, "uncounted") : sprintf(maxCountString, "%d", maxCount);
        sprintf(countInfoString, "TYPE=preview COUNT=%s MAXCOUNT=%s", countString, maxCountString); 
    }
    else
    {
        sprintf(countInfoString, "COUNT=%s", countString);
    }

    /* translate expiration date to string */
    FlcFeatureGetExpiration(feature, &expiration, NULL);
    (FlcDateIsPerpetual(expiration, &isPerpetual, NULL) && isPerpetual) ? 
        sprintf(expirationDateString, "permanent") : sDateToString(expiration, expirationDateString);
    sprintf(expirationString, "EXPIRATION=%s", expirationDateString);

    /* translate metered attributes to strings */
    FlcFeatureIsMetered(feature, &isMetered, NULL);
    FlcFeatureIsMeteredReusable(feature, &isMeteredReusable, NULL);
    FlcFeatureGetMeteredUndoInterval(feature, &meteredUndoInterval, NULL);
    if (isMetered)
    {
        FlcFeatureGetAvailableAcquisitionCount(feature, &meteredAvailableCount, NULL);
    }

    if (isMetered)
    {
        if (meteredUndoInterval)
        {
            sprintf(undoString, " UNDO_INTERVAL=%d", meteredUndoInterval);
        }
        sprintf(meteredString, "MODEL=metered%s%s", isMeteredReusable ? " REUSABLE" : "", meteredUndoInterval ? undoString : "");
    }

    /* prepend vendor string with the tag */
    charPtr = NULL;
    FlcFeatureGetVendorString(feature, &charPtr, NULL);
    if (charPtr)
    {
        sprintf(vendorString, "VENDOR_STRING=\"%s\"", charPtr);
    }

    /* prepend issuer with the tag */
    charPtr = NULL;
    FlcFeatureGetIssuer(feature, &charPtr, NULL);
    if (charPtr)
    {
        sprintf(issuerString, "ISSUER=\"%s\"", charPtr);
    }

    /* translate issued date to string */
    FlcFeatureGetIssued(feature, &issued, NULL);
    if (issued)
    {
        sDateToString(issued, issuedDateString);
        sprintf(issuedString, "ISSUED=%s", issuedDateString);
    }

    /* prepend notice with the tag */
    charPtr = NULL;
    FlcFeatureGetNotice(feature, &charPtr, NULL);
    if (charPtr)
    {
        sprintf(noticeString, "NOTICE=\"%s\"", charPtr);
    }

    /* prepend serial number with the tag */
    charPtr = NULL;
    FlcFeatureGetSerialNumber(feature, &charPtr, NULL);
    if (charPtr)
    {
        sprintf(serialNumberString, "SN=\"%s\"", charPtr);
    }

    /* translate start date to string */
    FlcFeatureGetStartDate(feature, &startDate, NULL);
    if (startDate)
    {
        sDateToString(startDate, startDateString);
        sprintf(startString, "START=%s", startDateString);
    }

    /* print feature info */
    sprintf(cmsg, "%d: %s %s %s %s",
              ++index,
              namePtr,
              versionPtr,
              countInfoString,
              expirationString);
    sAddFeatureInfo(cmsg, sizeof(cmsg), meteredString);
    sAddFeatureInfo(cmsg, sizeof(cmsg), vendorString);
    sAddFeatureInfo(cmsg, sizeof(cmsg), issuerString);
    sAddFeatureInfo(cmsg, sizeof(cmsg), issuedString);
    sAddFeatureInfo(cmsg, sizeof(cmsg), noticeString);
    sAddFeatureInfo(cmsg, sizeof(cmsg), serialNumberString);
    sAddFeatureInfo(cmsg, sizeof(cmsg), startString);

    featureInfo = (char *)malloc((FlcSize)(sizeof(char) * (strlen(cmsg) + 1)));
    if (!featureInfo)
    {
        return FLC_FALSE;
    }

    FlcCharToTChar((const FlcChar*)cmsg, featureInfo, strlen(cmsg) + 1);
    DisplayInfoString(featureInfo);
    free(featureInfo);

    return FLC_TRUE;
}

/*==========================================================================*/
static void sDateToString(const struct tm * date, char * string)
/*==========================================================================*/
{
    sprintf(string, "%d-%s-%d", date->tm_mday, months[date->tm_mon],date->tm_year + 1900);
}

/*==========================================================================*/
static void sAddFeatureInfo(char * buff, size_t size, const char *info)
/*==========================================================================*/
{
  if (info && strlen(info) > 0)
  {
    strncat(buff, " ", size);
    strncat(buff, info, size);
  }
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
    FlcBool                 boolValue       = FLC_FALSE;
    FlcUInt32               iii             = 0;
    char                   msg[1024]       = {0};
    const FlcChar *         formattedStatus = 0;


    if (!capabilityResponse  ||  !error)
    {
        DisplayErrorString("getting capability response details", 0);
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
        DisplayInfoString("no vendor dictionary in capability response");
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
      sprintf(msg, "number of status items in the response: %d", size);
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

    /*---------------------------------------------------------------------*/
    /* get confirmation request needed indicator                           */
    /*---------------------------------------------------------------------*/
    if (!FlcCapabilityResponseGetConfirmationRequestNeeded(capabilityResponse, &boolValue, error))
    {
        DisplayErrorString("getting confirmation request needed indicator", error);
        return FLC_FALSE;
    }

    if (boolValue == FLC_TRUE)
    {
        DisplayInfoString("confirmation request is needed");
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
static void sUsage(
    const char * applicationName)
/*=========================================================================*/
{
    char msg[1024] = {0};

    snprintf(msg, 1023,
        "\n%s [-generate outputfile]"
        "\n%s [-process inputfile]"
        "\n%s [-server url]"
        "\n\nwhere:"
        "\n-generate Generates capability request into a file."
        "\n-process  Processes capability response from a file."
        "\n-server   Sends request to a server and processes the response."
        "\n          For the test back-office server, use"
        "\n          http://hostname:8080/request."
        "\n          For FlexNet Operations, use"
        "\n          http://hostname:8888/flexnet/deviceservices."
        "\n          For FlexNet Embedded License Server, use"
        "\n          http://hostname:7070/fne/bin/capability."
        "\n          For Cloud License Server, use"
        "\n          https://<tenant>.compliance.flexnetoperations.com/instances/<instance-id>/request."
        "\n          For FNO Cloud, use"
        "\n          https://<tenant>.compliance.flexnetoperations.com/deviceservices.",
        applicationName, applicationName, applicationName);

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

