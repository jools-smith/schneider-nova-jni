/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   newfile.h
 * Author: jools
 *
 * Created on 10 February 2016, 17:49
 * Modified 25 June 2026
 */

#ifndef NOVA_H
#define NOVA_H

#include "tra.h"

extern "C" {

	int cf_save_jni_field_aliases_good(tra_Data *const p);

	int cf_save_jni_field_aliases_bad(tra_Data *const p);

	int cf_tamper_detected(tra_Data*const p);

	int do_initialize(tra_Data *const p);

	int do_initialize_success(tra_Data*const p);

	int do_initialize_fail(tra_Data*const p);

}

#endif /* NOVA_H */

