#ifndef __xui_convas_h__
#define __xui_convas_h__

#include "xui_rect2d.h"
#include "xui_colour.h"
#include "xui_family.h"
#include "xui_family_render.h"

enum
{
	TRIANGLE_UP,
	TRIANGLE_DOWN,
	TRIANGLE_LEFT,
	TRIANGLE_RIGHT
};

class xui_convas
{
	xui_declare_instance(xui_convas)

public:
	/*
	//constructor
	*/
	xui_convas( void );

	/*
	//destructor
	*/
	virtual ~xui_convas( void );

	/*
	//begin
	//clear
	*/
	void					begin				( void );
	void					clear				( const xui_colour& color );
	void					present				( void );

	/*
	//transform
	*/
	void					set_transform		( f32 angle, const xui_vector<f32>& scale, const xui_vector<f32>& translate );
	void					non_transform		( void );

	/*
	//cliprect
	//viewport
	*/
	const xui_rect2d<s32>&	get_viewport		( void ) const;
	void					set_viewport		( const xui_rect2d<s32>		rt );
	const xui_rect2d<s32>&	get_cliprect		( void ) const;
	void					set_cliprect		( const xui_rect2d<s32>&	rt );


	/*
	//image
	*/
	void					draw_image			( xui_bitmap*				image,
												  const xui_vector<s32>&	pt,
												  const xui_colour&			color );

	void					draw_image			( xui_bitmap*				image,
												  const xui_rect2d<s32>&	dst,
												  const xui_colour&			color );

	void					draw_image			( xui_bitmap*				image,
												  const xui_rect2d<s32>&	src,
												  const xui_rect2d<s32>&	dst,
												  const xui_colour&			color );

	/*
	//text
	*/
	std::wstring			trim_text			( const std::wstring&		text, 
												  const xui_family&			textfont, 
												  s32						maxwidth );

	xui_vector<s32>			calc_size			( const std::wstring&		text, 
												  const xui_family&			textfont, 
												  s32						maxwidth, 
												  bool						singleline );

	xui_rect2d<s32>			calc_draw			( const std::wstring&		text, 
												  const xui_family&			textfont, 
												  const xui_rect2d<s32>&	textrect, 
												  u08						textalign,
												  bool						singleline );

	std::wstring			calc_text			( const std::wstring&		text, 
												  const xui_family&			textfont, 
												  s32						maxwidth,
												  bool						singleline );

	u32						calc_char			( const std::wstring&		text, 
												  const xui_family&			textfont, 
												  s32						position );

	u32						calc_word			( const std::wstring&		text, 
												  const xui_family&			textfont, 
												  s32						maxwidth, 
												  bool						singleline,
												  std::wstring&				word );

	void					draw_text			( const std::wstring&		text,
												  const xui_family&			textfont,
												  const xui_rect2d<s32>&	drawrect,
												  const xui_family_render&	textdraw, 
												  bool						singleline );

	void					draw_text			( const std::wstring&		text,
												  const xui_family&			textfont,
												  const xui_vector<s32>&	screenpt,
												  const xui_family_render&	textdraw );

	/*
	//line
	*/
	void					draw_line			( const xui_vector<s32>&	p1, 
												  const xui_vector<s32>&	p2,
												  const xui_colour&			color );

	/*
	//path
	*/
	void					draw_path			( xui_vector<s32>*			pt,
												  u32						count,
												  const xui_colour&			color );

	void					fill_poly			( xui_vector<s32>*			pt, 
												  u32						count,
												  const xui_colour&			color );

	/*
	//triangle
	*/
	void					draw_triangle		( const xui_vector<s32>&	center,
												  s32						half,
												  u08						direction,
												  const xui_colour&			color );

	void					fill_triangle		( const xui_vector<s32>&	center, 
												  s32						half,
												  u08						direction,
												  const xui_colour&			color );

	/*
	//rectangle
	*/
	void					draw_rectangle		( const xui_rect2d<s32>&	rt,
												  const xui_colour&			color,
												  s32						thick = 1 );

	void					fill_rectangle		( const xui_rect2d<s32>&	rt, 
												  const xui_colour&			color,
												  bool						smooth = false );

	void					fill_rectangle		( const xui_rect2d<s32>&	rt, 
												  xui_colour*				colors );

	/*
	//round
	*/
	void					draw_round			( const xui_rect2d<s32>&	rt,
												  const xui_colour&			color,
												  s32						corner );

	void					draw_round			( const xui_rect2d<s32>&	rt,
												  const xui_colour&			color,
												  const xui_rect2d<s32>&	corner,
												  s32						thick = 1 );

	void					fill_round			( const xui_rect2d<s32>&	rt,
												  const xui_colour&			color, 
												  s32						corner );

	void					fill_round			( const xui_rect2d<s32>&	rt,
												  const xui_colour&			color,
												  const xui_rect2d<s32>&	corner );

	void					draw_circle			( const xui_vector<s32>&	center, 
												  s32						radius,
												  const xui_colour&			color,
												  s32						start,
												  s32						sweep );

	void					fill_circle			( const xui_vector<s32>&	center, 
												  s32						radius,
												  const xui_colour&			color,
												  s32						start,
												  s32						sweep );

	/*
	//arc
	*/
	void					draw_arc			( const xui_rect2d<s32>&	rt,
												  const xui_colour&			color,
												  s32						start, 
												  s32						sweep,
												  s32						precision,
												  bool						smooth = false );

	void					fill_arc			( const xui_rect2d<s32>&	rt,
												  const xui_colour&			color,
												  s32						start,
												  s32						sweep,
												  s32						precision );

	/*
	//flag
	*/
	void					draw_tick			( const xui_vector<s32>&	center,
												  s32						half,
												  const xui_colour&			color );
	/*
	//method
	*/
	xui_family_create*		get_family_create	( void );
	xui_family_member*		get_family_member	( const xui_family& family, u16 wc );

protected:
	/*
	//typedef
	*/
	typedef std::vector<xui_family_bitmap*>
		family_bitmapvec;

	/*
	//member
	*/
	xui_rect2d<s32>			m_viewport;
	xui_rect2d<s32>			m_cliprect;
	xui_family_create*		m_familycreate;
	family_bitmapvec		m_familybitmapvec;
};

#endif//__xui_convas_h__