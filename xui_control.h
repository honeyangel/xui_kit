#ifndef __xui_control_h__
#define __xui_control_h__

#include "xui_component.h"

enum
{
	k_sidestyle_n,	//none
	k_sidestyle_s,	//single
	k_sidestyle_d,	//double
};

enum
{
	k_flowstyle_h,
	k_flowstyle_v,
};

class xui_control : public xui_component
{
	xui_declare_rtti

public:
	static const xui_colour k_default_sidecolor;

	xui_control( const xui_vector<s32>& size );
	virtual ~xui_control( void );

	s32								get_clientw			( void ) const;
	s32								get_clienth			( void ) const;
	xui_vector<s32>					get_clientsz		( void ) const;
	void							set_clientsz		( const xui_vector<s32>& sz );
	const xui_rect2d<s32>&			get_borderrt		( void ) const;
	void							set_borderrt		( const xui_rect2d<s32>& rt );

	s32								get_scrollx			( void ) const;
	s32								get_scrolly			( void ) const;
	void							set_scrollx			( s32 x );
	void							set_scrolly			( s32 y );
	const xui_vector<s32>&			get_scrollpt		( void ) const;
	void							set_scrollpt		( const xui_vector<s32>& pt );

	s32								get_corner			( void ) const;
	void							set_corner			( s32 corner );

	bool							was_drawcolor		( void ) const;
	void							set_drawcolor		( bool flag );
	u08								get_sidestyle		( void ) const;
	void							set_sidestyle		( u08 sidestyle );
	const xui_colour&				get_sidecolor		( void ) const;
	void							set_sidecolor		( const xui_colour& color );

	xui_rect2d<s32>					get_clientrt		( void ) const;
	virtual xui_vector<s32>			get_screenpt		( void ) const;
	virtual xui_rect2d<s32>			get_cornerrt		( void ) const;
	virtual xui_rect2d<s32>			get_cornerrt		( xui_component* widget ) const;
	virtual xui_rect2d<s32>			get_renderrtins		( void ) const;

	virtual xui_component*			choose				( const xui_vector<s32>& pt );
	virtual xui_component*			choose_else			( const xui_vector<s32>& pt );
	virtual void 					update				( f32 delta );
	virtual void					update_else			( f32 delta );
	virtual void 					render				( void );
	virtual void					render_else			( void );

	xui_method<xui_method_args>		xm_setclientsz;
	xui_method<xui_method_args>		xm_setborderrt;
	xui_method<xui_method_args>		xm_renderelse;

protected:
	virtual void					on_setclientsz		( xui_method_args& args );
	virtual void					on_setborderrt		( xui_method_args& args );
	virtual void					on_renderback		( xui_method_args& args );
	virtual void					on_renderself		( xui_method_args& args );
	virtual void					on_perform			( xui_method_args& args );

protected:
	xui_vector<s32>					m_client;
	xui_rect2d<s32>					m_border;
	xui_vector<s32>					m_scroll;
	s32								m_corner;
	bool							m_drawcolor;
	u08								m_sidestyle;
	xui_colour						m_sidecolor;
	xui_vector<f32>					m_backpivot;
	xui_vector<f32>					m_backscale;
	std::vector<xui_component*>		m_widgetvec;
};

#endif//__xui_control_h__