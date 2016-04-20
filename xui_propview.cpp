#include "xui_convas.h"
#include "xui_scroll.h"
#include "xui_propdata.h"
#include "xui_kindctrl.h"
#include "xui_propview.h"

xui_implement_rtti(xui_propview, xui_container);

/*
//static
*/
const s32 xui_propview::default_nodeindent = 16;
const s32 xui_propview::default_lineheight = 20;

xui_method_explain(xui_propview, create,			xui_propview*			)( void )
{
	xui_propview* propview = new xui_propview(xui_vector<s32>(480, 380));
	xui_method_ptrcall(propview, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(propview, set_borderrt	)(4);
	xui_method_ptrcall(propview, set_corner		)(3);
	return propview;
}

/*
//constructor
*/
xui_create_explain(xui_propview)( const xui_vector<s32>& size )
: xui_container(size)
{
	m_proproot = NULL;
}

/*
//destructor
*/
xui_delete_explain(xui_propview)( void )
{
	for (xui_kindctrl_map::iterator itor = m_kindctrlmap.begin(); itor != m_kindctrlmap.end(); ++itor)
	{
		std::vector<xui_kindctrl*>& vec = (*itor).second;
		for (u32 i = 0; i < vec.size(); ++i)
		{
			xui_kindctrl* kindctrl = vec[i];
			if (kindctrl->get_parent() == this)
				continue;

			delete vec[i];
		}
	}
}

/*
//property
*/
xui_method_explain(xui_propview, get_proproot,		const xui_proproot_vec&	)( void ) const
{
	return m_proprootvec;
}
xui_method_explain(xui_propview, set_proproot,		void					)( xui_proproot* proproot )
{
	//detach
	for (u32 i = 0; i < m_proprootvec.size(); ++i)
		m_proprootvec[i]->non_ctrl();
	for (u32 i = 0; i < m_propelsevec.size(); ++i)
		m_propelsevec[i]->non_ctrl();

	//remove
	del_kindctrlall();

	//set
	m_proproot = proproot;
	m_proprootvec.clear();
	m_propelsevec.clear();
	if (m_proproot)
	{
		m_proproot->set_ctrl(this);
		m_proprootvec.push_back(m_proproot);

		const xui_propkind_vec& vec = m_proproot->get_propkind();
		for (u32 i = 0; i < vec.size(); ++i)
		{
			xui_propkind* propkind = vec[i];
			xui_kindctrl* kindctrl = get_kindctrl(propkind);
			if (kindctrl)
			{
				xui_method_ptrcall(kindctrl, set_propkind)(propkind);
				xui_method_ptrcall(propkind, set_ctrl	 )(kindctrl);
			}
		}
	}

	refresh();
}
xui_method_explain(xui_propview, set_proproot,		void					)( const xui_proproot_vec& proproot )
{
	if		(proproot.size() == 0)	set_proproot(NULL);
	else if (proproot.size() == 1)	set_proproot(proproot.front());
	else
	{
		//detach
		for (u32 i = 0; i < m_proprootvec.size(); ++i)
			m_proprootvec[i]->non_ctrl();
		for (u32 i = 0; i < m_propelsevec.size(); ++i)
			m_propelsevec[i]->non_ctrl();

		//remove
		del_kindctrlall();

		//set
		m_proproot	  = proproot.front();
		m_proprootvec = proproot;
		m_propelsevec.clear();
		for (u32 i = 0; i < m_proprootvec.size(); ++i)
			m_proprootvec[i]->set_ctrl(this);

		xui_propkind_vec vec = get_samekind();
		for (u32 i = 0; i < vec.size(); ++i)
		{
			xui_propkind* propkind = vec[i];
			xui_kindctrl* kindctrl = get_kindctrl(propkind);
			if (kindctrl)
			{
				xui_propkind_vec propkindall = get_propkindall(propkind);
				kindctrl->set_propkind(propkindall);
				for (xui_propkind_vec::iterator itor = propkindall.begin(); itor != propkindall.end(); ++itor)
				{
					(*itor)->set_ctrl(kindctrl);
				}
			}
		}

		refresh();
	}
}
xui_method_explain(xui_propview, add_propelse,		void					)( const xui_proproot_vec& lastroot, xui_proproot* propelse )
{
	u32 repidx = -1;
	for (u32 i = 0; i < m_propelsevec.size(); ++i)
	{
		xui_proproot_vec::const_iterator itor = std::find(
			lastroot.begin(),
			lastroot.end(), 
			m_propelsevec[i]);

		if (itor != lastroot.end())
		{
			repidx = i;
			break;
		}
	}

	if (repidx != -1)
	{
		for (u32 i = 0; i < m_propelsevec.size(); ++i)
			del_kindctrl(m_propelsevec[i]);

		m_propelsevec[repidx] = propelse;
	}
	else
	{
		m_propelsevec.push_back(propelse);
	}

	for (u32 i = 0; i < m_propelsevec.size(); ++i)
	{
		xui_proproot* proproot = m_propelsevec[i];
		if (proproot->get_ctrl())
			continue;

		proproot->set_ctrl(this);
		const xui_propkind_vec& vec = proproot->get_propkind();
		for (xui_propkind_vec::const_iterator itor = vec.begin(); itor != vec.end(); ++itor)
		{
			xui_propkind* propkind = (*itor);
			xui_kindctrl* kindctrl = get_kindctrl(propkind);
			if (kindctrl)
			{
				xui_method_ptrcall(kindctrl, set_propkind)(propkind);
				xui_method_ptrcall(propkind, set_ctrl	 )(kindctrl);
			}
		}

	}

	refresh();
}
xui_method_explain(xui_propview, del_propelse,		void					)( const xui_proproot_vec& lastroot )
{
	for (u32 i = 0; i < m_propelsevec.size(); ++i)
	{
		xui_proproot_vec::const_iterator itor = std::find(
			lastroot.begin(),
			lastroot.end(),
			m_propelsevec[i]);

		if (itor != lastroot.end())
		{
			m_propelsevec.erase(m_propelsevec.begin()+i);
			del_kindctrl(m_propelsevec[i]);
			refresh();
			break;
		}
	}
}
xui_method_explain(xui_propview, del_proproot,		void					)( xui_proproot* proproot )
{
	xui_proproot_vec::iterator itor = std::find(
		m_proprootvec.begin(),
		m_proprootvec.end(),
		proproot);

	if (itor != m_proprootvec.end())
	{
		m_proprootvec.erase(itor);
		xui_proproot_vec vec = m_proprootvec;
		set_proproot(vec);
	}
	else
	{
		itor = std::find(m_propelsevec.begin(), m_propelsevec.end(), proproot);
		if (itor != m_propelsevec.end())
		{
			m_propelsevec.erase(itor);
			del_kindctrl((*itor));
			refresh();
		}
	}
}

/*
//virtual
*/
xui_method_explain(xui_propview, render_else,		void					)( void )
{
	xui_colour vertexcolor = get_vertexcolor();
	xui_rect2d<s32>     rt = get_renderrtabs();
	rt.ay += m_corner;
	rt.by -= m_corner;
	xui_rect2d<s32> cliprect = xui_convas::get_ins()->get_cliprect();
	xui_convas::get_ins()->set_cliprect(cliprect.get_inter(rt));
	if (m_ascrollitem.size() > 1)
	{
		s32 bx = (m_vscroll == NULL) ? rt.bx : get_renderrtins().bx;
		xui_vector<s32> p1 = get_screenpt();
		xui_vector<s32> p2 = p1 + xui_vector<s32>(bx, 0);
		for (u32 i = 1; i < m_ascrollitem.size(); ++i)
		{
			xui_kindctrl* kindctrl = xui_dynamic_cast(xui_kindctrl, m_ascrollitem[i]);
			xui_propkind* propkind = kindctrl->get_propkind();
			if (kindctrl->was_visible() == false)
				continue;
			if (propkind && propkind->was_headshow() == false)
				continue;

			p1.y = kindctrl->get_renderrtabs().ay;
			p2.y = kindctrl->get_renderrtabs().ay;
			xui_convas::get_ins()->draw_line(p1, p2, m_sidecolor*vertexcolor);
		}
	}
	xui_convas::get_ins()->set_cliprect(cliprect);

	xui_container::render_else();
}

/*
//override
*/
xui_method_explain(xui_propview, on_invalid,		void					)( xui_method_args& args )
{
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> sz(0);

	for (u32 i = 0; i < m_ascrollitem.size(); ++i)
	{
		xui_kindctrl* kindctrl = xui_dynamic_cast(xui_kindctrl, m_ascrollitem[i]);
		xui_propkind* propkind = kindctrl->get_propkind();
		xui_method_ptrcall(kindctrl, set_enable )(propkind->can_edit());
		xui_method_ptrcall(kindctrl, set_visible)(propkind->can_show());
		if (propkind->can_show() == false)
			continue;

		sz.h += kindctrl->get_renderh();
	}

	sz.w = xui_max(sz.w, rt.get_w());
	sz.h = xui_max(sz.h, rt.get_h());

	if (get_clientsz() != sz)
	{
		set_clientsz(sz);
	}
	else
	{
		perform();
	}
}
xui_method_explain(xui_propview, on_perform,		void					)( xui_method_args& args )
{
	xui_container::on_perform(args);
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt;
	pt.x = m_border.ax;
	pt.y = m_border.ay;

	for (u32 i = 0; i < m_ascrollitem.size(); ++i)
	{
		xui_kindctrl* kindctrl = xui_dynamic_cast(xui_kindctrl, m_ascrollitem[i]);
		if (kindctrl->was_visible() == false)
			continue;

		kindctrl->on_perform_pt(pt);
		kindctrl->on_perform_w (rt.get_w());
		pt.y += kindctrl->get_renderh();
	}
}

/*
//method
*/
xui_method_explain(xui_propview, del_kindctrlall,	void					)( void )
{
	for (u32 i = 0; i < m_ascrollitem.size(); ++i)
		m_ascrollitem[i]->set_parent(NULL);

	m_ascrollitem.clear();
}
xui_method_explain(xui_propview, del_kindctrl,		void					)( xui_proproot* proproot )
{
	const xui_propkind_vec& vec = proproot->get_propkind();
	for (u32 i = 0; i < vec.size(); ++i)
		del_kindctrl(vec[i]);

	proproot->non_ctrl();
}
xui_method_explain(xui_propview, del_kindctrl,		void					)( xui_propkind* propkind )
{
	std::vector<xui_control*>::iterator itor = std::find(
		m_ascrollitem.begin(),
		m_ascrollitem.end(),
		propkind->get_ctrl());

	if (itor != m_ascrollitem.end())
	{
		propkind->get_ctrl()->set_parent(NULL);
		m_ascrollitem.erase(itor);
	}
}
xui_method_explain(xui_propview, get_kindctrl,		xui_kindctrl*			)( xui_propkind* propkind )
{
	xui_kindctrl* kindctrl = NULL;
	xui_kindctrl_map::iterator itor = m_kindctrlmap.find(propkind->get_type());
	if (itor != m_kindctrlmap.end())
	{
		std::vector<xui_kindctrl*>& vec = (*itor).second;
		for (u32 i = 0; i < vec.size(); ++i)
		{
			if (vec[i]->get_parent() == NULL)
			{
				kindctrl = vec[i];
				break;
			}
		}
	}

	if (kindctrl == NULL)
	{
		std::string      type = propkind->get_type();
		xui_kind_newctrl func = propkind->get_func();
		kindctrl  = (*func)(propkind);
		xui_method_ptrcall(kindctrl, set_name)(type);
		xui_method_ptrcall(kindctrl, refresh )();
		m_kindctrlmap[type].push_back(kindctrl);
	}

	xui_vector<s32> pt;
	pt.x = (m_hscroll == NULL) ? 0 : m_hscroll->get_value();
	pt.y = (m_vscroll == NULL) ? 0 : m_vscroll->get_value();
	xui_method_ptrcall(kindctrl, set_parent	 )(this);
	xui_method_ptrcall(kindctrl, set_scrollpt)(pt);
	m_ascrollitem.push_back(kindctrl);

	return kindctrl;
}
xui_method_explain(xui_propview, get_samekind,		xui_propkind_vec		)( void )
{
	xui_propkind_vec result;
	if (m_proprootvec.size() == 1)
	{
		result = m_proproot->get_propkind();
	}
	else
	if (m_proprootvec.size() >  1)
	{
		const xui_propkind_vec& vec = m_proproot->get_propkind();
		for (u32 i = 0; i < vec.size(); ++i)
		{
			xui_propkind* propkind = vec[i];
			bool hasexist = false;
			for (xui_propkind_vec::iterator itor = result.begin(); itor != result.end(); ++itor)
			{
				if ((*itor)->get_func() == propkind->get_func() &&
					(*itor)->get_name() == propkind->get_name())
				{
					hasexist = true;
					break;
				}
			}

			if (hasexist)
				continue;

			bool samekind = true;
			for (xui_proproot_vec::iterator itor = m_proprootvec.begin(); itor != m_proprootvec.end(); ++itor)
			{
				if ((*itor)->has_propkind(propkind) == false)
				{
					samekind = false;
					break;
				}
			}

			if (samekind)
				result.push_back(propkind);
		}
	}

	return result;
}
xui_method_explain(xui_propview, get_propkindall,	xui_propkind_vec		)( xui_propkind* propkind )
{
	xui_propkind_vec result;
	for (xui_proproot_vec::iterator itor = m_proprootvec.begin(); itor != m_proprootvec.end(); ++itor)
	{
		const xui_propkind_vec& vec = (*itor)->get_propkind();
		for (u32 i = 0; i < vec.size(); ++i)
		{
			if (vec[i]->get_func() == propkind->get_func() &&
				vec[i]->get_name() == propkind->get_name())
			{
				result.push_back(vec[i]);
				break;
			}
		}
	}

	return result;
}