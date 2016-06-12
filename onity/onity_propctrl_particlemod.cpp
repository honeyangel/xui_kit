#include "NPParticleSpriteSRC.h"
#include "NPParticleModLifetimeClasses.h"

#include "xui_convas.h"
#include "xui_desktop.h"
#include "xui_menu.h"
#include "xui_menuitem.h"
#include "xui_button.h"
#include "xui_toggle.h"
#include "xui_listview.h"
#include "xui_listitem.h"
#include "xui_textbox.h"
#include "onity_propctrl_particlemod.h"

//////////////////////////////////////////////////////////////////////////
//propdata
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(onity_propdata_particlemod)( xui_propkind* kind, const std::wstring& name, NPParticleSpriteSRC* spritesrc )
: xui_propdata(kind, name, onity_propctrl_particlemod::create)
, m_spritesrc(spritesrc)
{
	for (u32 i = 0; i < m_spritesrc->NumParticleMOD(); ++i)
		add_modifydata(m_spritesrc->At(i));
}

/*
//destructor
*/
xui_delete_explain(onity_propdata_particlemod)( void )
{
	modify_propdata_map::iterator itor = m_spritemodmap.begin();
	for (; itor != m_spritemodmap.end(); ++itor)
		delete (*itor).first;
}

/*
//method
*/
xui_method_explain(onity_propdata_particlemod, get_modifydatavec,	xui_propdata_vec)( void )
{
	xui_propdata_vec result;
	modify_propdata_map::iterator itor = m_spritemodmap.begin();
	for (; itor != m_spritemodmap.end(); ++itor)
		result.push_back((*itor).first);

	return result;
}

/*
//override
*/
xui_method_explain(onity_propdata_particlemod, non_ctrl,			void			)( void )
{
	xui_propdata::non_ctrl();
	modify_propdata_map::iterator itor = m_spritemodmap.begin();
	for (; itor != m_spritemodmap.end(); ++itor)
		(*itor).first->non_ctrl();
}

/*
//method
*/
xui_method_explain(onity_propdata_particlemod, add_modifydata,		void			)( NPParticleMOD* mod )
{
	xui_propdata* propdata = NULL;
	if		(NPIsExaKindOf(NPParticleSeedLifetimeMOD, mod)) propdata = new_seedlifetime(NPDynamicCast(NPParticleSeedLifetimeMOD, mod));
	//else if ()
	else
	{}

	if (propdata)
	{
		m_spritemodmap[propdata] = mod;
	}
}
xui_method_explain(onity_propdata_particlemod, new_seedlifetime,	xui_propdata*	)( NPParticleSeedLifetimeMOD* mod )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MinBaseLifetime",
		xui_propctrl_number::create,
		&mod->mMinBaseLifetime,
		NT_FLOAT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"MaxBaseLifetime",
		xui_propctrl_number::create,
		&mod->mMaxBaseLifetime,
		NT_FLOAT));

	return new xui_propdata_expand(m_kind, L"SeedLifetime", xui_propctrl_expand_plus::create, subprop);
}

//////////////////////////////////////////////////////////////////////////
//propctrl
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(onity_propctrl_particlemod, xui_propctrl);

/*
//constructor
*/
xui_create_explain(onity_propctrl_particlemod)( xui_propdata* propdata )
: xui_propctrl()
{
	m_border = xui_rect2d<s32>(0, 8, 0, 8);
	m_header = new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_header,	set_parent			)(this);
	xui_method_ptrcall(m_header,	set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_header,	set_borderrt		)(xui_rect2d<s32>(8, 2, 2, 2));
	xui_method_ptrcall(m_header,	ini_drawer			)(propdata->get_name());
	m_nontip = new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_nontip,	set_parent			)(this);
	xui_method_ptrcall(m_nontip,	set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_nontip,	set_borderrt		)(xui_rect2d<s32>(8, 2, 2, 2));
	xui_method_ptrcall(m_nontip,	ini_component		)(true, false);
	xui_method_ptrcall(m_nontip,	ini_drawer			)(L"Empty");

	m_insert = new xui_toggle(xui_vector<s32>(24, 16), TOGGLE_BUTTON);
	xui_method_ptrcall(m_insert,	set_parent			)(this);
	xui_method_ptrcall(m_insert,	set_backcolor		)(xui_colour::white);
	xui_method_ptrcall(m_insert,	set_movecolor		)(xui_button::default_downcolor);
	xui_method_ptrcall(m_insert,	xm_renderself		) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_insertrenderself);

	m_delete = new xui_button(xui_vector<s32>(24, 16));
	xui_method_ptrcall(m_delete,	set_parent			)(this);
	xui_method_ptrcall(m_delete,	set_backcolor		)(xui_colour::white);
	xui_method_ptrcall(m_delete,	set_movecolor		)(xui_button::default_downcolor);
	xui_method_ptrcall(m_delete,	xm_buttonclick		) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_deleteclick);
	xui_method_ptrcall(m_delete,	xm_renderself		) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_deleterenderself);

	m_middle = xui_listview::create(xui_vector<s32>(0), false);
	xui_method_ptrcall(m_middle,	set_parent			)(this);
	xui_method_ptrcall(m_middle,	set_drawcolor		)(false);
	xui_method_ptrcall(m_middle,	set_sidestyle		)(SIDESTYLE_N);
	xui_method_ptrcall(m_middle,	set_hscrollauto		)(false);
	xui_method_ptrcall(m_middle,	xm_invalid			) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_middleinvalid);
	xui_method_ptrcall(m_middle,	xm_selectedchange	) += new xui_method_member<xui_method_args, onity_propctrl_particlemod>(this, &onity_propctrl_particlemod::on_middleselection);

	m_expand = new xui_propctrl_expand_plus;
	xui_method_ptrcall(m_expand,	set_parent			)(this);
	xui_method_ptrcall(m_expand,	ini_component		)(true, false);

	m_widgetvec.push_back(m_header);
	m_widgetvec.push_back(m_insert);
	m_widgetvec.push_back(m_delete);
	m_widgetvec.push_back(m_middle);
	m_widgetvec.push_back(m_nontip);
	m_widgetvec.push_back(m_expand);
}

/*
//static
*/
xui_method_explain(onity_propctrl_particlemod, create,				xui_propctrl*	)( xui_propdata* propdata )
{
	return new onity_propctrl_particlemod(propdata);
}

/*
//override
*/
xui_method_explain(onity_propctrl_particlemod, on_linkpropdata,		void			)( void )
{
	onity_propdata_particlemod*  dataspritemod = dynamic_cast<onity_propdata_particlemod*>(m_propdata);
	xui_propdata_vec datavec = dataspritemod->get_modifydatavec();

	xui_method_ptrcall(m_middle, del_itemall)();
	for (u32 i = 0; i < datavec.size(); ++i)
	{
		xui_propdata*  data = datavec[i];
		xui_listitem * item = m_middle->add_item(data->get_name());
		item->set_data(data);
	}
	xui_method_ptrcall(m_expand, ini_component	)(true, false);
	xui_method_ptrcall(m_nontip, ini_component	)(true, datavec.size() == 0);
	xui_method_ptrcall(m_middle, refresh		)();
}
xui_method_explain(onity_propctrl_particlemod, on_editvalue,		void			)( xui_propedit* sender )
{}

/*
//callback
*/
xui_method_explain(onity_propctrl_particlemod, on_renderback,		void			)( xui_method_args& args )
{
	xui_propctrl::on_renderback(args);

	xui_rect2d<s32> rt;
	xui_colour		color = get_vertexcolor();
	rt = m_header->get_renderrtabs();
	xui_convas::get_ins()->fill_round(rt, xui_colour(1.0f, 0.4f) * color, xui_rect2d<s32>(3, 3, 0, 0));
	rt = m_middle->get_renderrtabs();
	xui_convas::get_ins()->fill_round(rt, xui_colour(1.0f, 0.3f) * color, xui_rect2d<s32>(0, 0, 0, 3));
	rt = m_insert->get_renderrtabs().get_union(m_delete->get_renderrtabs());
	xui_convas::get_ins()->fill_round(rt, xui_colour(1.0f, 0.3f) * color, xui_rect2d<s32>(0, 0, 3, 3));

	color *= m_sidecolor;
	rt = m_header->get_renderrtabs();
	xui_convas::get_ins()->draw_round(rt, color, xui_rect2d<s32>(3, 3, 0, 0));
	rt = m_middle->get_renderrtabs();
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax,		rt.by-3	), xui_vector<s32>(rt.ax,		rt.ay	), color);
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx-48,	rt.by	), xui_vector<s32>(rt.ax+3,		rt.by   ), color);
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx,		rt.ay	), xui_vector<s32>(rt.bx,		rt.by	), color);
	xui_convas::get_ins()->draw_arc (xui_rect2d<s32>(
		rt.ax,			 
		rt.by-6,
		rt.ax+6, 
		rt.by), color, 90, 90, 1);

	rt = m_insert->get_renderrtabs().get_union(m_delete->get_renderrtabs());
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx,		rt.ay	), xui_vector<s32>(rt.bx,		rt.by-3	), color);
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx-3,	rt.by	), xui_vector<s32>(rt.ax+3,		rt.by	), color);
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax,		rt.by-3	), xui_vector<s32>(rt.ax,		rt.ay	), color);
	xui_convas::get_ins()->draw_arc (xui_rect2d<s32>(
		rt.ax,			 
		rt.by-6,
		rt.ax+6, 
		rt.by), color, 90, 90, 1);
	xui_convas::get_ins()->draw_arc (xui_rect2d<s32>(
		rt.bx-6, 
		rt.by-6,			
		rt.bx,		   
		rt.by), color,  0, 90, 1);
}
xui_method_explain(onity_propctrl_particlemod, on_invalid,			void			)( xui_method_args& args )
{
	xui_vector<s32> sz;
	sz.w  = get_renderw();
	sz.h  = m_border.ay + m_border.by;
	sz.h += m_header->get_renderh();
	sz.h += m_middle->get_renderh();
	sz.h += m_insert->get_renderh();
	sz.h += m_expand->was_visible() ? m_expand->get_renderh() : 0;
	if (get_rendersz() != sz)
	{
		set_rendersz(sz);
	}
	else
	{
		perform();
	}
}
xui_method_explain(onity_propctrl_particlemod, on_perform,			void			)( xui_method_args& args )
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
	//expand
	pt.x  = rt.ax;
	pt.y += m_insert->get_renderh();
	m_expand->on_perform_x (pt.x);
	m_expand->on_perform_y (pt.y);
	m_expand->on_perform_w (rt.get_w());
}

/*
//event
*/
xui_method_explain(onity_propctrl_particlemod, on_middleinvalid,	void			)( xui_component* sender, xui_method_args& args )
{
	s32 h = (m_middle->get_itemcount() == 0) ? m_nontip->get_renderh() : (m_middle->get_itemcount() * m_middle->get_lineheight());
	h += m_middle->get_borderrt().ay;
	h += m_middle->get_borderrt().by;
	m_middle->set_renderh(h);
}
xui_method_explain(onity_propctrl_particlemod, on_middleselection,	void			)( xui_component* sender, xui_method_args& args )
{
	xui_propdata_vec datavec = m_expand->get_propdata();
	for (u32 i = 0; i < datavec.size(); ++i)
		datavec[i]->non_ctrl();

	std::vector<xui_listitem*> itemvec = m_middle->get_selecteditem();
	if (itemvec.size() > 0)
	{
		xui_listitem* item = itemvec.front();
		xui_propdata* data = (xui_propdata*)item->get_data();
		m_expand->set_propdata(data);
		m_expand->refresh();

		data->set_ctrl(m_expand);
	}

	m_expand->set_visible(itemvec.size() > 0);
	refresh();
}
xui_method_explain(onity_propctrl_particlemod, on_deleteclick,		void			)( xui_component* sender, xui_method_args& args )
{

}
xui_method_explain(onity_propctrl_particlemod, on_deleterenderself,	void			)( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt		= sender->get_renderrtabs();
	xui_colour		color   = sender->get_rendercolor() * sender->get_vertexcolor();
	xui_vector<s32> center	= xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);

	xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
		center.x-4,
		center.y-1,
		center.x+4,
		center.y+1), color);
}
xui_method_explain(onity_propctrl_particlemod, on_insertrenderself,	void			)( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt		= sender->get_renderrtabs();
	xui_colour		color   = sender->get_rendercolor() * sender->get_vertexcolor();
	xui_vector<s32> center	= xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);

	xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
		center.x-4,
		center.y-1,
		center.x+4,
		center.y+1), color);
	xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
		center.x-1,
		center.y-4,
		center.x+1,
		center.y+4), color);
}