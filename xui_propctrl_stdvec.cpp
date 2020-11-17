#include "xui_canvas.h"
#include "xui_scroll.h"
#include "xui_drawer.h"
#include "xui_listview.h"
#include "xui_listitem.h"
#include "xui_desktop.h"
#include "xui_kindctrl.h"
#include "xui_propview.h"
#include "xui_propctrl_stdvec.h"

//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(xui_propctrl_stdvec, xui_propctrl)

xui_propctrl* xui_propctrl_stdvec::create( xui_propdata* propdata )
{
	return new xui_propctrl_stdvec(propdata);
}

xui_propctrl_stdvec::xui_propctrl_stdvec( xui_propdata* propdata )
: xui_propctrl()
{
	m_dragelem = -1;
	m_dropelem = -1;

	//name
	m_namectrl = new xui_drawer(xui_vector<s32>(128, 20));
	m_namectrl->set_parent(this);
	m_namectrl->set_textalign(k_textalign_lc);
	m_widgetvec.push_back(m_namectrl);

	//plus
	m_propplus = new xui_plusctrl(k_plusrender_normal, true);
	m_propplus->xm_expand += new xui_method_member<xui_method_args, xui_propctrl_stdvec>(this, &xui_propctrl_stdvec::on_propexpand);
	m_propplus->set_parent(this);
	m_propplus->ini_component(true, true);
	m_widgetvec.push_back(m_propplus);

	//size
	m_propedit = new xui_propedit_stdvec(this);
	xui_drawer*  namectrl = m_propedit->get_namectrl();
	xui_control* editctrl = m_propedit->get_editctrl();
	namectrl->set_text(L"Size");
	namectrl->set_parent(this);
	editctrl->set_parent(this);
	m_widgetvec.push_back(namectrl);
	m_widgetvec.push_back(editctrl);
}

xui_propctrl_stdvec::~xui_propctrl_stdvec( void )
{
	delete m_propedit;
	for (u32 i = 0; i < m_ctrlpool.size(); ++i)
		delete m_ctrlpool[i];
}

bool xui_propctrl_stdvec::was_expanded( void ) const
{
	return m_propplus->was_expanded();
}

void xui_propctrl_stdvec::set_expanded( bool flag )
{
	m_propplus->set_expanded(flag);
}

void xui_propctrl_stdvec::on_linkpropdata( bool selfupdate )
{
	if (selfupdate == false)
	{
		m_namectrl->set_text(m_propdata->get_name());
		m_propedit->reset();
	}

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
	u32 count = 0;
	if (same == false)
		value = 0;
	if (value < m_propctrlvec.size())
	{
		count = m_propctrlvec.size()-value;
		for (u32 i = 0; i < count; ++i)
			del_propctrl();
	}
	else
	if (value > m_propctrlvec.size())
	{
		count = value - m_propctrlvec.size();
		for (u32 i = 0; i < count; ++i)
		{
			xui_propdata_stdvec* datastdvec = dynamic_cast<xui_propdata_stdvec*>(m_propdata);
			const xui_propdata_vec& vec = datastdvec->get_propvec();
			u32 index = value-count+i;
			add_propctrl(vec[index]);
		}
	}

	//propdata
	if (selfupdate == false || count > 0)
	{
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
}

void xui_propctrl_stdvec::on_editvalue( xui_propedit* sender )
{
	u32 value = m_propedit->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		xui_propdata_stdvec* data = dynamic_cast<xui_propdata_stdvec*>(m_propdatavec[i]);
		data->set_value(value);
	}
}

void xui_propctrl_stdvec::on_invalid( xui_method_args& args )
{
	xui_vector<s32> sz;
	sz.w = get_renderw();
	sz.h = xui_propview::k_default_lineheight + m_border.ay + m_border.by;

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
		sz.h += xui_propview::k_default_lineheight;
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

void xui_propctrl_stdvec::on_perform( xui_method_args& args )
{
	xui_control::on_perform(args);
	s32 height = xui_propview::k_default_lineheight;
	s32 indent = get_indent();
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt;
	//name
	m_namectrl->on_perform_y (rt.ay);
	m_namectrl->on_perform_w (rt.get_w());
	m_namectrl->set_textoffset(xui_vector<s32>(indent, 0));
	//plus
	pt.x = (indent-m_propplus->get_renderw());
	pt.y = rt.ay + (height-m_propplus->get_renderh())/2;
	m_propplus->on_perform_pt(pt);
	if (m_propplus->was_expanded())
	{
		xui_drawer*  namectrl = m_propedit->get_namectrl();
		xui_control* editctrl = m_propedit->get_editctrl();
		//size
		namectrl->on_perform_y (rt.ay + height);
		namectrl->on_perform_w (rt.get_w()/2);
		namectrl->set_textoffset(xui_vector<s32>(indent+xui_propview::k_default_nodeindent, 0));
		pt.x = rt.get_w()/2;
		pt.y = rt.ay + height + height/2 - editctrl->get_renderh()/2;
		editctrl->on_perform_pt(pt);
		editctrl->on_perform_w (rt.get_w() - pt.x);
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

void xui_propctrl_stdvec::add_propctrl( xui_propdata* propdata )
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
		xui_drawer* sortctrl	 = new xui_drawer(xui_vector<s32>(20));
		sortctrl->xm_updateself += new xui_method_member<xui_method_update, xui_propctrl_stdvec>(this, &xui_propctrl_stdvec::on_sortctrlupdateself);
		sortctrl->xm_renderself += new xui_method_member<xui_method_args,	xui_propctrl_stdvec>(this, &xui_propctrl_stdvec::on_sortctrlrenderself);
		sortctrl->xm_topdraw	+= new xui_method_member<xui_method_args,	xui_propctrl_stdvec>(this, &xui_propctrl_stdvec::on_sortctrltopdraw);
		sortctrl->xm_mousedown	+= new xui_method_member<xui_method_mouse,	xui_propctrl_stdvec>(this, &xui_propctrl_stdvec::on_sortctrlmousedown);
		sortctrl->xm_mousemove	+= new xui_method_member<xui_method_mouse,	xui_propctrl_stdvec>(this, &xui_propctrl_stdvec::on_sortctrlmousemove);
		sortctrl->xm_mouserise	+= new xui_method_member<xui_method_mouse,	xui_propctrl_stdvec>(this, &xui_propctrl_stdvec::on_sortctrlmouserise);
		propctrl->xm_perform	+= new xui_method_member<xui_method_args,	xui_propctrl_stdvec>(this, &xui_propctrl_stdvec::on_propctrlperform);
		propctrl->add_ctrlelse(sortctrl);
		propctrl->refresh();
	}

	propctrl->set_parent(this);
	m_widgetvec.push_back(propctrl);
	m_propctrlvec.push_back(propctrl);

	invalid();
}

void xui_propctrl_stdvec::del_propctrl( void )
{
	xui_propctrl* propctrl = m_propctrlvec.back();
	propctrl->set_parent(NULL);
	std::vector<xui_component*>::iterator itor = std::find(m_widgetvec.begin(), m_widgetvec.end(), propctrl);
	if (itor != m_widgetvec.end())
		m_widgetvec.erase(itor);

	u32 index = m_propctrlvec.size()-1;
	m_propctrlvec.erase(m_propctrlvec.begin()+index);

	m_ctrlpool.push_back(propctrl);

	invalid();
}

xui_propdata_vec xui_propctrl_stdvec::get_propdataall( u32 index )
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

void xui_propctrl_stdvec::on_propctrlperform( xui_component* sender, xui_method_args& args )
{
	xui_propctrl*  propctrl = xui_dynamic_cast(xui_propctrl, sender);
	xui_component* sortctrl = propctrl->get_ctrlelse();
	xui_vector<s32> pt;
	xui_rect2d<s32> rt = propctrl->get_renderrt();
	s32 indent = propctrl->get_indent();
	pt.x = indent - sortctrl->get_renderw();
	pt.y = xui_propview::k_default_lineheight / 2 - sortctrl->get_renderh() / 2;
	sortctrl->on_perform_pt(pt);
}

void xui_propctrl_stdvec::on_sortctrlupdateself( xui_component* sender, xui_method_update& args )
{
	if (sender->has_catch())
	{
		xui_propview* propview = get_propview();
		xui_rect2d<s32> rt = propview->get_renderrtins() + propview->get_screenpt();
		xui_vector<s32> pt = xui_desktop::get_ins()->get_mousecurr();
		xui_vector<s32> dw = xui_desktop::get_ins()->get_mousedown();
		if (xui_abs(pt.y-dw.y) < xui_propview::k_default_lineheight/2)
			return;

		s32 scroll_value =  0;
		if (pt.y > rt.ay && pt.y < rt.ay+xui_propview::k_default_lineheight/2)
			scroll_value = -xui_propview::k_default_lineheight/2;
		if (pt.y < rt.by && pt.y > rt.by-xui_propview::k_default_lineheight/2)
			scroll_value =  xui_propview::k_default_lineheight/2;

		xui_scroll* vscroll = propview->get_vscroll();
		if (scroll_value != 0 && vscroll)
		{
			vscroll->set_value(vscroll->get_value()+scroll_value);

			xui_method_mouse args;
			args.mouse = k_mb_left;
			args.point = pt;
			xui_desktop::get_ins()->os_mousemove(args);
		}
	}
}

void xui_propctrl_stdvec::on_sortctrlrenderself( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt     = sender->get_renderrtabs();
	xui_colour	    color  = sender->was_hover() ? xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f) : xui_button::k_default_backcolor;
	xui_vector<s32> center = xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);
	rt.ax = center.x - 6;
	rt.ay = center.y - 4;
	rt.bx = center.x + 6;
	rt.by = center.y - 2;
	xui_canvas::get_ins()->fill_rectangle(rt, color);
	rt.oft_y(3);
	xui_canvas::get_ins()->fill_rectangle(rt, color);
	rt.oft_y(3);
	xui_canvas::get_ins()->fill_rectangle(rt, color);
}

void xui_propctrl_stdvec::on_sortctrltopdraw( xui_component* sender, xui_method_args& args )
{
	if (args.wparam != get_window())
		return;

	if (m_dropelem != -1 && m_dropelem != m_dragelem)
	{
		if (m_dropelem < m_dragelem || m_dropelem > m_dragelem+1)
		{
			xui_vector<s32> pt = get_screenpt();
			s32 y = pt.y + m_border.ay + 2*xui_propview::k_default_lineheight;
			for (u32 i = 0; i < m_propctrlvec.size(); ++i)
			{
				if (i == m_dropelem)
					break;

				xui_propctrl* propctrl = m_propctrlvec[i];
				y += propctrl->get_renderh();
			}

			xui_vector<s32> p1(pt.x,			   y);
			xui_vector<s32> p2(pt.x+get_renderw(), y);
			xui_rect2d<s32> rt  = xui_rect2d<s32>(p1.x+1, p1.y-1, p2.x-1, p1.y+1);
			xui_canvas::get_ins()->fill_rectangle(rt, xui_colour::k_black);
			p1 += xui_vector<s32>(4, 0);
			xui_canvas::get_ins()->fill_triangle(p1, 3, k_triangle_right, xui_colour::k_black);
			p2 -= xui_vector<s32>(3, 0);
			xui_canvas::get_ins()->fill_triangle(p2, 3, k_triangle_left,  xui_colour::k_black);
		}
	}
}

void xui_propctrl_stdvec::on_sortctrlmousedown( xui_component* sender, xui_method_mouse& args )
{
	if (args.mouse == k_mb_left)
	{
		for (u32 i = 0; i < m_propctrlvec.size(); ++i)
		{
			if (sender->get_parent() == m_propctrlvec[i])
			{
				m_dragelem =  i;
				m_dropelem = -1;
				break;
			}
		}
	}
}

void xui_propctrl_stdvec::on_sortctrlmousemove( xui_component* sender, xui_method_mouse& args )
{
	if (sender->has_catch())
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
}

void xui_propctrl_stdvec::on_sortctrlmouserise( xui_component* sender, xui_method_mouse& args )
{
	if (args.mouse == k_mb_left)
	{
		if (m_dropelem != -1 && m_dropelem != m_dragelem)
		{
			if (m_dropelem < m_dragelem || m_dropelem > m_dragelem+1)
			{
				on_readyundo();

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
}

void xui_propctrl_stdvec::on_propexpand( xui_component* sender, xui_method_args& args )
{
	invalid();
}

//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(xui_propctrl_stdvec_root, xui_propctrl)

xui_propctrl* xui_propctrl_stdvec_root::create( xui_propdata* propdata )
{
	return new xui_propctrl_stdvec_root(propdata);
}

xui_propctrl_stdvec_root::xui_propctrl_stdvec_root( xui_propdata* propdata )
: xui_propctrl()
{
	m_border = xui_rect2d<s32>(0, 8, 0, 8);
	m_header = new xui_drawer(xui_vector<s32>(128, 20));
	m_header->set_parent(this);
	m_header->set_textalign(k_textalign_lc);
	m_header->set_borderrt(xui_rect2d<s32>(8, 2, 2, 2));
	m_header->ini_drawer(propdata->get_name());
	m_nontip = new xui_drawer(xui_vector<s32>(128, 20));
	m_nontip->set_parent(this);
	m_nontip->set_textalign(k_textalign_lc);
	m_nontip->set_borderrt(xui_rect2d<s32>(8, 2, 2, 2));
	m_nontip->ini_component(true, false);
	m_nontip->ini_drawer(L"Empty");

	m_insert = new xui_button(xui_vector<s32>(24, 16));
	m_insert->set_parent(this);
	m_insert->set_backcolor(xui_colour::k_white);
	m_insert->set_movecolor(xui_button::k_default_downcolor);
	m_insert->xm_buttonclick	 += new xui_method_member<xui_method_args, xui_propctrl_stdvec_root>(this, &xui_propctrl_stdvec_root::on_insertclick);
	m_insert->xm_renderself		 += new xui_method_member<xui_method_args, xui_propctrl_stdvec_root>(this, &xui_propctrl_stdvec_root::on_insertrenderself);

	m_delete = new xui_button(xui_vector<s32>(24, 16));
	m_delete->set_parent(this);
	m_delete->set_backcolor(xui_colour::k_white);
	m_delete->set_movecolor(xui_button::k_default_downcolor);
	m_delete->xm_buttonclick	 += new xui_method_member<xui_method_args, xui_propctrl_stdvec_root>(this, &xui_propctrl_stdvec_root::on_deleteclick);
	m_delete->xm_renderself		 += new xui_method_member<xui_method_args, xui_propctrl_stdvec_root>(this, &xui_propctrl_stdvec_root::on_deleterenderself);

	m_middle = xui_listview::create(xui_vector<s32>(0), false);
	m_middle->set_parent(this);
	m_middle->set_drawcolor(false);
	m_middle->set_sidestyle(k_sidestyle_n);
	m_middle->set_hscrollauto(false);
	m_middle->xm_invalid		 += new xui_method_member<xui_method_args, xui_propctrl_stdvec_root>(this, &xui_propctrl_stdvec_root::on_middleinvalid);
	m_middle->xm_selectedchange	 += new xui_method_member<xui_method_args, xui_propctrl_stdvec_root>(this, &xui_propctrl_stdvec_root::on_middleselection);

	m_widgetvec.push_back(m_header);
	m_widgetvec.push_back(m_insert);
	m_widgetvec.push_back(m_delete);
	m_widgetvec.push_back(m_middle);
	m_widgetvec.push_back(m_nontip);
}

void xui_propctrl_stdvec_root::on_linkpropdata( bool selfupdate )
{
	xui_propdata_stdvec_root*  datastdvec = dynamic_cast<xui_propdata_stdvec_root*>(m_propdata);
	xui_proproot_vec rootvec = datastdvec->get_rootvec();

	if (selfupdate == false || m_middle->get_itemcount() != rootvec.size())
	{
		m_middle->del_itemall();
		for (u32 i = 0; i < rootvec.size(); ++i)
		{
			xui_proproot*  root = rootvec[i];
			xui_propkind*  kind = root->get_propkind().front();
			xui_listitem * item = m_middle->add_item(kind->get_name());
			item->set_data(root);
		}

		m_middle->refresh();
		m_insert->set_enable(datastdvec->can_rootadd());
		m_delete->set_enable(datastdvec->can_rootdel() && rootvec.size() >  0);
		m_nontip->set_visible(rootvec.size() == 0);
	}
}

void xui_propctrl_stdvec_root::on_editvalue( xui_propedit* sender )
{}

void xui_propctrl_stdvec_root::on_renderback( xui_method_args& args )
{
	xui_propctrl::on_renderback(args);

	xui_rect2d<s32> rt;
	xui_colour		color = get_vertexcolor();
	rt = m_header->get_renderrtabs();
	xui_canvas::get_ins()->fill_round(rt, xui_colour(1.0f, 0.4f) * color, xui_rect2d<s32>(3, 3, 0, 0));
	rt = m_middle->get_renderrtabs();
	xui_canvas::get_ins()->fill_round(rt, xui_colour(1.0f, 0.3f) * color, xui_rect2d<s32>(0, 0, 0, 3));
	rt = m_insert->get_renderrtabs().get_union(m_delete->get_renderrtabs());
	xui_canvas::get_ins()->fill_round(rt, xui_colour(1.0f, 0.3f) * color, xui_rect2d<s32>(0, 0, 3, 3));

	color *= m_sidecolor;
	rt = m_header->get_renderrtabs();
	xui_canvas::get_ins()->draw_round(rt, color, xui_rect2d<s32>(3, 3, 0, 0));
	rt = m_middle->get_renderrtabs();
	xui_canvas::get_ins()->draw_line(xui_vector<s32>(rt.ax,		rt.by-3	), xui_vector<s32>(rt.ax,		rt.ay	), color);
	xui_canvas::get_ins()->draw_line(xui_vector<s32>(rt.bx-48,	rt.by	), xui_vector<s32>(rt.ax+3,		rt.by   ), color);
	xui_canvas::get_ins()->draw_line(xui_vector<s32>(rt.bx,		rt.ay	), xui_vector<s32>(rt.bx,		rt.by	), color);
	xui_canvas::get_ins()->draw_arc (xui_rect2d<s32>(
		rt.ax,			 
		rt.by-6,
		rt.ax+6, 
		rt.by), color, 90, 90, 1);

	rt = m_insert->get_renderrtabs().get_union(m_delete->get_renderrtabs());
	xui_canvas::get_ins()->draw_line(xui_vector<s32>(rt.bx,		rt.ay	), xui_vector<s32>(rt.bx,		rt.by-3	), color);
	xui_canvas::get_ins()->draw_line(xui_vector<s32>(rt.bx-3,	rt.by	), xui_vector<s32>(rt.ax+3,		rt.by	), color);
	xui_canvas::get_ins()->draw_line(xui_vector<s32>(rt.ax,		rt.by-3	), xui_vector<s32>(rt.ax,		rt.ay	), color);
	xui_canvas::get_ins()->draw_arc (xui_rect2d<s32>(
		rt.ax,			 
		rt.by-6,
		rt.ax+6, 
		rt.by), color, 90, 90, 1);
	xui_canvas::get_ins()->draw_arc (xui_rect2d<s32>(
		rt.bx-6, 
		rt.by-6,			
		rt.bx,		   
		rt.by), color,  0, 90, 1);
}

void xui_propctrl_stdvec_root::on_invalid( xui_method_args& args )
{
	xui_vector<s32> sz;
	sz.w  = get_renderw();
	sz.h  = m_border.ay + m_border.by;
	sz.h += m_header->get_renderh();
	sz.h += m_insert->get_renderh();
	sz.h += m_middle->get_renderh();
	if (get_rendersz() != sz)
	{
		set_rendersz(sz);
	}
	else
	{
		perform();
	}
}

void xui_propctrl_stdvec_root::on_perform( xui_method_args& args )
{
	xui_propctrl::on_perform(args);

	s32 indent = get_indent();
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt = rt.get_pt();
	//header
	m_header->on_perform_x (pt.x+indent);
	m_header->on_perform_y (pt.y);
	m_header->on_perform_w (rt.get_w()-indent);
	//middle
	pt.y += m_header->get_renderh();
	m_middle->on_perform_x (pt.x+indent);
	m_middle->on_perform_y (pt.y);
	m_middle->on_perform_w (rt.get_w()-indent);
	m_nontip->on_perform_x (pt.x+indent);
	m_nontip->on_perform_y (pt.y+m_middle->get_borderrt().ay);
	m_nontip->on_perform_w (rt.get_w()-indent);
	//delete
	pt.x  = rt.bx - m_delete->get_renderw();
	pt.y += m_middle->get_renderh();
	m_delete->on_perform_x (pt.x);
	m_delete->on_perform_y (pt.y);
	//insert
	pt.x -= m_insert->get_renderw();
	m_insert->on_perform_x (pt.x);
	m_insert->on_perform_y (pt.y);
}

void xui_propctrl_stdvec_root::on_middleinvalid( xui_component* sender, xui_method_args& args )
{
	s32 h = (m_middle->get_itemcount() == 0) ? m_nontip->get_renderh() : (m_middle->get_itemcount() * m_middle->get_lineheight());
	h += m_middle->get_borderrt().ay;
	h += m_middle->get_borderrt().by;
	m_middle->set_renderh(h);
}

void xui_propctrl_stdvec_root::on_middleselection( xui_component* sender, xui_method_args& args )
{
	xui_propdata_stdvec_root*  datastdvec = dynamic_cast<xui_propdata_stdvec_root*>(m_propdata);
	xui_proproot_vec rootvec = datastdvec->get_rootvec();

	std::vector<xui_listitem*> vec = m_middle->get_selecteditem();
	m_delete->set_enable(datastdvec->can_rootdel() && vec.size() > 0);

	xui_propview* propview = get_propview();
	if (vec.size() > 0)
	{
		xui_listitem* item = vec.front();
		xui_proproot* prop = (xui_proproot*)item->get_data();
		propview->add_propelse(rootvec, prop);
	}
	else
	{
		propview->del_propelse(rootvec);
	}
}

void xui_propctrl_stdvec_root::on_deleteclick( xui_component* sender, xui_method_args& args )
{
	std::vector<xui_listitem*> vec = m_middle->get_selecteditem();
	if (vec.size() > 0)
	{
		xui_propdata_stdvec_root* datastdvec = dynamic_cast<xui_propdata_stdvec_root*>(m_propdata);
		datastdvec->set_rootdel((xui_proproot*)vec.front()->get_data());
	}
}

void xui_propctrl_stdvec_root::on_deleterenderself( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt		= sender->get_renderrtabs();
	xui_colour		color   = sender->get_rendercolor() * sender->get_vertexcolor();
	xui_vector<s32> center	= xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);

	xui_canvas::get_ins()->fill_rectangle(xui_rect2d<s32>(
		center.x-4,
		center.y-1,
		center.x+4,
		center.y+1), color);
}

void xui_propctrl_stdvec_root::on_insertclick( xui_component* sender, xui_method_args& args )
{
	xui_propdata_stdvec_root* datastdvec = dynamic_cast<xui_propdata_stdvec_root*>(m_propdata);
	datastdvec->set_rootadd();
}

void xui_propctrl_stdvec_root::on_insertrenderself( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt		= sender->get_renderrtabs();
	xui_colour		color   = sender->get_rendercolor() * sender->get_vertexcolor();
	xui_vector<s32> center	= xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);

	xui_canvas::get_ins()->fill_rectangle(xui_rect2d<s32>(
		center.x-4,
		center.y-1,
		center.x+4,
		center.y+1), color);
	xui_canvas::get_ins()->fill_rectangle(xui_rect2d<s32>(
		center.x-1,
		center.y-4,
		center.x+1,
		center.y+4), color);
}