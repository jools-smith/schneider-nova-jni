#include "dump.h"

#include "TraWrapper.h"

TraWrapper::~TraWrapper() {
  if (m_tra_state) {
    tra_close(m_tra_state);
  }
}

void TraWrapper::enable(const bool allow) {
  if (loggit && !allow) {
    printf("TraWrapper logging being disabled\n");
  }

  loggit = allow;

  if (loggit) {
    printf("TraWrapper logging has been enabled\n");
  }
}

TraWrapper::operator tra_State*() {
  if (!m_tra_state) {

    if (loggit) {
      printf("load TRA declarative data...\n");
    }

    m_tra_state = tra_load_nova_declarative_data();

    if (loggit) {
      printf("TRA = %p\n", static_cast<void*>(m_tra_state));
    }
  }

  return m_tra_state;
}
