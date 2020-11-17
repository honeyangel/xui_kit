#include "xui_desktop.h"
#include "xui_canvas.h"
#include "xui_gridbox.h"

xui_implement_rtti(xui_gridbox, xui_control)

xui_gridbox* xui_gridbox::create( u32 row, u32 col, s32 titlewidth )
{
	xui_gridbox* gridbox = new xui_gridbox(row, col);
	gridbox->set_backcolor(xui_colour(1.0f, 0.20f));
	gridbox->set_drawcolor(true);
	gridbox->set_sidestyle(k_sidestyle_s);
	gridbox->set_corner(3);
	gridbox->set_colpixel(0, titlewidth);

	return gridbox;
}

xui_gridbox::xui_gridbox( u32 row, u32 col )
: xui_control(xui_vector<s32>(0))
{
	m_rowcount = row;
	m_colcount = col;
	for (u32 i = 0; i < k_max_row; ++i)
		m_rowpixel[i] = 24;
	for (u32 i = 0; i < k_max_col; ++i)
		m_colpixel[i] = 24;

	memset(m_ptrarray,  0, sizeof(m_ptrarray));
}

u32 xui_gridbox::get_rowcount( void ) const
{
	return m_rowcount;
}

void xui_gridbox::set_rowcount( u32 count )
{
	if (m_rowcount != count)
	{
		m_rowcount  = count;
		invalid();
	}
}

u32 xui_gridbox::get_colcount( void ) const
{
	return m_colcount;
}

void xui_gridbox::set_colcount( u32 count )
{
	if (m_colcount != count)
	{
		m_colcount  = count;
		invalid();
	}
}

s32 xui_gridbox::get_rowpixel( u32 row ) const
{
	return m_rowpixel[row];
}

void xui_gridbox::set_rowpixel( u32 row, s32 pixel )
{
	if (m_rowpixel[row] != pixel)
	{
		m_rowpixel[row]  = pixel;
		invalid();
	}
}

s32 xui_gridbox::get_colpixel( u32 col ) const
{
	return m_colpixel[col];
}

void xui_gridbox::set_colpixel( u32 col, s32 pixel )
{
	if (m_colpixel[col] != pixel)
	{
		m_colpixel[col]  = pixel;
		invalid();
	}
}

void xui_gridbox::set_gridctrl( u32 row, u32 col, xui_control* ctrl )
{
	std::vector<xui_component*>::iterator itor = std::find(
		m_widgetvec.begin(),
		m_widgetvec.end(),
		m_ptrarray[row][col]);

	if (itor != m_widgetvec.end())
	{
		m_widgetvec.erase(itor);
		m_ptrarray[row][col]->set_parent(NULL);
		xui_desktop::get_ins()->move_recycle(*itor);
	}

	m_ptrarray[row][col] = ctrl;
	if (ctrl)
	{
		ctrl->set_parent(this);
		ctrl->set_sidestyle(k_sidestyle_n);
		m_widgetvec.push_back(ctrl);
		perform();
	}
}

xui_rect2d<s32> xui_gridbox::get_cornerrt( xui_component* component ) const
{
	xui_rect2d<s32> corner(0);
	for (u32 row = 0; row < m_rowcount; ++row)
	{
		for (u32 col = 0; col < m_colcount; ++col)
		{
			if (m_ptrarray[row][col] == component)
			{
				if (row == 0 && 
					col == 0)
				{
					corner.value[0] = m_corner;
				}
				if (row == 0 && 
					col == m_colcount-1)
				{
					corner.value[1] = m_corner;
				}
				if (row == m_rowcount-1 && 
					col == m_colcount-1)
				{
					corner.value[2] = m_corner;
				}
				if (row == m_rowcount-1 && 
					col == 0)
				{
					corner.value[3] = m_corner;
				}

				return corner;
			}
		}
	}

	return corner;
}

void xui_gridbox::render_else( void )
{
	xui_control::render_else();

	if (m_sidestyle)
	{
		xui_rect2d<s32> cornerrt = xui_control::get_cornerrt();
		xui_rect2d<s32> renderrt = get_renderrtabs();
		xui_colour      color    = get_vertexcolor();

		renderrt.bx -= 1;
		renderrt.by -= 1;
		xui_colour side_color = m_sidecolor * color;

		if (m_rowcount > 1)
		{
			xui_vector<s32> p1 = renderrt.get_pt(); 
			xui_vector<s32> p2 = p1 + xui_vector<s32>(renderrt.get_w(), 0);
			for (u32 i = 0; i < m_rowcount-1; ++i)
			{
				s32 h = m_rowpixel[i];
				p1 += xui_vector<s32>(0, h);
				p2 += xui_vector<s32>(0, h);
				xui_canvas::get_ins()->draw_line(p1, p2, side_color);
			}
		}
	}
}

void xui_gridbox::on_invalid( xui_method_args& args )
{
	xui_vector<s32> sz(0);
	for (u32 col = 0; col < m_colcount; ++col)
		sz.w += m_colpixel[col];
	for (u32 row = 0; row < m_rowcount; ++row)
		sz.h += m_rowpixel[row];

	if (get_rendersz() != sz)
	{
		set_rendersz(sz);
	}
	else
	{
		perform();
	}
}

void xui_gridbox::on_perform( xui_method_args& args )
{
	xui_vector<s32> pt(0);
	xui_vector<s32> sz(0);
	for (u32 row = 0; row < m_rowcount; ++row)
	{
		pt.x  = 0;
		for (u32 col = 0; col < m_colcount; ++col)
		{
			sz.w   = m_colpixel[col];
			sz.h   = m_rowpixel[row];
			if (m_ptrarray[row][col])
			{
				m_ptrarray[row][col]->on_perform_pt(pt);
				m_ptrarray[row][col]->on_perform_sz(sz);
			}

			pt.x  += m_colpixel[col];
		}

		pt.y += m_rowpixel[row];
	}
}