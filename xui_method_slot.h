#ifndef __xui_method_slot_h__
#define __xui_method_slot_h__

#include "xui_header.h"

template<typename T>
class xui_method_slot
{
public:
	virtual void operator () ( xui_componet* sender, T& args ){};
};

template<typename T>
class xui_method_static : public xui_method_slot<T>
{
public:
	/*
	//typedef
	*/
	typedef void (*static_func)
		( xui_componet* sender, T& args );

	/*
	//constructor
	*/
	xui_method_static( static_func func )
	: m_func(func)
	{}

	/*
	//operator
	*/
	virtual void operator () ( xui_componet* sender, T& args )
	{
		(*m_func)(sender, args);
	}

protected:
	/*
	//member
	*/
	static_func	m_func;

protected:
	/*
	//hidden constructor
	*/
	xui_method_static( void ){};
	xui_method_static( const xui_method_static<T>& other ){};
};

template<typename T, typename C>
class xui_method_member : public xui_method_slot<T>
{
public:
	/*
	//typedef
	*/
	typedef void (C::*member_func)
		( xui_componet* sender, T& args );

	/*
	//constructor
	*/
	xui_method_member( C* user, member_func func )
	: m_user(user)
	, m_func(func)
	{};

	/*
	//operator
	*/
	virtual void operator () ( xui_componet* sender, T& args )
	{
		(m_user->*m_func)(sender, args);
	}

protected:
	/*
	//member
	*/
	C*			m_user;
	member_func	m_func;

protected:
	/*
	//hidden constructor
	*/
	xui_method_member( void ){}
	xui_method_member( const xui_method_member<T,C>& other ){}
};

#endif//__xui_method_slot_h__