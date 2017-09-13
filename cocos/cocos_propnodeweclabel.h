#ifndef __cocos_propnodeweclabel_h__
#define __cocos_propnodeweclabel_h__

#include "cocos_propnodecustom.h"

namespace cocos2d
{
	namespace ui
	{
		class WeCLabel;
	}
}

class cocos_propfile;
class cocos_propnodeweclabel : public cocos_propnodecustom
{
public:
	/*
	//constructor
	*/
	cocos_propnodeweclabel( cocos_propcsd* file, cocos2d::Node* node );

	/*
	//method
	*/
	cocos2d::ui::WeCLabel*		get_weclabel		( void );
	void						add_weclabel		( void );
	cocos_propfile*				get_fontfile		( void );
	void						set_fontfile		( cocos_propfile* file );

protected:
	/*
	//static
	*/
	static std::wstring			get_text			( void* userptr );
	static void					set_text			( void* userptr, const std::wstring& value );
	static void*				get_font			( void* userptr );
	static void					set_font			( void* userptr, void* value );
	static f64					get_fontsize		( void* userptr );
	static void					set_fontsize		( void* userptr, f64   value );
	static xui_colour			get_fontdraw		( void* userptr );
	static void					set_fontdraw		( void* userptr, const xui_colour& value );
	static s32					get_textalignhorz	( void* userptr );
	static void					set_textalignhorz	( void* userptr, s32   value );
	static s32					get_textalignvert	( void* userptr );
	static void					set_textalignvert	( void* userptr, s32   value );
	static s32					get_overflow		( void* userptr );
	static void					set_overflow		( void* userptr, s32   value );
	static f64					get_chargrap		( void* userptr );
	static void					set_chargrap		( void* userptr, f64   value );
	static f64					get_linegrap		( void* userptr );
	static void					set_linegrap		( void* userptr, f64   value );
	static f64					get_maxline			( void* userptr );
	static void					set_maxline			( void* userptr, f64   value );
	static f64					get_globalzorder	( void* userptr );
	static void					set_globalzorder	( void* userptr, f64   value );
	static bool					was_bbcode			( void* userptr );
	static void					set_bbcode			( void* userptr, bool  value );
	static s32					get_effect			( void* userptr );
	static void					set_effect			( void* userptr, s32   value );
	static xui_colour			get_effectcolor		( void* userptr );
	static void					set_effectcolor		( void* userptr, const xui_colour& value );
	static f64					get_outlinewidth	( void* userptr );
	static void					set_outlinewidth	( void* userptr, f64   value );
	static xui_vector<f64>		get_shadowoffset	( void* userptr );
	static void					set_shadowoffset	( void* userptr, const xui_vector<f64>& value );
	static bool					was_gradient		( void* userptr );
	static void					set_gradient		( void* userptr, bool  value );
	static xui_colour			get_gradienttop		( void* userptr );
	static void					set_gradienttop		( void* userptr, const xui_colour& value );
	static xui_colour			get_gradientbottom	( void* userptr );
	static void					set_gradientbottom	( void* userptr, const xui_colour& value );

	/*
	//member
	*/
	cocos_propfile*				m_fontfile;
	cocos2d::ui::WeCLabel*		m_weclabel;
};

#endif//__cocos_propnodeweclabel_h__