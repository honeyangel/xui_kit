#ifndef __onity_layerdata_h__
#define __onity_layerdata_h__

#include "onity_timedata.h"

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
	virtual std::wstring	get_text( u32 index );
	virtual void			set_text( u32 index, const std::wstring& text );
	virtual bool			get_flag( u32 index );
	virtual void			set_flag( u32 index, bool flag );
};

#endif//__onity_layerdata_h__