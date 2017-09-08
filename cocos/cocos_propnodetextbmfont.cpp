#include "ui/UITextBMFont.h"
#include "cocostudio/CocosStudioExtension.h"

#include "xui_global.h"
#include "xui_kindctrl.h"
#include "cocos_propfnt.h"
#include "cocos_resource.h"
#include "cocos_filedata.h"
#include "cocos_mainform.h"
#include "cocos_project.h"
#include "cocos_propdata_fnt.h"
#include "cocos_propnodetextbmfont.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodetextbmfont)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodewidget(cocos_resource::icon_window, file, node)
, m_textbmfont(dynamic_cast<cocos2d::ui::TextBMFont*>(node))
{
	add_location	();
	add_anchor		();
	add_position	();
	add_size		(false, NULL);
	add_aligndock	(false);
	add_base		();
	add_touch		();
	add_common		();
	add_color		();
	add_special		();
	add_textbmfont	();
	add_advance		();
	add_callback	();
	add_userdata	();
}

/*
//method
*/
xui_method_explain(cocos_propnodetextbmfont, get_textbmfont,	cocos2d::ui::TextBMFont*)( void )
{
	return m_textbmfont;
}
xui_method_explain(cocos_propnodetextbmfont, get_fontfile,		cocos_propfile*			)( void )
{
	cocos2d::ResourceData data = m_textbmfont->getRenderFile();
	if (m_fontfile == NULL && data.file.length() > 0)
	{
		std::wstring full = xui_global::ascii_to_unicode(data.file);
		std::wstring path = cocos_filedata::get_path(full);
		std::wstring file = cocos_filedata::get_file(full);
		path = path.substr(0, path.length() - 1);

		cocos_project* project = cocos_mainform::get_ptr()->get_project();
		m_fontfile = project->get_pathfile(path, file);
	}

	return m_fontfile;
}
xui_method_explain(cocos_propnodetextbmfont, set_fontfile,		void					)( cocos_propfile* file )
{
	m_fontfile = file;
	if (m_fontfile)
	{
		m_textbmfont->setFntFile(xui_global::unicode_to_ascii(m_fontfile->get_fullname()));
	}
	else
	{
		m_textbmfont->setFntFile("");
	}
}
xui_method_explain(cocos_propnodetextbmfont, add_textbmfont,	void					)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "TextBMFont", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodetextbmfont>(this, &cocos_propnodetextbmfont::on_propchanged);
	kind->add_propdata(new xui_propdata_string(
		kind,
		L"Text",
		xui_propctrl_string::create,
		get_string,
		set_string,
		this));
	kind->add_propdata(new cocos_propdata_fnt(
		kind,
		L"FontFile",
		get_font,
		set_font,
		this));

	add_propkind(kind);
}

/*
//static
*/
xui_method_explain(cocos_propnodetextbmfont, get_font,			void*					)( void* userptr )
{
	cocos_propnodetextbmfont* prop = (cocos_propnodetextbmfont*)userptr;
	return prop->get_fontfile();
}
xui_method_explain(cocos_propnodetextbmfont, set_font,			void					)( void* userptr, void* value )
{
	cocos_propnodetextbmfont* prop = (cocos_propnodetextbmfont*)userptr;
	prop->set_fontfile((cocos_propfile*)value);
}
xui_method_explain(cocos_propnodetextbmfont, get_string,		std::wstring			)( void* userptr )
{
	cocos_propnodetextbmfont* prop = (cocos_propnodetextbmfont*)userptr;
	return xui_global::ascii_to_unicode(prop->get_textbmfont()->getString());
}
xui_method_explain(cocos_propnodetextbmfont, set_string,		void					)( void* userptr, const std::wstring& value )
{
	cocos_propnodetextbmfont* prop = (cocos_propnodetextbmfont*)userptr;
	prop->get_textbmfont()->setString(xui_global::unicode_to_ascii(value));
}