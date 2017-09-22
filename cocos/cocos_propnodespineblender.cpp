#include "external/SpineBlender.h"
#include "spine/SkeletonAnimation.h"
#include "xui_kindctrl.h"
#include "xui_global.h"
#include "xui_propctrl.h"
#include "xui_propctrl_vector.h"
#include "xui_textbox.h"
#include "cocos_mainform.h"
#include "cocos_propfile.h"
#include "cocos_project.h"
#include "cocos_filedata.h"
#include "cocos_propdata_spine.h"
#include "cocos_propdata_spineatlas.h"
#include "cocos_propdata_texture.h"
#include "cocos_propnodespineblender.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodespineblender)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodecustom(file, node)
, m_spineblender(dynamic_cast<cocos2d::ui::SpineBlender*>(node))
, m_spinefile(NULL)
, m_atlasfile(NULL)
{
	add_location	();
	add_anchor		();
	add_position	();
	add_size		(true, NULL);
	add_aligndock	(true);
	add_base		();
	add_touch		();
	add_common		();
	add_color		();
	add_advance		();
	add_callback	();
	add_userdata	();
	add_custom		();
	add_spineblender();
}

/*
//method
*/
xui_method_explain(cocos_propnodespineblender, get_spineblender,		cocos2d::ui::SpineBlender*	)( void )
{
	return m_spineblender;
}
xui_method_explain(cocos_propnodespineblender, add_spineblender,		void						)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "SpineBlender", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodespineblender>(this, &cocos_propnodespineblender::on_propchanged);
	kind->add_propdata(new cocos_propdata_spine(
		kind,
		L"Spine",
		get_spine,
		set_spine,
		this));
	kind->add_propdata(new cocos_propdata_spineatlas(
		kind,
		L"Atlas",
		get_atlas,
		set_atlas,
		this));
	kind->add_propdata(new cocos_propdata_spineanim(
		kind,
		L"AnimName",
		get_anim,
		set_anim,
		get_animvec,
		this));
	kind->add_propdata(new xui_propdata_bool(
		kind,
		L"Loop",
		xui_propctrl_bool::create,
		was_loop,
		set_loop,
		this));
	kind->add_propdata(new cocos_propdata_texture(
		kind,
		L"MaskTex",
		get_masktex,
		set_masktex,
		this));
	kind->add_propdata(new xui_propdata_bool(
		kind,
		L"RenderTargetEnable",
		xui_propctrl_bool::create,
		was_rendertarget,
		set_rendertarget,
		this));
	kind->add_propdata(new xui_propdata_vector(
		kind,
		L"RenderTargetSize",
		xui_propctrl_vector::create,
		get_rendertargetsize,
		set_rendertargetsize,
		this,
		NT_UNSIGNEDINT));
	kind->add_propdata(new xui_propdata_vector(
		kind,
		L"SpineOffset",
		xui_propctrl_vector::create,
		get_spineoffset,
		set_spineoffset,
		this,
		NT_INT));
	kind->add_propdata(new xui_propdata_number_func(
		kind,
		L"SpineScale",
		xui_propctrl_number::create,
		get_spinescale,
		set_spinescale,
		this,
		NT_FLOAT));

	add_propkind(kind);
}
xui_method_explain(cocos_propnodespineblender, get_spinefile,			cocos_propfile*				)( void )
{
	if (m_spinefile == NULL && m_spineblender->getSpinePath().length() > 0)
	{
		std::wstring full = xui_global::ascii_to_unicode(m_spineblender->getSpinePath());
		std::wstring path = cocos_filedata::get_path(full);
		std::wstring file = cocos_filedata::get_file(full);
		path = path.substr(0, path.length() - 1);

		cocos_project* project = cocos_mainform::get_ptr()->get_project();
		m_spinefile = project->get_pathfile(path, file);
	}

	return m_spinefile;
}
xui_method_explain(cocos_propnodespineblender, set_spinefile,			void						)( cocos_propfile* file )
{
	m_spinefile = file;
	if (m_spinefile)
	{
		m_spineblender->setSpinePath(xui_global::unicode_to_ascii(m_spinefile->get_fullname()));
		if (m_spineblender->getAtlasPath().length() == 0)
		{
			std::wstring full = m_spinefile->get_fullname();
			std::string  path = xui_global::unicode_to_ascii(cocos_filedata::get_path(full));
			std::string  safe = xui_global::unicode_to_ascii(cocos_filedata::get_safe(full));
			m_spineblender->setAtlasPath(path+safe+".atlas");
		}
	}
	else
	{
		m_spineblender->setSpinePath("");
	}

	xui_propkind* kind = get_propkind("SpineBlender");
	if (kind)
	{
		xui_propdata* data = kind->get_propdata(L"AnimName");
		if (data->get_ctrl())
			data->get_ctrl()->on_linkpropdata();
	}
}
xui_method_explain(cocos_propnodespineblender, get_atlasfile,			cocos_propfile*				)( void )
{
	if (m_atlasfile == NULL && m_spineblender->getAtlasPath().length() > 0)
	{
		std::wstring full = xui_global::ascii_to_unicode(m_spineblender->getAtlasPath());
		std::wstring path = cocos_filedata::get_path(full);
		std::wstring file = cocos_filedata::get_file(full);
		path = path.substr(0, path.length() - 1);

		cocos_project* project = cocos_mainform::get_ptr()->get_project();
		m_atlasfile = project->get_pathfile(path, file);
	}

	return m_atlasfile;
}
xui_method_explain(cocos_propnodespineblender, set_atlasfile,			void						)( cocos_propfile* file )
{
	m_atlasfile = file;
	if (m_atlasfile)
	{
		m_spineblender->setAtlasPath(xui_global::unicode_to_ascii(m_atlasfile->get_fullname()));
	}
	else
	{
		m_spineblender->setAtlasPath("");
	}
}

/*
//static
*/
xui_method_explain(cocos_propnodespineblender, get_spine,				void*						)( void* userptr )
{
	cocos_propnodespineblender* prop = (cocos_propnodespineblender*)userptr;
	return prop->get_spinefile();
}
xui_method_explain(cocos_propnodespineblender, set_spine,				void						)( void* userptr, void* value )
{
	cocos_propnodespineblender* prop = (cocos_propnodespineblender*)userptr;
	prop->set_spinefile((cocos_propfile*)value);
}
xui_method_explain(cocos_propnodespineblender, get_atlas,				void*						)( void* userptr )
{
	cocos_propnodespineblender* prop = (cocos_propnodespineblender*)userptr;
	return prop->get_atlasfile();
}
xui_method_explain(cocos_propnodespineblender, set_atlas,				void						)( void* userptr, void* value )
{
	cocos_propnodespineblender* prop = (cocos_propnodespineblender*)userptr;
	prop->set_atlasfile((cocos_propfile*)value);
}
xui_method_explain(cocos_propnodespineblender, get_animvec,				cocos_string_vec			)( void* userptr )
{
	cocos_propnodespineblender* prop = (cocos_propnodespineblender*)userptr;
	cocos_string_vec vec;
	spine::SkeletonAnimation* skeletonAnim = prop->get_spineblender()->getSkeletonAnim();
	if (skeletonAnim)
	{
		spSkeleton* skeleton = skeletonAnim->getSkeleton();
		for (s32 i = 0; i < skeleton->data->animationsCount; ++i)
		{
			std::string name = skeleton->data->animations[i]->name;
			vec.push_back(xui_global::ascii_to_unicode(name));
		}
	}

	return vec;
}
xui_method_explain(cocos_propnodespineblender, get_anim,				std::wstring				)( void* userptr )
{
	cocos_propnodespineblender* prop = (cocos_propnodespineblender*)userptr;
	std::string name = prop->get_spineblender()->getSpineAnimName();
	return xui_global::ascii_to_unicode(name);
}
xui_method_explain(cocos_propnodespineblender, set_anim,				void						)( void* userptr, const std::wstring& value )
{
	cocos_propnodespineblender* prop = (cocos_propnodespineblender*)userptr;
	prop->get_spineblender()->setSpineAnimName(xui_global::unicode_to_ascii(value));
}
xui_method_explain(cocos_propnodespineblender, was_loop,				bool						)( void* userptr )
{
	cocos_propnodespineblender* prop = (cocos_propnodespineblender*)userptr;
	return prop->get_spineblender()->getLoop();
}
xui_method_explain(cocos_propnodespineblender, set_loop,				void						)( void* userptr, bool  value )
{
	cocos_propnodespineblender* prop = (cocos_propnodespineblender*)userptr;
	prop->get_spineblender()->setLoop(value);
}
xui_method_explain(cocos_propnodespineblender, get_masktex,				void*						)( void* userptr )
{
	cocos_propnodespineblender* prop = (cocos_propnodespineblender*)userptr;
	std::string path = prop->get_spineblender()->getMaskPath();
	if (path.length() > 0)
		return cocos2d::Director::getInstance()->getTextureCache()->addImage(path);

	return NULL;
}
xui_method_explain(cocos_propnodespineblender, set_masktex,				void						)( void* userptr, void* value )
{
	cocos_propnodespineblender* prop = (cocos_propnodespineblender*)userptr;
	if (value == NULL)
	{
		prop->get_spineblender()->setMaskPath("");
	}
	else
	{
		cocos2d::Texture2D* texture = (cocos2d::Texture2D*)value;
		prop->get_spineblender()->setMaskPath(texture->getFileName());
	}
}
xui_method_explain(cocos_propnodespineblender, was_rendertarget,		bool						)( void* userptr )
{
	cocos_propnodespineblender* prop = (cocos_propnodespineblender*)userptr;
	return prop->get_spineblender()->getRenderTargetEnabled();
}
xui_method_explain(cocos_propnodespineblender, set_rendertarget,		void						)( void* userptr, bool  value )
{
	cocos_propnodespineblender* prop = (cocos_propnodespineblender*)userptr;
	prop->get_spineblender()->setRenderTargetEnabled(value);
}
xui_method_explain(cocos_propnodespineblender, get_rendertargetsize,	xui_vector<f64>				)( void* userptr )
{
	cocos_propnodespineblender* prop = (cocos_propnodespineblender*)userptr;
	f64 w = prop->get_spineblender()->getRenderTargetWidth();
	f64 h = prop->get_spineblender()->getRenderTargetHeight();
	return xui_vector<f64>(w, h);
}
xui_method_explain(cocos_propnodespineblender, set_rendertargetsize,	void						)( void* userptr, const xui_vector<f64>& value )
{
	cocos_propnodespineblender* prop = (cocos_propnodespineblender*)userptr;
	prop->get_spineblender()->setRenderTargetWidth(value.w);
	prop->get_spineblender()->setRenderTargetHeight(value.h);
}
xui_method_explain(cocos_propnodespineblender, get_spineoffset,			xui_vector<f64>				)( void* userptr )
{
	cocos_propnodespineblender* prop = (cocos_propnodespineblender*)userptr;
	f64 x = prop->get_spineblender()->getSpineOffsetX();
	f64 y = prop->get_spineblender()->getSpineOffsetY();
	return xui_vector<f64>(x, y);
}
xui_method_explain(cocos_propnodespineblender, set_spineoffset,			void						)( void* userptr, const xui_vector<f64>& value )
{
	cocos_propnodespineblender* prop = (cocos_propnodespineblender*)userptr;
	prop->get_spineblender()->setSpineOffsetX(value.x);
	prop->get_spineblender()->setSpineOffsetY(value.y);
}
xui_method_explain(cocos_propnodespineblender, get_spinescale,			f64							)( void* userptr )
{
	cocos_propnodespineblender* prop = (cocos_propnodespineblender*)userptr;
	return prop->get_spineblender()->getSpineScale();
}
xui_method_explain(cocos_propnodespineblender, set_spinescale,			void						)( void* userptr, f64   value )
{
	cocos_propnodespineblender* prop = (cocos_propnodespineblender*)userptr;
	prop->get_spineblender()->setSpineScale(value);
}