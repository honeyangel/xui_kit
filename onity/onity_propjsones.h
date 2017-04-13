#ifndef __onity_propjsones_h__
#define __onity_propjsones_h__

#include "EntityTemplate.h"
#include "onity_propfile.h"

class onity_propjsones : public onity_propfile
{
public:
	/*
	//constructor
	*/
	onity_propjsones( const std::wstring& fullname );

	/*
	//destructor
	*/
	virtual ~onity_propjsones( void );

	/*
	//method
	*/
	xui_proproot_vec		get_subprop		( void );
	xui_proproot*			add_subprop		( void );
	xui_proproot*			add_subprop		( xui_proproot* prop );
	void					del_subprop		( xui_proproot* prop );
	xui_proproot*			get_subprop		( Omiga::EntityTemplate* temp );
	void					set_modify		( bool flag );

	/*
	//virtual
	*/
	virtual bool			can_rename		( void );
	virtual bool			was_modify		( void );
	virtual void			load			( void );
	virtual void			save			( void );
	virtual void			auto_save		( void );
	virtual void			save_as			( const std::wstring& full );

protected:
	/*
	//method
	*/
	void					loadfromfile	( bool notify );

	/*
	//member
	*/
	bool					m_modify;
	xui_proproot_vec		m_subprop;
};

#endif//__onity_propjsones_h__