#include "NP2DSTransRef.h"
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
#include "onity_boundbox_entitystar.h"
#include "onity_propentitynode.h"
#include "onity_mainform.h"
#include "onity_resource.h"
#include "onity_inspector.h"
#include "onity_propentitystar.h"

/*
//constructor
*/
xui_create_explain(onity_propentitystar)( onity_propentitynode* node, u32 index )
: onity_propeditnode()
, m_groupnode(node)
, m_starindex(index)
{
	m_assetkind = new xui_propkind(this, L"Star", "StarDesign", xui_kindctrl::create, onity_resource::icon_design, true);
	m_assetkind->add_propdata(new xui_propdata_bool(
		m_assetkind,
		L"Visible",
		xui_propctrl_bool::create,
		was_visible,
		NULL,
		this));

	add_propkind(m_transkind);
	add_propkind(m_paramkind);
	add_propkind(m_assetkind);

	m_boundctrl = new onity_boundbox_entitystar(this);
}

/*
//method
*/
xui_method_explain(onity_propentitystar, get_groupnode,	onity_propentitynode*	)( void )
{
	return m_groupnode;
}
xui_method_explain(onity_propentitystar, get_starindex,	u32						)( void )
{
	return m_starindex;
}
xui_method_explain(onity_propentitystar, was_visible,	bool					)( void )
{
	Omiga::Entity* entity = m_groupnode->get_entity();
	Omiga::TransformComponent* starTransform = entity->GetComponent<Omiga::TransformComponent>();
	BreezeGame::StarAIComp* starAI = entity->GetComponent<BreezeGame::StarAIComp>();
	std::vector<BreezeGame::StarData>& starDataVec = starAI->GetStarVec();
	if (m_starindex < starDataVec.size())
	{
		BreezeGame::StarVisualComp* starVisual = entity->GetComponent<BreezeGame::StarVisualComp>();
		BreezeGame::MultiFrameRef* frameref = starVisual->GetFrameRef(starDataVec[m_starindex].m_starType);
		Omiga::Transform transform;
		bool visible;
		frameref->GetRenderNode(starDataVec[m_starindex].m_id, transform, visible);
		return visible;
	}

	return false;
}
xui_method_explain(onity_propentitystar, get_2dsref,	NP2DSTransRef*			)( void )
{
	Omiga::Entity* entity = m_groupnode->get_entity();
	Omiga::TransformComponent* starTransform = entity->GetComponent<Omiga::TransformComponent>();
	BreezeGame::StarAIComp* starAI = entity->GetComponent<BreezeGame::StarAIComp>();
	std::vector<BreezeGame::StarData>& starDataVec = starAI->GetStarVec();
	if (m_starindex < starDataVec.size())
	{
		return starDataVec[m_starindex].m_LinkRef;
	}

	return NULL;
}
xui_method_explain(onity_propentitystar, get_scale,		xui_vector<f64>			)( void )
{
	Omiga::Entity* entity = m_groupnode->get_entity();
	Omiga::TransformComponent* starTransform = entity->GetComponent<Omiga::TransformComponent>();
	BreezeGame::StarAIComp* starAI = entity->GetComponent<BreezeGame::StarAIComp>();
	std::vector<BreezeGame::StarData>& starDataVec = starAI->GetStarVec();
	if (m_starindex < starDataVec.size())
	{
		BreezeGame::StarVisualComp* starVisual = entity->GetComponent<BreezeGame::StarVisualComp>();
		BreezeGame::MultiFrameRef* frameref = starVisual->GetFrameRef(starDataVec[m_starindex].m_starType);
		Omiga::Transform transform;
		bool visible;
		frameref->GetRenderNode(starDataVec[m_starindex].m_id, transform, visible);
		Omiga::Vec2Df scale = starTransform->GetScale();
		return xui_vector<f64>((f64)scale.x, (f64)scale.y);
	}

	return xui_vector<f64>(1.0);
}
xui_method_explain(onity_propentitystar, set_scale,		void					)( const xui_vector<f64>& value )
{
	Omiga::Entity* entity = m_groupnode->get_entity();
	Omiga::TransformComponent* starTransform = entity->GetComponent<Omiga::TransformComponent>();
	BreezeGame::StarAIComp* starAI = entity->GetComponent<BreezeGame::StarAIComp>();
	std::vector<BreezeGame::StarData>& starDataVec = starAI->GetStarVec();
	if (m_starindex < starDataVec.size())
	{
		BreezeGame::StarVisualComp* starVisual = entity->GetComponent<BreezeGame::StarVisualComp>();
		BreezeGame::MultiFrameRef* frameref = starVisual->GetFrameRef(starDataVec[m_starindex].m_starType);
		Omiga::Transform transform;
		bool visible;
		frameref->GetRenderNode(starDataVec[m_starindex].m_id, transform, visible);
		transform.Scale = Vec2Df((f32)value.x, (f32)value.y);
		frameref->UpdateRenderNode(starDataVec[m_starindex].m_id, transform, visible);

		NP2DSTransRef* linkref = starDataVec[m_starindex].m_LinkRef;
		if (linkref)
		{
			linkref->SetWorldScale(NPVector3((npf32)value.x, (npf32)value.y, 1.0f));
		}
	}
}

/*
//static
*/
xui_method_explain(onity_propentitystar, was_visible,	bool					)( void* userptr )
{
	onity_propentitystar* prop = (onity_propentitystar*)userptr;
	return prop->was_visible();
}