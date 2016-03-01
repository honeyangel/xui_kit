#ifndef __onity_timeline_h__
#define __onity_timeline_h__

#include "xui_dockpage.h"

class onity_timeline : public xui_dockpage
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_timeline( void );

protected:
	/*
	//member
	*/
	xui_timeview*	m_view;
};

#endif//__onity_timeline_h__