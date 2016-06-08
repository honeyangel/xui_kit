#include "NPRender.h"
#include "NPSourceTexture.h"
#include "NP2DSImage.h"
#include "NP2DSTextureCache.h"
#include "NP2DSRenderUtil.h"
#include "NP2DSRenderStep.h"

#include "xui_convas.h"
#include "xui_propview.h"
#include "onity_renderview.h"
#include "onity_propctrl_texture.h"

xui_implement_rtti(onity_propctrl_texture, xui_propctrl_object);

/*
//constructor
*/
xui_create_explain(onity_propctrl_texture)( xui_propdata* propdata )
: xui_propctrl_object(propdata)
{
	m_viewctrl = new onity_renderview(xui_vector<s32>(64), xui_vector<s32>(64));
	xui_method_ptrcall(m_viewctrl, xm_renderself) += new xui_method_member<xui_method_args, onity_propctrl_texture>(this, &onity_propctrl_texture::on_viewctrlrenderself);
	xui_method_ptrcall(m_viewctrl, set_sidestyle)(SIDESTYLE_S);
	xui_method_ptrcall(m_viewctrl, set_parent	)(this);
	m_widgetvec.push_back(m_viewctrl);
}

/*
//static
*/
xui_method_explain(onity_propctrl_texture, create,					xui_propctrl*	)( xui_propdata* propdata )
{
	return new onity_propctrl_texture(propdata);
}

/*
//override
*/
xui_method_explain(onity_propctrl_texture, on_linkpropdata,			void			)( void )
{
	xui_propctrl_object::on_linkpropdata();
	refresh();
}

/*
//callback
*/
xui_method_explain(onity_propctrl_texture, on_invalid,				void			)( xui_method_args& args )
{
	xui_vector<s32> sz;
	sz.w = get_renderw();
	sz.h = xui_propview::default_lineheight + m_border.ay + m_border.by;

	if (m_propdata)
	{
		bool same = true;
		xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(m_propdata);
		void* value = dataobject->get_value();
		for (u32 i = 0; i < m_propdatavec.size(); ++i)
		{
			xui_propdata_object* data = dynamic_cast<xui_propdata_object*>(m_propdatavec[i]);
			if (data->get_value() != value)
			{
				same = false;
				break;
			}
		}

		m_viewctrl->set_visible(same && value != NULL);
		if (m_viewctrl->was_visible())
			sz.h += m_viewctrl->get_renderh() + 4;
	}

	if (get_rendersz() != sz)
	{
		set_rendersz(sz);
	}
	else
	{
		perform();
	}
}
xui_method_explain(onity_propctrl_texture, on_perform,				void			)( xui_method_args& args )
{
	xui_propctrl_object::on_perform(args);
	xui_rect2d<s32> rt = get_renderrtins();
	m_viewctrl->on_perform_x(rt.get_w()/3);
	m_viewctrl->on_perform_y(xui_propview::default_lineheight+2);
}

/*
//event
*/
xui_method_explain(onity_propctrl_texture, on_viewctrlrenderself,	void			)( xui_component* sender, xui_method_args& args )
{
	xui_convas::get_ins()->clear(xui_colour(1.0f, 90.0f/255.0f));
	extern bool gInitCompleted;
	if (gInitCompleted == false)
		return;

	xui_propdata_object* dataobject = dynamic_cast<xui_propdata_object*>(m_propdata);
	NPSourceTexture*     texture    = (NPSourceTexture*)dataobject->get_value();
	if (texture)
	{
		npu32 w = texture->GetWidth ();
		npu32 h = texture->GetHeight();
		npf32 s = (npf32)m_viewctrl->get_renderw() / xui_max(w, h);
		NPTransform t;
		t.SetIdentity();
		t.SetS(NPVector3(s, s, 1.0f));
		NP2DSImage image(-1, "", -1, 0, 0, (nps16)texture->GetPixelWidth(), (nps16)texture->GetPixelHeight(), true);
		NP2DSRenderUtil::GetIns()->DrawImage(
			&image, 
			t.GetMatrix(),
			NPColor::White, 
			0,
			0,
			0,
			0,
			texture->GetKey());
	}

	xui_vector<s32> size = m_viewctrl->get_rendersz();
	NPRender::GetIns()->SetResolutionW(size.w);
	NPRender::GetIns()->SetResolutionH(size.h);
	NPRender::GetIns()->SetViewport(0, 0, size.w, size.h);
	NP2DSRenderStep::GetIns()->SetEntryLocalT(NPVector3::Zero);
	NP2DSRenderStep::GetIns()->SetEntryWorldS(NPVector3::PositiveOne);
	NP2DSRenderStep::GetIns()->RenderImmediate();
}