#include "xui_dropbox.h"
#include "xui_propedit.h"
#include "xui_propview.h"
#include "onity_propctrl_pivot.h"

xui_implement_rtti(onity_propctrl_pivot, xui_propctrl_vector);

/*
//constructor
*/
xui_create_explain(onity_propctrl_pivot)( xui_propdata* propdata )
: xui_propctrl_vector(propdata)
{
	xui_textbox* subxtext = xui_dynamic_cast(xui_textbox, m_subxedit->get_editctrl());
	xui_textbox* subytext = xui_dynamic_cast(xui_textbox, m_subyedit->get_editctrl());
	subxtext->xm_textchanged	+= new xui_method_member<xui_method_args, onity_propctrl_pivot>(this, &onity_propctrl_pivot::on_editctrltextchanged);
	subytext->xm_textchanged	+= new xui_method_member<xui_method_args, onity_propctrl_pivot>(this, &onity_propctrl_pivot::on_editctrltextchanged);

	m_subxenum = new xui_dropbox(xui_vector<s32>(128, 18), NULL);
	xui_method_ptrcall(m_subxenum, set_parent	)(this);
	xui_method_ptrcall(m_subxenum, set_backcolor)(xui_colour::darkgray);
	xui_method_ptrcall(m_subxenum, set_drawcolor)(true);
	xui_method_ptrcall(m_subxenum, set_borderrt	)(xui_rect2d<s32>(4, 2, 0, 2));
	xui_method_ptrcall(m_subxenum, set_sidestyle)(SIDESTYLE_S);
	xui_method_ptrcall(m_subxenum, set_corner	)(3);
	xui_method_ptrcall(m_subxenum, set_readonly	)(true);
	xui_method_ptrcall(m_subxenum, add_item		)(L"None");
	xui_method_ptrcall(m_subxenum, add_item		)(L"Left");
	xui_method_ptrcall(m_subxenum, add_item		)(L"Right");
	xui_method_ptrcall(m_subxenum, add_item		)(L"Center");
	m_subxenum->xm_nonfocus		+= new xui_method_member<xui_method_args, onity_propctrl_pivot>(this, &onity_propctrl_pivot::on_enumctrlnonfocus);
	m_subxenum->xm_getfocus		+= new xui_method_member<xui_method_args, onity_propctrl_pivot>(this, &onity_propctrl_pivot::on_enumctrlgetfocus);
	m_subxenum->xm_selection	+= new xui_method_member<xui_method_args, onity_propctrl_pivot>(this, &onity_propctrl_pivot::on_enumctrlselection);

	m_subyenum = new xui_dropbox(xui_vector<s32>(128, 18), NULL);
	xui_method_ptrcall(m_subyenum, set_parent	)(this);
	xui_method_ptrcall(m_subyenum, set_backcolor)(xui_colour::darkgray);
	xui_method_ptrcall(m_subyenum, set_drawcolor)(true);
	xui_method_ptrcall(m_subyenum, set_borderrt	)(xui_rect2d<s32>(4, 2, 0, 2));
	xui_method_ptrcall(m_subyenum, set_sidestyle)(SIDESTYLE_S);
	xui_method_ptrcall(m_subyenum, set_corner	)(3);
	xui_method_ptrcall(m_subyenum, set_readonly	)(true);
	xui_method_ptrcall(m_subyenum, add_item		)(L"None");
	xui_method_ptrcall(m_subyenum, add_item		)(L"Top");
	xui_method_ptrcall(m_subyenum, add_item		)(L"Bottom");
	xui_method_ptrcall(m_subyenum, add_item		)(L"Center");
	m_subyenum->xm_nonfocus		+= new xui_method_member<xui_method_args, onity_propctrl_pivot>(this, &onity_propctrl_pivot::on_enumctrlnonfocus);
	m_subyenum->xm_getfocus		+= new xui_method_member<xui_method_args, onity_propctrl_pivot>(this, &onity_propctrl_pivot::on_enumctrlgetfocus);
	m_subyenum->xm_selection	+= new xui_method_member<xui_method_args, onity_propctrl_pivot>(this, &onity_propctrl_pivot::on_enumctrlselection);

	m_widgetvec.push_back(m_subxenum);
	m_widgetvec.push_back(m_subyenum);

	on_perform_h(xui_propview::default_lineheight*2);
}

/*
//create
*/
xui_method_explain(onity_propctrl_pivot, create,					xui_propctrl*	)( xui_propdata* propdata )
{
	return new onity_propctrl_pivot(propdata);
}

/*
//propdata
*/
xui_method_explain(onity_propctrl_pivot, on_linkpropdata,			void			)( bool selfupdate )
{
	xui_propctrl_vector::on_linkpropdata(selfupdate);

	xui_method_args args;
	on_editctrltextchanged(m_subxedit->get_editctrl(), args);
	on_editctrltextchanged(m_subyedit->get_editctrl(), args);
}

/*
//override
*/
xui_method_explain(onity_propctrl_pivot, on_perform,				void			)( xui_method_args& args )
{
	xui_propctrl_vector::on_perform(args);
	xui_control* subxedit = m_subxedit->get_editctrl();
	xui_control* subyedit = m_subyedit->get_editctrl();
	xui_method_ptrcall(m_subxenum,	on_perform_x)(subxedit->get_renderx());
	xui_method_ptrcall(m_subxenum,	on_perform_y)(subxedit->get_rendery()+xui_propview::default_lineheight);
	xui_method_ptrcall(m_subxenum,	on_perform_w)(subxedit->get_renderw());
	xui_method_ptrcall(m_subyenum,	on_perform_x)(subyedit->get_renderx());
	xui_method_ptrcall(m_subyenum,	on_perform_y)(subyedit->get_rendery()+xui_propview::default_lineheight);
	xui_method_ptrcall(m_subyenum,	on_perform_w)(subyedit->get_renderw());
}

/*
//event
*/
xui_method_explain(onity_propctrl_pivot, on_editctrltextchanged,	void			)( xui_component* sender, xui_method_args& args )
{
	onity_propdata_pivot* datapivot = dynamic_cast<onity_propdata_pivot*>(m_propdata);
	xui_rect2d<f64> box = datapivot->get_boundbox();

	xui_textbox* textctrl = xui_dynamic_cast(xui_textbox, sender);
	if (textctrl == m_subxedit->get_editctrl())
	{
		if (textctrl->get_text().empty())
		{
			m_subxenum->ini_dropbox(-1);
		}
		else
		{
			f64 x = m_subxedit->get_value();
			f64 w = (f64)(s32)(box.get_w()/2);
			if		(x == box.ax  )		m_subxenum->ini_dropbox(1);
			else if (x == box.bx  )		m_subxenum->ini_dropbox(2);
			else if (x == box.ax+w)		m_subxenum->ini_dropbox(3);
			else						m_subxenum->ini_dropbox(0);
		}
	}

	if (sender == m_subyedit->get_editctrl())
	{
		if (textctrl->get_text().empty())
		{
			m_subyenum->ini_dropbox(-1);
		}
		else
		{
			f64 y = m_subyedit->get_value();
			f64 h = (f64)(s32)(box.get_h()/2);
			if		(y == box.ay  )		m_subyenum->ini_dropbox(1);
			else if (y == box.by  )		m_subyenum->ini_dropbox(2);
			else if (y == box.ay+h)		m_subyenum->ini_dropbox(3);
			else						m_subyenum->ini_dropbox(0);
		}
	}
}
xui_method_explain(onity_propctrl_pivot, on_enumctrlnonfocus,		void			)( xui_component* sender, xui_method_args& args )
{
	if (sender == m_subxenum || m_subxenum->was_ancestor(sender))
	{
		m_subxenum->set_sidecolor(xui_control::default_sidecolor);
	}
	else
	if (sender == m_subyenum || m_subyenum->was_ancestor(sender))
	{
		m_subyenum->set_sidecolor(xui_control::default_sidecolor);
	}
}
xui_method_explain(onity_propctrl_pivot, on_enumctrlgetfocus,		void			)( xui_component* sender, xui_method_args& args )
{
	if (sender == m_subxenum || m_subxenum->was_ancestor(sender))
	{
		m_subxenum->set_sidecolor(xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f));
	}
	else
	if (sender == m_subyenum || m_subyenum->was_ancestor(sender))
	{
		m_subyenum->set_sidecolor(xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f));
	}
}
xui_method_explain(onity_propctrl_pivot, on_enumctrlselection,		void			)( xui_component* sender, xui_method_args& args )
{
	onity_propdata_pivot* datapivot = dynamic_cast<onity_propdata_pivot*>(m_propdata);
	xui_rect2d<f64> box = datapivot->get_boundbox();

	if (sender == m_subxenum)
	{
		u32 index = m_subxenum->get_selectedindex();
		if (index != -1 && index > 0)
		{
			f64 x = 0.0;
			f64 w = (f64)(s32)(box.get_w()/2);
			switch (index)
			{
			case ALIGNHORZ_L: x = box.ax  ;	break;
			case ALIGNHORZ_C: x = box.ax+w; break;
			case ALIGNHORZ_R: x = box.bx  ;	break;
			}

			m_subxedit->set_value(x);
			on_editvalue(m_subxedit);
		}
	}
	else
	if (sender == m_subyenum)
	{
		u32 index = m_subyenum->get_selectedindex();
		if (index != -1 && index > 0)
		{
			f64 y = 0.0;
			f64 h = (f64)(s32)(box.get_h()/2);
			switch (index)
			{
			case ALIGNVERT_T: y = box.ay  ;	break;
			case ALIGNVERT_C: y = box.ay+h; break;
			case ALIGNVERT_B: y = box.by  ;	break;
			}

			m_subyedit->set_value(y);
			on_editvalue(m_subyedit);
		}
	}
}