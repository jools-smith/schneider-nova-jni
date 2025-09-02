#ifndef TRA_WRAPPER_H
#define TRA_WRAPPER_H

#include "tra.h"

#include "tra_gen/nova_declarative_data.h"

class TraWrapper {
    tra_State *m_tra_state;

public:
    TraWrapper() : m_tra_state(0) {
    }

    virtual ~TraWrapper();
    /*!
     * Perform lazy load of TRA engine
     */
    operator tra_State*();
};

#endif
