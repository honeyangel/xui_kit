#include "NP2DSTransRef.h"
#include "Game/Scene/Star/StarVisualComp.h"
#include "Game/Scene/Star/StarAIComp.h"
#include "Game/Scene/MultiFrameRef.h"
#include "Entity.h"
#include "Entity/Component/Visual/WGVisualManager.h"
#include "Entity/Component/Transform/TransformComponent.h"

#include "xui_treeview.h"
#include "xui_treenode.h"
#include "xui_treedata.h"
#include "onity_propentitystar.h"
#include "onity_propentitynode.h"
#include "onity_boundbox_entitystar.h"
#include "onity_hierarchy.h"
#include "onity_mainform.h"

/*
//constructor
*/
xui_create_explain(onity_boundbox_entitystar)( xui_proproot* prop )
: onity_boundbox(prop, ST_NONE)
{}

/*
//override
*/
xui_method_explain(onity_boundbox_entitystar, ori_position, xui_vector<s32>	)( void )
{
	onity_propentitystar* prop = dynamic_cast<onity_propentitystar*>(m_linkprop);
	Omiga::Entity* entity = prop->get_groupnode()->get_entity();
	Omiga::TransformComponent* starTransform = entity->GetComponent<Omiga::TransformComponent>();
	BreezeGame::StarAIComp* starAI = entity->GetComponent<BreezeGame::StarAIComp>();
	std::vector<BreezeGame::StarData>& starDataVec = starAI->GetStarVec();
	u32 starindex = prop->get_starindex();
	if (starindex < starDataVec.size())
	{
		BreezeGame::StarVisualComp* starVisual = entity->GetComponent<BreezeGame::StarVisualComp>();
		BreezeGame::MultiFrameRef* frameref = starVisual->GetFrameRef(starDataVec[starindex].m_starType);
		Omiga::Transform transform;
		bool visible;
		frameref->GetRenderNode(starDataVec[starindex].m_id, transform, visible);
		Omiga::Vec2Df pos = starTransform->GetPosition() + transform.Position;
		return xui_vector<s32>((s32)pos.x, (s32)pos.y);
	}

	return xui_vector<s32>(0);
}
xui_method_explain(onity_boundbox_entitystar, set_position, void			)( const xui_vector<s32>& pos )
{
	onity_propentitystar* prop = dynamic_cast<onity_propentitystar*>(m_linkprop);
	Omiga::Entity* entity = prop->get_groupnode()->get_entity();
	Omiga::TransformComponent* starTransform = entity->GetComponent<Omiga::TransformComponent>();
	BreezeGame::StarAIComp* starAI = entity->GetComponent<BreezeGame::StarAIComp>();
	std::vector<BreezeGame::StarData>& starDataVec = starAI->GetStarVec();
	u32 starindex = prop->get_starindex();
	if (starindex < starDataVec.size())
	{
		BreezeGame::StarVisualComp* starVisual = entity->GetComponent<BreezeGame::StarVisualComp>();
		BreezeGame::MultiFrameRef* frameref = starVisual->GetFrameRef(starDataVec[starindex].m_starType);
		Omiga::Transform transform;
		bool visible;
		frameref->GetRenderNode(starDataVec[starindex].m_id, transform, visible);
		transform.Position = Vec2Df((f32)pos.x, (f32)pos.y) - starTransform->GetPosition();
		frameref->UpdateRenderNode(starDataVec[starindex].m_id, transform, visible);

		NP2DSTransRef* linkref = starDataVec[starindex].m_LinkRef;
		if (linkref)
		{
			linkref->SetWorldTrans(NPVector3((f32)pos.x - (f32)entity->GetLeftPos(), (f32)pos.y, 0.0f));
		}
	}
}
xui_method_explain(onity_boundbox_entitystar, ori_bounding, xui_rect2d<s32>	)( void )
{
	onity_propentitystar* prop = dynamic_cast<onity_propentitystar*>(m_linkprop);
	Omiga::Entity* entity = prop->get_groupnode()->get_entity();
	Omiga::TransformComponent* starTransform = entity->GetComponent<Omiga::TransformComponent>();
	BreezeGame::StarAIComp* starAI = entity->GetComponent<BreezeGame::StarAIComp>();
	std::vector<BreezeGame::StarData>& starDataVec = starAI->GetStarVec();
	u32 starindex = prop->get_starindex();
	if (starindex < starDataVec.size())
	{
		BreezeGame::StarVisualComp* starVisual = entity->GetComponent<BreezeGame::StarVisualComp>();
		BreezeGame::MultiFrameRef* frameref = starVisual->GetFrameRef(starDataVec[starindex].m_starType);
		Omiga::Transform transform;
		bool visible;
		frameref->GetRenderNode(starDataVec[starindex].m_id, transform, visible);
		NPRect bounding = frameref->GetLocalBounding();
		Omiga::Vec2Df pos = BreezeGame::WGVisualManager::Instance()->GetCameraCoord(starTransform->GetPosition() + transform.Position);
		bounding.OftXY((s32)pos.x, (s32)pos.y);
		return xui_rect2d<s32>(bounding.LT, bounding.TP, bounding.RT, bounding.BM);
	}

	return xui_rect2d<s32>(0);
}
xui_method_explain(onity_boundbox_entitystar, was_selected, bool			)( void )
{
	onity_propentitystar* prop = dynamic_cast<onity_propentitystar*>(m_linkprop);
	xui_treeview* treeview = onity_mainform::get_ptr()->get_hierarchy()->get_treeview();
	xui_treedata* treedata = prop->get_linkdata(treeview);
	return treedata->get_node()->was_selected();
}