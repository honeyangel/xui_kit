#include "NP2DSParam.h"
#include "NP2DSStateCtrl.h"

#include "xui_desktop.h"
#include "xui_convas.h"
#include "xui_toggle.h"
#include "xui_menu.h"
#include "xui_menuitem.h"
#include "onity_param.h"
#include "onity_paramview.h"

xui_implement_rtti(onity_paramview, xui_control);

/*
//constructor
*/
xui_create_explain(onity_paramview)( void )
: xui_control(xui_vector<s32>(0))
, m_editfile(NULL)
{
	xui_menu* menu = xui_menu::create(60);
	m_bool	= menu->add_item(NULL, L"Bool" );
	m_int	= menu->add_item(NULL, L"Int"  );
	m_float = menu->add_item(NULL, L"Float");
	xui_method_ptrcall(m_bool,	xm_click		) += new xui_method_member<xui_method_args, onity_paramview>(this, &onity_paramview::on_menuitemclick);
	xui_method_ptrcall(m_int,	xm_click		) += new xui_method_member<xui_method_args, onity_paramview>(this, &onity_paramview::on_menuitemclick);
	xui_method_ptrcall(m_float, xm_click		) += new xui_method_member<xui_method_args, onity_paramview>(this, &onity_paramview::on_menuitemclick);

	m_title = new xui_drawer(xui_vector<s32>(140, 20));
	xui_method_ptrcall(m_title, set_parent		)(this);
	xui_method_ptrcall(m_title, ini_component	)(ALIGNHORZ_L, ALIGNVERT_T, 0);
	xui_method_ptrcall(m_title, ini_drawer		)(L"Parameters");

	m_add	= new xui_toggle(xui_vector<s32>( 16, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_add,	set_parent		)(this);
	xui_method_ptrcall(m_add,	ini_component	)(ALIGNHORZ_R, ALIGNVERT_T, 0);
	xui_method_ptrcall(m_add,	set_backcolor	)(xui_colour::white);
	xui_method_ptrcall(m_add,	set_movecolor	)(xui_button::default_downcolor);
	xui_method_ptrcall(m_add,	set_menu		)(menu);
	xui_method_ptrcall(m_add,	xm_renderself	) += new xui_method_member<xui_method_args, onity_paramview>(this, &onity_paramview::on_addrenderself);
	m_widgetvec.push_back(m_title);
	m_widgetvec.push_back(m_add);
	invalid();
}

/*
//method
*/
xui_method_explain(onity_paramview, set_editfile,		void			)( NP2DSStateCtrl* editfile )
{
	if (m_editfile != editfile)
	{
		m_editfile  = editfile;
		del_paramctrlall();

		if (m_editfile)
		{
			const NP2DSStateCtrl::ParamVec& vec = m_editfile->GetParamVec();
			for (u32 i = 0; i < vec.size(); ++i)
			{
				add_paramctrl(vec[i]);
			}
		}
	}
}
xui_method_explain(onity_paramview, add_paramctrl,		void			)( NP2DSParam* param )
{
	onity_param* paramctrl = new onity_param(param);
	paramctrl->set_parent(this);
	m_widgetvec.push_back(paramctrl);
	m_paramctrl.push_back(paramctrl);

	invalid();
}
xui_method_explain(onity_paramview, del_paramctrl,		void			)( NP2DSParam* param )
{
	for (u32 i = 0; i < m_paramctrl.size(); ++i)
	{
		onity_param* paramctrl = m_paramctrl[i];
		if (paramctrl->get_param() == param)
		{
			std::vector<xui_component*>::iterator itor = std::find(
				m_widgetvec.begin(), 
				m_widgetvec.end(), 
				paramctrl);

			if (itor != m_widgetvec.end())
			{
				m_widgetvec.erase(itor);
				paramctrl->set_parent(NULL);
				xui_desktop::get_ins()->move_recycle(paramctrl);
			}

			m_paramctrl.erase(m_paramctrl.begin()+i);
			invalid();
			break;
		}
	}
}
xui_method_explain(onity_paramview, del_paramctrlall,	void			)( void )
{
	for (u32 i = 0; i < m_paramctrl.size(); ++i)
	{
		onity_param* paramctrl = m_paramctrl[i];
		std::vector<xui_component*>::iterator itor = std::find(
			m_widgetvec.begin(), 
			m_widgetvec.end(), 
			paramctrl);

		if (itor != m_widgetvec.end())
		{
			m_widgetvec.erase(itor);
			paramctrl->set_parent(NULL);
			xui_desktop::get_ins()->move_recycle(paramctrl);
		}
	}

	m_paramctrl.clear();
	invalid();
}

/*
//virtual
*/
xui_method_explain(onity_paramview, get_cornerrt,		xui_rect2d<s32>	)( void ) const
{
	return xui_rect2d<s32>(0, m_corner, 0, 0);
}

/*
//callback
*/
xui_method_explain(onity_paramview, on_invalid,			void			)( xui_method_args& args )
{
	xui_vector<s32> size(onity_param::default_width, 0);
	size.w += m_border.ax + m_border.bx;
	size.h += m_border.ay + m_border.by;
	size.h += m_title->get_renderh();
	size.h += m_paramctrl.size() * onity_param::default_height;

	if (get_rendersz() != size)
	{
		set_rendersz(size);
	}
	else
	{
		perform();
	}
}
xui_method_explain(onity_paramview, on_perform,			void			)( xui_method_args& args )
{
	xui_control::on_perform(args);
	for (u32 i = 0; i < m_paramctrl.size(); ++i)
	{
		xui_control* paramctrl = m_paramctrl[i];
		paramctrl->on_perform_x(m_border.ax);
		paramctrl->on_perform_y(m_border.ay+m_title->get_renderh()+i*onity_param::default_height);
	}
}

/*
//event
*/
xui_method_explain(onity_paramview, on_menuitemclick,	void			)( xui_component* sender, xui_method_args& args )
{
	if (m_editfile)
	{
		npu08 type;
		if		(sender == m_bool ) type = DT_BOOL ;
		else if (sender == m_int  ) type = DT_INT  ;
		else if (sender == m_float) type = DT_FLOAT;

		std::stringstream temp("New Param");
		if (m_editfile->HasParam(temp.str()))
		{
			s32 number = 0;
			while (true)
			{
				temp.str("");
				temp << "New Param";
				temp << number;
				if (m_editfile->HasParam(temp.str()) == false)
					break;

				++number;
			}
		}

		add_paramctrl(m_editfile->AddParam(temp.str(), type));
	}
}
xui_method_explain(onity_paramview, on_addrenderself,	void			)( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt		= sender->get_renderrtabs();
	xui_colour		color   = sender->get_rendercolor() * sender->get_vertexcolor();
	xui_vector<s32> center	= xui_vector<s32>(rt.ax + rt.get_w()/2, rt.ay + rt.get_h()/2);

	xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
		center.x-4,
		center.y-1,
		center.x+4,
		center.y+1), color);
	xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
		center.x-1,
		center.y-4,
		center.x+1,
		center.y+4), color);
}