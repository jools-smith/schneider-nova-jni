/**************************************************************************************************
* NOTICE OF COPYRIGHT AND OWNERSHIP OF SOFTWARE:
*
* Copyright (c) 1997-2019 Flexera Software LLC. All Rights Reserved.
*
* This computer program is the property of Flexera Software LLC of
* Schaumburg, Illinois, U.S.A. Any use, copy, publication, distribution, display, modification, or
* transmission of this computer program in whole or in part in any form or by any means without
* the prior express written permission of Flexera Software LLC is
* strictly prohibited.
*
* Except when expressly provided by Flexera Software LLC in writing,
* possession of this computer program shall not be construed to confer any license or rights under
* any of Flexera Software LLC's intellectual property rights, whether by
* estoppel, implication, or otherwise.
*
* ALL COPIES OF THIS PROGRAM MUST DISPLAY THIS NOTICE OF COPYRIGHT AND OWNERSHIP IN FULL.
**************************************************************************************************/

#include <iostream>
//#define NDEBUG to disable assertions
//#undef NDEBUG to enable assertions even in release
#include <assert.h>

#ifndef tra_t_hpp_defined
#define tra_t_hpp_defined

/** @file
 *  Defines the TDT an interface to TRA.
 *
 *  Refer to the getting started guide for TRA for usage of this API
 */

/**
 * @addtogroup tra_headers
 * @{
 */


template< int A, int B, int C, int D, int Feature = 0 >
struct TDT_T
{
	typedef TDT_T< A, B, C, D, Feature > type;
public:

	explicit TDT_T< A, B, C, D, Feature >(tra_State * T, TRA_VARIABLE_INDEX alias = TRA_VARIABLE_INDEX_NONE )
		: m_T(T)
		, m_alias(alias)
		, m_dy_key(0)
		, m_st_key(0)
	{
#ifndef NDEBUG
		//avoid unused compiler warning
		int TDT_constructor_worked =
#endif
			// coverity[check_return]
			tra_new(m_T, Feature, alias, this, &m_alias, &m_dy_key, &m_st_key);
#ifndef NDEBUG
		if (0 != TDT_constructor_worked)
		{
			tra_report_errors(m_T, TDT_constructor_worked);
		}
#endif
		assert(0 == TDT_constructor_worked);
		assert(0 != m_st_key);
	}
	~TDT_T< A, B, C, D, Feature >()
	{
		tra_delete(m_T, get_alias());
	}
	void set(int v)
	{
		// coverity[check_return]
		tra_set_value(m_T, get_alias(), v);
	}
	int get() const
	{
		int r = 0; //prepare to fail!
		tra_get_value(m_T, get_alias(), &r);
		return r;
	}
	//Copy Constructors
	TDT_T< A, B, C, D, Feature >(const type & other)
		: m_T(other.get_state())
		, m_alias(other.get_alias())
		, m_dy_key(0)
		, m_st_key(0)
	{
#ifndef NDEBUG
		//avoid unused compiler warning
		int TDT_constructor_worked =
#endif //NDEBUG
			tra_new(m_T, Feature, other.get_alias(), this, &m_alias, &m_dy_key, &m_st_key);
		assert(0 == TDT_constructor_worked);
		assert(0 != m_st_key);
	}

	//Assignment operators
	type & operator = (int v)
	{
		set(v);
		return *this;
	}
	type & operator = (const type & rhs)
	{
		// Check for self-assignment!
		if (this == &rhs)
		{	// Same object?
			return *this;	// Yes, so skip assignment, and just return *this.
		}
		tra_copy(m_T, get_alias(), rhs.get_alias());
		return *this;
	}

	//Arithmetic operators
	type & operator += (const type & rhs)
	{
		tra_call(m_T, TRA_SF_ADD_ALIAS_1, NULL, get_alias(), rhs.get_alias(), NULL);
		return *this;
	}
	type operator + (const type & rhs)
	{
		TDT_T<A, B, C, D> temp(m_T, get_alias());
		tra_call(m_T, TRA_SF_ADD_ALIAS_2, NULL, temp.get_alias(), rhs.get_alias(), NULL);
		return temp;
	}
	type & operator -= (const type & rhs){
		tra_call(m_T, TRA_SF_SUBTRACT_ALIAS_1, NULL, get_alias(), rhs.get_alias(), NULL);
		return *this;
	}
	type operator - (const type & rhs)
	{
		type temp(m_T, get_alias());
		tra_call(m_T, TRA_SF_SUBTRACT_ALIAS_2, NULL, temp.get_alias(), rhs.get_alias(), NULL);
		return temp;
	}
	type & operator *= (const type & rhs)
	{
		tra_call(m_T, TRA_SF_MULTIPLY_ALIAS_1, NULL, get_alias(), rhs.get_alias(), NULL);
		return *this;
	}
	type operator * (const type & rhs)
	{
		type temp(m_T, get_alias());
		tra_call(m_T, TRA_SF_MULTIPLY_ALIAS_2, NULL, temp.get_alias(), rhs.get_alias(), NULL);
		return temp;
	}
	type & operator ++ ()
	{
		TDT_T<A, B, C, D, Feature> rhs(m_T, TRA_VARIABLE_SV_ONE_ALIAS_4);
		tra_call(m_T, TRA_SF_ADD_ALIAS_2, NULL, get_alias(), rhs.get_alias(), NULL);
		return *this;
	}
	type operator ++ (int)
	{
		type old_value(m_T, get_alias());
		TDT_T<A, B, C, D, Feature> rhs(m_T, TRA_VARIABLE_SV_ONE_ALIAS_1);
		tra_call(m_T, TRA_SF_ADD_ALIAS_1, NULL, get_alias(), rhs.get_alias(), NULL);
		return old_value;
	}
	type & operator -- ()
	{
		TDT_T<A, B, C, D, Feature> rhs(m_T, TRA_VARIABLE_SV_ONE_ALIAS_2);
		tra_call(m_T, TRA_SF_SUBTRACT_ALIAS_2, NULL, get_alias(), rhs.get_alias(), NULL);
		return *this;
	}
	type operator -- (int)
	{
		type old_value(m_T, get_alias());
		TDT_T<A, B, C, D, Feature> rhs(m_T, TRA_VARIABLE_SV_ONE_ALIAS_3);
		tra_call(m_T, TRA_SF_SUBTRACT_ALIAS_1, NULL, get_alias(), rhs.get_alias(), NULL);
		return old_value;
	}

	//Comparators
	inline bool operator < (const type & rhs)
	{
		int reply = -1;
		tra_call(m_T, TRA_SF_LESS_THAN_ALIAS_2, NULL, get_alias(), rhs.get_alias(), &reply);
		return 0 != reply;
	}
	inline bool operator > (const type & rhs)
	{
		int reply = -1;
		tra_call(m_T, TRA_SF_LESS_THAN_ALIAS_1, NULL, rhs.get_alias(), get_alias(), &reply);
		return 0 != reply;
	}
	inline bool operator == (const type & rhs)
	{
		int less = 0, greater = 0;
		tra_call(m_T, TRA_SF_LESS_THAN_ALIAS_3, NULL, get_alias(), rhs.get_alias(), &less);
		tra_call(m_T, TRA_SF_LESS_THAN_ALIAS_2, NULL, rhs.get_alias(), get_alias(), &greater);
		return ((0 == less) && (0 == greater));
	}
	inline bool operator != (const type & rhs)
	{
		int less = 0, greater = 0;
		tra_call(m_T, TRA_SF_LESS_THAN_ALIAS_1, NULL, get_alias(), rhs.get_alias(), &less);
		tra_call(m_T, TRA_SF_LESS_THAN_ALIAS_2, NULL, rhs.get_alias(), get_alias(), &greater);
		return ((0 != less) || (0 != greater));
	}
	inline bool operator <= (const type & rhs)
	{
		int reply = -1;
		tra_call(m_T, TRA_SF_LESS_THAN_ALIAS_3, NULL, rhs.get_alias(), get_alias(), &reply);
		return 0 == reply;
	}
	inline bool operator >= (const type & rhs)
	{
		int reply = -1;
		tra_call(m_T, TRA_SF_LESS_THAN_ALIAS_1, NULL, get_alias(), rhs.get_alias(), &reply);
		return 0 == reply;
	}
	operator int()const
	{
		return get();
	}

private:
	TRA_VARIABLE_INDEX get_alias() const
	{
		int key = 0;
		int reply = m_alias ^ m_dy_key;
		if (A == m_st_key) key = B ^ C ^ D;
		if (B == m_st_key) key = A ^ C ^ D;
		if (C == m_st_key) key = B ^ A ^ D;
		if (D == m_st_key) key = B ^ C ^ A;
		assert(key != 0);
		reply = reply ^ key;
		assert(reply != 0);
		return (TRA_VARIABLE_INDEX)reply;
	}
	tra_State* get_state()const
	{
		assert(m_T != NULL);
		return m_T;
	}
	mutable tra_State * m_T;
	int m_alias;
	int m_dy_key;
	int m_st_key;
};

/** @} */ /* end of addtogroup tra_headers */

#endif // tra_t_hpp_defined
