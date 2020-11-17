#ifndef __xui_timedata_h__
#define __xui_timedata_h__

#include "xui_treedata.h"

enum KeyStyle
{
	k_ks_static,
	k_ks_linear,
	k_ks_bezier,
};

typedef std::map<s32, u08> xui_keystyle_map;

class xui_timedata : public xui_treedata
{
	friend class xui_timeline;

public:
	xui_timedata( void );
	xui_timedata( const std::wstring& text, xui_bitmap* icon );
	xui_timedata( const std::wstring& text, xui_bitmap* icon, const std::map<s32, u08>& keyframe );
	virtual ~xui_timedata( void ){};

	xui_timeline*				get_line			( void );

	virtual xui_colour			get_keycolor		( void ) const;
	virtual void				cal_keyframe		( void );

	u32							get_keyframecount	( void ) const;
	xui_keystyle_map			get_allstyle		( void ) const;
	std::vector<s32>			get_allframe		( void ) const;
	bool						has_keyframe		( s32 frame ) const;
	s32							get_keyframe		( u32 index ) const;
	u08							get_keystyle		( u32 index ) const;

protected:
	void						set_line			( xui_timeline* line );

	xui_timeline*				m_line;
	xui_keystyle_map			m_keyframe;
};

#endif//__xui_timedata_h__