#include "xui_component.h"
#include "xui_convas.h"
#include "onity_boundbox.h"

extern s32 xui_round( f64 value );

/*
//constructor
*/
xui_create_explain(onity_boundbox)( xui_proproot* prop, u08 type )
: m_linkprop(prop)
, m_sizetype(type)
, m_bounding(xui_rect2d<s32>(0))
{}

/*
//position
*/
xui_method_explain(onity_boundbox, get_position,	xui_vector<s32>	)( const xui_vector<s32>& trans, f64 ratio )
{
	xui_vector<s32>  pt = ori_position();
	pt.x = xui_round(pt.x*ratio + trans.x*ratio);
	pt.y = xui_round(pt.y*ratio + trans.y*ratio);

	return pt;
}
xui_method_explain(onity_boundbox, ori_position,	xui_vector<s32>	)( void )
{
	return xui_vector<s32>(0);
}
xui_method_explain(onity_boundbox, set_position,	void			)( const xui_vector<s32>& pos )
{}

/*
//bounding
*/
xui_method_explain(onity_boundbox, get_bounding,	xui_rect2d<s32>	)( const xui_vector<s32>& trans, f64 ratio )
{
	xui_rect2d<s32> rt = ori_bounding();
	rt.ax = xui_round(rt.ax*ratio + trans.x*ratio);
	rt.ay = xui_round(rt.ay*ratio + trans.y*ratio);
	rt.bx = xui_round(rt.bx*ratio + trans.x*ratio);
	rt.by = xui_round(rt.by*ratio + trans.y*ratio);

	return rt;
}
xui_method_explain(onity_boundbox, ori_bounding,	xui_rect2d<s32>	)( void )
{
	return xui_rect2d<s32>(0);
}
xui_method_explain(onity_boundbox, opt_bounding,	void			)( const xui_vector<s32>& trans, f64 ratio, const xui_vector<s32>& delta, u08 op )
{
	if (op != BO_NONE)
	{
		switch (op)
		{
		case BO_MOVE:
			m_bounding.ax += delta.x;
			m_bounding.bx += delta.x;
			break;
		case BO_SIZE_L:
		case BO_SIZE_LT:
		case BO_SIZE_LB:
			m_bounding.ax += delta.x;
			break;
		case BO_SIZE_R:
		case BO_SIZE_RT:
		case BO_SIZE_RB:
			m_bounding.bx += delta.x;
			break;
		}

		switch (op)
		{
		case BO_MOVE:
			m_bounding.ay += delta.y;
			m_bounding.by += delta.y;
			break;
		case BO_SIZE_T:
		case BO_SIZE_LT:
		case BO_SIZE_RT:
			m_bounding.ay += delta.y;
			break;
		case BO_SIZE_B:
		case BO_SIZE_LB:
		case BO_SIZE_RB:
			m_bounding.by += delta.y;
			break;
		}

		xui_rect2d<s32> rt;
		rt.ax = xui_round(m_bounding.ax/ratio - trans.x);
		rt.bx = xui_round(m_bounding.bx/ratio - trans.x);
		rt.ay = xui_round(m_bounding.ay/ratio - trans.y);
		rt.by = xui_round(m_bounding.by/ratio - trans.y);
		set_bounding(rt, op);
	}
}
xui_method_explain(onity_boundbox, set_bounding,	void			)( const xui_rect2d<s32>& rt, u08 op )
{
	if (op == BO_MOVE)
	{
		xui_vector<s32> delta = rt.get_pt() - ori_bounding().get_pt();
		xui_vector<s32> pt = ori_position();
		set_position(pt+delta);
	}
}

/*
//method
*/
xui_method_explain(onity_boundbox, get_linkprop,	xui_proproot*	)( void )
{
	return m_linkprop;
}
xui_method_explain(onity_boundbox, syn_bounding,	void 			)( const xui_vector<s32>& trans, f64 ratio )
{
	m_bounding = get_bounding(trans, ratio);
}
xui_method_explain(onity_boundbox, hit_operator,	u08				)( const xui_vector<s32>& trans, f64 ratio, const xui_vector<s32>& pt )
{
	xui_rect2d<s32> rt = get_bounding(trans, ratio);
	if (m_sizetype != ST_NONE)
	{
		s32 hx = rt.ax + rt.get_w()/2;
		s32 hy = rt.ay + rt.get_h()/2;
		if (xui_abs(pt.x-hx) <= 4)
		{
			if (xui_abs(pt.y-rt.ay) <= 4)
				return BO_SIZE_T;
			if (xui_abs(pt.y-rt.by) <= 4)
				return BO_SIZE_B;
		}
		if (xui_abs(pt.y-hy) <= 4)
		{
			if (xui_abs(pt.x-rt.ax) <= 4)
				return BO_SIZE_L;
			if (xui_abs(pt.x-rt.bx) <= 4)
				return BO_SIZE_R;
		}

		if (m_sizetype == ST_ENTIRE)
		{
			if (xui_abs(pt.x-rt.ax) <= 4)
			{
				if (xui_abs(pt.y-rt.ay) <= 4)
					return BO_SIZE_LT;
				if (xui_abs(pt.y-rt.by) <= 4)
					return BO_SIZE_LB;
			}
			if (xui_abs(pt.x-rt.bx) <= 4)
			{
				if (xui_abs(pt.y-rt.ay) <= 4)
					return BO_SIZE_RT;
				if (xui_abs(pt.y-rt.by) <= 4)
					return BO_SIZE_RB;
			}
		}
	}

	return rt.was_inside(pt) ? BO_MOVE : BO_NONE;
}
xui_method_explain(onity_boundbox, get_opcursor,	u32				)( u08 op )
{
	switch (op)
	{
	case BO_MOVE:	
		return CURSOR_HAND;
	case BO_SIZE_L:
	case BO_SIZE_R:
		return CURSOR_WE;
	case BO_SIZE_T:
	case BO_SIZE_B:
		return CURSOR_NS;
	case BO_SIZE_LT:
	case BO_SIZE_RB:
		return CURSOR_NWSE;
	case BO_SIZE_RT:
	case BO_SIZE_LB:
		return CURSOR_NESW;
	default:
		return CURSOR_DEFAULT;
	}
}
xui_method_explain(onity_boundbox, was_selected,	bool			)( void )
{
	return false;
}

/*
//draw
*/
xui_method_explain(onity_boundbox, draw,			void			)( const xui_vector<s32>& trans, f64 ratio, const xui_vector<s32>& pt, bool alwaysdraw )
{
	xui_rect2d<s32> rect  = get_bounding(trans, ratio) + pt;
	xui_vector<s32> pivot = get_position(trans, ratio) + pt;
	bool selected = was_selected();
	if (selected || alwaysdraw)
	{
		draw_bounding(rect, pivot);
		if (selected)
			draw_operator(rect, pivot);
	}
}
xui_method_explain(onity_boundbox, draw_bounding,	void			)( const xui_rect2d<s32>& rt, const xui_vector<s32>& pivot )
{
	xui_convas::get_ins()->draw_rectangle(rt, xui_colour(1.0f, 0.7f));
}
xui_method_explain(onity_boundbox, draw_operator,	void			)( const xui_rect2d<s32>& rt, const xui_vector<s32>& pivot )
{
	if (m_sizetype != ST_NONE)
	{
		s32 hx = rt.ax + rt.get_w()/2;
		s32 hy = rt.ay + rt.get_h()/2;
		xui_rect2d<s32> op_lc(rt.ax-4, hy-4, rt.ax+4, hy+4);
		xui_convas::get_ins()->fill_rectangle(op_lc, xui_colour::white);
		xui_convas::get_ins()->draw_rectangle(op_lc, xui_colour::black);
		xui_rect2d<s32> op_rc(rt.bx-4, hy-4, rt.bx+4, hy+4);
		xui_convas::get_ins()->fill_rectangle(op_rc, xui_colour::white);
		xui_convas::get_ins()->draw_rectangle(op_rc, xui_colour::black);
		xui_rect2d<s32> op_tc(hx-4, rt.ay-4, hx+4, rt.ay+4);
		xui_convas::get_ins()->fill_rectangle(op_tc, xui_colour::white);
		xui_convas::get_ins()->draw_rectangle(op_tc, xui_colour::black);
		xui_rect2d<s32> op_bc(hx-4, rt.by-4, hx+4, rt.by+4);
		xui_convas::get_ins()->fill_rectangle(op_bc, xui_colour::white);
		xui_convas::get_ins()->draw_rectangle(op_bc, xui_colour::black);

		if (m_sizetype == ST_ENTIRE)
		{
			xui_rect2d<s32> op_lt(rt.ax-4, rt.ay-4, rt.ax+4, rt.ay+4);
			xui_convas::get_ins()->fill_rectangle(op_lt, xui_colour::white);
			xui_convas::get_ins()->draw_rectangle(op_lt, xui_colour::black);
			xui_rect2d<s32> op_rt(rt.bx-4, rt.ay-4, rt.bx+4, rt.ay+4);
			xui_convas::get_ins()->fill_rectangle(op_rt, xui_colour::white);
			xui_convas::get_ins()->draw_rectangle(op_rt, xui_colour::black);
			xui_rect2d<s32> op_lb(rt.ax-4, rt.by-4, rt.ax+4, rt.by+4);
			xui_convas::get_ins()->fill_rectangle(op_lb, xui_colour::white);
			xui_convas::get_ins()->draw_rectangle(op_lb, xui_colour::black);
			xui_rect2d<s32> op_rb(rt.bx-4, rt.by-4, rt.bx+4, rt.by+4);
			xui_convas::get_ins()->fill_rectangle(op_rb, xui_colour::white);
			xui_convas::get_ins()->draw_rectangle(op_rb, xui_colour::black);
		}
	}
}