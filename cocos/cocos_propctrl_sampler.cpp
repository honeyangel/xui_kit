#include "renderer/CCTexture2D.h"
#include "xui_propctrl_stdvec.h"
#include "xui_convas.h"
#include "xui_desktop.h"
#include "xui_global.h"
#include "xui_menu.h"
#include "xui_menuitem.h"
#include "xui_button.h"
#include "xui_toggle.h"
#include "xui_listview.h"
#include "xui_listitem.h"
#include "xui_numbbox.h"
#include "xui_dropbox.h"
#include "xui_propkind.h"
#include "cocos_propctrl_asset.h"
#include "cocos_selector.h"
#include "cocos_filedata.h"
#include "cocos_mainform.h"
#include "cocos_project.h"
#include "cocos_propctrl_sampler.h"
#include "cocos_propdata_texture.h"

class cocos_propdata_texture_impl : public xui_propdata_object_impl<cocos2d::Texture2D*>
{
public:
	/*
	//constructor
	*/
	cocos_propdata_texture_impl( xui_propkind* kind, const std::wstring& name, cocos2d::Texture2D**	ptr )
	: xui_propdata_object_impl<cocos2d::Texture2D*>(
		kind, 
		name, 
		cocos_propctrl_asset::create, 
		"Texture2D", 
		cocos_selector::get_ptr, 
		cocos_propdata_texture::get_icon, 
		cocos_propdata_texture::get_name, 
		ptr)
	{
		xm_doubleclick += new xui_method_member<xui_method_args, cocos_propdata_texture_impl>(this, &cocos_propdata_texture_impl::on_doubleclick);
	}

protected:
	/*
	//event
	*/
	void on_doubleclick( xui_component* sender, xui_method_args& args )
	{
		cocos2d::Texture2D* texture = (cocos2d::Texture2D*)get_value();
		if (texture)
		{
			std::wstring full = xui_global::ascii_to_unicode(texture->getFileName());
			std::wstring work = xui_global::get_workpath();
			if (full != L"dummy" && full.length() > work.length())
			{
				full.erase(0, work.length() + 1);
				std::wstring  path = cocos_filedata::get_path(full);
				std::wstring  file = cocos_filedata::get_file(full);
				path = path.substr(0, path.length() - 1);

				cocos_project* project = cocos_mainform::get_ptr()->get_project();
				project->loc_filenode(path, file, 0);
				cocos_mainform::get_ptr()->set_pageshow(project);
			}
		}
	}
};

//////////////////////////////////////////////////////////////////////////
//propdata
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(cocos_propdata_sampler)( xui_propkind* kind, const std::wstring& name, sampler_vec* vec )
: xui_propdata(kind, name, cocos_propctrl_sampler::create)
, m_samplervec(vec)
{
	for (u32 i = 0; i < m_samplervec->size(); ++i)
		add_samplerdata(&(*m_samplervec)[i]);
}

/*
//destructor
*/
xui_delete_explain(cocos_propdata_sampler)( void )
{
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
		delete m_propdatavec[i];
}

/*
//method
*/
xui_method_explain(cocos_propdata_sampler, get_samplerdatavec,	const xui_propdata_vec&	)( void ) const
{
	return m_propdatavec;
}
xui_method_explain(cocos_propdata_sampler, reset,				void					)( void )
{
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
	{
		m_propdatavec[i]->non_ctrl();
		delete m_propdatavec[i];
	}

	m_propdatavec.clear();
	for (u32 i = 0; i < m_samplervec->size(); ++i)
		add_samplerdata(&(*m_samplervec)[i]);

	if (m_ctrl)
		m_ctrl->on_linkpropdata();
}

/*
//override
*/
xui_method_explain(cocos_propdata_sampler, non_ctrl,			void					)( void )
{
	xui_propdata::non_ctrl();
	for (u32 i = 0; i < m_propdatavec.size(); ++i)
		m_propdatavec[i]->non_ctrl();
}

/*
//method
*/
xui_method_explain(cocos_propdata_sampler, add_samplerdata,		void					)( cocos_propmaterial::sampler* sampler_value )
{
	xui_propdata_vec subprop;
	subprop.push_back(new cocos_propdata_texture_impl(
		m_kind,
		L"Texture",
		&sampler_value->texture));
	subprop.push_back(new xui_propdata_bool(
		m_kind,
		L"Mipmap",
		xui_propctrl_bool::create,
		NULL,
		NULL,
		&sampler_value->mipmap));
	xui_propenum_map textmap;
	textmap[GL_REPEAT]			= L"REPEAT";
	textmap[GL_CLAMP_TO_EDGE]	= L"CLAMP_TO_EDGE";
	subprop.push_back(new xui_propdata_enum_impl<s32>(
		m_kind,
		L"WrapS",
		xui_propctrl_enum::create,
		textmap,
		&sampler_value->wraps));
	subprop.push_back(new xui_propdata_enum_impl<s32>(
		m_kind,
		L"WrapT",
		xui_propctrl_enum::create,
		textmap,
		&sampler_value->wrapt));
	textmap.clear();
	textmap[GL_NEAREST]					= L"NEAREST";
	textmap[GL_LINEAR]					= L"LINEAR";
	textmap[GL_NEAREST_MIPMAP_NEAREST]	= L"NEAREST_MIPMAP_NEAREST";
	textmap[GL_LINEAR_MIPMAP_NEAREST]	= L"LINEAR_MIPMAP_NEAREST";
	textmap[GL_NEAREST_MIPMAP_LINEAR]	= L"NEAREST_MIPMAP_LINEAR";
	textmap[GL_LINEAR_MIPMAP_LINEAR]	= L"LINEAR_MIPMAP_LINEAR";
	subprop.push_back(new xui_propdata_enum_impl<s32>(
		m_kind, 
		L"minfilter",
		xui_propctrl_enum::create,
		textmap,
		&sampler_value->minfilter));
	subprop.push_back(new xui_propdata_enum_impl<s32>(
		m_kind,
		L"magfilter",
		xui_propctrl_enum::create,
		textmap,
		&sampler_value->magfilter));

	m_propdatavec.push_back(new xui_propdata_expand(
		m_kind,
		xui_global::ascii_to_unicode(sampler_value->name),
		xui_propctrl_expand_plus::create,
		subprop));
}

//////////////////////////////////////////////////////////////////////////
//propctrl
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(cocos_propctrl_sampler, xui_propctrl);

/*
//constructor
*/
xui_create_explain(cocos_propctrl_sampler)( xui_propdata* propdata )
	: xui_propctrl()
{
	m_border = xui_rect2d<s32>(0, 8, 0, 8);
	m_header = new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_header, set_parent			)(this);
	xui_method_ptrcall(m_header, set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_header, set_borderrt		)(xui_rect2d<s32>(8, 2, 2, 2));
	xui_method_ptrcall(m_header, ini_drawer			)(propdata->get_name());
	m_nontip = new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_nontip, set_parent			)(this);
	xui_method_ptrcall(m_nontip, set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_nontip, set_borderrt		)(xui_rect2d<s32>(8, 2, 2, 2));
	xui_method_ptrcall(m_nontip, ini_component		)(true, false);
	xui_method_ptrcall(m_nontip, ini_drawer			)(L"Empty");

	m_middle = xui_listview::create(xui_vector<s32>(0), false);
	xui_method_ptrcall(m_middle, set_parent			)(this);
	xui_method_ptrcall(m_middle, set_drawcolor		)(false);
	xui_method_ptrcall(m_middle, set_sidestyle		)(SIDESTYLE_N);
	xui_method_ptrcall(m_middle, set_hscrollauto	)(false);
	xui_method_ptrcall(m_middle, xm_invalid			) += new xui_method_member<xui_method_args, cocos_propctrl_sampler>(this, &cocos_propctrl_sampler::on_middleinvalid);
	xui_method_ptrcall(m_middle, xm_selectedchange	) += new xui_method_member<xui_method_args, cocos_propctrl_sampler>(this, &cocos_propctrl_sampler::on_middleselection);

	m_expand = new xui_propctrl_expand_plus;
	xui_method_ptrcall(m_expand, set_parent			)(this);
	xui_method_ptrcall(m_expand, ini_component		)(true, false);

	m_widgetvec.push_back(m_header);
	m_widgetvec.push_back(m_middle);
	m_widgetvec.push_back(m_nontip);
	m_widgetvec.push_back(m_expand);
}

/*
//static
*/
xui_method_explain(cocos_propctrl_sampler, create,				xui_propctrl*			)( xui_propdata* propdata )
{
	return new cocos_propctrl_sampler(propdata);
}

/*
//override
*/
xui_method_explain(cocos_propctrl_sampler, on_linkpropdata,		void					)( bool selfupdate )
{
	cocos_propdata_sampler* datasampler = dynamic_cast<cocos_propdata_sampler*>(m_propdata);
	const xui_propdata_vec& datavec = datasampler->get_samplerdatavec();

	if (selfupdate == false || m_middle->get_itemcount() != datavec.size())
	{
		xui_method_ptrcall(m_middle, del_itemall)();
		for (u32 i = 0; i < datavec.size(); ++i)
		{
			xui_propdata* data = datavec[i];
			if (data)
			{
				xui_listitem * item = m_middle->add_item(data->get_name());
				item->set_data(data);
			}
		}
		xui_method_ptrcall(m_expand, ini_component)(true, false);
		xui_method_ptrcall(m_nontip, ini_component)(true, datavec.size() == 0);
		xui_method_ptrcall(m_middle, refresh)();
		refresh();
	}
}
xui_method_explain(cocos_propctrl_sampler, on_editvalue,		void					)( xui_propedit* sender )
{}

/*
//callback
*/
xui_method_explain(cocos_propctrl_sampler, on_renderback,		void					)( xui_method_args& args )
{
	xui_propctrl::on_renderback(args);

	xui_rect2d<s32> rt;
	xui_colour		color = get_vertexcolor();
	rt = m_header->get_renderrtabs();
	xui_convas::get_ins()->fill_round(rt, xui_colour(1.0f, 0.4f) * color, xui_rect2d<s32>(3, 3, 0, 0));
	rt = m_middle->get_renderrtabs();
	xui_convas::get_ins()->fill_round(rt, xui_colour(1.0f, 0.3f) * color, xui_rect2d<s32>(0, 0, 0, 3));
	//rt = m_insert->get_renderrtabs().get_union(m_delete->get_renderrtabs());
	//xui_convas::get_ins()->fill_round(rt, xui_colour(1.0f, 0.3f) * color, xui_rect2d<s32>(0, 0, 3, 3));

	color *= m_sidecolor;
	rt = m_header->get_renderrtabs();
	xui_convas::get_ins()->draw_round(rt, color, xui_rect2d<s32>(3, 3, 0, 0));
	rt = m_middle->get_renderrtabs();
	xui_convas::get_ins()->draw_round(rt, color, xui_rect2d<s32>(0, 0, 3, 3));
	//xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax, rt.by - 3), xui_vector<s32>(rt.ax, rt.ay), color);
	//xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx - 48, rt.by), xui_vector<s32>(rt.ax + 3, rt.by), color);
	//xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx, rt.ay), xui_vector<s32>(rt.bx, rt.by), color);
	//xui_convas::get_ins()->draw_arc(xui_rect2d<s32>(
	//	rt.ax,
	//	rt.by - 6,
	//	rt.ax + 6,
	//	rt.by), color, 90, 90, 1);

	//rt = m_insert->get_renderrtabs().get_union(m_delete->get_renderrtabs());
	//xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx, rt.ay), xui_vector<s32>(rt.bx, rt.by - 3), color);
	//xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx - 3, rt.by), xui_vector<s32>(rt.ax + 3, rt.by), color);
	//xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax, rt.by - 3), xui_vector<s32>(rt.ax, rt.ay), color);
	//xui_convas::get_ins()->draw_arc(xui_rect2d<s32>(
	//	rt.ax,
	//	rt.by - 6,
	//	rt.ax + 6,
	//	rt.by), color, 90, 90, 1);
	//xui_convas::get_ins()->draw_arc(xui_rect2d<s32>(
	//	rt.bx - 6,
	//	rt.by - 6,
	//	rt.bx,
	//	rt.by), color, 0, 90, 1);
}
xui_method_explain(cocos_propctrl_sampler, on_invalid,			void					)( xui_method_args& args )
{
	xui_vector<s32> sz;
	sz.w = get_renderw();
	sz.h = m_border.ay + m_border.by;
	sz.h += m_header->get_renderh();
	sz.h += m_middle->get_renderh();
	sz.h += m_expand->was_visible() ? m_expand->get_renderh() : 0;
	if (get_rendersz() != sz)
	{
		set_rendersz(sz);
	}
	else
	{
		perform();
	}
}
xui_method_explain(cocos_propctrl_sampler, on_perform,			void					)( xui_method_args& args )
{
	xui_propctrl::on_perform(args);

	s32 indent = get_indent();
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt = rt.get_pt();
	//header
	m_header->on_perform_x(pt.x + indent);
	m_header->on_perform_y(pt.y);
	m_header->on_perform_w(rt.get_w() - indent);
	//middle
	pt.y += m_header->get_renderh();
	m_middle->on_perform_x(pt.x + indent);
	m_middle->on_perform_y(pt.y);
	m_middle->on_perform_w(rt.get_w() - indent);
	m_nontip->on_perform_x(pt.x + indent);
	m_nontip->on_perform_y(pt.y + m_middle->get_borderrt().ay);
	m_nontip->on_perform_w(rt.get_w() - indent);
	//expand
	pt.x = rt.ax;
	pt.y += m_middle->get_renderh();
	m_expand->on_perform_x(pt.x);
	m_expand->on_perform_y(pt.y);
	m_expand->on_perform_w(rt.get_w());
}

/*
//event
*/
xui_method_explain(cocos_propctrl_sampler, on_middleinvalid,	void					)( xui_component* sender, xui_method_args& args )
{
	s32 h = (m_middle->get_itemcount() == 0) ? m_nontip->get_renderh() : (m_middle->get_itemcount() * m_middle->get_lineheight());
	h += m_middle->get_borderrt().ay;
	h += m_middle->get_borderrt().by;
	m_middle->set_renderh(h);
}
xui_method_explain(cocos_propctrl_sampler, on_middleselection,	void					)( xui_component* sender, xui_method_args& args )
{
	xui_propdata_vec datavec = m_expand->get_propdata();
	for (u32 i = 0; i < datavec.size(); ++i)
		datavec[i]->non_ctrl();

	std::vector<xui_listitem*> itemvec = m_middle->get_selecteditem();
	if (itemvec.size() > 0)
	{
		xui_listitem* item = itemvec.front();
		xui_propdata* data = (xui_propdata*)item->get_data();
		m_expand->set_propdata(data);
		m_expand->refresh();

		data->set_ctrl(m_expand);
	}

	m_expand->set_visible(itemvec.size() > 0);
	refresh();
}
