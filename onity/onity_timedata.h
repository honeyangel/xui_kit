#ifndef __onity_timedata_h__
#define __onity_timedata_h__

#include "xui_timedata.h"

class onity_timedata : public xui_timedata
{
public:
	/*
	//constructor
	*/
	onity_timedata( xui_bitmap* icon, xui_proproot* prop );

	/*
	//destructor
	*/
	virtual ~onity_timedata( void );

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

#endif//__onity_timedata_h__