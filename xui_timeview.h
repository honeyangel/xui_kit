#ifndef __xui_timeview_h__
#define __xui_timeview_h__

#include "xui_family_render.h"
#include "xui_treeview.h"

enum 
{
	TIMEDRAG_NONE,
	TIMEDRAG_SELECT,
	TIMEDRAG_SELECT_AND_AFTER,
};

typedef std::map<s32, std::vector<xui_timeline*>> xui_keyframe_map;

class xui_timeview : public xui_container
{
	friend class xui_timeline;
	xui_declare_rtti

public:
	/*
	//static
	*/
	static xui_timeview*				create					( s32 width, const std::vector<xui_treecolumn>& columninfo );

	/*
	//constructor
	*/
	xui_timeview( const xui_vector<s32>& size, const std::vector<xui_treecolumn>& columninfo, s32 lineheight = 24 );

	/*
	//control
	*/
	xui_timegrad*						get_timegrad			( void );
	xui_timetool*						get_timetool			( void );
	xui_timehead*						get_timehead			( void );
	xui_treeview*						get_timetree			( void );

	/*
	//property
	*/
	void								add_toolitem			( xui_component*			item );
	void								set_headtext			( const std::wstring&		text );
	void								set_gradfont			( const xui_family&			font );
	void								set_graddraw			( const xui_family_render&  draw );

	/*
	//method
	*/
	const xui_keyframe_map&				get_allframe			( void ) const;
	s32									get_curframe			( void ) const;
	void								set_curframe			( s32 frame );
	s32									get_keyspace			( void ) const;
	s32									get_keylarge			( void ) const;
	s32									get_keysmall			( void ) const;
	void								set_keyspace			( s32 space );

	/*
	//drag
	*/
	s32									get_droptime			( void ) const;
	void								set_droptime			( s32 time );
	s32									get_dragtime			( void ) const;
	void								set_dragtime			( s32 time );
	u08									get_dragmode			( void ) const;
	void								set_dragmode			( u08 mode );

	/*
	//rect
	*/
	bool								was_rectmove			( xui_component* layer, s32 frame );
	void								set_rectshow			( s32 startlayer, s32 finallayer, s32 startframe, s32 finalframe );
	void								set_rectrise			( void );

	/*
	//selected
	*/
	s32									get_selectedallfirst	( void ) const;
	bool								was_selectedall			( const std::vector<xui_timeline*>& lines, s32 frame ) const;
	bool								has_selectedline		( const std::vector<xui_timeline*>& lines, s32 frame ) const;
	void								set_selectedline		( s32 start, s32 final, bool selected );
	void								non_selectedline		( bool fireMethod = true );
	std::vector<xui_timeline*>			get_selectedline		( void );

	/*
	//timeline
	*/
	std::vector<xui_timeline*>			get_timelinetotal		( bool total = true );
	u32									get_timelinecount		( void ) const;
	std::vector<xui_timeline*>			get_timelinearray		( void ) const;
	xui_timeline*						get_timeline			( u32 index );
	xui_timeline*						add_timeline			( u32 index, xui_timedata* data );
	void								del_timeline			( xui_timeline* line );

	/*
	//rectangle
	*/
	virtual xui_rect2d<s32>				get_renderrtins			( void ) const;

	/*
	//virtual
	*/
	virtual xui_component*				choose_else				( const xui_vector<s32>& pt );
	virtual void						update_else				( f32 delta );
	virtual void						render_else				( void );

	/*
	//method
	*/
	xui_method<xui_method_args>			xm_selecteddrag;
	xui_method<xui_method_args>			xm_selectedchange;
	xui_method<xui_method_args>			xm_curframechange;
	xui_method<xui_method_args>			xm_timelineaddlayer;
	xui_method<xui_method_args>			xm_timelinedellayer;
	xui_method<xui_method_args>			xm_timelineselected;
	xui_method<xui_method_treedragdrop>	xm_timelinedragover;
	xui_method<xui_method_treedragdrop>	xm_timelinedragdrop;

protected:
	/*
	//override
	*/
	virtual void						on_invalid				( xui_method_args& args );
	virtual void						on_perform				( xui_method_args& args );
	virtual void						on_vertvalue			( xui_method_args& args );
	virtual void						on_renderself			( xui_method_args& args );

	/*
	//method
	*/
	xui_timeline*						create_line				( xui_treenode* node, xui_timedata* data );
	void								delete_line				( xui_timeline* line );

	/*
	//event
	*/
	void								on_timetreenonfocus		( xui_component* sender, xui_method_args&			args );
	void								on_timetreegetfocus		( xui_component* sender, xui_method_args&			args );
	void								on_timetreeupdateself	( xui_component* sender, xui_method_update&			args );
	void								on_timetreesetclientsz	( xui_component* sender, xui_method_args&			args );
	void								on_timetreeselection	( xui_component* sender, xui_method_args&			args );
	void								on_timetreedragover		( xui_component* sender, xui_method_treedragdrop&	args );
	void								on_timetreedragdrop		( xui_component* sender, xui_method_treedragdrop&	args );
	void								on_timetreeinvalid		( xui_component* sender, xui_method_args&			args );
	void								on_timeviewdraghorz		( xui_component* sender, xui_method_update&			args );
	void								on_timeviewdragvert		( xui_component* sender, xui_method_update&			args );
	void								on_timerectdraghorz		( xui_component* sender, xui_method_update&			args );
	void								on_kssliderscroll		( xui_component* sender, xui_method_args&			args );
	void								on_fpnumberrenderself	( xui_component* sender, xui_method_args&			args );
	void								on_fpnumbermousemove	( xui_component* sender, xui_method_mouse&			args );
	void								on_tldeleterenderself	( xui_component* sender, xui_method_args&			args );
	void								on_tldeleteclick		( xui_component* sender, xui_method_args&			args );

	/*
	//member
	*/
	xui_treeview*						m_timetree;
	xui_timetool*						m_timetool;
	xui_timegrad*						m_timegrad;
	xui_timehead*						m_timehead;
	xui_timerect*						m_timerect;
	xui_slider*							m_ksslider;
	xui_drawer*							m_fpstring;
	xui_drawer*							m_fpnumber;
	xui_button*							m_tldelete;
	u08									m_dragmode;
	s32									m_dragtime;
	s32									m_droptime;
	s32									m_curframe;
	s32									m_keyspace;
	s32									m_keylarge;
	s32									m_keysmall;
	xui_keyframe_map					m_allframe;
};

#endif//__xui_timeview_h__