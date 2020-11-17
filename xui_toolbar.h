#ifndef __xui_toolbar_h__
#define __xui_toolbar_h__

#include "xui_control.h"

class xui_toolbar : public xui_control
{
	xui_declare_rtti

public:
	static xui_toolbar*	create		( void );

	xui_toolbar( const xui_vector<s32>& size );

	u08					get_flow	( void ) const;
	void				set_flow	( u08 flow );
	s32					get_grap	( void ) const;
	void				set_grap	( s32 grap );
	void				add_separate( void );
	void				add_item	( xui_component* component );
	void				del_item	( xui_component* component );

protected:
	virtual void		on_invalid	( xui_method_args& args );
	virtual void		on_perform	( xui_method_args& args );

	u08					m_flow;
	s32					m_grap;
};

#endif//__xui_toolbar_h__