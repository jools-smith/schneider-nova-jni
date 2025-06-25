/****************************************************************************
  Copyright (c) 2013-2020 Flexera Software LLC.
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
    UsageCaptureClient.c

    This example program illustrates how to:

    1. Send a capability request describing a usage-capture report,
       request, or undo operation to a server.
    2. Depending on the operation type, process the response.

    These communications can be performed directly over HTTP or using
    intermediate files for offline operations.

*****************************************************************************/

/* identity warning message */
const char * const emptyIdentity = 
    "License-enabled code requires client identity data, " \
    "which you create with pubidutil and printbin -C. " \
    "See the User Guide for more information.";

/* server parameters */
static char *     serverUrl       = NULL;

/* file names */
static char *     inputFile      = NULL;
static char *     outputFile     = NULL;

/* other input value */
static char *     correlationId  = NULL;
static char *     rightsId       = NULL;

/* functionality */
FlcCapabilityRequestOperation operation = FLC_CAPABILITY_REQUEST_OPERATION_UNKNOWN;
static char *     operationType     = "";
static FlcBool     generateRequest   = FLC_FALSE;
static FlcBool     storeRequest      = FLC_FALSE;
static FlcBool     processResponse   = FLC_FALSE;
static FlcBool     readResponse      = FLC_FALSE;
static FlcBool     talkToServer      = FLC_FALSE;
static FlcBool     acquireLicense    = FLC_FALSE;
static FlcBool     errorResponseOnly = FLC_FALSE;

/* capability request and response binary buffers */
static FlcUInt8 *  binaryRequest         = NULL;
static FlcSize     binaryRequestSize     = 0;
static FlcUInt8 *  binaryResponse        = NULL;
static FlcSize     binaryResponseSize    = 0;

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
    const char *       serverUrl,
    const FlcUInt8 *    request,
    FlcSize             requestSize,
    FlcUInt8 **         response,
    FlcSize *           responseSize,
    FlcErrorRef         error);

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
 *    Get capability response details
 */
static FlcBool sGetResponseDetails(
    FlcCapabilityResponseRef    capabilityResponse,
    FlcErrorRef                 error);

/**
 *    Display error status collection
 */
static void sDisplayErrorStatusInfo(
    FlcStatusCollectionRef    errorStatusCollection,
    FlcErrorRef               error);

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
      sUsage(argv[0]);
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
        sprintf(msg, "generated %scapability request", operationType);
        DisplayInfoString(msg); 
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
        sprintf(msg, "successfully sent capability request to %s%s", serverUrl,
                       (binaryResponse && (binaryResponseSize > 0)) ?
                       " - and received response" : "");
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
    /* process capability response                                         */
    /*---------------------------------------------------------------------*/
    if (processResponse &&
        (!errorResponseOnly || (binaryResponse && binaryResponseSize > 0)))
    {
        if (!sProcessCapabilityResponse(
                licensing, binaryResponse, binaryResponseSize,
                forceResponse, &responseAccepted, error))
        {
            (void)sCleanup(&licensing, &error);
            return -1;
        }

        if (responseAccepted)
        {
            DisplayInfoString("processed capability response");
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
    FlcBool   rc = FLC_TRUE, unknownArg = FLC_FALSE;
    FlcInt32  ii = 0;
    char *   arg = NULL;  
    char     msg[512] = {0};

      if (argc == 1)
      {
            return FLC_FALSE;
      }

      for (ii = 1; rc && ii < argc; ii++)
      {
        arg = argv[ii];
        if (strcmp("-help", arg) == 0 ||
            strcmp("-h",    arg) == 0)
        {
          return FLC_FALSE;
        }
        else if (strcmp("-request", arg) == 0)
        {
            operation = FLC_CAPABILITY_REQUEST_OPERATION_REQUEST;
            operationType = "";
        }
        else if (strcmp("-report", arg) == 0) 
        {
            operation = FLC_CAPABILITY_REQUEST_OPERATION_REPORT;
            operationType = "report ";
        }
        else if (strcmp("-undo", arg) == 0) 
        {
            operation = FLC_CAPABILITY_REQUEST_OPERATION_UNDO;
            operationType = "undo ";
        }
        else if (strcmp("-correlation", arg) == 0) 
        {
            rc = (argc <= ii + 1) ? FLC_FALSE : FLC_TRUE;
            if (rc)
            {
                correlationId = argv[++ii];
            }
        }
        else if (strcmp("-rightsid", arg) == 0) 
        {
            rc = (argc <= ii + 1) ? FLC_FALSE : FLC_TRUE;
            if (rc)
            {
                rightsId = argv[++ii];
            }
        }
        else if (strcmp("-server", arg) == 0) 
        { /* should be followed by 1 parameters for server url */
            rc = (argc <= ii + 1) ? FLC_FALSE : FLC_TRUE;
            if (rc)
            {
                serverUrl       = argv[++ii];
                generateRequest = FLC_TRUE;
                talkToServer    = FLC_TRUE;
                processResponse = FLC_TRUE;
            }
        }
        else if (strcmp("-generate", arg) == 0) 
        { /* should be followed by 1 parameter for output filename */
            rc = (argc <= ii + 1) ? FLC_FALSE : FLC_TRUE;
            if (rc)
            {
                outputFile      = argv[++ii];
                generateRequest = FLC_TRUE;
                storeRequest    = FLC_TRUE;
            }
        }
        else if (strcmp("-process", arg) == 0) 
        { /* should be followed by 1 parameter for input filename */
            rc = (argc <= ii + 1) ? FLC_FALSE : FLC_TRUE;
            if (rc)
            {
                inputFile       = argv[++ii];
                readResponse    = FLC_TRUE;
                processResponse = FLC_TRUE;
            }
        }
        else
        {
            rc = FLC_FALSE;
            unknownArg = FLC_TRUE;
        }
        if (!rc)
        {
            if (unknownArg)
            {
              sprintf(msg, "unknown option: %s", arg);
            }
            else
            {
              sprintf(msg, "invalid specification for option: %s", arg);
            }
            DisplayErrorString(msg, NULL);
        }
    }

    if (rc)
    {
        /* validate argument combinations */
        /* Undo operation must have a correlation Id */
        if (generateRequest && operation == FLC_CAPABILITY_REQUEST_OPERATION_UNDO && !correlationId) 
        {
            DisplayErrorString("undo option requires correlation ID", NULL);
            rc = FLC_FALSE;
        }
        else if (generateRequest && operation == FLC_CAPABILITY_REQUEST_OPERATION_REPORT && rightsId)
        {
            DisplayErrorString("report option is not compatible with rightsid option", NULL);
            rc = FLC_FALSE;
        }
        else if (readResponse &&
                 (operation != FLC_CAPABILITY_REQUEST_OPERATION_UNKNOWN || correlationId || rightsId))
        {
            DisplayErrorString("process option is not compatible with request attributes", NULL);
            rc = FLC_FALSE;
        }
        else if (!generateRequest && !readResponse && !talkToServer)
        {
            rc = FLC_FALSE;
        }
        else
        {
            if (generateRequest)
            {
                if (operation == FLC_CAPABILITY_REQUEST_OPERATION_UNKNOWN)
                {
                    operation = FLC_CAPABILITY_REQUEST_OPERATION_REQUEST;
                }
                else if (operation == FLC_CAPABILITY_REQUEST_OPERATION_REPORT)
                {
                    /* REPORT operation does not get a response unless there's an error */
                    errorResponseOnly = talkToServer;
                }
            }
            acquireLicense = processResponse && !errorResponseOnly;
        }
    }
    return rc;
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
    char      tmsg[512]   = {0};
    char *    tCid        = correlationId;
    FlcChar    buff[512]   = {0};
    char      tbuff[512]  = {0};


    FlcCapabilityRequestRef capabilityRequest = NULL;

    if (!licensing || !request || !requestSize || !error)
    {
        DisplayErrorString("generating capability request", NULL);
        return FLC_FALSE;
    }
    *request     = NULL;
    *requestSize = 0;

    /*---------------------------------------------------------------------*/
    /* Create capability request object                                    */
    /*---------------------------------------------------------------------*/
    if (!FlcCapabilityRequestCreate(licensing,
                                    &capabilityRequest,
                                    error))
    {
        DisplayErrorString("creating capability request object", error);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* Set the capability request user specifier correlation ID .          */                                                       
    /*---------------------------------------------------------------------*/
    if (tCid)
    {
        TCharToFlcChar(tCid, buff, sizeof(buff));
        if (!FlcCapabilityRequestSetCorrelationId(licensing, 
                                                  capabilityRequest,
                                                  buff, 
                                                  error))
        {
            DisplayErrorString("setting capability request correlation id", error);
            (void)FlcCapabilityRequestDelete(licensing, &capabilityRequest, NULL);
            return FLC_FALSE;
        }
        sprintf(tmsg, "capability request correlation ID: %s", tCid);
        DisplayInfoString(tmsg);
    }
    
    /*---------------------------------------------------------------------*/
    /* Set the rights ID if specified. Note that rights ID specification   */
    /* is incompatible with a report type capability request.              */
    /*---------------------------------------------------------------------*/
    if (operation != FLC_CAPABILITY_REQUEST_OPERATION_REPORT && rightsId)
    {
        TCharToFlcChar(rightsId, buff, sizeof(buff));
        if (!FlcCapabilityRequestAddRightsId(licensing, 
                                             capabilityRequest, 
                                             buff, 
                                             1,
                                             error))
        {
            DisplayErrorString("adding rights ID to the capability request", error);
            (void)FlcCapabilityRequestDelete(licensing, &capabilityRequest, NULL);
            return FLC_FALSE;
        }
    }

    /*---------------------------------------------------------------------*/
    /* Set the capability request operation type                           */
    /*---------------------------------------------------------------------*/
    if (!FlcCapabilityRequestSetOperation(licensing,
                                          capabilityRequest,
                                          operation, 
                                          error))
    {
        DisplayErrorString("setting capability request operation", error);
        (void)FlcCapabilityRequestDelete(licensing, &capabilityRequest, NULL);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* set Requestor ID, Acquisition ID and Enterprise ID                  */
    /*---------------------------------------------------------------------*/
    if (!FlcCapabilityRequestSetRequestorId(licensing, capabilityRequest, "Example Application", error)           ||
        !FlcCapabilityRequestSetAcquisitionId(licensing, capabilityRequest, "High resolution surveying", error) ||
        !FlcCapabilityRequestSetEnterpriseId(licensing, capabilityRequest, "5551212", error))
    {
        DisplayErrorString("setting capability request id values", error);
        (void)FlcCapabilityRequestDelete(licensing, &capabilityRequest, NULL);
        return FLC_FALSE;
    }

    if (operation != FLC_CAPABILITY_REQUEST_OPERATION_UNDO)
    {
        /* specify usage data; to return a metered feature, set the count to a negative value */
        if (!FlcCapabilityRequestAddDesiredFeature(licensing, 
                                                   capabilityRequest,
                                                   "survey", "1.0", 1,
                                                   error))
        {
            DisplayErrorString("adding capability request desired feature", error);
            (void)FlcCapabilityRequestDelete(licensing, &capabilityRequest, NULL);
            return FLC_FALSE;
        }
    }

    /*---------------------------------------------------------------------*/
    /* Set force response flag - optional                                  */
    /*---------------------------------------------------------------------*/
    if (!FlcCapabilityRequestSetForceResponse(licensing,
                                              capabilityRequest,
                                              forceResponse,
                                              error))
    {
        DisplayErrorString("setting capability request force response", error);
        (void)FlcCapabilityRequestDelete(licensing, &capabilityRequest, NULL);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* generate binary capability request                                  */
    /*---------------------------------------------------------------------*/
    if (!FlcCapabilityRequestGenerate(licensing,
                                      capabilityRequest, 
                                      request,
                                      requestSize,
                                      error))
    {
        DisplayErrorString("generating binary capability request", error);
        (void)FlcCapabilityRequestDelete(licensing, &capabilityRequest, NULL);
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
    /* return all non-metered licenses and release all metered licenses    */
    /*---------------------------------------------------------------------*/
    if (!FlcReturnAllLicenses(licensing, error))
    {
        DisplayErrorString("returning survey license", error);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* cleanup license storage                                             */
    /*---------------------------------------------------------------------*/
    FlcLicenseDelete(&license, NULL);
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

    FlcDictionaryRef        dictionary      = NULL;
    FlcStatusCollectionRef  status          = NULL;    
    FlcUInt32               size            = 0;
    FlcDictionaryValueType  valueType       = FLC_DICTIONARY_UNKNOWN_VALUE;
    const FlcChar *         key             = NULL;
    const FlcChar *         stringValue     = NULL;
    const FlcChar *         cCid            = NULL;
    FlcInt32                intValue        = 0;
    FlcUInt32               iii             = 0;
    const FlcChar *         formattedStatus = NULL;
    char                   msg[1024]       = {0};
    char                   tbuff[512]      = {0};


    if (!capabilityResponse  ||  !error)
    {
        DisplayErrorString("getting capability response details", 0);
        return FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* get correlation id from response                                    */
    /*---------------------------------------------------------------------*/
    if (!FlcCapabilityResponseGetCorrelationId(capabilityResponse, &cCid, error))
    {
        DisplayErrorString("getting capability response correlation id", 0);
        return FLC_FALSE;
    }
    else if (cCid)
    {
        FlcCharToTChar(cCid, tbuff, sizeof(tbuff)/sizeof(char));    
        sprintf(msg, "capability response correlation ID: %s", tbuff);
        DisplayInfoString(msg);
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

/*==========================================================================*/
static
void
sUsage(
    const char * applicationName)
/*==========================================================================*/
{
    char msg[1024] = {0};
    snprintf(msg, 1023,
        "\n%s [-request|-report|-undo]  [-correlation id] [-rightsid rights-id]"
        "\n          [-server url|-generate outputfile|-process inputfile]"
        "\n\nwhere:"
        "\n-request  Request operation.  This is the default operation type if unspecified."
        "\n-report   Report operation.  Incompatible with -rightsid specification."
        "\n-undo     Undo operation.  Correlation ID must be set with -correlation."
        "\n"
        "\n-correlation  Sets the correlation ID."
        "\n"
        "\n-rightsid     Sets the rights ID."
        "\n"
        "\n-server   Sends usage-capture message to a server and processes the response."
        "\n-generate Generates usage-capture message into a file."
        "\n-process  Processes server's response from a file.",
        applicationName);

    DisplayMessage(msg, "USAGE");
}

/*=========================================================================*/
FlcBool
TalkToServer(
    const char *       serverUrl,
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
    TCharToFlcChar(serverUrl, url, sizeof(url));
    rc = FlcCommSetServer(commInterface, url, error) &&
         FlcCommSendBinaryMessage(commInterface, (void *)request, requestSize, (void **)response, responseSize, error);
    (void)FlcCommDelete(&commInterface, 0);
    return rc;
}

