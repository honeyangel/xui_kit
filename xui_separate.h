#ifndef __xui_separate_h__
#define __xui_separate_h__

#include "xui_component.h"

class xui_separate : public xui_component
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_separate( const std::string& name, const xui_rect2d<s32>& rect );
	xui_separate( const xui_vector<s32>& size, xui_component* parent );

protected:
	/*
	//callback
	*/
	virtual void	on_renderback	( xui_method_args& args );
	u08				get_flow		( void );
};

#endif//__xui_separate_h__