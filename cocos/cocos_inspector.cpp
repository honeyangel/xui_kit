//#include "NPRender.h"
//#include "NPSourceTexture.h"
//#include "NP2DSImage.h"
//#include "NP2DSFrame.h"
//#include "NP2DSActor.h"
//#include "NP2DSLayer.h"
//#include "NP2DSFrameKey.h"
//#include "NP2DSAssetFile.h"
//#include "NP2DSTransRef.h"
//#include "NP2DSImageRef.h"
//#include "NP2DSFrameRef.h"
//#include "NP2DSActorRef.h"
//#include "NP2DSTextureCache.h"
//#include "NPParticleSFX.h"
//#include "NP2DSRenderStep.h"
#include "2d/CCNode.h"

#include "xui_desktop.h"
#include "xui_convas.h"
#include "xui_bitmap.h"
#include "xui_kindctrl.h"
#include "xui_propctrl.h"
#include "xui_propkind.h"
#include "xui_propdata.h"
#include "xui_proproot.h"
#include "xui_propview.h"
#include "xui_propctrl_vector.h"
#include "xui_propctrl_stdvec.h"
#include "xui_propctrl_object.h"
#include "xui_propctrl_slider.h"
#include "cocos_renderview.h"
#include "cocos_propfile.h"
#include "cocos_propleaf.h"
#include "cocos_resource.h"
#include "cocos_preview.h"
#include "cocos_inspector.h"

xui_implement_rtti(cocos_inspector, xui_dockpage);

/*
//constructor
*/
xui_create_explain(cocos_inspector)( void )
: xui_dockpage(xui_vector<s32>(400), AREALIMIT_L|AREALIMIT_R, 400, DOCKSTYLE_R)
, m_tipsnode(NULL)
{
	xm_keybddown += new xui_method_member<xui_method_keybd, cocos_inspector>(this, &cocos_inspector::on_inspectorkeybddown);
	ini_namectrl(cocos_resource::icon_inspector, L"Inspector");

	m_propview	= new xui_propview(xui_vector<s32>(200));
	xui_method_ptrcall(m_propview,	ini_component	)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_propview,	set_borderrt	)(xui_rect2d<s32>(4));
	m_drawview	= new cocos_preview();
	xui_method_ptrcall(m_drawview,	set_backcolor	)(xui_colour(1.0f, 0.15f));
	xui_method_ptrcall(m_drawview,	set_drawcolor	)(true);
	xui_method_ptrcall(m_drawview,	set_borderrt	)(xui_rect2d<s32>(0, 6, 0, 6));
	xui_method_ptrcall(m_drawview,	set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_drawview,	ini_component	)(0, 0, DOCKSTYLE_B);
	xui_method_ptrcall(m_drawview,	ini_component	)(true, true);
	m_sizectrl	= new xui_control(xui_vector<s32>(4));
	xui_method_ptrcall(m_sizectrl,	xm_mousemove	) += new xui_method_member<xui_method_mouse, cocos_inspector>(this, &cocos_inspector::on_sizectrlmousemove);
	xui_method_ptrcall(m_sizectrl,	ini_component	)(0, 0, DOCKSTYLE_U);
	xui_method_ptrcall(m_sizectrl,	set_cursor		)(CURSOR_NS);
	m_tipsview	= new cocos_renderview(xui_vector<s32>(100), xui_vector<s32>(128));
	xui_method_ptrcall(m_tipsview,	xm_renderself	) += new xui_method_member<xui_method_args,  cocos_inspector>(this, &cocos_inspector::on_tipsviewrenderself);
	xui_method_ptrcall(m_tipsview,	set_sidestyle	)(SIDESTYLE_S);

	add_pagectrl(m_drawview);
	add_pagectrl(m_propview);
	add_pagectrl(m_sizectrl);
}

/*
//destructor
*/
xui_delete_explain(cocos_inspector)( void )
{
	delete m_tipsnode;
	xui_desktop::get_ins()->move_recycle(m_tipsview);
}

/*
//method
*/
xui_method_explain(cocos_inspector,	get_preview,			cocos_preview*	)( void )
{
	return m_drawview;
}
xui_method_explain(cocos_inspector, get_propview,			xui_propview*	)( void )
{
	return m_propview;
}
xui_method_explain(cocos_inspector, set_proproot,			void			)( xui_proproot* proproot )
{
	m_propview->set_proproot(proproot);
	m_drawview->set_viewprop(proproot);
	m_drawview->set_needshow();
	refresh();
}
xui_method_explain(cocos_inspector, set_proproot,			void			)( const xui_proproot_vec& proproot )
{
	m_propview->set_proproot(proproot);
	m_drawview->set_viewprop(proproot.size() > 0 ? proproot.front() : NULL);
	m_drawview->set_needshow();
	refresh();
}

/*
//tips
*/
xui_method_explain(cocos_inspector, show_tips,				void			)( cocos2d::Node* value, const xui_vector<s32>& pt )
{
	if (m_tipsnode)
	{
		delete m_tipsnode;
		m_tipsnode = NULL;
	}

	//if (NPIsExaKindOf(NPParticleSFX, value))
	//{
	//	NPParticleSFX* particle = ((NPParticleSFX*)value)->CreateInstance();
	//	particle->SetFlag(PARTICLESFX_LOOPPLAY, true);
	//	particle->Active();
	//	m_tipsnode = particle;
	//}
	//else
	//if (NPIsSubKindOf(NP2DSAsset, value))
	//{
	//	NP2DSImage* image = NPDynamicCast(NP2DSImage, value);
	//	NP2DSFrame* frame = NPDynamicCast(NP2DSFrame, value);
	//	NP2DSActor* actor = NPDynamicCast(NP2DSActor, value);
	//	if (image)
	//	{
	//		NP2DSImageRef* imageref = new NP2DSImageRef;
	//		imageref->SetImage(image->GetOwnedFile()->GetKey(), image->GetKey());
	//		m_tipsnode = imageref;
	//	}
	//	if (frame)
	//	{
	//		NP2DSFrameRef* frameref = new NP2DSFrameRef;
	//		frameref->SetFrame(frame->GetOwnedFile()->GetKey(), frame->GetKey());
	//		m_tipsnode = frameref;
	//	}
	//	if (actor)
	//	{
	//		NP2DSActorRef* actorref = new NP2DSActorRef;
	//		actorref->SetActor(actor->GetOwnedFile()->GetKey(), actor->GetKey());
	//		m_tipsnode = actorref;
	//	}
	//}

	set_localtransform();

	xui_vector<s32> finalpt = pt;
	xui_window* window = get_window();
	if (window->get_owner())
	{
		finalpt.y += xui_propview::default_lineheight;
	}
	else
	{
		finalpt.x -= m_tipsview->get_renderw();
	}
	m_tipsview->set_data(value);
	m_tipsview->set_renderpt(finalpt);
	xui_desktop::get_ins()->set_floatctrl(window, m_tipsview);
}
xui_method_explain(cocos_inspector, hide_tips,				void			)( void )
{
	xui_desktop::get_ins()->set_floatctrl(NULL, NULL);
}

/*
//callback
*/
xui_method_explain(cocos_inspector, on_perform,				void			)( xui_method_args&   args )
{
	xui_dockpage::on_perform(args);
	xui_rect2d<s32> rt = m_propview->get_renderrt()+m_propview->get_renderpt();
	m_sizectrl->on_perform_w(rt.get_w());
	m_sizectrl->on_perform_x(rt.ax);
	m_sizectrl->on_perform_y(rt.by-m_sizectrl->get_renderh());
}
xui_method_explain(cocos_inspector, on_updateself,			void			)( xui_method_update& args )
{
	xui_dockpage::on_updateself(args);
	if (m_tipsnode)
		m_tipsnode->update(args.delta);
}

/*
//event
*/
xui_method_explain(cocos_inspector, on_inspectorkeybddown,	void			)( xui_component* sender, xui_method_keybd& args )
{
	if (args.kcode == KEY_S && args.ctrl)
	{
		xui_proproot_vec vec = m_propview->get_proproot();
		for (u32 i = 0; i < vec.size(); ++i)
		{
			cocos_propfile* propfile = dynamic_cast<cocos_propfile*>(vec[i]);
			cocos_propleaf* propleaf = dynamic_cast<cocos_propleaf*>(vec[i]);
			if (propleaf)
				propfile  = propleaf->get_propfile();
			if (propfile && propfile->was_modify())
			{
				propfile->save();
				args.handle = true;
			}
		}
	}
}
xui_method_explain(cocos_inspector, on_sizectrlmousemove,	void			)( xui_component* sender, xui_method_mouse& args )
{
	if (m_sizectrl->has_catch() && m_drawview->was_visible())
	{
		s32 delta  = xui_desktop::get_ins()->get_mousemove().y;
		s32 height = m_drawview->get_renderh() - delta;
		height = xui_max(height, 100);
		height = xui_min(height, 512);
		m_drawview->set_renderh(height);		
	}
}
xui_method_explain(cocos_inspector, on_tipsviewrenderself,	void			)( xui_component* sender, xui_method_args&  args )
{
	xui_convas::get_ins()->clear(xui_colour(1.0f, 0.25f));
	//extern bool gInitCompleted;
	//if (gInitCompleted == false)
	//	return;

	//xui_vector<s32> size = m_tipsview->get_rendersz();
	//if (m_tipsnode)
	//	m_tipsnode->Render();
	//else
	//{
	//	NPObjectRef* value = (NPObjectRef*)m_tipsview->get_data();
	//	if (NPIsSubKindOf(NPTexture, value))
	//	{
	//		NPTexture* texture = NPDynamicCast(NPTexture, value);
	//		if (texture)
	//		{
	//			npu32 w = texture->GetWidth ();
	//			npu32 h = texture->GetHeight();
	//			npf32 s = (npf32)(size.w) / xui_max(w, h);
	//			npf32 x = 0.0f;
	//			npf32 y = 0.0f;
	//			NPTransform t;
	//			t.SetIdentity();
	//			t.SetS(NPVector3(s, s, 1.0f));
	//			t.SetT(NPVector3(x, y, 0.0f));
	//			NP2DSImage image(-1, "", -1, 0, 0, (nps16)texture->GetPixelWidth(), (nps16)texture->GetPixelHeight(), true);
	//			NP2DSRenderUtil::GetIns()->DrawImage(
	//				&image, 
	//				t.GetMatrix(),
	//				NPColor::White, 
	//				0,
	//				0,
	//				0,
	//				0,
	//				texture->GetKey());
	//		}
	//	}
	//}

	//NPRender::GetIns()->SetResolutionW(size.w);
	//NPRender::GetIns()->SetResolutionH(size.h);
	//NPRender::GetIns()->SetViewport(0, 0, size.w, size.h);
	//NP2DSRenderStep::GetIns()->SetEntryLocalT(NPVector3::Zero);
	//NP2DSRenderStep::GetIns()->SetEntryWorldS(NPVector3::PositiveOne);
	//NP2DSRenderStep::GetIns()->RenderImmediate();
}

/*
//method
*/
xui_method_explain(cocos_inspector, set_localtransform,		void			)( void )
{
	xui_vector<s32> size = m_tipsview->get_rendersz();
	if (m_tipsnode)
	{
		//NP2DSTransRef* transref = NPDynamicCast(NP2DSTransRef, m_tipsnode);
		//NP2DSActorRef* actorref = NPDynamicCast(NP2DSActorRef, m_tipsnode);
		//NPParticleSFX* particle = NPDynamicCast(NPParticleSFX, m_tipsnode);
		//if (particle)
		//{
		//	NPVector3 trans = NPVector3(size.w/2.0f, size.h/2.0f, 0.0f);
		//	trans.x = xui_pixel_align(trans.x);
		//	trans.y = xui_pixel_align(trans.y);

		//	particle->SetLocalS(NPVector3(0.5f, 0.5f, 1.0f));
		//	particle->SetLocalT(trans);
		//}
		//else 
		//if (actorref && actorref->GetActor())
		//{
		//	NP2DSActor* actor = actorref->GetActor();

		//	NPRect bound = NPRect::Empty;
		//	for (s32 i = (s32)actor->GetLayerCount()-1; i >= 0; --i)
		//	{
		//		NP2DSLayer* layer = actor->GetLayer((npu16)i);
		//		if (layer->GetFrameKeyCount() == 0)
		//			continue;

		//		std::list<NP2DSFrameKey*>& frameKeyList = layer->GetFrameKeyList();
		//		NP2DSFrameKey* frameKey  = frameKeyList.front();
		//		bound = bound.GetUnion(frameKey->GetTransRef()->GetWorldBounding());
		//	}

		//	f32 sw = (f32)size.w / (f32)bound.GetW();
		//	f32 sh = (f32)size.h / (f32)bound.GetH();
		//	f32 s  = xui_min(sw, sh);
		//	if (s > 1.0f)
		//		s = 1.0f;

		//	NPVector3 scale = NPVector3(s, s, 1.0f);
		//	NPVector3 trans = NPVector3(
		//		(-bound.LT*s + (((f32)size.w - bound.GetW()*s)) / 2.0f), 
		//		(-bound.TP*s + (((f32)size.h - bound.GetH()*s)) / 2.0f),
		//		0.0f);

		//	trans.x = xui_pixel_align(trans.x);
		//	trans.y = xui_pixel_align(trans.y);

		//	actorref->SetLocalFlips(0);
		//	actorref->SetLocalAngle(0.0f);
		//	actorref->SetLocalScale(scale);
		//	actorref->SetLocalTrans(trans);
		//}
		//else
		//{
		//	NPRect bound = transref->GetOrignBounding();

		//	f32 sw = (f32)size.w / (f32)bound.GetW();
		//	f32 sh = (f32)size.h / (f32)bound.GetH();
		//	f32 s  = xui_min(sw, sh);
		//	if (s > 1.0f)
		//		s = 1.0f;

		//	NPVector3 scale = NPVector3(s, s, 1.0f);
		//	NPVector3 trans = NPVector3(
		//		(-bound.LT*s + (((f32)size.w - bound.GetW()*s)) / 2.0f), 
		//		(-bound.TP*s + (((f32)size.h - bound.GetH()*s)) / 2.0f),
		//		0.0f);

		//	trans.x = xui_pixel_align(trans.x);
		//	trans.y = xui_pixel_align(trans.y);

		//	transref->SetLocalScale(scale);
		//	transref->SetLocalTrans(trans);
		//}
	}
}