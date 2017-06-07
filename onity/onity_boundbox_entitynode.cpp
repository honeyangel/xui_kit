#include "NP2DSTransRef.h"
#include "NP2DSImageRef.h"
#include "NP2DSFrameRef.h"
#include "NP2DSActorRef.h"
#include "Entity.h"
#include "Entity/Component/Transform/TransformComponent.h"
#include "Entity/Component/Visual/WGVisualComponent.h"
#include "xui_treeview.h"
#include "xui_treenode.h"
#include "xui_treedata.h"

#include "onity_mainform.h"
#include "onity_hierarchy.h"
#include "onity_propentitynode.h"
#include "onity_boundbox_entitynode.h"

/*
//constructor
*/
xui_create_explain(onity_boundbox_entitynode)( xui_proproot* prop )
: onity_boundbox(prop, ST_NONE)
{}

/*
//override
*/
xui_method_explain(onity_boundbox_entitynode, ori_position, xui_vector<s32>	)( void )
{
	onity_propentitynode* prop = dynamic_cast<onity_propentitynode*>(m_linkprop);
	Omiga::TransformComponent* transform = prop->get_entity()->GetComponent<Omiga::TransformComponent>();
	if (transform)
	{
		Omiga::Vec2Df pos = transform->GetPosition();
		return xui_vector<s32>((s32)pos.x, (s32)pos.y);
	}

	return xui_vector<s32>(0);
}
xui_method_explain(onity_boundbox_entitynode, set_position,	void			)( const xui_vector<s32>& pos )
{
	onity_propentitynode* prop = dynamic_cast<onity_propentitynode*>(m_linkprop);
	Omiga::TransformComponent* transform = prop->get_entity()->GetComponent<Omiga::TransformComponent>();
	if (transform)
	{
		transform->SetPosition(Omiga::Vec2Df((f32)pos.x, (f32)pos.y));
		NP2DSTransRef* linkref = prop->get_entity()->GetLinkRef();
		if (linkref)
		{
			linkref->SetWorldTrans(NPVector3((f32)pos.x - (f32)prop->get_entity()->GetLeftPos(), (f32)pos.y, 0.0f));
		}
	}
}
xui_method_explain(onity_boundbox_entitynode, ori_bounding,	xui_rect2d<s32>	)( void )
{
	NPRect rt = NPRect::Empty;

	onity_propentitynode* prop = dynamic_cast<onity_propentitynode*>(m_linkprop);
	BreezeGame::WGVisualComponent* component = prop->get_entity()->GetComponent<BreezeGame::WGVisualComponent>();
	if (component)
	{
		cal_bounding(component->GetNode(), &rt);
	}

	return xui_rect2d<s32>(rt.LT, rt.TP, rt.RT, rt.BM);
}
xui_method_explain(onity_boundbox_entitynode, was_selected, bool			)( void )
{
	onity_propentitynode* prop = dynamic_cast<onity_propentitynode*>(m_linkprop);
	xui_treeview* treeview = onity_mainform::get_ptr()->get_hierarchy()->get_treeview();
	xui_treedata* treedata = prop->get_linkdata(treeview);
	return treedata->get_node()->was_selected();
}

/*
//method
*/
xui_method_explain(onity_boundbox_entitynode, cal_bounding,	void			)( NPNode* node, NPRect* result )
{
	if (node && node->WasVisible())
	{
		if (NPIsExaKindOf(NP2DSImageRef, node) ||
			NPIsExaKindOf(NP2DSFrameRef, node) ||
			NPIsSubKindOf(NP2DSActorRef, node))
		{
			NP2DSTransRef* transref = NPDynamicCast(NP2DSTransRef, node);
			if (transref->HasAsset())
				(*result) = result->GetUnion(transref->GetFinalBounding());
		}

		if (NPIsExaKindOf(NPNode,		 node) == false &&
			NPIsExaKindOf(NP2DSTransRef, node) == false &&
			NPIsExaKindOf(NP2DSActorRef, node) == false &&
			NPIsExaKindOf(NP2DSFrameRef, node) == false &&
			NPIsExaKindOf(NP2DSImageRef, node) == false)
			return;

		std::list<NPRenderObject*> children = node->GetChildren();
		for (std::list<NPRenderObject*>::iterator itor = children.begin(); itor != children.end(); ++itor)
		{
			NPNode* child = NPDynamicCast(NPNode, (*itor));
			cal_bounding(child, result);
		}
	}
}