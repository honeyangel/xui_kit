#include "xui_convas.h"
#include "xui_global.h"
#include "xui_desktop.h"
#include "xui_numbbox.h"
#include "xui_toggle.h"
#include "xui_panel.h"
#include "xui_propview.h"
#include "cocos_propdata_scale9.h"

//////////////////////////////////////////////////////////////////////////
xui_create_explain(cocos_propdata_scale9)( xui_propkind* kind, const std::wstring& name, get_func userget, set_func userset, void* userptr )
: xui_propdata(kind, name, cocos_propctrl_scale9::create)
, m_userget(userget)
, m_userset(userset)
, m_userptr(userptr)
{}
xui_method_explain(cocos_propdata_scale9, get_value,				cocos_value_scale9	)( void ) const
{
	return (*m_userget)(m_userptr);
}
xui_method_explain(cocos_propdata_scale9, set_value,				void				)( const cocos_value_scale9& value )
{
	if (get_value() != value)
	{
		(*m_userset)(m_userptr, value);
		on_valuechanged();
	}
}

//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(cocos_propctrl_scale9, xui_propctrl);

/*
//constructor
*/
xui_create_explain(cocos_propctrl_scale9)( xui_propdata* propdata )
: xui_propctrl()
, m_dragline(SCALE9LINE_NONE)
, m_downline(0)
{
	xui_drawer*  namectrl = NULL;
	xui_control* editctrl = NULL;

	m_booledit = new xui_propedit_bool(this);
	namectrl = m_booledit->get_namectrl();
	editctrl = m_booledit->get_editctrl();
	xui_method_ptrcall(namectrl,	set_parent		)(this);
	xui_method_ptrcall(editctrl,	set_parent		)(this);
	m_widgetvec.push_back(namectrl);
	m_widgetvec.push_back(editctrl);

	m_editpane = new xui_panel(xui_vector<s32>(180, 164));
	xui_method_ptrcall(m_editpane, set_parent		)(this);
	xui_method_ptrcall(m_editpane, set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_editpane, set_drawcolor	)(false);
	xui_method_ptrcall(m_editpane, set_hscrollauto	)(false);
	xui_method_ptrcall(m_editpane, set_vscrollauto	)(false);
	xui_method_ptrcall(m_editpane, xm_mousedown		) += new xui_method_member<xui_method_mouse, cocos_propctrl_scale9>(this, &cocos_propctrl_scale9::on_editpanemousedown);
	xui_method_ptrcall(m_editpane, xm_mousemove		) += new xui_method_member<xui_method_mouse, cocos_propctrl_scale9>(this, &cocos_propctrl_scale9::on_editpanemousemove);
	xui_method_ptrcall(m_editpane, xm_mouserise		) += new xui_method_member<xui_method_mouse, cocos_propctrl_scale9>(this, &cocos_propctrl_scale9::on_editpanemouserise);
	xui_method_ptrcall(m_editpane, xm_perform		) += new xui_method_member<xui_method_args,  cocos_propctrl_scale9>(this, &cocos_propctrl_scale9::on_editpaneperform);
	xui_method_ptrcall(m_editpane, xm_renderself	) += new xui_method_member<xui_method_args,	 cocos_propctrl_scale9>(this, &cocos_propctrl_scale9::on_editpanerenderself);
	m_widgetvec.push_back(m_editpane);

	m_lnumedit = new xui_propedit_number(this, NT_UNSIGNEDINT, 1.0, false);
	namectrl = m_lnumedit->get_namectrl();
	editctrl = m_lnumedit->get_editctrl();
	xui_method_ptrcall(namectrl,	ini_component	)(true, false);
	xui_method_ptrcall(editctrl,	ini_component	)(ALIGNHORZ_L, ALIGNVERT_B, 0);
	xui_method_ptrcall(editctrl,	set_renderw		)(32);
	m_editpane->add_child(namectrl);
	m_editpane->add_child(editctrl);

	m_rnumedit = new xui_propedit_number(this, NT_UNSIGNEDINT, 1.0, false);
	namectrl = m_rnumedit->get_namectrl();
	editctrl = m_rnumedit->get_editctrl();
	xui_method_ptrcall(namectrl,	ini_component	)(true, false);
	xui_method_ptrcall(editctrl,	ini_component	)(0, ALIGNVERT_B, 0);
	xui_method_ptrcall(editctrl,	set_renderw		)(32);
	m_editpane->add_child(namectrl);
	m_editpane->add_child(editctrl);

	m_tnumedit = new xui_propedit_number(this, NT_UNSIGNEDINT, 1.0, false);
	namectrl = m_tnumedit->get_namectrl();
	editctrl = m_tnumedit->get_editctrl();
	xui_method_ptrcall(namectrl,	ini_component	)(true, false);
	xui_method_ptrcall(editctrl,	ini_component	)(ALIGNHORZ_R, ALIGNVERT_T, 0);
	xui_method_ptrcall(editctrl,	set_renderw		)(32);
	m_editpane->add_child(namectrl);
	m_editpane->add_child(editctrl);

	m_bnumedit = new xui_propedit_number(this, NT_UNSIGNEDINT, 1.0, false);
	namectrl = m_bnumedit->get_namectrl();
	editctrl = m_bnumedit->get_editctrl();
	xui_method_ptrcall(namectrl,	ini_component	)(true, false);
	xui_method_ptrcall(editctrl,	ini_component	)(ALIGNHORZ_R, 0, 0);
	xui_method_ptrcall(editctrl,	set_renderw		)(32);
	m_editpane->add_child(namectrl);
	m_editpane->add_child(editctrl);
}

/*
//destructor
*/
xui_delete_explain(cocos_propctrl_scale9)( void )
{
	delete m_booledit;
	delete m_lnumedit;
	delete m_rnumedit;
	delete m_tnumedit;
	delete m_bnumedit;
}

/*
//create
*/
xui_method_explain(cocos_propctrl_scale9, create,					xui_propctrl*	)( xui_propdata* propdata )
{
	return new cocos_propctrl_scale9(propdata);
}

/*
//propdata
*/
xui_method_explain(cocos_propctrl_scale9, on_linkpropdata,			void			)( bool selfupdate /* = false */ )
{
	if (selfupdate == false)
	{
		xui_drawer* namectrl = m_booledit->get_namectrl();
		namectrl->set_text(m_propdata->get_name());

		m_booledit->reset();
		m_lnumedit->reset();
		m_rnumedit->reset();
		m_tnumedit->reset();
		m_bnumedit->reset();
		m_lnumedit->get_editctrl()->set_enable(false);
		m_rnumedit->get_editctrl()->set_enable(false);
		m_tnumedit->get_editctrl()->set_enable(false);
		m_bnumedit->get_editctrl()->set_enable(false);
	}

	if (m_propdatavec.size() == 1)
	{
		cocos_propdata_scale9* datascale9 = dynamic_cast<cocos_propdata_scale9*>(m_propdata);
		cocos_value_scale9 value = datascale9->get_value();
		m_booledit->set_value(value.flag);
		m_lnumedit->set_value(value.rect.ax);
		m_rnumedit->set_value(value.rect.bx);
		m_tnumedit->set_value(value.rect.ay);
		m_bnumedit->set_value(value.rect.by);
		m_lnumedit->get_editctrl()->set_enable(value.flag);
		m_rnumedit->get_editctrl()->set_enable(value.flag);
		m_tnumedit->get_editctrl()->set_enable(value.flag);
		m_bnumedit->get_editctrl()->set_enable(value.flag);
	}
}
xui_method_explain(cocos_propctrl_scale9, on_editvalue,				void			)( xui_propedit* sender )
{
	cocos_value_scale9 value;
	value.flag		= m_booledit->get_value();
	value.rect.ax	= m_lnumedit->get_value();
	value.rect.bx	= m_rnumedit->get_value();
	value.rect.ay	= m_tnumedit->get_value();
	value.rect.by	= m_bnumedit->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		cocos_propdata_scale9* data = dynamic_cast<cocos_propdata_scale9*>(m_propdatavec[i]);
		data->set_value(value);
	}

	if (sender == m_booledit)
	{
		m_lnumedit->get_editctrl()->set_enable(value.flag);
		m_rnumedit->get_editctrl()->set_enable(value.flag);
		m_tnumedit->get_editctrl()->set_enable(value.flag);
		m_bnumedit->get_editctrl()->set_enable(value.flag);
	}
}

/*
//override
*/
xui_method_explain(cocos_propctrl_scale9, on_invalid,				void			)( xui_method_args& args )
{
	xui_vector<s32> sz;
	sz.w = m_border.ax + m_border.bx + m_editpane->get_renderw();
	sz.h = m_border.ay + m_border.by + m_editpane->get_renderh() + xui_propview::default_lineheight;
	sz.w = xui_max(sz.w, get_renderw());

	if (get_rendersz() != sz)
	{
		set_rendersz(sz);
	}
	else
	{
		perform();
	}
}
xui_method_explain(cocos_propctrl_scale9, on_perform,				void			)( xui_method_args& args )
{
	xui_propctrl::on_perform(args);
	xui_rect2d<s32> rt = get_renderrt();
	xui_vector<s32> pt;
	xui_drawer*  namectrl = m_booledit->get_namectrl();
	xui_control* boolctrl = m_booledit->get_editctrl();
	//middpane
	pt.x = rt.get_w()/2 - m_editpane->get_borderrt().ax;
	pt.y = xui_propview::default_lineheight;
	xui_method_ptrcall(m_editpane,	on_perform_pt	)(pt);
	//boolctrl
	pt.x = rt.get_w()/2;
	pt.y = xui_propview::default_lineheight/2 - boolctrl->get_renderh()/2;
	xui_method_ptrcall(boolctrl,	on_perform_pt	)(pt);
	//namectrl
	s32 indent = get_indent();
	xui_method_ptrcall(namectrl,	on_perform_w	)(rt.get_w()/2);
	xui_method_ptrcall(namectrl,	set_textoffset	)(xui_vector<s32>(indent, 0));
}

/*
//event
*/
xui_method_explain(cocos_propctrl_scale9, on_editpaneperform,		void			)( xui_component* sender, xui_method_args&  args )
{
	xui_rect2d<s32> rt = m_editpane->get_renderrtins();
	xui_control* rnumctrl = m_rnumedit->get_editctrl();
	xui_control* bnumctrl = m_bnumedit->get_editctrl();
	rnumctrl->on_perform_x(rt.ax+100-rnumctrl->get_renderw());
	bnumctrl->on_perform_y(rt.ay+100-bnumctrl->get_renderh());
}
xui_method_explain(cocos_propctrl_scale9, on_editpanerenderself,	void			)( xui_component* sender, xui_method_args&  args )
{
	xui_rect2d<s32> rt = m_editpane->get_renderrtins() + m_editpane->get_screenpt();
	rt.set_w(100);
	rt.set_h(100);
	xui_convas::get_ins()->fill_round(rt, xui_button::default_backcolor, 16);
	rt.bx -= 1;
	rt.by -= 1;
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax, rt.ay), xui_vector<s32>(rt.ax, rt.by+32), xui_button::default_movecolor);
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx, rt.ay), xui_vector<s32>(rt.bx, rt.by+32), xui_button::default_movecolor);
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax, rt.ay), xui_vector<s32>(rt.bx+32, rt.ay), xui_button::default_movecolor);
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax, rt.by), xui_vector<s32>(rt.bx+32, rt.by), xui_button::default_movecolor);

	if (m_propdatavec.size() == 1)
	{
		cocos_propdata_scale9* datascale9 = dynamic_cast<cocos_propdata_scale9*>(m_propdata);
		cocos_value_scale9 value = datascale9->get_value();
		s32 lpos = m_lnumedit->get_value() / value.size.w * 100.0;
		s32 rpos = m_rnumedit->get_value() / value.size.w * 100.0;
		s32 tpos = m_tnumedit->get_value() / value.size.h * 100.0;
		s32 bpos = m_bnumedit->get_value() / value.size.h * 100.0;
		xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax+lpos, rt.ay+1), xui_vector<s32>(rt.ax+lpos, rt.by+32), xui_button::default_downcolor);
		xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx-rpos, rt.ay+1), xui_vector<s32>(rt.bx-rpos, rt.by+32), xui_button::default_downcolor);
		xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax+1, rt.ay+tpos), xui_vector<s32>(rt.bx+32, rt.ay+tpos), xui_button::default_downcolor);
		xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax+1, rt.by-bpos), xui_vector<s32>(rt.bx+32, rt.by-bpos), xui_button::default_downcolor);

		if (lpos > 10)
		{
			xui_vector<s32> p1(rt.ax,		rt.by+28);
			xui_vector<s32> p2(rt.ax+lpos,	rt.by+28);
			xui_convas::get_ins()->draw_line(p1, p2, xui_button::default_movecolor);
			xui_convas::get_ins()->draw_line(p1, xui_vector<s32>(p1.x+4, p1.y-4), xui_button::default_movecolor);
			xui_convas::get_ins()->draw_line(p1, xui_vector<s32>(p1.x+4, p1.y+4), xui_button::default_movecolor);
			xui_convas::get_ins()->draw_line(p2, xui_vector<s32>(p2.x-4, p2.y-4), xui_button::default_movecolor);
			xui_convas::get_ins()->draw_line(p2, xui_vector<s32>(p2.x-4, p2.y+4), xui_button::default_movecolor);
		}
		if (rpos > 10)
		{
			xui_vector<s32> p1(rt.bx-rpos,	rt.by+28);
			xui_vector<s32> p2(rt.bx,		rt.by+28);
			xui_convas::get_ins()->draw_line(p1, p2, xui_button::default_movecolor);
			xui_convas::get_ins()->draw_line(p1, xui_vector<s32>(p1.x+4, p1.y-4), xui_button::default_movecolor);
			xui_convas::get_ins()->draw_line(p1, xui_vector<s32>(p1.x+4, p1.y+4), xui_button::default_movecolor);
			xui_convas::get_ins()->draw_line(p2, xui_vector<s32>(p2.x-4, p2.y-4), xui_button::default_movecolor);
			xui_convas::get_ins()->draw_line(p2, xui_vector<s32>(p2.x-4, p2.y+4), xui_button::default_movecolor);
		}
		if (tpos > 10)
		{
			xui_vector<s32> p1(rt.bx+28, rt.ay);
			xui_vector<s32> p2(rt.bx+28, rt.ay+tpos);
			xui_convas::get_ins()->draw_line(p1, p2, xui_button::default_movecolor);
			xui_convas::get_ins()->draw_line(p1, xui_vector<s32>(p1.x-4, p1.y+4), xui_button::default_movecolor);
			xui_convas::get_ins()->draw_line(p1, xui_vector<s32>(p1.x+4, p1.y+4), xui_button::default_movecolor);
			xui_convas::get_ins()->draw_line(p2, xui_vector<s32>(p2.x-4, p2.y-4), xui_button::default_movecolor);
			xui_convas::get_ins()->draw_line(p2, xui_vector<s32>(p2.x+4, p2.y-4), xui_button::default_movecolor);
		}
		if (bpos > 10)
		{
			xui_vector<s32> p1(rt.bx+28, rt.by-bpos);
			xui_vector<s32> p2(rt.bx+28, rt.by);
			xui_convas::get_ins()->draw_line(p1, p2, xui_button::default_movecolor);
			xui_convas::get_ins()->draw_line(p1, xui_vector<s32>(p1.x-4, p1.y+4), xui_button::default_movecolor);
			xui_convas::get_ins()->draw_line(p1, xui_vector<s32>(p1.x+4, p1.y+4), xui_button::default_movecolor);
			xui_convas::get_ins()->draw_line(p2, xui_vector<s32>(p2.x-4, p2.y-4), xui_button::default_movecolor);
			xui_convas::get_ins()->draw_line(p2, xui_vector<s32>(p2.x+4, p2.y-4), xui_button::default_movecolor);
		}
	}
}
xui_method_explain(cocos_propctrl_scale9, on_editpanemousedown,		void			)( xui_component* sender, xui_method_mouse& args )
{
	if (args.mouse == MB_L)
	{
		if (m_propdatavec.size() == 1 && m_booledit->get_value())
		{
			m_dragline = hit_line(args.point, &m_downline);
		}
	}
}
xui_method_explain(cocos_propctrl_scale9, on_editpanemousemove,		void			)( xui_component* sender, xui_method_mouse& args )
{
	if (m_dragline != SCALE9LINE_NONE)
	{
		cocos_propdata_scale9* datascale9 = dynamic_cast<cocos_propdata_scale9*>(m_propdata);
		cocos_value_scale9 value = datascale9->get_value();
		xui_vector<s32> delta = xui_desktop::get_ins()->get_mousecurr() - xui_desktop::get_ins()->get_mousedown();
		if (m_dragline == SCALE9LINE_L)
		{
			s32 lpos = m_downline + delta.x;
			s32 lnum = lpos / 100.0f * value.size.w;
			lnum = xui_min(lnum, value.size.w);
			lnum = xui_max(lnum, 0);
			m_lnumedit->set_value(lnum);
		}
		else 
		if (m_dragline == SCALE9LINE_R)
		{
			s32 rpos = m_downline - delta.x;
			s32 rnum = rpos / 100.0f * value.size.w;
			rnum = xui_min(rnum, value.size.w);
			rnum = xui_max(rnum, 0);
			m_rnumedit->set_value(rnum);
		}
		else
		if (m_dragline == SCALE9LINE_T)
		{
			s32 tpos = m_downline + delta.y;
			s32 tnum = tpos / 100.0f * value.size.h;
			tnum = xui_min(tnum, value.size.h);
			tnum = xui_max(tnum, 0);
			m_tnumedit->set_value(tnum);
		}
		else
		if (m_dragline == SCALE9LINE_B)
		{
			s32 bpos = m_downline - delta.y;
			s32 bnum = bpos / 100.0f * value.size.h;
			bnum = xui_min(bnum, value.size.h);
			bnum = xui_max(bnum, 0);
			m_bnumedit->set_value(bnum);
		}

		on_editvalue(NULL);
	}
	else
	{
		if (m_propdatavec.size() == 1 && m_booledit->get_value() && m_editpane->has_catch() == false)
		{
			u08 line = hit_line(args.point, NULL);
			switch (line)
			{
			case SCALE9LINE_L:
			case SCALE9LINE_R:
				m_editpane->set_cursor(CURSOR_WE);
				break;
			case SCALE9LINE_T:
			case SCALE9LINE_B:
				m_editpane->set_cursor(CURSOR_NS);
				break;
			default:
				m_editpane->set_cursor(CURSOR_DEFAULT);
				break;
			}
		}
	}
}
xui_method_explain(cocos_propctrl_scale9, on_editpanemouserise,		void			)( xui_component* sender, xui_method_mouse& args )
{
	if (args.mouse == MB_L)
	{
		m_dragline = SCALE9LINE_NONE;
		m_downline = 0;
	}
}

/*
//method
*/
xui_method_explain(cocos_propctrl_scale9, hit_line,					u08				)( const xui_vector<s32>& screenpt, s32* downline )
{
	xui_vector<s32> pt = m_editpane->get_renderpt(screenpt);
	xui_rect2d<s32> rt = m_editpane->get_renderrtins();
	rt.set_w(100);
	rt.set_h(100);
	rt.bx -= 1;
	rt.by -= 1;
	cocos_propdata_scale9* datascale9 = dynamic_cast<cocos_propdata_scale9*>(m_propdata);
	cocos_value_scale9 value = datascale9->get_value();
	s32 lpos = m_lnumedit->get_value() / value.size.w * 100.0;
	s32 rpos = m_rnumedit->get_value() / value.size.w * 100.0;
	s32 tpos = m_tnumedit->get_value() / value.size.h * 100.0;
	s32 bpos = m_bnumedit->get_value() / value.size.h * 100.0;
	if		(pt.x >= rt.ax+lpos-2 && pt.x <= rt.ax+lpos+2)
	{
		if (downline) *downline = lpos;
		return SCALE9LINE_L;
	}
	else if (pt.x >= rt.bx-rpos-2 && pt.x <= rt.bx-rpos+2)	
	{
		if (downline) *downline = rpos;
		return SCALE9LINE_R;
	}
	else if (pt.y >= rt.ay+tpos-2 && pt.y <= rt.ay+tpos+2)	
	{
		if (downline) *downline = tpos;
		return SCALE9LINE_T;
	}
	else if (pt.y >= rt.by-bpos-2 && pt.y <= rt.by-bpos+2)	
	{
		if (downline) *downline = bpos;
		return SCALE9LINE_B;
	}
	else													
	{
		return SCALE9LINE_NONE;
	}
}