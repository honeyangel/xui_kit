#include "xui_convas.h"
#include "xui_scroll.h"
#include "xui_drawer.h"
#include "xui_desktop.h"
#include "xui_propplus.h"
#include "xui_kindctrl.h"
#include "xui_propview.h"
#include "xui_propctrl_stdvec.h"

xui_implement_rtti(xui_propctrl_stdvec, xui_propctrl);

/*
//create
*/
xui_method_explain(xui_propctrl_stdvec, create,				xui_propctrl*	)( xui_propdata* propdata )
{
	return new xui_propctrl_stdvec(propdata);
}

/*
//constructor
*/
xui_create_explain(xui_propctrl_stdvec)( xui_propdata* propdata )
: xui_propctrl()
{
	m_border   = xui_rect2d<s32>(0, 6, 0, 6);
	m_dragelem = -1;
	m_dropelem = -1;

	//plus
	m_propplus = new xui_propplus(this);
	xui_method_ptrcall(m_propplus,	ini_component	)(true, true);
	m_widgetvec.push_back(m_propplus);

	//name
	m_namectrl = new xui_drawer(xui_vector<s32>(128, 20), this);
	xui_method_ptrcall(m_namectrl,	set_sidecolor	)(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
	xui_method_ptrcall(m_namectrl,	set_textfont	)(xui_family("Arial", 16, false));
	xui_method_ptrcall(m_namectrl,	set_textcolor	)(xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
	xui_method_ptrcall(m_namectrl,	set_textalign	)(TA_LC);
	m_widgetvec.push_back(m_namectrl);

	//size
	m_propedit = new xui_propedit_stdvec(this);
	xui_drawer*  namectrl = m_propedit->get_namectrl();
	xui_control* editctrl = m_propedit->get_editctrl();
	xui_method_ptrcall(namectrl,	set_text		)(L"Size");
	xui_method_ptrcall(namectrl,	set_parent		)(this);
	xui_method_ptrcall(editctrl,	set_parent		)(this);
	m_widgetvec.push_back(namectrl);
	m_widgetvec.push_back(editctrl);
}

/*
//destructor
*/
xui_delete_explain(xui_propctrl_stdvec)( void )
{
	delete m_propedit;
	for (u32 i = 0; i < m_ctrlpool.size(); ++i)
		delete m_ctrlpool[i];
}

/*
//method
*/
xui_method_explain(xui_propctrl_stdvec, was_expanded,			bool			)( void ) const
{
	return m_propplus->was_expanded();
}
xui_method_explain(xui_propctrl_stdvec, set_expanded,			void			)( bool flag )
{
	m_propplus->set_expanded(flag);
}

/*
//propdata
*/
xui_method_explain(xui_propctrl_stdvec, on_linkpropdata,		void			)( void )
{
	m_namectrl->set_text(m_propdata->get_name());
	m_propedit->reset();

	bool same = true;
	xui_propdata_stdvec* datastdvec = dynamic_cast<xui_propdata_stdvec*>(m_propdata);
	u32 value = datastdvec->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_stdvec* data = dynamic_cast<xui_propdata_stdvec*>(m_propdatavec[i]);
		if (data->get_value() != value)
		{
			same = false;
			break;
		}
	}

	if (same)
	{
		m_propedit->set_value(value);
	}

	//propctrl
	if (same == false)
		value = 0;
	if (value < m_propctrlvec.size())
	{
		u32 count = m_propctrlvec.size()-value;
		for (u32 i = 0; i < count; ++i)
			del_propctrl();
	}
	else
	if (value > m_propctrlvec.size())
	{
		u32 count = value - m_propctrlvec.size();
		for (u32 i = 0; i < count; ++i)
		{
			xui_propdata_stdvec* datastdvec = dynamic_cast<xui_propdata_stdvec*>(m_propdata);
			const xui_propdata_vec& vec = datastdvec->get_propvec();
			u32 index = value-count+i;
			add_propctrl(vec[index]);
		}
	}

	//propdata
	for (u32 i = 0; i < m_propctrlvec.size(); ++i)
	{
		xui_propdata_vec propdataall = get_propdataall(i);
		xui_propctrl* propctrl = m_propctrlvec[i];
		propctrl->set_propdata(propdataall);

		for (xui_propdata_vec::iterator itor = propdataall.begin(); itor != propdataall.end(); ++itor)
		{
			(*itor)->set_ctrl(propctrl);
		}
	}
}
xui_method_explain(xui_propctrl_stdvec, on_editvalue,			void			)( xui_propedit* sender )
{
	u32 value = m_propedit->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_stdvec* data = dynamic_cast<xui_propdata_stdvec*>(m_propdatavec[i]);
		data->set_value(value);
	}
}

/*
//override
*/
xui_method_explain(xui_propctrl_stdvec, on_invalid,				void			)( xui_method_args& args )
{
	xui_propctrl::on_invalid(args);

	xui_vector<s32> sz;
	sz.w = get_renderw();
	sz.h = xui_propview::LINE_HEIGHT + m_border.ay + m_border.by;

	bool expand = m_propplus->was_expanded();
	xui_drawer*  namectrl = m_propedit->get_namectrl();
	xui_control* editctrl = m_propedit->get_editctrl();
	namectrl->set_visible(expand);
	editctrl->set_visible(expand);
	for (u32 i = 0; i < m_propctrlvec.size(); ++i)
	{
		xui_propctrl* propctrl = m_propctrlvec[i];
		propctrl->set_visible(expand);
	}

	if (m_propdata && expand)
	{
		sz.h += xui_propview::LINE_HEIGHT;
		for (u32 i = 0; i < m_propctrlvec.size(); ++i)
		{
			xui_propctrl* propctrl = m_propctrlvec[i];
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
xui_method_explain(xui_propctrl_stdvec, on_perform,				void			)( xui_method_args& args )
{
	xui_control::on_perform(args);
	s32 height = xui_propview::LINE_HEIGHT;
	s32 indent = get_indent();
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt;
	xui_vector<s32> sz;
	//name
	m_namectrl->on_perform_y (rt.ay);
	m_namectrl->on_perform_w (rt.get_w());
	m_namectrl->set_textoffset(xui_vector<s32>(indent, 0));
	//plus
	pt.x = (indent-16);
	pt.y = rt.ay + (height-12)/2;
	m_propplus->on_perform_pt(pt);
	m_propplus->on_perform_sz(12, 12);
	if (m_propplus->was_expanded())
	{
		xui_drawer*  namectrl = m_propedit->get_namectrl();
		xui_control* editctrl = m_propedit->get_editctrl();
		//size
		namectrl->on_perform_y (rt.ay + height);
		namectrl->on_perform_w (rt.get_w()/2);
		namectrl->set_textoffset(xui_vector<s32>(indent+xui_propview::NODE_INDENT, 0));
		pt.x = rt.get_w()/2;
		pt.y = rt.ay + height + height/2 - editctrl->get_renderh()/2;
		editctrl->on_perform_pt(pt);
		editctrl->on_perform_w (rt.get_w()/2);
		//prop
		pt.x = 0;
		pt.y = rt.ay + height*2;
		for (u32 i = 0; i < m_propctrlvec.size(); ++i)
		{
			xui_propctrl* propctrl = m_propctrlvec[i];
			propctrl->on_perform_pt(pt);
			propctrl->on_perform_w (rt.get_w());
			pt.y += propctrl->get_renderh();
		}
	}
}

/*
//method
*/
xui_method_explain(xui_propctrl_stdvec, add_propctrl,			void			)( xui_propdata* propdata )
{
	xui_propctrl* propctrl = NULL;
	if (m_ctrlpool.size() > 0)
	{
		propctrl = m_ctrlpool.back();
		u32 index = m_ctrlpool.size()-1;
		m_ctrlpool.erase(m_ctrlpool.begin()+index);
	}
	else
	{
		xui_prop_newctrl func	 = propdata->get_func();
		propctrl = (*func)(propdata);
		xui_drawer* sortctrl	 = new xui_drawer(xui_vector<s32>(16), propctrl);
		sortctrl->xm_updateself += new xui_method_member<xui_method_args,  xui_propctrl_stdvec>(this, &xui_propctrl_stdvec::on_sortctrlupdateself);
		sortctrl->xm_renderself += new xui_method_member<xui_method_args,  xui_propctrl_stdvec>(this, &xui_propctrl_stdvec::on_sortctrlrenderself);
		sortctrl->xm_topdraw	+= new xui_method_member<xui_method_args,  xui_propctrl_stdvec>(this, &xui_propctrl_stdvec::on_sortctrltopdraw);
		sortctrl->xm_mousedown	+= new xui_method_member<xui_method_mouse, xui_propctrl_stdvec>(this, &xui_propctrl_stdvec::on_sortctrlmousedown);
		sortctrl->xm_mousemove	+= new xui_method_member<xui_method_mouse, xui_propctrl_stdvec>(this, &xui_propctrl_stdvec::on_sortctrlmousemove);
		sortctrl->xm_mouserise	+= new xui_method_member<xui_method_mouse, xui_propctrl_stdvec>(this, &xui_propctrl_stdvec::on_sortctrlmouserise);
		xui_method_ptrcall(propctrl, add_ctrlelse	)(sortctrl);
		xui_method_ptrcall(propctrl, refresh		)();
		
		xui_vector<s32> pt;
		pt.x = get_indent();
		pt.y = propctrl->get_borderrt().ay + xui_propview::LINE_HEIGHT/2 - sortctrl->get_renderh()/2;
		sortctrl->on_perform_pt(pt);
	}

	propctrl->set_parent(this);
	m_widgetvec.push_back(propctrl);
	m_propctrlvec.push_back(propctrl);
}
xui_method_explain(xui_propctrl_stdvec, del_propctrl,			void			)( void )
{
	xui_propctrl* propctrl = m_propctrlvec.back();
	propctrl->set_parent(NULL);
	std::vector<xui_component*>::iterator itor = std::find(m_widgetvec.begin(), m_widgetvec.end(), propctrl);
	if (itor != m_widgetvec.end())
		m_widgetvec.erase(itor);

	u32 index = m_propctrlvec.size()-1;
	m_propctrlvec.erase(m_propctrlvec.begin()+index);

	m_ctrlpool.push_back(propctrl);
}
xui_method_explain(xui_propctrl_stdvec, get_propdataall,		xui_propdata_vec)( u32 index )
{
	xui_propdata_vec result;
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_stdvec* datastdvec = dynamic_cast<xui_propdata_stdvec*>(m_propdatavec[i]);
		const xui_propdata_vec& vec = datastdvec->get_propvec();
		result.push_back(vec[index]);
	}

	return result;
}

/*
//event
*/
xui_method_explain(xui_propctrl_stdvec, on_sortctrlupdateself,	void			)( xui_component* sender, xui_method_args&  args )
{
	xui_component* catchctrl = g_desktop->get_catchctrl();
	if (catchctrl == sender)
	{
		xui_propview* propview = get_propview();
		xui_rect2d<s32> rt = propview->get_renderrtins() + propview->get_screenpt();
		xui_vector<s32> pt = g_desktop->get_mousecurr();

		s32 scroll_value =  0;
		if (pt.y < rt.ay && pt.y < rt.ay+xui_propview::LINE_HEIGHT/2)
			scroll_value = -5;
		if (pt.y < rt.by && pt.y > rt.by-xui_propview::LINE_HEIGHT/2)
			scroll_value =  5;

		xui_scroll* vscroll = propview->get_vscroll();
		if (scroll_value != 0 && vscroll)
		{
			vscroll->set_value(vscroll->get_value()+scroll_value);

			xui_method_mouse args;
			args.mouse = MB_L;
			args.point = pt;
			g_desktop->os_mousemove(args);
		}
	}
}
xui_method_explain(xui_propctrl_stdvec, on_sortctrlrenderself,	void			)( xui_component* sender, xui_method_args&  args )
{
	xui_rect2d<s32> rt = sender->get_renderrtabs();
	rt.oft_x(rt.get_w()/4);
	rt.oft_y(rt.get_h()/2);
	rt.set_w(rt.get_w()/2);
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax, rt.ay), xui_vector<s32>(rt.bx, rt.ay), xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
	rt.oft_y(3);
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax, rt.ay), xui_vector<s32>(rt.bx, rt.ay), xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
	rt.oft_y(3);
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax, rt.ay), xui_vector<s32>(rt.bx, rt.ay), xui_colour(1.0f, 0.7f, 0.7f, 0.7f));
}
xui_method_explain(xui_propctrl_stdvec, on_sortctrltopdraw,		void			)( xui_component* sender, xui_method_args&  args )
{
	xui_propctrl::on_topdraw(args);
	if (m_dropelem != -1 && m_dropelem != m_dragelem)
	{
		if (m_dropelem < m_dragelem || m_dropelem > m_dragelem+1)
		{
			xui_vector<s32> pt = get_screenpt();
			s32 y = pt.y + m_border.ay + 2*xui_propview::LINE_HEIGHT;
			for (u32 i = 0; i < m_propctrlvec.size(); ++i)
			{
				if (i == m_dropelem)
					break;

				xui_propctrl* propctrl = m_propctrlvec[i];
				y += propctrl->get_renderh();
			}

			xui_vector<s32> p1(pt.x,			   y);
			xui_vector<s32> p2(pt.x+get_renderw(), y);
			xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(p1.x, p1.y, p2.x, p1.y+3), xui_colour(1.0f, 0.0f, 0.0f, 0.0f));
			xui_vector<s32> path[3];
			path[0] = xui_vector<s32>(p1.x,   p1.y-4);
			path[1] = xui_vector<s32>(p1.x+5, p1.y  );
			path[2] = xui_vector<s32>(p1.x,   p1.y+6);
			xui_convas::get_ins()->fill_poly(path, 3, xui_colour(1.0f, 0.0f, 0.0f, 0.0f));
			path[0] = xui_vector<s32>(p2.x,   p1.y-4);
			path[1] = xui_vector<s32>(p2.x-5, p1.y  );
			path[2] = xui_vector<s32>(p2.x,   p1.y+6);
			xui_convas::get_ins()->fill_poly(path, 3, xui_colour(1.0f, 0.0f, 0.0f, 0.0f));
		}
	}
}
xui_method_explain(xui_propctrl_stdvec, on_sortctrlmousedown,	void			)( xui_component* sender, xui_method_mouse& args )
{
	for (u32 i = 0; i < m_propctrlvec.size(); ++i)
	{
		if (sender->get_parent() == m_propctrlvec[i])
		{
			m_dragelem = i;
			break;
		}
	}
}
xui_method_explain(xui_propctrl_stdvec, on_sortctrlmousemove,	void			)( xui_component* sender, xui_method_mouse& args )
{
	m_dropelem = -1;

	xui_vector<s32> relative = get_renderpt(args.point);
	for (u32 i = 0; i < m_propctrlvec.size(); ++i)
	{
		xui_propctrl* propctrl = m_propctrlvec[i];
		if (propctrl->choose(relative))
		{
			m_dropelem = i;
			if (args.point.y > propctrl->get_screenpt().y + propctrl->get_renderh()/2)
				++m_dropelem;

			break;
		}
	}
}
xui_method_explain(xui_propctrl_stdvec, on_sortctrlmouserise,	void			)( xui_component* sender, xui_method_mouse& args )
{
	if (m_dropelem != -1 && m_dropelem != m_dragelem)
	{
		if (m_dropelem < m_dragelem || m_dropelem > m_dragelem+1)
		{
			for (u32 i = 0; i < m_propdatavec.size(); ++i)
			{
				xui_propdata_stdvec* data = dynamic_cast<xui_propdata_stdvec*>(m_propdatavec[i]);
				data->set_index(m_dragelem, m_dropelem);
			}
		}
	}	

	m_dragelem = -1;
	m_dropelem = -1;
}