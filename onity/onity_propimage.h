#ifndef __onity_propimage_h__
#define __onity_propimage_h__

#include "onity_prop2dsasset.h"

class NP2DSImage;
class onity_boundbox;
class onity_propimage : public onity_prop2dsasset
{
public:
	/*
	//constructor
	*/
	onity_propimage( onity_propfile* propfile, u32 id );

	/*
	//destructor
	*/
	virtual ~onity_propimage( void );

	/*
	//method
	*/
	onity_boundbox*			get_boundbox	( void );
	NP2DSImage*				get_image		( void );
	//virtual xui_vector<s32>	ori_position	( void );
	//virtual	xui_rect2d<s32>	ori_bounding	( void ); 
	//virtual void			set_position	( const xui_vector<s32>& pos );

protected:
	/*
	//static
	*/
	static s32				get_splitmode	( void* userptr );
	static void				set_splitmode	( void* userptr, s32  value );
	static bool				was_alphablend	( void* userptr );
	static void				set_alphablend	( void* userptr, bool value );
	static bool				was_scalestyle	( void* userptr );
	static void				set_scalestyle	( void* userptr, bool value );
	static xui_rect2d<f64>	get_rectangle	( void* userptr );
	static void				set_rectangle	( void* userptr, const xui_rect2d<f64>& value );

	/*
	//member
	*/
	xui_propkind*			m_imagekind;
	onity_boundbox*			m_boundctrl;
};

#endif//__onity_propimage_h__