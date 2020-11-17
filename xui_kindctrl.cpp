#include "xui_canvas.h"
#include "xui_bitmap.h"
#include "xui_toggle.h"
#include "xui_textbox.h"
#include "xui_propview.h"
#include "xui_kindctrl.h"

xui_implement_rtti(xui_kindctrl, xui_control)

const s32 xui_kindctrl::k_kind_height = 28;

xui_kindctrl* xui_kindctrl::create( xui_propkind* propkind )
{
	return new xui_kindctrl(propkind);
}

xui_kindctrl::xui_kindctrl( xui_propkind* propkind )
: xui_control(xui_vector<s32>(256, k_kind_height))
{
	m_border	= xui_rect2d<s32>(2, 0, 2, 0);
	m_propkind	= NULL;

	//icon
	m_iconctrl	= new xui_drawer  (xui_vector<s32>(24));
	m_iconctrl->set_parent(this);
	m_widgetvec.push_back(m_iconctrl);

	//name
	m_namectrl	= new xui_textbox (xui_vector<s32>(128, 20));
	m_namectrl->xm_nonfocus		+= new xui_method_member<xui_method_args, xui_kindctrl>(this, &xui_kindctrl::on_namectrlnonfocus);
	m_namectrl->xm_textenter	+= new xui_method_member<xui_method_args, xui_kindctrl>(this, &xui_kindctrl::on_namectrltextenter);
	m_namectrl->set_parent(this);
	m_namectrl->set_borderrt(xui_rect2d<s32>(4, 0, 2, 0));
	m_namectrl->set_backcolor(xui_colour::k_darkgray);
	m_namectrl->set_textalign(k_textalign_lc);
	m_widgetvec.push_back(m_namectrl);

	//flag
	m_flagctrl	= new xui_toggle  (xui_vector<s32>(16), k_toggle_normal);
	m_flagctrl->set_parent(this);
	m_flagctrl->set_corner(3);
	m_flagctrl->set_drawcolor(true);
	m_flagctrl->set_backcolor(xui_colour::k_darkgray);
	m_flagctrl->xm_toggleclick	+= new xui_method_member<xui_method_args, xui_kindctrl>(this, &xui_kindctrl::on_flagctrlclick);
	m_widgetvec.push_back(m_flagctrl);

	//plus
	m_kindplus	= new xui_plusctrl(k_plusrender_normal, true);
	m_kindplus->set_parent(this);
	m_kindplus->xm_expand		+= new xui_method_member<xui_method_args, xui_kindctrl>(this, &xui_kindctrl::on_kindexpand);
	m_widgetvec.push_back(m_kindplus);

	//prop
	if (propkind)
	{
		m_namectrl->ini_component(true, propkind->was_headshow());

		const xui_propdata_vec& vec = propkind->get_propdata();
		for (u32 i = 0; i < vec.size(); ++i)
		{
			xui_prop_newctrl  func = vec[i]->get_func();
			xui_propctrl* propctrl = (*func)(vec[i]);
			propctrl->refresh();
			propctrl->set_parent(this);
			m_widgetvec.push_back(propctrl);
			m_propctrlvec.push_back(propctrl);
		}
	}
}


bool xui_kindctrl::was_expanded( void ) const
{
	return m_kindplus->was_expanded();
}

void xui_kindctrl::set_expanded( bool flag )
{
	m_kindplus->set_expanded(flag);
}

xui_propkind* xui_kindctrl::get_propkind( void ) const
{
	return m_propkind;
}

void xui_kindctrl::set_propkind( xui_propkind* propkind )
{
	m_propkind = propkind;
	m_propkindvec.clear();
	m_propkindvec.push_back(m_propkind);

	on_propkindchange();
	refresh();
}

void xui_kindctrl::set_propkind( const xui_propkind_vec& propkind )
{
	m_propkind	  = propkind.front();
	m_propkindvec = propkind;

	on_propkindchange();
	refresh();
}

void xui_kindctrl::on_invalid( xui_method_args& args )
{
	if (m_propkind)
	{
		m_namectrl->set_textfont (m_propkind->get_textfont());
		m_namectrl->set_textdraw (m_propkind->get_textdraw());
		m_namectrl->set_sidestyle(m_propkind->xm_namechanged.count() >  0 ? k_sidestyle_s : k_sidestyle_n);
		m_namectrl->set_readonly (m_propkind->xm_namechanged.count() == 0);
		m_namectrl->set_drawcolor(m_propkind->xm_namechanged.count() >  0);
		m_namectrl->set_visible	 (m_propkind->was_headshow());
		m_iconctrl->set_visible	 (m_propkind->was_headshow());
		m_flagctrl->set_visible	 (m_propkind->was_headshow() &&  m_propkind->xm_flagchanged.count() >  0);
		m_kindplus->set_visible	 (m_propkind->was_headshow() && (m_propkind->was_plusshow() || m_propctrlvec.size() > 0));
		m_iconctrl->ini_drawer	 (m_propkind->get_icon(), xui_vector<s32>(24));

		bool name_same = true;
		bool flag_same = true;
		std::wstring name = m_propkind->get_name();
		bool		 flag = m_propkind->get_flag();
		for (u32 i = 1; i < m_propkindvec.size(); ++i)
		{
			if (name_same && name != m_propkindvec[i]->get_name())
				name_same = false;
			if (flag_same && flag != m_propkindvec[i]->get_flag())
				flag_same = false;
		}
		m_namectrl->ini_textbox(name_same ? name : L"");
		m_flagctrl->ini_toggle (flag_same ? flag : false);

		for (u32 i = 0; i < m_propctrlvec.size(); ++i)
		{
			m_propctrlvec[i]->set_visible(false);
		}
	}

	xui_propview* propview = xui_dynamic_cast(xui_propview, m_parent);
	xui_vector<s32> sz;
	sz.w  = get_renderw();
	sz.h  = m_namectrl->was_visible() ? k_kind_height : 0;
	sz.h += m_border.ay + m_border.by;
	sz.h += get_elsectrlsize();

	if (m_kindplus->was_visible() == false)
	{
		xui_propview* propview = xui_dynamic_cast(xui_propview, get_parent());
		if (propview)
		{
			bool expand = true;
			for (u32 i = 0; i < propview->get_kindctrlcount(); ++i)
			{
				xui_kindctrl* kindctrl = xui_dynamic_cast(xui_kindctrl, propview->get_kindctrl(i));
				if (kindctrl == this)
				{
					m_kindplus->set_expanded(expand);
					break;
				}
				else
				{
					xui_propkind* propkind = kindctrl->get_propkind();
					if (propkind->was_headshow())
					{
						expand = kindctrl->was_expanded();
					}
				}
			}
		}
	}

	if (m_propkind && m_kindplus->was_expanded())
	{
		const xui_propdata_vec& vec = m_propkind->get_propdata();
		for (u32 i = 0; i < m_propctrlvec.size(); ++i)
		{
			xui_propctrl* propctrl = m_propctrlvec[i];
			propctrl->set_enable (vec[i]->can_edit());
			propctrl->set_visible(vec[i]->can_show());
			if (vec[i]->can_show() == false)
				continue;

			sz.h += propctrl->get_renderh();
		}
	}

	if (get_rendersz() != sz)
	{
		set_rendersz(sz);
	}
	else
	{
		perform();
	}
}

void xui_kindctrl::on_perform( xui_method_args& args )
{
	xui_control::on_perform(args);
	s32 indent = xui_propview::k_default_nodeindent;
	s32 height = k_kind_height;

	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt;
	//icon
	pt.x  = rt.ax +  indent;
	pt.y  = rt.ay + (height-m_iconctrl->get_renderh())/2;
	m_iconctrl->on_perform_pt(pt);
	//name
	pt.x += m_iconctrl->get_renderw()+4;
	pt.y  = rt.ay + (height-m_namectrl->get_renderh())/2;
	m_namectrl->on_perform_pt(pt);
	m_namectrl->on_perform_w (rt.bx - pt.x - (m_iconctrl->was_visible() ? (m_iconctrl->get_renderw()+4) : 0));
	//flag
	pt.x  = rt.bx - m_iconctrl->get_renderw();
	pt.y  = rt.ay + (height-m_flagctrl->get_renderh())/2;
	m_flagctrl->on_perform_pt(pt);
	//plus
	pt.x  = rt.ax + (indent-m_kindplus->get_renderw());
	pt.y  = rt.ay + (height-m_kindplus->get_renderh())/2;
	m_kindplus->on_perform_pt(pt);

	//prop
	if (m_kindplus->was_expanded())
	{
		pt.x = rt.ax;
		pt.y = rt.ay + get_prevctrlsize() + (m_namectrl->was_visible() ? k_kind_height : 0);
		for (u32 i = 0; i < m_propctrlvec.size(); ++i)
		{
			xui_propctrl* propctrl = m_propctrlvec[i];
			if (propctrl->was_visible() == false)
				continue;

			propctrl->on_perform_pt(pt);
			propctrl->on_perform_w (rt.get_w());
			pt.y += propctrl->get_renderh();
		}
	}
}

void xui_kindctrl::on_namectrlnonfocus( xui_component* sender, xui_method_args& args )
{
	if (m_propkind)
	{
		m_namectrl->ini_textbox(m_propkind->get_name());
	}
}

void xui_kindctrl::on_namectrltextenter( xui_component* sender, xui_method_args& args )
{
	for (u32 i = 0; i < m_propkindvec.size(); ++i)
	{
		m_propkindvec[i]->xm_namechanged(sender, args);
	}
}

void xui_kindctrl::on_flagctrlclick( xui_component* sender, xui_method_args& args )
{
	for (u32 i = 0; i < m_propkindvec.size(); ++i)
	{
		m_propkindvec[i]->xm_flagchanged(sender, args);
	}
}

void xui_kindctrl::on_kindexpand( xui_component* sender, xui_method_args& args )
{
	if (m_kindplus->was_visible())
	{
		invalid();

		xui_propview* propview = xui_dynamic_cast(xui_propview, get_parent());
		bool afterkind = false;
		for (u32 i = 0; i < propview->get_kindctrlcount(); ++i)
		{
			xui_kindctrl* kindctrl = xui_dynamic_cast(xui_kindctrl, propview->get_kindctrl(i));
			if (afterkind)
			{
				xui_propkind* propkind = kindctrl->get_propkind();
				if (propkind->was_headshow())
					break;
				else
					kindctrl->invalid();
			}
			else
			{
				if (kindctrl == this)
					afterkind = true;
			}
		}
	}
}

void xui_kindctrl::on_propkindchange( void )
{
	for (u32 i = 0; i < m_propctrlvec.size(); ++i)
	{
		xui_propdata_vec propdataall = get_propdataall(i);
		xui_propctrl*    propctrl    = m_propctrlvec[i];
		propctrl->set_propdata(propdataall);

		for (xui_propdata_vec::iterator itor = propdataall.begin(); itor != propdataall.end(); ++itor)
		{
			(*itor)->set_ctrl(propctrl);
		}
	}
}

xui_propdata_vec xui_kindctrl::get_propdataall( u32 index )
{
	xui_propdata_vec result;
	for (u32 i = 0; i < m_propkindvec.size(); ++i)
	{
		const xui_propdata_vec& vec = m_propkindvec[i]->get_propdata();
		result.push_back(vec[index]);
	}

	return result;
}

s32 xui_kindctrl::get_elsectrlsize( void )
{
	return 0;
}

s32 xui_kindctrl::get_prevctrlsize( void )
{
	return 0;
}
