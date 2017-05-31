#include "xui_linebox.h"
#include "xui_button.h"
#include "onity_resource.h"
#include "onity_bounding.h"
#include "onity_alignbox.h"

/*
//constructor
*/
xui_create_explain(onity_alignbox)( const xui_vector<s32>& button_size, get_func func )
: m_func(func)
{
	m_left		= new xui_button(button_size);
	xui_method_ptrcall(m_left,		ini_drawer		)(onity_resource::icon_horzleft);
	xui_method_ptrcall(m_left,		set_drawcolor	)(true);
	xui_method_ptrcall(m_left,		set_enable		)(false);
	xui_method_ptrcall(m_left,		set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_left,		xm_buttonclick	) += new xui_method_member<xui_method_args,	onity_alignbox>(this, &onity_alignbox::on_horzbuttonclick);
	m_right		= new xui_button(button_size);
	xui_method_ptrcall(m_right,		ini_drawer		)(onity_resource::icon_horzright);
	xui_method_ptrcall(m_right,		set_drawcolor	)(true);
	xui_method_ptrcall(m_right,		set_enable		)(false);
	xui_method_ptrcall(m_right,		set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_right,		xm_buttonclick	) += new xui_method_member<xui_method_args,	onity_alignbox>(this, &onity_alignbox::on_horzbuttonclick);
	m_hcenter	= new xui_button(button_size);
	xui_method_ptrcall(m_hcenter,	ini_drawer		)(onity_resource::icon_horzcenter);
	xui_method_ptrcall(m_hcenter,	set_drawcolor	)(true);
	xui_method_ptrcall(m_hcenter,	set_enable		)(false);
	xui_method_ptrcall(m_hcenter,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_hcenter,	xm_buttonclick	) += new xui_method_member<xui_method_args, onity_alignbox>(this, &onity_alignbox::on_horzbuttonclick);

	m_top		= new xui_button(button_size);
	xui_method_ptrcall(m_top,		ini_drawer		)(onity_resource::icon_verttop);
	xui_method_ptrcall(m_top,		set_drawcolor	)(true);
	xui_method_ptrcall(m_top,		set_enable		)(false);
	xui_method_ptrcall(m_top,		set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_top,		xm_buttonclick	) += new xui_method_member<xui_method_args,	onity_alignbox>(this, &onity_alignbox::on_vertbuttonclick);
	m_bottom	= new xui_button(button_size);
	xui_method_ptrcall(m_bottom,	ini_drawer		)(onity_resource::icon_vertbottom);
	xui_method_ptrcall(m_bottom,	set_drawcolor	)(true);
	xui_method_ptrcall(m_bottom,	set_enable		)(false);
	xui_method_ptrcall(m_bottom,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_bottom,	xm_buttonclick	) += new xui_method_member<xui_method_args,	onity_alignbox>(this, &onity_alignbox::on_vertbuttonclick);
	m_vcenter	= new xui_button(button_size);
	xui_method_ptrcall(m_vcenter,	ini_drawer		)(onity_resource::icon_vertcenter);
	xui_method_ptrcall(m_vcenter,	set_drawcolor	)(true);
	xui_method_ptrcall(m_vcenter,	set_enable		)(false);
	xui_method_ptrcall(m_vcenter,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_vcenter,	xm_buttonclick	) += new xui_method_member<xui_method_args, onity_alignbox>(this, &onity_alignbox::on_vertbuttonclick);

	m_horzline = new xui_linebox(button_size);
	xui_method_ptrcall(m_horzline,	set_corner		)(3);
	xui_method_ptrcall(m_horzline,	add_linectrl	)(m_left);
	xui_method_ptrcall(m_horzline,	add_linectrl	)(m_hcenter);
	xui_method_ptrcall(m_horzline,	add_linectrl	)(m_right);
	xui_method_ptrcall(m_horzline,	refresh			)();
	m_vertline = new xui_linebox(button_size);
	xui_method_ptrcall(m_vertline,	set_corner		)(3);
	xui_method_ptrcall(m_vertline,	add_linectrl	)(m_top);
	xui_method_ptrcall(m_vertline,	add_linectrl	)(m_vcenter);
	xui_method_ptrcall(m_vertline,	add_linectrl	)(m_bottom);
	xui_method_ptrcall(m_vertline,	refresh			)();
}

/*
//method
*/
xui_method_explain(onity_alignbox, get_horzline,		xui_linebox*)( void )
{
	return m_horzline;
}
xui_method_explain(onity_alignbox, get_vertline,		xui_linebox*)( void )
{
	return m_vertline;
}
xui_method_explain(onity_alignbox, set_lineupdate,		void		)( void )
{
	std::vector<onity_bounding*> vec = (*m_func)();
	xui_method_ptrcall(m_left,		set_enable)(vec.size() > 1);
	xui_method_ptrcall(m_hcenter,	set_enable)(vec.size() > 1);
	xui_method_ptrcall(m_right,		set_enable)(vec.size() > 1);
	xui_method_ptrcall(m_top,		set_enable)(vec.size() > 1);
	xui_method_ptrcall(m_vcenter,	set_enable)(vec.size() > 1);
	xui_method_ptrcall(m_bottom,	set_enable)(vec.size() > 1);
}

/*
//event
*/
xui_method_explain(onity_alignbox, on_horzbuttonclick,	void		)( xui_component* sender, xui_method_args& args )
{
	std::vector<onity_bounding*> vec = (*m_func)();
	if (vec.size() > 1)
	{
		xui_rect2d<s32> headrect = vec.front()->ori_bounding();
		for (u32 i = 1; i < vec.size(); ++i)
		{
			xui_rect2d<s32> rect = vec[i]->ori_bounding();
			s32 delta = 0;
			if		(sender == m_left)		delta = headrect.ax - rect.ax; 
			else if (sender == m_hcenter)	delta = headrect.ax - rect.ax + headrect.get_w()/2 - rect.get_w()/2;
			else if (sender == m_right)		delta = headrect.bx - rect.bx;
			else
			{}

			vec[i]->set_position(vec[i]->ori_position()+xui_vector<s32>(delta, 0));
		}
	}
}
xui_method_explain(onity_alignbox, on_vertbuttonclick,	void		)( xui_component* sender, xui_method_args& args )
{
	std::vector<onity_bounding*> vec = (*m_func)();
	if (vec.size() > 1)
	{
		xui_rect2d<s32> headrect = vec.front()->ori_bounding();
		for (u32 i = 1; i < vec.size(); ++i)
		{
			xui_rect2d<s32> rect = vec[i]->ori_bounding();
			s32 delta = 0;
			if		(sender == m_top)		delta = headrect.ay - rect.ay; 
			else if (sender == m_vcenter)	delta = headrect.ay - rect.ay + headrect.get_h()/2 - rect.get_h()/2;
			else if (sender == m_bottom)	delta = headrect.by - rect.by;
			else
			{}

			vec[i]->set_position(vec[i]->ori_position()+xui_vector<s32>(0, delta));
		}
	}
}