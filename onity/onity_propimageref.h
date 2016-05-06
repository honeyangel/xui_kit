#ifndef __onity_propimageref_h__
#define __onity_propimageref_h__

#include "onity_proptransref.h"

class NP2DSImageRef;
class onity_propimageref : public onity_proptransref
{
public:
	/*
	//constructor
	*/
	onity_propimageref( onity_propfile* propfile, NP2DSTransRef* transref );

	/*
	//method
	*/
	NP2DSImageRef*			get_imageref	( void );

protected:
	/*
	//static
	*/
	void*					get_image		( void* userptr );
	void					set_image		( void* userptr, void* value );

	/*
	//member
	*/
	xui_propkind*			m_reskind;
};

#endif//__onity_propimageref_h__