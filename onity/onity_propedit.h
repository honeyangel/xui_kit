#ifndef __onity_propedit_h__
#define __onity_propedit_h__

#include "onity_proproot.h"

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
	virtual xui_vector<s32> get_position	( void );
	virtual void			set_position	( const xui_vector<s32>& pos );
	virtual xui_rect2d<s32>	get_bounding	( void );
};

#endif//__onity_propedit_h__