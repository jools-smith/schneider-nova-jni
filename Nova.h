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

extern "C"
{   
    bool TestFne(const std::string&licenseFilePath, std::stringstream&output);

    bool TestTra(std::stringstream&output);

    // TBC
}

#endif /* NOVA_H */

