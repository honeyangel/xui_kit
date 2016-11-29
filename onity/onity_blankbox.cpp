#include "xui_linebox.h"
#include "xui_button.h"
#include "onity_resource.h"
#include "onity_propedit.h"
#include "onity_blankbox.h"

/*
//constructor
*/
xui_create_explain(onity_blankbox)( const xui_vector<s32>& button_size, get_func propfunc )
: m_propfunc(propfunc)
{
	m_horzinc	= new xui_button(button_size);
	xui_method_ptrcall(m_horzinc,	ini_drawer		)(onity_resource::icon_horzinc);
	xui_method_ptrcall(m_horzinc,	set_drawcolor	)(true);
	xui_method_ptrcall(m_horzinc,	set_enable		)(false);
	xui_method_ptrcall(m_horzinc,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_horzinc,	xm_buttonclick	) += new xui_method_member<xui_method_args,	onity_blankbox>(this, &onity_blankbox::on_horzbuttonclick);
	m_horzdec	= new xui_button(button_size);
	xui_method_ptrcall(m_horzdec,	ini_drawer		)(onity_resource::icon_horzdec);
	xui_method_ptrcall(m_horzdec,	set_drawcolor	)(true);
	xui_method_ptrcall(m_horzdec,	set_enable		)(false);
	xui_method_ptrcall(m_horzdec,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_horzdec,	xm_buttonclick	) += new xui_method_member<xui_method_args,	onity_blankbox>(this, &onity_blankbox::on_horzbuttonclick);
	m_horzcancel= new xui_button(button_size);
	xui_method_ptrcall(m_horzcancel,ini_drawer		)(onity_resource::icon_horzcancel);
	xui_method_ptrcall(m_horzcancel,set_drawcolor	)(true);
	xui_method_ptrcall(m_horzcancel,set_enable		)(false);
	xui_method_ptrcall(m_horzcancel,set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_horzcancel,xm_buttonclick	) += new xui_method_member<xui_method_args,	onity_blankbox>(this, &onity_blankbox::on_horzbuttonclick);
	m_horzequal	= new xui_button(button_size);
	xui_method_ptrcall(m_horzequal,	ini_drawer		)(onity_resource::icon_horzequal);
	xui_method_ptrcall(m_horzequal,	set_drawcolor	)(true);
	xui_method_ptrcall(m_horzequal,	set_enable		)(false);
	xui_method_ptrcall(m_horzequal,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_horzequal,	xm_buttonclick	) += new xui_method_member<xui_method_args,	onity_blankbox>(this, &onity_blankbox::on_horzbuttonclick);

	m_vertinc	= new xui_button(button_size);
	xui_method_ptrcall(m_vertinc,	ini_drawer		)(onity_resource::icon_vertinc);
	xui_method_ptrcall(m_vertinc,	set_drawcolor	)(true);
	xui_method_ptrcall(m_vertinc,	set_enable		)(false);
	xui_method_ptrcall(m_vertinc,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_vertinc,	xm_buttonclick	) += new xui_method_member<xui_method_args,	onity_blankbox>(this, &onity_blankbox::on_vertbuttonclick);
	m_vertdec	= new xui_button(button_size);
	xui_method_ptrcall(m_vertdec,	ini_drawer		)(onity_resource::icon_vertdec);
	xui_method_ptrcall(m_vertdec,	set_drawcolor	)(true);
	xui_method_ptrcall(m_vertdec,	set_enable		)(false);
	xui_method_ptrcall(m_vertdec,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_vertdec,	xm_buttonclick	) += new xui_method_member<xui_method_args,	onity_blankbox>(this, &onity_blankbox::on_vertbuttonclick);
	m_vertcancel= new xui_button(button_size);
	xui_method_ptrcall(m_vertcancel,ini_drawer		)(onity_resource::icon_vertcancel);
	xui_method_ptrcall(m_vertcancel,set_drawcolor	)(true);
	xui_method_ptrcall(m_vertcancel,set_enable		)(false);
	xui_method_ptrcall(m_vertcancel,set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_vertcancel,xm_buttonclick	) += new xui_method_member<xui_method_args, onity_blankbox>(this, &onity_blankbox::on_vertbuttonclick);
	m_vertequal = new xui_button(button_size);
	xui_method_ptrcall(m_vertequal,	ini_drawer		)(onity_resource::icon_vertequal);
	xui_method_ptrcall(m_vertequal,	set_drawcolor	)(true);
	xui_method_ptrcall(m_vertequal,	set_enable		)(false);
	xui_method_ptrcall(m_vertequal,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_vertequal,	xm_buttonclick	) += new xui_method_member<xui_method_args, onity_blankbox>(this, &onity_blankbox::on_vertbuttonclick);

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
xui_method_explain(onity_blankbox, get_horzline,		xui_linebox*)( void )
{
	return m_horzline;
}
xui_method_explain(onity_blankbox, get_vertline,		xui_linebox*)( void )
{
	return m_vertline;
}
xui_method_explain(onity_blankbox, set_lineupdate,		void		)( void )
{
	xui_proproot_vec propvec = (*m_propfunc)();
	xui_method_ptrcall(m_horzinc,	set_enable)(propvec.size() > 2);
	xui_method_ptrcall(m_horzdec,	set_enable)(propvec.size() > 2);
	xui_method_ptrcall(m_horzcancel,set_enable)(propvec.size() > 2);
	xui_method_ptrcall(m_horzequal,	set_enable)(propvec.size() > 2);
	xui_method_ptrcall(m_vertinc,	set_enable)(propvec.size() > 2);
	xui_method_ptrcall(m_vertdec,	set_enable)(propvec.size() > 2);
	xui_method_ptrcall(m_vertcancel,set_enable)(propvec.size() > 2);
	xui_method_ptrcall(m_vertequal,	set_enable)(propvec.size() > 2);
}

/*
//event
*/
xui_method_explain(onity_blankbox, on_horzbuttonclick,	void		)( xui_component* sender, xui_method_args& args )
{
	xui_proproot_vec propvec = (*m_propfunc)();
	if (propvec.size() > 2)
	{
		std::vector<s32>	steparray;
		s32					stepequal	= 0;

		for (u32 i = 1; i < propvec.size(); ++i)
		{
			onity_propedit* prevprop	= dynamic_cast<onity_propedit*>(propvec[i-1]);
			onity_propedit* nextprop	= dynamic_cast<onity_propedit*>(propvec[i]);
			xui_rect2d<s32> prevrect	= prevprop->ori_bounding();
			xui_rect2d<s32> nextrect	= nextprop->ori_bounding();

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
		else if (sender == m_horzequal)		stepequal = stepequal / (propvec.size()-1);
		else
		{}

		for (u32 i = 1; i < propvec.size(); ++i)
		{
			onity_propedit* prevprop	= dynamic_cast<onity_propedit*>(propvec[i-1]);
			onity_propedit* nextprop	= dynamic_cast<onity_propedit*>(propvec[i]);
			xui_rect2d<s32> prevrect	= prevprop->ori_bounding();
			xui_rect2d<s32> nextrect	= nextprop->ori_bounding();

			s32 delta  =  0;
			if (sender == m_horzinc ||
				sender == m_horzdec)
				delta  =  prevrect.bx - nextrect.ax + steparray[i-1] ;
			else
				delta  =  prevrect.bx - nextrect.ax + stepequal;

			nextprop->set_position(nextprop->ori_position()+xui_vector<s32>(delta, 0));
		}
	}
}
xui_method_explain(onity_blankbox, on_vertbuttonclick,	void		)( xui_component* sender, xui_method_args& args )
{
	xui_proproot_vec propvec = (*m_propfunc)();
	if (propvec.size() > 2)
	{
		std::vector<s32>	steparray;
		s32					stepequal	= 0;

		for (u32 i = 1; i < propvec.size(); ++i)
		{
			onity_propedit* prevprop	= dynamic_cast<onity_propedit*>(propvec[i-1]);
			onity_propedit* nextprop	= dynamic_cast<onity_propedit*>(propvec[i]);
			xui_rect2d<s32> prevrect	= prevprop->ori_bounding();
			xui_rect2d<s32> nextrect	= nextprop->ori_bounding();

			if (sender == m_vertinc ||
				sender == m_vertdec ||
				sender == m_vertequal)
			{
				stepequal += (nextrect.ay-prevrect.ay-prevrect.get_h());
				if (sender == m_vertinc)
					steparray.push_back(nextrect.ay-prevrect.ay-prevrect.get_h()+1);
				if (sender == m_vertdec)
					steparray.push_back(nextrect.ay-prevrect.ay-prevrect.get_h()-1);
			}
		}

		if		(sender == m_vertcancel)	stepequal = 0;
		else if (sender == m_vertequal)		stepequal = stepequal / (propvec.size()-1);
		else
		{}

		for (u32 i = 1; i < propvec.size(); ++i)
		{
			onity_propedit* prevprop	= dynamic_cast<onity_propedit*>(propvec[i-1]);
			onity_propedit* nextprop	= dynamic_cast<onity_propedit*>(propvec[i]);
			xui_rect2d<s32> prevrect	= prevprop->ori_bounding();
			xui_rect2d<s32> nextrect	= nextprop->ori_bounding();

			s32 delta  =  0;
			if (sender == m_vertinc ||
				sender == m_vertdec)
				delta  =  prevrect.by - nextrect.ay + steparray[i-1] ;
			else
				delta  =  prevrect.by - nextrect.ay + stepequal;

			nextprop->set_position(nextprop->ori_position()+xui_vector<s32>(0, delta));
		}
	}
}
