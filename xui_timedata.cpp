#include "xui_timedata.h"

xui_timedata::xui_timedata( void )
: xui_treedata()
, m_line(NULL)
{}

xui_timedata::xui_timedata( const std::wstring& text, xui_bitmap* icon )
: xui_treedata(text, icon)
, m_line(NULL)
{}

xui_timedata::xui_timedata( const std::wstring& text, xui_bitmap* icon, const std::map<s32, u08>& keyframe )
: xui_treedata(text, icon)
, m_line(NULL)
{
	m_keyframe = keyframe;
}

xui_timeline* xui_timedata::get_line( void )
{
	return m_line;
}

xui_colour xui_timedata::get_keycolor( void ) const
{
	return xui_colour(1.0f, 0.0f, 1.0f, 0.0f);
}

void xui_timedata::cal_keyframe( void )
{}

u32 xui_timedata::get_keyframecount( void ) const
{
	return m_keyframe.size();
}

xui_keystyle_map xui_timedata::get_allstyle( void ) const
{
	return m_keyframe;
}

std::vector<s32> xui_timedata::get_allframe( void ) const
{
	std::vector<s32> keyframe;
	for (std::map<s32, u08>::const_iterator itor = m_keyframe.begin(); itor != m_keyframe.end(); ++itor)
	{
		keyframe.push_back((*itor).first);
	}

	return keyframe;
}

bool xui_timedata::has_keyframe( s32 frame ) const
{
	return m_keyframe.find(frame) != m_keyframe.end();
}

s32 xui_timedata::get_keyframe( u32 index ) const
{
	std::map<s32, u08>::const_iterator itor = m_keyframe.begin();
	std::advance(itor, index);
	return (*itor).first;
}

u08 xui_timedata::get_keystyle( u32 index ) const
{
	std::map<s32, u08>::const_iterator itor = m_keyframe.begin();
	std::advance(itor, index);
	return (*itor).second;
}

void xui_timedata::set_line( xui_timeline* line )
{
	m_line = line;
}