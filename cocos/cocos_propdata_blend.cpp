#include "2d/CCSprite.h"

#include "xui_linebox.h"
#include "xui_button.h"
#include "xui_propkind.h"
#include "xui_propedit.h"
#include "xui_propview.h"
#include "cocos_propdata_blend.h"

/*
//constructor
*/
xui_create_explain(cocos_propdata_blend)( xui_propkind* kind, getfunc userget, setfunc userset, void* userptr )
: xui_propdata(kind, L"Blend", cocos_propctrl_blend::create)
, m_userget(userget)
, m_userset(userset)
, m_userptr(userptr)
{}

/*
//method
*/
xui_method_explain(cocos_propdata_blend, get_value,			cocos_blend_value	)( void )
{
	return (*m_userget)(m_userptr);
}
xui_method_explain(cocos_propdata_blend, set_value,			void				)( const cocos_blend_value& value )
{
	if (get_value() != value)
	{
		(*m_userset)(m_userptr, value);
		on_valuechanged();
	}
}

/*
//override
*/
xui_method_explain(cocos_propdata_blend, do_serialize,		u08*				)( void )
{
	return get_byte<cocos_blend_value>(get_value());
}
xui_method_explain(cocos_propdata_blend, un_serialize,		void				)( u08* byte )
{
	(*m_userset)(m_userptr, get_cast<cocos_blend_value>(byte));
}

//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(cocos_propctrl_blend, xui_propctrl);

std::map<s32, std::wstring>	blend_textmap = {
	{ GL_ZERO,					L"GL_ZERO"					},
	{ GL_ONE,					L"GL_ONE"					},
	{ GL_SRC_COLOR,				L"GL_SRC_COLOR"				},
	{ GL_ONE_MINUS_SRC_COLOR,	L"GL_ONE_MINUS_SRC_COLOR"	},
	{ GL_SRC_ALPHA,				L"GL_SRC_ALPHA"				},
	{ GL_ONE_MINUS_SRC_ALPHA,	L"GL_ONE_MINUS_SRC_ALPHA"	},
	{ GL_DST_COLOR,				L"GL_DST_COLOR"				},
	{ GL_ONE_MINUS_DST_COLOR,	L"GL_ONE_MINUS_DST_COLOR"	},
	{ GL_DST_ALPHA,				L"GL_DST_ALPHA"				},
	{ GL_ONE_MINUS_DST_ALPHA,	L"GL_ONE_MINUS_DST_ALPHA"	},
	{ GL_SRC_ALPHA_SATURATE,	L"GL_SRC_ALPHA_SATURATE"	}
};

/*
//constructor
*/
xui_create_explain(cocos_propctrl_blend)( xui_propdata* propdata )
: xui_propctrl()
{
	m_srcedit = new xui_propedit_enum(this, blend_textmap);
	m_dstedit = new xui_propedit_enum(this, blend_textmap);
	xui_drawer*		srcname = m_srcedit->get_namectrl();
	xui_control*	srcedit = m_srcedit->get_editctrl();
	xui_drawer*		dstname = m_dstedit->get_namectrl();
	xui_control*	dstedit = m_dstedit->get_editctrl();
	srcname->set_text(L"BlendSrc");
	dstname->set_text(L"BlendDst");
	srcname->set_parent(this);
	srcedit->set_parent(this);
	dstname->set_parent(this);
	dstedit->set_parent(this);
	m_widgetvec.push_back(srcname);
	m_widgetvec.push_back(srcedit);
	m_widgetvec.push_back(dstname);
	m_widgetvec.push_back(dstedit);

	m_linedef = new xui_button(xui_vector<s32>(60, 18));
	m_lineadd = new xui_button(xui_vector<s32>(60, 18));
	xui_method_ptrcall(m_linedef, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_linedef, set_drawcolor	)(true);
	xui_method_ptrcall(m_linedef, set_corner	)(3);
	xui_method_ptrcall(m_linedef, set_textalign	)(TEXTALIGN_CC);
	xui_method_ptrcall(m_linedef, ini_drawer	)(L"Normal");
	xui_method_ptrcall(m_lineadd, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_lineadd, set_drawcolor	)(true);
	xui_method_ptrcall(m_lineadd, set_corner	)(3);
	xui_method_ptrcall(m_lineadd, set_textalign	)(TEXTALIGN_CC);
	xui_method_ptrcall(m_lineadd, ini_drawer	)(L"Additive");
	xui_method_ptrcall(m_linedef, xm_buttonclick) += new xui_method_member<xui_method_args, cocos_propctrl_blend>(this, &cocos_propctrl_blend::on_buttonclick);
	xui_method_ptrcall(m_lineadd, xm_buttonclick) += new xui_method_member<xui_method_args, cocos_propctrl_blend>(this, &cocos_propctrl_blend::on_buttonclick);
	m_linebox = new xui_linebox(xui_vector<s32>(100, 18));
	xui_method_ptrcall(m_linebox, set_parent	)(this);
	xui_method_ptrcall(m_linebox, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_linebox, set_corner	)(3);
	xui_method_ptrcall(m_linebox, add_linectrl	)(m_linedef);
	xui_method_ptrcall(m_linebox, add_linectrl	)(m_lineadd);
	m_widgetvec.push_back(m_linebox);
}

/*
//destructor
*/
xui_delete_explain(cocos_propctrl_blend)( void )
{
	delete m_srcedit;
	delete m_dstedit;
}

/*
//create
*/
xui_method_explain(cocos_propctrl_blend, create,			xui_propctrl*	)( xui_propdata* propdata )
{
	return new cocos_propctrl_blend(propdata);
}

/*
//propdata
*/
xui_method_explain(cocos_propctrl_blend, on_linkpropdata,	void			)( bool selfupdate /* = false */ )
{
	if (selfupdate == false)
	{
		m_srcedit->reset();
		m_dstedit->reset();
	}

	bool same = true;
	cocos_propdata_blend* datablend = dynamic_cast<cocos_propdata_blend*>(m_propdata);
	cocos_blend_value value = datablend->get_value();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		cocos_propdata_blend* data = dynamic_cast<cocos_propdata_blend*>(m_propdatavec[i]);
		if (data->get_value() != value)
		{
			same = false;
			break;
		}
	}

	if (same)
	{
		m_srcedit->set_value(get_valueindex(value.src));
		m_dstedit->set_value(get_valueindex(value.dst));
	}
}
xui_method_explain(cocos_propctrl_blend, on_editvalue,		void			)( xui_propedit* sender )
{
	cocos_propdata_blend* datablend = dynamic_cast<cocos_propdata_blend*>(m_propdata);
	cocos_blend_value value = datablend->get_value();
	if		(sender == m_srcedit) value.src = get_indexvalue(m_srcedit->get_value());
	else if (sender == m_dstedit) value.dst = get_indexvalue(m_dstedit->get_value());

	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		cocos_propdata_blend* data = dynamic_cast<cocos_propdata_blend*>(m_propdatavec[i]);
		data->set_value(value);
	}
}

/*
//override
*/
xui_method_explain(cocos_propctrl_blend, on_invalid,		void			)( xui_method_args& args )
{
	xui_propview* propview = get_propview();
	xui_vector<s32> sz;
	sz.w = get_renderw();
	sz.h = xui_propview::default_lineheight*3 + m_border.ay + m_border.by;

	if (get_rendersz() != sz)
	{
		set_rendersz(sz);
	}
	else
	{
		perform();
	}
}
xui_method_explain(cocos_propctrl_blend, on_perform,		void			)( xui_method_args& args )
{
	xui_propctrl::on_perform(args);

	s32 indent = get_indent();
	s32 height = xui_propview::default_lineheight;

	xui_drawer*  srcname = m_srcedit->get_namectrl();
	xui_control* srcedit = m_srcedit->get_editctrl();
	xui_drawer*  dstname = m_dstedit->get_namectrl();
	xui_control* dstedit = m_dstedit->get_editctrl();

	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt;
	xui_vector<s32> sz;

	//srcname
	pt.x = rt.ax;
	pt.y = rt.ay;
	sz.w = rt.get_w()/2;
	sz.h = height;
	srcname->on_perform_pt(pt);
	srcname->on_perform_sz(sz);
	srcname->set_textoffset(xui_vector<s32>(indent, 0));
	//dstname
	pt.y = rt.ay+height;
	dstname->on_perform_pt(pt);
	dstname->on_perform_sz(sz);
	dstname->set_textoffset(xui_vector<s32>(indent, 0));
	//srcedit
	pt.x = rt.get_w()/2;
	pt.y = rt.ay + height/2 -   srcedit->get_renderh()/2;
	srcedit->on_perform_pt(pt);
	srcedit->on_perform_w (rt.get_w()/2);
	//dstedit
	pt.y = rt.ay + height/2 -   dstedit->get_renderh()/2 + height;
	dstedit->on_perform_pt(pt);
	dstedit->on_perform_w (rt.get_w()/2);
	//linebox
	pt.y = rt.ay + height/2 - m_linebox->get_renderh()/2 + height*2;
	xui_method_ptrcall(m_linebox, on_perform_pt)(pt);
}

/*
//event
*/
xui_method_explain(cocos_propctrl_blend, on_buttonclick,	void			)( xui_component* sender, xui_method_args& args )
{
	cocos_blend_value value;
	if		(sender == m_linedef)	value = cocos_blend_value(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	else if (sender == m_lineadd)	value = cocos_blend_value(GL_SRC_ALPHA, GL_ONE);

	on_readyundo();

	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		cocos_propdata_blend* data = dynamic_cast<cocos_propdata_blend*>(m_propdatavec[i]);
		data->set_value(value);
	}

	m_srcedit->set_value(get_valueindex(value.src));
	m_dstedit->set_value(get_valueindex(value.dst));
}

/*
//method
*/
xui_method_explain(cocos_propctrl_blend, get_valueindex,	u32				)( s32 value )
{
	u32 index = 0;
	for (xui_propenum_map::iterator itor = blend_textmap.begin(); itor != blend_textmap.end(); ++itor)
	{
		if ((*itor).first == value)
			break;

		++index;
	}

	return index;
}
xui_method_explain(cocos_propctrl_blend, get_indexvalue,	s32				)( u32 index )
{
	xui_propenum_map::iterator itor = blend_textmap.begin();
	std::advance(itor, index);
	return (*itor).first;
}