#ifndef __xui_proproot_h__
#define __xui_proproot_h__

#include "xui_propkind.h"

class xui_proproot
{
public:
	/*
	//constructor
	*/
	xui_proproot( void );

	/*
	//destructor
	*/
	virtual ~xui_proproot( void );

	/*
	//method
	*/
	const xui_propkind_vec&	get_propkind	( void ) const;
	xui_propkind*			get_propkind	( const std::string& type );
	bool					has_propkind	( xui_propkind* propkind ) const;
	void					add_propkind	( xui_propkind* propkind );
	void					del_propkind	( xui_propkind* propkind );
	void					non_ctrl		( void );
	xui_propview*			get_ctrl		( void );
	void					set_ctrl		( xui_propview* propview );

	/*
	//virtual
	*/
	void					on_detach		( xui_proproot* proproot );
	virtual void			on_detach		( const xui_proproot_vec& proproot );
	virtual void			on_attach		( void );

protected:
	/*
	//member
	*/
	xui_propview*			m_ctrl;
	xui_propkind_vec		m_propkind;
};

#endif//__xui_proproot_h__
