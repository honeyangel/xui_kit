#include "NPFileName.h"
#include "NPRender.h"
#include "NPTexture.h"
#include "NP2DSImage.h"
#include "NP2DSFrame.h"
#include "NP2DSActor.h"
#include "NP2DSLayer.h"
#include "NP2DSFrameKey.h"
#include "NP2DSTransRef.h"
#include "NP2DSTextureCache.h"
#include "NP2DSRenderUtil.h"
#include "NP2DSRenderStep.h"

#include "xui_global.h"
#include "xui_scroll.h"
#include "xui_treenode.h"
#include "xui_treeview.h"
#include "onity_prop2dsasset.h"
#include "onity_filedata.h"
#include "onity_propfile.h"
#include "onity_renderview.h"
#include "onity_fileview.h"
#include "onity_tileview.h"

xui_implement_rtti(onity_tileview, xui_control);

/*
//constructor
*/
xui_create_explain(onity_tileview)( void )
: xui_control(xui_vector<s32>(0))
, m_tilesize(20)
, m_viewfile(NULL)
{
	m_drawview = new onity_renderview(xui_vector<s32>(100), xui_vector<s32>(2048, 1024));
	xui_method_ptrcall(m_drawview, xm_invalid			) += new xui_method_member<xui_method_args,  onity_tileview>(this, &onity_tileview::on_drawviewinvalid);
	xui_method_ptrcall(m_drawview, xm_renderself		) += new xui_method_member<xui_method_args,  onity_tileview>(this, &onity_tileview::on_drawviewrenderself);
	xui_method_ptrcall(m_drawview, xm_mousedown			) += new xui_method_member<xui_method_mouse, onity_tileview>(this, &onity_tileview::on_drawviewmousedown);
	xui_method_ptrcall(m_drawview, xm_mousewheel		) += new xui_method_member<xui_method_mouse, onity_tileview>(this, &onity_tileview::on_drawviewmousewheel);
	xui_method_ptrcall(m_drawview, xm_mousedoubleclick	) += new xui_method_member<xui_method_mouse, onity_tileview>(this, &onity_tileview::on_drawviewmousedclick);
	xui_method_ptrcall(m_drawview, set_parent			)(this);
	xui_method_ptrcall(m_drawview, set_borderrt			)(xui_rect2d<s32>(20));
	xui_method_ptrcall(m_drawview, ini_component		)(0, 0, DOCKSTYLE_F);

	m_viewroll = xui_scroll::create(FLOWSTYLE_V);
	xui_method_ptrcall(m_viewroll, set_parent			)(this);
	xui_method_ptrcall(m_viewroll, ini_component		)(true, false);
	xui_method_ptrcall(m_viewroll, ini_component		)(0, 0, DOCKSTYLE_R);

	m_widgetvec.push_back(m_viewroll);
	m_widgetvec.push_back(m_drawview);
}

/*
//method
*/
xui_method_explain(onity_tileview, get_lineview,			xui_treeview*		)( void )
{
	onity_fileview* fileview = xui_dynamic_cast(onity_fileview, m_parent);
	if (fileview)
		return fileview->get_lineview();

	return NULL;
}
xui_method_explain(onity_tileview, get_drawview,			onity_renderview*	)( void )
{
	return m_drawview;
}
xui_method_explain(onity_tileview, set_viewfile,			void				)( onity_propfile* propfile )
{
	if (m_viewfile != propfile)
	{
		m_viewfile  = propfile;
		m_drawview->invalid();
	}
}
xui_method_explain(onity_tileview, set_tilesize,			void				)( s32 size )
{
	if (m_tilesize != size)
	{
		m_tilesize  = size;
		m_drawview->invalid();
	}
}
xui_method_explain(onity_tileview, get_tileinfo,			void				)( s32& s, s32& c, s32& g, s32& w, s32& h )
{
	xui_rect2d<s32> rt = m_drawview->get_renderrtins();
	s = m_tilesize + m_tilesize/5*2;
	c =  rt.get_w() / s;
	g = (rt.get_w()-c*s) / (c-1);
	w = s + g;
	h = m_tilesize + name_size + vert_grap;
}
xui_method_explain(onity_tileview, set_tilevisible,			void				)( xui_treenode* node )
{

}

/*
//event
*/
xui_method_explain(onity_tileview, on_drawviewinvalid,		void				)( xui_component* sender, xui_method_args& args )
{
	xui_treeview* lineview = get_lineview();
	if (lineview)
	{
		u32 count = 0;
		if (m_viewfile)
		{

		}
		else
		{
			std::vector<xui_treenode*> nodes = lineview->get_entirenode(false);
			count = nodes.size();
		}

		s32 s, c, g, w, h;
		get_tileinfo(s, c, g, w, h);
		s32 r = (s32)ceilf((f32)count / (f32)c);
		xui_rect2d<s32> rt = m_drawview->get_renderrtins();
		if (h*r <= rt.get_h())
		{
			xui_method_ptrcall(m_viewroll, set_value	)(0);
			xui_method_ptrcall(m_viewroll, set_visible	)(false);
		}
		else
		{
			xui_method_ptrcall(m_viewroll, set_range	)(h*r - rt.get_h());
			xui_method_ptrcall(m_viewroll, set_visible	)(true);
		}

		refresh();
	}
}
xui_method_explain(onity_tileview, on_drawviewrenderself,	void				)( xui_component* sender, xui_method_args& args )
{
	xui_convas::get_ins()->clear(xui_colour(1.0f, 0.25f));
	extern bool gInitCompleted;
	if (gInitCompleted == false)
		return;

	xui_rect2d<s32> rt = m_drawview->get_renderrtins();
	xui_treeview* lineview = get_lineview();
	if (lineview)
	{
		s32 s, c, g, w, h;
		get_tileinfo(s, c, g, w, h);

		s32 ic = 0;
		s32 ir = 0;
		xui_rect2d<s32> tilert;
		for (u32 i = 0; i < lineview->get_upmostnodecount(); ++i)
		{
			xui_treenode* node = lineview->get_upmostnode(i);
			if (rt.ay + ir*h - m_viewroll->get_value() > rt.by)
				break;

			draw_node(ic, ir, rt.ax, rt.ay, w, s, node);
			if (++ic == c)
			{   ++ir;  ic = 0;   }

			if (node->was_expanded())
			{
				draw_background(ic, ir, c, g, w, h, node);
				for (u32 isub = 0; isub < node->get_leafnodecount(); ++isub)
				{
					xui_treenode* subnode = node->get_leafnode(isub);
					if (rt.ay + ir*h - m_viewroll->get_value() > rt.by)
						break;

					draw_node(ic, ir, rt.ax, rt.ay, w, s, subnode);
					if (++ic == c)
					{   ++ir;  ic = 0;   }
				}
			}
		}
	}

	xui_vector<s32> size = m_drawview->get_rendersz();
	NPRender::GetIns()->SetResolutionW(size.w);
	NPRender::GetIns()->SetResolutionH(size.h);
	NPRender::GetIns()->SetViewport(0, 0, size.w, size.h);
	NP2DSRenderStep::GetIns()->SetEntryLocalT(NPVector3::Zero);
	NP2DSRenderStep::GetIns()->SetEntryWorldS(NPVector3::PositiveOne);
	NP2DSRenderStep::GetIns()->RenderImmediate();
}
xui_method_explain(onity_tileview, on_drawviewmousedown,	void				)( xui_component* sender, xui_method_mouse& args )
{
	if (args.mouse == MB_L)
	{
		xui_rect2d<s32> rt = m_drawview->get_renderrtins();
		xui_vector<s32> pt = m_drawview->get_renderpt(args.point);
		s32 s, c, g, w, h;
		get_tileinfo(s, c, g, w, h);
		s32 ic = (pt.x-rt.ax)							/ w;
		s32 ir = (pt.y-rt.ay+m_viewroll->get_value())	/ h;
		u32 i  = (u32)(ir*c + ic);

		xui_rect2d<s32> tilert;
		tilert.ax = rt.ax + ic*w;
		tilert.ay = rt.ay + ir*h - m_viewroll->get_value();
		tilert.set_w(s);
		tilert.set_h(m_tilesize+name_size);

		xui_treeview* lineview = get_lineview();
		if (lineview == NULL)
			return;

		std::vector<xui_treenode*> nodes;
		if (m_viewfile)
		{

		}
		else
		{
			nodes = lineview->get_entirenode(false);
		}

		if (i < nodes.size() && tilert.was_inside(pt))
		{
			s32 half   = m_tilesize / 20;
			xui_rect2d<s32> drawrt = tilert;
			drawrt.ax += m_tilesize / 5;
			drawrt.bx -= m_tilesize / 5;
			drawrt.by -= name_size;
			xui_vector<s32> center(drawrt.bx, drawrt.ay+drawrt.get_h()/2);
			xui_rect2d<s32> plusrt;
			plusrt.ax = center.x-half  -3;
			plusrt.bx = center.x+half  +3;
			plusrt.ay = center.y-half*2-3;
			plusrt.by = center.y+half*2+3;

			xui_treenode* node = nodes[i];
			if (node->get_rootnode() == NULL && node->get_leafnodecount() > 0 && plusrt.was_inside(pt))
			{
				node->set_expanded(!node->was_expanded());
				m_drawview->invalid();
			}
			else
			{
				lineview->set_selectednode(node, true);
				lineview->set_nodevisible (node);
			}
		}
	}
}
xui_method_explain(onity_tileview, on_drawviewmousedclick,	void				)( xui_component* sender, xui_method_mouse& args )
{

}
xui_method_explain(onity_tileview, on_drawviewmousewheel,	void				)( xui_component* sender, xui_method_mouse& args )
{
	if (m_viewroll->was_visible())
	{
		m_viewroll->set_value(m_viewroll->get_value()-args.wheel);
		args.handle = true;
	}
}

/*
//method
*/
xui_method_explain(onity_tileview, draw_file,				void				)( const xui_rect2d<s32>& rt, xui_treenode* node )
{
	xui_rect2d<s32> drawrt = rt;
	drawrt.ax += m_tilesize/5;
	drawrt.bx -= m_tilesize/5;
	drawrt.by -= name_size;

	onity_filedata* filedata = (onity_filedata*)node->get_linkdata();
	std::wstring suff = filedata->get_suff();
	if (suff == L".PNG" || suff == L".png") draw_texture(drawrt, node);
	else									draw_icon	(drawrt, node);

	if (node->get_leafnodecount() > 0)
	{
		s32 half = m_tilesize / 20;
		xui_vector<s32> center(drawrt.bx, drawrt.ay+drawrt.get_h()/2);
		xui_rect2d<s32> plusrt;
		plusrt.ax = center.x-half  -3;
		plusrt.bx = center.x+half  +3;
		plusrt.ay = center.y-half*2-3;
		plusrt.by = center.y+half*2+3;
		xui_convas::get_ins()->fill_round   (plusrt, xui_colour::gray,  xui_rect2d<s32>(3));
		xui_convas::get_ins()->draw_round   (plusrt, xui_colour::white, xui_rect2d<s32>(3));
		xui_convas::get_ins()->fill_triangle(center, half, (node->was_expanded() ? TRIANGLE_LEFT : TRIANGLE_RIGHT), xui_colour::white);
	}
}
xui_method_explain(onity_tileview, draw_icon,				void				)( const xui_rect2d<s32>& rt, xui_treenode* node )
{
	onity_filedata* filedata = (onity_filedata*)node->get_linkdata();
	xui_colour color =  node->was_selected() ? xui_colour(1.0f, 42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f) : xui_colour::white;
	xui_convas::get_ins()->draw_image(filedata->get_icon(0), rt, color);
}
xui_method_explain(onity_tileview, draw_name,				void				)( const xui_rect2d<s32>& rt, xui_treenode* node )
{
	xui_rect2d<s32> drawrt = rt;
	drawrt.ay += m_tilesize;
	drawrt.ay += 2;
	drawrt.by -= 2;

	if (node->was_selected())
		xui_convas::get_ins()->fill_round(drawrt, xui_colour(1.0f, 42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f), xui_rect2d<s32>(8));

	onity_treedata* data   = (onity_treedata*)node->get_linkdata();
	std::wstring    text   = trim_string(data->get_text(0), xui_family::default, drawrt.get_w()-16);
	xui_rect2d<s32> textrt = xui_convas::get_ins()->calc_draw(text, xui_family::default, drawrt, TEXTALIGN_CC, true);
	xui_convas::get_ins()->draw_text(text, xui_family::default, textrt, xui_family_render::default);
}
xui_method_explain(onity_tileview, draw_leaf,				void				)( const xui_rect2d<s32>& rt, xui_treenode* node )
{
	xui_rect2d<s32> drawrt = rt;
	drawrt.ax += m_tilesize/5;
	drawrt.bx -= m_tilesize/5;
	drawrt.by -= name_size;

	if (node->was_selected())
		xui_convas::get_ins()->fill_rectangle(drawrt, xui_colour(0.5f, 42.0f/255.0f, 135.0f/255.0f, 190.0f/255.0f));

	onity_treedata*     data = (onity_treedata*)node->get_linkdata();
	onity_prop2dsasset* prop = dynamic_cast<onity_prop2dsasset*>(data->get_prop());
	if (prop)
	{
		NP2DSAsset* asset = prop->get_asset();
		if		(NPIsExaKindOf(NP2DSImage, asset))	draw_image(drawrt, (NP2DSImage*)asset);
		else if (NPIsExaKindOf(NP2DSFrame, asset))	draw_frame(drawrt, (NP2DSFrame*)asset);
		else if (NPIsExaKindOf(NP2DSActor, asset))	draw_actor(drawrt, (NP2DSActor*)asset);
		else
		{}
	}
}
xui_method_explain(onity_tileview, draw_texture,			void				)( const xui_rect2d<s32>& rt, xui_treenode* node )
{
	NPVector2 p1((npf32)rt.ax, (npf32)rt.ay);
	NPVector2 p2((npf32)rt.bx, (npf32)rt.by);
	NP2DSRenderUtil::GetIns()->DrawRect(p1, p2, NPMatrix4x4::Identity, NPColor(90.0f/255.0f, 90.0f/255.0f, 90.0f/255.0f));

	onity_filedata* filedata = (onity_filedata*)node->get_linkdata();
	std::string fullname = xui_global::unicode_to_ascii(filedata->get_full());
	std::string pathname = NPFileNameHelper::PathName(fullname);
	std::string filename = NPFileNameHelper::FileName(fullname);
	u32 id = NP2DSTextureCache::GetIns()->GetTextureID(pathname, filename, true);
	if (id != -1)
	{
		NPTexture* texture = NP2DSTextureCache::GetIns()->GetTexture(id);
		if (texture)
		{
			npu32 w = texture->GetWidth ();
			npu32 h = texture->GetHeight();
			npf32 s = (npf32)(m_tilesize) / xui_max(w, h);
			npf32 x = p1.x;
			npf32 y = p1.y;
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
				id);
		}
	}
}
xui_method_explain(onity_tileview, draw_image,				void				)( const xui_rect2d<s32>& rt, NP2DSImage* image )
{
	f32 sw = (f32)m_tilesize / (f32)image->GetSrcW();
	f32 sh = (f32)m_tilesize / (f32)image->GetSrcH();
	f32 s  = xui_min(sw, sh);
	if (s > 1.0f)
		s = 1.0f;

	NPVector3 scale = NPVector3(s, s, 1.0f);
	NPVector3 trans = NPVector3(
		(((f32)m_tilesize - image->GetSrcW() * s) / 2.0f) + (f32)rt.ax, 
		(((f32)m_tilesize - image->GetSrcH() * s) / 2.0f) + (f32)rt.ay,
		0.0f);

	trans.x = xui_pixel_align(trans.x);
	trans.y = xui_pixel_align(trans.y);

	NPTransform t;
	t.SetIdentity();
	t.SetS(scale);
	t.SetT(trans);

	NP2DSRenderUtil::GetIns()->DrawImage(
		image,
		t.GetMatrix(),
		NPColor::White,
		0,
		0,
		0,
		0);
}
xui_method_explain(onity_tileview, draw_frame,				void				)( const xui_rect2d<s32>& rt, NP2DSFrame* frame )
{
	NPRect bound = frame->GetBounding();

	f32 sw = (f32)m_tilesize / (f32)bound.GetW();
	f32 sh = (f32)m_tilesize / (f32)bound.GetH();
	f32 s  = xui_min(sw, sh);
	if (s > 1.0f)
		s = 1.0f;

	NPVector3 scale = NPVector3(s, s, 1.0f);
	NPVector3 trans = NPVector3(
		(-bound.LT*s + (((f32)rt.get_w() - bound.GetW()*s)) / 2.0f) + (f32)rt.ax, 
		(-bound.TP*s + (((f32)rt.get_h() - bound.GetH()*s)) / 2.0f) + (f32)rt.ay,
		0.0f);

	trans.x = xui_pixel_align(trans.x);
	trans.y = xui_pixel_align(trans.y);

	if (frame->WasAcceptScale())
	{
		bound.SetW((nps32)(scale.x * bound.GetW()));
		bound.SetH((nps32)(scale.y * bound.GetH()));
		bound.OftX((nps32)(trans.x));
		bound.OftY((nps32)(trans.y));
		NP2DSRenderUtil::GetIns()->DrawFrameStrench(
			frame,
			NPVector2::PositiveOne,
			bound,
			NPMatrix4x4::Identity,
			NPColor::White,
			0,
			0,
			0,
			0);
	}
	else
	{
		NPTransform t;
		t.SetIdentity();
		t.SetS(scale);
		t.SetT(trans);

		NP2DSRenderUtil::GetIns()->DrawFrame(
			frame,
			t.GetMatrix(),
			NPColor::White,
			0,
			0,
			0,
			0);
	}
}
xui_method_explain(onity_tileview, draw_actor,				void				)( const xui_rect2d<s32>& rt, NP2DSActor* actor )
{
	NPRect bound = NPRect::Empty;
	for (s32 i = (s32)actor->GetLayerCount()-1; i >= 0; --i)
	{
		NP2DSLayer* layer = actor->GetLayer((npu16)i);
		if (layer->GetFrameKeyCount() == 0)
			continue;

		std::list<NP2DSFrameKey*>& frameKeyList = layer->GetFrameKeyList();
		NP2DSFrameKey* frameKey  = frameKeyList.front();
		bound = bound.GetUnion(frameKey->GetTransRef()->GetWorldBounding());
	}

	f32 sw = (f32)m_tilesize / (f32)bound.GetW();
	f32 sh = (f32)m_tilesize / (f32)bound.GetH();
	f32 s  = xui_min(sw, sh);
	if (s > 1.0f)
		s = 1.0f;

	NPVector3 scale = NPVector3(s, s, 1.0f);
	NPVector3 trans = NPVector3(
		(-bound.LT*s + (((f32)rt.get_w() - bound.GetW()*s)) / 2.0f) + (f32)rt.ax, 
		(-bound.TP*s + (((f32)rt.get_h() - bound.GetH()*s)) / 2.0f) + (f32)rt.ay,
		0.0f);

	trans.x = xui_pixel_align(trans.x);
	trans.y = xui_pixel_align(trans.y);

	for (s32 i = (s32)actor->GetLayerCount()-1; i >= 0; --i)
	{
		NP2DSLayer* layer = actor->GetLayer((npu16)i);
		if (layer->GetFrameKeyCount() == 0)
			continue;

		std::list<NP2DSFrameKey*>& frameKeyList = layer->GetFrameKeyList();
		NP2DSFrameKey* frameKey  = frameKeyList.front();
		frameKey->GetTransRef()->Render(scale, trans);
	}
}
xui_method_explain(onity_tileview, draw_node,				void				)( s32 ic, s32 ir, s32 x, s32 y, s32 w, s32 s, xui_treenode* node )
{
	xui_rect2d<s32> tilert;
	tilert.ax = x + ic*w;
	tilert.ay = y + ir*(m_tilesize+name_size+vert_grap) - m_viewroll->get_value();
	tilert.set_w(s);
	tilert.set_h(m_tilesize+name_size);
	if (tilert.by < y)
		return;

	if (node->get_rootnode() == NULL && m_viewfile == NULL)
	{
		draw_file(tilert, node);
		draw_name(tilert, node);
	}
	else
	{
		draw_leaf(tilert, node);
		draw_name(tilert, node);
	}
}
xui_method_explain(onity_tileview, draw_background,			void				)( s32 ic, s32 ir, s32 c, s32 g, s32 w, s32 h, xui_treenode* node )
{
	xui_rect2d<s32> rt = m_drawview->get_renderrtins();

	s32 half = m_tilesize/20*2;
	s32 cnt  = (s32)node->get_leafnodecount();
	while (cnt > 0)
	{
		s32 lc = xui_min(cnt, c-ic);
		xui_rect2d<s32> drawrt;
		drawrt.ax = rt.ax + ic*w;
		drawrt.ay = rt.ay + ir*h - m_viewroll->get_value();
		drawrt.set_w(w*lc - g);
		drawrt.set_h(m_tilesize);
		if (drawrt.ay > rt.by)
			break;

		xui_convas::get_ins()->fill_round(drawrt, xui_colour(1.0f, 90.0f/255.0f), xui_rect2d<s32>(m_tilesize/10));

		if (ic == 0)
		{
			xui_vector<s32> center(drawrt.ax, drawrt.ay+drawrt.get_h()/2);
			xui_convas::get_ins()->fill_triangle(center, half, TRIANGLE_LEFT, xui_colour(1.0f, 90.0f/255.0f));
		}

		cnt -= lc;

		if (cnt > 0)
		{
			xui_vector<s32> center(drawrt.bx, drawrt.ay+drawrt.get_h()/2);
			xui_convas::get_ins()->fill_triangle(center, half, TRIANGLE_LEFT, xui_colour(1.0f, 0.25f));
		}

		++ir; ic = 0;
	}
}
xui_method_explain(onity_tileview, trim_string,				std::wstring		)( const std::wstring& text, const xui_family& font, s32 maxwidth )
{
	s32 w = xui_convas::get_ins()->calc_size(text, font, maxwidth, true).w;
	if (w > maxwidth)
	{
		std::wstring dot = L"...";
		s32 dotwidth = xui_convas::get_ins()->calc_size(dot, font, maxwidth, true).w;

		std::wstring result;
		xui_convas::get_ins()->calc_word(text, font, maxwidth-dotwidth, result);
		result += dot;
		return result;
	}
	else
	{
		return text;
	}
}