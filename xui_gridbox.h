#ifndef __xui_gridbox_h__
#define __xui_gridbox_h__

#include "xui_control.h"

class xui_gridbox : public xui_control
{
	xui_declare_rtti

public:
	static xui_gridbox*		create			( u32 row, u32 col, s32 titlewidth );

	xui_gridbox( u32 row = 2, u32 col = 2 );

	u32						get_rowcount	( void ) const;
	void					set_rowcount	( u32 count );
	u32						get_colcount	( void ) const;
	void					set_colcount	( u32 count );
	s32						get_rowpixel	( u32 row ) const;
	void					set_rowpixel	( u32 row, s32 pixel );
	s32						get_colpixel	( u32 col ) const;
	void					set_colpixel	( u32 col, s32 pixel );

	void					set_gridctrl	( u32 row, u32 col, xui_control* ctrl );
	virtual xui_rect2d<s32>	get_cornerrt	( xui_component* component ) const;
	virtual void			render_else		( void );

protected:
	virtual void			on_invalid		( xui_method_args& args );
	virtual void			on_perform		( xui_method_args& args );

	static const s32		k_max_row = 10;
	static const s32		k_max_col = 10;

	u32						m_rowcount;
	u32						m_colcount;
	s32						m_rowpixel[k_max_row];
	s32						m_colpixel[k_max_col];
	xui_control*			m_ptrarray[k_max_row][k_max_col];
};

#endif//__xui_gridbox_h__