#include "2d/CCSpriteFrameCache.h"
#include "2d/CCSpriteFrame.h"
#include "xui_global.h"
#include "xui_treedata.h"
#include "xui_treenode.h"
#include "xui_textbox.h"
#include "xui_propkind.h"
#include "xui_propctrl_vector.h"
#include "xui_propctrl_rect2d.h"
#include "xui_kindctrl.h"
#include "cocos_savekind.h"
#include "cocos_resource.h"
#include "cocos_propatlas.h"
#include "cocos_propframe.h"

/*
//constructor
*/
xui_create_explain(cocos_propframe)( xui_bitmap* icon, cocos_propfile* propfile, const std::string& name )
: cocos_propleaf(propfile)
, m_framename(name)
{
	m_framekind = new xui_propkind(this, xui_global::ascii_to_unicode(name), "SpriteFrame", xui_kindctrl::create, icon, true);
	m_framekind->add_propdata(new xui_propdata_vector(
		m_framekind,
		L"OriginalSize",
		xui_propctrl_vector::create,
		get_originalsize,
		NULL,
		this,
		NT_INT));
	m_framekind->add_propdata(new xui_propdata_vector(
		m_framekind,
		L"Offset",
		xui_propctrl_vector::create,
		get_offset,
		NULL,
		this,
		NT_INT));
	m_framekind->add_propdata(new xui_propdata_rect2d(
		m_framekind,
		L"Rect",
		xui_propctrl_rect2d::create,
		get_rect,
		NULL,
		this,
		NT_INT));
	m_framekind->add_propdata(new xui_propdata_bool(
		m_framekind,
		L"Rotated",
		xui_propctrl_bool::create,
		was_rotated,
		NULL,
		this));

	add_propkind(m_framekind);
}

/*
//method
*/
xui_method_explain(cocos_propframe, get_frame,			cocos2d::SpriteFrame*	)(void)
{
	return cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(m_framename);
}
xui_method_explain(cocos_propframe, get_framename,		const std::string&		)(void) const
{
	return m_framename;
}
xui_method_explain(cocos_propframe, get_frameicon,		xui_bitmap*				)(void)
{
	return m_framekind->get_icon();
}

/*
//override
*/
xui_method_explain(cocos_propframe, get_dragtype,		std::string				)(void)
{
	return "SpriteFrame";
}
xui_method_explain(cocos_propframe, get_dragdata,		void*					)(void)
{
	return get_frame();
}

/*
//static
*/
xui_method_explain(cocos_propframe, get_originalsize,	xui_vector<f64>			)( void* userptr )
{
	cocos_propframe* prop = (cocos_propframe*)userptr;
	cocos2d::SpriteFrame* frame = prop->get_frame();
	cocos2d::Size size = frame->getOriginalSizeInPixels();
	return xui_vector<f64>(size.width, size.height);
}
xui_method_explain(cocos_propframe, get_offset,			xui_vector<f64>			)( void* userptr )
{
	cocos_propframe* prop = (cocos_propframe*)userptr;
	cocos2d::SpriteFrame* frame = prop->get_frame();
	cocos2d::Vec2 offset = frame->getOffsetInPixels();
	return xui_vector<f64>(offset.x, offset.y);
}
xui_method_explain(cocos_propframe, get_rect,			xui_rect2d<f64>			)( void* userptr )
{
	cocos_propframe* prop = (cocos_propframe*)userptr;
	cocos2d::SpriteFrame* frame = prop->get_frame();
	cocos2d::Rect rect = frame->getRectInPixels();
	return xui_rect2d<f64>(
		rect.getMinX(),
		rect.getMinY(),
		rect.getMaxX(),
		rect.getMaxY());
}
xui_method_explain(cocos_propframe, was_rotated,		bool					)( void* userptr )
{
	cocos_propframe* prop = (cocos_propframe*)userptr;
	cocos2d::SpriteFrame* frame = prop->get_frame();
	return frame->isRotated();
}