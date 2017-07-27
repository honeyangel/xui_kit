#include "renderer/CCTexture2D.h"
#include "renderer/CCTextureCache.h"
#include "base/CCDirector.h"
#include "xui_global.h"
#include "xui_kindctrl.h"
#include "xui_propctrl.h"
#include "xui_textbox.h"
#include "cocos_filedata.h"
#include "cocos_resource.h"
#include "cocos_proptexture.h"

/*
//constructor
*/
xui_create_explain(cocos_proptexture)( const std::wstring& full )
: cocos_propfile(cocos_resource::icon_texture, full)
{
	m_texkind = new xui_propkind(this, L"Infomation", "Texture2D", xui_kindctrl::create, cocos_resource::icon_setting, false);
	m_texkind->add_propdata(new xui_propdata_number_func(
		m_texkind, 
		L"Width", 
		xui_propctrl_number::create, 
		get_texwidth, 
		NULL, 
		this, 
		NT_INT));
	m_texkind->add_propdata(new xui_propdata_number_func(
		m_texkind,
		L"Height",
		xui_propctrl_number::create,
		get_texheight,
		NULL,
		this,
		NT_INT));
	add_propkind(m_texkind);
}

/*
//method
*/
xui_method_explain(cocos_proptexture, get_texture,	cocos2d::Texture2D*)( void )
{
	std::string pathname = xui_global::unicode_to_ascii(cocos_filedata::get_path(m_fullname));
	std::string filename = xui_global::unicode_to_ascii(cocos_filedata::get_file(m_fullname));
	return cocos2d::Director::getInstance()->getTextureCache()->addImage(pathname+filename);
}

/*
//override
*/
xui_method_explain(cocos_proptexture, get_dragtype,		std::string		)( void )
{
	return "Texture2D";
}
xui_method_explain(cocos_proptexture, get_dragdata,		void*			)( void )
{
	return get_texture();
}
xui_method_explain(cocos_proptexture, ntf_modify,		void			)( void )
{
	std::string pathname = xui_global::unicode_to_ascii(cocos_filedata::get_path(m_fullname));
	std::string filename = xui_global::unicode_to_ascii(cocos_filedata::get_file(m_fullname));
	cocos2d::Director::getInstance()->getTextureCache()->reloadTexture(pathname+filename);
}
xui_method_explain(cocos_proptexture, can_rename,		bool			)( void )
{
	return false;
}

/*
//static
*/
xui_method_explain(cocos_proptexture, get_texwidth,		f64				)( void* user )
{
	cocos_proptexture* prop = (cocos_proptexture*)user;
	cocos2d::Texture2D* texture = prop->get_texture();
	if (texture)
		return (f64)texture->getPixelsWide();

	return 0.0;
}
xui_method_explain(cocos_proptexture, get_texheight,	f64				)( void* user )
{
	cocos_proptexture* prop = (cocos_proptexture*)user;
	cocos2d::Texture2D* texture = prop->get_texture();
	if (texture)
		return (f64)texture->getPixelsHigh();

	return 0.0;
}