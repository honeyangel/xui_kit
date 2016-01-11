#ifndef __xui_timeline_h__
#define __xui_timeline_h__

#include "xui_control.h"

class xui_timeline : public xui_control
{
public:
	/*
	//constructor
	*/
	xui_timeline( xui_timeview* timeview, xui_timedata* linkdata );

	/*
	//method
	*/
	xui_timedata*				get_linkdata		( void );

	/*
	//selected frame
	*/
	bool						was_selafter		( s32 frame ) const;
	bool						has_selframe		( s32 frame ) const;
	bool						has_selframe		( void ) const;
	const std::vector<s32>&		get_selframe		( void ) const;
	void						set_selframe		( const std::vector<s32>& selframe );
	bool						add_selframe		( s32 start, s32 final );
	void						del_selframe		( s32 start, s32 final );

	/*
	//timeline
	*/
	xui_timeline*				get_rootline		( void );
	void						get_timelinetotal	( std::vector<xui_timeline*>& lines, bool total = true );
	u32							get_timelinecount	( void ) const;
	std::vector<xui_timeline*>	get_timelinearray	( void ) const;
	xui_timeline*				get_timeline		( u32 index );
	xui_timeline*				add_timeline		( u32 index, xui_timedata* data );
	void						del_timeline		( xui_timeline* line );


protected:
	/*
	//override
	*/
	virtual void				on_mousedown		( xui_method_mouse& args );
	virtual void				on_mousemove		( xui_method_mouse& args );
	virtual void				on_mouserise		( xui_method_mouse& args );
	virtual void				on_renderself		( xui_method_args&  args );

	/*
	//method
	*/
	s32							hit_frame			( s32 x );

	/*
	//member
	*/
	xui_vector<s32>				m_downrela;
	xui_vector<s32>				m_currrela;
	xui_timedata*				m_linkdata;
	std::vector<s32>			m_selframe;
};

#endif//__xui_timeline_h__