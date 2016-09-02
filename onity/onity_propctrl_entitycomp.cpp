#include "xui_textbox.h"
#include "xui_dropbox.h"
#include "xui_button.h"
#include "xui_toggle.h"
#include "xui_panel.h"
#include "xui_global.h"
#include "xui_convas.h"
#include "onity_propkind_entitycomp.h"
#include "onity_propctrl_entitycomp.h"

//////////////////////////////////////////////////////////////////////////
//compattr
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(onity_compattr, xui_control);

/*
//static
*/
const s32 onity_compattr::default_height = 20;

/*
//constructor
*/
xui_create_explain(onity_compattr)( onity_propkind_entitycomp* compkind, const std::string& name )
: xui_control(xui_vector<s32>(128, default_height))
, m_compkind(compkind)
, m_attrname(name)
{
	BreezeGame::Json::Value* node = compkind->get_node();
	BreezeGame::Json::Value* attr = &((*node)[name]);

	m_flagctrl = new xui_toggle(xui_vector<s32>(16), TOGGLE_NORMAL);
	xui_method_ptrcall(m_flagctrl, set_parent		)(this);
	xui_method_ptrcall(m_flagctrl, set_corner		)(3);
	xui_method_ptrcall(m_flagctrl, set_drawcolor	)(true);
	xui_method_ptrcall(m_flagctrl, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_flagctrl, ini_component	)(0, ALIGNVERT_C, 0);

	m_namectrl = new xui_textbox(xui_vector<s32>(128, 18));
	xui_method_ptrcall(m_namectrl, xm_nonfocus		) += new xui_method_member<xui_method_args, onity_compattr>(this, &onity_compattr::on_ctrlnonfocus);
	xui_method_ptrcall(m_namectrl, xm_getfocus		) += new xui_method_member<xui_method_args, onity_compattr>(this, &onity_compattr::on_ctrlgetfocus);
	xui_method_ptrcall(m_namectrl, xm_textenter		) += new xui_method_member<xui_method_args, onity_compattr>(this, &onity_compattr::on_namectrltextenter);
	xui_method_ptrcall(m_namectrl, set_parent		)(this);
	xui_method_ptrcall(m_namectrl, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_namectrl, set_drawcolor	)(true);
	xui_method_ptrcall(m_namectrl, set_borderrt		)(xui_rect2d<s32>(4, 2, 0, 2));
	xui_method_ptrcall(m_namectrl, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_namectrl, ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_namectrl, ini_drawer		)(xui_global::ascii_to_unicode(name));

	u32 type = -1;
	if		(attr->isBool())	type = 0;
	else if (attr->isNumeric()) type = 1;
	else if (attr->isString())	type = 2;
	else
	{}

	m_typectrl = new xui_dropbox(xui_vector<s32>(64, 18), NULL);
	xui_method_ptrcall(m_typectrl, xm_nonfocus		) += new xui_method_member<xui_method_args, onity_compattr>(this, &onity_compattr::on_ctrlnonfocus);
	xui_method_ptrcall(m_typectrl, xm_getfocus		) += new xui_method_member<xui_method_args, onity_compattr>(this, &onity_compattr::on_ctrlgetfocus);
	xui_method_ptrcall(m_typectrl, xm_selection		) += new xui_method_member<xui_method_args, onity_compattr>(this, &onity_compattr::on_typectrlselection);
	xui_method_ptrcall(m_typectrl, set_parent		)(this);
	xui_method_ptrcall(m_typectrl, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_typectrl, set_drawcolor	)(true);
	xui_method_ptrcall(m_typectrl, set_borderrt		)(xui_rect2d<s32>(4, 2, 0, 2));
	xui_method_ptrcall(m_typectrl, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_typectrl, set_corner		)(3);
	xui_method_ptrcall(m_typectrl, set_readonly		)(true);
	xui_method_ptrcall(m_typectrl, add_item			)(L"Bool");
	xui_method_ptrcall(m_typectrl, add_item			)(L"Number");
	xui_method_ptrcall(m_typectrl, add_item			)(L"String");
	xui_method_ptrcall(m_typectrl, ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_typectrl, ini_dropbox		)(type);

	m_boolctrl = new xui_dropbox(xui_vector<s32>(64, 18), NULL);
	xui_method_ptrcall(m_boolctrl, xm_nonfocus		) += new xui_method_member<xui_method_args, onity_compattr>(this, &onity_compattr::on_ctrlnonfocus);
	xui_method_ptrcall(m_boolctrl, xm_getfocus		) += new xui_method_member<xui_method_args, onity_compattr>(this, &onity_compattr::on_ctrlgetfocus);
	xui_method_ptrcall(m_boolctrl, xm_selection		) += new xui_method_member<xui_method_args, onity_compattr>(this, &onity_compattr::on_boolctrlselection);
	xui_method_ptrcall(m_boolctrl, set_parent		)(this);
	xui_method_ptrcall(m_boolctrl, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_boolctrl, set_drawcolor	)(true);
	xui_method_ptrcall(m_boolctrl, set_borderrt		)(xui_rect2d<s32>(4, 2, 0, 2));
	xui_method_ptrcall(m_boolctrl, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_boolctrl, set_corner		)(3);
	xui_method_ptrcall(m_boolctrl, set_readonly		)(true);
	xui_method_ptrcall(m_boolctrl, add_item			)(L"True");
	xui_method_ptrcall(m_boolctrl, add_item			)(L"False");
	xui_method_ptrcall(m_boolctrl, ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_boolctrl, ini_component	)(true, attr->isBool());
	xui_method_ptrcall(m_boolctrl, ini_dropbox		)(attr->asBool() ? 0 : 1);

	std::wstringstream temp;
	if (attr->isNumeric())
		temp << attr->asDouble();

	m_numbctrl	= new xui_textbox(xui_vector<s32>(32, 18));
	xui_method_ptrcall(m_numbctrl, xm_nonfocus		) += new xui_method_member<xui_method_args, onity_compattr>(this, &onity_compattr::on_ctrlnonfocus);
	xui_method_ptrcall(m_numbctrl, xm_getfocus		) += new xui_method_member<xui_method_args, onity_compattr>(this, &onity_compattr::on_ctrlgetfocus);
	xui_method_ptrcall(m_numbctrl, xm_textchanged	) += new xui_method_member<xui_method_args, onity_compattr>(this, &onity_compattr::on_numbctrltextchanged);
	xui_method_ptrcall(m_numbctrl, set_parent		)(this);
	xui_method_ptrcall(m_numbctrl, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_numbctrl, set_drawcolor	)(true);
	xui_method_ptrcall(m_numbctrl, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_numbctrl, set_borderrt		)(xui_rect2d<s32>(4, 2, 2, 2));
	xui_method_ptrcall(m_numbctrl, set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_numbctrl, set_numbtype		)(NT_FLOAT);
	xui_method_ptrcall(m_numbctrl, ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_numbctrl, ini_component	)(true, attr->isNumeric());
	xui_method_ptrcall(m_numbctrl, ini_drawer		)(temp.str());

	m_textctrl	= new xui_textbox(xui_vector<s32>(32, 18));
	xui_method_ptrcall(m_textctrl, xm_nonfocus		) += new xui_method_member<xui_method_args, onity_compattr>(this, &onity_compattr::on_ctrlnonfocus);
	xui_method_ptrcall(m_textctrl, xm_getfocus		) += new xui_method_member<xui_method_args, onity_compattr>(this, &onity_compattr::on_ctrlgetfocus);
	xui_method_ptrcall(m_textctrl, xm_textchanged	) += new xui_method_member<xui_method_args, onity_compattr>(this, &onity_compattr::on_textctrltextchanged);
	xui_method_ptrcall(m_textctrl, set_parent		)(this);
	xui_method_ptrcall(m_textctrl, set_backcolor	)(xui_colour::darkgray);
	xui_method_ptrcall(m_textctrl, set_drawcolor	)(true);
	xui_method_ptrcall(m_textctrl, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_textctrl, set_borderrt		)(xui_rect2d<s32>(4, 2, 2, 2));
	xui_method_ptrcall(m_textctrl, set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_textctrl, ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_textctrl, ini_component	)(true, attr->isString());
	xui_method_ptrcall(m_textctrl, ini_drawer		)(attr->isString() ? xui_global::ascii_to_unicode(attr->asString()) : L"");

	m_widgetvec.push_back(m_flagctrl);
	m_widgetvec.push_back(m_namectrl);
	m_widgetvec.push_back(m_typectrl);
	m_widgetvec.push_back(m_boolctrl);
	m_widgetvec.push_back(m_numbctrl);
	m_widgetvec.push_back(m_textctrl);
}

/*
//method
*/
xui_method_explain(onity_compattr,			was_selected,			bool				)( void ) const
{
	return m_flagctrl->was_push();
}
xui_method_explain(onity_compattr,			get_attrname,			const std::string&	)( void ) const
{
	return m_attrname;
}

/*
//callback
*/
xui_method_explain(onity_compattr,			on_perform,				void				)( xui_method_args& args )
{
	xui_control::on_perform(args);

	xui_rect2d<s32> rt = get_renderrtins();
	s32 x =  rt.ax;
	s32 w = (rt.get_w()-20) / 2;

	//flag
	m_flagctrl->on_perform_x(x);
	//name
	x += 20;
	m_namectrl->on_perform_x(x);
	m_namectrl->on_perform_w(w);

	//type
	x += w+4;
	w = (w-8) / 2;
	m_typectrl->on_perform_x(x);
	m_typectrl->on_perform_w(w);

	x += w+4;
	m_boolctrl->on_perform_x(x);
	m_boolctrl->on_perform_w(w);
	m_numbctrl->on_perform_x(x);
	m_numbctrl->on_perform_w(w);
	m_textctrl->on_perform_x(x);
	m_textctrl->on_perform_w(w);
}

/*
//event
*/
xui_method_explain(onity_compattr,			on_ctrlnonfocus,		void				)( xui_component* sender, xui_method_args& args )
{
	xui_control* ctrl = xui_dynamic_cast(xui_control, sender);
	ctrl->set_sidecolor(xui_control::default_sidecolor);
}
xui_method_explain(onity_compattr,			on_ctrlgetfocus,		void				)( xui_component* sender, xui_method_args& args )
{
	xui_control* ctrl = xui_dynamic_cast(xui_control, sender);
	ctrl->set_sidecolor(xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f));
}
xui_method_explain(onity_compattr,			on_namectrltextenter,	void				)( xui_component* sender, xui_method_args& args )
{
	BreezeGame::Json::Value* node = m_compkind->get_node();
	std::string name = xui_global::unicode_to_ascii(m_namectrl->get_text());
	if (name.length() > 0 && node->isMember(name) == false)
	{
		(*node)[name] = (*node)[m_attrname];
		node->removeMember(m_attrname);
		m_attrname = name;

		xui_method_propdata other_args;
		m_compkind->xm_propchanged(NULL, other_args);
	}
	else
	{
		m_namectrl->ini_drawer(xui_global::ascii_to_unicode(m_attrname));
	}
}
xui_method_explain(onity_compattr,			on_typectrlselection,	void				)( xui_component* sender, xui_method_args& args )
{
	m_boolctrl->set_visible(false);
	m_numbctrl->set_visible(false);
	m_textctrl->set_visible(false);

	BreezeGame::Json::Value* node = m_compkind->get_node();
	u32 selectedindex = m_typectrl->get_selectedindex();
	switch (selectedindex)
	{
	case 0:
		{
			(*node)[m_attrname] = BreezeGame::Json::Value(true);
			xui_method_ptrcall(m_boolctrl, ini_dropbox	)(0);
			xui_method_ptrcall(m_boolctrl, set_visible	)(true);
		}
		break;
	case 1:
		{
			(*node)[m_attrname] = BreezeGame::Json::Value((double)0);
			xui_method_ptrcall(m_numbctrl, ini_drawer	)(L"0");
			xui_method_ptrcall(m_numbctrl, set_visible	)(true);
		}
		break;
	case 2:
		{
			(*node)[m_attrname] = BreezeGame::Json::Value("");
			xui_method_ptrcall(m_textctrl, ini_drawer	)(L"");
			xui_method_ptrcall(m_textctrl, set_visible	)(true);
		}
		break;
	}

	xui_method_propdata other_args;
	m_compkind->xm_propchanged(NULL, other_args);
}
xui_method_explain(onity_compattr,			on_boolctrlselection,	void				)( xui_component* sender, xui_method_args& args )
{
	u32 selectedindex = m_boolctrl->get_selectedindex();
	BreezeGame::Json::Value* node = m_compkind->get_node();
	(*node)[m_attrname] = BreezeGame::Json::Value(selectedindex == 0);

	xui_method_propdata other_args;
	m_compkind->xm_propchanged(NULL, other_args);
}
xui_method_explain(onity_compattr,			on_numbctrltextchanged,	void				)( xui_component* sender, xui_method_args& args )
{
	std::wstringstream temp(m_numbctrl->get_text());
	double  value;
	temp >> value;

	BreezeGame::Json::Value* node = m_compkind->get_node();
	(*node)[m_attrname] = BreezeGame::Json::Value(value);

	xui_method_propdata other_args;
	m_compkind->xm_propchanged(NULL, other_args);
}
xui_method_explain(onity_compattr,			on_textctrltextchanged,	void				)( xui_component* sender, xui_method_args& args )
{
	BreezeGame::Json::Value* node = m_compkind->get_node();
	(*node)[m_attrname] = BreezeGame::Json::Value(xui_global::unicode_to_ascii(m_textctrl->get_text()));

	xui_method_propdata other_args;
	m_compkind->xm_propchanged(NULL, other_args);
}


//////////////////////////////////////////////////////////////////////////
//propctrl
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(onity_propctrl_compattr, xui_propctrl);

/*
//constructor
*/
xui_create_explain(onity_propctrl_compattr)( void )
: xui_propctrl()
{
	m_border = xui_rect2d<s32>(0, 8, 0, 8);
	m_header = new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_header,	set_parent			)(this);
	xui_method_ptrcall(m_header,	set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_header,	set_borderrt		)(xui_rect2d<s32>(8, 2, 2, 2));
	xui_method_ptrcall(m_header,	ini_drawer			)(L"Attributes");
	m_nontip = new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_nontip,	set_parent			)(this);
	xui_method_ptrcall(m_nontip,	set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_nontip,	set_borderrt		)(xui_rect2d<s32>(8, 2, 2, 2));
	xui_method_ptrcall(m_nontip,	ini_component		)(true, false);
	xui_method_ptrcall(m_nontip,	ini_drawer			)(L"Empty");

	m_insert = new xui_button(xui_vector<s32>(24, 16));
	xui_method_ptrcall(m_insert,	set_parent			)(this);
	xui_method_ptrcall(m_insert,	set_backcolor		)(xui_colour::white);
	xui_method_ptrcall(m_insert,	set_movecolor		)(xui_button::default_downcolor);
	xui_method_ptrcall(m_insert,	xm_buttonclick		) += new xui_method_member<xui_method_args, onity_propctrl_compattr>(this, &onity_propctrl_compattr::on_insertclick);
	xui_method_ptrcall(m_insert,	xm_renderself		) += new xui_method_member<xui_method_args, onity_propctrl_compattr>(this, &onity_propctrl_compattr::on_insertrenderself);

	m_delete = new xui_button(xui_vector<s32>(24, 16));
	xui_method_ptrcall(m_delete,	set_parent			)(this);
	xui_method_ptrcall(m_delete,	set_backcolor		)(xui_colour::white);
	xui_method_ptrcall(m_delete,	set_movecolor		)(xui_button::default_downcolor);
	xui_method_ptrcall(m_delete,	xm_buttonclick		) += new xui_method_member<xui_method_args, onity_propctrl_compattr>(this, &onity_propctrl_compattr::on_deleteclick);
	xui_method_ptrcall(m_delete,	xm_renderself		) += new xui_method_member<xui_method_args, onity_propctrl_compattr>(this, &onity_propctrl_compattr::on_deleterenderself);

	m_middle = new xui_panel(xui_vector<s32>(0));
	xui_method_ptrcall(m_middle,	set_parent			)(this);
	xui_method_ptrcall(m_middle,	set_drawcolor		)(false);
	xui_method_ptrcall(m_middle,	set_sidestyle		)(SIDESTYLE_N);
	xui_method_ptrcall(m_middle,	set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_middle,	set_hscrollauto		)(false);
	xui_method_ptrcall(m_middle,	xm_invalid			) += new xui_method_member<xui_method_args, onity_propctrl_compattr>(this, &onity_propctrl_compattr::on_middleinvalid);
	xui_method_ptrcall(m_middle,	xm_perform			) += new xui_method_member<xui_method_args, onity_propctrl_compattr>(this, &onity_propctrl_compattr::on_middleperform);

	m_widgetvec.push_back(m_header);
	m_widgetvec.push_back(m_insert);
	m_widgetvec.push_back(m_delete);
	m_widgetvec.push_back(m_middle);
	m_widgetvec.push_back(m_nontip);
}

/*
//static
*/
xui_method_explain(onity_propctrl_compattr, create,					xui_propctrl*		)( xui_propdata* propdata )
{
	return new onity_propctrl_compattr;
}

/*
//override
*/
xui_method_explain(onity_propctrl_compattr, on_linkpropdata,		void				)( void )
{
	onity_propkind_entitycomp* compkind = dynamic_cast<onity_propkind_entitycomp*>(m_propdata->get_kind());
	BreezeGame::Json::Value*   node     = compkind->get_node();
	std::vector<std::string>   vec      = node->getMemberNames();

	xui_method_ptrcall(m_middle, del_children	)();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		if (vec[i] == "ClassName" ||
			vec[i] == "Family")
			continue;

		m_middle->add_child(new onity_compattr(compkind, vec[i]));
	}
	xui_method_ptrcall(m_middle, refresh		)();
	xui_method_ptrcall(m_insert, set_enable		)(true);
	xui_method_ptrcall(m_delete, set_enable		)(m_middle->get_childcount() >  0);
	xui_method_ptrcall(m_nontip, set_visible	)(m_middle->get_childcount() == 0);
}
xui_method_explain(onity_propctrl_compattr, on_editvalue,			void				)( xui_propedit* sender )
{}

/*
//callback
*/
xui_method_explain(onity_propctrl_compattr, on_renderback,			void				)( xui_method_args& args )
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
xui_method_explain(onity_propctrl_compattr, on_invalid,				void				)( xui_method_args& args )
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
xui_method_explain(onity_propctrl_compattr, on_perform,				void				)( xui_method_args& args )
{
	xui_propctrl::on_perform(args);

	s32 indent = get_indent();
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt = rt.get_pt();
	//header
	m_header->on_perform_x (pt.x+indent);
	m_header->on_perform_y (pt.y);
	m_header->on_perform_w (rt.get_w()-indent);
	//listview
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

/*
//event
*/
xui_method_explain(onity_propctrl_compattr, on_middleinvalid,		void				)( xui_component* sender, xui_method_args& args )
{
	s32 h = (m_middle->get_childcount() == 0) ? m_nontip->get_renderh() : (m_middle->get_childcount() * onity_compattr::default_height);
	h += m_middle->get_borderrt().ay;
	h += m_middle->get_borderrt().by;
	m_middle->set_renderh(h);
}
xui_method_explain(onity_propctrl_compattr, on_middleperform,		void				)( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt = m_middle->get_renderrtins();
	xui_rect2d<s32> bd = m_middle->get_borderrt();
	xui_vector<s32> pt = bd.get_pt();

	const std::vector<xui_control*>& vec = m_middle->get_children();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		vec[i]->on_perform_pt(pt);
		vec[i]->on_perform_w (rt.get_w());
		pt.y += onity_compattr::default_height;
	}
}
xui_method_explain(onity_propctrl_compattr, on_deleteclick,			void				)( xui_component* sender, xui_method_args& args )
{
	onity_propkind_entitycomp* compkind = dynamic_cast<onity_propkind_entitycomp*>(m_propdata->get_kind());
	BreezeGame::Json::Value*   node     = compkind->get_node();

	bool modify = false;
	std::vector<xui_control*> vec = m_middle->get_children();
	for (u32 i = 0; i < vec.size(); ++i)
	{
		onity_compattr* ctrl = xui_dynamic_cast(onity_compattr, vec[i]);
		if (ctrl->was_selected())
		{
			node->removeMember(ctrl->get_attrname());
			m_middle->del_child(ctrl);
			modify = true;
		}
	}

	if (modify == false)
		return;

	xui_method_propdata other_args;
	compkind->xm_propchanged(NULL, other_args);
	xui_method_ptrcall(m_middle, refresh		)();
	xui_method_ptrcall(m_delete, set_enable		)(m_middle->get_childcount() >  0);
	xui_method_ptrcall(m_nontip, set_visible	)(m_middle->get_childcount() == 0);
}
xui_method_explain(onity_propctrl_compattr, on_deleterenderself,	void				)( xui_component* sender, xui_method_args& args )
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
xui_method_explain(onity_propctrl_compattr, on_insertclick,			void				)( xui_component* sender, xui_method_args& args )
{
	onity_propkind_entitycomp* compkind = dynamic_cast<onity_propkind_entitycomp*>(m_propdata->get_kind());
	BreezeGame::Json::Value*   node     = compkind->get_node();

	std::stringstream temp;
	temp << "New Attribute";
	if (node->isMember(temp.str()))
	{
		s32 number  = 0;
		while (true)
		{
			temp.str("");
			temp << L"New Attribute";
			temp << number;
			if (node->isMember(temp.str()) == false)
				break;

			++number;
		}
	}

	(*node)[temp.str()] = BreezeGame::Json::Value("");
	xui_method_propdata other_args;
	compkind->xm_propchanged(NULL, other_args);

	xui_method_ptrcall(m_middle, add_child		)(new onity_compattr(compkind, temp.str()));
	xui_method_ptrcall(m_middle, refresh		)();
	xui_method_ptrcall(m_delete, set_enable		)(true);
	xui_method_ptrcall(m_nontip, set_visible	)(false);
}
xui_method_explain(onity_propctrl_compattr, on_insertrenderself,	void				)( xui_component* sender, xui_method_args& args )
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