#include "Game/Scene/Star/StarVisualComp.h"
#include "Game/Scene/Star/StarAIComp.h"
#include "Game/Scene/MultiFrameRef.h"
#include "Entity/Component/Visual/WGVisualManager.h"

#include "xui_textbox.h"
#include "xui_propdata.h"
#include "xui_propctrl.h"
#include "xui_kindctrl.h"
#include "xui_propctrl_vector.h"
#include "xui_propctrl_stdvec.h"
#include "onity_propctrl_transref.h"
#include "onity_propctrl_sceneparam.h"
#include "onity_propentity.h"
#include "onity_mainform.h"
#include "onity_resource.h"
#include "onity_inspector.h"
#include "onity_propentitystar.h"

/*
//constructor
*/
xui_create_explain(onity_propentitystar)( onity_propentity* node, u32 index )
: onity_propedit()
, m_starnode(node)
, m_subindex(index)
{
	m_starkind = new xui_propkind(this, L"Star", "Star", xui_kindctrl::create, onity_resource::icon_transform, true);
	m_starkind->add_propdata(new xui_propdata_vector(
		m_starkind, 
		L"Position", 
		xui_propctrl_vector::create, 
		get_position, 
		set_position, 
		this, 
		NT_INT));
	m_starkind->add_propdata(new xui_propdata_bool(
		m_starkind,
		L"Visible",
		xui_propctrl_bool::create,
		was_visible,
		NULL,
		this));

	add_propkind(m_starkind);
}

/*
//method
*/
xui_method_explain(onity_propentitystar, was_visible,	bool			)( void )
{
	Omiga::Entity* entity = m_starnode->get_entity();
	Omiga::TransformComponent* starTransform = entity->GetComponent<Omiga::TransformComponent>();
	BreezeGame::StarAIComp* starAI = entity->GetComponent<BreezeGame::StarAIComp>();
	std::vector<BreezeGame::StarData>& starDataVec = starAI->GetStarVec();
	if (m_subindex < starDataVec.size())
	{
		BreezeGame::StarVisualComp* starVisual = entity->GetComponent<BreezeGame::StarVisualComp>();
		BreezeGame::MultiFrameRef* frameref = starVisual->GetFrameRef(starDataVec[m_subindex].m_starType);
		Omiga::Transform transform;
		bool visible;
		frameref->GetRenderNode(starDataVec[m_subindex].m_id, transform, visible);
		return visible;
	}

	return false;
}
xui_method_explain(onity_propentitystar, ori_bounding,	xui_rect2d<s32>	)( void )
{
	Omiga::Entity* entity = m_starnode->get_entity();
	Omiga::TransformComponent* starTransform = entity->GetComponent<Omiga::TransformComponent>();
	BreezeGame::StarAIComp* starAI = entity->GetComponent<BreezeGame::StarAIComp>();
	std::vector<BreezeGame::StarData>& starDataVec = starAI->GetStarVec();
	if (m_subindex < starDataVec.size())
	{
		BreezeGame::StarVisualComp* starVisual = entity->GetComponent<BreezeGame::StarVisualComp>();
		BreezeGame::MultiFrameRef* frameref = starVisual->GetFrameRef(starDataVec[m_subindex].m_starType);
		Omiga::Transform transform;
		bool visible;
		frameref->GetRenderNode(starDataVec[m_subindex].m_id, transform, visible);
		NPRect bounding = frameref->GetLocalBounding();
		Omiga::Vec2Df pos = BreezeGame::WGVisualManager::Instance()->GetCameraCoord(starTransform->GetPosition() + transform.Position);
		bounding.OftXY(pos.x, pos.y);
		return xui_rect2d<s32>(bounding.LT, bounding.TP, bounding.RT, bounding.BM);
	}

	return xui_rect2d<s32>(0);
}
xui_method_explain(onity_propentitystar, ori_position,	xui_vector<s32>	)( void )
{
	Omiga::Entity* entity = m_starnode->get_entity();
	Omiga::TransformComponent* starTransform = entity->GetComponent<Omiga::TransformComponent>();
	BreezeGame::StarAIComp* starAI = entity->GetComponent<BreezeGame::StarAIComp>();
	std::vector<BreezeGame::StarData>& starDataVec = starAI->GetStarVec();
	if (m_subindex < starDataVec.size())
	{
		BreezeGame::StarVisualComp* starVisual = entity->GetComponent<BreezeGame::StarVisualComp>();
		BreezeGame::MultiFrameRef* frameref = starVisual->GetFrameRef(starDataVec[m_subindex].m_starType);
		Omiga::Transform transform;
		bool visible;
		frameref->GetRenderNode(starDataVec[m_subindex].m_id, transform, visible);
		Omiga::Vec2Df pos = starTransform->GetPosition() + transform.Position;
		return xui_vector<s32>(pos.x, pos.y);
	}

	return xui_vector<s32>(0);
}
xui_method_explain(onity_propentitystar, set_position,	void			)( const xui_vector<s32>& pos )
{
	Omiga::Entity* entity = m_starnode->get_entity();
	Omiga::TransformComponent* starTransform = entity->GetComponent<Omiga::TransformComponent>();
	BreezeGame::StarAIComp* starAI = entity->GetComponent<BreezeGame::StarAIComp>();
	std::vector<BreezeGame::StarData>& starDataVec = starAI->GetStarVec();
	if (m_subindex < starDataVec.size())
	{
		BreezeGame::StarVisualComp* starVisual = entity->GetComponent<BreezeGame::StarVisualComp>();
		BreezeGame::MultiFrameRef* frameref = starVisual->GetFrameRef(starDataVec[m_subindex].m_starType);
		Omiga::Transform transform;
		bool visible;
		frameref->GetRenderNode(starDataVec[m_subindex].m_id, transform, visible);
		transform.Position = Vec2Df(pos.x, pos.y) - starTransform->GetPosition();
		frameref->UpdateRenderNode(starDataVec[m_subindex].m_id, transform, visible);

		NP2DSTransRef* linkref = starDataVec[m_subindex].m_LinkRef;
		if (linkref)
		{
			linkref->SetWorldTrans(NPVector3(pos.x - entity->GetLeftPos(), pos.y, 0.0f));
		}
	}
}

/*
//static
*/
xui_method_explain(onity_propentitystar, get_position,	xui_vector<f64>	)( void* userptr )
{
	onity_propedit* prop = (onity_propedit*)userptr;
	return prop->ori_position().to<f64>();
}
xui_method_explain(onity_propentitystar, set_position,	void			)( void* userptr, const xui_vector<f64>& value )
{
	onity_propedit* prop = (onity_propedit*)userptr;
	prop->set_position(value.to<s32>());
}
xui_method_explain(onity_propentitystar, was_visible,	bool			)( void* userptr )
{
	onity_propentitystar* prop = (onity_propentitystar*)userptr;
	return prop->was_visible();
}