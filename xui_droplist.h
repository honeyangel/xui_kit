#ifndef __xui_droplist_h__
#define __xui_droplist_h__

#include "xui_listview.h"

class xui_droplist : public xui_listview
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_droplist( const xui_vector<s32>& size, bool drawtick );

protected:
	/*
	//event
	*/
	void	on_popactiontick	( xui_component* sender, xui_method_args& args );
};

#endif//__xui_droplist_h__