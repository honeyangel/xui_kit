#ifndef __xui_gridbox_h__
#define __xui_gridbox_h__

#include "xui_control.h"

class xui_gridbox : public xui_control
{
public:
	/*
	//constructor
	*/
	xui_gridbox( const std::string& name, const xui_rect2d<s32>& rect );

	/*
	//count
	*/
	u32						get_rowcount	( void ) const;
	void					set_rowcount	( u32 count );
	u32						get_colcount	( void ) const;
	void					set_colcount	( u32 count );

	/*
	//pixel
	*/
	s32						get_rowpixel	( u32 row ) const;
	void					set_rowpixel	( u32 row, s32 pixel );
	s32						get_colpixel	( u32 col ) const;
	void					set_colpixel	( u32 col, s32 pixel );

	/*
	//grid ctrl
	*/
	void					set_gridctrl	( u32 row, u32 col, xui_componet* componet );
	virtual xui_rect2d<s32>	get_cornerrt	( xui_componet* componet ) const;

protected:
	/*
	//callback
	*/
	virtual void			on_invalid		( xui_method_args& args );
	virtual void			on_perform		( xui_method_args& args );

	/*
	//static
	*/
	static const s32		MAX_ROW = 10;
	static const s32		MAX_COL = 10;

	/*
	//member
	*/
	u32						m_rowcount;
	u32						m_colcount;
	s32						m_rowpixel[MAX_ROW];
	s32						m_colpixel[MAX_COL];
	xui_componet*			m_ptrarray[MAX_ROW][MAX_COL];
};

#endif//__xui_gridbox_h__