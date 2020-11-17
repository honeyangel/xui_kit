#ifndef __xui_method_slot_h__
#define __xui_method_slot_h__

#include "xui_header.h"

template<typename T>
class xui_method_slot
{
public:
	virtual void operator () ( xui_component* sender, T& args ){};
};

template<typename T>
class xui_method_static : public xui_method_slot<T>
{
public:
	typedef void (*static_func)
		( xui_component* sender, T& args );

	xui_method_static( static_func func )
	: m_func(func)
	{}

	virtual void operator () ( xui_component* sender, T& args )
	{
		(*m_func)(sender, args);
	}

protected:
	static_func	m_func;

protected:
	xui_method_static( void ){};
	xui_method_static( const xui_method_static<T>& other ){};
};

template<typename T, typename C>
class xui_method_member : public xui_method_slot<T>
{
public:
	typedef void (C::*member_func)
		( xui_component* sender, T& args );

	xui_method_member( C* user, member_func func )
	: m_user(user)
	, m_func(func)
	{};

	virtual void operator () ( xui_component* sender, T& args )
	{
		(m_user->*m_func)(sender, args);
	}

protected:
	C*			m_user;
	member_func	m_func;

protected:
	xui_method_member( void ){}
	xui_method_member( const xui_method_member<T,C>& other ){}
};

#endif//__xui_method_slot_h__