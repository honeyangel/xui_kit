#include "xui_bitmap.h"
#include "xui_convas.h"
#include "xui_desktop.h"
#include "xui_textbox.h"

xui_implement_rtti(xui_textbox, xui_drawer);

/*
//static
*/
const xui_colour xui_textbox::default_selectedcolor = xui_colour(1.0f, 0.6f);
xui_method_explain(xui_textbox, create, xui_textbox*)( s32 width )
{
	xui_textbox* textbox = new xui_textbox(xui_vector<s32>(width, 24));
	xui_method_ptrcall(textbox, set_backcolor	)(xui_colour(1.0f, 0.20f));
	xui_method_ptrcall(textbox, set_drawcolor	)(true);
	xui_method_ptrcall(textbox, set_sidestyle	)(SIDESTYLE_S);
	xui_method_ptrcall(textbox, set_borderrt	)(xui_rect2d<s32>(4));
	xui_method_ptrcall(textbox, set_textalign	)(TEXTALIGN_RC);
	xui_method_ptrcall(textbox, set_hintdraw	)(xui_family_render(xui_colour::gray));

	return textbox;
}

/*
//constructor
*/
xui_create_explain(xui_textbox)( const xui_vector<s32>& size )
: xui_drawer(size)
{
	m_cursor		 = CURSOR_TEXT;
	m_password		 = false;
	m_readonly		 = false;
	m_numbonly		 = false;
	m_caretdrawer	 = new xui_caretdrawer;
	m_selectstart	 = 0;
	m_selectfinal	 = 0;
	m_caretcurrindex = 0;
	m_caretdragindex = 0;
	m_firstshowindex = 0;
}

/*
//destructor
*/
xui_delete_explain(xui_textbox)( void )
{
	delete m_caretdrawer;
}

/*
//init
*/
xui_method_explain(xui_textbox, ini_textbox,		void					)( const std::wstring& text )
{
	m_text = text;
	non_selecttext();
	set_caretindex(m_text.length());
}

/*
//hint text
*/
xui_method_explain(xui_textbox, get_hinttext,		const std::wstring&		)( void ) const
{
	return m_hinttext;
}
xui_method_explain(xui_textbox, set_hinttext,		void					)( const std::wstring& hint )
{
	m_hinttext = hint;
}
xui_method_explain(xui_textbox, get_hintdraw,		const xui_family_render&)( void ) const
{
	return m_hintdraw;
}
xui_method_explain(xui_textbox, set_hintdraw,		void					)( const xui_family_render& hintdraw )
{
	m_hintdraw = hintdraw;
}

/*
//property
*/
xui_method_explain(xui_textbox, was_password,		bool					)( void ) const
{
	return m_password;
}
xui_method_explain(xui_textbox, set_password,		void					)( bool flag )
{
	m_password = flag;
}

xui_method_explain(xui_textbox, was_numbonly,		bool					)( void ) const
{
	return m_numbonly;
}
xui_method_explain(xui_textbox, set_numbonly,		void					)( bool flag )
{
	m_numbonly = flag;
}

xui_method_explain(xui_textbox, was_readonly,		bool					)( void ) const
{
	return m_readonly;
}
xui_method_explain(xui_textbox, set_readonly,		void					)( bool flag )
{
	m_readonly = flag;
	if (m_readonly) m_cursor = CURSOR_DEFAULT;
	else			m_cursor = CURSOR_TEXT;
}

/*
//caret
*/
xui_method_explain(xui_textbox, get_caretindex,		u32						)( void ) const
{
	return m_caretcurrindex;
}
xui_method_explain(xui_textbox, set_caretindex,		void					)( u32 index, bool force )
{
	if (index > m_text.length())
		index = m_text.length();

	if (m_caretcurrindex != index || force)
	{
		m_caretcurrindex  = index;
		set_caretvisible();
		set_caretrect();
	}
}
xui_method_explain(xui_textbox, hit_caretindex,		u32						)( const xui_vector<s32>& relative ) const
{
	if (m_text.empty())
		return 0;

	xui_rect2d<s32> rt = get_rendertextrt();
	xui_vector<s32> pt = relative;
	//相对位置
	pt.x -= rt.ax;
	pt.y -= rt.ay;

	//计算字索引
	std::wstring temp(m_text.substr(m_firstshowindex, get_showcount()));
	u32 index = get_charindex(temp, pt.x);

	return m_firstshowindex + index;
}
xui_method_explain(xui_textbox, set_caretvisible,	void					)( void )
{
	//光标在可见字符前
	if (m_caretcurrindex < m_firstshowindex)
		m_firstshowindex = m_caretcurrindex;

	//光标在可见字符后，循环计算位置直到光标可见
	while (m_caretcurrindex > m_firstshowindex + get_showcount())
		++m_firstshowindex;
}

/*
//select
*/
xui_method_explain(xui_textbox, get_selectstart,	u32						)( void ) const
{
	return (m_selectstart == m_selectfinal) ? m_caretcurrindex : m_selectstart;
}
xui_method_explain(xui_textbox, get_selectfinal,	u32						)( void ) const
{
	return (m_selectstart == m_selectfinal) ? m_caretcurrindex : m_selectfinal;
}
xui_method_explain(xui_textbox, get_selectcount,	u32						)( void ) const
{
	return get_selectfinal() - get_selectstart();
}
xui_method_explain(xui_textbox, set_selecttext,		void					)( u32 start, u32 final )
{
	//确保索引不超范围
	if (start > m_text.length())
		start = m_text.length();
	if (final > m_text.length())
		final = m_text.length();

	//确保前后顺序
	m_selectstart = start <= final ? start : final;
	m_selectfinal = start <= final ? final : start;
}

/*
//update&render
*/
xui_method_explain(xui_textbox, update,				void					)( f32 delta )
{
	xui_drawer::update(delta);
	//catch
	if (has_catch())
	{
		xui_vector<s32> pt = xui_desktop::get_ins()->get_mousecurr();
		xui_rect2d<s32> rt = get_renderrtabs();
		if (pt.x < rt.ax)
		{
			if (m_caretcurrindex > 0)
			{
				set_caretindex(m_caretcurrindex-1);
				set_selecttext(m_caretdragindex, m_caretcurrindex);
			}
		}
		if (pt.x > rt.bx)
		{
			if (m_caretcurrindex < m_text.length())
			{
				set_caretindex(m_caretcurrindex+1);
				set_selecttext(m_caretdragindex, m_caretcurrindex);
			}
		}
	}

	//caret
	if (has_focus())
	{
		m_caretdrawer->update(delta);
	}
}

/*
//callback
*/
xui_method_explain(xui_textbox, on_keybddown,		void					)( xui_method_keybd& args )
{
	xui_drawer::on_keybddown(args);

	switch(args.kcode)
	{
	case KEY_SHIFT:	m_caretdragindex = m_caretcurrindex; break;
	case KEY_LARROW:do_larrow	(args.shift);	break;
	case KEY_RARROW:do_rarrow	(args.shift);	break;
	case KEY_HOME:	do_home		(args.shift);	break;
	case KEY_END:	do_end		(args.shift);	break;
	case KEY_BACK:	do_back		();				break;
	case KEY_ENTER:	do_enter	();				break;
	}

	if (args.ctrl == false)
		return;

	switch (args.kcode)
	{
	case KEY_A:		do_select	();				break;
	case KEY_C:		do_copy		();				break;
	case KEY_X:		do_cut		();				break;
	case KEY_V:		do_paste	();				break;
	}
}
xui_method_explain(xui_textbox, on_keybdchar,		void					)( xui_method_keybd& args )
{
	xui_drawer::on_keybdchar(args);

	if (args.wchar < 0x20)
		return;

	if (was_readonly())
		return;

	//删除选中字符
	del_selecttext();

	//只属数字
	if (was_numbonly() && (args.wchar < L'-' || args.wchar > L'9' || args.wchar == L'/'))
		return;

	//插入字符
	m_text.insert(m_caretcurrindex, 1, args.wchar);
	set_caretindex(m_caretcurrindex+1);

	//回调
	xui_method_args other_args;
	on_textchanged (other_args);
}
xui_method_explain(xui_textbox, on_mousedown,		void					)( xui_method_mouse& args )
{
	xui_drawer::on_mousedown(args);

	if (args.mouse == MB_L)
	{
		//取消选中
		non_selecttext();

		//设置光标
		xui_vector<s32> relative = get_renderpt(args.point);
		m_caretdragindex = hit_caretindex(relative);
		set_caretindex(m_caretdragindex, true);

		//重置
		m_caretdrawer->reset();
	}
}
xui_method_explain(xui_textbox, on_mousemove,		void					)( xui_method_mouse& args )
{
	xui_drawer::on_mousemove(args);

	if (has_catch())
	{
		xui_vector<s32> relative = get_renderpt(args.point);
		set_caretindex(hit_caretindex(relative));
		set_selecttext(m_caretcurrindex, m_caretdragindex);
	}
}
xui_method_explain(xui_textbox, on_textchanged,		void					)( xui_method_args&  args )
{
	//取消选中
	non_selecttext();

	//确保光标可见
	set_caretindex(m_caretcurrindex, true);

	//事件
	xui_drawer::on_textchanged(args);
}
xui_method_explain(xui_textbox, on_setrenderpt,		void					)( xui_method_args&  args )
{
	xui_drawer::on_setrenderpt(args);
	set_caretrect();
}
xui_method_explain(xui_textbox, on_setrendersz,		void					)( xui_method_args&  args )
{
	xui_drawer::on_setrendersz(args);
	set_caretrect();
}
xui_method_explain(xui_textbox, on_setborderrt,		void					)( xui_method_args&  args )
{
	xui_drawer::on_setborderrt(args);
	set_caretrect();
}
xui_method_explain(xui_textbox, on_renderself,		void					)( xui_method_args&  args )
{
	xui_colour color = get_vertexcolor();

	//绘制选中背景
	if (get_selectcount())
	{						
		u32 start = xui_max(get_selectstart(), m_firstshowindex);
		u32 final = xui_min(get_selectfinal(), m_firstshowindex + get_showcount());			
		if (final > start)
		{
			u32 x = get_textwidth(m_text.substr(m_firstshowindex, start-m_firstshowindex));
			u32 w = get_textwidth(m_text.substr(start,			  final-start));

			xui_rect2d<s32> rt = get_rendertextrt() + get_screenpt();
			rt.oft_x(x);
			rt.set_w(w);
			xui_convas::get_ins()->fill_rectangle(rt, color*default_selectedcolor);
		}
	}

	xui_drawer::on_renderself(args);

	//绘制提示
	if (m_text.empty() && m_hinttext.length() > 0)
	{
		xui_family_render textdraw = m_hintdraw;
		textdraw.normalcolor *= color;
		textdraw.strokecolor *= color;
		xui_convas::get_ins()->draw_text(
			m_hinttext, 
			m_textfont, 
			get_rendertextrt() + get_screenpt(), 
			textdraw);
	}

	if (has_focus() && m_readonly == false)
	{
		m_caretdrawer->render();
	}
}

/*
//virtual
*/
xui_method_explain(xui_textbox, get_rendertext,		std::wstring			)( void ) const
{
	std::wstring temp = m_text.substr(m_firstshowindex, get_showcount());
	if (m_password)
		std::fill(temp.begin(), temp.end(), L'*');

	return temp;
}
xui_method_explain(xui_textbox, get_rendericonpt,	xui_vector<s32>			)( void ) const
{
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt = rt.get_pt();
	pt.x = rt.ax;
	pt.y = rt.ay + (rt.get_h() - m_iconsize.h) / 2;

	return pt + m_iconoffset;
}
xui_method_explain(xui_textbox, get_rendertextrt,	xui_rect2d<s32>			)( void ) const
{
	std::wstring    text = get_rendertext  ();
	xui_rect2d<s32> rt   = get_renderrtins ();
	xui_vector<s32> pt   = get_rendericonpt();
	rt.ax  = pt.x + m_iconsize.w;
	rt.ax += m_textoffset.x;
	rt.ay += m_textoffset.y;

	if (text.length() == 0)
		text = m_hinttext;
	if (text.length() >  0)
	{
		rt = xui_convas::get_ins()->calc_draw(
			text, 
			m_textfont, 
			rt, 
			m_textalign, 
			true);
	}

	return rt;
}

/*
//keyboard
*/
xui_method_explain(xui_textbox, do_larrow,			void					)( bool shift )
{
	if (m_caretcurrindex > 0)
		set_caretindex(m_caretcurrindex-1);

	//选中改变
	if (shift)	set_selecttext(m_caretdragindex, m_caretcurrindex);
	else		non_selecttext();
}
xui_method_explain(xui_textbox, do_rarrow,			void					)( bool shift )
{
	if (m_caretcurrindex < m_text.length())
		set_caretindex(m_caretcurrindex+1);

	//选中改变
	if (shift)	set_selecttext(m_caretdragindex, m_caretcurrindex);
	else		non_selecttext();
}
xui_method_explain(xui_textbox, do_home,			void					)( bool shift )
{
	if (m_caretcurrindex > 0)
		set_caretindex(0);

	//选中操作
	if (shift)	set_selecttext(m_caretdragindex, m_caretcurrindex);
	else		non_selecttext();
}
xui_method_explain(xui_textbox, do_end,				void					)( bool shift )
{
	if (m_caretcurrindex < m_text.length())
		set_caretindex(m_text.length());

	//选中操作
	if (shift)	set_selecttext(m_caretdragindex, m_caretcurrindex);
	else		non_selecttext();
}
xui_method_explain(xui_textbox, do_enter,			void					)( void )
{
	xui_method_args    args;
	xm_textenter(this, args);
}
xui_method_explain(xui_textbox, do_back,			void					)( void )
{
	if (was_readonly())
		return;

	if (get_selectcount())
		del_selecttext();
	else
	{
		switch (m_textalign)
		{
		case TEXTALIGN_RT:
		case TEXTALIGN_RC:
		case TEXTALIGN_RB:
			if (m_firstshowindex > 0)
				--m_firstshowindex;
			break;
		}

		if (m_caretcurrindex > 0)
		{
			m_text.erase(m_caretcurrindex-1, 1);
			set_caretindex(m_caretcurrindex-1);

			//回调
			xui_method_args args;
			on_textchanged( args);
		}
	}
}
xui_method_explain(xui_textbox, do_delete,			void					)( void )
{
	if (was_readonly())
		return;

	if (get_selectcount())
		del_selecttext();
	else
	{
		if (m_caretcurrindex < m_text.length())
		{
			m_text.erase(m_caretcurrindex, 1);

			xui_method_args args;
			on_textchanged( args);
		}
	}
}
xui_method_explain(xui_textbox, do_select,			void					)( void )
{
	set_selecttext(0, m_text.length());
}
xui_method_explain(xui_textbox, do_copy,			void					)( void )
{
	if (m_password)
		return;

	std::wstring temp = get_selecttext();
	xui_desktop::get_ins()->set_pastetext(temp);
}
xui_method_explain(xui_textbox, do_cut,				void					)( void )
{
	if (m_readonly || m_password)
		return;

	do_copy();
	del_selecttext();
}
xui_method_explain(xui_textbox, do_paste,			void					)( void )
{
	if (m_readonly)
		return;

	std::wstring temp = xui_desktop::get_ins()->get_pastetext();
	xui_method_keybd args;
	for (u32 i = 0; i < temp.length(); ++i)
	{
		args.wchar = temp[i];
		on_keybdchar(      args);
		xm_keybdchar(this, args);
	}
}

/*
//selection
*/
xui_method_explain(xui_textbox, del_selecttext,		void					)( void )
{
	if (get_selectcount())
	{
		m_text.erase(get_selectstart(), get_selectcount());

		xui_method_args args;
		on_textchanged( args);
	}
}
xui_method_explain(xui_textbox, non_selecttext,		void					)( void )
{
	if (get_selectcount())
		set_selecttext(0, 0);
}
xui_method_explain(xui_textbox, get_selecttext,		std::wstring			)( void ) const
{
	if (get_selectcount())
		return m_text.substr(get_selectstart(), get_selectcount());

	return L"";
}

/*
//method
*/
xui_method_explain(xui_textbox, set_caretrect,		void					)( void )
{
	xui_rect2d<s32> rt = get_rendertextrt() + get_screenpt();
	if (get_rendertext().length() > 0)
	{
		rt.oft_x(get_textwidth(m_text.substr(m_firstshowindex, m_caretcurrindex-m_firstshowindex)));
	}
	else
	{
		switch (m_textalign)
		{
		case TEXTALIGN_CT:
		case TEXTALIGN_CC:
		case TEXTALIGN_CB:
			rt.ax += rt.get_w()/2-1;
			break;
		case TEXTALIGN_RT:
		case TEXTALIGN_RC:
		case TEXTALIGN_RB:
			rt.ax  = rt.bx-2;
			break;
		}
	}

	rt.set_w(2);
	m_caretdrawer->set_caretrect(rt);
}
xui_method_explain(xui_textbox, get_textwidth,		u32						)( const std::wstring& text ) const
{
	std::wstring temp = text;
	if (m_password)
		std::fill(temp.begin(), temp.end(), L'*');

	return xui_convas::get_ins()->calc_size(temp, m_textfont, 0, true).w;
}
xui_method_explain(xui_textbox, get_charindex,		u32						)( const std::wstring& text, s32 x ) const
{
	std::wstring temp = text;
	if (m_password)
		std::fill(temp.begin(), temp.end(), L'*');

	return xui_convas::get_ins()->calc_char(temp, m_textfont, x);
}
xui_method_explain(xui_textbox, get_showcount,		u32						)( void ) const
{
	std::wstring temp = m_text.substr(m_firstshowindex, m_text.length()-m_firstshowindex);
	if (m_password)
		std::fill(temp.begin(), temp.end(), L'*');

	s32 maxwidth = get_renderrtins().get_w();
	maxwidth -= (m_icon == NULL) ? 0 : m_icon->get_size().w;
	maxwidth -= m_iconoffset.x;
	maxwidth -= m_textoffset.x;

	return xui_convas::get_ins()->calc_text(temp, m_textfont, maxwidth).length();
}