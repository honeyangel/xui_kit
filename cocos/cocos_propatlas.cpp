#include "2d/CCSpriteFrameCache.h"
#include "renderer/CCTexture2D.h"
#include "renderer/CCTextureCache.h"
#include "base/CCDirector.h"
#include "xui_global.h"
#include "xui_propdata.h"
#include "xui_propkind.h"
#include "xui_kindctrl.h"
#include "xui_propctrl.h"
#include "xui_textbox.h"
#include "cocos_console.h"
#include "cocos_mainform.h"
#include "cocos_project.h"
#include "cocos_filedata.h"
#include "cocos_resource.h"
#include "cocos_propframe.h"
#include "cocos_propatlas.h"

/*
//constructor
*/
xui_create_explain(cocos_propatlas)( const std::wstring& full)
: cocos_propfile(cocos_resource::icon_module, full)
{
	get_subprop();
	get_texture();

	m_texkind = new xui_propkind(this, L"TextureInfo", "Atlas", xui_kindctrl::create, NULL, false, true);
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
//destructor
*/
xui_delete_explain(cocos_propatlas)( void )
{
	for (u32 i = 0; i < m_subprop.size(); ++i)
		delete m_subprop[i];
}

/*
//method
*/
xui_method_explain(cocos_propatlas, has_frame,		bool						)( const std::wstring& name )
{
	for (u32 i = 0; i < m_subprop.size(); ++i)
	{
		cocos_propframe* prop = dynamic_cast<cocos_propframe*>(m_subprop[i]);
		std::wstring framename = xui_global::ascii_to_unicode(prop->get_framename());
		if (xui_global::get_upper(framename).find(name) != -1)
			return true;
	}

	return false;
}
xui_method_explain(cocos_propatlas, get_texture,	cocos2d::Texture2D*			)( void )
{
	std::string pathname = xui_global::unicode_to_ascii(cocos_filedata::get_path(m_fullname));
	std::string safename = xui_global::unicode_to_ascii(cocos_filedata::get_safe(m_fullname));
	return cocos2d::Director::getInstance()->getTextureCache()->addImage(pathname+safename+".png");
}
xui_method_explain(cocos_propatlas, get_subprop,	std::vector<xui_proproot*>	)( void )
{
	if (m_subprop.empty())
	{
		std::string path = xui_global::unicode_to_ascii(cocos_filedata::get_path(m_fullname));
		std::string file = xui_global::unicode_to_ascii(cocos_filedata::get_file(m_fullname));
		cocos2d::sPlistDataWrapper data = cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesFromFile(path+file);
		if (data.frameVec.size() > 0)
		{
			std::set<std::string> nameset;
			for (u32 i = 0; i < data.frameVec.size(); ++i)
				nameset.insert(data.frameVec[i].frameName);

			for (std::set<std::string>::iterator itor = nameset.begin(); itor != nameset.end(); ++itor)
			{
				xui_proproot* prop = new cocos_propframe(cocos_resource::icon_module, this, (*itor));
				m_subprop.push_back(prop);
			}
		}
	}

	return m_subprop;
}
xui_method_explain(cocos_propatlas, ntf_modify,		void						)( void )
{
	cocos_propfile::ntf_modify();
	std::string pathname = xui_global::unicode_to_ascii(cocos_filedata::get_path(m_fullname));
	std::string safename = xui_global::unicode_to_ascii(cocos_filedata::get_safe(m_fullname));
	cocos2d::Director::getInstance()->getTextureCache()->reloadTexture(pathname+safename+".png");
}
xui_method_explain(cocos_propatlas, can_rename,		bool						)( void )
{
	return false;
}
xui_method_explain(cocos_propatlas, load,			void						)( void )
{
	if (m_subprop.size() > 0)
	{
		for (u32 i = 0; i < m_subprop.size(); ++i)
		{
			cocos_propframe* prop = dynamic_cast<cocos_propframe*>(m_subprop[i]);
			cocos2d::SpriteFrameCache::getInstance()->removeSpriteFrameByName(prop->get_framename());
			delete m_subprop[i];
		}
		m_subprop.clear();

		cocos_project* project = cocos_mainform::get_ptr()->get_project();
		project->ntf_load(this);
	}
}

/*
//static
*/
xui_method_explain(cocos_propatlas, get_texwidth,	f64							)( void* userptr )
{
	cocos_propatlas* prop = (cocos_propatlas*)userptr;
	cocos2d::Texture2D* texture = prop->get_texture();
	return (f64)texture->getPixelsWide();
}
xui_method_explain(cocos_propatlas, get_texheight,	f64							)( void* userptr )
{
	cocos_propatlas* prop = (cocos_propatlas*)userptr;
	cocos2d::Texture2D* texture = prop->get_texture();
	return (f64)texture->getPixelsHigh();
}