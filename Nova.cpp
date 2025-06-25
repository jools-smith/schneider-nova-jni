/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

// Nova.cpp : Defines the exported functions for the DLL application.
//

#define DUMP
#undef DUMP

#define LIB_EXPORT __attribute__ ((visibility ("default")))

#ifdef DUMP
#define DEBUG_PRINT(format, ...) printf(format, ##__VA_ARGS__)
#define DEBUG_PRINTLN(a) printf("%s...\n", a);
#define COUT cout
#else
#define DEBUG_PRINT(format, ...) //
#define DEBUG_PRINTLN(a) //
#endif

#include <ctime>
#include <string>
#include <sstream>
#include <set>
#include <vector>

#include "Nova.h"

using namespace std;

#include "tra.h"
#include "jni.h"
#include "tra_gen/nova_declarative_data.h"
#include "com_flexera_schneider_fnesigner_Nova.h"

#include "FlcLicensing.h"
#include "FlcLicenseManager.h"
#include "FlcFeature.h"
//#include "Nova.IdentityClient.h"
#include "fnedemo.RSA512.IdentityClient.h"

/*!
 * Wrap tra_State pointer for lazy initialization
 */
class CTra
{
	tra_State *m_pTraState = 0;

public:
	CTra() : m_pTraState(0)
	{
	}
	virtual~CTra()
	{
		if (m_pTraState)
		{
			tra_close(m_pTraState);
		}
	}
    /*!
     * Perform lazy load of TRA engine
     */
	operator tra_State* ()
	{
		if (NULL == m_pTraState)
		{
            DEBUG_PRINTLN("Lazy Load TRA");
			m_pTraState = tra_load_nova_declarative_data();
		}
		return m_pTraState;
	}
} tra;


struct UserData
{
	FlcErrorRef m_error;
	FlcLicensingRef m_licensing;
    FlcLicenseRef m_license;
	string error;
    string path;
    vector<string> features;
    
	UserData() : m_error(0), m_licensing(0), m_license(0)
	{
	}

	virtual~UserData()
	{
		if (m_license)
		{
			FlcLicenseDelete(&m_license, NULL);
		}       
        
		if (m_licensing)
		{
			FlcLicensingDelete(&m_licensing, NULL);
		}

		if (m_error)
		{
			FlcErrorDelete(&m_error);
		}
	}

    int initialize()
    {
        TFT status(tra, TRA_VARIABLE_zero_ALIAS_1);
        TFT istrue(tra, TRA_VARIABLE_one_ALIAS_1);
        
        DEBUG_PRINTLN("FlcErrorCreate");
		status = TFT(tra, TRA_VARIABLE_zero_ALIAS_2) + FlcErrorCreate(&m_error);
		if (status == istrue)
		{
            DEBUG_PRINTLN("FlcLicensingCreate");            
			status = TFT(tra, TRA_VARIABLE_zero_ALIAS_3) + FlcLicensingCreate(&m_licensing, identity_data, sizeof identity_data, NULL, NULL, m_error);
			if (status == istrue)
			{             
                DEBUG_PRINTLN("FlcAddBufferLicenseSourceFromFile");                     
                status = FlcAddBufferLicenseSourceFromFile(m_licensing, path.c_str(),  NULL, m_error);
				if (status == istrue)
				{
                    const string feature = tra_get_string(tra, TRA_STRING_feature_name_ALIAS_1);

                    const string version = tra_get_string(tra, TRA_STRING_feature_version_ALIAS_1);
                
                    DEBUG_PRINTLN("FlcAcquireLicense");    
                    status = TFT(tra, TRA_VARIABLE_zero_ALIAS_4) + FlcAcquireLicense(m_licensing, &m_license, feature.c_str(), version.c_str(), m_error);
                    if (status == istrue)
                    {
                        DEBUG_PRINTLN("FlcAcquireLicense succeeded");
                    }
				}
			}
		}

		error = FlcErrorGetMessage(m_error) ;  
        
        return status;
    }
    
    int dump(stringstream&stream)
	{
        TFT status(tra, TRA_VARIABLE_zero_ALIAS_5);
        TFT istrue(tra, TRA_VARIABLE_one_ALIAS_2);
        
        FlcFeatureCollectionRef features = 0;
        FlcSize size = 0;  
        
        DEBUG_PRINTLN("FlcGetFeatureCollection");

		status = TFT(tra, TRA_VARIABLE_zero_ALIAS_7) + FlcGetFeatureCollection(m_licensing, &features, m_error);
        if (status == istrue)
		{
            DEBUG_PRINTLN("FlcFeatureCollectionSize");

			status = TFT(tra, TRA_VARIABLE_zero_ALIAS_8) + FlcFeatureCollectionSize(features, &size, m_error);
            if (status == istrue)
            {
				for (FlcSize i = 0; i < size; i++)
				{
					DEBUG_PRINTLN("FlcFeatureCollectionGet");
					FlcFeatureRef feature = 0;
					
                    status = TFT(tra, TRA_VARIABLE_zero_ALIAS_9) + FlcFeatureCollectionGet(features, &feature, i, m_error);
                    if (status != istrue)
                    {
                        break;
                    }
                    
                    const FlcChar*name = 0;
                    status = TFT(tra, TRA_VARIABLE_zero_ALIAS_10) + FlcFeatureGetName(feature, &name, m_error);
                    if (status != istrue)
                    {
                        break;
                    }
                    
                    const FlcChar*version = 0;
                    status = TFT(tra, TRA_VARIABLE_zero_ALIAS_11) + FlcFeatureGetVersion(feature, &version, m_error);
                    if (status != istrue)
                    {
                        break;
                    }

                    stream << "license: " << name << " - " << version << std::endl;

				}
            }
		}

        if (status != istrue)
        {
            stream << "error: " << FlcErrorGetMessage(m_error);
        }
        
        if (features)
        {
            FlcFeatureCollectionDelete(&features, NULL);
        }
        
        return status;
	}

};

extern "C"
{
//    int do_initialize(tra_Data *ptr)
//	{
//		UserData*const data = static_cast<UserData*>(tra_get_user_data(ptr));
//
//		DEBUG_PRINTLN("do_initialize");
//        
//        TFT result(tra, TRA_VARIABLE_zero_ALIAS_12);
//        
//        result = data->initialize();
//        
//		return result;
//	}
        
    // TODO - need to include license check at some point
	int do_initialize(tra_Data *ptr)
	{
		DEBUG_PRINTLN("do_initialize");
        
        TFT result(tra, TRA_VARIABLE_zero_ALIAS_12);

        // license check would go here
        result += TFT(tra, TRA_VARIABLE_one_ALIAS_12);
        
		return result;
	}

	int do_checkout(tra_Data *)
	{
		DEBUG_PRINTLN("SUCCESS");

		tra_copy(tra, TRA_VARIABLE_status_ALIAS_1, TRA_VARIABLE_ax_ALIAS_2);
		tra_call(tra, TRA_SF_MULTIPLY_ALIAS_1, NULL, TRA_VARIABLE_status_ALIAS_2, TRA_VARIABLE_zero_ALIAS_1, NULL);
		tra_call(tra, TRA_SF_MULTIPLY_ALIAS_2, NULL, TRA_VARIABLE_status_ALIAS_3, TRA_VARIABLE_zero_ALIAS_2, NULL);
		tra_call(tra, TRA_SF_MULTIPLY_ALIAS_3, NULL, TRA_VARIABLE_status_ALIAS_4, TRA_VARIABLE_zero_ALIAS_3, NULL);

		tra_copy(tra, TRA_VARIABLE_status_ALIAS_5, TRA_VARIABLE_ax_ALIAS_3);
		tra_copy(tra, TRA_VARIABLE_status_ALIAS_5, TRA_VARIABLE_ax_ALIAS_4);
        
		tra_call(tra, TRA_SF_ADD_ALIAS_1, NULL, TRA_VARIABLE_status_ALIAS_6, TRA_VARIABLE_one_ALIAS_4, NULL);
		tra_call(tra, TRA_SF_ADD_ALIAS_2, NULL, TRA_VARIABLE_status_ALIAS_7, TRA_VARIABLE_zero_ALIAS_5, NULL);
		tra_call(tra, TRA_SF_SUBTRACT_ALIAS_3, NULL, TRA_VARIABLE_status_ALIAS_8, TRA_VARIABLE_one_ALIAS_6, NULL);

		int status;

		tra_call(tra, TRA_SF_LESS_THAN_ALIAS_1, NULL, TRA_VARIABLE_one_ALIAS_1, TRA_VARIABLE_status_ALIAS_9, &status);

		return status;
	}

	int do_initialize_fail(tra_Data *)
	{
		DEBUG_PRINTLN("FAILED");

		tra_copy(tra, TRA_VARIABLE_status_ALIAS_10, TRA_VARIABLE_bx_ALIAS_2);
		tra_call(tra, TRA_SF_MULTIPLY_ALIAS_1, NULL, TRA_VARIABLE_status_ALIAS_11, TRA_VARIABLE_one_ALIAS_1, NULL);
		tra_call(tra, TRA_SF_MULTIPLY_ALIAS_2, NULL, TRA_VARIABLE_status_ALIAS_12, TRA_VARIABLE_one_ALIAS_2, NULL);
		tra_call(tra, TRA_SF_MULTIPLY_ALIAS_3, NULL, TRA_VARIABLE_status_ALIAS_13, TRA_VARIABLE_one_ALIAS_3, NULL);

        // this is an invalid
		tra_copy(tra, TRA_VARIABLE_status_ALIAS_14, TRA_VARIABLE_bx_ALIAS_3);
		tra_copy(tra, TRA_VARIABLE_status_ALIAS_14, TRA_VARIABLE_bx_ALIAS_4);
        
		tra_call(tra, TRA_SF_SUBTRACT_ALIAS_1, NULL, TRA_VARIABLE_status_ALIAS_15, TRA_VARIABLE_one_ALIAS_7, NULL);
		tra_call(tra, TRA_SF_SUBTRACT_ALIAS_2, NULL, TRA_VARIABLE_status_ALIAS_16, TRA_VARIABLE_zero_ALIAS_8, NULL);
		tra_call(tra, TRA_SF_ADD_ALIAS_3, NULL, TRA_VARIABLE_status_ALIAS_17, TRA_VARIABLE_one_ALIAS_9, NULL);

		int status;

		tra_call(tra, TRA_SF_LESS_THAN_ALIAS_1, NULL, TRA_VARIABLE_one_ALIAS_2, TRA_VARIABLE_status_ALIAS_17, &status);

		return status;
	}
    
    
	LIB_EXPORT jboolean JNICALL Java_com_flexera_schneider_fnesigner_Nova_process(JNIEnv*env, jobject object)
	{           
        TFT status(tra, TRA_VARIABLE_minus_one_ALIAS_1); //-2
        TFT one(tra, TRA_VARIABLE_one_ALIAS_3);
        
        status -= one;
		if (env)
		{
            UserData userdata;

//            userdata.path = convert(env, licenseFilePath);


            // AX = 0
            tra_set_value(tra, TRA_VARIABLE_ax_ALIAS_1, TRA_STRING_identity_ALIAS_1);
            tra_set_value(tra, TRA_VARIABLE_bx_ALIAS_1, TRA_STRING_identity_bad_ALIAS_1);

            /**
             * For now ther is no license check
             * 
             * tra_if(tra, TRA_SNIF_initialize_ALIAS_1, &userdata);
             * 
             */
            tra_if(tra, TRA_SNIF_initialize_ALIAS_1, &userdata);
            status += TFT(tra, TRA_VARIABLE_one_ALIAS_4); // -1
            jclass source = env->GetObjectClass(object);
            if (source)
            {
                DEBUG_PRINT("### class %p\n", (void*)source);

                jfieldID field_m = env->GetFieldID(source, "message", "Ljava/lang/String;");

                DEBUG_PRINT("### field_m %p\n", (void*)field_m);

                if (field_m)
                {
                    jstring string = env->NewStringUTF(userdata.error.c_str());
                    if (string)
                    {
                        DEBUG_PRINT("### message %p\n", (void*)string);
                        env->SetObjectField(object, field_m, string);
                        status += TFT(tra, TRA_VARIABLE_one_ALIAS_5);   // 0                      
                    }
                }

                jfieldID field_i = env->GetFieldID(source, "identity", "Ljava/lang/String;");

                DEBUG_PRINT("### field_i %p\n", (void*)field_i);

                if (field_i)
                {
                    int alias = 0;
                    tra_get_value(tra, TRA_VARIABLE_status_ALIAS_17, &alias);

                    string id = tra_get_string(tra, alias);

                    DEBUG_PRINT("### %s\n", id.c_str());

                    jstring identity = env->NewStringUTF(id.c_str());
                    if (identity)
                    {
                        DEBUG_PRINT("### identity %p\n", (void*)identity);

                        env->SetObjectField(object, field_i, identity);
                        
                        status += TFT(tra, TRA_VARIABLE_one_ALIAS_6); // + 1                         
                    }
                }
            }
        }

        DEBUG_PRINT("RETURN %i %i %s\n", status.get(), one.get(), status == one ? "T" : "F");
         
		return status == one;
	}
       
    bool LIB_EXPORT TestTra(stringstream&stream)
    {
        TFT status(tra);
        
        tra_copy(tra, TRA_VARIABLE_status_ALIAS_1, TRA_VARIABLE_zero_ALIAS_1);
                
        // = 0
        int count = 144;

        tra_call(tra, TRA_SF_ADD_ALIAS_1, NULL, TRA_VARIABLE_status_ALIAS_2, TRA_VARIABLE_one_ALIAS_1, NULL);
        tra_call(tra, TRA_SF_ADD_ALIAS_2, NULL, TRA_VARIABLE_status_ALIAS_3, TRA_VARIABLE_one_ALIAS_2, NULL);
        tra_call(tra, TRA_SF_ADD_ALIAS_3, NULL, TRA_VARIABLE_status_ALIAS_4, TRA_VARIABLE_one_ALIAS_3, NULL);
        tra_call(tra, TRA_SF_ADD_ALIAS_1, NULL, TRA_VARIABLE_status_ALIAS_5, TRA_VARIABLE_one_ALIAS_4, NULL);
        tra_call(tra, TRA_SF_ADD_ALIAS_2, NULL, TRA_VARIABLE_status_ALIAS_6, TRA_VARIABLE_one_ALIAS_5, NULL);
        tra_call(tra, TRA_SF_ADD_ALIAS_3, NULL, TRA_VARIABLE_status_ALIAS_7, TRA_VARIABLE_one_ALIAS_6, NULL);
        tra_call(tra, TRA_SF_ADD_ALIAS_1, NULL, TRA_VARIABLE_status_ALIAS_8, TRA_VARIABLE_one_ALIAS_7, NULL);
        tra_call(tra, TRA_SF_ADD_ALIAS_2, NULL, TRA_VARIABLE_status_ALIAS_9, TRA_VARIABLE_one_ALIAS_8, NULL);
        tra_call(tra, TRA_SF_ADD_ALIAS_3, NULL, TRA_VARIABLE_status_ALIAS_10, TRA_VARIABLE_one_ALIAS_9, NULL);
        tra_call(tra, TRA_SF_ADD_ALIAS_1, NULL, TRA_VARIABLE_status_ALIAS_11, TRA_VARIABLE_one_ALIAS_10, NULL);
        tra_call(tra, TRA_SF_ADD_ALIAS_2, NULL, TRA_VARIABLE_status_ALIAS_12, TRA_VARIABLE_one_ALIAS_11, NULL);
        tra_call(tra, TRA_SF_ADD_ALIAS_3, NULL, TRA_VARIABLE_status_ALIAS_13, TRA_VARIABLE_one_ALIAS_12, NULL);
        tra_call(tra, TRA_SF_ADD_ALIAS_1, NULL, TRA_VARIABLE_status_ALIAS_14, TRA_VARIABLE_one_ALIAS_13, NULL);
        tra_call(tra, TRA_SF_ADD_ALIAS_2, NULL, TRA_VARIABLE_status_ALIAS_15, TRA_VARIABLE_one_ALIAS_14, NULL);
        tra_call(tra, TRA_SF_ADD_ALIAS_3, NULL, TRA_VARIABLE_status_ALIAS_16, TRA_VARIABLE_one_ALIAS_15, NULL);
        // 15

        tra_call(tra, TRA_SF_SUBTRACT_ALIAS_1, NULL, TRA_VARIABLE_status_ALIAS_2, TRA_VARIABLE_one_ALIAS_1, NULL);
        tra_call(tra, TRA_SF_SUBTRACT_ALIAS_2, NULL, TRA_VARIABLE_status_ALIAS_3, TRA_VARIABLE_one_ALIAS_2, NULL);
        tra_call(tra, TRA_SF_SUBTRACT_ALIAS_3, NULL, TRA_VARIABLE_status_ALIAS_4, TRA_VARIABLE_one_ALIAS_3, NULL);
        // 12
        
        tra_call(tra, TRA_SF_MULTIPLY_ALIAS_3, NULL, TRA_VARIABLE_status_ALIAS_1, TRA_VARIABLE_status_ALIAS_2, NULL);
        // 144
        
        TFT result(tra, TRA_VARIABLE_status_ALIAS_3);
        

        stream << "Expected count:" << count << " - actual count:" << result.get();

        return count == result;
    }

    bool LIB_EXPORT TestFne(const string&licenseFilePath, stringstream&stream)
    {
        UserData data;
        
        data.path = licenseFilePath;
        
        TFT status(tra);
        const TFT istrue(tra, TRA_VARIABLE_one_ALIAS_19);
        status = data.initialize();
        if (status == istrue)
        {
            status = data.dump(stream);
        }
            
        return status == istrue;
    }
} 
/* extern c */
