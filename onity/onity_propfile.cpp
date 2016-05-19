#include "xui_desktop.h"
#include "xui_dialog.h"
#include "onity_propleaf.h"
#include "onity_filedata.h"
#include "onity_filectrl.h"
#include "onity_resource.h"
#include "onity_savekind.h"
#include "onity_propfile.h"

/*
//constructor
*/
xui_create_explain(onity_propfile)( const std::wstring& full )
: xui_proproot()
, m_fullname(full)
{
	m_basekind = new xui_propkind(this, onity_filedata::get_safe(full), "File", onity_filectrl::create, onity_resource::icon_file, true);
	m_savekind = new onity_savekind(this, this);
	add_propkind(m_basekind);
	add_propkind(m_savekind);
}

/*
//method
*/
xui_method_explain(onity_propfile, get_full,			const std::wstring&	)( void ) const
{
	return m_fullname;
}

/*
//virtual
*/
xui_method_explain(onity_propfile, get_dragtype,		std::string			)( void )
{
	return "";
}
xui_method_explain(onity_propfile, get_dragdata,		void*				)( void )
{
	return NULL;
}
xui_method_explain(onity_propfile, was_modify,			bool				)( void )
{
	return false;
}
xui_method_explain(onity_propfile, load,				void				)( void )
{}
xui_method_explain(onity_propfile, save,				void				)( void )
{}
xui_method_explain(onity_propfile, ntf_rename,			void				)( const std::wstring& last, const std::wstring& curr )
{
	int npos = m_fullname.find(last);
	m_fullname.replace(npos, last.length(), curr);

	m_basekind->set_name(onity_filedata::get_safe(m_fullname));
	xui_kindctrl* kindctrl = m_basekind->get_ctrl();
	if (kindctrl)
		kindctrl->refresh();
}

/*
//override
*/
xui_method_explain(onity_propfile, on_detach,			void				)( const xui_proproot_vec& proproot )
{
	if (was_modify())
	{
		bool notify = true;
		for (u32 i = 0; i < proproot.size(); ++i)
		{
			onity_propfile* propfile = dynamic_cast<onity_propfile*>(proproot[i]);
			if (propfile == NULL)
			{
				onity_propleaf* propleaf = dynamic_cast<onity_propleaf*>(proproot[i]);
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
			text << get_full().c_str();
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
xui_method_explain(onity_propfile, on_notify_accept,	void				)( xui_component* sender, xui_method_args& args )
{
	onity_propfile* propfile = (onity_propfile*)sender->get_window()->get_data();
	propfile->save();
}
xui_method_explain(onity_propfile, on_notify_cancel,	void				)( xui_component* sender, xui_method_args& args )
{
	onity_propfile* propfile = (onity_propfile*)sender->get_window()->get_data();
	propfile->load();
}