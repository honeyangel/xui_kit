#include "xui_convas.h"
#include "xui_bitmap.h"
#include "xui_toggle.h"
#include "xui_textbox.h"
#include "xui_propview.h"
#include "xui_kindctrl.h"

xui_implement_rtti(xui_kindctrl, xui_control);

/*
//create
*/
const s32 xui_kindctrl::KIND_HEIGHT = 28;

xui_method_explain(xui_kindctrl, create,					xui_kindctrl*	)( xui_propkind* propkind )
{
	return new xui_kindctrl(propkind);
}

/*
//constructor
*/
xui_create_explain(xui_kindctrl)( xui_propkind* propkind )
: xui_control(xui_vector<s32>(256, KIND_HEIGHT))
{
	m_border	= xui_rect2d<s32>(2);
	m_propkind	= NULL;

	//icon
	m_iconctrl	= new xui_drawer  (xui_vector<s32>(24));
	xui_method_ptrcall(m_iconctrl, set_parent		)(this);
	m_widgetvec.push_back(m_iconctrl);

	//name
	m_namectrl	= new xui_textbox (xui_vector<s32>(128, 24));
	m_namectrl->xm_nonfocus		+= new xui_method_member<xui_method_args, xui_kindctrl>(this, &xui_kindctrl::on_namectrlnonfocus);
	m_namectrl->xm_textenter	+= new xui_method_member<xui_method_args, xui_kindctrl>(this, &xui_kindctrl::on_namectrltextenter);
	xui_method_ptrcall(m_namectrl, set_parent		)(this);
	xui_method_ptrcall(m_namectrl, set_borderrt		)(xui_rect2d<s32>(4, 2, 2, 2));
	xui_method_ptrcall(m_namectrl, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_namectrl, set_textalign	)(TEXTALIGN_LC);
	m_widgetvec.push_back(m_namectrl);

	//flag
	m_flagctrl	= new xui_toggle  (xui_vector<s32>(16), TOGGLE_NORMAL);
	xui_method_ptrcall(m_flagctrl, set_parent		)(this);
	xui_method_ptrcall(m_flagctrl, set_corner		)(3);
	xui_method_ptrcall(m_flagctrl, set_drawcolor	)(true);
	xui_method_ptrcall(m_flagctrl, set_backcolor	)(xui_colour::darkgray);
	m_flagctrl->xm_toggleclick	+= new xui_method_member<xui_method_args, xui_kindctrl>(this, &xui_kindctrl::on_flagctrlclick);
	m_widgetvec.push_back(m_flagctrl);

	//plus
	m_kindplus	= new xui_plusctrl(PLUSRENDER_NORMAL, true);
	xui_method_ptrcall(m_kindplus, set_parent		)(this);
	m_kindplus->xm_expand		+= new xui_method_member<xui_method_args, xui_kindctrl>(this, &xui_kindctrl::on_kindexpand);
	m_widgetvec.push_back(m_kindplus);

	//prop
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

/*
//method
*/
xui_method_explain(xui_kindctrl, was_expanded,				bool			)( void ) const
{
	return m_kindplus->was_expanded();
}
xui_method_explain(xui_kindctrl, set_expanded,				void			)( bool flag )
{
	m_kindplus->set_expanded(flag);
}

/*
//propkind
*/
xui_method_explain(xui_kindctrl, get_propkind,				xui_propkind*	)( void ) const
{
	return m_propkind;
}
xui_method_explain(xui_kindctrl, set_propkind,				void			)( xui_propkind* propkind )
{
	m_propkind = propkind;
	m_propkindvec.clear();
	m_propkindvec.push_back(m_propkind);

	for (u32 i = 0; i < m_propctrlvec.size(); ++i)
	{
		xui_propdata* propdata = m_propkind->get_propdata()[i];
		xui_propctrl* propctrl = m_propctrlvec[i];
		xui_method_ptrcall(propctrl, set_propdata)(propdata);
		xui_method_ptrcall(propdata, set_ctrl	 )(propctrl);
	}

	refresh();
}
xui_method_explain(xui_kindctrl, set_propkind,				void			)( const xui_propkind_vec& propkind )
{
	m_propkind	  = propkind.front();
	m_propkindvec = propkind;

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

	refresh();
}

/*
//override
*/
xui_method_explain(xui_kindctrl, on_invalid,				void			)( xui_method_args& args )
{
	if (m_propkind)
	{
		xui_method_ptrcall(m_namectrl, set_textfont	)(m_propkind->get_textfont());
		xui_method_ptrcall(m_namectrl, set_textdraw	)(m_propkind->get_textdraw());
		xui_method_ptrcall(m_namectrl, set_sidestyle)(m_propkind->xm_namechanged.count() >  0 ? SIDESTYLE_S            : SIDESTYLE_N);
		xui_method_ptrcall(m_namectrl, set_readonly	)(m_propkind->xm_namechanged.count() == 0);
		xui_method_ptrcall(m_namectrl, set_drawcolor)(m_propkind->xm_namechanged.count() >  0);
		xui_method_ptrcall(m_flagctrl, set_visible	)(m_propkind->xm_flagchanged.count() >  0);
		xui_method_ptrcall(m_kindplus, set_visible	)(m_propctrlvec.size() > 0);
		xui_method_ptrcall(m_iconctrl, ini_drawer	)(m_propkind->get_icon(), xui_vector<s32>(24));
		xui_method_ptrcall(m_namectrl, ini_textbox	)(m_propkind->get_name());
		xui_method_ptrcall(m_flagctrl, ini_toggle	)(m_propkind->get_flag());

		for (u32 i = 0; i < m_propctrlvec.size(); ++i)
		{
			m_propctrlvec[i]->set_visible(false);
		}
	}

	xui_propview* propview = xui_dynamic_cast(xui_propview, m_parent);
	xui_vector<s32> sz;
	sz.w  = get_renderw();
	sz.h  = KIND_HEIGHT;
	sz.h += m_border.ay + m_border.by;
	sz.h += get_elsectrlsize();

	if (m_propkind && m_kindplus->was_expanded())
	{
		const xui_propdata_vec& vec = m_propkind->get_propdata();
		for (u32 i = 0; i < m_propctrlvec.size(); ++i)
		{
			xui_propctrl* propctrl = m_propctrlvec[i];
			xui_method_ptrcall(propctrl, set_enable )(vec[i]->can_edit());
			xui_method_ptrcall(propctrl, set_visible)(vec[i]->can_show());
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
xui_method_explain(xui_kindctrl, on_perform,				void			)( xui_method_args& args )
{
	xui_control::on_perform(args);
	s32 indent = xui_propview::default_nodeindent;
	s32 height = KIND_HEIGHT;

	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt;
	//icon
	pt.x  = rt.ax +  indent;
	pt.y  = rt.ay + (height-m_iconctrl->get_renderh())/2;
	m_iconctrl->on_perform_pt(pt);
	//flag
	pt.x += m_iconctrl->get_renderw()+4;
	m_flagctrl->on_perform_pt(pt);
	//name
	pt.x += m_flagctrl->get_renderw()+4;
	pt.y  = rt.ay + (height-m_namectrl->get_renderh())/2;
	m_namectrl->on_perform_pt(pt);
	m_namectrl->on_perform_w (rt.bx-pt.x);
	//plus
	pt.x  = rt.ax + (indent-m_kindplus->get_renderw());
	pt.y  = rt.ay + (height-m_kindplus->get_renderh())/2;
	m_kindplus->on_perform_pt(pt);

	//prop
	if (m_kindplus->was_expanded())
	{
		pt.x = rt.ax;
		pt.y = rt.ay + height + get_prevctrlsize();
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

/*
//event
*/
xui_method_explain(xui_kindctrl, on_namectrlnonfocus,		void			)( xui_component* sender, xui_method_args& args )
{
	if (m_propkind)
	{
		m_namectrl->ini_textbox(m_propkind->get_name());
	}
}
xui_method_explain(xui_kindctrl, on_namectrltextenter,		void			)( xui_component* sender, xui_method_args& args )
{
	for (u32 i = 0; i < m_propkindvec.size(); ++i)
	{
		m_propkindvec[i]->xm_namechanged(sender, args);
	}
}
xui_method_explain(xui_kindctrl, on_flagctrlclick,			void			)( xui_component* sender, xui_method_args& args )
{
	for (u32 i = 0; i < m_propkindvec.size(); ++i)
	{
		m_propkindvec[i]->xm_flagchanged(sender, args);
	}
}
xui_method_explain(xui_kindctrl, on_kindexpand,				void			)( xui_component* sender, xui_method_args& args )
{
	invalid();
}

/*
//method
*/
xui_method_explain(xui_kindctrl, get_propdataall,			xui_propdata_vec)( u32 index )
{
	xui_propdata_vec result;
	for (u32 i = 0; i < m_propkindvec.size(); ++i)
	{
		const xui_propdata_vec& vec = m_propkindvec[i]->get_propdata();
		result.push_back(vec[index]);
	}

	return result;
}
xui_method_explain(xui_kindctrl, get_elsectrlsize,			s32				)( void )
{
	return 0;
}
xui_method_explain(xui_kindctrl, get_prevctrlsize,			s32				)( void )
{
	return 0;
}