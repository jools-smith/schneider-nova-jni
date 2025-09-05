#pragma once

//
// Created by jools on 7/2/25.
//

#pragma once

#include "FlcTypes.h"
#include "FlcError.h"
#include "FlcLicensing.h"
#include "FlcFeature.h"
#include "FlcLicense.h"
#include "FlcHostIds.h"
#include "dump.h"
#include <iostream>



template<typename T> class FneWrapperBase {
protected:
    T* object;

    explicit FneWrapperBase() : object(nullptr) {
    }
public:

    virtual ~FneWrapperBase() = default;

    // reference
    operator T*& () {
        return object;
    }

    // pointer to reference
     operator T** () {
        return &object;
    }
};


//template<typename T> class FneLicensingBase  {
//protected:
//	FlcErrorRef& error;
//
//    T* object;
//
//    explicit FneLicensingBase(FlcErrorRef&err) : error(err), object(nullptr) {
//    }
//public:
//
//    virtual ~FneLicensingBase() = default;
//
//    // reference
//    operator T*& () {
//        return object;
//    }
//
//    // pointer to reference
//     operator T** () {
//        return &object;
//    }
//};

class ErrorWrapper final : public FneWrapperBase<struct flcError> {
public:
    explicit ErrorWrapper();
    ~ErrorWrapper() override;
};

class LicensingWrapper final : public FneWrapperBase<struct flcLicensing> {
public:
   explicit LicensingWrapper();
    ~LicensingWrapper() override;
};

class HostIdsWrapper final : public FneWrapperBase<struct flcHostIds> {
public:
   explicit HostIdsWrapper();
    ~HostIdsWrapper() override;
};
