#ifndef __cocos_propfnt_h__
#define __cocos_propfnt_h__

#include "cocos_propfile.h"

namespace cocos2d
{
	class Texture2D;
	class BMFontConfiguration;
}

class cocos_propfnt : public cocos_propfile
{
public:
	/*
	//constructor
	*/
	cocos_propfnt( const std::wstring& full );

	/*
	//destructor
	*/
	virtual ~cocos_propfnt( void );

	/*
	//method
	*/
	cocos2d::Texture2D*				get_tex				( void );

	/*
	//override
	*/
	std::string						get_viewtext		( void );
	virtual std::string				get_dragtype		( void );
	virtual void*					get_dragdata		( void );
	virtual void					load				( void );

protected:
	/*
	//static
	*/
	static void*					get_texture			( void* userptr );
	static void						set_texture			( void* userptr, void* value );

	/*
	//member
	*/
	cocos2d::BMFontConfiguration*	m_bmpfont;
	xui_propkind*					m_texkind;
};

#endif//__onity_propfnt_h__