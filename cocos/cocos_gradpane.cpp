#include "xui_convas.h"
#include "xui_desktop.h"
#include "xui_menu.h"
#include "xui_menuitem.h"
#include "xui_dialog.h"
#include "xui_textbox.h"
#include "cocos_mainform.h"
#include "cocos_gradpane.h"

extern s32 xui_round( f64 value );

/*
//dialog
*/
class cocos_setval_dialog : public xui_dialog
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	cocos_setval_dialog( void )
	: xui_dialog(L"", 2)
	{
		m_number = new xui_textbox(xui_vector<s32>(120, 18));
		xui_method_ptrcall(m_number, set_backcolor	)(xui_colour::darkgray);
		xui_method_ptrcall(m_number, set_drawcolor	)(true);
		xui_method_ptrcall(m_number, set_borderrt	)(xui_rect2d<s32>(4, 2, 4, 2));
		xui_method_ptrcall(m_number, set_sidestyle	)(SIDESTYLE_S);
		xui_method_ptrcall(m_number, set_textalign	)(TEXTALIGN_LC);
		xui_method_ptrcall(m_number, set_numbtype	)(NT_INT);
		xui_method_ptrcall(m_number, ini_component	)(ALIGNHORZ_C, ALIGNVERT_T, 0);
		xui_method_ptrcall(m_number, req_focus		)();
		xui_method_ptrcall(m_detail, ini_component	)(true, false);

		add_child(m_number);
		refresh();
	}

	/*
	//method
	*/
	u32				get_index( void ) const
	{
		return (u32)m_number->get_data();
	}
	s32				get_value( void ) const
	{
		s32 value;
		std::wstringstream text(m_number->get_text().c_str());
		text >> value;

		return value;
	}
	void			set_value( u32 index, s32 value )
	{
		std::wstringstream text;
		text << value;

		xui_method_ptrcall(m_number, ini_textbox	)(text.str());
		xui_method_ptrcall(m_number, set_data		)((void*)index);
		xui_method_ptrcall(m_number, set_selecttext	)(0, text.str().length());
	}

protected:
	/*
	//member
	*/
	xui_textbox*	m_number;
};

xui_implement_rtti(cocos_setval_dialog, xui_dialog);
xui_implement_rtti(cocos_gradpane,		xui_drawer);

/*
//constructor
*/
xui_create_explain(cocos_gradpane)( s32 flowstyle )
: xui_drawer(xui_vector<s32>(40))
, m_flowstyle(flowstyle)
, m_downindex(-1)
, m_trans(0)
, m_ratio(1.0)
{
	m_rightmenu = xui_menu::create(80);
	m_setval	= m_rightmenu->add_item(NULL, L"Set Value");
	m_delete	= m_rightmenu->add_item(NULL, L"Delete");
	m_delall	= m_rightmenu->add_item(NULL, L"Delete All");
	xui_method_ptrcall(m_setval, xm_click) += new xui_method_member<xui_method_args, cocos_gradpane>(this, &cocos_gradpane::on_menuclick);
	xui_method_ptrcall(m_delete, xm_click) += new xui_method_member<xui_method_args, cocos_gradpane>(this, &cocos_gradpane::on_menuclick);
	xui_method_ptrcall(m_delall, xm_click) += new xui_method_member<xui_method_args, cocos_gradpane>(this, &cocos_gradpane::on_menuclick);
}

/*
//destructor
*/
xui_delete_explain(cocos_gradpane)( void )
{
	xui_desktop::get_ins()->move_recycle(m_rightmenu);
}

/*
//method
*/
xui_method_explain(cocos_gradpane, get_lines,			const std::vector<s32>&	)( void ) const
{
	return m_linearray;
}
xui_method_explain(cocos_gradpane, del_lines,			void					)( void )
{
	m_linearray.clear();
	m_downindex = -1;
}
xui_method_explain(cocos_gradpane, get_trans,			s32						)( void ) const
{
	return m_trans;
}
xui_method_explain(cocos_gradpane, set_trans,			void					)( s32 trans )
{
	m_trans = trans;
}
xui_method_explain(cocos_gradpane, get_ratio,			f64						)( void ) const
{
	return m_ratio;
}
xui_method_explain(cocos_gradpane, set_ratio,			void					)( f64 ratio )
{
	m_ratio = ratio;
}

/*
//override
*/
xui_method_explain(cocos_gradpane, on_mousedown,		void					)( xui_method_mouse& args )
{
	xui_drawer::on_mousedown(args);

	s32 line = 0;
	xui_vector<s32> pt = get_renderpt(args.point);
	switch (m_flowstyle)
	{
	case FLOWSTYLE_H: 
		line = xui_round((pt.x-m_trans) / m_ratio); 
		break;
	case FLOWSTYLE_V: 
		line = xui_round((get_renderh()-pt.y-m_trans) / m_ratio); 
		break;
	}

	for (u32 i = 0; i < m_linearray.size(); ++i)
	{
		if (m_linearray[i] >= line-1 &&
			m_linearray[i] <= line+1)
		{
			m_downindex = i;
			break;
		}
	}

	m_setval->set_enable(m_downindex != -1);
	m_delete->set_enable(m_downindex != -1);
	m_delall->set_enable(m_linearray.size() > 0);
}
xui_method_explain(cocos_gradpane, on_mousemove,		void					)( xui_method_mouse& args )
{
	xui_drawer::on_mousemove(args);
	m_cursor = CURSOR_DEFAULT;

	s32 line = 0;
	xui_vector<s32> pt = get_renderpt(args.point);
	switch (m_flowstyle)
	{
	case FLOWSTYLE_H: 
		line = xui_round((pt.x-m_trans) / m_ratio); 
		break;
	case FLOWSTYLE_V: 
		line = xui_round((get_renderh()-pt.y-m_trans) / m_ratio); 
		break;
	}

	if (has_catch() && m_downindex != -1)
	{
		m_linearray[m_downindex] = line;
	}
	else
	{
		for (u32 i = 0; i < m_linearray.size(); ++i)
		{
			if (m_linearray[i] >= line-1 &&
				m_linearray[i] <= line+1)
			{
				switch (m_flowstyle)
				{
				case FLOWSTYLE_H: 
					m_cursor = CURSOR_WE; 
					break;
				case FLOWSTYLE_V: 
					m_cursor = CURSOR_NS; 
					break;
				}

				break;
			}
		}
	}
}
xui_method_explain(cocos_gradpane, on_mouserise,		void					)( xui_method_mouse& args )
{
	xui_drawer::on_mouserise(args);
	if (args.mouse == MB_R)
	{
		xui_method_ptrcall(m_rightmenu,	refresh			)();
		xui_method_ptrcall(m_setval,	set_data		)((void*)m_downindex);
		xui_method_ptrcall(m_delete,	set_data		)((void*)m_downindex);

		xui_window* window = get_window();
		xui_method_ptrcall(m_rightmenu, set_renderpt	)(args.point);
		xui_method_ptrcall(m_rightmenu, set_showsubmenu	)(NULL);
		xui_method_ptrcall(m_rightmenu, req_focus		)();
		xui_desktop::get_ins()->set_floatctrl(window, m_rightmenu);
	}

	if (m_downindex != -1)
	{
		xui_vector<s32> pt = get_renderpt(args.point);
		switch (m_flowstyle)
		{
		case FLOWSTYLE_H:
			if (pt.x < 0 || pt.x > get_renderw())
				m_linearray.erase(m_linearray.begin()+m_downindex);
			break;
		case FLOWSTYLE_V:
			if (pt.y < 0 || pt.y > get_renderh())
				m_linearray.erase(m_linearray.begin()+m_downindex);
			break;
		}

		m_downindex  = -1;
	}
}
xui_method_explain(cocos_gradpane, on_mousedoubleclick, void					)( xui_method_mouse& args )
{
	xui_drawer::on_mousedoubleclick(args);
	xui_vector<s32> pt = get_renderpt(args.point);
	s32 line = 0;
	switch (m_flowstyle)
	{
	case FLOWSTYLE_H: 
		line = xui_round((pt.x-m_trans) / m_ratio); 
		break;
	case FLOWSTYLE_V: 
		line = xui_round((get_renderh()-pt.y-m_trans) / m_ratio); 
		break;
	}

	if (args.mouse == MB_L)
	{
		for (u32 i = 0; i < m_linearray.size(); ++i)
		{
			if (m_linearray[i] == line)
				return;
		}

		m_linearray.push_back(line);
	}
}
xui_method_explain(cocos_gradpane, on_renderself,		void					)( xui_method_args&  args )
{
	xui_drawer::on_renderself(args);

	s32 grap	= (s32)(ceil(20.0/m_ratio/5.0) * 5.0);
	s32 gradidx = -m_trans/m_ratio/grap - 1;
	s32 gradnum = 0;
	switch (m_flowstyle)
	{
	case FLOWSTYLE_H: 
		gradnum = (s32)(ceil((f64)get_renderw() / (grap * m_ratio))); 
		break;
	case FLOWSTYLE_V: 
		gradnum = (s32)(ceil((f64)get_renderh() / (grap * m_ratio))); 
		break;
	}

	xui_colour      color = get_vertexcolor();
	xui_vector<s32> pt	  = get_screenpt();

	//grad
	for (s32 i = 0; i < gradnum+1; ++i)
	{
		s32 grad	 = (gradidx+i) * grap;
		s32 position = xui_round(grad*m_ratio) + m_trans;
		s32 length	 = ((gradidx+i) % 5 == 0) ? 24 : 32;
		xui_vector<s32> p1;
		xui_vector<s32> p2;
		switch (m_flowstyle)
		{
		case FLOWSTYLE_H: 
			p1 = pt + xui_vector<s32>(position,		length);
			p2 = pt + xui_vector<s32>(position,		get_renderh());
			break;
		case FLOWSTYLE_V:
			p1 = pt + xui_vector<s32>(length,		get_renderh()-position);
			p2 = pt + xui_vector<s32>(get_renderw(),get_renderh()-position);
			break;
		}

		xui_convas::get_ins()->draw_line(p1, p2, color*xui_colour::black);

		if ((gradidx+i) % 5 != 0)
			continue;

		if (m_downindex != -1 &&
			m_linearray[m_downindex] > grad-grap &&
			m_linearray[m_downindex] < grad+grap)
			continue;

		f32 a;
		xui_vector<s32> tp;
		switch (m_flowstyle)
		{
		case FLOWSTYLE_H:
			a  = 0.0f;
			tp = pt + xui_vector<s32>(position+2,  2);
			break;
		case FLOWSTYLE_V:
			a  = xui_pi/2.0f;
			tp = pt + xui_vector<s32>(20, get_renderh()-position-2);
			break;
		}

		std::wstringstream text;
		text << grad;
		xui_convas::get_ins()->set_transform(a, xui_vector<f32>(1.0f), tp.to<f32>());
		xui_convas::get_ins()->draw_text(text.str(), m_textfont, xui_vector<s32>(0), m_textdraw);
		xui_convas::get_ins()->non_transform();
	}

	//line
	for (u32 i = 0; i < m_linearray.size(); ++i)
	{
		s32 line = m_linearray[i];
		s32 position = xui_round(line*m_ratio) + m_trans;
		xui_vector<s32> p1;
		xui_vector<s32> p2;
		switch (m_flowstyle)
		{
		case FLOWSTYLE_H: 
			p1 = pt + xui_vector<s32>(position,			24);
			p2 = pt + xui_vector<s32>(position,			get_renderh());
			break;
		case FLOWSTYLE_V:
			p1 = pt + xui_vector<s32>(24,				get_renderh()-position);
			p2 = pt + xui_vector<s32>(get_renderw(),	get_renderh()-position);
			break;
		}

		xui_convas::get_ins()->draw_line(p1, p2, color*xui_colour(1.0f, 0.0f, 1.0f, 1.0f));

		if (i != m_downindex)
			continue;

		f32 a;
		xui_vector<s32> tp;
		switch (m_flowstyle)
		{
		case FLOWSTYLE_H:
			a  = 0.0f;
			tp = pt + xui_vector<s32>(position+2,  2);
			break;
		case FLOWSTYLE_V:
			a  = xui_pi/2.0f;
			tp = pt + xui_vector<s32>(20, get_renderh()-position-2);
			break;
		}

		std::wstringstream text;
		text << line;
		xui_convas::get_ins()->set_transform(a, xui_vector<f32>(1.0f), tp.to<f32>());
		xui_convas::get_ins()->draw_text(text.str(), m_textfont, xui_vector<s32>(0), xui_family_render(xui_colour(1.0f, 0.0f, 1.0f, 1.0f)));
		xui_convas::get_ins()->non_transform();
	}
}

/*
//event
*/
xui_method_explain(cocos_gradpane, on_menuclick,		void					)( xui_component* sender, xui_method_args& args )
{
	if (sender == m_setval)
	{
		u32 index = (u32)m_setval->get_data();
		if (index < m_linearray.size())
		{
			cocos_setval_dialog* dialog = new cocos_setval_dialog;
			xui_desktop::get_ins()->add_child(dialog);
			dialog->set_value(index, m_linearray[index]);
			dialog->xm_accept += new xui_method_member<xui_method_args, cocos_gradpane>(this, &cocos_gradpane::on_acceptsetval);
		}
	}
	else
	if (sender == m_delete)
	{
		u32 index = (u32)m_delete->get_data();
		if (index < m_linearray.size())
			m_linearray.erase(m_linearray.begin()+index);
	}
	else
	if (sender == m_delall)
	{
		m_linearray.clear();
	}
}
xui_method_explain(cocos_gradpane, on_acceptsetval,		void					)( xui_component* sender, xui_method_args& args )
{
	cocos_setval_dialog* dialog = xui_dynamic_cast(cocos_setval_dialog, sender->get_window());
	u32 index = dialog->get_index();
	if (index < m_linearray.size())
	{
		m_linearray[index] = dialog->get_value();
	}
}