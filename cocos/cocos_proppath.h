#ifndef __cocos_proppath_h__
#define __cocos_proppath_h__

#include "cocos_propfile.h"

class cocos_proppath : public cocos_propfile
{
public:
	/*
	//constructor
	*/
	cocos_proppath( xui_bitmap* icon, const std::wstring& full );

	/*
	//destructor
	*/
	virtual ~cocos_proppath( void );

	/*
	//method
	*/
	s32						get_pathroll		( void ) const;
	void					set_pathroll		( s32 value );
	s32						get_fileroll		( void ) const;
	void					set_fileroll		( s32 value );
	cocos_propfile*			get_viewfile		( void );
	void					set_viewfile		( cocos_propfile* viewfile );
	bool					has_fileprop		( cocos_propfile* proproot );
	const xui_proproot_vec&	get_fileprop		( void ) const;
	void					add_fileprop		( cocos_propfile* proproot );
	void					del_fileprop		( cocos_propfile* proproot );
	const xui_proproot_vec&	get_pathprop		( void ) const;
	void					add_pathprop		( cocos_proppath* proproot );
	void					ini_treeprop		( xui_treenode* root );
	void					refresh				( void );

protected:
	/*
	//method
	*/
	cocos_propfile*			new_fileprop		( const std::wstring& full );
	cocos_proppath*			new_pathprop		( const std::wstring& full );

	/*
	//member
	*/
	s32						m_pathroll;
	s32						m_fileroll;
	cocos_propfile*			m_viewfile;
	xui_propkind*			m_pathkind;
	xui_proproot_vec		m_fileprop;
	xui_proproot_vec		m_pathprop;
};

#endif//__cocos_proppath_h__