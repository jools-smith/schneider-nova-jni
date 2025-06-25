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
#include <iostream>
#include <sstream>

#if defined(_MSC_VER) && _MSC_VER >= 1900
#pragma comment(lib, "legacy_stdio_definitions.lib")
#endif

#include "FlcError.h"
#include "FlcLicensing.h"
#include "FlcLicenseManager.h"

#include <tra.h>
#include "calc_declarative_data.h"

#if _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif

typedef struct USER_DATA
{
    tra_State * T;
    FlcErrorRef * error;
    FlcLicensingRef * licensing;
    FlcLicenseRef license;
    std::string license_file_path;
    TFT check;
    TFT pid;
}USER_DATA;

namespace
{
/*==========================================================================*/
void sDisplayMessage(const std::string & msg, const std::string & caption)
/*==========================================================================*/
{
    if ( !msg.empty()  && !caption.empty() )
    {
#ifdef UI_OUTPUT
        MessageBoxA(NULL, msg.c_str(), caption.c_str(), 0);
#else /* !UI_OUTPUT */
        std::cout << caption << ": " << msg << "\n";
#endif /* UI_OUTPUT */
    }
}

/*==========================================================================*/
void sDisplayErrorString(const std::string & msg, FlcErrorRef error)
/*==========================================================================*/
{
    std::ostringstream errorMessage;
    errorMessage << msg;
    if (error)
    {
        errorMessage << " " << FlcErrorGetMessage(error);
    }
    sDisplayMessage(errorMessage.str(), "ERROR");
}

/*==========================================================================*/
void sDisplayInfoString( const std::string & msg)
/*==========================================================================*/
{
    sDisplayMessage( msg, "INFO" );
}

/*==========================================================================*/
FlcBool sCleanup(FlcLicensingRef * licensing, FlcErrorRef * error)
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
void sUsage(tra_State * T, const std::string& applicationName, const std::string& fileName)
/*==========================================================================*/
{
    std::ostringstream msg;
    msg << applicationName;
    msg << tra_get_string( T, TRA_STRING_usage_ALIAS_1 );
    msg << fileName;
    msg << tra_get_string( T, TRA_STRING_new_lines_ALIAS_1 );

    sDisplayMessage( msg.str(), tra_get_string(T, TRA_STRING_USAGE_ALIAS_1) );
}

/*==========================================================================*/
int get_random_number()
/*==========================================================================*/
{
#ifdef _WIN32
    return (int)GetCurrentProcessId();
#else
    return getpid();
#endif
}

} /* end of anon namespace */

/*----------------------------------------------------------------------*/
/* the SNIF predicate acquire_highres will have prepared the values        */
/* based on the outcome of the acquire                                    */
/*----------------------------------------------------------------------*/
template < typename Ty >
int adjust( USER_DATA * pud )
{
    return pud->check;
}

template <>
int adjust< double >( USER_DATA * pud )
{
    return pud->check - pud->pid;
}

template < typename Ty >
bool is_basic()
{
    return true;
}

template <>
bool is_basic< double >()
{
    return false;
}

/*----------------------------------------------------------------------*/
/* Ty will be an int for lowres and double for highres                  */
/*----------------------------------------------------------------------*/
template< typename Ty >
void calc_loop( USER_DATA * pud )
{
    if ( !pud )
    {
        return;
    }
    TFT zero_1( pud->T, TRA_VARIABLE_ZERO_ALIAS_1 );
    TFT zero_2( pud->T, TRA_VARIABLE_ZERO_ALIAS_2 );
    TFT zero_3( pud->T, TRA_VARIABLE_ZERO_ALIAS_3 );
    TFT zero_4( pud->T, TRA_VARIABLE_ZERO_ALIAS_4 );
    TFT zero_5( pud->T, TRA_VARIABLE_ZERO_ALIAS_5 );
    TFT zero_6( pud->T, TRA_VARIABLE_ZERO_ALIAS_6 );
    Ty acc = zero_1;
    std::cout << tra_get_string( pud->T, adjust< Ty >( pud ) );

    //deal with initial operand being a command
    char peek = std::cin.peek();
    switch (peek)
    {
    case 'q':
    case 'Q':
        return;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
        break;
    default:
        std::cin >> peek;
        break;
    }

    std::cin >> acc;

    /* Check that the adjustment returns the correct value */
    while ( ( int )strlen( tra_get_string( pud->T, adjust< Ty >( pud ) ) ) > zero_2 )
    {
        char op = zero_3;
        Ty operand = zero_6;
        std::cin >> op;
        switch( op )
        {
        case 'q' :
        case 'Q' :
            return;
        case '.' :
            if (is_basic< Ty >())
            {
                std::cout << tra_get_string( pud->T, TRA_STRING_please_upgrade_ALIAS_1 );
                return;
            }
            break;
        case '+' :
            std::cin >> operand;
            acc = acc + operand;
            break;
        case '-' :
            std::cin >> operand;
            acc = acc - operand;
            break;
        case '*' :
            std::cin >> operand;
            acc = acc * operand;
            break;
        case '/' :
            std::cin >> operand;
            acc = acc / operand;
            break;
        case 'c' :
        case 'C' :
            acc = zero_4;
            break;
        default :
            continue;
        }
        std::cout << acc << tra_get_string( pud->T, TRA_STRING_NEW_LINE_ALIAS_1 );
    };
}

/*----------------------------------------------------------------------*/
/* return "highres" license                                                */
/*----------------------------------------------------------------------*/
void return_highres_license( USER_DATA * pud )
{
    if ( pud && (! FlcReturnLicense(
                                    * pud->licensing,
                                    & pud->license,
                                    * pud->error)))
    {
        sDisplayErrorString(tra_get_string( pud->T, TRA_STRING_returning_highres_ALIAS_1 ), * pud->error);
    }
}

/*----------------------------------------------------------------------*/
/* placeholder to allow for reporting of tamper detection               */
/*----------------------------------------------------------------------*/
extern "C" int report_callback( tra_Data * p )
{
    std::cout << tra_get_string_parameter(p);
    return 0;
}

extern "C" int highres( tra_Data * p )
{
    USER_DATA * pud = (USER_DATA *)tra_get_user_data( p );
    calc_loop< double >( pud );
    return_highres_license( pud );
    return 0;
}

/*----------------------------------------------------------------------*/
/* acquire_highres                                                        */
/*----------------------------------------------------------------------*/
extern "C" int acquire_highres(tra_Data * p)
{
    USER_DATA * pud = ( USER_DATA * )tra_get_user_data( p );
    if( ! pud )
    {
        return -1;
    }
    TFT pid( pud->T );
    TFT one( pud->T, TRA_VARIABLE_ONE_ALIAS_1 );
    TFT zero( pud->T, TRA_VARIABLE_ZERO_ALIAS_7 );
    TFT reply( pud->T, TRA_VARIABLE_ZERO_ALIAS_1 );
    pid = get_random_number();

    reply -= one;

    /*----------------------------------------------------------------------*/
    /* create buffer license source from file                               */
    /*----------------------------------------------------------------------*/
    if ( ! FlcAddBufferLicenseSourceFromFile(
        * pud->licensing,
        pud->license_file_path.c_str(),
        NULL,
        * pud->error ) )
    {
        if( FlcErrorGetCode( *pud->error ) == FLCSWC_FILE_OPEN )
        {
            std::ostringstream msg;
            msg << tra_get_string( pud->T, TRA_STRING_license_file_ALIAS_1 );
            msg << pud->license_file_path;
            msg << tra_get_string( pud->T, TRA_STRING_license_bin_not_found_ALIAS_1 );
            sDisplayInfoString( msg.str() );
        }
        else
        {
            sDisplayErrorString( tra_get_string( pud->T, TRA_STRING_adding_buffer_license_source_ALIAS_1 ), * pud->error );
        }
        return zero - one;
    }

    /*----------------------------------------------------------------------*/
    /* acquire "highres" license                                            */
    /*----------------------------------------------------------------------*/
    if ( ! FlcAcquireLicense(
            * pud->licensing,
            & pud->license,
            tra_get_string( pud->T, TRA_STRING_highres_ALIAS_1 ),
            tra_get_string( pud->T, TRA_STRING__1_0_ALIAS_1 ),
            * pud->error) )
    {
        sDisplayErrorString( tra_get_string( pud->T, TRA_STRING_acquiring_highres_ALIAS_1 ), * pud->error);
    }
    else
    {
        TFT match( pud->T );
        const FlcChar * name;
        FlcLicenseGetName(pud->license, & name, * pud->error );

        match = strcmp( name, tra_get_string(pud->T, TRA_STRING_highres_ALIAS_2 ) );
        /*----------------------------------------------------------------------*/
        /* set the expected values:                                                */
        /* reply is set to true                                                    */
        /* check is set to pid + TRA_STRING_PROMPT_ALIAS_1                        */
        /*----------------------------------------------------------------------*/
        reply ++;
        pud->check += pid;
        pud->check += match;
        reply += one;
        sDisplayInfoString(tra_get_string(pud->T, TRA_STRING_acquired_highres_license_ALIAS_1));
    }

    return reply;
}

extern "C" int lowres( tra_Data * p )
{
    USER_DATA * pud = (USER_DATA *)tra_get_user_data( p );
    calc_loop< int >( pud );
    return 0;
}

int main( int argc, char * argv[ ] )
{
    int reply = -1;
    FlcErrorRef error = NULL;
    FlcLicensingRef licensing = NULL;
    /*----------------------------------------------------------------------*/
    /* load the tra engine                                                    */
    /*----------------------------------------------------------------------*/
    tra_State * T = tra_load_calc_declarative_data();
    if (T)
    {
        /*choreograph the destruction of the TFTs in the USER_DATA*/
        {
            /*----------------------------------------------------------------------*/
            /* setup the USER_DATA structure                                        */
            /*----------------------------------------------------------------------*/
            USER_DATA ud = {
                T,
                & error,
                & licensing,
                NULL,         //license
                "",
                TFT( T ),     //check
                TFT( T ),     //pid
            };
            /* Set up the default inputFile */
            ud.license_file_path = tra_get_string( T, TRA_STRING_license_bin_ALIAS_1 );
            if (argc != 1 && argc != 2)
            {
                sUsage( T, argv[0], ud.license_file_path );
                return -1;
            }

            if (argc == 2)
            {
                std::string arg1(argv[1]);
                if (arg1.compare("-help")==0 ||
                    arg1.compare("-h")==0)
                {
                    sUsage( T, argv[0], ud.license_file_path );
                    return -1;
                }
                else
                {
                    /* Override the default inputFile */
                    ud.license_file_path = argv[1];
                }
            }
            /*----------------------------------------------------------------------*/
            /* create error object                                                    */
            /*----------------------------------------------------------------------*/
            if ( ! FlcErrorCreate(&error) )
            {
                sDisplayErrorString( tra_get_string( T, TRA_STRING_creating_error_object_ALIAS_1 ), NULL );
            }
            else
            {
                ud.error = & error;
                /*----------------------------------------------------------------------*/
                /* create licensing environment                                            */
                /*----------------------------------------------------------------------*/
                int len = 0;
                const FlcUInt8 * identity_data = (const FlcUInt8 *)tra_get_lstring( T, TRA_STRING_IDENTITY_ALIAS_1, &len );
                if (!FlcLicensingCreate(
                                        &licensing, identity_data, len, NULL, tra_get_string( T, TRA_STRING__1234567890_ALIAS_1 ), error))
                {
                    const char * msg = tra_get_string(T, TRA_STRING_creating_licensing_object_ALIAS_1);
                    sDisplayErrorString( msg, error);
                }
                else
                {
                    reply = 0;
                    ud.licensing = & licensing;
                    ud.check = ud.check + TRA_STRING_PROMPT_ALIAS_1;
                    ud.pid = get_random_number();
                    sDisplayInfoString( tra_get_string( T, TRA_STRING_licensing_environment_established_ALIAS_1 ) );
                    /*----------------------------------------------------------------------*/
                    /* perform the generated action when getting the string                    */
                    /* ( in this case, we check that flxCore has not been tampered with )    */
                    /*----------------------------------------------------------------------*/
                    std::cout << tra_get_string( T, TRA_STRING_PROMPT1_ALIAS_1 );

                    reply = tra_if( T, TRA_SNIF_run_calc_ALIAS_1, & ud );
                }
            }
            /* destroy the TFTs in the USER_DATA before it goes out of scope */
        }
        tra_report_errors( T, reply );
        tra_close( T );
    }
    /*----------------------------------------------------------------------*/
    /* cleanup                                                                */
    /*----------------------------------------------------------------------*/
    (void)sCleanup( &licensing, &error );

    return reply;
}
