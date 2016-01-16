#ifndef __xui_component_h__
#define __xui_component_h__

#include "xui_rect2d.h"
#include "xui_colour.h"
#include "xui_method.h"

enum
{
	ALIGNHORZ_N,	//none
	ALIGNHORZ_L,	//left
	ALIGNHORZ_R,	//right
	ALIGNHORZ_C,	//center
};

enum
{
	ALIGNVERT_N,	//none
	ALIGNVERT_T,	//top
	ALIGNVERT_B,	//bottom
	ALIGNVERT_C,	//center
};

enum
{
	DOCKSTYLE_N,	//none
	DOCKSTYLE_L,	//left
	DOCKSTYLE_T,	//top
	DOCKSTYLE_R,	//right
	DOCKSTYLE_B,	//bottom
	DOCKSTYLE_F,	//fill
};

enum
{
	CURSOR_DEFAULT,
	CURSOR_NS,
	CURSOR_WE,
	CURSOR_TEXT,
	CURSOR_SIZE,
	CURSOR_MOVE,
	CURSOR_HAND,
	CURSOR_DRAG,
	CURSOR_DRAGBAN,
	CURSOR_DRAGADD,
};

class xui_component
{
	xui_declare_root(xui_component)

public:
	friend class xui_desktop;

	/*
	//constructor
	*/
	xui_component( const xui_vector<s32>& size, xui_component* parent );

	/*
	//destructor
	*/
	virtual ~xui_component( void );

	/*
	//ini
	*/
	void							ini_component		( bool enable, bool visible );
	void							ini_component		( u08 alignhorz, u08 alignvert, u08 dockstyle );

	/*
	//name
	*/
	const std::string&				get_name			( void ) const;
	void							set_name			( const std::string& name );

	/*
	//data
	*/
	void*							get_data			( void );
	void							set_data			( void* data );

	/*
	//curosr
	*/
	u32								get_cursor			( void ) const;
	void							set_cursor			( u32 cursor );

	/* 
	//parent 
	*/
	xui_component*					get_parent			( void );
	void							set_parent			( xui_component* component );
	bool							was_parent			( xui_component* component ) const;	
	bool							was_ancestor		( xui_component* component ) const;

	/*
	//window
	*/
	xui_window*						get_window			( void );

	/* 
	//enable
	*/
	bool							was_enable			( void ) const;
	void							set_enable			( bool flag );

	/*
	//visible
	*/
	bool							was_visible			( void ) const;
	void							set_visible			( bool flag );

	/*
	//focus
	*/
	bool							has_focus			( void ) const;
	void							req_focus			( void );
	bool							has_catch			( void ) const;
	void							req_catch			( void );
	bool							was_hover			( void ) const;

	/*
	//background
	*/
	f32								get_backalpha		( void ) const;
	void							set_backalpha		( f32 alpha );
	const xui_colour&				get_backcolor		( void ) const;
	void							set_backcolor		( const xui_colour& color );
    const xui_colour&			    get_maskcolor       ( void ) const;
    void                            set_maskcolor       ( const xui_colour& color );

	/* 
	//layout 
	*/
	u08								get_alignhorz		( void ) const;
	void							set_alignhorz		( u08 alignhorz );
	u08								get_alignvert		( void ) const;
	void							set_alignvert		( u08 alignvert );
	u08								get_dockstyle		( void ) const;
	void							set_dockstyle		( u08 dockstyle );

	/*
	//location
	*/
	s32								get_renderx			( void ) const;
	s32								get_rendery			( void ) const;
	xui_vector<s32>					get_renderpt		( void ) const;
	void							set_renderx			( s32 x );
	void							set_rendery			( s32 y );
	void							set_renderpt		( const xui_vector<s32>& pt );

	s32								get_renderw			( void ) const;
	s32								get_renderh			( void ) const;
	xui_vector<s32>					get_rendersz		( void ) const;
	void							set_renderw			( s32 w );
	void							set_renderh			( s32 h );
	void							set_rendersz		( const xui_vector<s32>& sz );

	/*
	//virtual
	*/
	virtual xui_vector<s32>			get_renderpt		( const xui_vector<s32>& pt ) const;
	virtual xui_vector<s32>			get_screenpt		( void ) const;
	virtual xui_colour				get_vertexcolor		( void ) const;
	virtual xui_colour				get_rendercolor		( void ) const;

	/* 
	//rectangle 
	*/
	xui_rect2d<s32>					get_renderrt		( void ) const;
	xui_rect2d<s32>					get_renderrtabs		( void ) const;

	/*
	//choose
	//update
	//render
	*/
	virtual xui_component*			choose				( const xui_vector<s32>& pt );
	virtual void 					update				( f32 delta );
	virtual void 					render				( void );

	/* 
	//perform
	*/
	void							invalid				( void );
	void							refresh				( void );
	void							perform				( void );

	/*
	//perform set render method
	*/
	void							on_perform_x		( s32 x );
	void							on_perform_y		( s32 y );
	void							on_perform_pt		( s32 x, s32 y );
	void							on_perform_pt		( const xui_vector<s32>& pt );
	void							on_perform_w		( s32 w );
	void							on_perform_h		( s32 h );
	void							on_perform_sz		( s32 w, s32 h );
	void							on_perform_sz		( const xui_vector<s32>& sz );

	/*
	//method
	*/
	xui_method<xui_method_args>		xm_setrenderpt;
	xui_method<xui_method_args>		xm_setrendersz;

	xui_method<xui_method_args>		xm_getfocus;
	xui_method<xui_method_args>		xm_nonfocus;
	xui_method<xui_method_args>		xm_getcatch;
	xui_method<xui_method_args>		xm_noncatch;

	xui_method<xui_method_args>		xm_free;
	xui_method<xui_method_args>		xm_lock;
	xui_method<xui_method_args>		xm_show;
	xui_method<xui_method_args>		xm_hide;

	xui_method<xui_method_keybd>	xm_keybddown;
	xui_method<xui_method_keybd>	xm_keybdrise;
	xui_method<xui_method_keybd>	xm_keybdchar;
	xui_method<xui_method_mouse>	xm_mousedown;
	xui_method<xui_method_mouse>	xm_mouserise;
	xui_method<xui_method_mouse>	xm_mousemove;

	xui_method<xui_method_mouse>	xm_mouseenter;
	xui_method<xui_method_mouse>	xm_mouseleave;
	xui_method<xui_method_mouse>	xm_mouseclick;
	xui_method<xui_method_mouse>	xm_mousewheel;
	xui_method<xui_method_mouse>	xm_mousedoubleclick;

	xui_method<xui_method_dragdrop>	xm_mousedragitem;
	xui_method<xui_method_dragdrop> xm_mousedragenter;
	xui_method<xui_method_dragdrop> xm_mousedragleave;
	xui_method<xui_method_dragdrop>	xm_mousedragover;
	xui_method<xui_method_dragdrop>	xm_mousedragdrop;

	xui_method<xui_method_args>		xm_updateself;
	xui_method<xui_method_args>		xm_renderself;
	xui_method<xui_method_args>		xm_topdraw;
	xui_method<xui_method_args>		xm_invalid;
	xui_method<xui_method_args>		xm_perform;

protected:
	/*
	//callback
	*/
	virtual void					on_setrenderpt		( xui_method_args&		args );
	virtual void					on_setrendersz		( xui_method_args&		args );

	virtual void					on_getfocus			( xui_method_args&		args );
	virtual void					on_nonfocus			( xui_method_args&		args );
	virtual void					on_getcatch			( xui_method_args&		args );
	virtual void					on_noncatch			( xui_method_args&		args );

	virtual void					on_free				( xui_method_args&		args );
	virtual void					on_lock				( xui_method_args&		args );
	virtual void					on_show				( xui_method_args&		args );
	virtual void					on_hide				( xui_method_args&		args );

	virtual void					on_keybddown		( xui_method_keybd&		args );
	virtual void					on_keybdrise		( xui_method_keybd&		args );
	virtual void					on_keybdchar		( xui_method_keybd&		args );
	virtual void 					on_mousedown		( xui_method_mouse&		args );	
	virtual void 					on_mouserise		( xui_method_mouse&		args );	
	virtual void					on_mousemove		( xui_method_mouse&		args );

	virtual void					on_mouseenter		( xui_method_mouse&		args );
	virtual void					on_mouseleave		( xui_method_mouse&		args );
	virtual void					on_mouseclick		( xui_method_mouse&		args );
	virtual void					on_mousewheel		( xui_method_mouse&		args );
	virtual void					on_mousedoubleclick	( xui_method_mouse&		args );

	virtual void					on_mousedragitem	( xui_method_dragdrop&	args );
	virtual void					on_mousedragenter	( xui_method_dragdrop&  args );
	virtual void					on_mousedragleave	( xui_method_dragdrop&  args );
	virtual void					on_mousedragover	( xui_method_dragdrop&	args );
	virtual void					on_mousedragdrop	( xui_method_dragdrop&	args );

	virtual void					on_renderback		( xui_method_args&		args );
	virtual void					on_renderself		( xui_method_args&		args );
	virtual void					on_updateself		( xui_method_args&		args );
	virtual void					on_topdraw			( xui_method_args&		args );
	virtual void					on_invalid			( xui_method_args&		args );
	virtual void					on_perform			( xui_method_args&		args );

	/*
	//perform implement
	*/
	void							perform_alignhorz	( const xui_rect2d<s32>& rect, const std::vector<xui_component*>& vec );
	void							perform_alignvert	( const xui_rect2d<s32>& rect, const std::vector<xui_component*>& vec );
	void							perform_dockstyle	( const xui_rect2d<s32>& rect, const std::vector<xui_component*>& vec );

protected:
	/*
	//member
	*/
	std::string						m_name;
	void*							m_data;

	u32								m_cursor;
	xui_component*					m_parent;
	xui_rect2d<s32>					m_render;
	bool							m_enable;
	bool							m_visible;
	bool							m_invalid;

	u08								m_alignhorz;
	u08								m_alignvert;
	u08								m_dockstyle;

	f32								m_backalpha;
	xui_colour						m_backcolor;
    xui_colour						m_maskcolor;
};

#endif//__xui_component_h__