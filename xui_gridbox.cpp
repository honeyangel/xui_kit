#include "xui_gridbox.h"

/*
//constructor
*/
xui_create_explain(xui_gridbox)( const std::string& name, const xui_rect2d<s32>& rect )
: xui_control(name, rect)
{
	m_type	   += "gridbox";
	m_rowcount  = 1;
	m_colcount  = 1;
	memset(m_rowpixel, 50, sizeof(m_rowpixel));
	memset(m_colpixel, 50, sizeof(m_colpixel));
	memset(m_ptrarray,  0, sizeof(m_ptrarray));
}

/*
//count
*/
xui_method_explain(xui_gridbox, get_rowcount,	u32				)( void ) const
{
	return m_rowcount;
}
xui_method_explain(xui_gridbox, set_rowcount,	void			)( u32 count )
{
	if (m_rowcount != count)
	{
		m_rowcount  = count;
		invalid();
	}
}
xui_method_explain(xui_gridbox, get_colcount,	u32				)( void ) const
{
	return m_colcount;
}
xui_method_explain(xui_gridbox, set_colcount,	void			)( u32 count )
{
	if (m_colcount != count)
	{
		m_colcount  = count;
		invalid();
	}
}

/*
//pixel
*/
xui_method_explain(xui_gridbox, get_rowpixel,	s32				)( u32 row ) const
{
	return m_rowpixel[row];
}
xui_method_explain(xui_gridbox, set_rowpixel,	void			)( u32 row, s32 pixel )
{
	if (m_rowpixel[row] != pixel)
	{
		m_rowpixel[row]  = pixel;
		invalid();
	}
}
xui_method_explain(xui_gridbox, get_colpixel,	s32				)( u32 col ) const
{
	return m_colpixel[col];
}
xui_method_explain(xui_gridbox, set_colpixel,	void			)( u32 col, s32 pixel )
{
	if (m_colpixel[col] != pixel)
	{
		m_colpixel[col]  = pixel;
		invalid();
	}
}

/*
//grid ctrl
*/
xui_method_explain(xui_gridbox, set_gridctrl,	void			)( u32 row, u32 col, xui_component* componet )
{
	std::vector<xui_component*>::iterator itor = std::find(
		m_widgetvec.begin(),
		m_widgetvec.end(),
		m_ptrarray[row][col]);

	if (itor != m_widgetvec.end())
	{
		m_widgetvec.erase(itor);
		delete (*itor);
	}

	m_ptrarray[row][col] = componet;
	if (componet)
	{
		componet->set_parent(this);
		m_widgetvec.push_back(componet);
		perform();
	}
}
xui_method_explain(xui_gridbox, get_cornerrt,	xui_rect2d<s32>	)( xui_component* componet ) const
{
	xui_rect2d<s32> corner(0);
	for (u32 row = 0; row < m_rowcount; ++row)
	{
		for (u32 col = 0; col < m_colcount; ++col)
		{
			if (m_ptrarray[row][col] == componet)
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

/*
//callback
*/
xui_method_explain(xui_gridbox, on_invalid,		void			)( xui_method_args& args )
{
	//计算大小
	xui_vector<s32> sz(0);
	for (u32 col = 0; col < m_colcount; ++col)
		sz.w += m_colpixel[col];
	for (u32 row = 0; row < m_rowcount; ++row)
		sz.h += m_rowpixel[row];

	//布局
	if (get_rendersz() != sz)
	{
		set_rendersz(sz);
	}
	else
	{
		perform();
	}
}
xui_method_explain(xui_gridbox, on_perform,		void			)( xui_method_args& args )
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
				m_ptrarray[row][col]->set_renderpt(pt, false);
				m_ptrarray[row][col]->set_rendersz(sz, false);
			}

			pt.x  += m_colpixel[col];
		}

		pt.y += m_rowpixel[row];
	}
}