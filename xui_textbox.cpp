#include "xui_bitmap.h"
#include "xui_canvas.h"
#include "xui_family_create.h"
#include "xui_desktop.h"
#include "xui_global.h"
#include "xui_textbox.h"

xui_implement_rtti(xui_textbox, xui_drawer)

const xui_colour xui_textbox::k_default_selectedcolor = xui_colour(1.0f, 0.6f);

xui_textbox* xui_textbox::create( s32 width )
{
	xui_textbox* textbox = new xui_textbox(xui_vector<s32>(width, 24));
	textbox->set_backcolor(xui_colour(1.0f, 0.20f));
	textbox->set_drawcolor(true);
	textbox->set_sidestyle(k_sidestyle_s);
	textbox->set_borderrt(xui_rect2d<s32>(4));
	textbox->set_textalign(k_textalign_rc);
	textbox->set_hintdraw(xui_family_render(xui_colour::k_gray));

	return textbox;
}

xui_textbox::xui_textbox( const xui_vector<s32>& size )
: xui_drawer(size)
{
	m_cursor		 = k_cursor_text;
	m_password		 = false;
	m_readonly		 = false;
	m_numbtype		 = k_nt_none;
	m_caretdrawer	 = new xui_caretdrawer;
	m_selectstart	 = 0;
	m_selectfinal	 = 0;
	m_caretcurrindex = 0;
	m_caretdragindex = 0;
	m_firstshowindex = 0;
}

xui_textbox::~xui_textbox( void )
{
	delete m_caretdrawer;
}

void xui_textbox::ini_textbox( const std::wstring& text )
{
	m_text = text;

	if (m_selectstart > m_text.length())
		m_selectstart = 0;
	if (m_selectfinal > m_text.length())
		m_selectfinal = 0;

	u32 index = m_caretcurrindex;
	if (index > m_text.length())
		index = m_text.length();

	m_caretcurrindex = index;
	m_caretdragindex = index;
	set_caretvisible();
	set_caretrect();
}

const std::wstring& xui_textbox::get_hinttext( void ) const
{
	return m_hinttext;
}

void xui_textbox::set_hinttext( const std::wstring& hint )
{
	m_hinttext = hint;
}

const xui_family_render& xui_textbox::get_hintdraw( void ) const
{
	return m_hintdraw;
}

void xui_textbox::set_hintdraw( const xui_family_render& hintdraw )
{
	m_hintdraw = hintdraw;
}

bool xui_textbox::was_password( void ) const
{
	return m_password;
}

void xui_textbox::set_password( bool flag )
{
	m_password = flag;
}

u08 xui_textbox::get_numbtype( void ) const
{
	return m_numbtype;
}

void xui_textbox::set_numbtype( u08  type )
{
	m_numbtype = type;
}

bool xui_textbox::was_readonly( void ) const
{
	return m_readonly;
}

void xui_textbox::set_readonly( bool flag )
{
	m_readonly = flag;
	if (m_readonly) m_cursor = k_cursor_default;
	else			m_cursor = k_cursor_text;
}

u32 xui_textbox::get_caretindex( void ) const
{
	return m_caretcurrindex;
}

void xui_textbox::set_caretindex( u32 index, bool force )
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

u32 xui_textbox::hit_caretindex( const xui_vector<s32>& relative ) const
{
	if (m_text.empty())
		return 0;

	xui_rect2d<s32> rt = get_rendertextrt();
	xui_vector<s32> pt = relative;
	pt.x -= rt.ax;
	pt.y -= rt.ay;

	std::wstring temp(m_text.substr(m_firstshowindex, get_showcount()));
	u32 index = get_charindex(temp, pt.x);

	return m_firstshowindex + index;
}

void xui_textbox::set_caretvisible( void )
{
	if (m_caretcurrindex < m_firstshowindex)
		m_firstshowindex = m_caretcurrindex;

	while (m_caretcurrindex > m_firstshowindex + get_showcount())
		++m_firstshowindex;
}

u32 xui_textbox::get_selectstart( void ) const
{
	return (m_selectstart == m_selectfinal) ? m_caretcurrindex : m_selectstart;
}

u32 xui_textbox::get_selectfinal( void ) const
{
	return (m_selectstart == m_selectfinal) ? m_caretcurrindex : m_selectfinal;
}

u32 xui_textbox::get_selectcount( void ) const
{
	return get_selectfinal() - get_selectstart();
}

void xui_textbox::set_selecttext( u32 start, u32 final )
{
	if (start > m_text.length())
		start = m_text.length();
	if (final > m_text.length())
		final = m_text.length();

	m_selectstart = start <= final ? start : final;
	m_selectfinal = start <= final ? final : start;
}

void xui_textbox::update( f32 delta )
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

void xui_textbox::on_keybddown( xui_method_keybd& args )
{
	xui_drawer::on_keybddown(args);

	switch(args.kcode)
	{
	//case KEY_SHIFT:	m_caretdragindex = m_caretcurrindex; break;
	case k_key_larrow:  do_larrow	(args.shift);	break;
	case k_key_rarrow:  do_rarrow	(args.shift);	break;
	case k_key_home:    do_home		(args.shift);	break;
	case k_key_end:	    do_end		(args.shift);	break;
	case k_key_back:	do_back		();				break;
	case k_key_enter:	do_enter	();				break;
	case k_key_delete:  do_delete	();				break;
	}

	if (args.ctrl == false)
		return;

	switch (args.kcode)
	{
	case k_key_a:		do_select	();				break;
	case k_key_c:		do_copy		();				break;
	case k_key_x:		do_cut		();				break;
	case k_key_v:		do_paste	();				break;
	}
}

void xui_textbox::on_keybdchar( xui_method_keybd& args )
{
	xui_drawer::on_keybdchar(args);

	if (args.wchar < 0x20)
		return;

	if (was_readonly())
		return;

	del_selecttext();

	if (m_numbtype != k_nt_none)
	{
		switch (m_numbtype)
		{
		case k_nt_int:
			if (args.wchar < L'-' || args.wchar > L'9' || args.wchar == L'/' || args.wchar == L'.')
				return;
			break;
		case k_nt_unsignedint:
			if (args.wchar < L'0' || args.wchar > L'9')
				return;
			break;
		case k_nt_float:
			if (args.wchar < L'-' || args.wchar > L'9' || args.wchar == L'/')
				return;
			break;
		}
	}

	m_text.insert(m_caretcurrindex, 1, args.wchar);
	set_caretindex(m_caretcurrindex+1);

	xui_method_args other_args;
	on_textchanged (other_args);
}

void xui_textbox::on_mousedown( xui_method_mouse& args )
{
	xui_drawer::on_mousedown(args);

	if (args.mouse == k_mb_left)
	{
		non_selecttext();

		xui_vector<s32> relative = get_renderpt(args.point);
		m_caretdragindex = hit_caretindex(relative);
		set_caretindex(m_caretdragindex, true);

		m_caretdrawer->reset();
	}
}

void xui_textbox::on_mousemove( xui_method_mouse& args )
{
	xui_drawer::on_mousemove(args);

	if (has_catch())
	{
		xui_vector<s32> relative = get_renderpt(args.point);
		set_caretindex(hit_caretindex(relative));
		set_selecttext(m_caretcurrindex, m_caretdragindex);
	}
}

void xui_textbox::on_mousedoubleclick( xui_method_mouse& args )
{
	xui_drawer::on_mousedoubleclick(args);
	set_selecttext(0, m_text.length());
}

void xui_textbox::on_nonfocus( xui_method_args& args )
{
	xui_drawer::on_nonfocus(args);
	non_selecttext();
}

void xui_textbox::on_textchanged( xui_method_args& args )
{
	non_selecttext();
	set_caretindex(m_caretcurrindex, true);

	xui_drawer::on_textchanged(args);
}

void xui_textbox::on_setrenderpt( xui_method_args& args )
{
	xui_drawer::on_setrenderpt(args);
	set_caretrect();
}

void xui_textbox::on_setrendersz( xui_method_args& args )
{
	xui_drawer::on_setrendersz(args);
	set_caretrect();
}

void xui_textbox::on_setborderrt( xui_method_args& args )
{
	xui_drawer::on_setborderrt(args);
	set_caretrect();
}

void xui_textbox::on_renderself( xui_method_args&  args )
{
	xui_colour color = get_vertexcolor();

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
			xui_canvas::get_ins()->fill_rectangle(rt, color*k_default_selectedcolor);
		}
	}

	xui_drawer::on_renderself(args);

	if (m_text.empty() && m_hinttext.length() > 0)
	{
		xui_family_render textdraw = m_hintdraw;
		textdraw.normalcolor *= color;
		textdraw.strokecolor *= color;
		xui_canvas::get_ins()->draw_text(
			m_hinttext, 
			m_textfont, 
			get_rendertextrt() + get_screenpt(), 
			textdraw,
			m_singleline);
	}

	if (has_focus() && m_readonly == false)
	{
		m_caretdrawer->render();
	}
}

std::wstring xui_textbox::get_rendertext( void ) const
{
	std::wstring temp = m_text.substr(m_firstshowindex, get_showcount());
	if (m_password)
		std::fill(temp.begin(), temp.end(), L'*');

	return temp;
}

xui_vector<s32> xui_textbox::get_rendericonpt( void ) const
{
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt = rt.get_pt();
	pt.x = rt.ax;
	pt.y = rt.ay + (rt.get_h() - m_iconsize.h) / 2;

	return pt + m_iconoffset;
}

xui_rect2d<s32> xui_textbox::get_rendertextrt( void ) const
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
		rt = xui_canvas::get_ins()->calc_draw(
			text, 
			m_textfont, 
			rt, 
			m_textalign, 
			true);
	}

	return rt;
}

void xui_textbox::do_larrow( bool shift )
{
	if (m_caretcurrindex > 0)
		set_caretindex(m_caretcurrindex-1);

	if (shift)	set_selecttext(m_caretdragindex, m_caretcurrindex);
	else		non_selecttext();
}

void xui_textbox::do_rarrow( bool shift )
{
	if (m_caretcurrindex < m_text.length())
		set_caretindex(m_caretcurrindex+1);

	if (shift)	set_selecttext(m_caretdragindex, m_caretcurrindex);
	else		non_selecttext();
}

void xui_textbox::do_home( bool shift )
{
	if (m_caretcurrindex > 0)
		set_caretindex(0);

	if (shift)	set_selecttext(m_caretdragindex, m_caretcurrindex);
	else		non_selecttext();
}

void xui_textbox::do_end( bool shift )
{
	if (m_caretcurrindex < m_text.length())
		set_caretindex(m_text.length());

	if (shift)	set_selecttext(m_caretdragindex, m_caretcurrindex);
	else		non_selecttext();
}

void xui_textbox::do_enter( void )
{
	xui_method_args    args;
	xm_textenter(this, args);
}

void xui_textbox::do_back( void )
{
	if (was_readonly())
		return;

	if (get_selectcount())
		del_selecttext();
	else
	{
		switch (m_textalign)
		{
		case k_textalign_rt:
		case k_textalign_rc:
		case k_textalign_rb:
			if (m_firstshowindex > 0)
				--m_firstshowindex;
			break;
		}

		if (m_caretcurrindex > 0)
		{
			m_text.erase(m_caretcurrindex-1, 1);
			set_caretindex(m_caretcurrindex-1);

			xui_method_args args;
			on_textchanged( args);
		}
	}
}

void xui_textbox::do_delete( void )
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

void xui_textbox::do_select( void )
{
	set_selecttext(0, m_text.length());
}

void xui_textbox::do_copy( void )
{
	if (m_password)
		return;

	std::wstring temp = get_selecttext();
	xui_global::cpy_string(temp);
}

void xui_textbox::do_cut( void )
{
	if (m_readonly || m_password)
		return;

	do_copy();
	del_selecttext();
}

void xui_textbox::do_paste( void )
{
	if (m_readonly)
		return;

	std::wstring temp = xui_global::pst_string();
	xui_method_keybd args;
	for (u32 i = 0; i < temp.length(); ++i)
	{
		args.wchar = temp[i];
		on_keybdchar(      args);
		xm_keybdchar(this, args);
	}
}

void xui_textbox::del_selecttext( void )
{
	if (get_selectcount() > 0)
	{
		m_caretcurrindex = get_selectstart();
		m_text.erase(get_selectstart(), get_selectcount());

		xui_method_args args;
		on_textchanged( args);
	}
}

void xui_textbox::non_selecttext( void )
{
	m_caretdragindex = m_caretcurrindex;

	if (get_selectcount())
		set_selecttext(0, 0);
}

std::wstring xui_textbox::get_selecttext( void ) const
{
	if (get_selectcount())
		return m_text.substr(get_selectstart(), get_selectcount());

	return L"";
}

void xui_textbox::set_caretrect( void )
{
	xui_rect2d<s32> textrt = get_rendertextrt();
	xui_rect2d<s32> rt     = get_renderrtins ();
	rt.ax = textrt.ax;
	rt.bx = textrt.bx;

	if (get_rendertext().length() > 0)
	{
		rt.oft_x(get_textwidth(m_text.substr(m_firstshowindex, m_caretcurrindex-m_firstshowindex)));
	}
	else
	{
		switch (m_textalign)
		{
		case k_textalign_ct:
		case k_textalign_cc:
		case k_textalign_cb:
			rt.ax += rt.get_w()/2-1;
			break;
		case k_textalign_rt:
		case k_textalign_rc:
		case k_textalign_rb:
			rt.ax  = rt.bx-2;
			break;
		}

	}

	s32 height = xui_canvas::get_ins()->get_family_create()->get_height(m_textfont);
	switch (m_textalign)
	{
	case k_textalign_lc:
	case k_textalign_cc:
	case k_textalign_rc:
		rt.ay += (rt.get_h() - height) / 2;
		break;
	}

	rt.set_w(2);
	rt.set_h(height);

	m_caretdrawer->set_caretrect(rt+get_screenpt());
}

u32 xui_textbox::get_textwidth( const std::wstring& text ) const
{
	std::wstring temp = text;
	if (m_password)
		std::fill(temp.begin(), temp.end(), L'*');

	return xui_canvas::get_ins()->calc_size(temp, m_textfont, 0, true).w;
}

u32 xui_textbox::get_charindex( const std::wstring& text, s32 x ) const
{
	std::wstring temp = text;
	if (m_password)
		std::fill(temp.begin(), temp.end(), L'*');

	return xui_canvas::get_ins()->calc_char(temp, m_textfont, x);
}

u32 xui_textbox::get_showcount( void ) const
{
	std::wstring temp = m_text.substr(m_firstshowindex, m_text.length()-m_firstshowindex);
	if (m_password)
		std::fill(temp.begin(), temp.end(), L'*');

	s32 maxwidth = get_renderrtins().get_w();
	maxwidth -= (m_icon == NULL) ? 0 : m_icon->get_size().w;
	maxwidth -= m_iconoffset.x;
	maxwidth -= m_textoffset.x;

	return xui_canvas::get_ins()->calc_text(temp, m_textfont, maxwidth, m_singleline).length();
}