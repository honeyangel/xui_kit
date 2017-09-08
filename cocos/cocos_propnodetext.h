#ifndef __cocos_propnodetext_h__
#define __cocos_propnodetext_h__

#include "cocos_propnodewidget.h"

namespace cocos2d
{
	namespace ui
	{
		class Text;
	}
}

class cocos_propfile;
class cocos_propnodetext : public cocos_propnodewidget
{
public:
	/*
	//constructor
	*/
	cocos_propnodetext( cocos_propcsd* file, cocos2d::Node* node );

	/*
	//method
	*/
	cocos2d::ui::Text*		get_text			( void );
	cocos_propfile*			get_fontfile		( void );
	void					set_fontfile		( cocos_propfile* file );

protected:
	/*
	//method
	*/
	void					add_text			( void );

	/*
	//static
	*/
	static std::wstring		get_string			( void* userptr );
	static void				set_string			( void* userptr, const std::wstring& value );
	static s32				get_horzalign		( void* userptr );
	static void				set_horzalign		( void* userptr, s32   value );
	static s32				get_vertalign		( void* userptr );
	static void				set_vertalign		( void* userptr, s32   value );
	static void*			get_font			( void* userptr );
	static void				set_font			( void* userptr, void* value );
	static f64				get_fontsize		( void* userptr );
	static void				set_fontsize		( void* userptr, f64   value );
	static xui_colour		get_fontdraw		( void* userptr );
	static void				set_fontdraw		( void* userptr, const xui_colour& value );
	static bool				was_enableshadow	( void* userptr );
	static void				set_enableshadow	( void* userptr, bool  value );
	static xui_colour		get_shadowcolor		( void* userptr );
	static void				set_shadowcolor		( void* userptr, const xui_colour& value );
	static xui_vector<f64>	get_shadowoffset	( void* userptr );
	static void				set_shadowoffset	( void* userptr, const xui_vector<f64>& value );
	static bool				was_enableoutline	( void* userptr );
	static void				set_enableoutline	( void* userptr, bool  value );
	static f64				get_outlinewidth	( void* userptr );
	static void				set_outlinewidth	( void* userptr, f64   value );
	static xui_colour		get_outlinecolor	( void* userptr );
	static void				set_outlinecolor	( void* userptr, const xui_colour& value );

	/*
	//member
	*/
	cocos2d::ui::Text*		m_text;
	cocos_propfile*			m_fontfile;
};

#endif//__cocos_propnodetext_h__