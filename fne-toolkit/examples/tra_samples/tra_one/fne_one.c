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

#include <string.h>

#if defined(_MSC_VER) && _MSC_VER >= 1900
#pragma comment(lib, "legacy_stdio_definitions.lib")
#endif

#include "FlcLicenseManager.h"
#include "util/ErrDisplay.h"

#include <user_data.h>
#include <tra.h>
#include <tra_one_declarative_data.h>

const char * hostid = "1234567890";

static FlcBool sCleanup(FlcLicensingRef * licensing, FlcErrorRef * error);
static void sDisplayErrorStringA( const char * msg, FlcErrorRef error);
static void sDisplayInfoStringA( const char * msg);

/*==============================================================*/
int load_fne(USER_DATA * pud)
/*==============================================================*/
{
    int reply = -1;
    int len = 0;
    const FlcUInt8 * identity_data;
    if (!pud)
    {
        return reply;
    }
    /*----------------------------------------------------------------------*/
    /* create error object                                                  */
    /*----------------------------------------------------------------------*/
    pud->flx_error = NULL;
    pud->flx_licensing = NULL;
    if ( ! FlcErrorCreate(&pud->flx_error) )
    {
        sDisplayErrorStringA( tra_get_string( pud->T, TRA_STRING_creating_error_object_ALIAS_1 ), NULL );
        return reply;
    }
    /*----------------------------------------------------------------------*/
    /* create licensing environment                                         */
    /*----------------------------------------------------------------------*/
    identity_data = (const FlcUInt8 *)tra_get_lstring( pud->T, TRA_STRING_IDENTITY_ALIAS_1, &len );
    if (!FlcLicensingCreate(
                            &pud->flx_licensing, identity_data, len, NULL, tra_get_string( pud->T, TRA_STRING__1234567890_ALIAS_1 ), pud->flx_error))
    {
        const char * msg = tra_get_string(pud->T, TRA_STRING_creating_licensing_object_ALIAS_1);
        sDisplayErrorStringA( msg, pud->flx_error);
        return reply;
    }
    else
    {
        reply = 0;
    }
    return reply;
}
/*==============================================================*/
void unload_fne(USER_DATA * pud)
/*==============================================================*/
{
    /*----------------------------------------------------------------------*/
    /* cleanup FNE                                                          */
    /*----------------------------------------------------------------------*/
    (void)sCleanup( &pud->flx_licensing, &pud->flx_error );

}
/*==============================================================*/
int is_licensed(USER_DATA * pud)
/*==============================================================*/
{
    int reply = FlcAddBufferLicenseSourceFromFile(
        pud->flx_licensing,
        tra_get_string( pud->T, TRA_STRING_license_bin_ALIAS_2 ),
        NULL,
        pud->flx_error );
    /*----------------------------------------------------------------------*/
    /* create buffer license source from file                               */
    /*----------------------------------------------------------------------*/
    if ( ! reply)
    {
        if( FlcErrorGetCode( pud->flx_error ) == FLCSWC_FILE_OPEN )
        {
            sDisplayInfoStringA( tra_get_string( pud->T, TRA_STRING_license_bin_not_found_ALIAS_1 ) );
        }
        else
        {
            sDisplayErrorStringA( tra_get_string( pud->T, TRA_STRING_adding_buffer_license_source_ALIAS_1 ), pud->flx_error );
        }
    }
    /*Enable the hires TDT*/
    pud->error = tra_call(pud->T, TRA_FUNCTION_set_fne_license_OK_ALIAS_1, NULL, reply, 0, NULL );
    return reply;
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
        sDisplayErrorStringA("deleting licensing object", error ? *error : NULL);
        cleanupOK = FLC_FALSE;
    }

    /*---------------------------------------------------------------------*/
    /* delete error object                                                 */
    /*---------------------------------------------------------------------*/
    if (error && *error && !FlcErrorDelete(error))
    {
        sDisplayErrorStringA("deleting error object", NULL);
        cleanupOK = FLC_FALSE;
    }

    if (cleanupOK)
    {
        sDisplayInfoStringA("deleted licensing environment");
    }

    return cleanupOK;
}

static void sDisplayErrorStringA( const char * msg, FlcErrorRef error)
{
    if (msg)
    {
#ifdef _UNICODE
        char msg1[1600] = {0};
        FlcCharToTChar(msg, msg1, strlen(msg));
        DisplayErrorString(msg1,error);
        _setmode(_fileno(stdout), _O_TEXT);
#else
        DisplayErrorString(msg,error);
#endif
    }
}
static void sDisplayInfoStringA( const char * msg)
{
    if (msg)
    {
#ifdef _UNICODE
        char msg1[1600] = {0};
        FlcCharToTChar(msg, msg1, strlen(msg));
        DisplayInfoString(msg1);
        _setmode(_fileno(stdout), _O_TEXT);
#else
        DisplayInfoString(msg);
#endif
    }
}



