#ifndef __onity_layerdata_h__
#define __onity_layerdata_h__

#include "onity_timedata.h"

enum 
{
	LAYER_FLAG_LEAD = 1,
	LAYER_FLAG_SHOW,
};

class onity_layerdata : public onity_timedata
{
public:
	/*
	//constructor
	*/
	onity_layerdata( xui_proproot* prop );

	/*
	//override
	*/
	virtual xui_bitmap*		get_icon	( u32 index );
	virtual std::wstring	get_text	( u32 index );
	virtual void			set_text	( u32 index, const std::wstring& text );
	virtual bool			get_flag	( u32 index );
	virtual void			set_flag	( u32 index, bool flag );
	virtual xui_colour		get_keycolor( void ) const;
	virtual void			cal_keyframe( void );
};

#endif//__onity_layerdata_h__