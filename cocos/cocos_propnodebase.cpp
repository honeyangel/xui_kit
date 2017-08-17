#include "2d/CCNode.h"
#include "2d/CCSprite.h"
#include "2d/CCParticleSystemQuad.h"
#include "ui/UILayoutComponent.h"
#include "cocostudio/CCComExtensionData.h"

#include "xui_global.h"
#include "xui_numbbox.h"
#include "xui_kindctrl.h"
#include "xui_propkind.h"
#include "xui_propdata.h"
#include "xui_propctrl.h"
#include "xui_treedata.h"
#include "xui_treenode.h"
#include "xui_propctrl_vector.h"
#include "xui_propctrl_slider.h"
#include "cocos_boundbox.h"
#include "cocos_mainform.h"
#include "cocos_hierarchy.h"
#include "cocos_propcsd.h"
#include "cocos_propnodelayer.h"
#include "cocos_propnodeparticle.h"
#include "cocos_propnodereal.h"
#include "cocos_propnodesprite.h"
#include "cocos_nodedata.h"
#include "cocos_resource.h"
#include "cocos_propctrl_location.h"
#include "cocos_propnodebase.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodebase)( xui_bitmap* icon, cocos_propcsd* file, cocos2d::Node* node )
: cocos_proproot()
, m_icon(icon)
, m_file(file)
, m_node(node)
, m_boundbox(NULL)
{
	cocos2d::Vector<cocos2d::Node*> children = node->getChildren();
	for (s32 i = 0; i < children.size(); ++i)
	{
		m_leafprop.push_back(new_prop(file, children.at(i)));
	}

	add_title();
}

/*
//destructor
*/
xui_delete_explain(cocos_propnodebase)( void )
{
	delete m_node;
	delete m_boundbox;
	for (u32 i = 0; i < m_leafprop.size(); ++i)
		delete m_leafprop[i];
}

/*
//method
*/
xui_method_explain(cocos_propnodebase, get_icon,		xui_bitmap*				)( void )
{
	return m_icon;
}
xui_method_explain(cocos_propnodebase, get_file,		cocos_propcsd*			)( void )
{
	return m_file;
}
xui_method_explain(cocos_propnodebase, get_node,		cocos2d::Node*			)( void )
{
	return m_node;
}
xui_method_explain(cocos_propnodebase, get_boundbox,	cocos_boundbox*			)( void )
{
	return m_boundbox;
}
xui_method_explain(cocos_propnodebase, get_leafprop,	const xui_proproot_vec&	)( void ) const
{
	return m_leafprop;
}
xui_method_explain(cocos_propnodebase, del_propnode,	void					)( cocos_propnodebase* prop )
{
	xui_proproot_vec::iterator itor = std::find(
		m_leafprop.begin(),
		m_leafprop.end(),
		prop);

	if (itor != m_leafprop.end())
		m_leafprop.erase(itor);
}
xui_method_explain(cocos_propnodebase, add_propnode,	void					)( cocos_propnodebase* prop, u32 index /* = -1 */ )
{
	if (index == -1)
	{
		m_leafprop.push_back(prop);
	}
	else
	{
		xui_proproot_vec::iterator itor = m_leafprop.begin();
		std::advance(itor, index);
		m_leafprop.insert(itor, prop);
	}
}

/*
//event
*/
xui_method_explain(cocos_propnodebase, on_propchanged,	void					)( xui_component* sender, xui_method_propdata& args )
{
	m_file->set_modify(true);
}

/*
//static
*/
xui_method_explain(cocos_propnodebase, get_type,		std::wstring			)( cocos2d::Node* node )
{
	if		(dynamic_cast<cocos2d::ParticleSystemQuad*>(node))	return L"Particle";
	else if (dynamic_cast<cocos2d::Sprite*>(node))				return L"Sprite";
	else
	{
		if (node->getParent() == NULL)
			return L"Layer";
		else
			return L"Node";
	}
}
xui_method_explain(cocos_propnodebase, new_prop,		cocos_propnodebase*		)( cocos_propcsd* file, cocos2d::Node* node )
{
	std::wstring rtti = get_type(node);
	if		(rtti == L"Particle")	return new cocos_propnodeparticle	(file, node);
	else if (rtti == L"Sprite")		return new cocos_propnodesprite		(file, node);
	else if (rtti == L"Layer")		return new cocos_propnodelayer		(file, node);
	else if (rtti == L"Node")		return new cocos_propnodereal		(file, node);
	else
	{
		return NULL;
	}
}
xui_method_explain(cocos_propnodebase, new_prop,		cocos_propnodebase*		)( cocos_propcsd* file, const std::wstring& rtti )
{
	cocos2d::Node* node = NULL;
	if		(rtti == L"Particle")	node = cocos2d::ParticleSystemQuad::create();
	else if (rtti == L"Sprite")		node = cocos2d::Sprite::create();
	else if (rtti == L"Node")		node = cocos2d::Node::create();
	else
	{
		return NULL;
	}

	node->setName(xui_global::unicode_to_ascii(rtti));
	return new_prop(file, node);
}

/*
//method
*/
xui_method_explain(cocos_propnodebase, add_title,		void					)( void )
{
	xui_propkind* kind = new xui_propkind(this, get_type(m_node), "NodeTitle", xui_kindctrl::create, m_icon, true);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodebase>(this, &cocos_propnodebase::on_propchanged);
	kind->add_propdata(new xui_propdata_string(
		kind,
		L"Name",
		xui_propctrl_string::create,
		get_name,
		set_name,
		this));

	add_propkind(kind);
}
xui_method_explain(cocos_propnodebase, add_location,	void					)( void )
{
	add_propkind(new xui_propkind(
		this, 
		L"Location", 
		"NodeLocation", 
		xui_kindctrl::create, 
		m_icon, 
		true,
		true,
		false,
		true));
}
xui_method_explain(cocos_propnodebase, add_anchor,		void					)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "NodeAnchor", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodebase>(this, &cocos_propnodebase::on_propchanged);
	kind->add_propdata(new xui_propdata_vector(
		kind,
		L"Anchor",
		xui_propctrl_vector::create,
		get_anchor,
		set_anchor,
		this,
		NT_FLOAT));

	add_propkind(kind);
}
xui_method_explain(cocos_propnodebase, add_position,	void					)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "NodePosition", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodebase>(this, &cocos_propnodebase::on_propchanged);
	kind->add_propdata(new cocos_propdata_unitvec(
		kind,
		L"Position",
		cocos_propctrl_unitvecpt::create,
		get_position,
		set_position,
		NULL,
		this));

	add_propkind(kind);
}
xui_method_explain(cocos_propnodebase, add_size,		void					)( bool editsize, cocos_propdata_unitvec::get_func def_func )
{
	xui_propkind* kind = new xui_propkind(this, L"", "NodeSize", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodebase>(this, &cocos_propnodebase::on_propchanged);
	kind->add_propdata(new cocos_propdata_unitvec(
		kind,
		L"Size",
		cocos_propctrl_unitvecsz::create,
		get_size,
		editsize ? set_size : NULL,
		def_func,
		this));

	add_propkind(kind);
}
xui_method_explain(cocos_propnodebase, add_aligndock,	void					)( bool editsize )
{
	xui_propkind* kind = new xui_propkind(this, L"", "NodeAlignment", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodebase>(this, &cocos_propnodebase::on_propchanged);
	kind->add_propdata(new cocos_propdata_location(kind, this, editsize));
	add_propkind(kind);
}
xui_method_explain(cocos_propnodebase, add_base,		void					)( void )
{
	add_propkind(new xui_propkind(
		this,
		L"Base",
		"NodeBase",
		xui_kindctrl::create,
		m_icon,
		true,
		true,
		false,
		true));
}
xui_method_explain(cocos_propnodebase, add_common,		void					)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "NodeCommon", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodebase>(this, &cocos_propnodebase::on_propchanged);
	kind->add_propdata(new xui_propdata_bool(
		kind,
		L"Visible",
		xui_propctrl_bool::create,
		was_visible,
		set_visible,
		this));
	kind->add_propdata(new xui_propdata_number_func(
		kind,
		L"Tag",
		xui_propctrl_number::create,
		get_tag,
		set_tag,
		this,
		NT_INT));
	kind->add_propdata(new xui_propdata_vector(
		kind,
		L"Scale",
		xui_propctrl_vector::create,
		get_scale,
		set_scale,
		this,
		NT_FLOAT));
	kind->add_propdata(new xui_propdata_number_func(
		kind,
		L"Rotation",
		xui_propctrl_number::create,
		get_rotation,
		set_rotation,
		this,
		NT_FLOAT));
	kind->add_propdata(new xui_propdata_vector(
		kind,
		L"Bias",
		xui_propctrl_vector::create,
		get_bias,
		set_bias,
		this,
		NT_FLOAT));
	kind->add_propdata(new xui_propdata_number_func(
		kind,
		L"ZOrder",
		xui_propctrl_number::create,
		get_zorder,
		set_zorder,
		this,
		NT_INT));

	add_propkind(kind);

}
xui_method_explain(cocos_propnodebase, add_color,		void					)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "NodeColor", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodebase>(this, &cocos_propnodebase::on_propchanged);
	kind->add_propdata(new xui_propdata_colour(
		kind,
		L"Colour",
		xui_propctrl_colour::create,
		get_color,
		set_color,
		this));
	kind->add_propdata(new xui_propdata_number_func(
		kind,
		L"Opacity",
		xui_propctrl_slider::create,
		get_alpha,
		set_alpha,
		this,
		NT_UNSIGNEDINT,
		1,
		0,
		100));

	add_propkind(kind);
}
xui_method_explain(cocos_propnodebase, add_special,		void					)( void )
{
	add_propkind(new xui_propkind(
		this,
		L"Special",
		"NodeSpecial",
		xui_kindctrl::create,
		m_icon,
		true,
		true,
		false,
		true));
}
xui_method_explain(cocos_propnodebase, add_advance,		void					)( void )
{
	add_propkind(new xui_propkind(
		this,
		L"Advance",
		"NodeAdvance",
		xui_kindctrl::create,
		m_icon,
		true,
		true,
		false,
		true));
}
xui_method_explain(cocos_propnodebase, add_userdata,	void					)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "NodeUserData", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodebase>(this, &cocos_propnodebase::on_propchanged);
	kind->add_propdata(new xui_propdata_string(
		kind,
		L"UserData",
		xui_propctrl_string::create,
		get_userdata,
		set_userdata,
		this));

	add_propkind(kind);
}

/*
//static
*/
xui_method_explain(cocos_propnodebase, get_name,		std::wstring			)( void* userptr )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	return xui_global::ascii_to_unicode(prop->get_node()->getName());
}
xui_method_explain(cocos_propnodebase, set_name,		void					)( void* userptr, const std::wstring& value )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	prop->get_node()->setName(xui_global::unicode_to_ascii(value));
}

xui_method_explain(cocos_propnodebase, get_anchor,		xui_vector<f64>			)( void* userptr )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	cocos2d::ui::LayoutComponent* component = cocos2d::ui::LayoutComponent::bindLayoutComponent(prop->get_node());
	cocos2d::Point anchor = component->getAnchorPosition();
	return xui_vector<f64>(anchor.x, anchor.y);
}
xui_method_explain(cocos_propnodebase, set_anchor,		void					)( void* userptr, const xui_vector<f64>& value )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	cocos2d::ui::LayoutComponent* component = cocos2d::ui::LayoutComponent::bindLayoutComponent(prop->get_node());
	component->setAnchorPosition(cocos2d::Point(value.x, value.y));
}
xui_method_explain(cocos_propnodebase, get_position,	cocos_value_unitvec		)( void* userptr )
{
	cocos_value_unitvec result;
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	cocos2d::ui::LayoutComponent* component = cocos2d::ui::LayoutComponent::bindLayoutComponent(prop->get_node());
	cocos2d::Point pos = component->getPosition();
	result.x.useper = component->isPositionPercentXEnabled();
	result.x.per	= component->getPositionPercentX()*100;
	result.x.pix	= pos.x;
	result.y.useper	= component->isPositionPercentYEnabled();
	result.y.per	= component->getPositionPercentY()*100;
	result.y.pix	= pos.y;

	return result;
}
xui_method_explain(cocos_propnodebase, set_position,	void					)( void* userptr, const cocos_value_unitvec& value )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	cocos2d::ui::LayoutComponent* component = cocos2d::ui::LayoutComponent::bindLayoutComponent(prop->get_node());
	component->setPositionPercentXEnabled(value.x.useper);
	component->setPositionPercentYEnabled(value.y.useper);
	component->setPosition(cocos2d::Point(value.x.pix, value.y.pix));
	if (value.x.useper) component->setPositionPercentX(value.x.per/100);
	if (value.y.useper) component->setPositionPercentY(value.y.per/100);

	component->refreshLayout();
}
xui_method_explain(cocos_propnodebase, get_size,		cocos_value_unitvec		)( void* userptr )
{
	cocos_value_unitvec result;
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	cocos2d::ui::LayoutComponent* component = cocos2d::ui::LayoutComponent::bindLayoutComponent(prop->get_node());
	cocos2d::Size size = component->getSize();
	result.w.useper = component->isPercentWidthEnabled ();
	result.w.per	= component->getPercentWidth ()*100;
	result.w.pix	= size.width;
	result.h.useper = component->isPercentHeightEnabled();
	result.h.per	= component->getPercentHeight()*100;
	result.h.pix	= size.height;

	return result;
}
xui_method_explain(cocos_propnodebase, set_size,		void					)( void* userptr, const cocos_value_unitvec& value )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	cocos2d::ui::LayoutComponent* component = cocos2d::ui::LayoutComponent::bindLayoutComponent(prop->get_node());
	component->setPercentWidthEnabled (value.w.useper);
	component->setPercentHeightEnabled(value.h.useper);
	component->setSize(cocos2d::Size(value.w.pix, value.h.pix));
	if (value.w.useper) component->setPercentWidth (value.w.per/100);
	if (value.h.useper) component->setPercentHeight(value.h.per/100);

	component->refreshLayout();
}
xui_method_explain(cocos_propnodebase, was_visible,		bool					)( void* userptr )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	return prop->get_node()->isVisible();
}
xui_method_explain(cocos_propnodebase, set_visible,		void					)( void* userptr, bool value )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	prop->get_node()->setVisible(value);

	cocos_hierarchy* hierarchy = cocos_mainform::get_ptr()->get_hierarchy();
	xui_treedata* data = prop->get_linkdata(hierarchy->get_treeview());
	if (data)
	{
		data->get_node()->use_linkdata();
	}
}
xui_method_explain(cocos_propnodebase, get_tag,			f64						)( void* userptr )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	return prop->get_node()->getTag();
}
xui_method_explain(cocos_propnodebase, set_tag,			void					)( void* userptr, f64  value )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	prop->get_node()->setTag(value);
}
xui_method_explain(cocos_propnodebase, get_scale,		xui_vector<f64>			)( void* userptr )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	f64 x = prop->get_node()->getScaleX();
	f64 y = prop->get_node()->getScaleY();
	return xui_vector<f64>(x*100, y*100);
}
xui_method_explain(cocos_propnodebase, set_scale,		void					)( void* userptr, const xui_vector<f64>& value )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	prop->get_node()->setScaleX((f32)(value.x/100));
	prop->get_node()->setScaleY((f32)(value.y/100));
}
xui_method_explain(cocos_propnodebase, get_rotation,	f64						)( void* userptr )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	return prop->get_node()->getRotation();
}
xui_method_explain(cocos_propnodebase, set_rotation,	void					)( void* userptr, f64  value )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	prop->get_node()->setRotation(value);
}
xui_method_explain(cocos_propnodebase, get_bias,		xui_vector<f64>			)( void* userptr )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	f64 x = prop->get_node()->getRotationSkewX();
	f64 y = prop->get_node()->getRotationSkewY();
	return xui_vector<f64>(x, y);
}
xui_method_explain(cocos_propnodebase, set_bias,		void					)( void* userptr, const xui_vector<f64>& value )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	prop->get_node()->setRotationSkewX(value.x);
	prop->get_node()->setRotationSkewY(value.y);
}
xui_method_explain(cocos_propnodebase, get_zorder,		f64						)( void* userptr )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	return prop->get_node()->getGlobalZOrder();
}
xui_method_explain(cocos_propnodebase, set_zorder,		void					)( void* userptr, f64  value )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	prop->get_node()->setGlobalZOrder(value);
}
xui_method_explain(cocos_propnodebase, get_color,		xui_colour				)( void* userptr )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	cocos2d::Color3B color = prop->get_node()->getColor();
	return xui_colour(1.0f, color.r/255.0f, color.g/255.0f, color.b/255.0f);
}
xui_method_explain(cocos_propnodebase, set_color,		void					)( void* userptr, const xui_colour& value )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	prop->get_node()->setColor(cocos2d::Color3B(value.r*255.0f, value.g*255.0f, value.b*255.0f));
}
xui_method_explain(cocos_propnodebase, get_alpha,		f64						)( void* userptr )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	return (s32)(prop->get_node()->getOpacity()/255.0f*100.0f);
}
xui_method_explain(cocos_propnodebase, set_alpha,		void					)( void* userptr, f64 value )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	prop->get_node()->setOpacity(value/100.0f*255.0f);
}
xui_method_explain(cocos_propnodebase, get_userdata,	std::wstring			)( void* userptr )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	cocostudio::ComExtensionData* extensionData = (cocostudio::ComExtensionData*)prop->get_node()->getComponent(cocostudio::ComExtensionData::COMPONENT_NAME);
	if (extensionData)
		return xui_global::ascii_to_unicode(extensionData->getCustomProperty());

	return L"";
}
xui_method_explain(cocos_propnodebase, set_userdata,	void					)( void* userptr, const std::wstring& value )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	cocostudio::ComExtensionData* extensionData = (cocostudio::ComExtensionData*)prop->get_node()->getComponent(cocostudio::ComExtensionData::COMPONENT_NAME);
	if (extensionData == NULL)
	{
		extensionData = cocostudio::ComExtensionData::create();
		prop->get_node()->addComponent(extensionData);
	}

	extensionData->setCustomProperty(xui_global::unicode_to_ascii(value));
}