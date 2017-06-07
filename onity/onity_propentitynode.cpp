#include "NP2DSTransRef.h"
#include "NP2DSImageRef.h"
#include "NP2DSFrameRef.h"
#include "NP2DSActorRef.h"
#include "Entity.h"
#include "Entity/Component/Transform/TransformComponent.h"
#include "Entity/Component/InterAction/IAComponent.h"
#include "Entity/Component/Visual/WGVisualComponent.h"
#include "Entity/Component/Visual/WGVisualManager.h"
#include "Entity/Component/Physics/PhysicsComponent.h"
#include "Entity/Component/AI/GameObjAIComponent.h"
#include "Game/Jetpack/Base/JPBox2dPhysComp.h"
#include "xui_textbox.h"
#include "xui_kindctrl.h"
#include "xui_propctrl_vector.h"
#include "xui_propctrl_rect2d.h"
#include "xui_propctrl_slider.h"
#include "onity_resource.h"
#include "onity_propctrl_transref.h"
#include "onity_propctrl_rendertree.h"
#include "onity_boundbox_entitynode.h"
#include "onity_propentitynode.h"

/*
//constructor
*/
xui_create_explain(onity_propentitynode)( Omiga::Entity* entity )
: onity_propeditnode()
, m_entity(entity)
, m_graphkind(NULL)
, m_boundkind(NULL)
, m_box2dkind(NULL)
, m_logickind(NULL)
, m_inputkind(NULL)
{
	Omiga::TransformComponent*		transform	= m_entity->GetComponent<Omiga::TransformComponent>();
	BreezeGame::WGVisualComponent*	visual		= m_entity->GetComponent<BreezeGame::WGVisualComponent>();
	BreezeGame::PhysicsComponent*	physics		= m_entity->GetComponent<BreezeGame::PhysicsComponent>();
	BreezeGame::JPBox2dPhysComp*	box2d		= m_entity->GetComponent<BreezeGame::JPBox2dPhysComp>();
	BreezeGame::GameObjAIComponent* ai			= m_entity->GetComponent<BreezeGame::GameObjAIComponent>();
	Omiga::IAComponent*				ia			= m_entity->GetComponent<Omiga::IAComponent>();

	if (visual)
	{
		m_graphkind = new xui_propkind(this, L"Visual", "VisualComponent", xui_kindctrl::create, onity_resource::icon_visual, true);
		m_graphkind->add_propdata(new onity_propdata_rendertree(
			m_graphkind,
			L"Graphics",
			visual->GetNode()));
	}

	if (physics)
	{
		m_boundkind = new xui_propkind(this, L"Collision", "PhysicsComponent", xui_kindctrl::create, onity_resource::icon_physics, true);
		m_boundkind->add_propdata(new xui_propdata_bool(
			m_boundkind,
			L"Collidable",
			xui_propctrl_bool::create,
			was_collidable,
			NULL,
			this));
		m_boundkind->add_propdata(new xui_propdata_rect2d(
			m_boundkind,
			L"Rectangle",
			xui_propctrl_rect2d::create,
			get_collision,
			NULL,
			this,
			NT_UNSIGNEDINT));
	}

	if (box2d)
	{
		m_box2dkind = new xui_propkind(this, L"Box2d", "Box2dComponent", xui_kindctrl::create, onity_resource::icon_physics, true);
		m_box2dkind->add_propdata(new onity_propdata_2dsasset(
			m_box2dkind,
			L"Shape",
			onity_propctrl_asset::create,
			DROPTYPE_ACTOR,
			get_phyactor,
			set_phyactor,
			this));
		m_box2dkind->add_propdata(new xui_propdata_vector(
			m_box2dkind,
			L"Velocity",
			xui_propctrl_vector::create,
			get_velocity,
			set_velocity,
			this,
			NT_FLOAT));
		m_box2dkind->add_propdata(new xui_propdata_number_func(
			m_box2dkind,
			L"Damping",
			xui_propctrl_slider::create,
			get_damping,
			set_damping,
			this,
			NT_FLOAT,
			0.01,
			0,
			1));
		m_box2dkind->add_propdata(new xui_propdata_number_func(
			m_box2dkind,
			L"Gravity",
			xui_propctrl_slider::create,
			get_gravity,
			set_gravity,
			this,
			NT_FLOAT,
			0.1,
			0,
			5));
	}

	if (ai && false)
	{
		m_logickind = new xui_propkind(this, L"AI", "AIComponent", xui_kindctrl::create, onity_resource::icon_ai, true);
	}

	if (ia && false)
	{
		m_inputkind = new xui_propkind(this, L"IA", "IAComponent", xui_kindctrl::create, onity_resource::icon_input, true);
	}

	if (transform)
	{
		add_propkind(m_transkind);
	}
	else
	{
		delete m_transkind;
	}

	if (m_graphkind)	add_propkind(m_graphkind);
	if (m_boundkind)	add_propkind(m_boundkind);
	if (m_box2dkind)	add_propkind(m_box2dkind);
	if (m_logickind)	add_propkind(m_logickind);
	if (m_inputkind)	add_propkind(m_inputkind);

	if (m_entity->GetLinkRef())
	{
		add_propkind(m_paramkind);
	}
	else
	{
		delete m_paramkind;
		m_paramkind = NULL;
	}

	m_boundctrl = new onity_boundbox_entitynode(this);
}

/*
//method
*/
xui_method_explain(onity_propentitynode, get_entity,		Omiga::Entity*	)( void )
{
	return m_entity;
}
xui_method_explain(onity_propentitynode, get_2dsref,		NP2DSTransRef*	)( void )
{
	return m_entity->GetLinkRef();
}
xui_method_explain(onity_propentitynode, get_scale,			xui_vector<f64>	)( void )
{
	Omiga::TransformComponent* transform = m_entity->GetComponent<Omiga::TransformComponent>();
	if (transform)
	{
		Omiga::Vec2Df scale = transform->GetScale();
		return xui_vector<f64>((f64)scale.x, (f64)scale.y);
	}

	return xui_vector<f64>(1.0);
}
xui_method_explain(onity_propentitynode, set_scale,			void			)( const xui_vector<f64>& value )
{
	Omiga::TransformComponent* transform = m_entity->GetComponent<Omiga::TransformComponent>();
	if (transform)
	{
		transform->SetScale(Omiga::Vec2Df((f32)value.x, (f32)value.y));
		NP2DSTransRef* linkref = m_entity->GetLinkRef();
		if (linkref)
		{
			linkref->SetWorldScale(NPVector3((f32)value.x, (f32)value.y, 1.0f));
		}
	}
}
//xui_method_explain(onity_propentitynode, ori_bounding,		xui_rect2d<s32>	)( void )
//{
//	NPRect rt = NPRect::Empty;
//
//	BreezeGame::WGVisualComponent* component = m_entity->GetComponent<BreezeGame::WGVisualComponent>();
//	if (component)
//	{
//		cal_bounding(component->GetNode(), &rt);
//	}
//
//	return xui_rect2d<s32>(rt.LT, rt.TP, rt.RT, rt.BM);
//}
//xui_method_explain(onity_propentitynode, ori_position,		xui_vector<s32>	)( void )
//{
//	Omiga::TransformComponent* transform = m_entity->GetComponent<Omiga::TransformComponent>();
//	if (transform)
//	{
//		Omiga::Vec2Df pos = transform->GetPosition();
//		return xui_vector<s32>((s32)pos.x, (s32)pos.y);
//	}
//
//	return xui_vector<s32>(0);
//}
//xui_method_explain(onity_propentitynode, set_position,		void			)( const xui_vector<s32>& pos )
//{
//	Omiga::TransformComponent* transform = m_entity->GetComponent<Omiga::TransformComponent>();
//	if (transform)
//	{
//		transform->SetPosition(Omiga::Vec2Df((f32)pos.x, (f32)pos.y));
//		NP2DSTransRef* linkref = m_entity->GetLinkRef();
//		if (linkref)
//		{
//			linkref->SetWorldTrans(NPVector3((f32)pos.x - (f32)m_entity->GetLeftPos(), (f32)pos.y, 0.0f));
//		}
//	}
//}

/*
//static
*/
xui_method_explain(onity_propentitynode, was_collidable,	bool			)( void* userptr )
{
	onity_propentitynode*			prop		= (onity_propentitynode*)userptr;
	Omiga::Entity*					entity		= prop->get_entity();
	BreezeGame::PhysicsComponent*	physics		= entity->GetComponent<BreezeGame::PhysicsComponent>();
	return physics->IsCollidable();
}
xui_method_explain(onity_propentitynode, get_collision,		xui_rect2d<f64> )( void* userptr )
{
	onity_propentitynode*			prop		= (onity_propentitynode*)userptr;
	Omiga::Entity*					entity		= prop->get_entity();
	BreezeGame::PhysicsComponent*	physics		= entity->GetComponent<BreezeGame::PhysicsComponent>();
	Omiga::ORect<Float32>			box			= physics->GetBodyCollision();
	return xui_rect2d<f64>((f64)box.Left(), (f64)box.Top(), (f64)box.Right(), (f64)box.Bottom());
}
xui_method_explain(onity_propentitynode, get_phyactor,		void*			)( void* userptr )
{
	onity_propentitynode*			prop		= (onity_propentitynode*)userptr;
	Omiga::Entity*					entity		= prop->get_entity();
	BreezeGame::JPBox2dPhysComp*	box2d		= entity->GetComponent<BreezeGame::JPBox2dPhysComp>();
	return box2d->GetPhysActor();
}
xui_method_explain(onity_propentitynode, set_phyactor,		void			)( void* userptr, void* value )
{}
xui_method_explain(onity_propentitynode, get_velocity,		xui_vector<f64>	)( void* userptr )
{
	onity_propentitynode*			prop		= (onity_propentitynode*)userptr;
	Omiga::Entity*					entity		= prop->get_entity();
	BreezeGame::JPBox2dPhysComp*	box2d		= entity->GetComponent<BreezeGame::JPBox2dPhysComp>();
	b2Body*							body		= box2d->GetBody();
	b2Vec2							velocity	= body->GetLinearVelocity();
	return xui_vector<f64>((f64)velocity.x, (f64)velocity.y);
}
xui_method_explain(onity_propentitynode, set_velocity,		void			)( void* userptr, const xui_vector<f64>& value )
{
	onity_propentitynode*			prop		= (onity_propentitynode*)userptr;
	Omiga::Entity*					entity		= prop->get_entity();
	BreezeGame::JPBox2dPhysComp*	box2d		= entity->GetComponent<BreezeGame::JPBox2dPhysComp>();
	b2Body*							body		= box2d->GetBody();
	body->SetLinearVelocity(b2Vec2((f32)value.x, (f32)value.y));
}
xui_method_explain(onity_propentitynode, get_damping,		f64				)( void* userptr )
{
	onity_propentitynode*			prop		= (onity_propentitynode*)userptr;
	Omiga::Entity*					entity		= prop->get_entity();
	BreezeGame::JPBox2dPhysComp*	box2d		= entity->GetComponent<BreezeGame::JPBox2dPhysComp>();
	b2Body*							body		= box2d->GetBody();
	return (f64)body->GetLinearDamping();
}
xui_method_explain(onity_propentitynode, set_damping,		void			)( void* userptr, f64   value )
{
	onity_propentitynode*			prop		= (onity_propentitynode*)userptr;
	Omiga::Entity*					entity		= prop->get_entity();
	BreezeGame::JPBox2dPhysComp*	box2d		= entity->GetComponent<BreezeGame::JPBox2dPhysComp>();
	b2Body*							body		= box2d->GetBody();
	body->SetLinearDamping((f32)value);
}
xui_method_explain(onity_propentitynode, get_gravity,		f64				)( void* userptr )
{
	onity_propentitynode*			prop		= (onity_propentitynode*)userptr;
	Omiga::Entity*					entity		= prop->get_entity();
	BreezeGame::JPBox2dPhysComp*	box2d		= entity->GetComponent<BreezeGame::JPBox2dPhysComp>();
	b2Body*							body		= box2d->GetBody();
	return (f64)body->GetGravityScale();
}
xui_method_explain(onity_propentitynode, set_gravity,		void			)( void* userptr, f64   value )
{
	onity_propentitynode*			prop		= (onity_propentitynode*)userptr;
	Omiga::Entity*					entity		= prop->get_entity();
	BreezeGame::JPBox2dPhysComp*	box2d		= entity->GetComponent<BreezeGame::JPBox2dPhysComp>();
	b2Body*							body		= box2d->GetBody();
	body->SetGravityScale((f32)value);
}