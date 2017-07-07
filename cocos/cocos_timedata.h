#ifndef __cocos_timedata_h__
#define __cocos_timedata_h__

#include "xui_timedata.h"

class cocos_timedata : public xui_timedata
{
public:
	/*
	//constructor
	*/
	cocos_timedata( xui_bitmap* icon, xui_proproot* prop );

	/*
	//destructor
	*/
	virtual ~cocos_timedata( void );

	/*
	//method
	*/
	xui_proproot*		get_prop( void );
	void				set_null( void );

protected:
	/*
	//member
	*/
	xui_proproot*		m_prop;
};

#endif//__cocos_timedata_h__