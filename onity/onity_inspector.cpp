#include "TransformComponent.h"
#include "NP2DSTransRef.h"

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
#include "onity_inspector.h"

xui_implement_rtti(onity_inspector, xui_dockpage);

/*
//constructor
*/
xui_create_explain(onity_inspector)( void )
: xui_dockpage(xui_vector<s32>(300), AREALIMIT_L|AREALIMIT_R, 200, DOCKSTYLE_R)
{
	ini_namectrl(xui_bitmap::create("icon/inspector.png"), L"Inspector");

	m_view = new xui_propview(xui_vector<s32>(0));
	xui_method_ptrcall(m_view, ini_component)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_view, set_borderrt )(xui_rect2d<s32>(4));
	add_pagectrl(m_view);
}

/*
//callback
*/
//DEBUG
class onity_prop_transform : public xui_propkind
{
public:
	static xui_vector<f64>		get_translate	( void* userptr )
	{
		Omiga::TransformComponent* transformComponent = (Omiga::TransformComponent*)userptr;
		Omiga::Vec2Df position = transformComponent->GetPosition();
		return xui_vector<f64>((f64)position.x, (f64)position.y);
	}
	static void					set_translate	( void* userptr, const xui_vector<f64>& value )
	{
		Omiga::TransformComponent* transformComponent = (Omiga::TransformComponent*)userptr;
		transformComponent->SetPosition(Omiga::Vec2Df((f32)value.x, (f32)value.y));
	}
	static xui_vector<f64>		get_scale		( void* userptr )
	{
		Omiga::TransformComponent* transformComponent = (Omiga::TransformComponent*)userptr;
		Omiga::Vec2Df scale = transformComponent->GetScale();
		return xui_vector<f64>((f64)scale.x, (f64)scale.y);
	}
	static void					set_scale		( void* userptr, const xui_vector<f64>& value )
	{
		Omiga::TransformComponent* transformComponent = (Omiga::TransformComponent*)userptr;
		transformComponent->SetScale(Omiga::Vec2Df((f32)value.x, (f32)value.y));
	}
	static void					on_flagchange	( xui_component* sender, xui_method_args& args )
	{

	}

	onity_prop_transform( xui_proproot* root, const std::wstring& name )
	: xui_propkind(root, name, xui_kindctrl::create, xui_bitmap::create("icon/local.png"), true)
	{
		m_rot = 0;
		add_propdata(new xui_propdata_vector_func(this, L"P", xui_propctrl_vector_button::create, onity_prop_transform::get_translate, onity_prop_transform::set_translate, (void*)(&m_transform)));
		add_propdata(new xui_propdata_vector_func(this, L"S", xui_propctrl_vector_button::create, onity_prop_transform::get_scale,     onity_prop_transform::set_scale,     (void*)(&m_transform), 1, xui_vector<f64>(1)));
		add_propdata(new xui_propdata_number_impl<s32>(this, L"Rotation", xui_propctrl_number::create, &m_rot));
		xm_flagchanged += new xui_method_static<xui_method_args>(onity_prop_transform::on_flagchange);
	}

protected:
	Omiga::TransformComponent	m_transform;
	s32							m_rot;
};
class onity_prop_visual : public xui_propkind
{
public:
	onity_prop_visual( xui_proproot* root, const std::wstring& name )
	: xui_propkind(root, name, xui_kindctrl::create, xui_bitmap::create("icon/animator.png"), true)
	{
		add_propdata(new xui_propdata_stdvec_impl<NP2DSTransRef*>(this, L"Nodes", xui_propctrl_stdvec::create, NULL, NULL, onity_prop_visual::stdvec_newprop, &m_test));
		xm_flagchanged += new xui_method_static<xui_method_args>(onity_prop_visual::on_flagchange);
	}

	static void					on_flagchange	( xui_component* sender, xui_method_args& args )
	{

	}
	static xui_bitmap*			transref_geticon( xui_propdata* propdata)
	{
		return xui_bitmap::create("icon/animator.png");
	}
	static std::wstring			transref_getname(xui_propdata* propdata)
	{
		return L"scene node";
	}

	static xui_propdata*		stdvec_newprop	( void* ptr, xui_propkind* kind )
	{
		return new xui_propdata_object_impl<NP2DSTransRef*>(kind, L"", xui_propctrl_object::create, "NPNode", NULL, onity_prop_visual::transref_geticon, onity_prop_visual::transref_getname, (NP2DSTransRef**)ptr);
	}

protected:
	std::vector<NP2DSTransRef*>	m_test;
};
class onity_prop_ai : public xui_propkind
{
public:
	onity_prop_ai( xui_proproot* root, const std::wstring& name )
	: xui_propkind(root, name, xui_kindctrl::create, xui_bitmap::create("icon/game.png"), true)
	{
		m_prop_value = 60;
		m_test_value = 0;
		m_bool_value = true;

		add_propdata(new xui_propdata_number_impl<s32>(this, L"Speed", xui_propctrl_number::create, &m_prop_value));
		add_propdata(new xui_propdata_bool(this, L"Break", xui_propctrl_bool::create, &m_bool_value));
		add_propdata(new xui_propdata_number_impl<s32>(this, L"Gravity", xui_propctrl_slider::create, &m_test_value, 2, -50, 50));
		xm_flagchanged += new xui_method_static<xui_method_args>(onity_prop_ai::on_flagchange);
	}

	static void					on_flagchange	( xui_component* sender, xui_method_args& args )
	{

	}

protected:
	s32							m_prop_value;
	s32							m_test_value;
	bool						m_bool_value;
};

xui_method_explain(onity_inspector, on_load, void)( xui_method_args& args )
{
	xui_dockpage::on_load(args);
	xui_proproot* proproot = new xui_proproot;
	proproot->add_propkind(new onity_prop_transform	(proproot, L"Transform"));
	proproot->add_propkind(new onity_prop_visual	(proproot, L"Visual"));
	proproot->add_propkind(new onity_prop_ai		(proproot, L"AI"));
	m_view->set_proproot(proproot);
}