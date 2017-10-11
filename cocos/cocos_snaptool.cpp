#include "xui_convas.h"
#include "xui_control.h"
#include "xui_toggle.h"
#include "xui_desktop.h"
#include "xui_treenode.h"
#include "cocos_resource.h"
#include "cocos_treedata.h"
#include "cocos_boundbox.h"
#include "cocos_snaptool.h"

xui_create_explain(cocos_snaptool)( xui_control* drawview )
: m_drawview(drawview)
, m_trans(xui_vector<s32>(0))
, m_ratio(1.0)
{
	m_snapctrl	= new xui_toggle(xui_vector<s32>(80, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_snapctrl,	ini_component	)(0, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_snapctrl,	set_corner		)(3);
	xui_method_ptrcall(m_snapctrl,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_snapctrl,	set_drawcolor	)(true);
	xui_method_ptrcall(m_snapctrl,	set_textalign	)(TEXTALIGN_CC);
	xui_method_ptrcall(m_snapctrl,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_snapctrl,	ini_drawer		)(cocos_resource::icon_magent);
	xui_method_ptrcall(m_snapctrl,	ini_drawer		)(L"Attract");
	xui_method_ptrcall(m_snapctrl,	ini_toggle		)(true);
}

/*
//method
*/
xui_method_explain(cocos_snaptool, set_trans,		void					)( const xui_vector<s32>& trans )
{
	m_trans = trans;
}
xui_method_explain(cocos_snaptool, set_ratio,		void					)( f64 ratio )
{
	m_ratio = ratio;
}
xui_method_explain(cocos_snaptool, cal_selfbbox,	cocos_boundbox*			)( const std::vector<cocos_boundbox*>& selectedvec )
{
	if (selectedvec.size() > 0)
	{
		s32 viewh = m_drawview->get_renderh();
		xui_vector<s32> pt = m_drawview->get_renderpt(xui_desktop::get_ins()->get_mousecurr());
		for (u32 i = 0; i < selectedvec.size(); ++i)
		{
			cocos_boundbox* bbox = selectedvec[i];
			xui_rect2d<s32> rect = bbox->get_bounding(m_trans, m_ratio, viewh);
			if (rect.was_inside(pt))
				return bbox;
		}
	}

	return NULL;
}
xui_method_explain(cocos_snaptool, cal_snapinfo,	void					)( const std::vector<cocos_boundbox*>& boundboxvec, const xui_rect2d<s32>& self, u08 mode, s32 snaplength )
{
	m_horzsnap.clear();
	m_vertsnap.clear();
	m_horzstep.clear();
	m_vertstep.clear();
	m_horzmidd.clear();
	m_vertmidd.clear();

	if (m_snapctrl->was_push() == false)
		return;

	for (u32 i = 0; i < boundboxvec.size(); ++i)
	{
		xui_rect2d<s32> currrect = boundboxvec[i]->ori_bounding();
		if (snaplength > 0)
		{
			if (currrect.bx < self.ax-snaplength ||
				currrect.ax > self.bx+snaplength)
				continue;
		}

		if (mode == DRAGMOVE_UNLIMIT || mode == DRAGMOVE_X)
		{
			s32 l = currrect.ax;
			s32 r = currrect.bx;
			s32 c = currrect.ax+currrect.get_w()/2;
			m_horzsnap[l].push_back(snap_info(boundboxvec[i]));
			m_horzsnap[r].push_back(snap_info(boundboxvec[i]));
			m_horzmidd[c].push_back(snap_info(boundboxvec[i]));
		}
		if (mode == DRAGMOVE_UNLIMIT || mode == DRAGMOVE_Y)
		{
			s32 t = currrect.ay;
			s32 b = currrect.by;
			s32 c = currrect.ay+currrect.get_h()/2;
			m_vertsnap[t].push_back(snap_info(boundboxvec[i]));
			m_vertsnap[b].push_back(snap_info(boundboxvec[i]));
			m_vertmidd[c].push_back(snap_info(boundboxvec[i]));
		}
	}

	if (boundboxvec.size() < 2)
		return;

	for (u32 i = 0; i < boundboxvec.size()-1; ++i)
	{
		xui_rect2d<s32> currrect = boundboxvec[i  ]->ori_bounding();
		xui_rect2d<s32> nextrect = boundboxvec[i+1]->ori_bounding();

		if (snaplength > 0)
		{
			if (currrect.bx < self.ax-snaplength ||
				currrect.ax > self.bx+snaplength)
				continue;
		}

		if (mode == DRAGMOVE_UNLIMIT || mode == DRAGMOVE_X)
		{
			s32 horzstep = 0;
			if		(nextrect.ax > currrect.bx) horzstep = nextrect.ax - currrect.bx;
			else if (nextrect.bx < currrect.ax) horzstep = currrect.ax - nextrect.bx;
			else
			{}

			if (horzstep > 0)
				m_horzstep[horzstep].push_back(snap_info(boundboxvec[i], boundboxvec[i+1]));
		}
		if (mode == DRAGMOVE_UNLIMIT || mode == DRAGMOVE_Y)
		{
			s32 vertstep = 0;
			if		(nextrect.ay > currrect.by) vertstep = nextrect.ay - currrect.by;
			else if (nextrect.by < currrect.ay) vertstep = currrect.ay - nextrect.by;
			else
			{}

			if (vertstep > 0)
				m_vertstep[vertstep].push_back(snap_info(boundboxvec[i], boundboxvec[i+1]));
		}
	}
}
xui_method_explain(cocos_snaptool, use_snapinfo,	void					)( const std::vector<cocos_boundbox*>& boundboxvec, const std::vector<cocos_boundbox*>& selectedvec )
{
	xui_vector<s32> snap;
	xui_vector<s32> step;
	xui_rect2d<s32> self = cal_selfrect(selectedvec);
	xui_vector<s32> move = cal_snapmove(boundboxvec, self, snap, step, NULL, NULL);
	for (u32 i = 0; i < selectedvec.size(); ++i)
	{
		cocos_boundbox* bbox = selectedvec[i];
		bbox->set_position(bbox->ori_position()+move);
	}
}
xui_method_explain(cocos_snaptool, get_snapctrl,	xui_toggle*				)( void )
{
	return m_snapctrl;
}
xui_method_explain(cocos_snaptool, set_snapdraw,	void					)( const std::vector<cocos_boundbox*>& boundboxvec, const std::vector<cocos_boundbox*>& selectedvec )
{
	xui_vector<s32> snap;
	xui_vector<s32> step;
	xui_rect2d<s32> horz;
	xui_rect2d<s32> vert;
	xui_rect2d<s32> self = cal_selfrect(selectedvec);
	xui_vector<s32> move = cal_snapmove(boundboxvec, self, snap, step, &horz, &vert);

	cocos_snapinfo_map::iterator itor;
	if (move.x != 0 && step.x == 0)
	{
		itor = m_horzsnap.find(snap.x);
		if (itor != m_horzsnap.end())
			draw_horzsnap(self, (*itor).first, (*itor).second);
		itor = m_horzmidd.find(snap.x);
		if (itor != m_horzmidd.end())
			draw_horzsnap(self, (*itor).first, (*itor).second);
	}
	if (move.y != 0 && step.y == 0)
	{
		itor = m_vertsnap.find(snap.y);
		if (itor != m_vertsnap.end())
			draw_vertsnap(self, (*itor).first, (*itor).second);
		itor = m_vertmidd.find(snap.y);
		if (itor != m_vertmidd.end())
			draw_vertsnap(self, (*itor).first, (*itor).second);
	}

	if (move.x != 0 && step.x != 0)
	{
		itor = m_horzstep.find(step.x);
		if (itor != m_horzstep.end())
			draw_horzstep(self, (*itor).first, (*itor).second, horz);
	}
	if (move.y != 0 && step.y != 0)
	{
		itor = m_vertstep.find(step.y);
		if (itor != m_vertstep.end())
			draw_vertstep(self, (*itor).first, (*itor).second, vert);
	}
}
xui_method_explain(cocos_snaptool, cal_snapmove,	xui_vector<s32>			)( const std::vector<cocos_boundbox*>& boundboxvec, const xui_rect2d<s32>& self, xui_vector<s32>& snap, xui_vector<s32>& step, xui_rect2d<s32>* horz, xui_rect2d<s32>* vert )
{
	xui_vector<s32> move(0);
	if (boundboxvec.size() > 0)
	{
		cocos_snapinfo_map::iterator itor;
		for (itor = m_horzsnap.begin(); itor != m_horzsnap.end(); ++itor)
		{
			s32 line = (*itor).first;
			if (self.ax >= line-4 && self.ax <= line+4 && cal_linestep(move.x, line-self.ax))
				snap.x   = line;
			if (self.bx >= line-4 && self.bx <= line+4 && cal_linestep(move.x, line-self.bx))
				snap.x   = line;
		}
		for (itor = m_vertsnap.begin(); itor != m_vertsnap.end(); ++itor)
		{
			s32 line = (*itor).first;
			if (self.ay >= line-4 && self.ay <= line+4 && cal_linestep(move.y, line-self.ay))
				snap.y   = line;
			if (self.by >= line-4 && self.by <= line+4 && cal_linestep(move.y, line-self.by))
				snap.y   = line;
		}

		xui_vector<s32> midd(self.ax+self.get_w()/2, self.ay+self.get_h()/2);
		for (itor = m_horzmidd.begin(); itor != m_horzmidd.end(); ++itor)
		{
			s32 line = (*itor).first;
			if (midd.x  >= line-4 && midd.x  <= line+4 && cal_linestep(move.x, line-midd.x))
				snap.x   = line;
		}
		for (itor = m_vertmidd.begin(); itor != m_vertmidd.end(); ++itor)
		{
			s32 line = (*itor).first;
			if (midd.y  >= line-4 && midd.y  <= line+4 && cal_linestep(move.y, line-midd.y))
				snap.y   = line;
		}

		for (u32 i = 0; i < boundboxvec.size(); ++i)
		{
			xui_rect2d<s32> rect = boundboxvec[i]->ori_bounding();
			xui_rect2d<s32> temp = rect.get_inter(self);

			s32 horzstep = 0;
			s32 vertstep = 0;
			if (temp.get_h() > 0)
			{
				if		(self.ax > rect.bx) horzstep = self.ax - rect.bx;
				else if (self.bx < rect.ax) horzstep = rect.ax - self.bx;
				else
				{}
			}
			if (temp.get_w() > 0)
			{
				if		(self.ay > rect.by) vertstep = self.ay - rect.by;
				else if (self.by < rect.ay) vertstep = rect.ay - self.by;
				else
				{}
			}

			s32 steparray[7] = {0, -1, 1, -2, 2, -3, 3};
			if (horzstep > 0)
			{
				for (u08 istep = 0; istep < 5; ++istep)
				{
					s32 line = horzstep + steparray[istep];
					if (step.x != 0 && line >= step.x)
						continue;

					itor = m_horzstep.find(line);
					if (itor != m_horzstep.end())
					{
						if		(self.ax > rect.bx && cal_linestep(move.x, rect.bx+line-self.ax)) { step.x = line; if (horz) *horz = rect; }
						else if (self.bx < rect.ax && cal_linestep(move.x, rect.ax-line-self.bx)) { step.x = line; if (horz) *horz = rect; }
						else
						{}

						break;
					}
				}
			}
			if (vertstep > 0)
			{
				for (u08 istep = 0; istep < 5; ++istep)
				{
					s32 line = vertstep + steparray[istep];
					if (step.y != 0 && line >= step.y)
						continue;

					itor = m_vertstep.find(line);
					if (itor != m_vertstep.end())
					{
						if		(self.ay > rect.by && cal_linestep(move.y, rect.by+line-self.ay)) { step.y = line; if (vert) *vert = rect; }
						else if (self.by < rect.ay && cal_linestep(move.y, rect.ay-line-self.by)) { step.y = line; if (vert) *vert = rect; }
						else
						{}

						break;
					}
				}
			}
		}
	}

	return xui_vector<s32>(
		((move.x == 10) ? 0 : move.x),
		((move.y == 10) ? 0 : move.y));
}
xui_method_explain(cocos_snaptool, cal_linestep,	bool					)( s32& last, s32 temp )
{
	if (last == 0 || abs(last) > abs(temp))
	{
		last = temp;
		return true;
	}

	return false;
}
xui_method_explain(cocos_snaptool, cal_selfrect,	xui_rect2d<s32>			)( const std::vector<cocos_boundbox*>& selectedvec )
{
	if (selectedvec.size() > 0)
	{
		s32 viewh = m_drawview->get_renderh();
		xui_vector<s32> pt = m_drawview->get_renderpt(xui_desktop::get_ins()->get_mousecurr());
		for (u32 i = 0; i < selectedvec.size(); ++i)
		{
			cocos_boundbox* bbox = selectedvec[i];
			xui_rect2d<s32> rect = bbox->get_bounding(m_trans, m_ratio, viewh);
			if (rect.was_inside(pt))
				return bbox->ori_bounding();
		}
	}

	return xui_rect2d<s32>();
}

/*
//draw snap
*/
xui_method_explain(cocos_snaptool, draw_horzsnap,	void					)( const xui_rect2d<s32>& self, s32 snap, const std::vector<snap_info>& vec )
{
	s32 ymin = self.ay;
	s32 ymax = self.by;
	for (u32 i = 0; i < vec.size(); ++i)
	{
		xui_rect2d<s32> rect = vec[i].curr->ori_bounding();
		ymin = xui_min(ymin, rect.ay);
		ymax = xui_max(ymax, rect.by);
	}

	xui_vector<s32> pt = m_drawview->get_screenpt();
	xui_vector<s32> p1(xui_round(snap*m_ratio + m_trans.x), m_drawview->get_renderh() - (xui_round(ymin*m_ratio) + m_trans.y));
	xui_vector<s32> p2(xui_round(snap*m_ratio + m_trans.x), m_drawview->get_renderh() - (xui_round(ymax*m_ratio) + m_trans.y));
	xui_convas::get_ins()->draw_line(p1+pt, p2+pt, xui_colour::blue);
}
xui_method_explain(cocos_snaptool, draw_vertsnap,	void					)( const xui_rect2d<s32>& self, s32 snap, const std::vector<snap_info>& vec )
{
	s32 xmin = self.ax;
	s32 xmax = self.bx;
	for (u32 i = 0; i < vec.size(); ++i)
	{
		xui_rect2d<s32> rect = vec[i].curr->ori_bounding();
		xmin = xui_min(xmin, rect.ax);
		xmax = xui_max(xmax, rect.bx);
	}

	xui_vector<s32> pt = m_drawview->get_screenpt();
	xui_vector<s32> p1(xui_round(xmin*m_ratio + m_trans.x), m_drawview->get_renderh() - (xui_round(snap*m_ratio) + m_trans.y));
	xui_vector<s32> p2(xui_round(xmax*m_ratio + m_trans.x), m_drawview->get_renderh() - (xui_round(snap*m_ratio) + m_trans.y));
	xui_convas::get_ins()->draw_line(p1+pt, p2+pt, xui_colour::blue);
}
xui_method_explain(cocos_snaptool, draw_horzstep,	void					)( const xui_rect2d<s32>& self, s32 snap, const std::vector<snap_info>& vec, const xui_rect2d<s32>& curr )
{
	std::vector< xui_rect2d<s32> > currrectvec;
	std::vector< xui_rect2d<s32> > nextrectvec;
	for (u32 i = 0; i < vec.size(); ++i)
	{
		currrectvec.push_back(vec[i].curr->ori_bounding());
		nextrectvec.push_back(vec[i].next->ori_bounding());
	}

	currrectvec.push_back(curr);
	nextrectvec.push_back(self);

	xui_vector<s32> pt = m_drawview->get_screenpt();
	u32 count = xui_min(currrectvec.size(), nextrectvec.size());
	for (u32 i = 0; i < count; ++i)
	{
		xui_rect2d<s32>& currrect = currrectvec[i];
		xui_rect2d<s32>& nextrect = nextrectvec[i];
		xui_rect2d<s32>  temprect = nextrect.get_inter(currrect);
		s32 xmin = 0;
		if		(nextrect.ax > currrect.bx) xmin = currrect.bx;
		else if (nextrect.bx < currrect.ax) xmin = nextrect.bx;
		else
		{}

		s32 xmax = xmin + snap;
		s32 ymid = (temprect.get_h() > 0) ? (temprect.ay+temprect.get_h()/2) : (nextrect.ay+nextrect.get_h()/2); 

		xui_vector<s32> p1;
		xui_vector<s32> p2;
		p1 = xui_vector<s32>(xui_round(xmin*m_ratio + m_trans.x), m_drawview->get_renderh() - (xui_round(ymid*m_ratio) + m_trans.y));
		p2 = xui_vector<s32>(xui_round(xmax*m_ratio + m_trans.x), m_drawview->get_renderh() - (xui_round(ymid*m_ratio) + m_trans.y));
		xui_convas::get_ins()->draw_line(p1+pt, p2+pt, xui_colour::white);
		if (temprect.get_h() > 0)
			continue;

		s32 ymin = xui_min(currrect.ay, nextrect.ay);
		s32 ymax = xui_max(currrect.by, nextrect.by);
		s32 temp = nextrect.ax > currrect.bx ? currrect.bx : currrect.ax;
		p1 = xui_vector<s32>(xui_round(temp*m_ratio + m_trans.x), m_drawview->get_renderh() - (xui_round(ymin*m_ratio) + m_trans.y));
		p2 = xui_vector<s32>(xui_round(temp*m_ratio + m_trans.x), m_drawview->get_renderh() - (xui_round(ymax*m_ratio) + m_trans.y));
		xui_convas::get_ins()->draw_line(p1+pt, p2+pt, xui_colour::red);
	}
}
xui_method_explain(cocos_snaptool, draw_vertstep,	void					)( const xui_rect2d<s32>& self, s32 snap, const std::vector<snap_info>& vec, const xui_rect2d<s32>& curr )
{
	std::vector< xui_rect2d<s32> > currrectvec;
	std::vector< xui_rect2d<s32> > nextrectvec;
	for (u32 i = 0; i < vec.size(); ++i)
	{
		currrectvec.push_back(vec[i].curr->ori_bounding());
		nextrectvec.push_back(vec[i].next->ori_bounding());
	}

	currrectvec.push_back(curr);
	nextrectvec.push_back(self);

	xui_vector<s32> pt = m_drawview->get_screenpt();
	u32 count = xui_min(currrectvec.size(), nextrectvec.size());
	for (u32 i = 0; i < count; ++i)
	{
		xui_rect2d<s32>& currrect = currrectvec[i];
		xui_rect2d<s32>& nextrect = nextrectvec[i];
		xui_rect2d<s32>  temprect = nextrect.get_inter(currrect);
		s32 ymin = 0;
		if		(nextrect.ay > currrect.by) ymin = currrect.by;
		else if (nextrect.by < currrect.ay) ymin = nextrect.by;
		else
		{}

		s32 ymax = ymin + snap;
		s32 xmid = (temprect.get_w() > 0) ? (temprect.ax+temprect.get_w()/2) : (nextrect.ax+nextrect.get_w()/2); 

		xui_vector<s32> p1;
		xui_vector<s32> p2;
		p1 = xui_vector<s32>(xui_round(xmid*m_ratio + m_trans.x), m_drawview->get_renderh() - (xui_round(ymin*m_ratio) + m_trans.y));
		p2 = xui_vector<s32>(xui_round(xmid*m_ratio + m_trans.x), m_drawview->get_renderh() - (xui_round(ymax*m_ratio) + m_trans.y));
		xui_convas::get_ins()->draw_line(p1+pt, p2+pt, xui_colour::white);
		if (temprect.get_w() > 0)
			continue;

		s32 xmin = xui_min(currrect.ax, nextrect.ax);
		s32 xmax = xui_max(currrect.bx, nextrect.bx);
		s32 temp = nextrect.ay > currrect.by ? currrect.by : currrect.ay;
		p1 = xui_vector<s32>(xui_round(xmin*m_ratio + m_trans.x), m_drawview->get_renderh() - (xui_round(temp*m_ratio) + m_trans.y));
		p2 = xui_vector<s32>(xui_round(xmax*m_ratio + m_trans.x), m_drawview->get_renderh() - (xui_round(temp*m_ratio) + m_trans.y));
		xui_convas::get_ins()->draw_line(p1+pt, p2+pt, xui_colour::red);
	}
}