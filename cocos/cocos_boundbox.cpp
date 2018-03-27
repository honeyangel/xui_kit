#include "2d/CCNode.h"
#include "ui/UILayoutComponent.h"
#include "math/CCAffineTransform.h"

#include "xui_convas.h"
#include "xui_treenode.h"
#include "xui_treedata.h"
#include "cocos_mainform.h"
#include "cocos_hierarchy.h"
#include "cocos_propcsd.h"
#include "cocos_propnodebase.h"
#include "cocos_boundbox.h"

extern s32 xui_round( f64 value );

/*
//constructor
*/
xui_create_explain(cocos_boundbox)( cocos_propnodebase* prop )
: m_linkprop(prop)
, m_operator(OP_NONE)
, m_ignoresz(false)
, m_bounding(xui_rect2d<s32>(0))
, m_pivotbox(xui_vector<s32>(0))
{}

/*
//position
*/
xui_method_explain(cocos_boundbox, get_pivotbox,	xui_vector<s32>		)( const xui_vector<s32>& trans, f64 ratio, s32 viewh )
{
	cocos2d::Vec2 anchor = m_ignoresz ? cocos2d::Vec2(0.5f, 0.5f) : m_linkprop->get_node()->getAnchorPoint();
	xui_rect2d<s32> rt = get_bounding(trans, ratio, viewh);
	xui_vector<s32> pt;
	pt.x = rt.ax + rt.get_w()*anchor.x;
	pt.y = rt.by - rt.get_h()*anchor.y;
	return pt;
}
xui_method_explain(cocos_boundbox, get_position,	xui_vector<s32>		)( const xui_vector<s32>& trans, f64 ratio, s32 viewh )
{
	xui_vector<s32>  pt = ori_position();
	pt.x = xui_round(pt.x*ratio) + trans.x;
	pt.y = viewh - (xui_round(pt.y*ratio) + trans.y);

	return pt;
}
xui_method_explain(cocos_boundbox, ori_position,	xui_vector<s32>		)( void )
{
	cocos2d::Vec2 pos = m_linkprop->get_node()->getPosition();
	return xui_vector<s32>(pos.x, pos.y);
}
xui_method_explain(cocos_boundbox, set_position,	void				)( const xui_vector<s32>& pos )
{
	//cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	cocos2d::ui::LayoutComponent* component = cocos2d::ui::LayoutComponent::bindLayoutComponent(m_linkprop->get_node());
	component->setPosition(cocos2d::Point(pos.x, pos.y));
	component->refreshLayout();

	//m_linkprop->get_node()->setPosition(pos.x, pos.y);
	m_linkprop->get_file()->set_modify(true);
}

/*
//bounding
*/
xui_method_explain(cocos_boundbox, get_bounding,	xui_rect2d<s32>		)( const xui_vector<s32>& trans, f64 ratio, s32 viewh )
{
	xui_rect2d<s32> rt = ori_bounding();
	s32 ax = xui_round(rt.ax*ratio) + trans.x;
	s32 bx = xui_round(rt.bx*ratio) + trans.x;
	s32 ay = viewh - (xui_round(rt.by*ratio) + trans.y);
	s32 by = viewh - (xui_round(rt.ay*ratio) + trans.y);

	return xui_rect2d<s32>(ax, ay, bx, by);
}
xui_method_explain(cocos_boundbox, ori_bounding,	xui_rect2d<s32>		)( void )
{
	cocos2d::Rect rt = m_ignoresz 
		? cocos2d::RectApplyAffineTransform(cocos2d::Rect(-25.0f, -25.0f, 50.0f, 50.0f), m_linkprop->get_node()->getNodeToParentAffineTransform(NULL))
		: m_linkprop->get_node()->screenBoundingBox();

	return xui_rect2d<s32>(rt.getMinX(), rt.getMinY(), rt.getMaxX(), rt.getMaxY());
}
xui_method_explain(cocos_boundbox, opt_bounding,	void				)( const xui_vector<s32>& trans, f64 ratio, s32 viewh, const xui_vector<s32>& delta, u08 op )
{
	if (op == BO_NONE)
		return;

	if (op == BO_MOVE_P)
	{
		m_pivotbox += delta;
		f32 x = (f32)(m_pivotbox.x - m_bounding.ax) / (f32)m_bounding.get_w();
		f32 y = (f32)(m_bounding.by - m_pivotbox.y) / (f32)m_bounding.get_h();
		if (x < 0.0f) x = 0.0f;
		if (x > 1.0f) x = 1.0f;
		if (y < 0.0f) y = 0.0f;
		if (y > 1.0f) y = 1.0f;
		cocos2d::ui::LayoutComponent* component = cocos2d::ui::LayoutComponent::bindLayoutComponent(m_linkprop->get_node());
		component->setAnchorPosition(cocos2d::Point(x, y));
	}
	else
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
		rt.ax = xui_round((m_bounding.ax-trans.x) / ratio);
		rt.bx = xui_round((m_bounding.bx-trans.x) / ratio);
		rt.ay = xui_round((viewh-m_bounding.by-trans.y) / ratio);
		rt.by = xui_round((viewh-m_bounding.ay-trans.y) / ratio);
		set_bounding(rt, op);
	}

	m_linkprop->get_file()->set_modify(true);
}
xui_method_explain(cocos_boundbox, set_bounding,	void				)( const xui_rect2d<s32>& rt, u08 op )
{
	if (op == BO_MOVE)
	{
		xui_vector<s32> delta = rt.get_pt() - ori_bounding().get_pt();
		xui_vector<s32> pt = ori_position();
		cocos2d::AffineTransform transform = cocos2d::AffineTransformMakeIdentity();
		if (m_linkprop->get_node()->getParent())
			transform = m_linkprop->get_node()->getParent()->getNodeToParentAffineTransform(NULL);

		delta.x = (s32)(delta.x / transform.a);
		delta.y = (s32)(delta.y / transform.d);
		set_position(pt+delta);
	}
	else
	{
		f32 x = m_linkprop->get_node()->getScaleX();
		f32 y = m_linkprop->get_node()->getScaleY();

		m_linkprop->get_node()->setScaleX(1.0f);
		m_linkprop->get_node()->setScaleY(1.0f);
		xui_rect2d<s32> orirt = ori_bounding();
		cocos2d::Vec2   pivot = m_ignoresz ? cocos2d::Vec2(0.5f, 0.5f) : m_linkprop->get_node()->getAnchorPoint(); 
		pivot.x = (s32)(pivot.x * orirt.get_w()) + orirt.ax;
		pivot.y = (s32)(pivot.y * orirt.get_h()) + orirt.ay;
		switch (op)
		{
		case BO_SIZE_L:
		case BO_SIZE_LT:
		case BO_SIZE_LB:
			if (pivot.x > rt.ax)
				x = (pivot.x - rt.ax) / (pivot.x - orirt.ax);
			break;
		case BO_SIZE_R:
		case BO_SIZE_RT:
		case BO_SIZE_RB:
			if (pivot.x < rt.bx)
				x = (rt.bx - pivot.x) / (orirt.bx - pivot.x);
			break;
		}

		switch (op)
		{
		case BO_SIZE_T:
		case BO_SIZE_LT:
		case BO_SIZE_RT:
			if (pivot.y < rt.by)
				y = (rt.by - pivot.y) / (orirt.by - pivot.y);
			break;
		case BO_SIZE_B:
		case BO_SIZE_LB:
		case BO_SIZE_RB:
			if (pivot.y > rt.ay)
				y = (pivot.y - rt.ay) / (pivot.y - orirt.ay);
			break;
		}

		m_linkprop->get_node()->setScaleX(x);
		m_linkprop->get_node()->setScaleY(y);
	}
}

/*
//method
*/
xui_method_explain(cocos_boundbox, add_operator,	void				)( u08 op )
{
	m_operator |= op;
}
xui_method_explain(cocos_boundbox, set_ignoresz,	void				)( bool flag )
{
	m_ignoresz = flag;
}
xui_method_explain(cocos_boundbox, get_linkprop,	cocos_propnodebase*	)( void )
{
	return m_linkprop;
}
xui_method_explain(cocos_boundbox, syn_bounding,	void 				)( const xui_vector<s32>& trans, f64 ratio, s32 viewh )
{
	m_bounding = get_bounding(trans, ratio, viewh);
	m_pivotbox = get_pivotbox(trans, ratio, viewh);
}
xui_method_explain(cocos_boundbox, hit_operator,	u08					)( const xui_vector<s32>& trans, f64 ratio, s32 viewh, const xui_vector<s32>& pt )
{
	if ((m_operator&OP_PIVOT) != 0)
	{
		xui_vector<s32>  pivot = get_pivotbox(trans, ratio, viewh);
		if (xui_abs(pt.x - pivot.x) <= 3 &&
			xui_abs(pt.y - pivot.y) <= 3)
			return BO_MOVE_P;
	}

	if ((m_operator&OP_SCALE) != 0)
	{
		xui_rect2d<s32>  rt = get_bounding(trans, ratio, viewh);
		s32 hx = rt.ax + rt.get_w()/2;
		s32 hy = rt.ay + rt.get_h()/2;
		if (xui_abs(pt.x-hx) <= 3)
		{
			if (xui_abs(pt.y-rt.ay) <= 3)
				return BO_SIZE_T;
			if (xui_abs(pt.y-rt.by) <= 3)
				return BO_SIZE_B;
		}
		if (xui_abs(pt.y-hy) <= 3)
		{
			if (xui_abs(pt.x-rt.ax) <= 3)
				return BO_SIZE_L;
			if (xui_abs(pt.x-rt.bx) <= 3)
				return BO_SIZE_R;
		}
		if (xui_abs(pt.x-rt.ax) <= 3)
		{
			if (xui_abs(pt.y-rt.ay) <= 3)
				return BO_SIZE_LT;
			if (xui_abs(pt.y-rt.by) <= 3)
				return BO_SIZE_LB;
		}
		if (xui_abs(pt.x-rt.bx) <= 3)
		{
			if (xui_abs(pt.y-rt.ay) <= 3)
				return BO_SIZE_RT;
			if (xui_abs(pt.y-rt.by) <= 3)
				return BO_SIZE_RB;
		}
	}

	return BO_NONE;
}
xui_method_explain(cocos_boundbox, get_opcursor,	u32					)( u08 op )
{
	switch (op)
	{
	case BO_MOVE:
	case BO_MOVE_P:
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
xui_method_explain(cocos_boundbox, was_selected,	bool				)( void )
{
	cocos_hierarchy* hierarchy = cocos_mainform::get_ptr()->get_hierarchy();
	xui_treedata* data = m_linkprop->get_linkdata(hierarchy->get_treeview());
	if (data)
	{
		xui_treenode* node = data->get_node();
		if (node)
			return node->was_selected();
	}

	return false;
}

/*
//draw
*/
xui_method_explain(cocos_boundbox, draw,			void				)( const xui_vector<s32>& trans, f64 ratio, s32 viewh, const xui_vector<s32>& pt, bool alwaysdraw )
{
	if (m_operator != OP_NONE)
	{
		xui_rect2d<s32> rt    = get_bounding(trans, ratio, viewh) + pt;
		xui_vector<s32> pivot = get_pivotbox(trans, ratio, viewh) + pt;
		bool selected = was_selected();
		if (selected || alwaysdraw)
		{
			draw_bounding(rt);
			if (selected)
				draw_operator(rt, pivot);
		}
	}
}
xui_method_explain(cocos_boundbox, draw_bounding,	void				)( const xui_rect2d<s32>& rt )
{
	xui_convas::get_ins()->draw_rectangle(rt, xui_colour(1.0f, 0.7f));
}
xui_method_explain(cocos_boundbox, draw_operator,	void				)( const xui_rect2d<s32>& rt, const xui_vector<s32>& pivot )
{
	if ((m_operator&OP_SCALE) != 0)
	{
		s32 hx = rt.ax + rt.get_w()/2;
		s32 hy = rt.ay + rt.get_h()/2;
		xui_rect2d<s32> op_lc(rt.ax-3, hy-3, rt.ax+3, hy+3);
		xui_convas::get_ins()->fill_rectangle(op_lc, xui_colour::black);
		xui_convas::get_ins()->draw_rectangle(op_lc, xui_colour::white);
		xui_rect2d<s32> op_rc(rt.bx-3, hy-3, rt.bx+3, hy+3);
		xui_convas::get_ins()->fill_rectangle(op_rc, xui_colour::black);
		xui_convas::get_ins()->draw_rectangle(op_rc, xui_colour::white);
		xui_rect2d<s32> op_tc(hx-3, rt.ay-3, hx+3, rt.ay+3);
		xui_convas::get_ins()->fill_rectangle(op_tc, xui_colour::black);
		xui_convas::get_ins()->draw_rectangle(op_tc, xui_colour::white);
		xui_rect2d<s32> op_bc(hx-3, rt.by-3, hx+3, rt.by+3);
		xui_convas::get_ins()->fill_rectangle(op_bc, xui_colour::black);
		xui_convas::get_ins()->draw_rectangle(op_bc, xui_colour::white);

		xui_rect2d<s32> op_lt(rt.ax-3, rt.ay-3, rt.ax+3, rt.ay+3);
		xui_convas::get_ins()->fill_rectangle(op_lt, xui_colour::black);
		xui_convas::get_ins()->draw_rectangle(op_lt, xui_colour::white);
		xui_rect2d<s32> op_rt(rt.bx-3, rt.ay-3, rt.bx+3, rt.ay+3);
		xui_convas::get_ins()->fill_rectangle(op_rt, xui_colour::black);
		xui_convas::get_ins()->draw_rectangle(op_rt, xui_colour::white);
		xui_rect2d<s32> op_lb(rt.ax-3, rt.by-3, rt.ax+3, rt.by+3);
		xui_convas::get_ins()->fill_rectangle(op_lb, xui_colour::black);
		xui_convas::get_ins()->draw_rectangle(op_lb, xui_colour::white);
		xui_rect2d<s32> op_rb(rt.bx-3, rt.by-3, rt.bx+3, rt.by+3);
		xui_convas::get_ins()->fill_rectangle(op_rb, xui_colour::black);
		xui_convas::get_ins()->draw_rectangle(op_rb, xui_colour::white);
	}

	if ((m_operator&OP_PIVOT) != 0 || m_ignoresz)
	{
		xui_convas::get_ins()->fill_triangle(xui_vector<s32>(pivot.x, pivot.y-3), 3, TRIANGLE_UP,   xui_colour::white);
		xui_convas::get_ins()->fill_triangle(xui_vector<s32>(pivot.x, pivot.y+3), 3, TRIANGLE_DOWN, xui_colour::white);

		xui_vector<s32> path[4];
		path[0] = xui_vector<s32>(pivot.x - 6, pivot.y);
		path[1] = xui_vector<s32>(pivot.x, pivot.y - 6);
		path[2] = xui_vector<s32>(pivot.x + 6, pivot.y);
		path[3] = xui_vector<s32>(pivot.x, pivot.y + 6);
		xui_convas::get_ins()->draw_path(path, 4, xui_colour::black);
	}
}