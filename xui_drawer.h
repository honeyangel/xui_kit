#ifndef __xui_drawer_h__
#define __xui_drawer_h__

#include "xui_family.h"
#include "xui_family_render.h"
#include "xui_control.h"

enum
{
	IMAGE_FRONT_TEXT,
	IMAGE_AFTER_TEXT,
	IMAGE_ABOVE_TEXT,
	IMAGE_BELOW_TEXT,
};

enum
{
	IMAGE_L,
	IMAGE_R,
	IMAGE_T,
	IMAGE_B,
	IMAGE_C,
};

class xui_drawer : public xui_control
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_drawer( const xui_vector<s32>& size, xui_component* parent );

	/*
	//init
	*/
	void						ini_drawer		( const std::wstring& text, const xui_family& textfont, const xui_family_render& textdraw );
	void						ini_drawer		( xui_bitmap* icon );
	void						ini_drawer		( xui_bitmap* icon, const std::wstring& text );

	/*
	//text
	*/
	const std::wstring&			get_text		( void ) const;
	void						set_text		( const std::wstring& text );

	/*
	//icon
	*/
	xui_bitmap*					get_icon		( void );
	void						set_icon		( xui_bitmap* icon );

	/*
	//font
	*/
	const xui_family&			get_textfont	( void ) const;
	void						set_textfont	( const xui_family& textfont );

	/*
	//text draw
	*/
	const xui_family_render&	get_textdraw	( void ) const;
	void						set_textdraw	( const xui_family_render& textdraw );

	/*
	//text color
	//text align
	*/
	const xui_colour&			get_textcolor	( void ) const;
	void						set_textcolor	( const xui_colour& color );
	u08							get_textalign	( void ) const;
	void						set_textalign	( u08 align );
	const xui_vector<s32>&		get_textoffset	( void ) const;
	void						set_textoffset	( const xui_vector<s32>& offset );

	/*
	//icon align
	*/
	u08							get_iconalign	( void ) const;
	void						set_iconalign	( u08 align );
	const xui_vector<s32>&		get_iconoffset	( void ) const;
	void						set_iconoffset	( const xui_vector<s32>& offset );

	/*
	//single line
	*/
	bool						was_singleline	( void ) const;
	void						set_singleline	( bool flag );

	/*
	//virtual
	*/
	virtual xui_bitmap*			get_rendericon	( void ) const;
	virtual std::wstring		get_rendertext	( void ) const;
	virtual xui_vector<s32>		get_rendericonpt( void ) const;
	virtual xui_rect2d<s32>		get_rendertextrt( void ) const;

	/*
	//method
	*/
	xui_method<xui_method_args>	xm_textchanged;
	xui_method<xui_method_args> xm_iconchanged;
	xui_method<xui_method_args>	xm_fontchanged;

protected:
	/*
	//callback
	*/
	virtual void				on_iconchanged	( xui_method_args& args );
	virtual void				on_textchanged	( xui_method_args& args );
	virtual void				on_fontchanged	( xui_method_args& args );
	virtual void				on_renderself	( xui_method_args& args );

protected:
	/*
	//member
	*/
	std::wstring				m_text;
	xui_bitmap*					m_icon;
	xui_family					m_textfont;
	xui_family_render			m_textdraw;
	u08							m_textalign;
	u08							m_iconalign;
	xui_vector<s32>				m_textoffset;
	xui_vector<s32>				m_iconoffset;
	bool						m_singleline;
};

#endif//__xui_drawer_h__