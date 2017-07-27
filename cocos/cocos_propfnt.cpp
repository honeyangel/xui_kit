#include "2d/CCFontFNT.h"
#include "base/CCDirector.h"
#include "renderer/CCTextureCache.h"
#include "renderer/CCTexture2D.h"
#include "xui_global.h"
#include "xui_propkind.h"
#include "xui_kindctrl.h"
#include "cocos_propdata_texture.h"
#include "cocos_resource.h"
#include "cocos_mainform.h"
#include "cocos_inspector.h"
#include "cocos_propfnt.h"

/*
//constructor
*/
xui_create_explain(cocos_propfnt)( const std::wstring& full )
: cocos_propfile(cocos_resource::icon_transition, full)
{
	m_bmpfont = cocos2d::BMFontConfiguration::create(xui_global::unicode_to_ascii(full));
	m_texkind = new xui_propkind(this, L"TextureInfo", "BMFont", xui_kindctrl::create, NULL, false, true);
	m_texkind->add_propdata(new cocos_propdata_texture(
		m_texkind,
		L"Texture",
		get_texture,
		set_texture,
		this));

	add_propkind(m_texkind);
}

/*
//destructor
*/
xui_delete_explain(cocos_propfnt)( void )
{
	delete m_bmpfont;
}

/*
//method
*/
xui_method_explain(cocos_propfnt, get_tex,		cocos2d::Texture2D*	)( void )
{
	std::string name = m_bmpfont->getAtlasName();
	return cocos2d::Director::getInstance()->getTextureCache()->addImage(name);
}
xui_method_explain(cocos_propfnt, get_viewtext, std::string			)( void )
{
	std::wstring text;
	std::set<unsigned int>* charset = m_bmpfont->getCharacterSet();
	if (charset)
	{
		for (std::set<unsigned int>::iterator itor = charset->begin(); itor != charset->end(); ++itor)
		{
			unsigned int charid = (*itor);
			text.append(1, (wchar_t)charid);
			if (text.length() > 8)
				break;
		}
	}

	return xui_global::unicode_to_utf8(text);
}
xui_method_explain(cocos_propfnt, get_dragtype, std::string			)( void )
{
	return "FNT";
}
xui_method_explain(cocos_propfnt, get_dragdata, void*				)( void )
{
	return this;
}
xui_method_explain(cocos_propfnt, load,			void				)( void )
{
	if (get_ctrl())
	{
		cocos_inspector* inspector = cocos_mainform::get_ptr()->get_inspector();
		inspector->set_proproot(NULL);
	}

	m_bmpfont->initWithFNTfile(xui_global::unicode_to_ascii(m_fullname));
}

/*
//static
*/
xui_method_explain(cocos_propfnt, get_texture,	void*				)( void* userptr )
{
	cocos_propfnt* prop = (cocos_propfnt*)userptr;
	return prop->get_tex();
}
xui_method_explain(cocos_propfnt, set_texture, void					)( void* userptr, void* value )
{}