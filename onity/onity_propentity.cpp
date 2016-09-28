#include "NP2DSTransRef.h"
#include "Entity.h"
#include "Component/Transform/TransformComponent.h"
#include "Component/InterAction/IAComponent.h"
#include "Entity/Component/Visual/WGVisualComponent.h"
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
#include "onity_propentity.h"

/*
//constructor
*/
xui_create_explain(onity_propentity)( Omiga::Entity* entity )
: m_entity(entity)
, m_transkind(NULL)
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

	if (transform)
	{
		m_transkind = new xui_propkind(this, L"Transform", "TransformComponent", xui_kindctrl::create, NULL, true);
		m_transkind->add_propdata(new xui_propdata_vector(
			m_transkind, 
			L"Position", 
			xui_propctrl_vector::create, 
			get_position, 
			set_position, 
			this, 
			NT_INT));
		m_transkind->add_propdata(new xui_propdata_vector(
			m_transkind,
			L"Scale",
			xui_propctrl_vector::create,
			get_scale,
			set_scale,
			this,
			NT_FLOAT));
		m_transkind->add_propdata(new xui_propdata_number_func(
			m_transkind,
			L"Rotation",
			xui_propctrl_number::create,
			get_rotation,
			set_rotation,
			this,
			NT_INT));
	}

	if (visual)
	{
		m_graphkind = new xui_propkind(this, L"Visual", "VisualComponent", xui_kindctrl::create, NULL, true);
		m_graphkind->add_propdata(new onity_propdata_rendertree(
			m_graphkind,
			L"Graphics",
			visual->GetNode()));
	}

	if (physics)
	{
		m_boundkind = new xui_propkind(this, L"Collision", "PhysicsComponent", xui_kindctrl::create, NULL, true);
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
		m_box2dkind = new xui_propkind(this, L"Box2d", "Box2dComponent", xui_kindctrl::create, NULL, true);
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
		m_logickind = new xui_propkind(this, L"AI", "AIComponent", xui_kindctrl::create, NULL, true);
	}

	if (ia && false)
	{
		m_inputkind = new xui_propkind(this, L"IA", "IAComponent", xui_kindctrl::create, NULL, true);
	}

	if (m_transkind)	add_propkind(m_transkind);
	if (m_graphkind)	add_propkind(m_graphkind);
	if (m_boundkind)	add_propkind(m_boundkind);
	if (m_box2dkind)	add_propkind(m_box2dkind);
	if (m_logickind)	add_propkind(m_logickind);
	if (m_inputkind)	add_propkind(m_inputkind);
}

/*
//method
*/
xui_method_explain(onity_propentity, get_entity,		Omiga::Entity*	)( void )
{
	return m_entity;
}

/*
//static
*/
xui_method_explain(onity_propentity, get_position,		xui_vector<f64>	)( void* userptr )
{
	onity_propentity*				prop		= (onity_propentity*)userptr;
	Omiga::Entity*					entity		= prop->get_entity();
	Omiga::TransformComponent*		transform	= entity->GetComponent<Omiga::TransformComponent>();
	Omiga::Vec2Df					position	= transform->GetPosition();
	return xui_vector<f64>((f64)position.x, (f64)position.y);
}
xui_method_explain(onity_propentity, set_position,		void			)( void* userptr, const xui_vector<f64>& value )
{
	onity_propentity*				prop		= (onity_propentity*)userptr;
	Omiga::Entity*					entity		= prop->get_entity();
	Omiga::TransformComponent*		transform	= entity->GetComponent<Omiga::TransformComponent>();
	transform->SetPosition(Omiga::Vec2Df((f32)value.x, (f32)value.y));
}
xui_method_explain(onity_propentity, get_scale,			xui_vector<f64>	)( void* userptr )
{
	onity_propentity*				prop		= (onity_propentity*)userptr;
	Omiga::Entity*					entity		= prop->get_entity();
	Omiga::TransformComponent*		transform	= entity->GetComponent<Omiga::TransformComponent>();
	Omiga::Vec2Df					scale		= transform->GetScale();
	return xui_vector<f64>((f64)scale.x, (f64)scale.y);
}
xui_method_explain(onity_propentity, set_scale,			void			)( void* userptr, const xui_vector<f64>& value )
{
	onity_propentity*				prop		= (onity_propentity*)userptr;
	Omiga::Entity*					entity		= prop->get_entity();
	Omiga::TransformComponent*		transform	= entity->GetComponent<Omiga::TransformComponent>();
	transform->SetScale(Omiga::Vec2Df((f32)value.x, (f32)value.y));
}
xui_method_explain(onity_propentity, get_rotation,		f64				)( void* userptr )
{
	onity_propentity*				prop		= (onity_propentity*)userptr;
	Omiga::Entity*					entity		= prop->get_entity();
	Omiga::TransformComponent*		transform	= entity->GetComponent<Omiga::TransformComponent>();
	return (f64)transform->GetRotation();
}
xui_method_explain(onity_propentity, set_rotation,		void			)( void* userptr, f64   value )
{
	onity_propentity*				prop		= (onity_propentity*)userptr;
	Omiga::Entity*					entity		= prop->get_entity();
	Omiga::TransformComponent*		transform	= entity->GetComponent<Omiga::TransformComponent>();
	transform->SetRotation((f32)value);
}
xui_method_explain(onity_propentity, was_collidable,	bool			)( void* userptr )
{
	onity_propentity*				prop		= (onity_propentity*)userptr;
	Omiga::Entity*					entity		= prop->get_entity();
	BreezeGame::PhysicsComponent*	physics		= entity->GetComponent<BreezeGame::PhysicsComponent>();
	return physics->IsCollidable();
}
xui_method_explain(onity_propentity, get_collision,		xui_rect2d<f64> )( void* userptr )
{
	onity_propentity*				prop		= (onity_propentity*)userptr;
	Omiga::Entity*					entity		= prop->get_entity();
	BreezeGame::PhysicsComponent*	physics		= entity->GetComponent<BreezeGame::PhysicsComponent>();
	Omiga::ORect<Float32>			box			= physics->GetBodyCollision();
	return xui_rect2d<f64>((f64)box.Left(), (f64)box.Top(), (f64)box.Right(), (f64)box.Bottom());
}
xui_method_explain(onity_propentity, get_phyactor,		void*			)( void* userptr )
{
	onity_propentity*				prop		= (onity_propentity*)userptr;
	Omiga::Entity*					entity		= prop->get_entity();
	BreezeGame::JPBox2dPhysComp*	box2d		= entity->GetComponent<BreezeGame::JPBox2dPhysComp>();
	return box2d->GetPhysActor();
}
xui_method_explain(onity_propentity, set_phyactor,		void			)( void* userptr, void* value )
{}
xui_method_explain(onity_propentity, get_velocity,		xui_vector<f64>	)( void* userptr )
{
	onity_propentity*				prop		= (onity_propentity*)userptr;
	Omiga::Entity*					entity		= prop->get_entity();
	BreezeGame::JPBox2dPhysComp*	box2d		= entity->GetComponent<BreezeGame::JPBox2dPhysComp>();
	b2Body*							body		= box2d->GetBody();
	b2Vec2							velocity	= body->GetLinearVelocity();
	return xui_vector<f64>((f64)velocity.x, (f64)velocity.y);
}
xui_method_explain(onity_propentity, set_velocity,		void			)( void* userptr, const xui_vector<f64>& value )
{
	onity_propentity*				prop		= (onity_propentity*)userptr;
	Omiga::Entity*					entity		= prop->get_entity();
	BreezeGame::JPBox2dPhysComp*	box2d		= entity->GetComponent<BreezeGame::JPBox2dPhysComp>();
	b2Body*							body		= box2d->GetBody();
	body->SetLinearVelocity(b2Vec2((f32)value.x, (f32)value.y));
}
xui_method_explain(onity_propentity, get_damping,		f64				)( void* userptr )
{
	onity_propentity*				prop		= (onity_propentity*)userptr;
	Omiga::Entity*					entity		= prop->get_entity();
	BreezeGame::JPBox2dPhysComp*	box2d		= entity->GetComponent<BreezeGame::JPBox2dPhysComp>();
	b2Body*							body		= box2d->GetBody();
	return (f64)body->GetLinearDamping();
}
xui_method_explain(onity_propentity, set_damping,		void			)( void* userptr, f64   value )
{
	onity_propentity*				prop		= (onity_propentity*)userptr;
	Omiga::Entity*					entity		= prop->get_entity();
	BreezeGame::JPBox2dPhysComp*	box2d		= entity->GetComponent<BreezeGame::JPBox2dPhysComp>();
	b2Body*							body		= box2d->GetBody();
	body->SetLinearDamping((f32)value);
}
xui_method_explain(onity_propentity, get_gravity,		f64				)( void* userptr )
{
	onity_propentity*				prop		= (onity_propentity*)userptr;
	Omiga::Entity*					entity		= prop->get_entity();
	BreezeGame::JPBox2dPhysComp*	box2d		= entity->GetComponent<BreezeGame::JPBox2dPhysComp>();
	b2Body*							body		= box2d->GetBody();
	return (f64)body->GetGravityScale();
}
xui_method_explain(onity_propentity, set_gravity,		void			)( void* userptr, f64   value )
{
	onity_propentity*				prop		= (onity_propentity*)userptr;
	Omiga::Entity*					entity		= prop->get_entity();
	BreezeGame::JPBox2dPhysComp*	box2d		= entity->GetComponent<BreezeGame::JPBox2dPhysComp>();
	b2Body*							body		= box2d->GetBody();
	body->SetGravityScale((f32)value);
}