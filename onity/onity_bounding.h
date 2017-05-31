#ifndef __onity_bounding_h__
#define __onity_bounding_h__

#include "xui_vector.h"
#include "xui_rect2d.h"

enum 
{
	DRAGMOVE_UNLIMIT,
	DRAGMOVE_X,
	DRAGMOVE_Y,
};

class onity_bounding
{
public:
	/*
	//constructor
	*/
	onity_bounding( void );

	/*
	//virtual
	*/
	virtual xui_vector<s32>	ori_position	( void ) = 0;
	virtual	xui_rect2d<s32>	ori_bounding	( void ) = 0; 
	virtual void			set_position	( const xui_vector<s32>& pos ) = 0;
	xui_vector<s32>			get_position	( const xui_vector<s32>& trans, f64 ratio );
	xui_rect2d<s32>			get_bounding	( const xui_vector<s32>& trans, f64 ratio );
	void					mov_position	( const xui_vector<s32>& delta );
	void					set_lockdata	( bool flag );

protected:
	/*
	//member
	*/
	xui_vector<s32>			m_position;
	xui_rect2d<s32>			m_bounding;
	bool					m_lockdata;
};

#endif//__onity_bounding_h__