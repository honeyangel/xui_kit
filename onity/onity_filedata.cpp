#include "NPFileName.h"

#include "xui_global.h"
#include "xui_treenode.h"
#include "xui_proproot.h"
#include "xui_kindctrl.h"
#include "onity_project.h"
#include "onity_mainform.h"
#include "onity_resource.h"
#include "onity_propfile.h"
#include "onity_proptexture.h"
#include "onity_propparticle.h"
#include "onity_propmodule.h"
#include "onity_propsprite.h"
#include "onity_propaction.h"
#include "onity_propcontroller.h"
#include "onity_propjsones.h"
#include "onity_propcourse.h"
#include "onity_filedata.h"

/*
//constructor
*/
xui_create_explain(onity_filedata)( xui_bitmap* icon, const std::wstring& full, xui_proproot* prop )
: onity_treedata(icon, prop)
{
	m_text = full;
}

/*
//method
*/
xui_method_explain(onity_filedata, get_path,		std::wstring		)( void ) const
{
	return get_path(m_text);
}
xui_method_explain(onity_filedata, get_file,		std::wstring		)( void ) const
{
	return get_file(m_text);
}
xui_method_explain(onity_filedata, get_suff,		std::wstring		)( void ) const
{
	return get_suff(m_text);
}
xui_method_explain(onity_filedata, get_full,		const std::wstring&	)( void ) const
{
	return m_text;
}
xui_method_explain(onity_filedata, set_full,		void				)( const std::wstring& full )
{
	m_text = full;
	m_node->use_linkdata();
}

/*
//override
*/
xui_method_explain(onity_filedata, get_icon,		xui_bitmap*			)( u32 index )
{
	if (m_text == L"None")
		return NULL;

	return m_icon;
}
xui_method_explain(onity_filedata, get_text,		std::wstring		)( u32 index )
{
	return get_safe(m_text);
}
xui_method_explain(onity_filedata, set_text,		void				)( u32 index, const std::wstring& text )
{
	onity_propfile* propfile = dynamic_cast<onity_propfile*>(m_prop);
	if (propfile && propfile->can_rename() == false)
		return;

	xui_global::set_fwatchclose();
	std::wstring suff = get_suff();
	std::wstring curr = get_path()+text+suff;
	std::wstring last = m_text;
	if (xui_global::rna_file(last, curr))
	{
		ntf_rename(last, curr);
		onity_project* project = onity_mainform::get_ptr()->get_project();
		project->ntf_rename(last, curr);
	}
	xui_global::set_fwatchstart(xui_global::get_workpath());
}

/*
//virtual
*/
xui_method_explain(onity_filedata, ntf_rename,		void				)( const std::wstring& last, const std::wstring& curr )
{
	int npos = m_text.find(last);
	m_text.replace(npos, last.length(), curr);
	m_node->use_linkdata();

	onity_propfile* propfile = dynamic_cast<onity_propfile*>(m_prop);
	if (propfile)
		propfile->ntf_rename(last, curr);
}

/*
//static
*/
xui_method_explain(onity_filedata, get_path,		std::wstring		)( const std::wstring& full )
{
	std::wstring  temp = full;

	int index  = temp.find_last_of(L'/');
	if (index == -1)
		index  = temp.find_last_of(L'\\');
	if (index == -1)
		return L"";

	return temp.erase(index+1);
}
xui_method_explain(onity_filedata, get_file,		std::wstring		)( const std::wstring& full )
{
	std::wstring temp = full;

	int npos  = temp.find_last_of(L'/');
	if (npos == -1)
		npos  = temp.find_last_of(L'\\');
	if (npos != -1)
		temp.erase(0, npos+1);

	return temp;
}
xui_method_explain(onity_filedata, get_suff,		std::wstring		)( const std::wstring& full )
{
	std::wstring temp = full;
	int npos  = temp.find_last_of('.');
	if (npos == -1)
		temp = L"";
	else
		temp.erase(0, npos);

	if (temp == L".PNG" || temp == L".Png")
		temp =  L".png";

	return temp;
}
xui_method_explain(onity_filedata, get_safe,		std::wstring		)( const std::wstring& full )
{
	std::wstring temp = get_file(full);

	int npos  = temp.find_last_of(L'.');
	if (npos != -1)
		temp.erase(npos);

	return temp;
}
xui_method_explain(onity_filedata, new_fileprop,	xui_proproot*		)( const std::wstring& full )
{
	std::wstring suff = get_suff(full);

	u08 type = META_NONE;
	if		(suff == L".npModule") type = META_MODULE;
	else if (suff == L".npSprite") type = META_SPRITE;
	else if (suff == L".npAction") type = META_ACTION;
	else
	{}

	if (type != META_NONE)
		onity_prop2dsres::load_meta(type, xui_global::unicode_to_ascii(full));

	if (suff == L".png" && full.find(L"MODULE") != -1 && full.find(L"BIG") == -1)
		return NULL;

	onity_propfile* propfile = NULL;
	if		(suff == L".png")			propfile = new onity_proptexture	(full);
	else if (suff == L".npModule")		propfile = new onity_propmodule		(full);
	else if (suff == L".npSprite")		propfile = new onity_propsprite		(full);
	else if (suff == L".npAction")		propfile = new onity_propaction		(full);
	else if (suff == L".npCourse")		propfile = new onity_propcourse		(full);
	else if (suff == L".particle")		propfile = new onity_propparticle	(full);
	else if (suff == L".controller")	propfile = new onity_propcontroller	(full);
	else if (suff == L".json")			propfile = new onity_propjsones		(full);
	else
	{}

	if (propfile && xui_global::has_file(full+L".tmp"))
		onity_mainform::get_ptr()->add_unsavedfile(full);

	return propfile;
}