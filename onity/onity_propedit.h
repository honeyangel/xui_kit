#ifndef __onity_propedit_h__
#define __onity_propedit_h__

#include "onity_proproot.h"

enum 
{
	DRAGMOVE_UNLIMIT,
	DRAGMOVE_X,
	DRAGMOVE_Y,
};

class onity_propedit : public onity_proproot
{
public:
	/*
	//constructor
	*/
	onity_propedit( void );

	/*
	//virtual
	*/
	xui_vector<s32>			get_position	( const xui_vector<s32>& trans, f64 ratio );
	virtual void			set_position	( const xui_vector<s32>& pos );
	virtual xui_vector<s32>	ori_position	( void );
	virtual	xui_rect2d<s32>	ori_bounding	( void ); 
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

#endif//__onity_propedit_h__