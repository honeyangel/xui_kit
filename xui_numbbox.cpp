#include "xui_convas.h"
#include "xui_global.h"
#include "xui_button.h"
#include "xui_numbbox.h"

xui_implement_rtti(xui_numbbox, xui_textbox);

/*
//constructor
*/
xui_create_explain(xui_numbbox)(const xui_vector<s32>& size, u08 numbtype, f64 interval, bool showarrow ) 
: xui_textbox(size)
{
	m_numbtype = numbtype;
	m_interval = interval;
	m_incarrow = NULL;
	m_decarrow = NULL;
	m_holdtime = 0.0f;

	if (showarrow)
	{
		m_incarrow = new xui_button(xui_vector<s32>(16, 8));
		m_incarrow->set_parent(this);
		m_incarrow->xm_buttonclick	+= new xui_method_member<xui_method_args,	xui_numbbox>(this, &xui_numbbox::on_arrowclick);
		m_incarrow->xm_renderself	+= new xui_method_member<xui_method_args,	xui_numbbox>(this, &xui_numbbox::on_arrowrenderself);
		m_incarrow->xm_updateself	+= new xui_method_member<xui_method_update, xui_numbbox>(this, &xui_numbbox::on_arrowupdateself);
		m_decarrow = new xui_button(xui_vector<s32>(16, 8));
		m_decarrow->set_parent(this);
		m_decarrow->xm_buttonclick	+= new xui_method_member<xui_method_args,	xui_numbbox>(this, &xui_numbbox::on_arrowclick);
		m_decarrow->xm_renderself	+= new xui_method_member<xui_method_args,	xui_numbbox>(this, &xui_numbbox::on_arrowrenderself);
		m_decarrow->xm_updateself	+= new xui_method_member<xui_method_update, xui_numbbox>(this, &xui_numbbox::on_arrowupdateself);

		m_widgetvec.push_back(m_incarrow);
		m_widgetvec.push_back(m_decarrow);
	}
}

/*
//override
*/
xui_method_explain(xui_numbbox, get_renderrtins,	xui_rect2d<s32>	)( void ) const
{
	xui_rect2d<s32> rt = xui_textbox::get_renderrtins();
	if (m_incarrow)
		rt.bx -= m_incarrow->get_renderw();

	return rt;
}

/*
//callback
*/
xui_method_explain(xui_numbbox, on_perform,			void			)( xui_method_args&  args )
{
	xui_textbox::on_perform(args);
	if (m_incarrow && m_decarrow)
	{
		xui_rect2d<s32> rt = xui_textbox::get_renderrtins();
		xui_vector<s32> pt(rt.bx-m_incarrow->get_renderw(), rt.ay+rt.get_h()/2);
		m_incarrow->on_perform_pt(pt.x, pt.y-m_incarrow->get_renderh());
		m_decarrow->on_perform_pt(pt.x, pt.y);
	}
}
xui_method_explain(xui_numbbox, on_mousewheel,		void			)( xui_method_mouse& args )
{
	xui_textbox::on_mousewheel(args);

	f64 value = _wtof(m_text.c_str()) + (args.wheel > 0 ? m_interval : -m_interval);
	if (value < 0.0 && m_numbtype == NT_UNSIGNEDINT)
		value = 0.0;

	char temp[32];
	if (m_numbtype == NT_FLOAT) sprintf(temp, "%.2f", (f32)value);
	else						sprintf(temp, "%d",   (s32)value);
	set_text(xui_global::ascii_to_unicode(temp));

	xui_method_args    enter_args;
	xm_textenter(this, enter_args);

	args.handle = true;
}

/*
//event
*/
xui_method_explain(xui_numbbox, on_arrowupdateself, void			)( xui_component* sender, xui_method_update& args )
{
	if (m_incarrow->has_catch() ||
		m_decarrow->has_catch())
	{
		m_holdtime += args.delta;
		if (m_holdtime > 0.5f && sender->has_catch())
		{
			xui_button* button = xui_dynamic_cast(xui_button, sender);
			xui_method_args	other_args;
			button->xm_buttonclick(button, other_args);
		}
	}
	else
	{
		m_holdtime  = 0.0f;
	}
}
xui_method_explain(xui_numbbox, on_arrowrenderself, void			)( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt    = sender->get_renderrtabs();
	xui_colour      color = sender->get_rendercolor();
	xui_vector<s32> center(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);
	if (sender == m_incarrow)
		xui_convas::get_ins()->fill_triangle(center, 3, TRIANGLE_UP,   color);
	if (sender == m_decarrow)
		xui_convas::get_ins()->fill_triangle(center, 3, TRIANGLE_DOWN, color);
}
xui_method_explain(xui_numbbox, on_arrowclick,		void			)( xui_component* sender, xui_method_args& args )
{
	f64 value = _wtof(m_text.c_str());
	if (sender == m_incarrow)
		value  += m_interval;
	if (sender == m_decarrow)
		value  -= m_interval;

	if (value  < 0.0 && m_numbtype == NT_UNSIGNEDINT)
		value  = 0.0;

	char temp[32];
	if (m_numbtype == NT_FLOAT) sprintf(temp, "%.2f", (f32)value);
	else						sprintf(temp, "%d",   (s32)value);
	set_text(xui_global::ascii_to_unicode(temp));

	xui_method_args    enter_args;
	xm_textenter(this, enter_args);
}