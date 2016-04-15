#ifndef __onity_propframe_h__
#define __onity_propframe_h__

#include "onity_prop2dsasset.h"

class onity_propframe : public onity_prop2dsasset
{
public:
	/*
	//constructor
	*/
	onity_propframe( onity_propfile* propfile, u32 id );

protected:
	/*
	//static
	*/
	static bool					was_boundcustom	( void* userptr );
	static void					set_boundcustom	( void* userptr, bool value );
	static xui_rect2d<f64>		get_boundingbox	( void* userptr );
	static void					set_boundingbox	( void* userptr, const xui_rect2d<f64>& value );

	/*
	//member
	*/
	xui_propkind*				m_framekind;
};

#endif//__onity_propframe_h__