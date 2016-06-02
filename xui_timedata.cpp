#include "xui_timedata.h"

/*
//constructor
*/
xui_create_explain(xui_timedata)( void )
: xui_treedata()
, m_line(NULL)
{}
xui_create_explain(xui_timedata)( const std::wstring& text, xui_bitmap* icon, const std::map<s32, u08>& keyframe )
: xui_treedata(text, icon)
, m_line(NULL)
{
	m_keyframe = keyframe;
}

/*
//method
*/
xui_method_explain(xui_timedata, get_line,			xui_timeline*		)( void )
{
	return m_line;
}

/*
//interface
*/
xui_method_explain(xui_timedata, get_keyframecount,	u32					)( void ) const
{
	return m_keyframe.size();
}
xui_method_explain(xui_timedata, get_allstyle,		xui_keystyle_map	)( void ) const
{
	return m_keyframe;
}
xui_method_explain(xui_timedata, get_allframe,		std::vector<s32>	)( void ) const
{
	std::vector<s32> keyframe;
	for (std::map<s32, u08>::const_iterator itor = m_keyframe.begin(); itor != m_keyframe.end(); ++itor)
	{
		keyframe.push_back((*itor).first);
	}

	return keyframe;
}
xui_method_explain(xui_timedata, get_keycolor,		xui_colour			)( void ) const
{
	return xui_colour(1.0f, 0.0f, 1.0f, 0.0f);
}
xui_method_explain(xui_timedata, has_keyframe,		bool				)( s32 frame ) const
{
	return m_keyframe.find(frame) != m_keyframe.end();
}
xui_method_explain(xui_timedata, get_keyframe,		s32					)( u32 index ) const
{
	std::map<s32, u08>::const_iterator itor = m_keyframe.begin();
	std::advance(itor, index);
	return (*itor).first;
}
xui_method_explain(xui_timedata, get_keystyle,		u08					)( u32 index ) const
{
	std::map<s32, u08>::const_iterator itor = m_keyframe.begin();
	std::advance(itor, index);
	return (*itor).second;
}

/*
//method
*/
xui_method_explain(xui_timedata, set_line,			void				)( xui_timeline* line )
{
	m_line = line;
}