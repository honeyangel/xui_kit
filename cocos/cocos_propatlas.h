#ifndef __cocos_propatlas_h__
#define __cocos_propatlas_h__

#include "cocos_propfile.h"

namespace cocos2d
{
	class Texture2D;
}

class cocos_propatlas : public cocos_propfile
{
public:
	/*
	//constructor
	*/
	cocos_propatlas( const std::wstring& full );

	/*
	//destructor
	*/
	virtual ~cocos_propatlas( void );

	/*
	//virtual
	*/
	bool						has_frame		( const std::wstring& name );
	cocos2d::Texture2D*			get_texture		( void );
	std::vector<xui_proproot*>	get_subprop		( void );
	virtual void				ntf_modify		( void );
	virtual bool				can_rename		( void );
	virtual void				load			( void );

protected:
	/*
	//static
	*/
	static f64					get_texwidth	( void* userptr );
	static f64					get_texheight	( void* userptr );

	/*
	//member
	*/
	xui_propkind*				m_texkind;
	std::vector<xui_proproot*>	m_subprop;
};

#endif//__cocos_propatlas_h__