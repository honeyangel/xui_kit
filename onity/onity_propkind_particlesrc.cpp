#include "NPRender.h"
#include "NPSourceTexture.h"
#include "NP2DSImage.h"
#include "NP2DSTextureCache.h"
#include "NP2DSRenderUtil.h"
#include "NP2DSRenderStep.h"
#include "NPParticleSpriteSRC.h"

#include "xui_desktop.h"
#include "xui_convas.h"
#include "xui_scroll.h"
#include "xui_button.h"
#include "xui_toggle.h"
#include "xui_textbox.h"
#include "xui_propview.h"
#include "xui_propdata.h"
#include "xui_propctrl.h"
#include "xui_propctrl_expand.h"
#include "onity_resource.h"
#include "onity_renderview.h"
#include "onity_propparticle.h"
#include "onity_propdata_texture.h"
#include "onity_propctrl_particlemod.h"
#include "onity_propkind_particlesrc.h"

//////////////////////////////////////////////////////////////////////////
//propkind
//////////////////////////////////////////////////////////////////////////
xui_create_explain(onity_propkind_particlesrc)( xui_proproot* root, NPParticleSpriteSRC* spritesrc )
: xui_propkind(root, L"SpriteSRC", "SpriteSRC", onity_kindctrl_particlesrc::create, NULL, true)
, m_spritesrc(spritesrc)
{
	add_common();
	add_sprite();
	add_senior();

	add_propdata(new onity_propdata_particlemod(
		this, 
		L"MOD", 
		spritesrc));
}

/*
//method
*/
xui_method_explain(onity_propkind_particlesrc, get_spritesrc,			NPParticleSpriteSRC*)( void )
{
	return m_spritesrc;
}
xui_method_explain(onity_propkind_particlesrc, add_common,				void				)( void )
{
	xui_propdata_vec subprop;
	xui_propenum_map textmap;
	subprop.push_back(new xui_propdata_number_impl<f32>(
		this, 
		L"SpawnRate", 
		xui_propctrl_number::create, 
		&m_spritesrc->mSpawnRate, 
		NT_UNSIGNEDINT));
	subprop.push_back(new xui_propdata_number_func(
		this, 
		L"MaxAmount", 
		xui_propctrl_number::create, 
		get_maxamount,
		set_maxamount,
		this,
		NT_UNSIGNEDINT));
	textmap[0] = L"Local";
	textmap[1] = L"World";
	subprop.push_back(new xui_propdata_enum_impl<EParticleXFormMode>(
		this, 
		L"XFormMode",
		xui_propctrl_enum::create,
		textmap,
		&m_spritesrc->mXFormMode));
	textmap.clear();
	textmap[0] = L"None";
	textmap[1] = L"Particle";
	textmap[2] = L"Vertices";
	subprop.push_back(new xui_propdata_enum_impl<EParticleBoundMode>(
		this,
		L"BoundMode",
		xui_propctrl_enum::create,
		textmap,
		&m_spritesrc->mBoundMode));
	subprop.push_back(new xui_propdata_bool(
		this, 
		L"AutoUpDir",
		xui_propctrl_bool::create,
		NULL,
		NULL,
		&m_spritesrc->mAutoUpDir));
	subprop.push_back(new xui_propdata_number_impl<u32>(
		this,
		L"MaxExtraBrust",
		xui_propctrl_number::create,
		&m_spritesrc->mMaxExtraBrust,
		NT_UNSIGNEDINT));
	subprop.push_back(new xui_propdata_number_impl<u32>(
		this,
		L"MinExtraBrust",
		xui_propctrl_number::create,
		&m_spritesrc->mMinExtraBrust,
		NT_UNSIGNEDINT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		this,
		L"InitDelayTime",
		xui_propctrl_number::create,
		&m_spritesrc->mInitDelayTime,
		NT_FLOAT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		this,
		L"LoopDelayTime",
		xui_propctrl_number::create,
		&m_spritesrc->mLoopDelayTime,
		NT_FLOAT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		this,
		L"Duration",
		xui_propctrl_number::create,
		&m_spritesrc->mDuration,
		NT_FLOAT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		this,
		L"TimeScale",
		xui_propctrl_number::create,
		&m_spritesrc->mTimeScale,
		NT_FLOAT));
	subprop.push_back(new xui_propdata_number_impl<u32>(
		this,
		L"RepeatTimes",
		xui_propctrl_number::create,
		&m_spritesrc->mRepeatTimes,
		NT_UNSIGNEDINT));

	add_propdata(new xui_propdata_expand(
		this,
		L"Common",
		xui_propctrl_expand::create,
		subprop));
}
xui_method_explain(onity_propkind_particlesrc, add_sprite,				void				)( void )
{
	xui_propdata_vec subprop;
	xui_propenum_map textmap;
	subprop.push_back(new onity_propdata_texture(
		this,
		L"BaseTex",
		get_texfile,
		set_texfile,
		this));
	textmap[0] = L"Additive";
	textmap[1] = L"AlphaBase";
	textmap[2] = L"ColorBase";
	textmap[3] = L"None";
	subprop.push_back(new xui_propdata_enum_impl<ESpriteParticleBlendMode>(
		this,
		L"BlendMode",
		xui_propctrl_enum::create,
		textmap,
		&m_spritesrc->mBlendMode));
	textmap.clear();
	textmap[0] = L"TopLeft";
	textmap[1] = L"TopCenter";
	textmap[2] = L"TopRight";
	textmap[3] = L"CenterLeft";
	textmap[4] = L"Center";
	textmap[5] = L"CenterRight";
	textmap[6] = L"BottomLeft";
	textmap[7] = L"BottomCenter";
	textmap[8] = L"BottomRight";
	subprop.push_back(new xui_propdata_enum_func(
		this,
		L"Origin",
		xui_propctrl_enum::create,
		textmap,
		get_origin,
		set_origin,
		this));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		this,
		L"TilingU",
		xui_propctrl_number::create,
		&m_spritesrc->mTilingU,
		NT_UNSIGNEDINT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		this,
		L"TilingV",
		xui_propctrl_number::create,
		&m_spritesrc->mTilingV,
		NT_UNSIGNEDINT));

	add_propdata(new xui_propdata_expand(
		this,
		L"Sprite",
		xui_propctrl_expand::create,
		subprop));
}
xui_method_explain(onity_propkind_particlesrc, add_senior,				void				)( void )
{
	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_number_impl<f32>(
		this, 
		L"TexAtlasBiasX",
		xui_propctrl_number::create,
		&m_spritesrc->mTexAtlasBiasX,
		NT_UNSIGNEDINT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		this,
		L"TexAtlasBiasY",
		xui_propctrl_number::create,
		&m_spritesrc->mTexAtlasBiasY,
		NT_UNSIGNEDINT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		this,
		L"TexAtlasSizeX",
		xui_propctrl_number::create,
		&m_spritesrc->mTexAtlasSizeX,
		NT_UNSIGNEDINT));
	subprop.push_back(new xui_propdata_number_impl<f32>(
		this,
		L"TexAtlasSizeY",
		xui_propctrl_number::create,
		&m_spritesrc->mTexAtlasSizeY,
		NT_UNSIGNEDINT));

	add_propdata(new xui_propdata_expand(
		this,
		L"Senior",
		xui_propctrl_expand::create,
		subprop));
}

/*
//static
*/
xui_method_explain(onity_propkind_particlesrc, get_maxamount,			f64					)( void* userptr )
{
	onity_propkind_particlesrc* prop = (onity_propkind_particlesrc*)userptr;
	NPParticleSpriteSRC* spritesrc = prop->get_spritesrc();
	return (f64)spritesrc->mMaxAmount;
}
xui_method_explain(onity_propkind_particlesrc, set_maxamount,			void				)( void* userptr, f64 value )
{
	onity_propkind_particlesrc* prop = (onity_propkind_particlesrc*)userptr;
	NPParticleSpriteSRC* spritesrc = prop->get_spritesrc();
	spritesrc->SetMaxAmount((u32)value);
}
xui_method_explain(onity_propkind_particlesrc, get_origin,				s32					)( void* userptr )
{
	onity_propkind_particlesrc* prop = (onity_propkind_particlesrc*)userptr;
	NPParticleSpriteSRC* spritesrc = prop->get_spritesrc();
	return (s32)spritesrc->mOrigin;
}
xui_method_explain(onity_propkind_particlesrc, set_origin,				void				)( void* userptr, s32 value )
{
	onity_propkind_particlesrc* prop = (onity_propkind_particlesrc*)userptr;
	NPParticleSpriteSRC* spritesrc = prop->get_spritesrc();
	spritesrc->SetOrigin((ESpriteParticleOrigin)value);
}
xui_method_explain(onity_propkind_particlesrc, get_texfile,				void*				)( void* userptr )
{
	onity_propkind_particlesrc* prop = (onity_propkind_particlesrc*)userptr;
	NPParticleSpriteSRC* spritesrc = prop->get_spritesrc();
	if (spritesrc->mTexelID != -1)
		return NP2DSTextureCache::GetIns()->GetTexture(spritesrc->mTexelID);

	return NULL;
}
xui_method_explain(onity_propkind_particlesrc, set_texfile,				void				)( void* userptr, void* value )
{
	onity_propkind_particlesrc* prop = (onity_propkind_particlesrc*)userptr;
	NPParticleSpriteSRC* spritesrc = prop->get_spritesrc();
	NPSourceTexture* texture = (NPSourceTexture*)value;
	spritesrc->SetBaseTexel((texture == NULL) ? -1 : texture->GetKey());
}

//////////////////////////////////////////////////////////////////////////
//kindctrl
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(onity_kindctrl_particlesrc, xui_kindctrl);

/*
//constructor
*/
xui_create_explain(onity_kindctrl_particlesrc)( xui_propkind* propkind )
: xui_kindctrl(propkind)
{
	m_iconview = new onity_renderview(xui_vector<s32>(24), xui_vector<s32>(32));
	xui_method_ptrcall(m_iconview, xm_renderself	) += new xui_method_member<xui_method_args,		onity_kindctrl_particlesrc>(this, &onity_kindctrl_particlesrc::on_iconviewrenderself);
	xui_method_ptrcall(m_iconview, set_parent		)(this);
	xui_method_ptrcall(m_iconview, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_iconctrl, ini_component	)(true, false);

	m_killctrl = new xui_button(xui_vector<s32>(16));
	xui_method_ptrcall(m_killctrl, xm_buttonclick	) += new xui_method_member<xui_method_args,		onity_kindctrl_particlesrc>(this, &onity_kindctrl_particlesrc::on_killctrlclick);
	xui_method_ptrcall(m_killctrl, set_parent		)(this);
	xui_method_ptrcall(m_killctrl, set_iconalign	)(IMAGE_C);
	xui_method_ptrcall(m_killctrl, ini_drawer		)(onity_resource::icon_delete);

	m_sortctrl	= new xui_drawer(xui_vector<s32>(20));
	xui_method_ptrcall(m_sortctrl, set_parent		)(this);
	xui_method_ptrcall(m_sortctrl, xm_updateself	) += new xui_method_member<xui_method_update,	onity_kindctrl_particlesrc>(this, &onity_kindctrl_particlesrc::on_sortctrlupdateself);
	xui_method_ptrcall(m_sortctrl, xm_renderself	) += new xui_method_member<xui_method_args,		onity_kindctrl_particlesrc>(this, &onity_kindctrl_particlesrc::on_sortctrlrenderself);
	xui_method_ptrcall(m_sortctrl, xm_topdraw		) += new xui_method_member<xui_method_args,		onity_kindctrl_particlesrc>(this, &onity_kindctrl_particlesrc::on_sortctrltopdraw);
	xui_method_ptrcall(m_sortctrl, xm_mouserise		) += new xui_method_member<xui_method_mouse,	onity_kindctrl_particlesrc>(this, &onity_kindctrl_particlesrc::on_sortctrlmouserise);

	m_widgetvec.push_back(m_iconview);
	m_widgetvec.push_back(m_killctrl);
	m_widgetvec.push_back(m_sortctrl);
}

/*
//static
*/
xui_method_explain(onity_kindctrl_particlesrc, create,					xui_kindctrl*		)( xui_propkind* propkind )
{
	return new onity_kindctrl_particlesrc(propkind);
}

/*
//callback
*/
xui_method_explain(onity_kindctrl_particlesrc, on_perform,				void				)( xui_method_args& args )
{
	xui_kindctrl::on_perform(args);
	m_iconview->on_perform_x(m_iconctrl->get_renderx());
	m_iconview->on_perform_y(m_iconctrl->get_rendery());
	m_killctrl->on_perform_x(m_flagctrl->get_renderx()- 4-m_killctrl->get_renderw());
	m_killctrl->on_perform_y(m_flagctrl->get_rendery());
	m_sortctrl->on_perform_x(m_killctrl->get_renderx()- 4-m_sortctrl->get_renderw());
	m_sortctrl->on_perform_y(m_killctrl->get_rendery());
	m_namectrl->on_perform_w(m_namectrl->get_renderw()-12-m_killctrl->get_renderw()-m_sortctrl->get_renderw());
}
xui_method_explain(onity_kindctrl_particlesrc, on_killctrlclick,		void				)( xui_component* sender, xui_method_args&	 args )
{
	onity_propparticle* propparticle = dynamic_cast<onity_propparticle*>(m_propkind->get_root());
	propparticle->del_spritesrc(m_propkind);
}
xui_method_explain(onity_kindctrl_particlesrc, on_iconviewrenderself,	void				)( xui_component* sender, xui_method_args&	 args )
{
	xui_convas::get_ins()->clear(xui_colour(1.0f, 0.25f));
	extern bool gInitCompleted;
	if (gInitCompleted == false)
		return;

	xui_vector<s32> size = m_iconview->get_rendersz();
	onity_propkind_particlesrc* propspritesrc = dynamic_cast<onity_propkind_particlesrc*>(m_propkind);
	NPParticleSpriteSRC* spritesrc = propspritesrc->get_spritesrc();
	if (spritesrc->mTexelID != -1)
	{
		NPTexture* texture = NP2DSTextureCache::GetIns()->GetTexture(spritesrc->mTexelID);
		if (texture)
		{
			npu32 w = texture->GetWidth ();
			npu32 h = texture->GetHeight();
			npf32 s = (npf32)(size.w) / xui_max(w, h);
			npf32 x = 0.0f;
			npf32 y = 0.0f;
			NPTransform t;
			t.SetIdentity();
			t.SetS(NPVector3(s, s, 1.0f));
			t.SetT(NPVector3(x, y, 0.0f));
			NP2DSImage image(-1, "", -1, 0, 0, (nps16)texture->GetPixelWidth(), (nps16)texture->GetPixelHeight(), true);
			NP2DSRenderUtil::GetIns()->DrawImage(
				&image, 
				t.GetMatrix(),
				NPColor::White, 
				0,
				0,
				0,
				0,
				texture->GetKey());
		}
	}

	NPRender::GetIns()->SetResolutionW(size.w);
	NPRender::GetIns()->SetResolutionH(size.h);
	NPRender::GetIns()->SetViewport(0, 0, size.w, size.h);
	NP2DSRenderStep::GetIns()->SetEntryLocalT(NPVector3::Zero);
	NP2DSRenderStep::GetIns()->SetEntryWorldS(NPVector3::PositiveOne);
	NP2DSRenderStep::GetIns()->RenderImmediate();
}
xui_method_explain(onity_kindctrl_particlesrc, on_sortctrlupdateself,	void				)( xui_component* sender, xui_method_update& args )
{
	if (sender->has_catch())
	{
		xui_propview* propview = m_propkind->get_root()->get_ctrl();
		xui_rect2d<s32> rt = propview->get_renderrtins() + propview->get_screenpt();
		xui_vector<s32> pt = xui_desktop::get_ins()->get_mousecurr();
		xui_vector<s32> dw = xui_desktop::get_ins()->get_mousedown();
		if (xui_abs(pt.y-dw.y) < xui_propview::default_lineheight/2)
			return;

		s32 scroll_value =  0;
		if (pt.y > rt.ay && pt.y < rt.ay+xui_propview::default_lineheight/2)
			scroll_value = -xui_propview::default_lineheight/2;
		if (pt.y < rt.by && pt.y > rt.by-xui_propview::default_lineheight/2)
			scroll_value =  xui_propview::default_lineheight/2;

		xui_scroll* vscroll = propview->get_vscroll();
		if (scroll_value != 0 && vscroll)
		{
			vscroll->set_value(vscroll->get_value()+scroll_value);

			xui_method_mouse args;
			args.mouse = MB_L;
			args.point = pt;
			xui_desktop::get_ins()->os_mousemove(args);
		}
	}
}
xui_method_explain(onity_kindctrl_particlesrc, on_sortctrlrenderself,	void				)( xui_component* sender, xui_method_args&	 args )
{
	xui_rect2d<s32> rt     = sender->get_renderrtabs();
	xui_colour	    color  = sender->was_hover() ? xui_colour(1.0f,  42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f) : xui_button::default_backcolor;
	xui_vector<s32> center = xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);
	rt.ax = center.x - 6;
	rt.ay = center.y - 4;
	rt.bx = center.x + 6;
	rt.by = center.y - 2;
	xui_convas::get_ins()->fill_rectangle(rt, color);
	rt.oft_y(3);
	xui_convas::get_ins()->fill_rectangle(rt, color);
	rt.oft_y(3);
	xui_convas::get_ins()->fill_rectangle(rt, color);
}
xui_method_explain(onity_kindctrl_particlesrc, on_sortctrltopdraw,		void				)( xui_component* sender, xui_method_args&	 args )
{
	xui_kindctrl::on_topdraw(args);
	s32 dragelem = get_sortdrag();
	s32 dropelem = get_sortdrop(xui_desktop::get_ins()->get_mousecurr());
	if (dropelem != -1 && dropelem != dragelem)
	{
		if (dropelem < dragelem || dropelem > dragelem+1)
		{
			xui_propview* propview = m_propkind->get_root()->get_ctrl();

			xui_vector<s32> pt;
			for (u32 i = 0, index = 0; i < propview->get_kindctrlcount(); ++i)
			{
				xui_control* kindctrl = propview->get_kindctrl(i);
				if (i == 0)
				{
					pt = kindctrl->get_screenpt();
				}

				onity_kindctrl_particlesrc* kindctrlsrc = xui_dynamic_cast(onity_kindctrl_particlesrc, kindctrl);
				if (kindctrlsrc)
				{
					if (index == dropelem)
						break;

					++index;
				}
				else
				{
					if (index != 0)
						break;
				}

				pt.y += kindctrl->get_renderh();
			}

			xui_vector<s32> p1(pt.x,									  pt.y);
			xui_vector<s32> p2(pt.x+propview->get_renderrtins().get_w(),  pt.y);
			xui_rect2d<s32> rt  = xui_rect2d<s32>(p1.x+1, p1.y-1, p2.x-1, p1.y+1);
			xui_convas::get_ins()->fill_rectangle(rt, xui_colour::black);
			p1 += xui_vector<s32>(4, 0);
			xui_convas::get_ins()->fill_triangle(p1, 3, TRIANGLE_RIGHT, xui_colour::black);
			p2 -= xui_vector<s32>(3, 0);
			xui_convas::get_ins()->fill_triangle(p2, 3, TRIANGLE_LEFT,  xui_colour::black);
		}
	}
}
xui_method_explain(onity_kindctrl_particlesrc, on_sortctrlmouserise,	void				)( xui_component* sender, xui_method_mouse&  args )
{
	if (args.mouse == MB_L)
	{
		u32 dragelem = get_sortdrag();
		u32 dropelem = get_sortdrop(args.point);
		if (dropelem != -1 && dropelem != dragelem)
		{
			if (dropelem < dragelem || dropelem > dragelem+1)
			{
				for (u32 i = 0; i < m_propkindvec.size(); ++i)
				{
					onity_propparticle* propparticle = dynamic_cast<onity_propparticle*>(m_propkindvec[i]);
					propparticle->set_spritesrcindex(m_propkindvec[i], dragelem, dropelem);
				}
			}
		}	
	}
}

/*
//method
*/
xui_method_explain(onity_kindctrl_particlesrc, get_sortdrag,			u32					)( void )
{
	u32 dragelem = -1;

	if (m_propkind)
	{
		xui_propview* propview = m_propkind->get_root()->get_ctrl();
		if (propview)
		{
			for (u32 i = 0, index = 0; i < propview->get_kindctrlcount(); ++i)
			{
				onity_kindctrl_particlesrc* kindctrlsrc = xui_dynamic_cast(onity_kindctrl_particlesrc, propview->get_kindctrl(i));
				if (kindctrlsrc)
				{
					if (kindctrlsrc == this)
					{
						dragelem = index;
						break;
					}

					++index;
				}
			}
		}
	}

	return dragelem;
}
xui_method_explain(onity_kindctrl_particlesrc, get_sortdrop,			u32					)( const xui_vector<s32>& pt )
{
	u32 dropelem = -1;

	if (m_propkind)
	{
		xui_propview* propview = m_propkind->get_root()->get_ctrl();
		if (propview)
		{
			for (u32 i = 0, index = 0; i < propview->get_kindctrlcount(); ++i)
			{
				onity_kindctrl_particlesrc* kindctrlsrc = xui_dynamic_cast(onity_kindctrl_particlesrc, propview->get_kindctrl(i));
				if (kindctrlsrc)
				{
					xui_rect2d<s32> rt = kindctrlsrc->get_renderrtabs();
					if (pt.y >= rt.ay && pt.y < rt.by)
					{
						if (pt.y < rt.ay+28)
							dropelem = index;
						if (pt.y > rt.by-28)
							dropelem = index+1;

						break;
					}

					++index;
				}
			}
		}
	}

	return dropelem;
}

xui_implement_rtti(onity_kindctrl_particleadd, xui_kindctrl);

/*
//constructor
*/
xui_create_explain(onity_kindctrl_particleadd)( xui_propkind* propkind )
: xui_kindctrl(propkind)
{
	m_prevctrl	= new xui_button(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_prevctrl,	xm_buttonclick	) += new xui_method_member<xui_method_args, onity_kindctrl_particleadd>(this, &onity_kindctrl_particleadd::on_prevctrlclick);
	xui_method_ptrcall(m_prevctrl,	set_parent		)(this);
	xui_method_ptrcall(m_prevctrl,	set_corner		)(3);
	xui_method_ptrcall(m_prevctrl,	set_borderrt	)(xui_rect2d<s32>(8, 2, 2, 2));
	xui_method_ptrcall(m_prevctrl,	set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(m_prevctrl,	set_drawcolor	)(true);
	xui_method_ptrcall(m_prevctrl,	set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_prevctrl,	set_iconsize	)(xui_vector<s32>(0));
	xui_method_ptrcall(m_prevctrl,	ini_drawer		)(L"Add Sprite SRC");
	m_widgetvec.push_back(m_prevctrl);
}

/*
//static
*/
xui_method_explain(onity_kindctrl_particleadd, create,					xui_kindctrl*		)( xui_propkind* propkind )
{
	return new onity_kindctrl_particleadd(propkind);
}

/*
//callback
*/
xui_method_explain(onity_kindctrl_particleadd, on_perform, void)( xui_method_args& args )
{
	xui_kindctrl::on_perform(args);
	s32 indent = xui_propview::default_nodeindent;
	s32 height = KIND_HEIGHT;

	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt;
	//prevctrl
	pt.x  = rt.ax +  indent;
	pt.y  = rt.ay + (height-m_prevctrl->get_renderh())/2;
	m_prevctrl->on_perform_pt(pt);
	m_prevctrl->on_perform_w (rt.get_w()-indent);
}

/*
//override
*/
xui_method_explain(onity_kindctrl_particleadd, get_elsectrlsize,		s32					)( void )
{
	return KIND_HEIGHT;
}
xui_method_explain(onity_kindctrl_particleadd, get_prevctrlsize,		s32					)( void )
{
	return KIND_HEIGHT;
}

/*
//event
*/
xui_method_explain(onity_kindctrl_particleadd, on_prevctrlclick,		void				)( xui_component* sender, xui_method_args&	 args )
{
	onity_propparticle* propparticle = dynamic_cast<onity_propparticle*>(m_propkind->get_root());
	propparticle->add_spritesrc();
}