#ifndef __xui_textbox_h__
#define __xui_textbox_h__

#include "xui_drawer.h"
#include "xui_caretdrawer.h"

class xui_textbox : public xui_drawer
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_textbox( const xui_vector<s32>& size, xui_component* parent );

	/*
	//destructor
	*/
	virtual ~xui_textbox( void );

	/*
	//init
	*/
	void						ini_textbox		( const std::wstring& text );

	/*
	//hint text
	*/
	const std::wstring&			get_hinttext	( void ) const;
	void						set_hinttext	( const std::wstring& hint );
	const xui_family_render&	get_hintdraw	( void ) const;
	void						set_hintdraw	( const xui_family_render& hintdraw );

	/*
	//property
	*/
	bool						was_password	( void ) const;
	void						set_password	( bool flag );
	bool						was_numbonly	( void ) const;
	void						set_numbonly	( bool flag );
	bool						was_readonly	( void ) const;
	void						set_readonly	( bool flag );

	/*
	//caret
	*/
	u32							get_caretindex	( void ) const;
	u32							hit_caretindex	( const xui_vector<s32>& pt ) const;
	void						set_caretindex	( u32 index, bool force = false );
	void						set_caretvisible( void );

	/*
	//select
	*/
	u32							get_selectstart ( void ) const;
	u32							get_selectfinal ( void ) const;
	u32							get_selectcount ( void ) const;
	void						set_selecttext	( u32 start, u32 final );

	/*
	//update&render
	*/
	virtual void				update			( f32 delta );
	virtual void				render			( void );

	/*
	//method
	*/
	xui_method<xui_method_args>	xm_textenter;

protected:
	/*
	//callback
	*/
	virtual void				on_keybddown	( xui_method_keybd& args );
	virtual void				on_keybdchar	( xui_method_keybd& args );
	virtual void				on_mousedown	( xui_method_mouse& args );
	virtual void				on_mousemove	( xui_method_mouse& args );
	virtual void				on_renderself	( xui_method_args&  args );
	virtual void				on_textchanged	( xui_method_args&  args );
	virtual void				on_setrenderpt	( xui_method_args&  args );
	virtual void				on_setrendersz	( xui_method_args&  args );
	virtual void				on_setborderrt	( xui_method_args&  args );

	/*
	//virtual
	*/
	virtual std::wstring		get_rendertext	( void ) const;
	virtual xui_vector<s32>		get_rendericonpt( void ) const;
	virtual xui_rect2d<s32>		get_rendertextrt( void ) const;

	/*
	//keyboard
	*/
	void						do_larrow		( bool shift );
	void						do_rarrow		( bool shift );
	void						do_home			( bool shift );
	void						do_end			( bool shift );
	void						do_enter		( void );
	void						do_back			( void );
	void						do_delete		( void );
	void						do_select		( void );
	void						do_copy			( void );
	void						do_cut			( void );
	void						do_paste		( void );

	/*
	//selection
	*/
	void						del_selecttext	( void );
	void						non_selecttext	( void );
	std::wstring				get_selecttext	( void ) const;

	/*
	//method
	*/
	void						set_caretrect	( void );
	u32							get_textwidth	( const std::wstring& text ) const;
	u32							get_charindex	( const std::wstring& text, s32 x ) const;
	u32							get_showcount	( void ) const;

protected:
	/*
	//member
	*/
	xui_family_render			m_hintdraw;
	std::wstring				m_hinttext;
	bool						m_password;
	bool						m_readonly;
	bool						m_numbonly;
	u32							m_selectstart;
	u32							m_selectfinal;
	xui_caretdrawer*			m_caretdrawer;
	u32							m_caretcurrindex;
	u32							m_caretshowindex;
	u32							m_firstshowindex;
	u32							m_caretdragindex;
};

#endif//__xui_textbox_h__