#ifndef __onity_proppath_h__
#define __onity_proppath_h__

#include "onity_propfile.h"

class onity_proppath : public onity_propfile
{
public:
	/*
	//constructor
	*/
	onity_proppath( xui_bitmap* icon, const std::wstring& full );

	/*
	//destructor
	*/
	virtual ~onity_proppath( void );

	/*
	//method
	*/
	s32						get_pathroll		( void ) const;
	void					set_pathroll		( s32 value );
	s32						get_fileroll		( void ) const;
	void					set_fileroll		( s32 value );
	onity_propfile*			get_viewfile		( void );
	void					set_viewfile		( onity_propfile* viewfile );
	bool					has_fileprop		( onity_propfile* proproot );
	const xui_proproot_vec&	get_fileprop		( void ) const;
	void					add_fileprop		( onity_propfile* proproot );
	void					del_fileprop		( onity_propfile* proproot );
	const xui_proproot_vec& get_pathprop		( void ) const;
	void					add_pathprop		( onity_proppath* proproot );
	void					ini_treeprop		( xui_treenode* root );

protected:
	/*
	//method
	*/
	onity_propfile*			new_fileprop		( const std::wstring& full );
	onity_proppath*			new_pathprop		( const std::wstring& full );

	/*
	//member
	*/
	s32						m_pathroll;
	s32						m_fileroll;
	onity_propfile*			m_viewfile;
	xui_propkind*			m_pathkind;
	xui_proproot_vec		m_fileprop;
	xui_proproot_vec		m_pathprop;
};

#endif//__onity_proppath_h__