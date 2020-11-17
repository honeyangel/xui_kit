#ifndef __xui_separate_h__
#define __xui_separate_h__

#include "xui_component.h"

class xui_separate : public xui_component
{
	xui_declare_rtti

public:
	xui_separate( const xui_vector<s32>& size, u08 flow );

protected:
	virtual void	on_renderback	( xui_method_args& args );

	u08				m_flow;
};

#endif//__xui_separate_h__