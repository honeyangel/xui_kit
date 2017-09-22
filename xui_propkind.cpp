#include "xui_propkind.h"

/*
//constructor
*/
xui_create_explain(xui_propkind)( xui_proproot* root, const std::wstring& name, const std::string& type, xui_kind_newctrl func, xui_bitmap* icon, bool flag, bool headshow, bool tail, bool plusshow, const xui_family& textfont, const xui_family_render& textdraw )
{
	m_root		= root;
	m_func		= func;
	m_type		= type;
	m_ctrl		= NULL;
	m_show		= true;
	m_edit		= true;
	m_name		= name;
	m_icon		= icon;
	m_flag		= flag;
	m_tail		= tail;
	m_headshow	= headshow;
	m_plusshow	= plusshow;
	m_textfont	= textfont;
	m_textdraw	= textdraw;
}

/*
//destructor
*/
xui_delete_explain(xui_propkind)( void )
{
	for (u32 i = 0; i < m_propdata.size(); ++i)
		delete m_propdata[i];
}

/*
//method
*/
xui_method_explain(xui_propkind, get_root,		xui_proproot*			)( void ) const
{
	return m_root;
}
xui_method_explain(xui_propkind, get_type,		const std::string&		)( void ) const
{
	return m_type;
}
xui_method_explain(xui_propkind, get_name,		const std::wstring&		)( void ) const
{
	return m_name;
}
xui_method_explain(xui_propkind, set_name,		void					)( const std::wstring& name )
{
	m_name = name;
}
xui_method_explain(xui_propkind, get_func,		xui_kind_newctrl		)( void ) const
{
	return m_func;
}
xui_method_explain(xui_propkind, get_icon,		xui_bitmap*				)( void ) const
{
	return m_icon;
}
xui_method_explain(xui_propkind, set_icon,		void					)( xui_bitmap* icon )
{
	m_icon = icon;
}
xui_method_explain(xui_propkind, get_flag,		bool					)( void ) const
{
	return m_flag;
}
xui_method_explain(xui_propkind, set_flag,		void					)( bool flag )
{
	m_flag = flag;
}
xui_method_explain(xui_propkind, was_tail,		bool					)( void ) const
{
	return m_tail;
}
xui_method_explain(xui_propkind, was_headshow,	bool					)( void ) const
{
	return m_headshow;
}
xui_method_explain(xui_propkind, was_plusshow,	bool					)( void ) const
{
	return m_plusshow;
}
xui_method_explain(xui_propkind, get_textfont,	const xui_family&		)( void ) const
{
	return m_textfont;
}
xui_method_explain(xui_propkind, get_textdraw,	const xui_family_render&)( void ) const
{
	return m_textdraw;
}
xui_method_explain(xui_propkind, get_propdata,	const xui_propdata_vec&	)( void ) const
{
	return m_propdata;
}
xui_method_explain(xui_propkind, get_propdata,	xui_propdata*			)( const std::wstring& name )
{
	for (u32 i = 0; i < m_propdata.size(); ++i)
	{
		if (m_propdata[i]->get_name() == name)
			return m_propdata[i];
	}

	return NULL;
}
xui_method_explain(xui_propkind, add_propdata,	void					)( xui_propdata* propdata )
{
	m_propdata.push_back(propdata);
}

xui_method_explain(xui_propkind, get_ctrl,		xui_kindctrl*			)( void ) const
{
	return m_ctrl;
}
xui_method_explain(xui_propkind, set_ctrl,		void					)( xui_kindctrl* ctrl )
{
	m_ctrl = ctrl;
}
xui_method_explain(xui_propkind, non_ctrl,		void					)( void )
{
	m_ctrl = NULL;
	for (u32 i = 0; i < m_propdata.size(); ++i)
		m_propdata[i]->non_ctrl();
}
xui_method_explain(xui_propkind, can_show,		bool					)( void ) const
{
	return m_show;
}
xui_method_explain(xui_propkind, set_show,		void					)( bool flag )
{
	m_show = flag;
}
xui_method_explain(xui_propkind, can_edit,		bool					)( void ) const
{
	return m_edit;
}
xui_method_explain(xui_propkind, set_edit,		void					)( bool flag )
{
	m_edit = flag;
}