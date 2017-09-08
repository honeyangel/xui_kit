#ifndef __cocos_propnodetextbmfont_h__
#define __cocos_propnodetextbmfont_h__

#include "cocos_propnodewidget.h"

namespace cocos2d
{
	namespace ui
	{
		class TextBMFont;
	}
}

class cocos_propfile;
class cocos_propnodetextbmfont : public cocos_propnodewidget
{
public:
	/*
	//constructor
	*/
	cocos_propnodetextbmfont( cocos_propcsd* file, cocos2d::Node* node );

	/*
	//method
	*/
	cocos2d::ui::TextBMFont*	get_textbmfont	( void );
	cocos_propfile*				get_fontfile	( void );
	void						set_fontfile	( cocos_propfile* file );

protected:
	/*
	//method
	*/
	void						add_textbmfont	( void );

	/*
	//static
	*/
	static std::wstring			get_string		( void* userptr );
	static void					set_string		( void* userptr, const std::wstring& value );
	static void*				get_font		( void* userptr );
	static void					set_font		( void* userptr, void* value );

	/*
	//member
	*/
	cocos2d::ui::TextBMFont*	m_textbmfont;
	cocos_propfile*				m_fontfile;
};

#endif//__cocos_propnodetextbmfont_h__