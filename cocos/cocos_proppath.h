#ifndef __cocos_proppath_h__
#define __cocos_proppath_h__

#include "cocos_propfile.h"

class cocos_proppath : public cocos_propfile
{
public:
	/*
	//constructor
	*/
	cocos_proppath( const std::wstring& full );

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
	const xui_proproot_vec&	get_fileprop		( void ) const;
	void					add_fileprop		( cocos_propfile* proproot );

protected:
	/*
	//member
	*/
	s32						m_pathroll;
	s32						m_fileroll;
	cocos_propfile*			m_viewfile;
	xui_propkind*			m_pathkind;
	xui_proproot_vec		m_fileprop;
};

#endif//__cocos_proppath_h__