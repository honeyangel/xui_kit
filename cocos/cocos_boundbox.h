#ifndef __cocos_boundbox_h__
#define __cocos_boundbox_h__

#include "xui_vector.h"
#include "xui_rect2d.h"

enum 
{
	OP_NONE		= 0,
	OP_TRANS	= 0x01,
	OP_PIVOT	= 0x02,
	OP_SCALE	= 0x04,
};

enum 
{
	BO_NONE,
	BO_MOVE,
	BO_MOVE_P,
	BO_SIZE_L,
	BO_SIZE_T,
	BO_SIZE_R,
	BO_SIZE_B,
	BO_SIZE_LT,
	BO_SIZE_RT,
	BO_SIZE_LB,
	BO_SIZE_RB,
};

class cocos_propnodebase;
class cocos_boundbox
{
public:
	/*
	//constructor
	*/
	cocos_boundbox( cocos_propnodebase* prop );

	/*
	//static
	*/
	static u32				get_opcursor	( u08 op );

	/*
	//position
	*/
	xui_vector<s32>			get_pivotbox	( const xui_vector<s32>& trans, f64 ratio, s32 viewh );
	xui_vector<s32>			get_position	( const xui_vector<s32>& trans, f64 ratio, s32 viewh );
	virtual xui_vector<s32>	ori_position	( void );
	virtual void			set_position	( const xui_vector<s32>& pos );

	/*
	//bounding
	*/
	xui_rect2d<s32>			get_bounding	( const xui_vector<s32>& trans, f64 ratio, s32 viewh );
	virtual	xui_rect2d<s32>	ori_bounding	( void ); 
	virtual void			opt_bounding	( const xui_vector<s32>& trans, f64 ratio, s32 viewh, const xui_vector<s32>& delta, u08 op );
	virtual void			set_bounding	( const xui_rect2d<s32>& rt, u08 op );

	/*
	//method
	*/
	cocos_propnodebase*		get_linkprop	( void );
	void					syn_bounding	( const xui_vector<s32>& trans, f64 ratio, s32 viewh );
	void					add_operator	( u08 op );
	void					set_ignoresz	( bool flag );
	virtual u08				hit_operator	( const xui_vector<s32>& trans, f64 ratio, s32 viewh, const xui_vector<s32>& pt );
	virtual bool			was_selected	( void );

	/*
	//render
	*/
	virtual void			draw			( const xui_vector<s32>& trans, f64 ratio, s32 viewh, const xui_vector<s32>& pt, bool alwaysdraw = false );
	virtual void			draw_bounding	( const xui_rect2d<s32>& rt );
	virtual void			draw_operator	( const xui_rect2d<s32>& rt, const xui_vector<s32>& pivot );

protected:
	/*
	//member
	*/
	cocos_propnodebase*		m_linkprop;
	xui_rect2d<s32>			m_bounding;
	xui_vector<s32>			m_pivotbox;
	u08						m_operator;
	bool					m_ignoresz;
};

#endif//__cocos_boundbox_h__