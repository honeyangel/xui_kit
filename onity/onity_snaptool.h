#ifndef __onity_snaptool_h__
#define __onity_snaptool_h__

#include "xui_rect2d.h"

inline s32 xui_round( f64 value )
{
	return (s32)(value + (value > 0.0 ? 0.5 : -0.5));
}

class onity_bounding;
class onity_snaptool
{
public:
	/*
	//constructor
	*/
	onity_snaptool( xui_control* drawview );

	/*
	//method
	*/
	void					set_trans					( const xui_vector<s32>& trans );
	void					set_ratio					( f64 ratio );
	xui_toggle*				get_snapctrl				( void );
	onity_bounding*			cal_selfprop				( const std::vector<onity_bounding*>& selectedvec );
	void					cal_snapinfo				( const std::vector<onity_bounding*>& boundingvec, const xui_rect2d<s32>& self, u08 mode, s32 snaplength );
	void					use_snapinfo				( const std::vector<onity_bounding*>& boundingvec, const std::vector<onity_bounding*>& selectedvec );
	void					set_snapdraw				( const std::vector<onity_bounding*>& boundingvec, const std::vector<onity_bounding*>& selectedvec );

protected:
	/*
	//snapinfo
	*/
	class snap_info
	{
	public:
		snap_info( onity_bounding* _curr, onity_bounding* _next = NULL )
		{
			curr = _curr;
			next = _next;
		}

		onity_bounding*		curr;
		onity_bounding*		next;
	};

	/*
	//calculate
	*/
	xui_rect2d<s32>			cal_selfrect				( const std::vector<onity_bounding*>& selectedvec );
	xui_vector<s32>			cal_snapmove				( const std::vector<onity_bounding*>& boundingvec, const xui_rect2d<s32>& self, xui_vector<s32>& snap, xui_vector<s32>& step, xui_rect2d<s32>* horz, xui_rect2d<s32>* vert );
	bool					cal_linestep				( s32& last, s32 temp );

	/*
	//draw
	*/
	void					draw_horzsnap				( const xui_rect2d<s32>& self, s32 snap, const std::vector<snap_info>& vec );
	void					draw_vertsnap				( const xui_rect2d<s32>& self, s32 snap, const std::vector<snap_info>& vec );
	void					draw_horzstep				( const xui_rect2d<s32>& self, s32 snap, const std::vector<snap_info>& vec, const xui_rect2d<s32>& curr );
	void					draw_vertstep				( const xui_rect2d<s32>& self, s32 snap, const std::vector<snap_info>& vec, const xui_rect2d<s32>& curr );

	/*
	//typedef
	*/
	typedef std::map< s32, std::vector<snap_info> >
		onity_snapinfo_map;

	/*
	//member
	*/
	xui_vector<s32>			m_trans;
	f64						m_ratio;
	xui_toggle*				m_snapctrl;
	xui_control*			m_drawview;
	onity_snapinfo_map		m_horzsnap;
	onity_snapinfo_map		m_vertsnap;
	onity_snapinfo_map		m_horzmidd;
	onity_snapinfo_map		m_vertmidd;
	onity_snapinfo_map		m_horzstep;
	onity_snapinfo_map		m_vertstep;
};

#endif//__onity_snaptool_h__