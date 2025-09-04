#include "dump.h"

#include "TraWrapper.h"

TraWrapper::~TraWrapper() {
    if (m_tra_state) {
        tra_close(m_tra_state);
    }
}

TraWrapper::operator tra_State*() {
    if (!m_tra_state) {
        DEBUG_PRINTLN("load TRA declarative data...")

        m_tra_state = tra_load_nova_declarative_data();

        DEBUG_PRINT("TRA = %p", (void*)m_tra_state)
    }

    return m_tra_state;
}
