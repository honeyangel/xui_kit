#include "xui_kindctrl.h"
#include "xui_dialog.h"
#include "xui_desktop.h"
#include "cocos_treedata.h"
#include "cocos_timedata.h"
#include "cocos_savekind.h"
#include "cocos_propfile.h"
#include "cocos_propleaf.h"

/*
//constructor
*/
xui_create_explain(cocos_propleaf)( cocos_propfile* propfile )
: cocos_proproot()
{
	m_savekind = new cocos_savekind(this, propfile);
	add_propkind(m_savekind);
}

/*
//method
*/
xui_method_explain(cocos_propleaf, get_propfile,	cocos_propfile*	)( void )
{
	return m_savekind->get_propfile();
}

/*
//virtual
*/
xui_method_explain(cocos_propleaf, get_dragtype,	std::string		)( void )
{
	return "";
}
xui_method_explain(cocos_propleaf, get_dragdata,	void*			)( void )
{
	return NULL;
}

/*
//override
*/
xui_method_explain(cocos_propleaf, on_detach,		void			)( const xui_proproot_vec& proproot )
{
	cocos_propfile* selffile = get_propfile();
	if (selffile->was_modify())
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

			if (propfile && propfile == selffile)
			{
				notify = false;
				break;
			}
		}

		if (notify)
		{
			std::wstringstream text;
			text << L"Asset file has Changed.\n\'";
			text << selffile->get_fullname().c_str();
			text << L"\'\n";
			xui_dialog* dialog = (xui_dialog*)xui_desktop::get_ins()->show_message(text.str(), 2);
			dialog->set_text(0, L"Apply" );
			dialog->set_text(1, L"Revert");
			dialog->set_data(selffile);
			dialog->xm_accept += new xui_method_static<xui_method_args>(cocos_propfile::on_notify_accept);
			dialog->xm_cancel += new xui_method_static<xui_method_args>(cocos_propfile::on_notify_cancel);
		}
	}
}