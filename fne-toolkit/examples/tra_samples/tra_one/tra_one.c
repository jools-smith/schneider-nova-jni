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

#include <stdio.h>

#include <tra.h>
#include <tra_one_declarative_data.h>
#include <user_data.h>

int global_tamper_count = 0;

/*==============================================================*/
/* This callback function is declared in tra_one_gen.tra        */
int on_tamper( tra_Data * p )
/*==============================================================*/
{
    ++global_tamper_count;
    printf( "\nWe appear to be hacked\n\ncount %d. reason: %s", global_tamper_count, tra_get_string_parameter(p));
    return 0;
}

/*==============================================================*/
int load_tra_and_fne( USER_DATA * pud )
/*==============================================================*/
{
    int reply = -1;
    if (pud )
    {
        pud->error = -1;
        /*initialize the tra engine*/
        pud->T = tra_load_tra_one_declarative_data();
        if (pud->T)
        {
            int hacked_flag = 0;
            /*check if we have been hacked by getting a value that should be == 1*/
            pud->error = tra_get_value(pud->T,  TRA_VARIABLE_SV_ONE_ALIAS_1, & hacked_flag);
            if (1 != hacked_flag && TRA_OK == pud->error)
            {
                /*if we've been hacked ask the engine why*/
                pud->error = tra_call(pud->T, TRA_FUNCTION_get_last_hacked_reason_ALIAS_1, NULL, 0, 0, NULL );
            }
            else
            {
                load_fne(pud);
            }
        }
    }
    return reply;
}

/*==============================================================*/
int unload_tra_and_fne( USER_DATA * pud )
/*==============================================================*/
{
    unload_fne(pud);
    tra_report_errors(pud->T, pud->error);
    tra_close(pud->T);
    return 0;
}


