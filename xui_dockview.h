#ifndef __xui_dockview_h__
#define __xui_dockview_h__

#include "xui_container.h"

class xui_dockview : public xui_container
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_dockview( const xui_vector<s32>& size );

	/*
	//method
	*/
	xui_dockpage*		get_showpage( void );
	void				set_showpage( xui_dockpage* page );

protected:
	/*
	//member
	*/
	xui_dockpage*		m_showpage;
};

#endif//__xui_dockview_h__