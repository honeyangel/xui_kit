#ifndef __cocos_proptexture_h__
#define __cocos_proptexture_h__

#include "cocos_propfile.h"

namespace cocos2d
{
	class Texture2D;
}

class cocos_proptexture : public cocos_propfile
{
public:
	/*
	//constructor
	*/
	cocos_proptexture( const std::wstring& full );

	/*
	//method
	*/
	cocos2d::Texture2D*		get_texture		( void );

	/*
	//override
	*/
	virtual std::string		get_dragtype	( void );
	virtual void*			get_dragdata	( void );
	virtual void			ntf_modify		( void );
	virtual bool			can_rename		( void );

protected:
	/*
	//static
	*/
	static f64				get_texwidth	( void* user );
	static f64				get_texheight	( void* user );

	/*
	//member
	*/
	xui_propkind*			m_texkind;
};

#endif//__cocos_proptexture_h__