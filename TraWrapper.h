#pragma once

#include "tra.h"

#include "tra_gen/nova_declarative_data.h"

class TraWrapper final {
    tra_State *m_tra_state;
    bool loggit;

public:
    TraWrapper() : m_tra_state(nullptr), loggit(false) {
#ifdef ENABLE_DEBUG_MACROS
      loggit = true;
#endif
    }

    virtual ~TraWrapper();

    void enable(bool allow=true);

    /*!
     * Perform lazy load of TRA engine
     */
    operator tra_State*();
};
