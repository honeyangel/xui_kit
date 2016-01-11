#ifndef __xui_treeplus_h__
#define __xui_treeplus_h__

#include "xui_componet.h"

class xui_treeplus : public xui_componet
{
public:
	/*
	//constructor
	*/
	xui_treeplus( xui_treenode* treenode );

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

#endif//__xui_treeplus_h__