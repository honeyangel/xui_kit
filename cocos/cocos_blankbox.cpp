#include "xui_linebox.h"
#include "xui_button.h"
#include "cocos_resource.h"
#include "cocos_boundbox.h"
#include "cocos_blankbox.h"

/*
//constructor
*/
xui_create_explain(cocos_blankbox)( const xui_vector<s32>& button_size, get_func func )
: m_func(func)
{
	m_horzinc	= new xui_button(button_size);
	xui_method_ptrcall(m_horzinc,	ini_drawer		)(cocos_resource::icon_horzinc);
	xui_method_ptrcall(m_horzinc,	set_drawcolor	)(true);
	xui_method_ptrcall(m_horzinc,	set_enable		)(false);
	xui_method_ptrcall(m_horzinc,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_horzinc,	xm_buttonclick	) += new xui_method_member<xui_method_args,	cocos_blankbox>(this, &cocos_blankbox::on_horzbuttonclick);
	m_horzdec	= new xui_button(button_size);
	xui_method_ptrcall(m_horzdec,	ini_drawer		)(cocos_resource::icon_horzdec);
	xui_method_ptrcall(m_horzdec,	set_drawcolor	)(true);
	xui_method_ptrcall(m_horzdec,	set_enable		)(false);
	xui_method_ptrcall(m_horzdec,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_horzdec,	xm_buttonclick	) += new xui_method_member<xui_method_args,	cocos_blankbox>(this, &cocos_blankbox::on_horzbuttonclick);
	m_horzcancel= new xui_button(button_size);
	xui_method_ptrcall(m_horzcancel,ini_drawer		)(cocos_resource::icon_horzcancel);
	xui_method_ptrcall(m_horzcancel,set_drawcolor	)(true);
	xui_method_ptrcall(m_horzcancel,set_enable		)(false);
	xui_method_ptrcall(m_horzcancel,set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_horzcancel,xm_buttonclick	) += new xui_method_member<xui_method_args,	cocos_blankbox>(this, &cocos_blankbox::on_horzbuttonclick);
	m_horzequal	= new xui_button(button_size);
	xui_method_ptrcall(m_horzequal,	ini_drawer		)(cocos_resource::icon_horzequal);
	xui_method_ptrcall(m_horzequal,	set_drawcolor	)(true);
	xui_method_ptrcall(m_horzequal,	set_enable		)(false);
	xui_method_ptrcall(m_horzequal,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_horzequal,	xm_buttonclick	) += new xui_method_member<xui_method_args,	cocos_blankbox>(this, &cocos_blankbox::on_horzbuttonclick);

	m_vertinc	= new xui_button(button_size);
	xui_method_ptrcall(m_vertinc,	ini_drawer		)(cocos_resource::icon_vertinc);
	xui_method_ptrcall(m_vertinc,	set_drawcolor	)(true);
	xui_method_ptrcall(m_vertinc,	set_enable		)(false);
	xui_method_ptrcall(m_vertinc,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_vertinc,	xm_buttonclick	) += new xui_method_member<xui_method_args,	cocos_blankbox>(this, &cocos_blankbox::on_vertbuttonclick);
	m_vertdec	= new xui_button(button_size);
	xui_method_ptrcall(m_vertdec,	ini_drawer		)(cocos_resource::icon_vertdec);
	xui_method_ptrcall(m_vertdec,	set_drawcolor	)(true);
	xui_method_ptrcall(m_vertdec,	set_enable		)(false);
	xui_method_ptrcall(m_vertdec,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_vertdec,	xm_buttonclick	) += new xui_method_member<xui_method_args,	cocos_blankbox>(this, &cocos_blankbox::on_vertbuttonclick);
	m_vertcancel= new xui_button(button_size);
	xui_method_ptrcall(m_vertcancel,ini_drawer		)(cocos_resource::icon_vertcancel);
	xui_method_ptrcall(m_vertcancel,set_drawcolor	)(true);
	xui_method_ptrcall(m_vertcancel,set_enable		)(false);
	xui_method_ptrcall(m_vertcancel,set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_vertcancel,xm_buttonclick	) += new xui_method_member<xui_method_args, cocos_blankbox>(this, &cocos_blankbox::on_vertbuttonclick);
	m_vertequal = new xui_button(button_size);
	xui_method_ptrcall(m_vertequal,	ini_drawer		)(cocos_resource::icon_vertequal);
	xui_method_ptrcall(m_vertequal,	set_drawcolor	)(true);
	xui_method_ptrcall(m_vertequal,	set_enable		)(false);
	xui_method_ptrcall(m_vertequal,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_vertequal,	xm_buttonclick	) += new xui_method_member<xui_method_args, cocos_blankbox>(this, &cocos_blankbox::on_vertbuttonclick);

	m_horzline = new xui_linebox(button_size);
	xui_method_ptrcall(m_horzline,	set_corner		)(3);
	xui_method_ptrcall(m_horzline,	add_linectrl	)(m_horzinc);
	xui_method_ptrcall(m_horzline,	add_linectrl	)(m_horzdec);
	xui_method_ptrcall(m_horzline,	add_linectrl	)(m_horzcancel);
	xui_method_ptrcall(m_horzline,	add_linectrl	)(m_horzequal);
	xui_method_ptrcall(m_horzline,	refresh			)();
	m_vertline = new xui_linebox(button_size);
	xui_method_ptrcall(m_vertline,	set_corner		)(3);
	xui_method_ptrcall(m_vertline,	add_linectrl	)(m_vertinc);
	xui_method_ptrcall(m_vertline,	add_linectrl	)(m_vertdec);
	xui_method_ptrcall(m_vertline,	add_linectrl	)(m_vertcancel);
	xui_method_ptrcall(m_vertline,	add_linectrl	)(m_vertequal);
	xui_method_ptrcall(m_vertline,	refresh			)();
}

/*
//method
*/
xui_method_explain(cocos_blankbox, get_horzline,		xui_linebox*)( void )
{
	return m_horzline;
}
xui_method_explain(cocos_blankbox, get_vertline,		xui_linebox*)( void )
{
	return m_vertline;
}
xui_method_explain(cocos_blankbox, set_lineupdate,		void		)( void )
{
	std::vector<cocos_boundbox*> vec = (*m_func)();
	xui_method_ptrcall(m_horzinc,	set_enable)(vec.size() > 2);
	xui_method_ptrcall(m_horzdec,	set_enable)(vec.size() > 2);
	xui_method_ptrcall(m_horzcancel,set_enable)(vec.size() > 2);
	xui_method_ptrcall(m_horzequal,	set_enable)(vec.size() > 2);
	xui_method_ptrcall(m_vertinc,	set_enable)(vec.size() > 2);
	xui_method_ptrcall(m_vertdec,	set_enable)(vec.size() > 2);
	xui_method_ptrcall(m_vertcancel,set_enable)(vec.size() > 2);
	xui_method_ptrcall(m_vertequal,	set_enable)(vec.size() > 2);
}

/*
//event
*/
xui_method_explain(cocos_blankbox, on_horzbuttonclick,	void		)( xui_component* sender, xui_method_args& args )
{
	std::vector<cocos_boundbox*> vec = (*m_func)();
	if (vec.size() > 2)
	{
		std::vector<s32>	steparray;
		s32					stepequal	= 0;

		for (u32 i = 1; i < vec.size(); ++i)
		{
			xui_rect2d<s32> prevrect	= vec[i-1]->ori_bounding();
			xui_rect2d<s32> nextrect	= vec[i  ]->ori_bounding();

			if (sender == m_horzinc ||
				sender == m_horzdec ||
				sender == m_horzequal)
			{
				stepequal += (nextrect.ax-prevrect.ax-prevrect.get_w());
				if (sender == m_horzinc)
					steparray.push_back(nextrect.ax-prevrect.ax-prevrect.get_w()+1);
				if (sender == m_horzdec)
					steparray.push_back(nextrect.ax-prevrect.ax-prevrect.get_w()-1);
			}
		}

		if		(sender == m_horzcancel)	stepequal = 0;
		else if (sender == m_horzequal)		stepequal = stepequal / ((s32)vec.size()-1);
		else
		{}

		for (u32 i = 1; i < vec.size(); ++i)
		{
			xui_rect2d<s32> prevrect	= vec[i-1]->ori_bounding();
			xui_rect2d<s32> nextrect	= vec[i  ]->ori_bounding();

			s32 delta  =  0;
			if (sender == m_horzinc ||
				sender == m_horzdec)
				delta  =  prevrect.bx - nextrect.ax + steparray[i-1] ;
			else
				delta  =  prevrect.bx - nextrect.ax + stepequal;

			vec[i]->set_position(vec[i]->ori_position()+xui_vector<s32>(delta, 0));
		}
	}
}
xui_method_explain(cocos_blankbox, on_vertbuttonclick,	void		)( xui_component* sender, xui_method_args& args )
{
	std::vector<cocos_boundbox*> vec = (*m_func)();
	if (vec.size() > 2)
	{
		std::vector<s32>	steparray;
		s32					stepequal	= 0;

		for (u32 i = 1; i < vec.size(); ++i)
		{
			xui_rect2d<s32> prevrect	= vec[i-1]->ori_bounding();
			xui_rect2d<s32> nextrect	= vec[i  ]->ori_bounding();

			if (sender == m_vertinc ||
				sender == m_vertdec ||
				sender == m_vertequal)
			{
				stepequal += (nextrect.by-prevrect.by+prevrect.get_h());
				if (sender == m_vertinc)
					steparray.push_back(nextrect.by-prevrect.by+prevrect.get_h()-1);
				if (sender == m_vertdec)
					steparray.push_back(nextrect.by-prevrect.by+prevrect.get_h()+1);
			}
		}

		if		(sender == m_vertcancel)	stepequal = 0;
		else if (sender == m_vertequal)		stepequal = stepequal / ((s32)vec.size()-1);
		else
		{}

		for (u32 i = 1; i < vec.size(); ++i)
		{
			xui_rect2d<s32> prevrect	= vec[i-1]->ori_bounding();
			xui_rect2d<s32> nextrect	= vec[i  ]->ori_bounding();

			s32 delta  =  0;
			if (sender == m_vertinc ||
				sender == m_vertdec)
				delta  =  prevrect.ay - nextrect.by + steparray[i-1] ;
			else
				delta  =  prevrect.ay - nextrect.by + stepequal;

			vec[i]->set_position(vec[i]->ori_position()+xui_vector<s32>(0, delta));
		}
	}
}
