#include "NPRender.h"
#include "NP2DSRenderStep.h"
#include "NP2DSActor.h"
#include "NP2DSLayer.h"
#include "NP2DSFrameKey.h"
#include "NP2DSImageRef.h"
#include "NP2DSFrameRef.h"

#include "xui_global.h"
#include "xui_convas.h"
#include "xui_scroll.h"
#include "onity_renderview.h"
#include "onity_proplayer.h"
#include "onity_propactor.h"
#include "onity_lineview.h"

xui_implement_rtti(onity_lineview, xui_control);

/*
//constructor
*/
xui_create_explain(onity_lineview)( void )
: xui_control(xui_vector<s32>(150))
, m_viewprop(NULL)
, m_selected(-1)
{
	m_drawview = new onity_renderview(xui_vector<s32>(128), xui_vector<s32>(2048, 128));
	xui_method_ptrcall(m_drawview, xm_invalid			) += new xui_method_member<xui_method_args,		onity_lineview>(this, &onity_lineview::on_drawviewinvalid);
	xui_method_ptrcall(m_drawview, xm_renderself		) += new xui_method_member<xui_method_args,		onity_lineview>(this, &onity_lineview::on_drawviewrenderself);
	xui_method_ptrcall(m_drawview, xm_mousewheel		) += new xui_method_member<xui_method_mouse,	onity_lineview>(this, &onity_lineview::on_drawviewmousewheel);
	xui_method_ptrcall(m_drawview, xm_mousedown			) += new xui_method_member<xui_method_mouse,	onity_lineview>(this, &onity_lineview::on_drawviewmousedown);
	xui_method_ptrcall(m_drawview, set_parent			)(this);
	xui_method_ptrcall(m_drawview, set_borderrt			)(xui_rect2d<s32>(20));
	xui_method_ptrcall(m_drawview, ini_component		)(0, 0, DOCKSTYLE_F);

	m_viewroll = xui_scroll::create(FLOWSTYLE_H);
	xui_method_ptrcall(m_viewroll, set_parent			)(this);
	xui_method_ptrcall(m_viewroll, ini_component		)(true, false);
	xui_method_ptrcall(m_viewroll, ini_component		)(0, 0, DOCKSTYLE_B);

	m_widgetvec.push_back(m_viewroll);
	m_widgetvec.push_back(m_drawview);
}

/*
//method
*/
xui_method_explain(onity_lineview, get_drawview,			onity_renderview*	)( void )
{
	return m_drawview;
}
xui_method_explain(onity_lineview, get_viewprop,			onity_propactor*	)( void )
{
	return m_viewprop;
}
xui_method_explain(onity_lineview, set_viewprop,			void				)( onity_propactor* viewprop )
{
	if (m_viewprop != viewprop)
	{
		m_viewprop  = viewprop;
		m_selected	= -1;
		xui_method_ptrcall(m_drawview, refresh	)();
		xui_method_ptrcall(m_viewroll, set_value)(0);
	}
}

/*
//event
*/
xui_method_explain(onity_lineview, on_drawviewinvalid,		void				)( xui_component* sender, xui_method_args& args )
{
	u32 count = 0;
	if (m_viewprop)
	{
		const xui_proproot_vec& layers = m_viewprop->get_layers();
		count = layers.size();
	}

	xui_rect2d<s32> rt = m_drawview->get_renderrtins();
	s32 w = count * (tile_size+horz_grap);
	if (w <= rt.get_w())
	{
		xui_method_ptrcall(m_viewroll, set_value	)(0);
		xui_method_ptrcall(m_viewroll, set_visible	)(false);
	}
	else
	{
		xui_method_ptrcall(m_viewroll, set_range	)(w - rt.get_w());
		xui_method_ptrcall(m_viewroll, set_visible	)(true);
	}

	refresh();
}
xui_method_explain(onity_lineview, on_drawviewrenderself,	void				)( xui_component* sender, xui_method_args& args )
{
	xui_convas::get_ins()->clear(xui_colour(1.0f, 0.25f));
	extern bool gInitCompleted;
	if (gInitCompleted == false)
		return;

	if (m_viewprop)
	{
		xui_rect2d<s32> rt = m_drawview->get_renderrtins();

		const xui_proproot_vec& layers = m_viewprop->get_layers();
		xui_rect2d<s32> drawrt;
		drawrt.ax = rt.ax - horz_grap;
		drawrt.ay = rt.ay;
		drawrt.set_w(layers.size() * (tile_size+horz_grap) + horz_grap);
		drawrt.set_h(tile_size);
		xui_convas::get_ins()->fill_round(drawrt, xui_colour(1.0f, 90.0f/255.0f), xui_rect2d<s32>(tile_size/10));

		for (u16 i = 0; i < layers.size(); ++i)
		{
			if (rt.ax + (s32)(i*(tile_size+horz_grap)) - m_viewroll->get_value() > rt.bx)
				break;

			draw_layer(i, rt.ax, rt.ay, dynamic_cast<onity_proplayer*>(layers[i]));
		}
	}

	xui_vector<s32> size = m_drawview->get_rendersz();
	NPRender::GetIns()->SetResolutionW(size.w);
	NPRender::GetIns()->SetResolutionH(size.h);
	NPRender::GetIns()->SetViewport(0, 0, size.w, size.h);
	NP2DSRenderStep::GetIns()->SetEntryLocalT(NPVector3::Zero);
	NP2DSRenderStep::GetIns()->SetEntryWorldS(NPVector3::PositiveOne);
	NP2DSRenderStep::GetIns()->RenderImmediate();
}
xui_method_explain(onity_lineview, on_drawviewmousewheel,	void				)( xui_component* sender, xui_method_mouse& args )
{
	if (m_viewroll->was_visible())
	{
		m_viewroll->set_value(m_viewroll->get_value()-args.wheel);
		args.handle = true;
	}
}
xui_method_explain(onity_lineview, on_drawviewmousedown,	void				)( xui_component* sender, xui_method_mouse& args )
{
	if (args.mouse == MB_L)
	{
		xui_rect2d<s32> rt = m_drawview->get_renderrtins();
		xui_vector<s32> pt = m_drawview->get_renderpt(args.point);
		m_selected = (pt.x-rt.ax+m_viewroll->get_value()) / (tile_size+horz_grap);
	}
}

/*
//method
*/
xui_method_explain(onity_lineview, draw_layer,				void				)( u16 index, s32 x, s32 y,   onity_proplayer* proplayer )
{
	xui_rect2d<s32> tilert;
	tilert.ax = x + index*(tile_size+horz_grap);
	tilert.ay = y - m_viewroll->get_value();
	tilert.set_w(tile_size);
	tilert.set_h(tile_size+name_size);

	draw_tile(tilert, proplayer);
	draw_name(tilert, xui_global::ascii_to_unicode(proplayer->get_layer()->GetName()), index);
}
xui_method_explain(onity_lineview, draw_tile,				void				)( const xui_rect2d<s32>& rt, onity_proplayer* proplayer )
{
	NP2DSLayer* layer = proplayer->get_layer();
	if (layer->GetFrameKeyCount() == 0)
		return;

	NP2DSFrameKey* frameKey = layer->GetFrameKeyList().front();
	NPRect bound = frameKey->GetTransRef()->GetWorldBounding();

	f32 sw = (f32)tile_size / (f32)bound.GetW();
	f32 sh = (f32)tile_size / (f32)bound.GetH();
	f32 s  = xui_min(sw, sh);
	if (s > 1.0f)
		s = 1.0f;

	NPVector3 scale = NPVector3(s, s, 1.0f);
	NPVector3 trans = NPVector3(
		(-bound.LT*s + (((f32)tile_size - bound.GetW()*s)) / 2.0f) + (f32)rt.ax, 
		(-bound.TP*s + (((f32)tile_size - bound.GetH()*s)) / 2.0f) + (f32)rt.ay,
		0.0f);

	trans.x = xui_pixel_align(trans.x);
	trans.y = xui_pixel_align(trans.y);
	frameKey->GetTransRef()->Render(scale, trans);
}
xui_method_explain(onity_lineview, draw_name,				void				)( const xui_rect2d<s32>& rt, const std::wstring& name, u16 index )
{
	xui_rect2d<s32> drawrt = rt;
	drawrt.ay += tile_size;
	drawrt.ay += 2;
	drawrt.by -= 2;

	if (m_selected == index)
		xui_convas::get_ins()->fill_round(drawrt, xui_colour(1.0f, 42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f), xui_rect2d<s32>(8));

	std::wstringstream temp;
	temp << index;
	temp << L"#";
	temp << name.c_str();
	std::wstring    text   = xui_convas::get_ins()->trim_text(temp.str(), xui_family::default, drawrt.get_w()-16);
	xui_rect2d<s32> textrt = xui_convas::get_ins()->calc_draw(text, xui_family::default, drawrt, TEXTALIGN_CC, true);
	xui_convas::get_ins()->draw_text(text, xui_family::default, textrt, xui_family_render::default, true);
}