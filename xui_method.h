#ifndef __xui_method_h__
#define __xui_method_h__

#include "xui_method_slot.h"
#include "xui_method_args.h"

template<typename T>
class xui_method
{
public:
	/*
	//constructor
	*/
	xui_method( void )
	{}

	/*
	//destructor
	*/
    virtual ~xui_method( void )
	{
		for (u32 i = 0; i < m_slot_vec.size(); ++i)
			delete m_slot_vec[i];
	}

	/*
	//operator
	*/
	void operator += ( xui_method_slot<T>* slot )
	{
		m_slot_vec.push_back(slot);
	}

	void operator () ( xui_component* sender, T&  args )
	{
		for (u32 i = 0; i < m_slot_vec.size(); ++i)
			(*m_slot_vec[i])(sender, args);
	}

	/*
	//method
	*/
	u32	 count() const
	{
		return m_slot_vec.size();
	}

protected:
	/*
	//typedef
	*/
	typedef std::vector< xui_method_slot<T>* >
		xui_slot_vec;

	/*
	//member
	*/
	xui_slot_vec	m_slot_vec;

	/*
	//hidden
	*/
	xui_method<T>& operator = ( const xui_method<T>& other ){}
};

#endif//__xui_method_h__