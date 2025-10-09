#pragma once

#include "tra.h"

#include "tra_gen/nova_declarative_data.h"

class TraWrapper {
    tra_State *m_tra_state;
    bool loggit;

public:
    TraWrapper() : m_tra_state(0), loggit(false) {
#ifdef ENABLE_DEBUG_MACROS
      loggit = true;
#endif
    }

    virtual ~TraWrapper();

    void enable(const bool allow=true);

    /*!
     * Perform lazy load of TRA engine
     */
    operator tra_State*();
};
