#include "xui_desktop.h"
#include "xui_dialog.h"
#include "xui_global.h"
#include "cocos_treedata.h"
#include "cocos_timedata.h"
#include "cocos_propleaf.h"
#include "cocos_filedata.h"
#include "cocos_filectrl.h"
#include "cocos_resource.h"
#include "cocos_savekind.h"
#include "cocos_propfile.h"

/*
//constructor
*/
xui_create_explain(cocos_propfile)( xui_bitmap* icon, const std::wstring& fullname )
: cocos_proproot()
, m_fullname(fullname)
, m_savekind(NULL)
{
	m_basekind = new xui_propkind(this, cocos_filedata::get_safe(fullname), "File", cocos_filectrl::create, icon, true);
	add_propkind(m_basekind);

	std::wstring suff = cocos_filedata::get_suff(fullname);
	if (suff.length() > 0)
	{
		m_savekind = new cocos_savekind(this, this);
		add_propkind(m_savekind);
	}
}

/*
//method
*/
xui_method_explain(cocos_propfile, get_fileicon,		xui_bitmap*			)( void )
{
	return m_basekind->get_icon();
}
xui_method_explain(cocos_propfile, get_fullname,		const std::wstring&	)( void ) const
{
	return m_fullname;
}
xui_method_explain(cocos_propfile, del_tempfile,		void				)( void )
{
	if (xui_global::has_file(m_fullname+L".tmp"))
	{
		xui_global::del_file(m_fullname+L".tmp");
	}
}

/*
//virtual
*/
xui_method_explain(cocos_propfile, get_dragtype,		std::string			)( void )
{
	return "";
}
xui_method_explain(cocos_propfile, get_dragdata,		void*				)( void )
{
	return NULL;
}
xui_method_explain(cocos_propfile, was_modify,			bool				)( void )
{
	return false;
}
xui_method_explain(cocos_propfile, can_rename,			bool				)( void )
{
	return true;
}
xui_method_explain(cocos_propfile, load,				void				)( void )
{}
xui_method_explain(cocos_propfile, save,				void				)( void )
{
	xui_global::set_fwatchclose();
	save_as(m_fullname, false);
	xui_global::set_fwatchstart(xui_global::get_workpath());
	del_tempfile();
}
xui_method_explain(cocos_propfile, auto_save,			void				)( void )
{
	xui_global::set_fwatchclose();
	save_as(m_fullname+L".tmp", true);
	xui_global::set_fwatchstart(xui_global::get_workpath());
}
xui_method_explain(cocos_propfile, ntf_modify,			void				)( void )
{
	if (was_modify())
	{
		std::wstringstream text;
		text << L"Asset file was changed extern.\n\'";
		text << get_fullname().c_str();
		text << L"\'\nDo you want to reload ?";
		xui_dialog* dialog = (xui_dialog*)xui_desktop::get_ins()->show_message(text.str(), 2);
		dialog->set_text(0, L"Reload" );
		dialog->set_text(1, L"Ignore");
		dialog->set_data(this);
		dialog->xm_accept += new xui_method_static<xui_method_args>(on_modify_reload);
	}
	else
	{
		load();
	}
}
xui_method_explain(cocos_propfile, ntf_rename,			void				)( const std::wstring& last, const std::wstring& curr )
{
	del_tempfile();
	int npos = m_fullname.find(last);
	m_fullname.replace(npos, last.length(), curr);

	m_basekind->set_name(cocos_filedata::get_safe(m_fullname));
	xui_kindctrl* kindctrl = m_basekind->get_ctrl();
	if (kindctrl)
		kindctrl->refresh();
}

/*
//override
*/
xui_method_explain(cocos_propfile, on_detach,			void				)( const xui_proproot_vec& proproot )
{
	if (was_modify())
	{
		bool notify = true;
		for (u32 i = 0; i < proproot.size(); ++i)
		{
			cocos_propfile* propfile = dynamic_cast<cocos_propfile*>(proproot[i]);
			if (propfile == NULL)
			{
				cocos_propleaf* propleaf = dynamic_cast<cocos_propleaf*>(proproot[i]);
				if (propleaf)
					propfile = propleaf->get_propfile();
			}

			if (propfile && propfile == this)
			{
				notify = false;
				break;
			}
		}

		if (notify)
		{
			std::wstringstream text;
			text << L"Asset file has Changed.\n\'";
			text << get_fullname().c_str();
			text << L"\'\n";
			xui_dialog* dialog = (xui_dialog*)xui_desktop::get_ins()->show_message(text.str(), 2);
			dialog->set_text(0, L"Apply" );
			dialog->set_text(1, L"Revert");
			dialog->set_data(this);
			dialog->xm_accept += new xui_method_static<xui_method_args>(on_notify_accept);
			dialog->xm_cancel += new xui_method_static<xui_method_args>(on_notify_cancel);
		}
	}
}

/*
//static
*/
xui_method_explain(cocos_propfile, on_notify_accept,	void				)( xui_component* sender, xui_method_args& args )
{
	cocos_propfile* propfile = (cocos_propfile*)sender->get_window()->get_data();
	propfile->save();
}
xui_method_explain(cocos_propfile, on_notify_cancel,	void				)( xui_component* sender, xui_method_args& args )
{
	cocos_propfile* propfile = (cocos_propfile*)sender->get_window()->get_data();
	propfile->load();
	propfile->del_tempfile();
}
xui_method_explain(cocos_propfile, on_modify_reload,	void				)( xui_component* sender, xui_method_args& args )
{
	cocos_propfile* propfile = (cocos_propfile*)sender->get_window()->get_data();
	propfile->load();
	propfile->del_tempfile();
}