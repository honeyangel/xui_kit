#ifndef __xui_convas_h__
#define __xui_convas_h__

#include "xui_rect2d.h"
#include "xui_colour.h"
#include "xui_family.h"

enum
{
	TA_LT,
	TA_LC,
	TA_LB,
	TA_CT,
	TA_CC,
	TA_CB,
	TA_RT,
	TA_RC,
	TA_RB,
};

class xui_convas
{
public:
	/*
	//constructor
	*/
	xui_convas( void );

	/*
	//clip
	*/
	xui_rect2d<s32>		get_clip_rect		( void ) const;
	void				set_clip_rect		( const xui_rect2d<s32>&	rect );

	/*
	//image
	*/
	void				draw_image			( xui_bitmap*				image,
											  const xui_vector<s32>&	pt,
											  const xui_colour&			color );

	void				draw_image			( xui_bitmap*				image,
											  const xui_rect2d<s32>&	dst,
											  const xui_colour&			color );

	void				draw_image			( xui_bitmap*				image,
											  const xui_rect2d<s32>&	src,
											  const xui_rect2d<s32>&	dst,
											  const xui_colour&			color );

	/*
	//text
	*/
	xui_vector<s32>		calc_size			( const std::wstring&		text, 
											  const xui_family&			family, 
											  const xui_rect2d<s32>&	rect, 
											  bool						flag );

	xui_rect2d<s32>		calc_rect			( const std::wstring&		text, 
											  const xui_family&			family, 
											  const xui_rect2d<s32>&	rect, 
											  u08						align,
											  bool						flag );

	std::wstring		calc_text			( const std::wstring&		text, 
											  const xui_family&			family, 
											  s32						maxw );

	u32					calc_char			( const std::wstring&		text, 
											  const xui_family&			family, 
											  s32						x );

	u32					calc_word			( const std::wstring&		text, 
											  const xui_family&			family, 
											  s32						maxw, 
											  std::wstring&				word );

	void				draw_text			( const std::wstring&		text,
											  const xui_family&			family,
											  const xui_rect2d<s32>&	rect,
											  const xui_colour&			color );

	void				draw_text			( const std::wstring&		text,
											  const xui_family&			family,
											  const xui_vector<s32>&	pt,
											  const xui_colour&			color );

	/*
	//line
	*/
	void				draw_line			( const xui_vector<s32>&	min, 
											  const xui_vector<s32>&	max,
											  const xui_colour&			color,
											  s32						width = 1 );

	/*
	//path
	*/
	void				draw_path			( xui_vector<s32>*			path,
											  u32						count,
											  const xui_colour&			color );

	void				fill_poly			( xui_vector<s32>*			path, 
											  u32						count,
											  const xui_colour&			color );

	/*
	//rectangle
	*/
	void				draw_rectangle		( const xui_rect2d<s32>&	rect,
											  const xui_colour&			color );

	void				fill_rectangle		( const xui_rect2d<s32>&	rect, 
											  const xui_colour&			color );

	/*
	//round
	*/
	void				draw_round			( const xui_rect2d<s32>&	rect,
											  const xui_colour&			color,
											  s32						corner );

	void				draw_round			( const xui_rect2d<s32>&	rect,
											  const xui_colour&			color,
											  const xui_rect2d<s32>&	corner );

	void				fill_round			( const xui_rect2d<s32>&	rect,
											  const xui_colour&			color, 
											  s32						corner );

	void				fill_round			( const xui_rect2d<s32>&	rect,
											  const xui_colour&			color,
											  const xui_rect2d<s32>&	corner );

	/*
	//arc
	*/
	void				draw_arc			( const xui_rect2d<s32>&	rect,
											  const xui_colour&			color,
											  f32						start, 
											  f32						sweep,
											  f32						precision );

	void				fill_arc			( const xui_rect2d<s32>&	rect,
											  const xui_colour&			color,
											  f32						start,
											  f32						sweep,
											  f32						precision );

protected:
	/*
	//method
	*/
	xui_family_bitmap*	get_family_bitmap	( const xui_family& family );
	xui_family_member*	get_family_member	( const xui_family& family, u16 wc );
};

/*
//global
*/
extern xui_convas* g_convas;

#endif//__xui_convas_h__