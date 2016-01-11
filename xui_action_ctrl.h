#ifndef __xui_action_ctrl_h__
#define __xui_action_ctrl_h__

#if 0
#include "xui_action_data.h"

class xui_action_ctrl
{
public:
	/*
	//constructor
	*/
	xui_action_ctrl( xui_widget* widget );

	/*
	//method
	*/
	void					set_event			( u32 event );
	void					add_frame			( s32 frame );
	void					set_framevec		( const std::vector<s32>& framevec );

	bool					was_play			( void ) const;
	void					set_play			( bool flag );
	bool					was_loop			( void ) const;
	void					set_loop			( bool flag );
	bool					was_soft			( void ) const;
	void					set_soft			( bool flag );

	f32						get_timetotal		( void ) const;
	f32						get_time			( void ) const;
	void					set_time			( f32 time );

	virtual void			reset				( void );
	virtual void			clear				( void );

	virtual void			update				( f32 delta_time );
	virtual void			update_widget		( void ) = 0;

protected:
	/*
	//method
	*/
	u32						get_currframeidx	( void ) const;
	u32						get_nextframeidx	( void ) const;

	/*
	//member
	*/
	bool					m_loop;
	bool					m_soft;
	bool					m_play;
	f32						m_steptime;
	f32						m_lasttime;
	f32						m_currtime;

	xui_widget*				m_widget;
	u32						m_event;
	std::vector<s32>		m_framevec;
};

class xui_action_ctrl_move : public xui_action_ctrl
{
public:
	/*
	//constructor
	*/
	xui_action_ctrl_move( xui_widget* widget )
	: xui_action_ctrl(widget)
	{}

	/*
	//method
	*/
	void					add_movex			( s32				movex );
	void					add_movey			( s32				movey );
	void					add_sizew			( s32				sizew );
	void					add_sizeh			( s32				sizeh );
	void					add_alpha			( f32				alpha );
	void					add_color			( const xui_colour& color );

	void					set_movexvec		( const std::vector<s32>&		 movexvec );
	void					set_moveyvec		( const std::vector<s32>&		 moveyvec );
	void					set_sizewvec		( const std::vector<s32>&		 sizewvec );
	void					set_sizehvec		( const std::vector<s32>&		 sizehvec );
	void					set_alphavec		( const std::vector<f32>&		 alphavec );
	void					set_colorvec		( const std::vector<xui_colour>& colorvec );

	void					set_actiondata		( const xui_action_data& data );

	virtual void			clear				( void );
	virtual void			update_widget		( void );

protected:
	/*
	//member
	*/
	std::vector<s32>		m_movexvec;
	std::vector<s32>		m_moveyvec;
	std::vector<s32>		m_sizewvec;
	std::vector<s32>		m_sizehvec;
	std::vector<f32>		m_alphavec;
	std::vector<xui_colour>	m_colorvec;
};

#endif
#endif//__xui_action_ctrl_h__