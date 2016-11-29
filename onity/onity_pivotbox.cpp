#include "xui_convas.h"
#include "xui_desktop.h"
#include "xui_drawer.h"
#include "xui_panel.h"
#include "xui_propdata.h"
#include "onity_resource.h"
#include "onity_propedit.h"
#include "onity_scene.h"
#include "onity_mainform.h"
#include "onity_pivotbox.h"

extern s32 xui_round( f64 value );

/*
//constructor
*/
xui_create_explain(onity_pivotbox)( xui_panel* owner )
: m_owner(owner)
, m_style(PIVOTSTYLE_NONE)
{
	m_bound = new xui_drawer(xui_vector<s32>(100));
	xui_method_ptrcall(m_bound, set_drawcolor	)(true);
	xui_method_ptrcall(m_bound,	set_backcolor	)(xui_colour(0.5f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f));
	xui_method_ptrcall(m_bound, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_bound, xm_setrenderpt	) += new xui_method_member<xui_method_args,  onity_pivotbox>(this, &onity_pivotbox::on_boundsetrenderrt);
	xui_method_ptrcall(m_bound, xm_setrendersz	) += new xui_method_member<xui_method_args,  onity_pivotbox>(this, &onity_pivotbox::on_boundsetrenderrt);
	xui_method_ptrcall(m_bound, xm_renderself	) += new xui_method_member<xui_method_args,  onity_pivotbox>(this, &onity_pivotbox::on_boundrenderself);

	m_pivot = new xui_drawer(xui_vector<s32>(32));
	xui_method_ptrcall(m_pivot,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_pivot,	ini_drawer		)(onity_resource::icon_bigpivot);
	xui_method_ptrcall(m_pivot,	xm_setrenderpt	) += new xui_method_member<xui_method_args,  onity_pivotbox>(this, &onity_pivotbox::on_pivotsetrenderpt);
	xui_method_ptrcall(m_pivot,	xm_mousemove	) += new xui_method_member<xui_method_mouse, onity_pivotbox>(this, &onity_pivotbox::on_pivotmousemove);
	xui_method_ptrcall(m_pivot,	xm_mouserise	) += new xui_method_member<xui_method_mouse, onity_pivotbox>(this, &onity_pivotbox::on_pivotmouserise);

	m_lsize = new xui_drawer(xui_vector<s32>(32));
	xui_method_ptrcall(m_lsize,	set_maskcolor	)(xui_colour::red);
	xui_method_ptrcall(m_lsize,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_lsize,	ini_drawer		)(onity_resource::icon_biglarrow);
	xui_method_ptrcall(m_lsize,	xm_mousemove	) += new xui_method_member<xui_method_mouse, onity_pivotbox>(this, &onity_pivotbox::on_arrowmousemove);
	xui_method_ptrcall(m_lsize, xm_mouserise	) += new xui_method_member<xui_method_mouse, onity_pivotbox>(this, &onity_pivotbox::on_arrowmouserise);
	m_rsize = new xui_drawer(xui_vector<s32>(32));
	xui_method_ptrcall(m_rsize,	set_maskcolor	)(xui_colour::red);
	xui_method_ptrcall(m_rsize,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_rsize,	ini_drawer		)(onity_resource::icon_bigrarrow);
	xui_method_ptrcall(m_rsize, xm_mousemove	) += new xui_method_member<xui_method_mouse, onity_pivotbox>(this, &onity_pivotbox::on_arrowmousemove);
	xui_method_ptrcall(m_rsize, xm_mouserise	) += new xui_method_member<xui_method_mouse, onity_pivotbox>(this, &onity_pivotbox::on_arrowmouserise);
	m_tsize = new xui_drawer(xui_vector<s32>(32));
	xui_method_ptrcall(m_tsize, set_maskcolor	)(xui_colour::red);
	xui_method_ptrcall(m_tsize,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_tsize,	ini_drawer		)(onity_resource::icon_bigtarrow);
	xui_method_ptrcall(m_tsize, xm_mousemove	) += new xui_method_member<xui_method_mouse, onity_pivotbox>(this, &onity_pivotbox::on_arrowmousemove);
	xui_method_ptrcall(m_tsize, xm_mouserise	) += new xui_method_member<xui_method_mouse, onity_pivotbox>(this, &onity_pivotbox::on_arrowmouserise);
	m_bsize = new xui_drawer(xui_vector<s32>(32));
	xui_method_ptrcall(m_bsize, set_maskcolor	)(xui_colour::red);
	xui_method_ptrcall(m_bsize,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_bsize,	ini_drawer		)(onity_resource::icon_bigbarrow);
	xui_method_ptrcall(m_bsize, xm_mousemove	) += new xui_method_member<xui_method_mouse, onity_pivotbox>(this, &onity_pivotbox::on_arrowmousemove);
	xui_method_ptrcall(m_bsize, xm_mouserise	) += new xui_method_member<xui_method_mouse, onity_pivotbox>(this, &onity_pivotbox::on_arrowmouserise);

	xui_method_ptrcall(m_owner,	add_child		)(m_bound);
	xui_method_ptrcall(m_owner,	add_child		)(m_pivot);
	xui_method_ptrcall(m_owner,	add_child		)(m_lsize);
	xui_method_ptrcall(m_owner,	add_child		)(m_rsize);
	xui_method_ptrcall(m_owner,	add_child		)(m_tsize);
	xui_method_ptrcall(m_owner,	add_child		)(m_bsize);
	set_visible(false);
}

/*
//method
*/
xui_method_explain(onity_pivotbox, ori_pivot,			xui_vector<s32>	)( void )
{
	onity_scene* scene = onity_mainform::get_ptr()->get_scene();
	xui_vector<s32> trans = scene->get_trans();
	f64				ratio = scene->get_ratio();

	xui_vector<s32> pt = m_pivot->get_renderpt();
	xui_vector<s32> sz = m_pivot->get_rendersz();
	s32 x = xui_round((pt.x+sz.w/2)/ratio) - trans.x;
	s32 y = xui_round((pt.y+sz.h/2)/ratio) - trans.y;
	return xui_vector<s32>(x, y);
}
xui_method_explain(onity_pivotbox, set_visible,			void			)( bool flag )
{
	m_bound->set_visible(flag);
	m_pivot->set_visible(flag);
	m_lsize->set_visible(flag);
	m_rsize->set_visible(flag);
	m_tsize->set_visible(flag);
	m_bsize->set_visible(flag);
	set_rectupdate();
}
xui_method_explain(onity_pivotbox, set_rectupdate,		void			)( bool update_pivot )
{
	if (m_bound->was_visible())
	{
		onity_scene* scene = onity_mainform::get_ptr()->get_scene();
		xui_vector<s32> trans = scene->get_trans();
		f64             ratio = scene->get_ratio();

		xui_rect2d<s32> rt;
		xui_proproot_vec propvec = onity_scene::get_selectedprop();
		for (u32 i = 0; i < propvec.size(); ++i)
		{
			onity_propedit* prop = dynamic_cast<onity_propedit*>(propvec[i]);
			if (prop)
			{
				if (rt.was_valid() == false)
					rt = prop->get_bounding(trans, ratio);
				else
					rt = rt.get_union(prop->get_bounding(trans, ratio));
			}
		}

		rt.ax -= m_pivot->get_renderw()/2;
		rt.bx += m_pivot->get_renderw()/2;
		rt.ay -= m_pivot->get_renderh()/2;
		rt.by += m_pivot->get_renderh()/2;
		m_bound->set_renderpt(rt.get_pt());
		m_bound->set_rendersz(rt.get_sz());

		if (update_pivot)
		{
			m_pivot->set_renderpt(cal_pivotstyle(rt));
		}

		xui_method_args args;
		on_pivotsetrenderpt(m_pivot, args);
	}
}

/*
//event
*/
xui_method_explain(onity_pivotbox, on_boundrenderself,	void			)( xui_component* sender, xui_method_args&  args )
{
	if (m_pivot->has_catch())
	{
		xui_colour   color = xui_colour(0.5f, 1.0f, 0.0f, 0.0f);
		xui_bitmap*  image = onity_resource::icon_bigpivot;
		xui_rect2d<s32> rt = m_bound->get_renderrtabs();
		xui_vector<s32> pt;
		pt.x = rt.ax;
		pt.y = rt.ay + rt.get_h()/2 - m_pivot->get_renderh()/2;
		xui_convas::get_ins()->draw_image(image, pt, color);
		pt.x = rt.bx - m_pivot->get_renderw();
		xui_convas::get_ins()->draw_image(image, pt, color);
		pt.x = rt.ax + rt.get_w()/2 - m_pivot->get_renderw()/2;
		pt.y = rt.ay;
		xui_convas::get_ins()->draw_image(image, pt, color);
		pt.y = rt.by - m_pivot->get_renderh();
		xui_convas::get_ins()->draw_image(image, pt, color);
		pt.x = rt.ax + rt.get_w()/2 - m_pivot->get_renderw()/2;
		pt.y = rt.ay + rt.get_h()/2 - m_pivot->get_renderh()/2;
		xui_convas::get_ins()->draw_image(image, pt, color);
	}

	if (m_lsize->has_catch() ||
		m_rsize->has_catch() ||
		m_tsize->has_catch() ||
		m_bsize->has_catch())
	{
		xui_rect2d<s32> rt = m_bound->get_renderrtabs();
		rt.ax += m_pivot->get_renderw()/2;
		rt.ay += m_pivot->get_renderh()/2;
		rt.bx -= m_pivot->get_renderw()/2;
		rt.by -= m_pivot->get_renderh()/2;
		xui_convas::get_ins()->draw_rectangle(rt, xui_colour::red);
	}
}
xui_method_explain(onity_pivotbox, on_boundsetrenderrt,	void			)( xui_component* sender, xui_method_args&  args )
{
	xui_rect2d<s32> rt = m_bound->get_renderrt()+m_bound->get_renderpt();
	xui_vector<s32> pt;
	pt.x = rt.ax - m_lsize->get_renderw();
	pt.y = rt.ay + rt.get_h()/2 - m_lsize->get_renderh()/2;
	m_lsize->on_perform_pt(pt);
	pt.x = rt.bx;
	m_rsize->on_perform_pt(pt);
	pt.x = rt.ax + rt.get_w()/2 - m_rsize->get_renderw()/2;
	pt.y = rt.ay - m_tsize->get_renderh();
	m_tsize->on_perform_pt(pt);
	pt.y = rt.by;
	m_bsize->on_perform_pt(pt);
}
xui_method_explain(onity_pivotbox, on_pivotsetrenderpt, void			)( xui_component* sender, xui_method_args&  args )
{
	xui_rect2d<s32> rt = m_bound->get_renderrt()+m_bound->get_renderpt();
	xui_vector<s32> pt = m_pivot->get_renderpt();
	xui_vector<s32> sz = m_pivot->get_rendersz();
	m_lsize->set_visible(pt.x > rt.ax);
	m_rsize->set_visible(pt.x < rt.bx-sz.w);
	m_tsize->set_visible(pt.y > rt.ay);
	m_bsize->set_visible(pt.y < rt.by-sz.h);
}
xui_method_explain(onity_pivotbox, on_pivotmousemove,	void			)( xui_component* sender, xui_method_mouse& args )
{
	if (m_pivot->has_catch())
	{
		xui_rect2d<s32> rt = m_bound->get_renderrt() + m_bound->get_renderpt();
		xui_vector<s32> pt = m_pivot->get_renderpt() + xui_desktop::get_ins()->get_mousemove();
		m_pivot->set_renderpt(get_pivotlimit(rt, pt));
	}
}
xui_method_explain(onity_pivotbox, on_pivotmouserise,	void			)( xui_component* sender, xui_method_mouse& args )
{
	if (m_pivot->has_catch())
	{
		xui_rect2d<s32> rt = m_bound->get_renderrt() + m_bound->get_renderpt();
		xui_vector<s32> pt = m_pivot->get_renderpt();
		xui_vector<s32> sz = m_pivot->get_rendersz();

		s32 x = pt.x - rt.ax - rt.get_w()/2 + sz.w/2;
		s32 y = pt.y - rt.ay - rt.get_h()/2 + sz.h/2;
		m_style = PIVOTSTYLE_NONE;
		if (xui_abs(x) <= 5 && xui_abs(y) <= 5)
		{
			m_style = PIVOTSTYLE_CC;
		}
		else
		if (xui_abs(y) <= 5)
		{
			if (pt.x <= rt.ax+5)
				m_style = PIVOTSTYLE_LC;
			if (pt.x >= rt.bx-sz.w-5)
				m_style = PIVOTSTYLE_RC;
		}
		else
		if (xui_abs(x) <= 5)
		{
			if (pt.y <= rt.ay+5)
				m_style = PIVOTSTYLE_TC;
			if (pt.y >= rt.by-sz.h-5)
				m_style = PIVOTSTYLE_BC;
		}

		m_pivot->set_renderpt(cal_pivotstyle(rt));
	}
}
xui_method_explain(onity_pivotbox, on_arrowmousemove,	void			)( xui_component* sender, xui_method_mouse& args )
{
	if (sender->has_catch())
	{
		onity_scene* scene = onity_mainform::get_ptr()->get_scene();
		xui_vector<s32> trans = scene->get_trans();
		f64             ratio = scene->get_ratio();

		xui_rect2d<s32> oribox;
		xui_proproot_vec propvec = onity_scene::get_selectedprop();
		for (u32 i = 0; i < propvec.size(); ++i)
		{
			onity_propedit* prop = dynamic_cast<onity_propedit*>(propvec[i]);
			if (prop)
			{
				if (oribox.was_valid() == false)
					oribox = prop->get_bounding(trans, ratio);
				else
					oribox = oribox.get_union(prop->get_bounding(trans, ratio));
			}
		}

		xui_vector<s32> pt = m_pivot->get_renderpt();
		xui_vector<s32> sz = m_pivot->get_rendersz();
		xui_rect2d<s32> rt = m_bound->get_renderrt() + m_bound->get_renderpt();
		xui_vector<s32> move = xui_desktop::get_ins()->get_mousemove();
		xui_vector<s32> half = xui_vector<s32>(sz.w/2, sz.h/2);
		xui_vector<s32> midd = xui_vector<s32>(pt.x+half.w, pt.y+half.h);

		if (sender == m_lsize)
		{
			rt.ax += move.x;
			rt.ax  = xui_min(rt.ax, pt.x);
			if (m_rsize->was_visible())
			{
				f64 ratio = (f64)(pt.x-rt.ax) / (f64)(midd.x-oribox.ax);
				f64 scale = (oribox.bx-midd.x) * ratio;
				rt.bx = midd.x + (s32)xui_pixel_align(scale) + half.w;
			}
		}
		else
		if (sender == m_rsize)
		{
			rt.bx += move.x;
			rt.bx  = xui_max(rt.bx, pt.x+sz.w);
			if (m_lsize->was_visible())
			{
				f64 ratio = (f64)(rt.bx-pt.x-sz.w) / (f64)(oribox.bx-midd.x);
				f64 scale = (midd.x-oribox.ax) * ratio;
				rt.ax = midd.x - (s32)xui_pixel_align(scale) - half.w;
			}
		}
		else
		if (sender == m_tsize)
		{
			rt.ay += move.y;
			rt.ay  = xui_min(rt.ay, pt.y);
			if (m_bsize->was_visible())
			{
				f64 ratio = (f64)(pt.y-rt.ay) / (f64)(midd.y-oribox.ay);
				rt.by = midd.y + (s32)((oribox.by-midd.y) * ratio) + half.h;
			}
		}
		else
		if (sender == m_bsize)
		{
			rt.by += move.y;
			rt.by  = xui_max(rt.by, pt.y+sz.h);
			if (m_tsize->was_visible())
			{
				f64 ratio = (f64)(rt.by-pt.y-sz.h) / (f64)(oribox.by-midd.y);
				rt.ay = midd.y - (s32)((midd.y-oribox.ay) * ratio) - half.h;
			}
		}

		m_bound->set_renderpt(rt.get_pt());
		m_bound->set_rendersz(rt.get_sz());
	}
}
xui_method_explain(onity_pivotbox, on_arrowmouserise,	void			)( xui_component* sender, xui_method_mouse& args )
{
	if (sender->has_catch())
	{
		onity_scene* scene = onity_mainform::get_ptr()->get_scene();
		xui_vector<s32> trans = scene->get_trans();
		f64             ratio = scene->get_ratio();

		xui_rect2d<s32> oribox;
		xui_proproot_vec propvec = onity_scene::get_selectedprop();
		for (u32 i = 0; i < propvec.size(); ++i)
		{
			onity_propedit* prop = dynamic_cast<onity_propedit*>(propvec[i]);
			if (prop)
			{
				if (oribox.was_valid() == false)
					oribox = prop->get_bounding(trans, ratio);
				else
					oribox = oribox.get_union(prop->get_bounding(trans, ratio));
			}
		}

		xui_vector<s32> pt = m_pivot->get_renderpt();
		xui_vector<s32> sz = m_pivot->get_rendersz();
		xui_rect2d<s32> rt = m_bound->get_renderrt() + m_bound->get_renderpt();
		xui_vector<s32> half = sz / 2;
		xui_vector<s32> midd = pt + half;

		xui_vector<s32> pivot = ori_pivot();
		xui_vector<f64> scale = (rt.get_sz()-sz).to<f64>() / oribox.get_sz().to<f64>();
		for (u32 i = 0; i < propvec.size(); ++i)
		{
			onity_propedit* prop = dynamic_cast<onity_propedit*>(propvec[i]);
			if (prop)
			{
				//xui_rect2d<s32> orirt = prop->ori_bounding();
				xui_vector<s32> oript = prop->ori_position();
				//xui_vector<s32> orimd = orirt.get_pt() + orirt.get_sz()/2;
				//xui_vector<s32> oriof = oript - orimd;
				xui_vector<s32> final =  oript;
				xui_vector<f64> delta = (oript-pivot).to<f64>() * scale;
				if (sender == m_lsize || sender == m_rsize)
					final.x = pivot.x + (s32)xui_pixel_align(delta.x);
				if (sender == m_tsize || sender == m_bsize)
					final.y = pivot.y + (s32)xui_pixel_align(delta.y);

				prop->set_position(final);
			}
		}

		set_rectupdate(false);
	}
}

/*
//method
*/
xui_method_explain(onity_pivotbox, get_pivotlimit,		xui_vector<s32>	)( const xui_rect2d<s32>& rt, const xui_vector<s32>& pt )
{
	xui_vector<s32> result = pt;
	result.x = xui_max(result.x, rt.ax);
	result.x = xui_min(result.x, rt.bx-m_pivot->get_renderw());
	result.y = xui_max(result.y, rt.ay);
	result.y = xui_min(result.y, rt.by-m_pivot->get_renderh());
	return result;
}
xui_method_explain(onity_pivotbox, cal_pivotstyle,		xui_vector<s32>	)( const xui_rect2d<s32>& rt )
{
	xui_vector<s32> pt = m_pivot->get_renderpt();
	xui_vector<s32> sz = m_pivot->get_rendersz();
	switch (m_style)
	{
	case PIVOTSTYLE_LC:
	case PIVOTSTYLE_RC:
	case PIVOTSTYLE_CC:
		pt.y = rt.ay + rt.get_h()/2 - m_pivot->get_renderh()/2;
		break;
	case PIVOTSTYLE_TC:
		pt.y = rt.ay;
		break;
	case PIVOTSTYLE_BC:
		pt.y = rt.by - sz.h;
		break;
	}

	switch (m_style)
	{
	case PIVOTSTYLE_TC:
	case PIVOTSTYLE_BC:
	case PIVOTSTYLE_CC:
		pt.x = rt.ax + rt.get_w()/2 - m_pivot->get_renderw()/2;
		break;
	case PIVOTSTYLE_LC:
		pt.x = rt.ax;
		break;
	case PIVOTSTYLE_RC:
		pt.x = rt.bx - sz.w;
		break;
	}

	return get_pivotlimit(rt, pt);
}