#include "2d/CCNode.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrame.h"
#include "2d/CCParticleSystemQuad.h"
#include "ui/UILayoutComponent.h"
#include "ui/UIWidget.h"
#include "ui/UIButton.h"
#include "ui/UICheckBox.h"
#include "ui/UIImageView.h"
#include "ui/UIText.h"
#include "ui/UITextBMFont.h"
#include "ui/UILoadingBar.h"
#include "ui/UISlider.h"
#include "ui/UITextField.h"
#include "ui/UILayout.h"
#include "ui/UIPageView.h"
#include "ui/UIListView.h"
#include "ui/UIScrollView.h"
#include "cocostudio/CCComExtensionData.h"
#include "external/LayoutCenter.h"
#include "external/AdapterNode.h"
#include "external/ShaderRect.h"
#include "external/TextIFBM.h"
#include "external/SpineNode.h"
#include "external/SpineBlender.h"
#include "2d/WeCLabel.h"

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
#include "cocos_propnodebutton.h"
#include "cocos_propnodecheckbox.h"
#include "cocos_propnodeimageview.h"
#include "cocos_propnodetext.h"
#include "cocos_propnodetextbmfont.h"
#include "cocos_propnodeloadingbar.h"
#include "cocos_propnodeslider.h"
#include "cocos_propnodetextfield.h"
#include "cocos_propnodepanel.h"
#include "cocos_propnodescroll.h"
#include "cocos_propnodepageview.h"
#include "cocos_propnodelistview.h"
#include "cocos_propnodelayoutcenter.h"
#include "cocos_propnodeadapter.h"
#include "cocos_propnodeshaderrect.h"
#include "cocos_propnodetextifbm.h"
#include "cocos_propnodeweclabel.h"
#include "cocos_propnodespinenode.h"
#include "cocos_propnodespineblender.h"
#include "cocos_propctrl_flip.h"
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
{
	m_boundbox = new cocos_boundbox(this);
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
	else if (dynamic_cast<cocos2d::ui::Button*>(node))			return L"Button";
	else if (dynamic_cast<cocos2d::ui::CheckBox*>(node))		return L"CheckBox";
	else if (dynamic_cast<cocos2d::ui::ImageView*>(node))		return L"ImageView";
	else if (dynamic_cast<cocos2d::ui::Text*>(node))			return L"Text";
	else if (dynamic_cast<cocos2d::ui::TextBMFont*>(node))		return L"TextBMFont";
	else if (dynamic_cast<cocos2d::ui::LoadingBar*>(node))		return L"LoadingBar";
	else if (dynamic_cast<cocos2d::ui::Slider*>(node))			return L"Slider";
	else if (dynamic_cast<cocos2d::ui::TextField*>(node))		return L"TextField";
	else if (dynamic_cast<cocos2d::ui::ListView*>(node))		return L"ListView";
	else if (dynamic_cast<cocos2d::ui::PageView*>(node))		return L"PageView";
	else if (dynamic_cast<cocos2d::ui::ScrollView*>(node))		return L"ScrollView";
	else if (dynamic_cast<cocos2d::ui::Layout*>(node))			return L"Layout";
	else if (dynamic_cast<cocos2d::ui::LayoutCenter*>(node))	return L"LayoutCenter";
	else if (dynamic_cast<cocos2d::ui::AdapterNode*>(node))		return L"AdapterNode";
	else if (dynamic_cast<cocos2d::ui::ShaderRect*>(node))		return L"ShaderRect";
	else if (dynamic_cast<cocos2d::ui::TextIFBM*>(node))		return L"TextIFBM";
	else if (dynamic_cast<cocos2d::ui::WeCLabel*>(node))		return L"WeCLabel";
	else if (dynamic_cast<cocos2d::ui::SpineNode*>(node))		return L"SpineNode";
	else if (dynamic_cast<cocos2d::ui::SpineBlender*>(node))	return L"SpineBlender";
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
	if		(rtti == L"Particle")		return new cocos_propnodeparticle		(file, node);
	else if (rtti == L"Sprite")			return new cocos_propnodesprite			(file, node);
	else if (rtti == L"Layer")			return new cocos_propnodelayer			(file, node);
	else if (rtti == L"Node")			return new cocos_propnodereal			(file, node);
	else if (rtti == L"Button")			return new cocos_propnodebutton			(file, node);
	else if (rtti == L"CheckBox")		return new cocos_propnodecheckbox		(file, node);
	else if (rtti == L"ImageView")		return new cocos_propnodeimageview		(file, node);
	else if (rtti == L"Text")			return new cocos_propnodetext			(file, node);
	else if (rtti == L"TextBMFont")		return new cocos_propnodetextbmfont		(file, node);
	else if (rtti == L"LoadingBar")		return new cocos_propnodeloadingbar		(file, node);
	else if (rtti == L"Slider")			return new cocos_propnodeslider			(file, node);
	else if (rtti == L"TextField")		return new cocos_propnodetextfield		(file, node);
	else if (rtti == L"Layout")			return new cocos_propnodelayout			(file, node);
	else if (rtti == L"ScrollView")		return new cocos_propnodescrollview		(file, node);
	else if (rtti == L"ListView")		return new cocos_propnodelistview		(file, node);
	else if (rtti == L"PageView")		return new cocos_propnodepageview		(file, node);
	else if (rtti == L"LayoutCenter")	return new cocos_propnodelayoutcenter	(file, node);
	else if (rtti == L"AdapterNode")	return new cocos_propnodeadapter		(file, node);
	else if (rtti == L"ShaderRect")		return new cocos_propnodeshaderrect		(file, node);
	else if (rtti == L"TextIFBM")		return new cocos_propnodetextifbm		(file, node);
	else if (rtti == L"WeCLabel")		return new cocos_propnodeweclabel		(file, node);
	else if (rtti == L"SpineNode")		return new cocos_propnodespinenode		(file, node);
	else if (rtti == L"SpineBlender")	return new cocos_propnodespineblender	(file, node);
	else
	{
		return NULL;
	}
}
xui_method_explain(cocos_propnodebase, new_prop,		cocos_propnodebase*		)( cocos_propcsd* file, const std::wstring& rtti )
{
	cocos2d::Node* node = NULL;
	if		(rtti == L"Particle")		node = cocos2d::ParticleSystemQuad::create();
	else if (rtti == L"Sprite")			node = cocos2d::Sprite::create();
	else if (rtti == L"Node")			node = cocos2d::Node::create();
	else if (rtti == L"Button")			node = cocos2d::ui::Button::create();
	else if (rtti == L"CheckBox")		node = cocos2d::ui::CheckBox::create();
	else if (rtti == L"ImageView")		node = cocos2d::ui::ImageView::create();
	else if (rtti == L"Text")			node = cocos2d::ui::Text::create();
	else if (rtti == L"TextBMFont")		node = cocos2d::ui::TextBMFont::create();
	else if (rtti == L"LoadingBar")		node = cocos2d::ui::LoadingBar::create();
	else if (rtti == L"Slider")			node = cocos2d::ui::Slider::create();
	else if (rtti == L"TextField")		node = cocos2d::ui::TextField::create();
	else if (rtti == L"Layout")			node = cocos2d::ui::Layout::create();
	else if (rtti == L"ScrollView")		node = cocos2d::ui::ScrollView::create();
	else if (rtti == L"ListView")		node = cocos2d::ui::ListView::create();
	else if (rtti == L"PageView")		node = cocos2d::ui::PageView::create();
	else if (rtti == L"LayoutCenter")	node = cocos2d::ui::LayoutCenter::create();
	else if (rtti == L"AdapterNode")	node = cocos2d::ui::AdapterNode::create();
	else if (rtti == L"ShaderRect")		node = cocos2d::ui::ShaderRect::create();
	else if (rtti == L"TextIFBM")		node = cocos2d::ui::TextIFBM::create();
	else if (rtti == L"WeCLabel")		node = cocos2d::ui::WeCLabel::create();
	else if (rtti == L"SpineNode")		node = cocos2d::ui::SpineNode::create();
	else if (rtti == L"SpineBlender")	node = cocos2d::ui::SpineBlender::create();
	else
	{
		return NULL;
	}

	node->setName(xui_global::unicode_to_ascii(rtti));
	node->setContentSize(cocos2d::Size(100, 100));
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
	m_boundbox->add_operator(OP_PIVOT);

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
	m_boundbox->add_operator(OP_TRANS);

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
	m_boundbox->add_operator(OP_SCALE);

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
xui_method_explain(cocos_propnodebase, add_flip,		void					)( void )
{
	xui_propkind* kind = new xui_propkind(this, L"", "NodeFlip", xui_kindctrl::create, m_icon, true, false);
	kind->xm_propchanged += new xui_method_member<xui_method_propdata, cocos_propnodebase>(this, &cocos_propnodebase::on_propchanged);
	kind->add_propdata(new cocos_propdata_flip(kind, this));
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
	cocos2d::ui::Widget* widget = dynamic_cast<cocos2d::ui::Widget*>(prop->get_node());
	if (widget)
		return widget->getGlobalZOrderOffset();

	return prop->get_node()->getGlobalZOrder();
}
xui_method_explain(cocos_propnodebase, set_zorder,		void					)( void* userptr, f64  value )
{
	cocos_propnodebase* prop = (cocos_propnodebase*)userptr;
	cocos2d::ui::Widget* widget = dynamic_cast<cocos2d::ui::Widget*>(prop->get_node());
	if (widget)
		widget->setGlobalZOffset(value);
	else
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

/*
//helper
*/
xui_method_explain(cocos_propnodebase, get_image,		void*					)( cocos2d::Sprite* sprite )
{
	if (sprite)
	{
		cocos2d::SpriteFrame* frame = sprite->getSpriteFrame(false);
		if (frame && frame->getName().length() > 0)
		{
			return frame;
		}
		else
		{
			return sprite->getTexture();
		}
	}

	return NULL;
}
xui_method_explain(cocos_propnodebase, set_image,		void					)( cocos2d::Sprite* sprite, void* value )
{
	cocos2d::Ref*			base = (cocos2d::Ref*)value;
	cocos2d::Texture2D*		texture = dynamic_cast<cocos2d::Texture2D*>(base);
	cocos2d::SpriteFrame*	frame = dynamic_cast<cocos2d::SpriteFrame*>(base);
	if (frame)
	{
		sprite->initWithSpriteFrame(frame);
	}
	else
	if (texture)
	{
		sprite->initWithTexture(texture);
	}
	else
	{
		sprite->init();
	}
}