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
#include "FlcWindbackDetection.h"

#include "util/ErrDisplay.h"
#include "util/FileIO.h"
#include "identity/IdentityClient.h"

/* Use this to direct output to Windows MessageBox
#define UI_OUTPUT
*/

/****************************************************************************
    View.c

    This example program enables you to:
    1. View features presented in the provided buffer or served buffer
       license file and trusted storage.
    2. Find out the validity status of the features.
*****************************************************************************/

const static char * months[12] = { "jan", "feb", "mar", "apr",
                                   "may", "jun", "jul", "aug",
                                   "sep", "oct", "nov", "dec" };

/* identity warning message */
const char * const emptyIdentity = 
    "License-enabled code requires client identity data, " \
    "which you create with pubidutil and printbin -C. " \
    "See the User Guide for more information.";

const char * const invalidFile =
    "Specified file does not contain buffer or " \
    "served buffer license data.";

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
static FlcBool sValidateArgs(
    int     argc,
    char * argv[]);

/**
 *    Example program usage
 */
static void sUsage(
    const char * applicationName);

/**
 *    Cleanup processing
 */
static FlcBool sCleanup(
    FlcLicensingRef * licensing,
    FlcErrorRef *     error);

/**
 *    Display feature collection entries
 */
static FlcBool sGetFeatures(
    FlcFeatureCollectionRef featureCollection, 
    FlcErrorRef             error);

/**
 *    Display the attributes of an individual feature
 */
static FlcBool sPrintFeatureAttributes(
    FlcFeatureRef feature,
    FlcUInt32     index,
    FlcErrorRef   error);

/**
 *    Display vendor dictionary entries
 */
static FlcBool sGetVendorDictionary(
    FlcDictionaryRef        dictionary,
    FlcErrorRef             error);

/**
 *    Display the a vendor dictionary item
 */
static FlcBool sPrintVendorDictionaryItem(
    FlcDictionaryRef    dictionary,
    FlcUInt32           index,
    FlcErrorRef         error);

/**
 *    Format feature host ids
 */
static FlcBool sHostIdString(
    FlcHostIdsRef   hostIds,
    char **         string,
    FlcErrorRef     error);

/**
 *    Utility functions
 */
static void sDateToString(const struct tm * date,  char * string);
static void sAddFeatureInfo(char * buff, size_t size, const char *info);


/*=========================================================================*/
int main(int argc, char * argv[])
/*=========================================================================*/
{
    /* licensing environment object */
    FlcLicensingRef licensing   = NULL;

    /* error object */
    FlcErrorRef error            = NULL;

    /* feature collection */
    FlcFeatureCollectionRef featureCollection = NULL;

    /* vendor dictionary */
    FlcDictionaryRef vendorDictionary = NULL;

    /* provided file type */
    FlcMessageType licenseDataType = FLC_MESSAGE_TYPE_UNKNOWN;

    /* other */
    char       msg[512]        = {0};
    FlcChar     tsPath[512]     = {0};
    FlcBool     displayOK       = FLC_FALSE;
    FlcBool        isContainerized    = FLC_FALSE;

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
    /* read license file data                                              */
    /*---------------------------------------------------------------------*/
    if (inputFile)
    {
        if (!ReadFileData(inputFile, &buffer, &bufferSize))
        {
            (void)sCleanup(&licensing, &error);
            return -1;
        }
        sprintf(msg, "read data from %s", inputFile);
        DisplayInfoString(msg);
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
    /* Check if Containerized                                           */
    /*---------------------------------------------------------------------*/
    if (!FlcIsContainerized(licensing,&isContainerized,error))
    {
        DisplayErrorString("Checking if client is containerized", error);
        (void)sCleanup(&licensing, &error);
        return -1;
    }

    if (isContainerized)
    {
         DisplayInfoString("Client is contained in a docker host");
    }
    else
    {
         DisplayInfoString("Client is not contained in a docker host");
    }

    /*---------------------------------------------------------------------*/
    /* enable windback detection                                           */
    /*---------------------------------------------------------------------*/
    if (!FlcClockWindbackDetectionEnable(licensing, 86400, 86400, error))
    {
        DisplayErrorString("enabling clock windback detection", error);
        (void)sCleanup(&licensing, &error);
        return -1;
    }

    /*---------------------------------------------------------------------*/
    /* print buffer features info                                          */
    /*---------------------------------------------------------------------*/
    if (buffer && bufferSize)
    {
        if (!FlcGetMessageType(licensing, &licenseDataType, buffer, bufferSize, error))
        {
            DisplayErrorString("obtaining provided license file type", error);
            (void)sCleanup(&licensing, &error);
                return -1;
        }
        if (licenseDataType != FLC_MESSAGE_TYPE_BUFFER_LICENSE && 
            licenseDataType != FLC_MESSAGE_TYPE_BUFFER_CAPABILITY_RESPONSE)
        {
            DisplayErrorString(invalidFile, NULL);
            (void)sCleanup(&licensing, &error);
                return -1;
        }
        if (!FlcGetFeatureCollectionFromData(
                  licensing, &featureCollection, FLC_TRUE,
                  buffer, bufferSize, error))
        {
            DisplayErrorString("creating feature collection from the buffer", error);
            (void)sCleanup(&licensing, &error);
                return -1;
            }
        else 
        {
            DisplayInfoString("==============================================");
                sprintf(msg, "Features found in %s:\n", inputFile);
            DisplayInfoString(msg);
            displayOK = sGetFeatures(featureCollection, error);
            if (!FlcFeatureCollectionDelete(&featureCollection, displayOK ? error : NULL))
            {
                DisplayErrorString("deleting buffer feature collection", displayOK ? error : NULL);
                FlcErrorReset(error);
            }
            if (!displayOK)
            {
                (void)sCleanup(&licensing, &error);
                return -1;
            }
        }
    }

    /*---------------------------------------------------------------------*/
    /* print trusted storage features info                                 */
    /*---------------------------------------------------------------------*/
    featureCollection = NULL;
    if (!FlcGetTrustedStorageFeatureCollection(
              licensing, &featureCollection, FLC_TRUE, error))
    {
        DisplayErrorString("creating feature collection from trusted storage", error);
        (void)sCleanup(&licensing, &error);
        return -1;
    }
    else 
    {
        DisplayInfoString("==============================================");
        DisplayInfoString("Features found in trusted storage:\n");
        displayOK = sGetFeatures(featureCollection, error);
        if (!FlcFeatureCollectionDelete(&featureCollection, displayOK ? error : NULL))
        {
            DisplayErrorString("deleting trusted storage feature collection", displayOK ? error : NULL);
            FlcErrorReset(error);
        }
        if (!displayOK)
        {
            (void)sCleanup(&licensing, &error);
            return -1;
        }
    }

    /*---------------------------------------------------------------------*/
    /* print trials features info                                          */
    /*---------------------------------------------------------------------*/
    featureCollection = NULL;
    if (!FlcGetTrialFeatureCollection(
              licensing, &featureCollection, FLC_TRUE, error))
    {
        DisplayErrorString("creating feature collection from trials storage", error);
        (void)sCleanup(&licensing, &error);
        return FLC_FALSE;
    }
    else 
    {
        DisplayInfoString("==============================================");
        DisplayInfoString("Features found in trials storage:\n");
        displayOK = sGetFeatures(featureCollection, error);
        if (!FlcFeatureCollectionDelete(&featureCollection, displayOK ? error : NULL))
        {
            DisplayErrorString("deleting trials feature collection", displayOK ? error : NULL);
            FlcErrorReset(error);
        }
        if (!displayOK)
        {
            (void)sCleanup(&licensing, &error);
            return -1;
        }
    }

    
    DisplayInfoString("==============================================");


    if (!FlcGetTrustedStorageVendorDictionary(licensing, &vendorDictionary, error))
    {
        DisplayErrorString("obtaining trusted storage vendor dictionary", error);
        (void)sCleanup(&licensing, &error);
        return -1;
    }

    /*----------------------------------------------------------------------*/
    /* print trusted storage vendor dictionary info                         */
    /*----------------------------------------------------------------------*/
    if (vendorDictionary)
    {
        DisplayInfoString("==============================================");
        DisplayInfoString("Vendor dictionary items found in trusted storage:\n");
        displayOK = sGetVendorDictionary(vendorDictionary, error);
        if (!displayOK)
        {
            (void)sCleanup(&licensing, &error);
            return -1;
        }

        DisplayInfoString("==============================================");
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
            inputFile = argv[1];
        }
    }

    return FLC_TRUE;
}

/*==========================================================================*/
static void sUsage(
    const char * applicationName)
/*==========================================================================*/
{
    char msg[512] = {0};

    snprintf(msg, 511,
        "\n%s [binary_license_file]"
        "\nDisplays feature keyword information and validity based on contents "
        "\nof a binary license file and trusted storage, when available.",
        applicationName);

    DisplayMessage(msg, "USAGE");
}

/*==========================================================================*/
static
FlcBool
sCleanup(
    FlcLicensingRef * licensing,
    FlcErrorRef *     error)
/*==========================================================================*/
{
    FlcBool cleanupOK = FLC_TRUE;

    /*---------------------------------------------------------------------*/
    /* delete license data buffer                                          */
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

/*=========================================================================*/
static FlcBool sGetFeatures(FlcFeatureCollectionRef featureCollection,
                            FlcErrorRef             error)
/*=========================================================================*/
{
    FlcUInt32       size     = 0;
    FlcUInt32       index    = 0;
    FlcFeatureRef   feature  = NULL;

    /*---------------------------------------------------------------------*/
    /* get feature collection size                                         */
    /*---------------------------------------------------------------------*/
    if (!FlcFeatureCollectionSize(featureCollection, &size, error))
    {
        DisplayErrorString("getting feature collection size", error);
        return FLC_FALSE;
    }

    for (index = 0; index < size; index++)
    {
        if (!FlcFeatureCollectionGet(featureCollection, &feature, index, error))
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

    const struct tm * expiration   = NULL;
    const struct tm * issued       = NULL;
    const struct tm * startDate    = NULL;
    FlcHostIdsRef hostIds          = NULL;

    FlcBool   isPerpetual              = FLC_FALSE;
    FlcBool   isMetered                = FLC_FALSE;
    FlcBool   isMeteredReusable        = FLC_FALSE;
    FlcUInt32 meteredUndoInterval      = 0;
    FlcInt32  meteredAvailableCount    = 0;
    FlcInt32  status                   = 0; 

    char   countString[512]            = {0};
    char   expirationString[512]       = {0};
    char   meteredString[512]          = {0};
    char   undoString[256]             = {0};
    char   vendorString[512]           = {0};
    char   issuerString[512]           = {0};
    char   issuedDateString[512]       = {0};
    char   issuedString[512]           = {0};
    char   noticeString[512]           = {0};
    char   serialNumberString[512]     = {0};
    char   startDateString[512]        = {0};
    char   startString[512]            = {0};
    char * hostIdString                = NULL;

    char   statusString[512]          = {0};
    char   statusDesc[256]            = {0};
    char * featureInfo                = NULL;

    if (!feature)
    {
        DisplayErrorString("printing feature attributes", NULL);
        return FLC_FALSE;
    }

    /* get feature name and version */
    FlcFeatureGetName(feature, &namePtr, NULL);
    FlcFeatureGetVersion(feature, &versionPtr, NULL);

    /* translate expiration date to string */
    FlcFeatureGetExpiration(feature, &expiration, NULL);
    (FlcDateIsPerpetual(expiration, &isPerpetual, NULL) && isPerpetual) ? 
        sprintf(expirationString, "permanent") : sDateToString(expiration, expirationString);

    /* translate count to string */
    FlcFeatureGetCount(feature, &count, NULL);
    count == FLC_FEATURE_UNCOUNTED_VALUE ? 
        sprintf(countString, "uncounted") : sprintf(countString, "%d", count);

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

    /* get feature hostid(s) */
    FlcFeatureGetHostIds(feature, &hostIds, NULL);
    if (hostIds && !sHostIdString(hostIds, &hostIdString, error))
    {
        return FLC_FALSE;
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
              expirationString,
              countString);
    sAddFeatureInfo(cmsg, sizeof(cmsg), meteredString);
    sAddFeatureInfo(cmsg, sizeof(cmsg), hostIdString);
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

    FlcFeatureGetValidStatusForAcquisition(feature, &status, NULL);
    if (status == FLCERR_SUCCESS)
    {
        if (isMetered && !meteredAvailableCount)
        {
            DisplayInfoString("     Entire count consumed");
        }
        else if (isMetered && meteredAvailableCount)
        {
            sprintf(statusString, "     Available for acquisition: %d", meteredAvailableCount);
            DisplayInfoString(statusString);
        }
        else
        {
            DisplayInfoString("     Valid for acquisition");
        }
    }
    else
    {
        charPtr = FlcErrorCodeGetDescription(status);
        FlcCharToTChar(charPtr, statusDesc, sizeof(statusDesc)/sizeof(char));
        sprintf(statusString, "     Not valid for acquisition: %s", statusDesc);
        DisplayInfoString(statusString);
    }

    if (hostIdString)
    {
      free(hostIdString);
    }

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

/*==========================================================================*/
static FlcBool sHostIdString(FlcHostIdsRef  hostIds,
                             char **        string,
                             FlcErrorRef    error)
/*==========================================================================*/
{
    FlcUInt32         idCount = 0;
    FlcUInt32         index = 0;
    FlcInt32          idType = 0;
    const FlcChar *   idValue = NULL;
    FlcSize           size = 0, offset = 0;
    char *            temp = NULL;

    const char hostIdPrefix[][20] = {
                      "UNKNOWN",
                      "UNSUPPORTED",
                      "",/*LONG*/
                      "",/*ETHERNET*/
                      "",/*ANY*/
                      "USER=",
                      "DISPLAY=",
                      "HOSTNAME=",
                      "ID_STRING=",
                      "FLEXID=7-",
                      "DISK_SERIAL_NUM=",
                      "INTERNET=",
                      "INTERNET=",
                      "FLEXID=8-",
                      "FLEXID=9-",
                      "HOSTNAME=",
                      "FLEXID=6-",
                      "COMPOSITE=",
                      "VENDOR=",
                      "FLEXID=10-",
                      "VM_UUID=",
                      "AMZN_EIP=",
                      "AMZN_AMI=",
                      "TOLERANT=",
                      "AMZN_IID=",
                      "EXTENDED=",
                      "PUBLISHED_DEFINED=",
                      "CONTAINER_ID=" };
    FlcInt32 hostIdPrefixCount = (FlcInt32)sizeof(hostIdPrefix)/sizeof(hostIdPrefix[0]);

    if (!hostIds || !string)
    {
        DisplayErrorString("translating hostid(s) to string", NULL);
        return FLC_FALSE;
    }
    *string = NULL;
    FlcHostIdsGetIdCount(hostIds, &idCount, NULL);
    if (idCount <= 0)
    {
        return FLC_TRUE;
    }
    for (index = 0; index < idCount; index++)
    {
        FlcHostIdsGetId(hostIds, index, &idType, &idValue, NULL);
        /* add one for space */
        size += (FlcSize)(strlen(hostIdPrefix[idType]) + strlen(idValue) + 1);
    }
    size += 8;    /* "HOSTID=" plus null */
    if (idCount > 1)
    {
        size += 2; /* " " */
    }
    /* allocate buffer */
    temp = (char *)malloc(size);
    if (!temp)
    {
        DisplayErrorString("allocating memory", NULL);
        return FLC_FALSE;
    }
    /* copy data */
    strncpy(&temp[offset], "HOSTID=", strlen("HOSTID="));
    offset += (FlcSize)strlen("HOSTID=");
    if (idCount > 1)
    {
        temp[offset++] = '"';
    }
    for (index = 0; index < idCount; index++)
    {
        FlcHostIdsGetId(hostIds, index, &idType, &idValue, NULL);
        if (idType >= hostIdPrefixCount)
        {
          idType = 0;
        }
        /* prefix */
        strncpy(&temp[offset], hostIdPrefix[idType], strlen(hostIdPrefix[idType]));
        offset += (FlcSize)strlen(hostIdPrefix[idType]);
        /* value */
        strncpy(&temp[offset], idValue, strlen(idValue));
        offset += (FlcSize)strlen(idValue);
        /* space */
        temp[offset++] = ' ';
    }
    offset--;
    if (idCount > 1)
    {
      temp[offset++] = '"';
    }
    temp[offset] = '\0';
    *string = temp;
    return FLC_TRUE;
}

/*==========================================================================*/
static
FlcBool
sGetVendorDictionary(
    FlcDictionaryRef    dictionary,
    FlcErrorRef         error)
/*==========================================================================*/
{
    FlcUInt32   size     = 0;
    FlcUInt32   index    = 0;

    /*----------------------------------------------------------------------*/
    /* get vendor dictionary size                                           */
    /*----------------------------------------------------------------------*/

    if (!FlcDictionaryGetSize(dictionary, &size, error))
    {
        DisplayErrorString("getting vendor dictionary size", error);
        return FLC_FALSE;
    }

    /* print dictionary items */
    for (index = 0; index < size; index++)
    {
        if (!sPrintVendorDictionaryItem(dictionary, index, error))
        {
            DisplayErrorString("printing vendor dictionary item", error);
            return FLC_FALSE;
        }
    }

    /*----------------------------------------------------------------------*/
    return FLC_TRUE;
}

/*==========================================================================*/
static
FlcBool
sPrintVendorDictionaryItem(
    FlcDictionaryRef    dictionary,
    FlcUInt32           index,
    FlcErrorRef         error)
/*==========================================================================*/
{
    FlcDictionaryValueType valueType   = FLC_DICTIONARY_UNKNOWN_VALUE;
    const FlcChar *        key         = NULL;
    const FlcChar *        stringValue = NULL;
    FlcInt32               intValue    = 0;
    char                   cmsg[1024]  = {0};
    char                  tmsg[1024]  = {0};


    /*----------------------------------------------------------------------*/
    /* get the type of the dictionary item value                            */
    /*----------------------------------------------------------------------*/

    if (!FlcDictionaryGetValueType(dictionary, index, &valueType, error))
    {
        DisplayErrorString("getting item value type from vendor dictionary", error);
        return FLC_FALSE;
    }

    /*----------------------------------------------------------------------*/
    /* get key and value according to the type                              */
    /*----------------------------------------------------------------------*/
    switch (valueType)
    {
        case FLC_DICTIONARY_STR_VALUE:
            if (!FlcDictionaryGetStringItem(dictionary, index, &key, &stringValue, error))
            {
                DisplayErrorString("getting string item from vendor dictionary", error);
                return FLC_FALSE;
            }
            sprintf(cmsg, "%d: key=\"%s\"   value=\"%s\"", index+1, key, stringValue);
            break;

        case FLC_DICTIONARY_INT_VALUE:
            if (!FlcDictionaryGetIntItem(dictionary, index, &key, &intValue, error))
            {
                DisplayErrorString("getting integer item from vendor dictionary", error);
                return FLC_FALSE;
            }
            sprintf(cmsg, "%d: key=\"%s\"   value=%d", index+1, key, intValue);
            break;

        default:
            DisplayErrorString("unknown item value type from vendor dictionary", error);
            return FLC_FALSE;
    }

    /*----------------------------------------------------------------------*/
    /* print dictionary item                                                */
    /*----------------------------------------------------------------------*/

    FlcCharToTChar((const FlcChar*)cmsg, tmsg, sizeof(tmsg)/sizeof(char));
    DisplayInfoString(tmsg);

    return FLC_TRUE;
}
