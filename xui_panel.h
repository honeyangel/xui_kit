#ifndef __xui_panel_h__
#define __xui_panel_h__

#include "xui_container.h"

class xui_panel : public xui_container
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_panel( const xui_vector<s32>& size, xui_component* parent );

	/*
	//destructor
	*/
	virtual ~xui_panel( void );

	/*
	//child
	*/
	const std::vector<xui_control*>&	get_children		( void ) const;
	u32									get_childcount		( void ) const;
	u32									get_childindex		( xui_control* child ) const;
	void								set_childindex		( xui_control* child, u32 index );

	bool								was_child			( xui_control* child ) const;
	bool								was_descendant		( xui_control* child ) const;

	xui_control*						get_child			( u32 index ) const;
	xui_control*						get_child			( const std::string& name ) const;
	xui_control*						get_descendant		( const std::string& name ) const;

	void								add_child			( xui_control* child );
	void								del_child			( xui_control* child, bool destroy = true );
	void								del_children		( void );

	/*
	//mov
	*/
	void								movfore				( xui_control* child );
	void								movback				( xui_control* child );
	void								realign				( void );

	/*
	//virtual
	*/
	virtual xui_component*				choose_else			( const xui_vector<s32>& pt );
	virtual void						update_else			( f32 delta );
	virtual void						render_else			( void );

	/*
	//method
	*/
	xui_method<xui_method_args>			xm_addchild;
	xui_method<xui_method_args>			xm_delchild;

protected:
	/*
	//callback
	*/
	virtual void						on_addchild			( xui_method_args& args );
	virtual void						on_delchild			( xui_method_args& args );
	virtual void						on_perform			( xui_method_args& args );
	virtual void						on_invalid			( xui_method_args& args );
	virtual void						on_vertvalue		( xui_method_args& args );
	virtual void						on_horzvalue		( xui_method_args& args );

protected:
	/*
	//member
	*/
	std::vector<xui_control*>			m_childctrl;
	std::vector<xui_component*>			m_resizevec;
};

#endif//__xui_panel_h__