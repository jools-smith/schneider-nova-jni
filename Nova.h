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

#include <string>
#include <sstream>

extern "C" {
extern bool test_fne(const std::string &licenseFilePath, std::stringstream &output);

extern bool test_tra(std::stringstream &output);

extern bool quick_test_tra();
}

#endif /* NOVA_H */

