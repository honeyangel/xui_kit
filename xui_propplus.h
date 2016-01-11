#ifndef __xui_propplus_h__
#define __xui_propplus_h__

#include "xui_componet.h"

class xui_propplus : public xui_componet
{
public:
	/*
	//constructor
	*/
	xui_propplus( void );

	/*
	//method
	*/
	bool			was_expanded	( void ) const;
	void			set_expanded	( bool flag );

protected:
	/*
	//callback
	*/
	virtual void	on_mousedown	( xui_method_mouse& args );
	virtual void	on_renderself	( xui_method_args&  args );

	/*
	//member
	*/
	bool			m_expanded;
};

#endif//__xui_propplus_h__