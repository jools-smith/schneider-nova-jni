#include "UserData.h"

using namespace std;

UserData::~UserData() {
    if (m_license) {
        FlcLicenseDelete(&m_license, NULL);
    }

    if (m_licensing) {
        FlcLicensingDelete(&m_licensing, NULL);
    }

    if (m_error) {
        FlcErrorDelete(&m_error);
    }
}

int UserData::test_initialize() {
    TFT status(m_tra, TRA_VARIABLE_zero_ALIAS_1);
    TFT istrue(m_tra, TRA_VARIABLE_one_ALIAS_1);

    DEBUG_PRINTLN("FlcErrorCreate");
    status = TFT(m_tra, TRA_VARIABLE_zero_ALIAS_2) + FlcErrorCreate(&m_error);
    if (status == istrue) {
        DEBUG_PRINTLN("FlcLicensingCreate");
        status = TFT(m_tra, TRA_VARIABLE_zero_ALIAS_3)
                + FlcLicensingCreate(&m_licensing, identity_data, sizeof identity_data, NULL, NULL, m_error);
        if (status == istrue) {
            DEBUG_PRINTLN("FlcAddBufferLicenseSourceFromFile");
            status = FlcAddBufferLicenseSourceFromFile(m_licensing, path.c_str(), NULL, m_error);
            if (status == istrue) {
                const string feature = tra_get_string(m_tra, TRA_STRING_feature_name_ALIAS_1);

                const string version = tra_get_string(m_tra, TRA_STRING_feature_version_ALIAS_1);

                DEBUG_PRINTLN("FlcAcquireLicense");
                status = TFT(m_tra, TRA_VARIABLE_zero_ALIAS_4)
                        + FlcAcquireLicense(m_licensing, &m_license, feature.c_str(), version.c_str(), m_error);
                if (status == istrue) {
                    DEBUG_PRINTLN("FlcAcquireLicense succeeded");
                }
            }
        }
    }

    error = FlcErrorGetMessage(m_error);

    return status;
}

/**
 * for external testing only - not part of JNI functionality
 */
int UserData::test_dump(stringstream &stream) {
    TFT status(m_tra, TRA_VARIABLE_zero_ALIAS_5);
    TFT istrue(m_tra, TRA_VARIABLE_one_ALIAS_2);

    FlcFeatureCollectionRef features = 0;
    FlcSize size = 0;

    DEBUG_PRINTLN("FlcGetFeatureCollection");

    status = TFT(m_tra, TRA_VARIABLE_zero_ALIAS_7) + FlcGetFeatureCollection(m_licensing, &features, m_error);
    if (status == istrue) {
        DEBUG_PRINTLN("FlcFeatureCollectionSize");

        status = TFT(m_tra, TRA_VARIABLE_zero_ALIAS_8) + FlcFeatureCollectionSize(features, &size, m_error);
        if (status == istrue) {
            for (FlcSize i = 0; i < size; i++) {
                DEBUG_PRINTLN("FlcFeatureCollectionGet");
                FlcFeatureRef feature = 0;

                status = TFT(m_tra, TRA_VARIABLE_zero_ALIAS_9) + FlcFeatureCollectionGet(features, &feature, i, m_error);
                if (status != istrue) {
                    break;
                }

                const FlcChar *name = 0;
                status = TFT(m_tra, TRA_VARIABLE_zero_ALIAS_10) + FlcFeatureGetName(feature, &name, m_error);
                if (status != istrue) {
                    break;
                }

                const FlcChar *version = 0;
                status = TFT(m_tra, TRA_VARIABLE_zero_ALIAS_11) + FlcFeatureGetVersion(feature, &version, m_error);
                if (status != istrue) {
                    break;
                }

                stream << "license: " << name << " - " << version << std::endl;

            }
        }
    }

    if (status != istrue) {
        stream << "error: " << FlcErrorGetMessage(m_error);
    }

    if (features) {
        FlcFeatureCollectionDelete(&features, NULL);
    }

    return status;
}
