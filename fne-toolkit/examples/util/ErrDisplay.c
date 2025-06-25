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

#if defined(_MSC_VER) && _MSC_VER >= 1900
#pragma comment(lib, "legacy_stdio_definitions.lib")
#endif

#include "ErrDisplay.h"

/* Global Buffers -- used for display only */
char msg[1024]     = {0};


void
FlcCharToTChar(
    const FlcChar *    data,
    char *            result,
    size_t             size)
{
    /* FlcChar is UTF-8 */
    if (!data || !result || !size)
    {
        return;
    }

    strncpy(result, data, size);
}

void
TCharToFlcChar(
    const char *    data,
    FlcChar *        result,
    size_t           size)
{
    /* FlcChar is UTF-8 */

    if (!data || !result || !size)
    {
        return;
    }

    strncpy(result, data, size);
}

/*==========================================================================*/
void
DisplayMessage(
    const char * msg,
    const char * caption)
/*==========================================================================*/
{
    if ( !msg )
    {
        return;
    }

#ifdef UI_OUTPUT
    MessageBox(NULL, msg, caption, 0);
#else /* !UI_OUTPUT */

#if defined(WIN32) || defined(_WIN32_WCE)
#ifdef _UNICODE
    _setmode(_fileno(stdout), _O_WTEXT);
#endif
    if(caption)
        _tprintf("%s: %s\n", caption, msg);
    else
        _tprintf("%s\n", msg);
#else
    if(caption)
        printf("%s: %s\n", caption, msg);
    else
        printf("%s\n", msg);
#endif

#endif /* UI_OUTPUT */
}

void DisplayErrorString(const char * msg,  FlcErrorRef error)
{
    char msg1[1600] = {0};
    char errorMessage[1600] = {0};
#ifdef _UNICODE    
    char tErrorMessage[1600] = {0};
#endif        
    FlcInt32 statCode = 0;

    /*----------------------------------------------------------------------*/
    if ( !msg )
    {
        return;
    }

    /*----------------------------------------------------------------------*/
    if (error)
    {
        if (FlcErrorGetCode(error) == FLCERR_RESPONSE_SERVER_ERROR)
        {
            statCode = FlcErrorGetSystemCode(error);
        }
        if (statCode)
        {
            sprintf(errorMessage, "%s\nServer status: (%s)",
                FlcErrorGetMessage(error),
                FlcBackOfficeErrorCodeGetDescription(statCode));
        }
        else
        {
            sprintf(errorMessage, "%s", FlcErrorGetMessage(error));
        }

#ifdef _UNICODE                
        FlcCharToTChar(errorMessage, tErrorMessage, strlen(errorMessage));
        sprintf(msg1, "%s: %s", msg, tErrorMessage);                
#else
        sprintf(msg1, "%s: %s", msg, errorMessage);                        
#endif        
                
    }
    else
    {
        sprintf(msg1, "%s", msg);
    }

    DisplayMessage(msg1, "ERROR");
}


void DisplayInfoString(const char * msg)
{
    DisplayMessage(msg, "INFO");
}


static char * months[12] = { "jan",
                        "feb",
                        "mar",
                        "apr",
                        "may",
                        "jun",
                        "jul",
                        "aug",
                        "sep",
                        "oct",
                        "nov",
                        "dec" };

char * GetMonth(int month)
{
    return months[month];
}

