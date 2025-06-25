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
#include <fstream>
#include <vector>
#include <string.h>
#include <tra.h>
#include <tra_one_declarative_data.h>
#include <tra_tdt.hpp>
#include "user_data.h"

#if defined(_MSC_VER) && _MSC_VER >= 1900
#pragma comment(lib, "legacy_stdio_definitions.lib")
#endif

#if _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif

void get_input();

/*==============================================================*/
/* is_basic template                                            */
/* Ty will be an int for lowres and double for highres          */
/*==============================================================*/
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

/*==============================================================*/
/* calc_loop template                                           */
/* Ty will be an int for lowres and double for highres          */
/*==============================================================*/
template< typename Ty >
void calc_loop (tra_State * T)
{
    //check flxCore is not hacked. If it has, following retieval of values will just return 0
    std::cout << tra_get_string( T, TRA_STRING_USAGE_ALIAS_1 );

    int zero_1( 0 );
    int zero_2( 0 );
    int zero_3( 0 );
    int zero_4( 0 );
    TFT one( T, TRA_VARIABLE_SV_ONE_ALIAS_2 );
    Ty acc = zero_1;

    get_input();
    std::cin >> acc;

    if (! is_basic< Ty >())
    {
        //validate that the highres TDT has been enabled
        highres hr( T, TRA_VARIABLE_SV_ONE_ALIAS_3);
        if (hr != 1)
        {
            return; //unexpected as we claimed fp support but it is not enabled in the highres TDT
        }
    }


    //this will break if we have been hacked as one will be equal to 0!
    while (0 != one)
    {
        char op = zero_3;
        Ty operand = zero_2;
        std::cin >> op;
        switch( op )
        {
            case '\0' :
            case 'q' :
            case 'Q' :
                return;
            case 'c' :
            case 'C' :
                acc = zero_4;
                break;
            case '.' :
                if (is_basic< Ty >())
                {
                    //This could easily be replaced with a tra_get_string() function to hide this string from static analysis
                    std::cout << "Please Upgrade for floating point support\n";
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
                if (operand == 0){
                    return;
                }
                acc = acc / operand;
                break;
            default :
                continue;
        }
        std::cout << acc << "\n";
    };
}

//#define HACK1 // patch out calls to FNE
/*==============================================================*/
int main( int argc, char * argv[ ] )
/*==============================================================*/
{
    int worked = -1;

    USER_DATA ud;
    if( load_tra_and_fne( &ud ) )
    {
#ifdef HACK1
        calc_loop<double>(ud.T);
#else
        //check if we have a highres license
        if (is_licensed( & ud ) )
        {
            calc_loop<double>(ud.T);
        }
        else
        {
            calc_loop<int>(ud.T);
        }
#endif
        if (unload_tra_and_fne( &ud ) )
        {
            worked = 0;
        }
    }
    return worked;
}

void get_input()
{
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
}



