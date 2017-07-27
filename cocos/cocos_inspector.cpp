#include "2d/CCNode.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrame.h"
#include "2d/CCParticleSystemQuad.h"
#include "renderer/CCTexture2D.h"
#include "base/CCDirector.h"

#include "xui_desktop.h"
#include "xui_convas.h"
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
#include "cocos_renderview.h"
#include "cocos_propfile.h"
#include "cocos_propleaf.h"
#include "cocos_resource.h"
#include "cocos_preview.h"
#include "cocos_inspector.h"

xui_implement_rtti(cocos_inspector, xui_dockpage);

/*
//constructor
*/
xui_create_explain(cocos_inspector)( void )
: xui_dockpage(xui_vector<s32>(400), AREALIMIT_L|AREALIMIT_R, 400, DOCKSTYLE_R)
, m_tipsnode(NULL)
{
	xm_keybddown += new xui_method_member<xui_method_keybd, cocos_inspector>(this, &cocos_inspector::on_inspectorkeybddown);
	ini_namectrl(cocos_resource::icon_inspector, L"Inspector");

	m_propview	= new xui_propview(xui_vector<s32>(200));
	xui_method_ptrcall(m_propview,	ini_component	)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_propview,	set_borderrt	)(xui_rect2d<s32>(4));
	m_drawview	= new cocos_preview();
	xui_method_ptrcall(m_drawview,	set_backcolor	)(xui_colour(1.0f, 0.15f));
	xui_method_ptrcall(m_drawview,	set_drawcolor	)(true);
	xui_method_ptrcall(m_drawview,	set_borderrt	)(xui_rect2d<s32>(0, 6, 0, 6));
	xui_method_ptrcall(m_drawview,	set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_drawview,	ini_component	)(0, 0, DOCKSTYLE_B);
	xui_method_ptrcall(m_drawview,	ini_component	)(true, false);
	m_sizectrl	= new xui_control(xui_vector<s32>(4));
	xui_method_ptrcall(m_sizectrl,	xm_mousemove	) += new xui_method_member<xui_method_mouse, cocos_inspector>(this, &cocos_inspector::on_sizectrlmousemove);
	xui_method_ptrcall(m_sizectrl,	ini_component	)(0, 0, DOCKSTYLE_U);
	xui_method_ptrcall(m_sizectrl,	set_cursor		)(CURSOR_NS);
	m_tipsview	= new cocos_renderview(xui_vector<s32>(100), xui_vector<s32>(128));
	xui_method_ptrcall(m_tipsview,	xm_renderself	) += new xui_method_member<xui_method_args,  cocos_inspector>(this, &cocos_inspector::on_tipsviewrenderself);
	xui_method_ptrcall(m_tipsview,	set_sidestyle	)(SIDESTYLE_S);

	add_pagectrl(m_drawview);
	add_pagectrl(m_propview);
	add_pagectrl(m_sizectrl);
}

/*
//destructor
*/
xui_delete_explain(cocos_inspector)( void )
{
	if (m_tipsnode)
	{
		m_tipsview->get_2droot()->removeChild(m_tipsnode);
		delete m_tipsnode;
	}

	xui_desktop::get_ins()->move_recycle(m_tipsview);
}

/*
//method
*/
xui_method_explain(cocos_inspector,	get_preview,			cocos_preview*	)( void )
{
	return m_drawview;
}
xui_method_explain(cocos_inspector, get_propview,			xui_propview*	)( void )
{
	return m_propview;
}
xui_method_explain(cocos_inspector, set_proproot,			void			)( xui_proproot* proproot )
{
	m_propview->set_proproot(proproot);
	m_drawview->set_viewprop(proproot);
	m_drawview->set_needshow();
	refresh();
}
xui_method_explain(cocos_inspector, set_proproot,			void			)( const xui_proproot_vec& proproot )
{
	m_propview->set_proproot(proproot);
	m_drawview->set_viewprop(proproot.size() > 0 ? proproot.front() : NULL);
	m_drawview->set_needshow();
	refresh();
}

/*
//tips
*/
xui_method_explain(cocos_inspector, show_tips,				void			)( cocos2d::Ref* value, const xui_vector<s32>& pt )
{
	if (m_tipsnode)
	{
		m_tipsview->get_2droot()->removeChild(m_tipsnode);
		delete m_tipsnode;
		m_tipsnode = NULL;
	}

	if (value)
	{
		cocos2d::Texture2D*			texture		= dynamic_cast<cocos2d::Texture2D*		>(value);
		cocos2d::SpriteFrame*		frame		= dynamic_cast<cocos2d::SpriteFrame*	>(value);
		cocos2d::ParticleSystem*	particle	= dynamic_cast<cocos2d::ParticleSystem*	>(value);
		if		(texture)	m_tipsnode = cocos2d::Sprite::createWithTexture(texture);
		else if (frame)		m_tipsnode = cocos2d::Sprite::createWithSpriteFrame(frame);
		else if (particle)	m_tipsnode = cocos2d::ParticleSystemQuad::create(particle->getResourceFile());

		if (m_tipsnode)
			m_tipsview->get_2droot()->addChild(m_tipsnode);
	}

	set_localtransform();

	xui_vector<s32> finalpt = pt;
	xui_window* window = get_window();
	if (window->get_owner())
	{
		finalpt.y += xui_propview::default_lineheight;
	}
	else
	{
		finalpt.x -= m_tipsview->get_renderw();
	}

	m_tipsview->set_renderpt(finalpt);
	xui_desktop::get_ins()->set_floatctrl(window, m_tipsview);
}
xui_method_explain(cocos_inspector, hide_tips,				void			)( void )
{
	xui_desktop::get_ins()->set_floatctrl(NULL, NULL);
}

/*
//callback
*/
xui_method_explain(cocos_inspector, on_perform,				void			)( xui_method_args&   args )
{
	xui_dockpage::on_perform(args);
	xui_rect2d<s32> rt = m_propview->get_renderrt()+m_propview->get_renderpt();
	m_sizectrl->on_perform_w(rt.get_w());
	m_sizectrl->on_perform_x(rt.ax);
	m_sizectrl->on_perform_y(rt.by-m_sizectrl->get_renderh());
}
xui_method_explain(cocos_inspector, on_updateself,			void			)( xui_method_update& args )
{
	xui_dockpage::on_updateself(args);
	if (m_tipsnode)
		m_tipsnode->update(args.delta);
}

/*
//event
*/
xui_method_explain(cocos_inspector, on_inspectorkeybddown,	void			)( xui_component* sender, xui_method_keybd& args )
{
	if (args.kcode == KEY_S && args.ctrl)
	{
		xui_proproot_vec vec = m_propview->get_proproot();
		for (u32 i = 0; i < vec.size(); ++i)
		{
			cocos_propfile* propfile = dynamic_cast<cocos_propfile*>(vec[i]);
			cocos_propleaf* propleaf = dynamic_cast<cocos_propleaf*>(vec[i]);
			if (propleaf)
				propfile  = propleaf->get_propfile();
			if (propfile && propfile->was_modify())
			{
				propfile->save();
				args.handle = true;
			}
		}
	}
}
xui_method_explain(cocos_inspector, on_sizectrlmousemove,	void			)( xui_component* sender, xui_method_mouse& args )
{
	if (m_sizectrl->has_catch() && m_drawview->was_visible())
	{
		s32 delta  = xui_desktop::get_ins()->get_mousemove().y;
		s32 height = m_drawview->get_renderh() - delta;
		height = xui_max(height, 100);
		height = xui_min(height, 512);
		m_drawview->set_renderh(height);		
	}
}
xui_method_explain(cocos_inspector, on_tipsviewrenderself,	void			)( xui_component* sender, xui_method_args&  args )
{
	xui_convas::get_ins()->clear(xui_colour(1.0f, 0.25f));
	cocos2d::Director::getInstance()->drawScene();
}

/*
//method
*/
xui_method_explain(cocos_inspector, set_localtransform,		void			)( void )
{
	xui_vector<s32> size = m_tipsview->get_rendersz();
	if (m_tipsnode)
	{
		cocos2d::Sprite* sprite = dynamic_cast<cocos2d::Sprite*>(m_tipsnode);
		if (sprite)
		{
			s32 w = sprite->getTextureRect().size.width;
			s32 h = sprite->getTextureRect().size.height;
			f32 sw = (f32)size.w / w;
			f32 sh = (f32)size.h / h;
			f32 s = xui_min(sw, sh);
			if (s > 1.0f)
				s = 1.0f;

			cocos2d::Vec2 anchor = sprite->getAnchorPointInPoints();
			cocos2d::Vec2 offset = sprite->getOffsetPosition();

			f32 x =  anchor.x*s - offset.x*s + (((f32)size.w - w*s) / 2.0f);
			f32 y = -anchor.y*s + offset.y*s + (((f32)size.h - h*s) / 2.0f);
			sprite->setScale(s, s);
			sprite->setPosition(x, size.h - y - h*s);
		}
		else
		{
			m_tipsnode->setPosition(size.w / 2, size.h / 2);
		}
	}
}