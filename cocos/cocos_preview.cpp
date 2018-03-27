#include "2d/CCNode.h"
#include "2d/CCScene.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrame.h"
#include "2d/CCSpriteFrameCache.h"
#include "2d/CCParticleSystem.h"
#include "ui/UITextBMFont.h"
#include "SpineNode.h"
#include "spine/SkeletonAnimation.h"
#include "base/CCDirector.h"
#include "xui_global.h"
#include "xui_convas.h"
#include "xui_panel.h"
#include "xui_button.h"
#include "xui_slider.h"
#include "xui_treeview.h"
#include "cocos_filedata.h"
#include "cocos_glview.h"
#include "cocos_proptexture.h"
#include "cocos_propatlas.h"
#include "cocos_propframe.h"
#include "cocos_propparticle.h"
#include "cocos_propttf.h"
#include "cocos_propfnt.h"
#include "cocos_propspine.h"
#include "cocos_renderview.h"
#include "cocos_resource.h"
#include "cocos_mainform.h"
#include "cocos_project.h"
#include "cocos_preview.h"

xui_implement_rtti(cocos_preview, xui_control);

/*
//constructor
*/
xui_create_explain(cocos_preview)( void )
: xui_control(xui_vector<s32>(260))
, m_prop(NULL)
, m_node(NULL)
, m_scale(1.0f)
{
	m_play	= new xui_button(xui_vector<s32>(24));
	xui_method_ptrcall(m_play,	ini_component	)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_play,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_play,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_play,	ini_drawer		)(cocos_resource::icon_play, xui_vector<s32>(16));

	xui_method_ptrcall(m_play,	xm_buttonclick	) += new xui_method_member<xui_method_args,  cocos_preview>(this, &cocos_preview::on_buttonclick);
	m_speed	= xui_slider::create(FLOWSTYLE_H);
	xui_method_ptrcall(m_speed,	ini_component	)(ALIGNHORZ_R, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_speed,	ini_scroll		)(16, 8);
	xui_method_ptrcall(m_speed,	xm_scroll		) += new xui_method_member<xui_method_args,  cocos_preview>(this, &cocos_preview::on_speedscroll);
	m_text	= new xui_drawer(xui_vector<s32>(32, 24));
	xui_method_ptrcall(m_text,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_text,	set_textalign	)(TEXTALIGN_RC);
	xui_method_ptrcall(m_text,	ini_drawer		)(L"1.0x");

	m_head	= new xui_panel(xui_vector<s32>(200, 24));
	xui_method_ptrcall(m_head,	set_parent		)(this);
	xui_method_ptrcall(m_head,	set_borderrt	)(xui_rect2d<s32>(2, 0, 0, 0));
	xui_method_ptrcall(m_head,	set_drawcolor	)(false);
	xui_method_ptrcall(m_head,	ini_component	)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_head,	add_child		)(m_play);
	xui_method_ptrcall(m_head,	add_child		)(m_text);
	xui_method_ptrcall(m_head,	add_child		)(m_speed);
	xui_method_ptrcall(m_head,	xm_perform		) += new xui_method_member<xui_method_args,  cocos_preview>(this, &cocos_preview::on_headperform);

	m_small	= new xui_button(xui_vector<s32>(24));
	xui_method_ptrcall(m_small,	ini_component	)(0, 0, DOCKSTYLE_R);
	xui_method_ptrcall(m_small,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_small,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_small,	ini_drawer		)(cocos_resource::icon_small, xui_vector<s32>(16));
	xui_method_ptrcall(m_small,	xm_buttonclick	) += new xui_method_member<xui_method_args,  cocos_preview>(this, &cocos_preview::on_buttonclick);
	m_large	= new xui_button(xui_vector<s32>(24));
	xui_method_ptrcall(m_large,	ini_component	)(0, 0, DOCKSTYLE_R);
	xui_method_ptrcall(m_large,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_large,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_large,	ini_drawer		)(cocos_resource::icon_large, xui_vector<s32>(16));
	xui_method_ptrcall(m_large,	xm_buttonclick	) += new xui_method_member<xui_method_args,  cocos_preview>(this, &cocos_preview::on_buttonclick);
	m_reset	= new xui_button(xui_vector<s32>(24));
	xui_method_ptrcall(m_reset,	ini_component	)(0, 0, DOCKSTYLE_R);
	xui_method_ptrcall(m_reset,	set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_reset,	set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_reset,	ini_drawer		)(cocos_resource::icon_reset, xui_vector<s32>(16));
	xui_method_ptrcall(m_reset,	xm_buttonclick	) += new xui_method_member<xui_method_args,  cocos_preview>(this, &cocos_preview::on_buttonclick);

	m_tool	= new xui_panel(xui_vector<s32>(200, 24));
	xui_method_ptrcall(m_tool,	set_parent		)(this);
	xui_method_ptrcall(m_tool,	set_drawcolor	)(false);
	xui_method_ptrcall(m_tool,	set_borderrt	)(xui_rect2d<s32>(0, 0, 0, 2));
	xui_method_ptrcall(m_tool,	ini_component	)(0, 0, DOCKSTYLE_B);
	xui_method_ptrcall(m_tool,	add_child		)(m_small);
	xui_method_ptrcall(m_tool,	add_child		)(m_large);
	xui_method_ptrcall(m_tool,	add_child		)(m_reset);

	m_view	= new cocos_renderview(xui_vector<s32>(200), xui_vector<s32>(512));
	xui_method_ptrcall(m_view,	set_parent		)(this);
	xui_method_ptrcall(m_view,	ini_component	)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_view,	xm_updateself	) += new xui_method_member<xui_method_update, cocos_preview>(this, &cocos_preview::on_viewupdateself);
	xui_method_ptrcall(m_view,	xm_renderself	) += new xui_method_member<xui_method_args,   cocos_preview>(this, &cocos_preview::on_viewrenderself);
	xui_method_ptrcall(m_view,	xm_mousewheel	) += new xui_method_member<xui_method_mouse,  cocos_preview>(this, &cocos_preview::on_viewmousewheel);

	m_ctrl	= new xui_action_ctrl_impl<f32>(this);
	xui_method_ptrcall(m_ctrl,	set_soft		)(true);
	xui_method_ptrcall(m_ctrl,	xm_tick			) += new xui_method_member<xui_method_args,	  cocos_preview>(this, &cocos_preview::on_ctrltick);

	m_widgetvec.push_back(m_head);
	m_widgetvec.push_back(m_tool);
	m_widgetvec.push_back(m_view);

	m_sprite = cocos2d::Sprite::create();
	m_fxroot = cocos2d::Node::create();
	m_uitext = cocos2d::ui::TextBMFont::create();
	m_spanim = cocos2d::ui::SpineNode::create();
	m_view->get_2droot()->addChild(m_sprite);
	m_view->get_2droot()->addChild(m_fxroot);
	m_view->get_2droot()->addChild(m_uitext);
	m_view->get_2droot()->addChild(m_spanim);
}

/*
//destructor
*/
xui_delete_explain(cocos_preview)( void )
{
	m_view->get_2droot()->removeChild(m_sprite);
	m_view->get_2droot()->removeChild(m_fxroot);
	m_view->get_2droot()->removeChild(m_uitext);
	m_view->get_2droot()->removeChild(m_spanim);
	m_fxroot->removeAllChildren();
	delete m_sprite;
	delete m_fxroot;
	delete m_uitext;
	delete m_spanim;
	delete m_ctrl;
}

/*
//method
*/
xui_method_explain(cocos_preview, get_drawnode,			cocos2d::Node*		)( void )
{
	return m_node;
}
xui_method_explain(cocos_preview, get_drawview,			cocos_renderview*	)( void )
{
	return m_view;
}
xui_method_explain(cocos_preview, set_toolshow,			void				)( bool flag )
{
	m_head->set_visible(flag);
	m_tool->set_visible(flag);
	refresh();
}
xui_method_explain(cocos_preview, set_needshow,			void				)( void )
{
	cocos_propparticle* propparticle	= dynamic_cast<cocos_propparticle*	>(m_prop);
	cocos_propframe*    propframe		= dynamic_cast<cocos_propframe*		>(m_prop);
	cocos_propatlas*	propatlas		= dynamic_cast<cocos_propatlas*		>(m_prop);
	cocos_proptexture*  propimage		= dynamic_cast<cocos_proptexture*	>(m_prop);
	cocos_propttf*		propttf			= dynamic_cast<cocos_propttf*		>(m_prop);	
	cocos_propfnt*		propfnt			= dynamic_cast<cocos_propfnt*		>(m_prop);
	cocos_propspine*	propspine		= dynamic_cast<cocos_propspine*		>(m_prop);
	set_visible(
		propparticle ||
		propframe	 ||
		propatlas	 ||
		propimage	 ||
		propttf		 ||
		propfnt		 ||
		propspine);
}
xui_method_explain(cocos_preview, set_viewprop,			void				)( xui_proproot* prop, bool play )
{
	m_prop = prop;

	if (m_node)
	{
		m_node->setVisible(false);
		m_node = NULL;
	}

	cocos_propparticle* propparticle	= dynamic_cast<cocos_propparticle*	>(m_prop);
	cocos_propframe*    propframe		= dynamic_cast<cocos_propframe*		>(m_prop);
	cocos_propatlas*	propatlas		= dynamic_cast<cocos_propatlas*		>(m_prop);
	cocos_proptexture*  propimage		= dynamic_cast<cocos_proptexture*   >(m_prop);
	cocos_propfnt*		propfnt			= dynamic_cast<cocos_propfnt*		>(m_prop);
	cocos_propspine*	propspine		= dynamic_cast<cocos_propspine*		>(m_prop);
	if (propparticle)
	{
		cocos2d::ParticleSystem* particle = propparticle->get_particle();
		particle->resetSystem();
		m_fxroot->removeAllChildren();
		m_fxroot->addChild(particle);
		m_node = m_fxroot;
	}
	else
	if (propspine)
	{
		std::wstring full = propspine->get_fullname();
		std::string  path = xui_global::unicode_to_ascii(cocos_filedata::get_path(full));
		std::string  safe = xui_global::unicode_to_ascii(cocos_filedata::get_safe(full));
		m_spanim->setSpinePath(path+safe+".json");
		m_spanim->setAtlasPath(path+safe+".atlas");
		m_node = m_spanim;
	}
	else
	if (propframe)
	{
		m_sprite->setSpriteFrame(propframe->get_frame());
		m_node = m_sprite;
	}
	else
	if (propatlas || propimage)
	{
		cocos2d::Texture2D* texture = NULL;
		if (propatlas)
			texture = propatlas->get_texture();
		if (propimage)
			texture = propimage->get_texture();

		m_sprite->initWithTexture(texture);
		m_node = m_sprite;
	}
	else
	if (propfnt)
	{
		m_uitext->setFntFile(xui_global::unicode_to_ascii(propfnt->get_fullname()));
		m_uitext->setString(propfnt->get_viewtext());
		m_node = m_uitext;
	}

	if (m_node)
	{
		m_node->setVisible(true);
	}

	m_scale = 1.0f;
	m_rect	= xui_rect2d<s32>(0);
	m_ctrl->clear();
}

xui_method_explain(cocos_preview, set_drawrect,			void				)( const xui_rect2d<s32>& rect )
{
	m_rect = rect;
}

/*
//callback
*/
xui_method_explain(cocos_preview, on_updateself,		void				)( xui_method_update& args )
{
	xui_control::on_updateself(args);
	m_ctrl->update(args.delta);
}

/*
//event
*/
xui_method_explain(cocos_preview, on_buttonclick,		void				)( xui_component* sender, xui_method_args&   args )
{
	if (sender == m_play)
	{
		if (m_node == m_fxroot)
		{
			cocos2d::ParticleSystem* particle = dynamic_cast<cocos2d::ParticleSystem*>(m_fxroot->getChildren().at(0));
			particle->resetSystem();
		}
	}
	else
	if (sender == m_large)
	{
		m_scale = get_incscale();
		m_ctrl->clear();
	}
	else
	if (sender == m_small)
	{
		m_scale = get_decscale();
		m_ctrl->clear();
	}
	else
	if (sender == m_reset)
	{
		m_scale = 1.0f;
		m_ctrl->clear();
	}
}
xui_method_explain(cocos_preview, on_speedscroll,		void				)( xui_component* sender, xui_method_args&   args )
{
	f32 speed = get_speed();
	char temp[32];
	sprintf(temp, "%3.1fx", speed);
	m_text->set_text(xui_global::ascii_to_unicode(temp));
}
xui_method_explain(cocos_preview, on_headperform,		void				)( xui_component* sender, xui_method_args&	 args )
{
	m_text->on_perform_x(m_speed->get_renderx()-m_text->get_renderw());
}
xui_method_explain(cocos_preview, on_viewupdateself,	void				)( xui_component* sender, xui_method_update& args )
{
	if (m_node)
		m_node->update(args.delta * get_speed());
}
xui_method_explain(cocos_preview, on_viewrenderself,	void				)( xui_component* sender, xui_method_args&   args )
{
	xui_convas::get_ins()->clear(get_backcolor());

	set_localtransform();
	cocos2d::Director::getInstance()->drawScene();
	cocos_propttf* ttf = dynamic_cast<cocos_propttf*>(m_prop);
	if (ttf)
	{
		xui_family font;
		font.face = xui_global::get_fontface(xui_global::unicode_to_ascii(ttf->get_fullname()));
		font.size = 16;
		xui_family_render draw;
		draw.normalcolor = xui_colour::white;
		xui_rect2d<s32> rt = xui_convas::get_ins()->calc_draw(L"123abc确认", font, m_view->get_renderrt(), TEXTALIGN_CC, true);
		xui_convas::get_ins()->draw_text(L"123abc确认", font, rt, draw, true);
	}
}
xui_method_explain(cocos_preview, on_viewmousewheel,	void				)( xui_component* sender, xui_method_mouse&  args )
{
	if (m_tool->was_visible())
	{
		f32 start = m_scale;
		f32 final = m_scale;
		if (args.wheel > 0) final = get_incscale();
		else 				final = get_decscale();

		xui_action_ctrl_impl<f32>* action = (xui_action_ctrl_impl<f32>*)m_ctrl;
		action->clear();
		action->add_time(0.0f);
		action->add_time(0.5f);
		action->add_data(start);
		action->add_data(final);
		action->set_play(true);

		args.handle = true;
	}
}
xui_method_explain(cocos_preview, on_ctrltick,			void				)( xui_component* sender, xui_method_args& args )
{
	xui_action_ctrl_impl<f32>* action = (xui_action_ctrl_impl<f32>*)m_ctrl;
	m_scale = action->sample();
	if (m_scale == 0.1f || m_scale == 5.0f)
		m_ctrl->clear();
}

/*
//method
*/
xui_method_explain(cocos_preview, get_speed,			f32					)( void )
{
	f32 speed = 1.0f;
	s32 value = m_speed->get_range()/2 - m_speed->get_value();
	if (value > 0)
		speed -= value * 0.1f;
	if (value < 0)
		speed -= value * 1.0f;

	return speed;
}
xui_method_explain(cocos_preview, set_speed,			void				)( f32 speed )
{
	s32 range = m_speed->get_range();
	f32 value = 1.0f - speed;
	if (value > 0.0f)
		m_speed->set_value(range/2 - (s32)(value*10.0f));
	if (value < 0.0f)
		m_speed->set_value(range/2 - (s32)(value));
}
xui_method_explain(cocos_preview, set_localtransform,	void				)( void )
{
	xui_vector<s32> size = m_view->get_rendersz();
	if (m_node == m_sprite)
	{
		s32 w  = m_sprite->getTextureRect().size.width;
		s32 h  = m_sprite->getTextureRect().size.height;
		f32 sw = (f32)size.w / w;
		f32 sh = (f32)size.h / h;
		f32 s  = xui_min(sw, sh);
		if (s > 1.0f)
			s = m_scale;
		else
			s = m_scale * s;

		cocos2d::Vec2 anchor = m_sprite->getAnchorPointInPoints();
		cocos2d::Vec2 offset = m_sprite->getOffsetPosition();

		f32 x =   anchor.x*s - offset.x*s + (((f32)size.w - w*s) / 2.0f);
		f32 y = - anchor.y*s + offset.y*s + (((f32)size.h - h*s) / 2.0f);
		m_sprite->setScale(s, s);
		m_sprite->setPosition(x, m_view->get_renderh() - y - h*s);
	}
	else
	if (m_node == m_spanim)
	{
		spSkeletonData* data = m_spanim->getSkeletonAnim()->getSkeleton()->data;
		s32 w = data->width;
		s32 h = data->height;
		f32 sw = (f32)size.w / w;
		f32 sh = (f32)size.h / h;
		f32 s = xui_min(sw, sh);
		if (s > 1.0f)
			s = m_scale;
		else
			s = m_scale * s;

		f32 x = (((f32)size.w - w*s) / 2.0f);
		f32 y = (((f32)size.h - h*s) / 2.0f);
		m_spanim->setScale(s, s);
		m_spanim->setPosition(cocos2d::Vec2(x, m_view->get_renderh() - y - h*s));
	}
	else
	if (m_node == m_fxroot)
	{
		m_fxroot->setScale(m_scale);
		m_fxroot->setPosition(size.w/2, size.h/2);
	}
	else
	if (m_node == m_uitext)
	{
		m_uitext->setScale(m_scale);
		m_uitext->setPosition(cocos2d::Vec2(size.w/2, size.h/2));
	}
}
xui_method_explain(cocos_preview, get_incscale,			f32					)( void )
{
	xui_action_ctrl_impl<f32>* action = (xui_action_ctrl_impl<f32>*)m_ctrl;
	f32 scale = action->has_data() ? action->get_data(1) : m_scale;
	if (scale <  1.0f)	scale += 0.1f;
	else				scale += 1.0f;

	return xui_min(scale, 5.0f);
}
xui_method_explain(cocos_preview, get_decscale,			f32					)( void )
{
	xui_action_ctrl_impl<f32>* action = (xui_action_ctrl_impl<f32>*)m_ctrl;
	f32 scale = action->has_data() ? action->get_data(1) : m_scale;
	if (scale <= 1.0f)	scale -= 0.1f;
	else				scale -= 1.0f;

	return xui_max(scale, 0.1f);
}
