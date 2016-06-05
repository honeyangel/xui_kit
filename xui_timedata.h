#ifndef __xui_timedata_h__
#define __xui_timedata_h__

#include "xui_treedata.h"

enum KeyStyle
{
	KS_STATIC,
	KS_LINEAR,
	KS_BEZIER,
};

typedef std::map<s32, u08> xui_keystyle_map;

class xui_timedata : public xui_treedata
{
	friend class xui_timeline;

public:
	/*
	//constructor
	*/
	xui_timedata( void );
	xui_timedata( const std::wstring& text, xui_bitmap* icon );
	xui_timedata( const std::wstring& text, xui_bitmap* icon, const std::map<s32, u08>& keyframe );

	/*
	//destructor
	*/
	virtual ~xui_timedata( void ){};

	/*
	//method
	*/
	xui_timeline*				get_line			( void );

	/*
	//interface
	*/
	virtual xui_colour			get_keycolor		( void ) const;
	virtual void				cal_keyframe		( void );

	u32							get_keyframecount	( void ) const;
	xui_keystyle_map			get_allstyle		( void ) const;
	std::vector<s32>			get_allframe		( void ) const;
	bool						has_keyframe		( s32 frame ) const;
	s32							get_keyframe		( u32 index ) const;
	u08							get_keystyle		( u32 index ) const;

protected:
	/*
	//method
	*/
	void						set_line			( xui_timeline* line );

	/*
	//member
	*/
	xui_timeline*				m_line;
	xui_keystyle_map			m_keyframe;
};

#endif//__xui_timedata_h__