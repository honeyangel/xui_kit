#include "NPRender.h"
#include "NP2DSAssetFile.h"
#include "NP2DSImage.h"
#include "NP2DSFrame.h"
#include "NP2DSActor.h"
#include "NP2DSLayer.h"
#include "NP2DSActorRef.h"
#include "NP2DSFrameKey.h"
#include "NP2DSRenderStep.h"
#include "xui_toolbar.h"
#include "xui_linebox.h"
#include "xui_panel.h"
#include "xui_toggle.h"
#include "xui_treeview.h"
#include "xui_treenode.h"
#include "xui_timeview.h"
#include "xui_timeline.h"
#include "xui_timetool.h"
#include "xui_timer.h"

#include "onity_resource.h"
#include "onity_alignbox.h"
#include "onity_timeline.h"
#include "onity_mainform.h"
#include "onity_timedata.h"
#include "onity_propactor.h"
#include "onity_proplayer.h"
#include "onity_propframekey.h"
#include "onity_boundbox.h"
#include "onity_layerdata.h"
#include "onity_renderview.h"
#include "onity_snaptool.h"
#include "onity_action.h"

xui_implement_rtti(onity_action, onity_asset);

/*
//constructor
*/
xui_create_explain(onity_action)( void )
: onity_asset()
{
	ini_namectrl(onity_resource::icon_scene, L"Action");

	m_collider	= new xui_toggle(xui_vector<s32>(20, 20), TOGGLE_BUTTON);
	xui_method_ptrcall(m_collider,	ini_drawer		)(onity_resource::icon_scene);
	xui_method_ptrcall(m_collider,	set_drawcolor	)(true);
	xui_method_ptrcall(m_collider,	set_enable		)(false);
	xui_method_ptrcall(m_collider,	set_corner		)(3);
	xui_method_ptrcall(m_collider,	set_iconalign	)(IMAGE_C);

	m_alignbox	= new onity_alignbox(xui_vector<s32>(20), get_selectedvec);
	xui_method_ptrcall(m_linetool,	add_item		)(m_showbbox);
	xui_method_ptrcall(m_linetool,	add_separate	)();
	xui_method_ptrcall(m_linetool,	add_item		)(m_collider);
	xui_method_ptrcall(m_linetool,	add_separate	)();
	xui_method_ptrcall(m_linetool,	add_item		)(m_alignbox->get_horzline());
	xui_method_ptrcall(m_linetool,	add_separate	)();
	xui_method_ptrcall(m_linetool,	add_item		)(m_alignbox->get_vertline());
	xui_method_ptrcall(m_drawview,	xm_mousedragover) += new xui_method_member<xui_method_dragdrop,	onity_action>(this, &onity_action::on_drawviewmousedragover);
	xui_method_ptrcall(m_drawview,	xm_mousedragdrop) += new xui_method_member<xui_method_dragdrop,	onity_action>(this, &onity_action::on_drawviewmousedragdrop);

	m_actorref = new NP2DSActorRef;
	add_pagectrl(m_headpane);
	add_pagectrl(m_fillpane);
}

/*
//destructor
*/
xui_delete_explain(onity_action)( void )
{
	delete m_actorref;
}

/*
//method
*/
xui_method_explain(onity_action, get_selectedvec,			onity_boundbox_vec	)( void )
{
	return onity_mainform::get_ptr()->get_action()->get_selectedboundbox();
}
xui_method_explain(onity_action, hit_propvisible,			onity_boundbox*		)( const xui_vector<s32>& pt )
{
	onity_timeline*	 timeline = onity_mainform::get_ptr()->get_timeline();
	xui_timeview*	 timeview = timeline->get_timeview();
	onity_propactor* editprop = timeline->get_editprop();
	if (editprop)
	{
		s32 curframe = timeview->get_curframe();
		std::vector<xui_timeline*> linevec = timeview->get_timelinetotal();
		for (u32 i = 0; i < linevec.size(); ++i)
		{
			onity_timedata*		data		= (onity_timedata*)linevec[i]->get_linkdata();
			onity_proplayer*	prop		= dynamic_cast<onity_proplayer*>(data->get_prop());
			onity_propframekey* propframe	= dynamic_cast<onity_propframekey*>(prop->get_framekey(curframe));
			if (propframe && false)
			{
				onity_boundbox* bbox = propframe->get_boundbox();
				xui_rect2d<s32> rect = bbox->get_bounding(m_trans, m_ratio);
				if (rect.was_inside(pt))
					return bbox;
			}
		}
	}

	return NULL;
}
xui_method_explain(onity_action, get_selectedboundbox,		onity_boundbox_vec	)( void )
{
	onity_boundbox_vec vec;

	onity_timeline*  timeline = onity_mainform::get_ptr()->get_timeline();
	xui_timeview*	 timeview = timeline->get_timeview();
	onity_propactor* editprop = timeline->get_editprop();
	if (editprop)
	{
		s32 curframe = timeview->get_curframe();
		std::vector<xui_timeline*> linevec = timeview->get_selectedline();
		for (u32 i = 0; i < linevec.size(); ++i)
		{
			if (linevec[i]->has_selframe(curframe))
			{
				onity_timedata*		data		= (onity_timedata*)linevec[i]->get_linkdata();
				onity_proplayer*	prop		= dynamic_cast<onity_proplayer*>(data->get_prop());
				onity_propframekey* propframe	= dynamic_cast<onity_propframekey*>(prop->get_framekey(curframe));
				//vec.push_back(propframe->get_boundbox());
			}
		}
	}

	return vec;
}
xui_method_explain(onity_action, add_framekey,				onity_propframekey*	)( xui_timeline* timeline, s32 curframe, bool moveafter /* = false */ )
{
	onity_layerdata* layerdata = (onity_layerdata*)timeline->get_linkdata();
	onity_proplayer* proplayer = dynamic_cast<onity_proplayer*>(layerdata->get_prop());
	onity_propactor* propactor = proplayer->get_actor();
	if (moveafter || proplayer->get_framekey(curframe))
	{
		std::list<NP2DSFrameKey*> keylist = proplayer->get_layer()->GetFrameKeyList();
		for (std::list<NP2DSFrameKey*>::iterator itor = keylist.begin(); itor != keylist.end(); ++itor)
		{
			npu16 time = (*itor)->GetTime();
			if (time >= curframe)
				(*itor)->SetTime(time+1);
		}
	}

	NP2DSActor* actor = NPDynamicCast(NP2DSActor, propactor->get_asset());
	npu16 index = actor->GetLayerIndex(proplayer->get_layer());

	onity_propframekey* prop = dynamic_cast<onity_propframekey*>(proplayer->add_framekey(curframe));
	prop->get_framekey()->Clone(m_actorref->GetFrameKey(index));
	prop->get_framekey()->SetTime(curframe);
	prop->set_transref();
	return prop;
}

/*
//event
*/
xui_method_explain(onity_action, on_fillpanekeybddown,		void				)( xui_component* sender, xui_method_keybd&		args )
{

}
xui_method_explain(onity_action, on_drawviewrenderself,		void				)( xui_component* sender, xui_method_args&		args )
{
	xui_convas::get_ins()->clear(xui_colour(1.0f, 0.3f));

	xui_vector<s32> size = sender->get_rendersz();
	NPRender::GetIns()->SetResolutionW(size.w);
	NPRender::GetIns()->SetResolutionH(size.h);
	NPRender::GetIns()->SetViewport(0, 0, size.w, size.h);

	onity_timeline*  timeline = onity_mainform::get_ptr()->get_timeline();
	xui_timeview*	 timeview = timeline->get_timeview();
	onity_propactor* editprop = timeline->get_editprop();
	if (editprop)
	{
		NP2DSActor* actor = NPDynamicCast(NP2DSActor, editprop->get_asset());
		if (actor)
		{
			m_actorref->SetActor(actor->GetOwnedFile()->GetKey(), actor->GetKey());
			m_actorref->SetTime(timeview->get_curframe());
			m_actorref->SetPlay(true);
			m_actorref->SetLocalFlips(0);
			m_actorref->SetLocalAngle(0.0f);
			m_actorref->SetLocalScale(NPVector3::PositiveOne);
			m_actorref->SetLocalTrans(NPVector3::Zero);
			m_actorref->Synch();
			m_actorref->Update(0.0f);

			xui_timer* timer = timeview->get_timetool()->get_playtimer();
			std::vector<xui_timeline*> linevec = timeview->get_timelinetotal();
			if (timer->was_enable())
			{
				for (int i = linevec.size()-1; i >= 0; --i)
				{
					bool show = linevec[i]->get_linkdata()->get_flag(LAYER_FLAG_SHOW);
					m_actorref->SetLayerShow(i, show);
				}

				m_actorref->Render();
			}
			else
			{
				for (int i = linevec.size()-1; i >= 0; --i)
				{
					bool show = linevec[i]->get_linkdata()->get_flag(LAYER_FLAG_SHOW);
					if (!show)
						continue;

					m_actorref->GetFrameKey(i)->GetTransRef()->Render();
				}
			}
		}
	}

	NPVector3 scale((f32)m_ratio,   (f32)m_ratio,   1.0f);
	NPVector3 trans((f32)m_trans.x, (f32)m_trans.y, 0.0f);
	NP2DSRenderStep::GetIns()->SetEntryLocalT(trans);
	NP2DSRenderStep::GetIns()->SetEntryWorldS(scale);
	NP2DSRenderStep::GetIns()->RenderImmediate();
}
xui_method_explain(onity_action, on_drawviewrenderelse,		void				)( xui_component* sender, xui_method_args&		args )
{
	onity_asset::on_drawviewrenderelse(sender, args);

	xui_rect2d<s32> cliprect = xui_convas::get_ins()->get_cliprect();
	xui_convas::get_ins()->set_cliprect(cliprect.get_inter(m_drawview->get_renderrtabs()));

	xui_vector<s32> pt = m_drawview->get_screenpt();
	onity_timeline*  timeline = onity_mainform::get_ptr()->get_timeline();
	xui_timeview*	 timeview = timeline->get_timeview();
	onity_propactor* editprop = timeline->get_editprop();
	if (editprop)
	{
		s32 x1 = pt.x + xui_round(m_trans.x*m_ratio);
		s32 y1 = pt.y + xui_round(m_trans.y*m_ratio);
		s32 x2 = pt.x + xui_round(m_trans.x*m_ratio + 960.0f*m_ratio);
		s32 y2 = pt.y + xui_round(m_trans.y*m_ratio + 640.0f*m_ratio);
		xui_convas::get_ins()->draw_rectangle(xui_rect2d<s32>(x1, y1, x2, y2), xui_colour::gray);

		//if (get_treeview())
		//{
		//	xui_proproot_vec vec = m_editprop->get_subprop();
		//	for (u32 i = 0; i < vec.size(); ++i)
		//	{
		//		onity_propimage* prop = dynamic_cast<onity_propimage*>(vec[i]);
		//		onity_boundbox*  bbox = prop->get_boundbox();
		//		bbox->draw(m_trans, m_ratio, pt, m_showbbox->was_push());
		//	}
		//}
	}

	xui_convas::get_ins()->set_cliprect(cliprect);
}
xui_method_explain(onity_action, on_drawviewmousedragover,	void				)( xui_component* sender, xui_method_dragdrop&	args )
{
	onity_timeline*  timeline = onity_mainform::get_ptr()->get_timeline();
	onity_propactor* editprop = timeline->get_editprop();
	if (editprop)
	{
		NP2DSActor* actor = NPDynamicCast(NP2DSActor, editprop->get_asset());
		if (actor && actor->GetLayerCount() > 0)
		{
			if (args.type == "NP2DSImage"		||
				args.type == "NP2DSFrame"		||
				args.type == "NP2DSImageVector"	||
				args.type == "NP2DSFrameVector")
			{
				args.allow = true;
			}
		}
	}
}
xui_method_explain(onity_action, on_drawviewmousedragdrop,	void				)( xui_component* sender, xui_method_dragdrop&	args )
{
	xui_timeview*	 timeview = onity_mainform::get_ptr()->get_timeline()->get_timeview();
	onity_propactor* editprop = onity_mainform::get_ptr()->get_timeline()->get_editprop();
	if (editprop)
	{
		xui_timeline* timeline = NULL;
		s32 curframe = timeview->get_curframe();
		std::vector<xui_treenode*> nodevec = timeview->get_timetree()->get_selectednode();
		if (nodevec.size() > 0)
		{
			timeline = (xui_timeline*)nodevec.front()->get_data();
		}
		else
		{
			timeline = timeview->get_timeline(0);
		}

		if (args.type == "NP2DSImage" ||
			args.type == "NP2DSFrame")
		{
			onity_propframekey* prop = add_framekey(timeline, curframe);
			NP2DSAsset* asset = (NP2DSAsset*)args.data;
			if (NPIsExaKindOf(NP2DSImage, asset))
				prop->get_framekey()->SetImage(asset->GetOwnedFile()->GetKey(), asset->GetKey());
			if (NPIsExaKindOf(NP2DSFrame, asset))
				prop->get_framekey()->SetFrame(asset->GetOwnedFile()->GetKey(), asset->GetKey());

			prop->set_transref();

			timeline->get_linkdata()->cal_keyframe();
			timeview->set_selectedline(timeline, curframe, curframe, true);
			timeview->invalid();
		}
		else
		{
			std::vector<void*>  datavec = *((std::vector<void*>*)args.data);
			for (u32 i = 0; i < datavec.size(); ++i)
			{
				onity_propframekey* prop = add_framekey(timeline, curframe+i, true);
				NP2DSAsset* asset = (NP2DSAsset*)datavec[i];
				if (NPIsExaKindOf(NP2DSImage, asset))
					prop->get_framekey()->SetImage(asset->GetOwnedFile()->GetKey(), asset->GetKey());
				if (NPIsExaKindOf(NP2DSFrame, asset))
					prop->get_framekey()->SetFrame(asset->GetOwnedFile()->GetKey(), asset->GetKey());

				prop->set_transref();
			}

			timeline->get_linkdata()->cal_keyframe();
			timeview->set_selectedline(timeline, curframe, curframe+datavec.size()-1, true);
			timeview->invalid();
		}
	}
}

/*
//override
*/
xui_method_explain(onity_action, on_mousepickimpl,			void				)( onity_boundbox* pick, bool alt, bool ctrl, bool shift, u08 op )
{
	onity_timeline*  timeline = onity_mainform::get_ptr()->get_timeline();
	xui_timeview*	 timeview = timeline->get_timeview();
	onity_propactor* editprop = timeline->get_editprop();
	if (editprop)
	{
		onity_propframekey* propframe	= dynamic_cast<onity_propframekey*>(pick->get_linkprop());
		onity_proplayer*	proplayer	= propframe->get_layer();
		onity_layerdata*	data		= (onity_layerdata*)proplayer->get_linkdata(timeview->get_timetree());
		xui_timeline*		line		= data->get_line();

		s32  curframe = timeview->get_curframe();
		bool selected = line->has_selframe(curframe);
		if (ctrl && !shift)
		{
			timeview->set_selectedline(line, curframe, curframe, !selected);
		}
		else
		if (selected == false)
		{
			timeview->non_selectedline(false);
			timeview->set_selectedline(line, curframe, curframe, true);
		}
	}
}
xui_method_explain(onity_action, on_mulselectimpl,			void				)( const xui_rect2d<s32>& rt, bool ctrl )
{
	onity_timeline*  timeline = onity_mainform::get_ptr()->get_timeline();
	xui_timeview*	 timeview = timeline->get_timeview();
	onity_propactor* editprop = timeline->get_editprop();
	if (editprop)
	{
		s32 curframe = timeview->get_curframe();
		std::vector<xui_timeline*> nextvec;
		std::vector<xui_timeline*> linevec = timeview->get_timelinetotal();
		for (u32 i = 0; i < linevec.size(); ++i)
		{
			onity_timedata*		data		= (onity_timedata*)linevec[i]->get_linkdata();
			onity_proplayer*	prop		= dynamic_cast<onity_proplayer*>(data->get_prop());
			onity_propframekey* propframe	= dynamic_cast<onity_propframekey*>(prop->get_framekey(curframe));
			if (propframe && false)
			{
				onity_boundbox* bbox = propframe->get_boundbox();
				xui_rect2d<s32> temp = bbox->get_bounding(m_trans, m_ratio);
				if (temp.get_inter(rt).was_valid())
				{
					nextvec.push_back(linevec[i]);
				}
			}
		}

		if (nextvec.size() > 0)
		{
			if (ctrl == false)
				timeview->non_selectedline(false);

			timeview->set_selectedline(nextvec, curframe, curframe, true);
		}
	}
}