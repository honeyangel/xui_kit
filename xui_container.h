#ifndef __xui_container_h__
#define __xui_container_h__

#include "xui_control.h"

class xui_container : public xui_control
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_container( const xui_vector<s32>& size );

	/*
	//destructor
	*/
	virtual ~xui_container( void );

	/*
	//show
	*/
	xui_scroll*					get_vscroll		( void ) const;
	xui_scroll*					get_hscroll		( void ) const;
	bool						was_vscrollshow	( void ) const;
	void						set_vscrollshow	( bool flag );
	bool						was_hscrollshow	( void ) const;
	void						set_hscrollshow	( bool flag );
	bool						was_vscrollauto	( void ) const;
	void						set_vscrollauto	( bool flag );
	bool						was_hscrollauto ( void ) const;
	void						set_hscrollauto	( bool flag );

	/*
	//menu
	*/
	xui_menu*					get_contextmenu	( void );
	void						set_contextmenu	( xui_menu* menu );

	/*
	//rectangle
	*/
	virtual xui_rect2d<s32>		get_renderrtins	( void ) const;

	/*
	//virtual
	*/
	virtual xui_component*		choose_else		( const xui_vector<s32>& pt );
	virtual void				update_else		( f32 delta );
	virtual void				render_else		( void );

	/*
	//method
	*/
	xui_method<xui_method_args>	xm_vertvalue;
	xui_method<xui_method_args> xm_horzvalue;

protected:
	/*
	//callback
	*/
	virtual void				on_perform		( xui_method_args&  args );
	virtual void				on_setrendersz	( xui_method_args&  args );
	virtual void				on_setclientsz	( xui_method_args&  args );
	virtual void				on_setborderrt	( xui_method_args&  args );
	virtual void				on_mousedown	( xui_method_mouse& args );
	virtual void				on_mousewheel	( xui_method_mouse& args );

	/*
	//method
	*/
	virtual void				on_vertvalue	( xui_method_args&  args );
	virtual void				on_horzvalue	( xui_method_args&  args );
	void						on_scroll		( xui_component* sender, xui_method_args& args );

	/*
	//scroll
	*/
	void						update_scroll	( void );
	void						create_scroll	( u08 style );
	void						delete_scroll	( u08 style );

	/*
	//member
	*/
	xui_scroll*					m_vscroll;
	xui_scroll*					m_hscroll;
	bool						m_vscrollshow;
	bool						m_hscrollshow;
	bool						m_vscrollauto;
	bool						m_hscrollauto;
	std::vector<xui_control*>	m_ascrollitem;
	xui_menu*					m_contextmenu;
};

#endif//__xui_container_h__