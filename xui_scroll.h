#ifndef __xui_scroll_h__
#define __xui_scroll_h__

#include "xui_control.h"
#include "xui_action_ctrl.h"

enum
{
	k_arrow_dec,
	k_arrow_inc,
	k_arrow_max,
};

class xui_scroll : public xui_control
{
	friend class xui_scrollthumb;
	xui_declare_rtti

public:
	static const s32			k_default_size = 16;
	static xui_scroll*			create			( u08 style );

	xui_scroll( const xui_vector<s32>& size, u08 style );
	virtual ~xui_scroll( void );

	void						ini_scroll		( s32 range, s32 value );
	xui_scrollarrow*			get_arrow		( u08 arrow ) const;
	xui_scrollthumb*			get_thumb		( void ) const;
	xui_action_ctrl*			get_valueaction	( void );
	s32							get_smallchange ( void ) const;
	void						set_smallchange ( s32 value );
	s32							get_largechange	( void ) const;
	void						set_largechange	( s32 value );
	bool						was_thumbresize	( void ) const;
	void						set_thumbresize	( bool flag );
	s32							get_range		( void ) const;
	void						set_range		( s32 range );
	s32							get_value		( void ) const;
	void						set_value		( s32 value, bool stop_action = true );
	virtual xui_rect2d<s32>		get_hallowrt	( void ) const;

	xui_method<xui_method_args>	xm_scroll;

protected:
	virtual void				on_lock			( xui_method_args&   args );
	virtual void				on_free			( xui_method_args&   args );
	virtual void				on_perform		( xui_method_args&   args );
	virtual void				on_mousedown	( xui_method_mouse&  args );
	virtual void				on_updateself	( xui_method_update& args );

	void						resize_thumb	( void );
	void						update_thumb	( void );
	void						update_value	( void );
	void						on_actiontick	( xui_component* sender, xui_method_args& args );

	xui_action_ctrl*			m_valueaction;
	s32							m_smallchange;
	s32							m_largechange;
	bool						m_thumbresize;
	u08							m_style;
	s32							m_value;
	s32							m_range;
	xui_scrollthumb*			m_thumb;
	xui_scrollarrow*			m_arrow[k_arrow_max];
};

#endif//__xui_scroll_h__