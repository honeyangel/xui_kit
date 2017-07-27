#include "xui_propctrl_stdvec.h"
#include "xui_convas.h"
#include "xui_desktop.h"
#include "xui_global.h"
#include "xui_menu.h"
#include "xui_menuitem.h"
#include "xui_button.h"
#include "xui_toggle.h"
#include "xui_listview.h"
#include "xui_listitem.h"
#include "xui_numbbox.h"
#include "xui_dropbox.h"
#include "xui_propkind.h"
#include "cocos_propctrl_uniform.h"

//////////////////////////////////////////////////////////////////////////
//propdata
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(cocos_propdata_uniform)( xui_propkind* kind, const std::wstring& name, uniform_vec* vec )
: xui_propdata(kind, name, cocos_propctrl_uniform::create)
, m_uniformvec(vec)
{
	for (u32 i = 0; i < m_uniformvec->size(); ++i)
		add_uniformdata(&(*m_uniformvec)[i]);
}

/*
//destructor
*/
xui_delete_explain(cocos_propdata_uniform)( void )
{
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
		delete m_propdatavec[i];
}

/*
//method
*/
xui_method_explain(cocos_propdata_uniform, get_uniformdatavec,	const xui_propdata_vec&	)( void ) const
{
	return m_propdatavec;
}
xui_method_explain(cocos_propdata_uniform, reset,				void					)( void )
{
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		m_propdatavec[i]->non_ctrl();
		delete m_propdatavec[i];
	}

	m_propdatavec.clear();
	for (u32 i = 0; i < m_uniformvec->size(); ++i)
		add_uniformdata(&(*m_uniformvec)[i]);

	if (m_ctrl)
		m_ctrl->on_linkpropdata();
}

/*
//override
*/
xui_method_explain(cocos_propdata_uniform, non_ctrl,			void					)( void )
{
	xui_propdata::non_ctrl();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
		m_propdatavec[i]->non_ctrl();
}

/*
//method
*/
xui_method_explain(cocos_propdata_uniform, add_uniformdata,		xui_propdata*			)( cocos_propmaterial::uniform* uniform_value )
{
	xui_propdata* propdata = NULL;
	
	if		(uniform_value->data.size() == 1 )	propdata = new_float(uniform_value);
	else if (uniform_value->data.size() == 2 )	propdata = new_vec2(uniform_value);
	else if (uniform_value->data.size() == 3 )	propdata = new_vec3(uniform_value);
	else if (uniform_value->data.size() == 4 )	propdata = new_vec4(uniform_value);
	else if (uniform_value->data.size() == 16)	propdata = new_mat4(uniform_value);

	if (propdata)
		m_propdatavec.push_back(propdata);

	return propdata;
}
xui_method_explain(cocos_propdata_uniform, new_float,			xui_propdata*			)( cocos_propmaterial::uniform* uniform_value )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"Value",
		xui_propctrl_number::create,
		&uniform_value->data[0],
		NT_FLOAT));
	
	return new xui_propdata_expand(
		m_kind, 
		xui_global::ascii_to_unicode(uniform_value->name), 
		xui_propctrl_expand_plus::create, 
		subprop);
}
xui_method_explain(cocos_propdata_uniform, new_vec2,			xui_propdata*			)( cocos_propmaterial::uniform* uniform_value )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"x",
		xui_propctrl_number::create,
		&uniform_value->data[0],
		NT_FLOAT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"y",
		xui_propctrl_number::create,
		&uniform_value->data[1],
		NT_FLOAT));

	return new xui_propdata_expand(
		m_kind,
		xui_global::ascii_to_unicode(uniform_value->name),
		xui_propctrl_expand_plus::create,
		subprop);
}
xui_method_explain(cocos_propdata_uniform, new_vec3,			xui_propdata*			)( cocos_propmaterial::uniform* uniform_value )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"x",
		xui_propctrl_number::create,
		&uniform_value->data[0],
		NT_FLOAT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"y",
		xui_propctrl_number::create,
		&uniform_value->data[1],
		NT_FLOAT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"z",
		xui_propctrl_number::create,
		&uniform_value->data[2],
		NT_FLOAT));

	return new xui_propdata_expand(
		m_kind,
		xui_global::ascii_to_unicode(uniform_value->name),
		xui_propctrl_expand_plus::create,
		subprop);
}
xui_method_explain(cocos_propdata_uniform, new_vec4,			xui_propdata*			)( cocos_propmaterial::uniform* uniform_value )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"x",
		xui_propctrl_number::create,
		&uniform_value->data[0],
		NT_FLOAT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"y",
		xui_propctrl_number::create,
		&uniform_value->data[1],
		NT_FLOAT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"z",
		xui_propctrl_number::create,
		&uniform_value->data[2],
		NT_FLOAT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		m_kind,
		L"w",
		xui_propctrl_number::create,
		&uniform_value->data[3],
		NT_FLOAT));

	return new xui_propdata_expand(
		m_kind,
		xui_global::ascii_to_unicode(uniform_value->name),
		xui_propctrl_expand_plus::create,
		subprop);
}
xui_method_explain(cocos_propdata_uniform, new_mat4,			xui_propdata*			)( cocos_propmaterial::uniform* uniform_value )
{
	xui_propdata_vec subprop;
	for (u32 i = 0; i < uniform_value->data.size(); ++i)
	{
		std::wstringstream text;
		text << L"m";
		text << i/4;
		text << i%4;
		subprop.push_back(new xui_propdata_number_impl<f32>(
			m_kind,
			text.str(),
			xui_propctrl_number::create,
			&uniform_value->data[i],
			NT_FLOAT));
	}

	return new xui_propdata_expand(
		m_kind,
		xui_global::ascii_to_unicode(uniform_value->name),
		xui_propctrl_expand_plus::create,
		subprop);
}

//////////////////////////////////////////////////////////////////////////
//propctrl
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(cocos_propctrl_uniform, xui_propctrl);

/*
//constructor
*/
xui_create_explain(cocos_propctrl_uniform)( xui_propdata* propdata )
: xui_propctrl()
{
	m_border = xui_rect2d<s32>(0, 8, 0, 8);
	m_header = new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_header, set_parent			)(this);
	xui_method_ptrcall(m_header, set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_header, set_borderrt		)(xui_rect2d<s32>(8, 2, 2, 2));
	xui_method_ptrcall(m_header, ini_drawer			)(propdata->get_name());
	m_nontip = new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_nontip, set_parent			)(this);
	xui_method_ptrcall(m_nontip, set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_nontip, set_borderrt		)(xui_rect2d<s32>(8, 2, 2, 2));
	xui_method_ptrcall(m_nontip, ini_component		)(true, false);
	xui_method_ptrcall(m_nontip, ini_drawer			)(L"Empty");

	m_middle = xui_listview::create(xui_vector<s32>(0), false);
	xui_method_ptrcall(m_middle, set_parent			)(this);
	xui_method_ptrcall(m_middle, set_drawcolor		)(false);
	xui_method_ptrcall(m_middle, set_sidestyle		)(SIDESTYLE_N);
	xui_method_ptrcall(m_middle, set_hscrollauto	)(false);
	xui_method_ptrcall(m_middle, xm_invalid			) += new xui_method_member<xui_method_args, cocos_propctrl_uniform>(this, &cocos_propctrl_uniform::on_middleinvalid);
	xui_method_ptrcall(m_middle, xm_selectedchange	) += new xui_method_member<xui_method_args, cocos_propctrl_uniform>(this, &cocos_propctrl_uniform::on_middleselection);

	m_expand = new xui_propctrl_expand_plus;
	xui_method_ptrcall(m_expand, set_parent			)(this);
	xui_method_ptrcall(m_expand, ini_component		)(true, false);

	m_widgetvec.push_back(m_header);
	m_widgetvec.push_back(m_middle);
	m_widgetvec.push_back(m_nontip);
	m_widgetvec.push_back(m_expand);
}

/*
//static
*/
xui_method_explain(cocos_propctrl_uniform, create,				xui_propctrl*			)( xui_propdata* propdata )
{
	return new cocos_propctrl_uniform(propdata);
}

/*
//override
*/
xui_method_explain(cocos_propctrl_uniform, on_linkpropdata,		void					)( bool selfupdate )
{
	cocos_propdata_uniform* datauniform = dynamic_cast<cocos_propdata_uniform*>(m_propdata);
	const xui_propdata_vec& datavec = datauniform->get_uniformdatavec();

	if (selfupdate == false || m_middle->get_itemcount() != datavec.size())
	{
		xui_method_ptrcall(m_middle, del_itemall)();
		for (u32 i = 0; i < datavec.size(); ++i)
		{
			xui_propdata* data = datavec[i];
			if (data)
			{
				xui_listitem * item = m_middle->add_item(data->get_name());
				item->set_data(data);
			}
		}
		xui_method_ptrcall(m_expand, ini_component	)(true, false);
		xui_method_ptrcall(m_nontip, ini_component	)(true, datavec.size() == 0);
		xui_method_ptrcall(m_middle, refresh		)();
		refresh();
	}
}
xui_method_explain(cocos_propctrl_uniform, on_editvalue,		void					)( xui_propedit* sender )
{}

/*
//callback
*/
xui_method_explain(cocos_propctrl_uniform, on_renderback,		void					)( xui_method_args& args )
{
	xui_propctrl::on_renderback(args);

	xui_rect2d<s32> rt;
	xui_colour		color = get_vertexcolor();
	rt = m_header->get_renderrtabs();
	xui_convas::get_ins()->fill_round(rt, xui_colour(1.0f, 0.4f) * color, xui_rect2d<s32>(3, 3, 0, 0));
	rt = m_middle->get_renderrtabs();
	xui_convas::get_ins()->fill_round(rt, xui_colour(1.0f, 0.3f) * color, xui_rect2d<s32>(0, 0, 0, 3));
	//rt = m_insert->get_renderrtabs().get_union(m_delete->get_renderrtabs());
	//xui_convas::get_ins()->fill_round(rt, xui_colour(1.0f, 0.3f) * color, xui_rect2d<s32>(0, 0, 3, 3));

	color *= m_sidecolor;
	rt = m_header->get_renderrtabs();
	xui_convas::get_ins()->draw_round(rt, color, xui_rect2d<s32>(3, 3, 0, 0));
	rt = m_middle->get_renderrtabs();
	xui_convas::get_ins()->draw_round(rt, color, xui_rect2d<s32>(0, 0, 3, 3));
	//xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax, rt.by - 3), xui_vector<s32>(rt.ax, rt.ay), color);
	//xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx - 48, rt.by), xui_vector<s32>(rt.ax + 3, rt.by), color);
	//xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx, rt.ay), xui_vector<s32>(rt.bx, rt.by), color);
	//xui_convas::get_ins()->draw_arc(xui_rect2d<s32>(
	//	rt.ax,
	//	rt.by - 6,
	//	rt.ax + 6,
	//	rt.by), color, 90, 90, 1);

	//rt = m_insert->get_renderrtabs().get_union(m_delete->get_renderrtabs());
	//xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx, rt.ay), xui_vector<s32>(rt.bx, rt.by - 3), color);
	//xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx - 3, rt.by), xui_vector<s32>(rt.ax + 3, rt.by), color);
	//xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax, rt.by - 3), xui_vector<s32>(rt.ax, rt.ay), color);
	//xui_convas::get_ins()->draw_arc(xui_rect2d<s32>(
	//	rt.ax,
	//	rt.by - 6,
	//	rt.ax + 6,
	//	rt.by), color, 90, 90, 1);
	//xui_convas::get_ins()->draw_arc(xui_rect2d<s32>(
	//	rt.bx - 6,
	//	rt.by - 6,
	//	rt.bx,
	//	rt.by), color, 0, 90, 1);
}
xui_method_explain(cocos_propctrl_uniform, on_invalid,			void					)( xui_method_args& args )
{
	xui_vector<s32> sz;
	sz.w  = get_renderw();
	sz.h  = m_border.ay + m_border.by;
	sz.h += m_header->get_renderh();
	sz.h += m_middle->get_renderh();
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
xui_method_explain(cocos_propctrl_uniform, on_perform,			void					)( xui_method_args& args )
{
	xui_propctrl::on_perform(args);

	s32 indent = get_indent();
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt = rt.get_pt();
	//header
	m_header->on_perform_x(pt.x + indent);
	m_header->on_perform_y(pt.y);
	m_header->on_perform_w(rt.get_w() - indent);
	//middle
	pt.y += m_header->get_renderh();
	m_middle->on_perform_x(pt.x + indent);
	m_middle->on_perform_y(pt.y);
	m_middle->on_perform_w(rt.get_w() - indent);
	m_nontip->on_perform_x(pt.x + indent);
	m_nontip->on_perform_y(pt.y + m_middle->get_borderrt().ay);
	m_nontip->on_perform_w(rt.get_w() - indent);
	//expand
	pt.x = rt.ax;
	pt.y += m_middle->get_renderh();
	m_expand->on_perform_x(pt.x);
	m_expand->on_perform_y(pt.y);
	m_expand->on_perform_w(rt.get_w());
}

/*
//event
*/
xui_method_explain(cocos_propctrl_uniform, on_middleinvalid,	void					)( xui_component* sender, xui_method_args& args )
{
	s32 h = (m_middle->get_itemcount() == 0) ? m_nontip->get_renderh() : (m_middle->get_itemcount() * m_middle->get_lineheight());
	h += m_middle->get_borderrt().ay;
	h += m_middle->get_borderrt().by;
	m_middle->set_renderh(h);
}
xui_method_explain(cocos_propctrl_uniform, on_middleselection,	void					)( xui_component* sender, xui_method_args& args )
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
