#ifndef __onity_boundbox_entitynode_h__
#define __onity_boundbox_entitynode_h__

#include "onity_boundbox.h"

class NPNode;
class NPRect;
class onity_boundbox_entitynode : public onity_boundbox
{
public:
	/*
	//constructor
	*/
	onity_boundbox_entitynode( xui_proproot* prop );

	/*
	//override
	*/
	virtual xui_vector<s32> ori_position	( void );
	virtual void			set_position	( const xui_vector<s32>& pos );
	virtual xui_rect2d<s32>	ori_bounding	( void );
	virtual bool			was_selected	( void );

protected:
	/*
	//method
	*/
	void					cal_bounding	( NPNode* node, NPRect* result );
};

#endif//__onity_boundbox_entitynode_h__