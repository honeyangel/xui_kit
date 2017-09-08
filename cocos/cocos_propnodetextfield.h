#ifndef __cocos_propnodetextfield_h__
#define __cocos_propnodetextfield_h__

#include "cocos_propnodewidget.h"
#include "cocos_propdata_bs.h"

namespace cocos2d
{
	namespace ui
	{
		class TextField;
	}
}

class cocos_propfile;
class cocos_propnodetextfield : public cocos_propnodewidget
{
public:
	/*
	//constructor
	*/
	cocos_propnodetextfield( cocos_propcsd* file, cocos2d::Node* node );

	/*
	//method
	*/
	cocos2d::ui::TextField*		get_textfield		( void );
	cocos_propfile*				get_fontfile		( void );
	void						set_fontfile		( cocos_propfile* file );

protected:
	/*
	//method
	*/
	void						add_textfield		( void );

	/*
	//static
	*/
	static cocos_value_unitvec	def_size			( void* userptr );
	static std::wstring			get_placehold		( void* userptr );
	static void					set_placehold		( void* userptr, const std::wstring& value );
	static std::wstring			get_string			( void* userptr );
	static void					set_string			( void* userptr, const std::wstring& value );
	static void*				get_font			( void* userptr );
	static void					set_font			( void* userptr, void* value );
	static f64					get_fontsize		( void* userptr );
	static void					set_fontsize		( void* userptr, f64 value );
	static xui_colour			get_fontdraw		( void* userptr );
	static void					set_fontdraw		( void* userptr, const xui_colour& value );
	static cocos_value_bs		get_textlength		( void* userptr );
	static void					set_textlength		( void* userptr, const cocos_value_bs& value );
	static cocos_value_bs		get_password		( void* userptr );
	static void					set_password		( void* userptr, const cocos_value_bs& value );

	/*
	//member
	*/
	cocos_propfile*				m_fontfile;
	cocos2d::ui::TextField*		m_textfield;
};

#endif//__cocos_propnodetextfield_h__