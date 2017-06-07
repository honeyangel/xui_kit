#ifndef __onity_boundbox_image_h__
#define __onity_boundbox_image_h__

#include "onity_boundbox.h"

class onity_boundbox_image : public onity_boundbox
{
public:
	/*
	//constructor
	*/
	onity_boundbox_image( xui_proproot* prop );

	/*
	//override
	*/
	virtual xui_vector<s32>	ori_position	( void );
	virtual void			set_position	( const xui_vector<s32>& pos );
	virtual xui_rect2d<s32>	ori_bounding	( void );
	virtual void			set_bounding	( const xui_rect2d<s32>& rt, u08 op );
	virtual bool			was_selected	( void );

	/*
	//draw
	*/
	virtual void			draw_operator	( const xui_rect2d<s32>& rt, const xui_vector<s32>& pivot );
};

#endif//__onity_boundbox_image_h__