#include "NP2DSAsset.h"
#include "NP2DSAssetFile.h"
#include "NP2DSActor.h"
#include "NP2DSLayer.h"
#include "NP2DSFrameKey.h"
#include "NP2DSFrameRef.h"
#include "NP2DSActorRef.h"
#include "NPParticleSFX.h"
#include "NPRender.h"
#include "NP2DSRenderStep.h"

#include "xui_global.h"
#include "xui_convas.h"
#include "xui_panel.h"
#include "xui_button.h"
#include "xui_slider.h"
#include "onity_filedata.h"
#include "onity_propactor.h"
#include "onity_propframe.h"
#include "onity_propparticle.h"
#include "onity_renderview.h"
#include "onity_resource.h"
#include "onity_preview.h"

xui_implement_rtti(onity_preview, xui_control);

/*
//constructor
*/
xui_create_explain(onity_preview)( void )
: xui_control(xui_vector<s32>(260))
, m_node(NULL)
, m_scale(1.0f)
{
	m_play	= new xui_button(xui_vector<s32>(24));
	xui_method_ptrcall(m_play,	ini_component	)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_play,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_play,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_play,	ini_drawer		)(onity_resource::icon_play, xui_vector<s32>(16));

	xui_method_ptrcall(m_play,	xm_buttonclick	) += new xui_method_member<xui_method_args,  onity_preview>(this, &onity_preview::on_buttonclick);
	m_speed	= xui_slider::create(FLOWSTYLE_H);
	xui_method_ptrcall(m_speed,	ini_component	)(ALIGNHORZ_R, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_speed,	ini_scroll		)(16, 8);
	xui_method_ptrcall(m_speed,	xm_scroll		) += new xui_method_member<xui_method_args,  onity_preview>(this, &onity_preview::on_speedscroll);
	m_text	= new xui_drawer(xui_vector<s32>(32, 24));
	xui_method_ptrcall(m_text,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_text,	set_textalign	)(TEXTALIGN_RC);
	xui_method_ptrcall(m_text,	ini_drawer		)(L"1.0x");

	m_head	= new xui_panel(xui_vector<s32>(200, 24));
	xui_method_ptrcall(m_head,	set_parent		)(this);
	xui_method_ptrcall(m_head,	set_borderrt	)(xui_rect2d<s32>(2, 0, 0, 0));
	xui_method_ptrcall(m_head,	set_drawcolor	)(false);
	xui_method_ptrcall(m_head,	ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_head,	add_child		)(m_play);
	xui_method_ptrcall(m_head,	add_child		)(m_text);
	xui_method_ptrcall(m_head,	add_child		)(m_speed);
	xui_method_ptrcall(m_head,	xm_perform		) += new xui_method_member<xui_method_args,  onity_preview>(this, &onity_preview::on_headperform);

	m_small	= new xui_button(xui_vector<s32>(24));
	xui_method_ptrcall(m_small,	ini_component	)(0, 0, DOCKSTYLE_R);
	xui_method_ptrcall(m_small,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_small,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_small,	ini_drawer		)(onity_resource::icon_small, xui_vector<s32>(16));
	xui_method_ptrcall(m_small,	xm_buttonclick	) += new xui_method_member<xui_method_args,  onity_preview>(this, &onity_preview::on_buttonclick);
	m_large	= new xui_button(xui_vector<s32>(24));
	xui_method_ptrcall(m_large,	ini_component	)(0, 0, DOCKSTYLE_R);
	xui_method_ptrcall(m_large,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_large,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_large,	ini_drawer		)(onity_resource::icon_large, xui_vector<s32>(16));
	xui_method_ptrcall(m_large,	xm_buttonclick	) += new xui_method_member<xui_method_args,  onity_preview>(this, &onity_preview::on_buttonclick);
	m_reset	= new xui_button(xui_vector<s32>(24));
	xui_method_ptrcall(m_reset,	ini_component	)(0, 0, DOCKSTYLE_R);
	xui_method_ptrcall(m_reset,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_reset,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_reset,	ini_drawer		)(onity_resource::icon_reset, xui_vector<s32>(16));
	xui_method_ptrcall(m_reset,	xm_buttonclick	) += new xui_method_member<xui_method_args,  onity_preview>(this, &onity_preview::on_buttonclick);

	m_tool	= new xui_panel(xui_vector<s32>(200, 24));
	xui_method_ptrcall(m_tool,	set_parent		)(this);
	xui_method_ptrcall(m_tool,	set_drawcolor	)(false);
	xui_method_ptrcall(m_tool,	set_borderrt	)(xui_rect2d<s32>(0, 0, 0, 2));
	xui_method_ptrcall(m_tool,	ini_component	)(0, 0, DOCKSTYLE_B);
	xui_method_ptrcall(m_tool,	add_child		)(m_small);
	xui_method_ptrcall(m_tool,	add_child		)(m_large);
	xui_method_ptrcall(m_tool,	add_child		)(m_reset);

	m_view	= new onity_renderview(xui_vector<s32>(200), xui_vector<s32>(512));
	xui_method_ptrcall(m_view,	set_parent		)(this);
	xui_method_ptrcall(m_view,	ini_component	)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_view,	xm_updateself	) += new xui_method_member<xui_method_update, onity_preview>(this, &onity_preview::on_viewupdateself);
	xui_method_ptrcall(m_view,	xm_renderself	) += new xui_method_member<xui_method_args,   onity_preview>(this, &onity_preview::on_viewrenderself);
	xui_method_ptrcall(m_view,	xm_mousewheel	) += new xui_method_member<xui_method_mouse,  onity_preview>(this, &onity_preview::on_viewmousewheel);

	m_widgetvec.push_back(m_head);
	m_widgetvec.push_back(m_tool);
	m_widgetvec.push_back(m_view);
}

/*
//destructor
*/
xui_delete_explain(onity_preview)( void )
{
	delete m_node;
}

/*
//method
*/
xui_method_explain(onity_preview, get_drawnode,			NPNode*				)( void )
{
	return m_node;
}
xui_method_explain(onity_preview, get_drawview,			onity_renderview*	)( void )
{
	return m_view;
}
xui_method_explain(onity_preview, set_toolshow,			void				)( bool flag )
{
	m_head->set_visible(flag);
	m_tool->set_visible(flag);
	refresh();
}
xui_method_explain(onity_preview, set_needshow,			void				)( void )
{
	set_visible(m_node != NULL);
}
xui_method_explain(onity_preview, set_viewprop,			void				)( xui_proproot* prop, bool play )
{
	if (m_node)
	{
		if (NPIsExaKindOf(NPParticleSFX, m_node) == false)
			delete m_node;

		m_node = NULL;
	}

	onity_propparticle* propparticle = dynamic_cast<onity_propparticle*>(prop);
	onity_propframe*    propframe    = dynamic_cast<onity_propframe*   >(prop);
	onity_propactor*    propactor    = dynamic_cast<onity_propactor*   >(prop);
	if (propparticle && propparticle->get_particle())
	{
		NPParticleSFX* particle = propparticle->get_particle();
		particle->SetFlag(PARTICLESFX_LOOPPLAY, true);
		particle->Active();
		m_node = particle;
	}
	else
	if (propframe)
	{
		NP2DSFrameRef* frameref = new NP2DSFrameRef;
		NP2DSAsset* asset = propframe->get_asset();
		frameref->SetFrame(asset->GetOwnedFile()->GetKey(), asset->GetKey());
		m_node = frameref;
	}
	else
	if (propactor)
	{
		NP2DSActorRef* actorref = new NP2DSActorRef;
		NP2DSAsset* asset = propactor->get_asset();
		actorref->SetActor(asset->GetOwnedFile()->GetKey(), asset->GetKey());
		actorref->SetPlay(play);
		m_node = actorref;
	}

	m_scale = 1.0f;
	m_rect	= xui_rect2d<s32>(0);
}

xui_method_explain(onity_preview, set_drawrect,			void				)( const xui_rect2d<s32>& rect )
{
	m_rect = rect;
}

/*
//event
*/
xui_method_explain(onity_preview, on_buttonclick,		void				)( xui_component* sender, xui_method_args&   args )
{
	if (sender == m_play)
	{
		NP2DSActorRef* actorref = NPDynamicCast(NP2DSActorRef, m_node);
		NPParticleSFX* particle = NPDynamicCast(NPParticleSFX, m_node);
		if (actorref)
		{
			actorref->Reset();
			actorref->SetPlay(true);
		}
		if (particle)
		{
			particle->Finish(true);
			particle->Active();
		}
	}
	else
	if (sender == m_large || sender == m_small)
	{
		if (sender == m_small && m_scale > 0.1f)
		{
			if (m_scale <= 1.0f)	m_scale -= 0.1f;
			else					m_scale -= 1.0f;
		}
		if (sender == m_large && m_scale < 5.0f)
		{
			if (m_scale <= 1.0f)	m_scale += 0.1f;
			else					m_scale += 1.0f;
		}
	}
	else
	if (sender == m_reset)
	{
		m_scale = 1.0f;
	}
}
xui_method_explain(onity_preview, on_speedscroll,		void				)( xui_component* sender, xui_method_args&   args )
{
	f32 speed = get_speed();
	char temp[32];
	sprintf(temp, "%3.1fx", speed);
	m_text->set_text(xui_global::ascii_to_unicode(temp));
}
xui_method_explain(onity_preview, on_headperform,		void				)( xui_component* sender, xui_method_args&	 args )
{
	m_text->on_perform_x(m_speed->get_renderx()-m_text->get_renderw());
}
xui_method_explain(onity_preview, on_viewupdateself,	void				)( xui_component* sender, xui_method_update& args )
{
	if (m_node)
		m_node->Update(args.delta * get_speed());
}
xui_method_explain(onity_preview, on_viewrenderself,	void				)( xui_component* sender, xui_method_args&   args )
{
	xui_convas::get_ins()->clear(get_backcolor());
	extern bool gInitCompleted;
	if (gInitCompleted == false)
		return;

	xui_vector<s32> size = m_view->get_rendersz();
	if (m_node)
	{
		set_localtransform();

		NPVector3 scale = NPVector3(m_scale, m_scale, 1.0f);
		NPVector3 trans = NPVector3(
			((f32)size.w - size.w*m_scale) / 2.0f, 
			((f32)size.h - size.h*m_scale) / 2.0f,
			0.0f);

		trans.x = xui_pixel_align(trans.x);
		trans.y = xui_pixel_align(trans.y);
		NP2DSTransRef* transref = NPDynamicCast(NP2DSTransRef, m_node);
		NPParticleSFX* particle = NPDynamicCast(NPParticleSFX, m_node);
		if (transref)
		{
			xui_method_ptrcall(transref, SetWorldScale	)(scale);
			xui_method_ptrcall(transref, SetWorldTrans	)(trans);
		}
		if (particle)
		{
			xui_method_ptrcall(particle, SetWorldS		)(scale);
			xui_method_ptrcall(particle, SetWorldT		)(trans);
		}

		m_node->Render();

		if (m_rect.was_valid())
		{
			NP2DSRenderUtil::GetIns()->DrawLineBox(
				NPVector2((npf32)m_rect.ax, (npf32)m_rect.ay), 
				NPVector2((npf32)m_rect.bx, (npf32)m_rect.by), 
				m_node->GetFinalMatrix(),
				NPColor::Green);
		}
	}

	NPRender::GetIns()->SetResolutionW(size.w);
	NPRender::GetIns()->SetResolutionH(size.h);
	NPRender::GetIns()->SetViewport(0, 0, size.w, size.h);
	NP2DSRenderStep::GetIns()->SetEntryLocalT(NPVector3::Zero);
	NP2DSRenderStep::GetIns()->SetEntryWorldS(NPVector3::PositiveOne);
	NP2DSRenderStep::GetIns()->RenderImmediate();
}
xui_method_explain(onity_preview, on_viewmousewheel,	void				)( xui_component* sender, xui_method_mouse&  args )
{
	if (m_tool->was_visible())
	{
		xui_method_args other_args;
		if (args.wheel > 0) on_buttonclick(m_large, other_args);
		else 				on_buttonclick(m_small, other_args);

		args.handle = true;
	}
}

/*
//method
*/
xui_method_explain(onity_preview, get_speed,			f32					)( void )
{
	f32 speed = 1.0f;
	s32 value = m_speed->get_range()/2 - m_speed->get_value();
	if (value > 0)
		speed -= value * 0.1f;
	if (value < 0)
		speed -= value * 1.0f;

	return speed;
}
xui_method_explain(onity_preview, set_speed,			void				)( f32 speed )
{
	s32 range = m_speed->get_range();
	f32 value = 1.0f - speed;
	if (value > 0.0f)
		m_speed->set_value(range/2 - (s32)(value*10.0f));
	if (value < 0.0f)
		m_speed->set_value(range/2 - (s32)(value));
}
xui_method_explain(onity_preview, set_localtransform,	void				)( void )
{
	xui_vector<s32> size = m_view->get_rendersz();
	if (m_node)
	{
		NP2DSFrameRef* frameref = NPDynamicCast(NP2DSFrameRef, m_node);
		NP2DSActorRef* actorref = NPDynamicCast(NP2DSActorRef, m_node);
		NPParticleSFX* particle = NPDynamicCast(NPParticleSFX, m_node);
		if (frameref)
		{
			NPRect bound = frameref->GetOrignBounding();

			f32 sw = (f32)size.w / (f32)bound.GetW();
			f32 sh = (f32)size.h / (f32)bound.GetH();
			f32 s  = xui_min(sw, sh);
			if (s > 1.0f)
				s = 1.0f;

			NPVector3 scale = NPVector3(s, s, 1.0f);
			NPVector3 trans = NPVector3(
				(-bound.LT*s + (((f32)size.w - bound.GetW()*s)) / 2.0f), 
				(-bound.TP*s + (((f32)size.h - bound.GetH()*s)) / 2.0f),
				0.0f);

			trans.x = xui_pixel_align(trans.x);
			trans.y = xui_pixel_align(trans.y);

			frameref->SetLocalScale(scale);
			frameref->SetLocalTrans(trans);
		}

		if (actorref && actorref->GetActor())
		{
			NP2DSActor* actor = actorref->GetActor();

			NPRect bound = NPRect::Empty;
			for (s32 i = (s32)actor->GetLayerCount()-1; i >= 0; --i)
			{
				NP2DSLayer* layer = actor->GetLayer((npu16)i);
				if (layer->GetFrameKeyCount() == 0)
					continue;

				std::list<NP2DSFrameKey*>& frameKeyList = layer->GetFrameKeyList();
				NP2DSFrameKey* frameKey  = frameKeyList.front();
				bound = bound.GetUnion(frameKey->GetTransRef()->GetWorldBounding());
			}

			f32 sw = (f32)size.w / (f32)bound.GetW();
			f32 sh = (f32)size.h / (f32)bound.GetH();
			f32 s  = xui_min(sw, sh);
			if (s > 1.0f)
				s = 1.0f;

			NPVector3 scale = NPVector3(s, s, 1.0f);
			NPVector3 trans = NPVector3(
				(-bound.LT*s + (((f32)size.w - bound.GetW()*s)) / 2.0f), 
				(-bound.TP*s + (((f32)size.h - bound.GetH()*s)) / 2.0f),
				0.0f);

			trans.x = xui_pixel_align(trans.x);
			trans.y = xui_pixel_align(trans.y);

			actorref->SetLocalFlips(0);
			actorref->SetLocalAngle(0.0f);
			actorref->SetLocalScale(scale);
			actorref->SetLocalTrans(trans);
		}

		if (particle)
		{
			NPVector3 trans = NPVector3(size.w/2.0f, size.h/2.0f, 0.0f);
			trans.x = xui_pixel_align(trans.x);
			trans.y = xui_pixel_align(trans.y);

			particle->SetLocalT(trans);
		}
	}
}
