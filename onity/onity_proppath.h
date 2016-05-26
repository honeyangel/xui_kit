#ifndef __onity_proppath_h__
#define __onity_proppath_h__

#include "onity_propfile.h"

class onity_proppath : public onity_propfile
{
public:
	/*
	//constructor
	*/
	onity_proppath( const std::wstring& full );

	/*
	//destructor
	*/
	virtual ~onity_proppath( void );

	/*
	//method
	*/
	s32						get_slider			( void ) const;
	void					set_slider			( s32 slider );
	const xui_proproot_vec&	get_fileprop		( void ) const;
	void					add_fileprop		( xui_proproot* proproot );

protected:
	/*
	//member
	*/
	s32						m_slider;
	xui_propkind*			m_pathkind;
	xui_proproot_vec		m_fileprop;

};

#endif//__onity_proppath_h__