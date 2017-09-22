#include "external/SpineNode.h"
#include "spine/SkeletonAnimation.h"

#include "xui_kindctrl.h"
#include "xui_global.h"
#include "cocos_propfile.h"
#include "cocos_filedata.h"
#include "cocos_mainform.h"
#include "cocos_project.h"
#include "cocos_propdata_spine.h"
#include "cocos_propdata_spineatlas.h"
#include "cocos_propnodespinenode.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodespinenode)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodecustom(file, node)
, m_spinenode(dynamic_cast<cocos2d::ui::SpineNode*>(node))
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
	add_spinenode	();
}

/*
//method
*/
xui_method_explain(cocos_propnodespinenode, get_spinenode,	cocos2d::ui::SpineNode*	)( void )
{
	return m_spinenode;
}
xui_method_explain(cocos_propnodespinenode, add_spinenode,	void					)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "SpineNode", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodespinenode>(this, &cocos_propnodespinenode::on_propchanged);
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
		L"DefaultAnim",
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

	add_propkind(kind);
}
xui_method_explain(cocos_propnodespinenode, get_spinefile,	cocos_propfile*			)( void )
{
	if (m_spinefile == NULL && m_spinenode->getSpinePath().length() > 0)
	{
		std::wstring full = xui_global::ascii_to_unicode(m_spinenode->getSpinePath());
		std::wstring path = cocos_filedata::get_path(full);
		std::wstring file = cocos_filedata::get_file(full);
		path = path.substr(0, path.length() - 1);

		cocos_project* project = cocos_mainform::get_ptr()->get_project();
		m_spinefile = project->get_pathfile(path, file);
	}

	return m_spinefile;
}
xui_method_explain(cocos_propnodespinenode, set_spinefile,	void					)( cocos_propfile* file )
{
	m_spinefile = file;
	if (m_spinefile)
	{
		m_spinenode->setSpinePath(xui_global::unicode_to_ascii(m_spinefile->get_fullname()));
		if (m_spinenode->getAtlasPath().length() == 0)
		{
			std::wstring full = m_spinefile->get_fullname();
			std::string  path = xui_global::unicode_to_ascii(cocos_filedata::get_path(full));
			std::string  safe = xui_global::unicode_to_ascii(cocos_filedata::get_safe(full));
			m_spinenode->setAtlasPath(path+safe+".atlas");
		}
	}
	else
	{
		m_spinenode->setSpinePath("");
	}

	xui_propkind* kind = get_propkind("SpineNode");
	if (kind)
	{
		xui_propdata* data = kind->get_propdata(L"DefaultAnim");
		if (data->get_ctrl())
			data->get_ctrl()->on_linkpropdata();
	}
}
xui_method_explain(cocos_propnodespinenode, get_atlasfile,	cocos_propfile*			)( void )
{
	if (m_atlasfile == NULL && m_spinenode->getAtlasPath().length() > 0)
	{
		std::wstring full = xui_global::ascii_to_unicode(m_spinenode->getAtlasPath());
		std::wstring path = cocos_filedata::get_path(full);
		std::wstring file = cocos_filedata::get_file(full);
		path = path.substr(0, path.length() - 1);

		cocos_project* project = cocos_mainform::get_ptr()->get_project();
		m_atlasfile = project->get_pathfile(path, file);
	}

	return m_atlasfile;
}
xui_method_explain(cocos_propnodespinenode, set_atlasfile,	void					)( cocos_propfile* file )
{
	m_atlasfile = file;
	if (m_atlasfile)
	{
		m_spinenode->setAtlasPath(xui_global::unicode_to_ascii(m_atlasfile->get_fullname()));
	}
	else
	{
		m_spinenode->setAtlasPath("");
	}
}

/*
//static
*/
xui_method_explain(cocos_propnodespinenode, get_spine,		void*					)( void* userptr )
{
	cocos_propnodespinenode* prop = (cocos_propnodespinenode*)userptr;
	return prop->get_spinefile();
}
xui_method_explain(cocos_propnodespinenode, set_spine,		void					)( void* userptr, void* value )
{
	cocos_propnodespinenode* prop = (cocos_propnodespinenode*)userptr;
	prop->set_spinefile((cocos_propfile*)value);
}
xui_method_explain(cocos_propnodespinenode, get_atlas,		void*					)( void* userptr )
{
	cocos_propnodespinenode* prop = (cocos_propnodespinenode*)userptr;
	return prop->get_atlasfile();
}
xui_method_explain(cocos_propnodespinenode, set_atlas,		void					)( void* userptr, void* value )
{
	cocos_propnodespinenode* prop = (cocos_propnodespinenode*)userptr;
	prop->set_atlasfile((cocos_propfile*)value);
}
xui_method_explain(cocos_propnodespinenode, was_loop,		bool					)( void* userptr )
{
	cocos_propnodespinenode* prop = (cocos_propnodespinenode*)userptr;
	return prop->get_spinenode()->getLoop();
}
xui_method_explain(cocos_propnodespinenode, set_loop,		void					)( void* userptr, bool  value )
{
	cocos_propnodespinenode* prop = (cocos_propnodespinenode*)userptr;
	prop->get_spinenode()->setLoop(value);
}
xui_method_explain(cocos_propnodespinenode, get_animvec,	cocos_string_vec		)( void* userptr )
{
	cocos_propnodespinenode* prop = (cocos_propnodespinenode*)userptr;
	cocos_string_vec vec;
	spine::SkeletonAnimation* skeletonAnim = prop->get_spinenode()->getSkeletonAnim();
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
xui_method_explain(cocos_propnodespinenode, get_anim,		std::wstring			)( void* userptr )
{
	cocos_propnodespinenode* prop = (cocos_propnodespinenode*)userptr;
	std::string name = prop->get_spinenode()->getDefaultAnimName();
	return xui_global::ascii_to_unicode(name);
}
xui_method_explain(cocos_propnodespinenode, set_anim,		void					)( void* userptr, const std::wstring& value )
{
	cocos_propnodespinenode* prop = (cocos_propnodespinenode*)userptr;
	prop->get_spinenode()->setDefaultAnimName(xui_global::unicode_to_ascii(value));
}