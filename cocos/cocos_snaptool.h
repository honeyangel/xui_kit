#ifndef __cocos_snaptool_h__
#define __cocos_snaptool_h__

#include "xui_rect2d.h"

inline s32 xui_round( f64 value )
{
	return (s32)(value + (value > 0.0 ? 0.5 : -0.5));
}

enum 
{
	DRAGMOVE_UNLIMIT,
	DRAGMOVE_X,
	DRAGMOVE_Y,
};

class cocos_boundbox;
class cocos_snaptool
{
public:
	/*
	//constructor
	*/
	cocos_snaptool( xui_control* drawview );

	/*
	//method
	*/
	void					set_trans					( const xui_vector<s32>& trans );
	void					set_ratio					( f64 ratio );
	xui_toggle*				get_snapctrl				( void );
	cocos_boundbox*			cal_selfbbox				( const std::vector<cocos_boundbox*>& selectedvec );
	void					cal_snapinfo				( const std::vector<cocos_boundbox*>& boundboxvec, const xui_rect2d<s32>& self, u08 mode, s32 snaplength );
	void					use_snapinfo				( const std::vector<cocos_boundbox*>& boundboxvec, const std::vector<cocos_boundbox*>& selectedvec );
	void					set_snapdraw				( const std::vector<cocos_boundbox*>& boundboxvec, const std::vector<cocos_boundbox*>& selectedvec );

protected:
	/*
	//snapinfo
	*/
	class snap_info
	{
	public:
		snap_info( cocos_boundbox* _curr, cocos_boundbox* _next = NULL )
		{
			curr = _curr;
			next = _next;
		}

		cocos_boundbox*		curr;
		cocos_boundbox*		next;
	};

	/*
	//calculate
	*/
	xui_rect2d<s32>			cal_selfrect				( const std::vector<cocos_boundbox*>& selectedvec );
	xui_vector<s32>			cal_snapmove				( const std::vector<cocos_boundbox*>& boundboxvec, const xui_rect2d<s32>& self, xui_vector<s32>& snap, xui_vector<s32>& step, xui_rect2d<s32>* horz, xui_rect2d<s32>* vert );
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
		cocos_snapinfo_map;

	/*
	//member
	*/
	xui_vector<s32>			m_trans;
	f64						m_ratio;
	xui_toggle*				m_snapctrl;
	xui_control*			m_drawview;
	cocos_snapinfo_map		m_horzsnap;
	cocos_snapinfo_map		m_vertsnap;
	cocos_snapinfo_map		m_horzmidd;
	cocos_snapinfo_map		m_vertmidd;
	cocos_snapinfo_map		m_horzstep;
	cocos_snapinfo_map		m_vertstep;
};

#endif//__cocos_snaptool_h__