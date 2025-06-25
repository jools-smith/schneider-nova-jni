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

#include <tra.h>
#include "FlcError.h"
#include "FlcLicensing.h"

typedef struct USER_DATA
{
    tra_State * T;
    int error;
    FlcErrorRef flx_error;
    FlcLicensingRef flx_licensing;
}USER_DATA;

#if defined( __cplusplus )
extern"C"
{
#endif

    int load_tra_and_fne( USER_DATA * );
    int is_licensed( USER_DATA * );
    int unload_tra_and_fne( USER_DATA * );
    int load_fne( USER_DATA * );
    void unload_fne( USER_DATA * );


#if defined( __cplusplus )
}
#endif
