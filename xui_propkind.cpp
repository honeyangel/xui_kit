#include "xui_global.h"
#include "xui_propkind.h"

xui_propkind::xui_propkind( xui_proproot* root, const std::wstring& name, const std::string& type, xui_kind_newctrl func, xui_bitmap* icon, bool flag, bool headshow, bool tail, bool plusshow, const xui_family& textfont, const xui_family_render& textdraw )
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

xui_propkind::~xui_propkind( void )
{
	for (u32 i = 0; i < m_propdata.size(); ++i)
		delete m_propdata[i];
}

xui_proproot* xui_propkind::get_root( void ) const
{
	return m_root;
}

const std::string& xui_propkind::get_type( void ) const
{
	return m_type;
}

const std::wstring& xui_propkind::get_name( void ) const
{
	return m_name;
}

void xui_propkind::set_name( const std::wstring& name )
{
	m_name = name;
}

xui_kind_newctrl xui_propkind::get_func( void ) const
{
	return m_func;
}

xui_bitmap* xui_propkind::get_icon( void ) const
{
	return m_icon;
}

void xui_propkind::set_icon( xui_bitmap* icon )
{
	m_icon = icon;
}

bool xui_propkind::get_flag( void ) const
{
	return m_flag;
}

void xui_propkind::set_flag( bool flag )
{
	m_flag = flag;
}

bool xui_propkind::was_tail( void ) const
{
	return m_tail;
}

bool xui_propkind::was_headshow( void ) const
{
	return m_headshow;
}

bool xui_propkind::was_plusshow( void ) const
{
	return m_plusshow;
}

const xui_family& xui_propkind::get_textfont( void ) const
{
	return m_textfont;
}

const xui_family_render& xui_propkind::get_textdraw( void ) const
{
	return m_textdraw;
}

const xui_propdata_vec& xui_propkind::get_propdata( void ) const
{
	return m_propdata;
}

xui_propdata* xui_propkind::get_propdata( const std::wstring& name )
{
	for (u32 i = 0; i < m_propdata.size(); ++i)
	{
		if (m_propdata[i]->get_name() == name)
			return m_propdata[i];
	}

	return NULL;
}

void xui_propkind::add_propdata( xui_propdata* propdata )
{
	m_propdata.push_back(propdata);
}

std::wstring xui_propkind::get_proppath( xui_propdata* propdata )
{
	for (u32 i = 0; i < m_propdata.size(); ++i)
	{
		std::wstring temp = m_propdata[i]->get_path(propdata);
		if (temp.length() > 0)
		{
			return xui_global::ascii_to_unicode(m_type)+temp;
		}
	}

	return L"";
}

xui_kindctrl* xui_propkind::get_ctrl( void ) const
{
	return m_ctrl;
}

void xui_propkind::set_ctrl( xui_kindctrl* ctrl )
{
	m_ctrl = ctrl;
}

void xui_propkind::non_ctrl( void )
{
	m_ctrl = NULL;
	for (u32 i = 0; i < m_propdata.size(); ++i)
		m_propdata[i]->non_ctrl();
}

bool xui_propkind::can_show( void ) const
{
	return m_show;
}

void xui_propkind::set_show( bool flag )
{
	m_show = flag;
}

bool xui_propkind::can_edit( void ) const
{
	return m_edit;
}

void xui_propkind::set_edit( bool flag )
{
	m_edit = flag;
}
