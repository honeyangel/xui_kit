#include "xui_bitmap.h"
#include "xui_family_bitmap.h"
#include "xui_family_member.h"
#include "xui_family_create.h"
#include "xui_native_device.h"
#include "xui_canvas.h"

xui_implement_instance_member(xui_canvas)
xui_implement_instance_method(xui_canvas)

xui_canvas::xui_canvas( void )
{
	m_viewport		= xui_rect2d<s32>(0);
	m_cliprect		= xui_rect2d<s32>(0);
	m_familycreate	= new xui_family_create;
}

xui_canvas::~xui_canvas( void )
{
	delete m_familycreate;
	for (u32 i = 0; i < m_familybitmapvec.size(); ++i)
		delete m_familybitmapvec[i];
}

void xui_canvas::begin( void )
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_POLYGON_SMOOTH);
	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void xui_canvas::clear( const xui_colour& color )
{
	set_cliprect(m_viewport);
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void xui_canvas::present( void )
{}

void xui_canvas::set_transform( f32 angle, const xui_vector<f32>& scale, const xui_vector<f32>& translate )
{
	glMatrixMode(GL_MODELVIEW);

	f32 fs = sin(angle);
	f32 fc = cos(angle);

	f32 m[16];
	memset(m, 0, sizeof(f32) * 16);
	m[0]  =  fc;
	m[1]  =  fs;
	m[4]  = -fs;
	m[5]  =  fc;
	m[10] =  1.0f;
	m[12] =  translate.x;
	m[13] =  translate.y;
	m[14] =  0.0f;
	m[15] =  1.0f;
	glLoadMatrixf(m);
}

void xui_canvas::non_transform( void )
{
	glMatrixMode(GL_MODELVIEW);
	f32 m[16];
	memset(m, 0, sizeof(f32) * 16);
	m[0]  =  1.0f;
	m[5]  =  1.0f;
	m[10] =  1.0f;
	m[15] =  1.0f;
	glLoadMatrixf(m);
}

const xui_rect2d<s32>& xui_canvas::get_cliprect( void ) const
{
	return m_cliprect;
}

void xui_canvas::set_cliprect( const xui_rect2d<s32>&	rt )
{
    xui_vector<f32> scale = xui_native_device::get_window_scale();

	if (m_cliprect != rt)
	{
		m_cliprect  = rt;
		if (m_cliprect.get_w() > 0 &&
			m_cliprect.get_h() > 0)
		{
			glEnable (GL_SCISSOR_TEST);
			glScissor(
				(s32)(m_cliprect.ax * scale.x), 
				(s32)(m_viewport.get_h() * scale.y - m_cliprect.by * scale.y),
				(s32)(m_cliprect.get_w() * scale.x),
				(s32)(m_cliprect.get_h() * scale.y));
		}
		else
		{
			glDisable(GL_SCISSOR_TEST);
		}
	}
}

const xui_rect2d<s32>& xui_canvas::get_viewport( void ) const
{
	return m_viewport;
}

void xui_canvas::set_viewport( const xui_rect2d<s32>& rt )
{
	if (m_viewport != rt)
	{
		m_viewport  = rt;

		int w = m_viewport.get_w();
		int h = m_viewport.get_h();
		glMatrixMode(GL_PROJECTION);

		f32 m[16];
		memset(m, 0, sizeof(f32) * 16);
		m[0]  =  2.0f / (f32)w;
		m[5]  = -2.0f / (f32)h;
		m[10] =  1.0f / 1000.0f;
		m[12] = -1.0f;
		m[13] =  1.0f;
		m[14] =  1.0f / 1000.0f;
		m[15] =  1.0f;
		glLoadMatrixf(m);

        xui_vector<f32> scale = xui_native_device::get_window_scale();
		glViewport(
			(s32)(m_viewport.ax * scale.x),
			(s32)(m_viewport.ay * scale.y),
			(s32)(m_viewport.get_w() * scale.x),
			(s32)(m_viewport.get_h() * scale.y));
	}
}

void xui_canvas::draw_image( 
    xui_bitmap*				image,
	const xui_vector<s32>&	pt,
	const xui_colour&		color )
{
	draw_image(
		image,
		xui_rect2d<s32>(pt, image->get_size()),
		color);
}

void xui_canvas::draw_image( 
    xui_bitmap*				image,
	const xui_rect2d<s32>&	dst,
	const xui_colour&		color )
{
	draw_image(
		image,
		xui_rect2d<s32>(xui_vector<s32>(0), image->get_size()),
		dst,
		color);
}

void xui_canvas::draw_image( 
    xui_bitmap*				image,
	const xui_rect2d<s32>&	src,
	const xui_rect2d<s32>&	dst,
	const xui_colour&		color )
{
	if (color.a == 0.0f)
		return;

	image->set_bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,	  GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,	  GL_CLAMP);

	glDisable(GL_POLYGON_SMOOTH);
	glUseProgram(0);
	glBegin(GL_QUADS);
	glColor4fv(color.value);

	f32 invw = 1.0f / (f32)image->get_size().w;
	f32 invh = 1.0f / (f32)image->get_size().h;

	glTexCoord2f((f32)src.ax*invw, (f32)src.ay*invh); glVertex2f((f32)dst.ax, (f32)dst.ay);	
	glTexCoord2f((f32)src.ax*invw, (f32)src.by*invh); glVertex2f((f32)dst.ax, (f32)dst.by); 
	glTexCoord2f((f32)src.bx*invw, (f32)src.by*invh); glVertex2f((f32)dst.bx, (f32)dst.by); 
	glTexCoord2f((f32)src.bx*invw, (f32)src.ay*invh); glVertex2f((f32)dst.bx, (f32)dst.ay); 

	glEnd();

	image->non_bind();
}

std::wstring xui_canvas::trim_text( 
    const std::wstring&		text, 
	const xui_family&		textfont, 
	s32                     maxwidth )
{
	s32 w = calc_size(text, textfont, maxwidth, true).w;
	if (w > maxwidth)
	{
		std::wstring dot = L"...";
		s32 dotwidth = calc_size(dot, textfont, maxwidth, true).w;

		std::wstring result = calc_text(text, textfont, maxwidth-dotwidth, true);
		result += dot;
		return result;
	}
	else
	{
		return text;
	}
}

xui_vector<s32> xui_canvas::calc_size( 
    const std::wstring&		text,
	const xui_family&		textfont,
	s32						maxwidth,
	bool					singleline )
{
	s32 height = m_familycreate->get_height(textfont);

	xui_vector<s32> size(0);
	if (singleline)
	{
		for (u32 i = 0; i < text.length(); ++i)
		{
			//非可见字符
			if (text[i] < 0x20)
				continue;

			xui_family_member* member = get_family_member(textfont, text[i]);
			size.w += member->advance;
			size.w += textfont.horz;
		}

		size.h = height;
	}
	else
	{
		//绘制多行
		s32 textline = 0;
		s32 curwidth = 0;

		//single line buffer
		std::wstring buffer;

		wchar_t*p = (wchar_t*)text.c_str();
		while (*p)
		{
			if (*p == L'\n')
			{
				buffer.clear();

				curwidth = 0;
				++textline;
				++p;
			}
			else
			{
				std::wstring word;
				s32 sw  = calc_word(std::wstring(p), textfont, maxwidth, singleline, word);
				if (sw == 0 && word.empty())
					break;

				if (sw+curwidth > maxwidth)
				{
					//宽度太小
					if (curwidth == 0)
						break;

					buffer.clear();

					curwidth = 0;
					++textline;
				}
				else
				{
					curwidth += sw;
					buffer	 += word;
					p		 += word.length();
				}
			}
		}

		if (buffer.length() > 0)
			++textline;

		size.w = textline > 1 ? maxwidth : curwidth;
		size.h = height * textline + ((textline > 1) ? (textline-1)*textfont.vert : 0);
	}

	return size;
}

xui_rect2d<s32> xui_canvas::calc_draw( 
    const std::wstring&		text,
	const xui_family&		textfont,
	const xui_rect2d<s32>&	calcrect,
	u08						textalign,
	bool					singleline )
{
	xui_rect2d<s32> drawrect = calcrect;
	xui_vector<s32> size     = calc_size(text, textfont, calcrect.get_w(), singleline);
	drawrect.set_sz(size);

	//水平方向对齐
	switch (textalign)
	{
	case k_textalign_ct:	//CT
	case k_textalign_cc:	//CC
	case k_textalign_cb:	//CB
		drawrect.oft_x((calcrect.get_sz().w - size.w) / 2);
		break;
	case k_textalign_rt:	//RT
	case k_textalign_rc:	//RC
	case k_textalign_rb:	//RB
		drawrect.oft_x((calcrect.get_sz().w - size.w));
		break;
	}

	//竖直方向对齐
	switch (textalign)
	{
	case k_textalign_lc:	//LC
	case k_textalign_cc:	//CC
	case k_textalign_rc:	//RC
		drawrect.oft_y((calcrect.get_sz().h - size.h) / 2);
		break;
	case k_textalign_lb:	//LB
	case k_textalign_cb:	//CB
	case k_textalign_rb:	//RB
		drawrect.oft_y((calcrect.get_sz().h - size.h));
		break;
	}

	return drawrect;
}

std::wstring xui_canvas::calc_text( 
    const std::wstring&		text,
	const xui_family&		textfont,
	s32						maxwidth,
	bool					singleline )
{
	//绘制多行
	std::wstring temp;

	s32 curwidth = 0;
	wchar_t*p = (wchar_t*)text.c_str();
	while (*p)
	{
		if (*p == L'\n')
		{
			temp += *p;
			break;
		}
		else
		{
			std::wstring word;
			s32 sw =  calc_word(std::wstring(p), textfont, maxwidth, singleline, word);
			if (sw == 0 && word.empty())
				break;

			if (sw+curwidth > maxwidth)
			{
				break;
			}
			else
			{
				curwidth += sw;
				temp     += word;
				p	     += word.length();
			}
		}
	}

	return temp;
}

u32 xui_canvas::calc_char( 
    const std::wstring&		text,
    const xui_family&		textfont,
    s32						position )
{
	if (position < 0)	
		return 0;

	s32 currx  = 0;
	u32 index  = 0;
	for (u32 i = 0; i < text.length(); ++i)
	{			
		if (text[i] == L'\n')
			continue;

		xui_family_member* member = get_family_member(textfont, text[i]);
		if (position < currx + member->advance/2)
			break;

		currx += member->advance;
		currx += textfont.horz;
		++index;
	}

	return index;
}

u32 xui_canvas::calc_word( 
    const std::wstring&		text,
	const xui_family&		textfont,
	s32						maxwidth,
	bool					singleline,
	std::wstring&			word )
{
	word.clear();

	s32 curwidth = 0;
	for (u32 i = 0; i < text.length(); ++i)
	{
		//非可见字符
		if (text[i] < 0x20)
		{
			if (word.length())
				break;
				
			word += text[i];
			continue;
		}

		xui_family_member* member = get_family_member(textfont, text[i]);

		//单词字符
		if (singleline == false && ((text[i] >= L'a' && text[i] <= L'z') || (text[i] >= L'A' && text[i] <= L'Z')))
		{
			if (curwidth + member->advance + textfont.horz > maxwidth)
				break;

			curwidth += member->advance;
			curwidth += textfont.horz;
			word     += text[i];
		}
		//空格中文
		else
		{
			if (word.length())
				break;

			if (curwidth + member->advance + textfont.horz > maxwidth)
				break;

			curwidth += member->advance;
			curwidth += textfont.horz;
			word     += text[i];
			break;
		}
	}

	return curwidth;
}

void xui_canvas::draw_text( 
    const std::wstring&		text,
	const xui_family&		textfont,
	const xui_rect2d<s32>&	drawrect,
	const xui_family_render&textdraw, 
	bool					singleline )
{
	s32 height = m_familycreate->get_height(textfont);

	xui_vector<s32> pt = drawrect.get_pt();

	//绘制多行
	s32 curwidth = 0;
	s32 maxwidth = drawrect.get_w();
	std::wstring buffer;

	wchar_t*p = (wchar_t*)text.c_str();
	while (*p)
	{
		if(*p == L'\n')
		{
			draw_text(buffer, textfont, pt, textdraw);
			pt.y += height;
			pt.y += textfont.vert;
			buffer.clear();

			curwidth = 0;
			++p;
		}
		else
		{
			std::wstring word;
			s32 sw =  calc_word(std::wstring(p), textfont, maxwidth, singleline, word);
			if (sw == 0 && word.empty())
				break;

			if (sw+curwidth > maxwidth)
			{
				//宽度太小
				if (curwidth == 0)
					break;

				draw_text(buffer, textfont, pt, textdraw);
				pt.y += height;
				pt.y += textfont.vert;
				buffer.clear();

				curwidth  = 0;
			}
			else
			{
				curwidth += sw;
				buffer	 += word;
				p		 += word.length();
			}
		}
	}

	if (!buffer.empty())
		draw_text(buffer, textfont, pt, textdraw);
}

void xui_canvas::draw_text( 
    const std::wstring&		text,
	const xui_family&		textfont,
	const xui_vector<s32>&	screenpt,
	const xui_family_render&textdraw )
{
	s32 ascender = m_familycreate->get_ascender(textfont);

	xui_rect2d<s32> dst;
	dst.set_pt(screenpt);

	for (u32 i = 0; i < text.length(); ++i)
	{
		//非可见字符
		if (text[i] < 0x20)
			continue;

		xui_family_member* calc_member = get_family_member(textfont, text[i]);
        xui_vector<f32> scale = xui_native_device::get_window_scale();
        xui_family drawfont = textfont;
        drawfont.size = (s32)(drawfont.size * scale.y);
        xui_family_member* draw_member = get_family_member(drawfont, text[i]);
		s32 x =  calc_member->bearing.x;
		s32 y = -calc_member->bearing.y + ascender;

		if (textfont.bold)
		{
			dst.set_sz(calc_member->stroke.get_sz());
			draw_image(draw_member->bitmap, draw_member->stroke, dst+xui_vector<s32>(x, y), textdraw.strokecolor);
			dst.set_sz(calc_member->normal.get_sz());
			draw_image(draw_member->bitmap, draw_member->normal, dst+xui_vector<s32>(x, y), textdraw.normalcolor);
		}
		else
		{
			if (textdraw.renderstyle == k_textdraw_shadow)
			{
				xui_vector<s32> offset;
				offset = xui_vector<s32>(x+1, y  );
				draw_image(draw_member->bitmap, draw_member->normal, dst+offset, textdraw.shadowcolor);
				offset = xui_vector<s32>(x,   y+1);
				draw_image(draw_member->bitmap, draw_member->normal, dst+offset, textdraw.shadowcolor);
				offset = xui_vector<s32>(x+1, y+1);
				draw_image(draw_member->bitmap, draw_member->normal, dst+offset, textdraw.shadowcolor);
			}

			dst.set_sz(calc_member->normal.get_sz());
			draw_image(draw_member->bitmap, draw_member->normal, dst+xui_vector<s32>(x, y), textdraw.normalcolor);
		}

        dst.oft_x(calc_member->advance+textfont.horz);
	}
}

void xui_canvas::draw_line( 
    const xui_vector<s32>&	p1, 
    const xui_vector<s32>&	p2,
    const xui_colour&		color )
{
	if (color.a == 0.0f || m_cliprect.was_valid() == false)
		return;

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LINE_SMOOTH);
	glUseProgram(0);
	glBegin(GL_LINES);
	glColor4fv(color.value);

	glVertex2f((f32)p1.x+0.375f, (f32)p1.y+0.375f);
	glVertex2f((f32)p2.x+0.375f, (f32)p2.y+0.375f);

	glEnd();
}

void xui_canvas::draw_path( 
    xui_vector<s32>*		pt,
	u32						count,
	const xui_colour&		color )
{
	if (color.a == 0.0f || m_cliprect.was_valid() == false)
		return;

	glDisable(GL_TEXTURE_2D);
	glUseProgram(0);
	glEnable(GL_LINE_SMOOTH);
	glBegin(GL_LINE_LOOP);
	glColor4fv(color.value);

	for (u32 i = 0; i < count; ++i)
		glVertex2f((f32)pt[i].x+0.375f, (f32)pt[i].y+0.375f);

	glEnd();
}

void xui_canvas::fill_poly( 
    xui_vector<s32>*		pt,
	u32						count,
	const xui_colour&		color )
{
	if (color.a == 0.0f || m_cliprect.was_valid() == false)
		return;

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_POLYGON_SMOOTH);
	glUseProgram(0);
	glBegin(GL_TRIANGLES);
	glColor4fv(color.value);

	for (u32 i = 0; i < count; ++i)
		glVertex2f((f32)pt[i].x, (f32)pt[i].y);

	glEnd();
}

void xui_canvas::draw_triangle( 
    const xui_vector<s32>&	center, 
	s32						half, 
	u08						direction,
	const xui_colour&		color )
{
	xui_vector<s32> pt[3];
	switch (direction)
	{
	case k_triangle_up:
		pt[0] = xui_vector<s32>(center.x-half*2,   center.y+half-1);
		pt[1] = xui_vector<s32>(center.x+half*2-1, center.y+half-1);
		pt[2] = xui_vector<s32>(center.x,          center.y-half);
		break;
	case k_triangle_down:
		pt[0] = xui_vector<s32>(center.x-half*2,   center.y-half);
		pt[1] = xui_vector<s32>(center.x+half*2-1, center.y-half);
		pt[2] = xui_vector<s32>(center.x,          center.y+half-1);
		break;
	case k_triangle_left:
		pt[0] = xui_vector<s32>(center.x+half-1,   center.y-half*2);
		pt[1] = xui_vector<s32>(center.x+half-1,   center.y+half*2-1);
		pt[2] = xui_vector<s32>(center.x-half,     center.y);
		break;
	case k_triangle_right:
		pt[0] = xui_vector<s32>(center.x-half,     center.y-half*2);
		pt[1] = xui_vector<s32>(center.x-half,     center.y+half*2-1);
		pt[2] = xui_vector<s32>(center.x+half-1,   center.y);
		break;
	}

	draw_path(pt, 3, color);
}

void xui_canvas::fill_triangle( 
    const xui_vector<s32>&	center, 
	s32						half, 
	u08						direction,
	const xui_colour&		color )
{
	xui_vector<s32> pt[3];
	switch (direction)
	{
	case k_triangle_up:
		pt[0] = xui_vector<s32>(center.x-half*2, center.y+half);
		pt[1] = xui_vector<s32>(center.x+half*2, center.y+half);
		pt[2] = xui_vector<s32>(center.x,        center.y-half);
		break;
	case k_triangle_down:
		pt[0] = xui_vector<s32>(center.x-half*2, center.y-half);
		pt[1] = xui_vector<s32>(center.x+half*2, center.y-half);
		pt[2] = xui_vector<s32>(center.x,        center.y+half);
		break;
	case k_triangle_left:
		pt[0] = xui_vector<s32>(center.x+half,   center.y-half*2);
		pt[1] = xui_vector<s32>(center.x+half,   center.y+half*2);
		pt[2] = xui_vector<s32>(center.x-half,   center.y);
		break;
	case k_triangle_right:
		pt[0] = xui_vector<s32>(center.x-half,   center.y-half*2);
		pt[1] = xui_vector<s32>(center.x-half,   center.y+half*2);
		pt[2] = xui_vector<s32>(center.x+half,   center.y);
		break;
	}

	fill_poly(pt, 3, color);
}

void xui_canvas::draw_rectangle( 
    const xui_rect2d<s32>&	rt,
	const xui_colour&		color,
	s32						thick )
{
	draw_line(xui_vector<s32>(rt.ax, rt.ay), xui_vector<s32>(rt.bx, rt.ay),	color);
	draw_line(xui_vector<s32>(rt.bx, rt.ay), xui_vector<s32>(rt.bx, rt.by),	color);
	draw_line(xui_vector<s32>(rt.bx, rt.by), xui_vector<s32>(rt.ax, rt.by),	color);
	draw_line(xui_vector<s32>(rt.ax, rt.by), xui_vector<s32>(rt.ax, rt.ay),	color);
	if (thick > 1)
	{
		for (s32 i = 1; i < thick; ++i)
		{
			xui_rect2d<s32> temp;
			temp.ax = rt.ax+i;
			temp.ay = rt.ay+i;
			temp.bx = rt.bx-i;
			temp.by = rt.by-i;
			draw_rectangle(temp, color);
		}
	}
}

void xui_canvas::fill_rectangle( 
    const xui_rect2d<s32>&	rt,
	const xui_colour&		color,
	bool					smooth )
{
	if (color.a == 0.0f || m_cliprect.was_valid() == false)
		return;

	if (smooth) glEnable (GL_POLYGON_SMOOTH);
	else		glDisable(GL_POLYGON_SMOOTH);

	glDisable(GL_TEXTURE_2D);
	glUseProgram(0);
	glBegin(GL_QUADS);
	glColor4fv(color.value);

	glVertex2f((f32)rt.ax, (f32)rt.ay);
	glVertex2f((f32)rt.ax, (f32)rt.by);
	glVertex2f((f32)rt.bx, (f32)rt.by);
	glVertex2f((f32)rt.bx, (f32)rt.ay);

	glEnd();
}

void xui_canvas::fill_rectangle( 
    const xui_rect2d<s32>&	rt, 
	xui_colour*				colors )
{
	if (m_cliprect.was_valid() == false)
		return;

	glDisable(GL_POLYGON_SMOOTH);
	glDisable(GL_TEXTURE_2D);
	glUseProgram(0);
	glBegin(GL_QUADS);

	glColor4fv(colors[0].value);
	glVertex2f((f32)rt.ax, (f32)rt.ay);

	glColor4fv(colors[1].value);
	glVertex2f((f32)rt.ax, (f32)rt.by);

	glColor4fv(colors[2].value);
	glVertex2f((f32)rt.bx, (f32)rt.by);

	glColor4fv(colors[3].value);
	glVertex2f((f32)rt.bx, (f32)rt.ay);

	glEnd();
}

void xui_canvas::draw_round( 
    const xui_rect2d<s32>&	rt,
	const xui_colour&		color,
	s32						corner )
{
	draw_round(
		rt, 
		color, 
		xui_rect2d<s32>(corner));
}

void xui_canvas::draw_round( 
    const xui_rect2d<s32>&	rt,
	const xui_colour&		color,
	const xui_rect2d<s32>&  corner,
	s32						thick )
{
	//左上/右上/右下/左下
	s32 corner0 = corner.value[0];
	s32 corner1 = corner.value[1];
	s32 corner2 = corner.value[2];
	s32 corner3 = corner.value[3];
	if (corner0) 
		draw_arc(xui_rect2d<s32>(
			rt.ax,			 
			rt.ay,
			rt.ax+2*corner0, 
			rt.ay+2*corner0), color, 180, 90, 1);
	if (corner1) 
		draw_arc(xui_rect2d<s32>(
			rt.bx-2*corner1, 
			rt.ay,			
			rt.bx,		   
			rt.ay+2*corner1), color, 270, 90, 1);
	if (corner2) 
		draw_arc(xui_rect2d<s32>(
			rt.bx-2*corner2, 
			rt.by-2*corner2, 
			rt.bx,		   
			rt.by),			  color,   0, 90, 1);
	if (corner3) 
		draw_arc(xui_rect2d<s32>(
			rt.ax,			 
			rt.by-2*corner3, 
			rt.ax+2*corner3, 
			rt.by),			  color,  90, 90, 1);

	draw_line(xui_vector<s32>(rt.ax+corner0, rt.ay),		 xui_vector<s32>(rt.bx-corner1, rt.ay),			color);
	draw_line(xui_vector<s32>(rt.bx,		 rt.ay+corner1), xui_vector<s32>(rt.bx,		    rt.by-corner2),	color);
	draw_line(xui_vector<s32>(rt.bx-corner2, rt.by),		 xui_vector<s32>(rt.ax+corner3, rt.by),			color);
	draw_line(xui_vector<s32>(rt.ax,		 rt.by-corner3), xui_vector<s32>(rt.ax,		    rt.ay+corner0),	color);

	if (thick > 1)
	{
		for (s32 i = 1; i < thick; ++i)
		{
			xui_rect2d<s32> temp;
			temp.ax = rt.ax+i;
			temp.ay = rt.ay+i;
			temp.bx = rt.bx-i;
			temp.by = rt.by-i;
			draw_round(temp, color, corner);
		}
	}
}

void xui_canvas::fill_round( 
    const xui_rect2d<s32>&	rt,
	const xui_colour&		color,
	s32						corner )
{
	fill_round(
		rt,
		color,
		xui_rect2d<s32>(corner));
}

void xui_canvas::fill_round( 
    const xui_rect2d<s32>&	rt,
	const xui_colour&		color,
	const xui_rect2d<s32>&  corner )
{
	//左上/右上/右下/左下
	s32 corner0 = corner.value[0];
	s32 corner1 = corner.value[1];
	s32 corner2 = corner.value[2];
	s32 corner3 = corner.value[3];
	if (corner0) 
		fill_arc(xui_rect2d<s32>(
			rt.ax,
			rt.ay,
			rt.ax+2*corner0, 
			rt.ay+2*corner0), color, 180, 90, 1);
	if (corner1) 
		fill_arc(xui_rect2d<s32>(
			rt.bx-2*corner1,	
			rt.ay,
			rt.bx,
			rt.ay+2*corner1), color, 270, 90, 1);
	if (corner2) 
		fill_arc(xui_rect2d<s32>(
			rt.bx-2*corner2,	
			rt.by-2*corner2, 
			rt.bx,
			rt.by),			  color,   0, 90, 1);
	if (corner3) 
		fill_arc(xui_rect2d<s32>(
			rt.ax,
			rt.by-2*corner3, 
			rt.ax+2*corner3, 
			rt.by),			  color,  90, 90, 1);

	//上边
	fill_rectangle(xui_rect2d<s32>(
		rt.ax + corner0,	
		rt.ay,		 
		rt.bx - corner1, 
		rt.ay +(corner0|corner1)),	
		color);
	//右边
	fill_rectangle(xui_rect2d<s32>(
		rt.bx -(corner1|corner2),	
		rt.ay + corner1, 
		rt.bx,		  
		rt.by - corner2),	
		color);
	//下边
	fill_rectangle(xui_rect2d<s32>(
		rt.ax + corner3,	
		rt.by -(corner2|corner3), 
		rt.bx - corner2, 
		rt.by),			
		color);
	//左边
	fill_rectangle(xui_rect2d<s32>(
		rt.ax,			
		rt.ay + corner0, 
		rt.ax +(corner3|corner0), 
		rt.by - corner3),	
		color);
	//中间
	fill_rectangle(xui_rect2d<s32>(
		rt.ax +(corner3|corner0),	
		rt.ay +(corner0|corner1), 
		rt.bx -(corner1|corner2), 
		rt.by -(corner2|corner3)),	
		color);
}

void xui_canvas::draw_circle( 
    const xui_vector<s32>&	center,
	s32						radius,
	const xui_colour&		color,
	s32						start,
	s32						sweep )
{
	xui_rect2d<s32> rt;
	rt.ax = center.x - radius;
	rt.bx = center.x + radius;
	rt.ay = center.y - radius;
	rt.by = center.y + radius;
	draw_arc(rt, color, start, sweep, 1, true);
}

void xui_canvas::fill_circle( 
    const xui_vector<s32>&	center, 
	s32						radius,
	const xui_colour&		color, 
	s32						start, 
	s32						sweep )
{
	xui_rect2d<s32> rt;
	rt.ax = center.x - radius;
	rt.bx = center.x + radius;
	rt.ay = center.y - radius;
	rt.by = center.y + radius;
	fill_arc(rt, color, start, sweep, 1);
}

void xui_canvas::draw_arc( 
    const xui_rect2d<s32>&	rt,
	const xui_colour&		color,
	s32						start,
	s32						sweep,
	s32						precision,
bool						smooth )
{
	if (color.a == 0.0f || m_cliprect.was_valid() == false)
		return;

	if (smooth)		glEnable (GL_LINE_SMOOTH);
	else			glDisable(GL_LINE_SMOOTH);
	glDisable(GL_TEXTURE_2D);
	glUseProgram(0);
	glBegin(GL_LINE_STRIP);
	glColor4fv(color.value);

	f32 halfw = rt.get_w() / 2.0f;
	f32 halfh = rt.get_h() / 2.0f;
	f32 lastx = -1.0f;
	f32 lasty = -1.0f;
	s32 count = sweep / precision;
	for (s32 i = 0; i <= count; ++i)
	{
		s32 d = start + precision *  i;
		f32 x = cos(d*xui_pi/180.0f) * halfw + halfw + (f32)rt.ax;
		f32 y = sin(d*xui_pi/180.0f) * halfh + halfh + (f32)rt.ay;
		if (xui_pixel_align(x) == lastx && 
			xui_pixel_align(y) == lasty)
			continue;

		glVertex2f(xui_pixel_align(x)+0.375f, xui_pixel_align(y)+0.375f);
		lastx = xui_pixel_align(x);
		lasty = xui_pixel_align(y);
	}

	glEnd();
}

void xui_canvas::fill_arc( 
    const xui_rect2d<s32>&	rt,
	const xui_colour&		color,
	s32						start,
	s32						sweep,
	s32						precision )
{
	if (color.a == 0.0f || m_cliprect.was_valid() == false)
		return;

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_POLYGON_SMOOTH);
	glUseProgram(0);
	glBegin(GL_TRIANGLE_FAN);
	glColor4fv(color.value);

	f32 halfw = rt.get_w() / 2.0f;
	f32 halfh = rt.get_h() / 2.0f;
	f32 lastx = -1.0f;
	f32 lasty = -1.0f;
	s32 count = sweep / precision;

	glVertex2f(halfw + (f32)rt.ax, halfh + (f32)rt.ay);
	for (s32 i = 0; i <= count; ++i)
	{
		s32 d = start + precision *  i;
		f32 x = cos(d*xui_pi/180.0f) * halfw + halfw + (f32)rt.ax;
		f32 y = sin(d*xui_pi/180.0f) * halfh + halfh + (f32)rt.ay;
		if (xui_pixel_align(x) == lastx && 
			xui_pixel_align(y) == lasty)
			continue;

		glVertex2f(xui_pixel_align(x), xui_pixel_align(y));
		lastx = xui_pixel_align(x);
		lasty = xui_pixel_align(y);
	}

	glEnd();
}

void xui_canvas::draw_tick( 
    const xui_vector<s32>&	center, 
	s32						half, 
	const xui_colour&		color )
{
	xui_vector<s32> p1;
	xui_vector<s32> p2;

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LINE_SMOOTH);
	glUseProgram(0);
	glBegin(GL_LINES);
	glColor4fv(color.value);

	p1 = xui_vector<s32>(center.x-half+1, center.y  );
	p2 = xui_vector<s32>(center.x-half+5, center.y+half-1);
	glVertex2f((f32)(p1.x  ), (f32)(p1.y  ));
	glVertex2f((f32)(p2.x  ), (f32)(p2.y  ));
	glVertex2f((f32)(p1.x+1), (f32)(p1.y  ));
	glVertex2f((f32)(p2.x+1), (f32)(p2.y  ));
	glVertex2f((f32)(p1.x  ), (f32)(p1.y-1));
	glVertex2f((f32)(p2.x  ), (f32)(p2.y-1));
	glVertex2f((f32)(p1.x+1), (f32)(p1.y-1));
	glVertex2f((f32)(p2.x+1), (f32)(p2.y-1));

	p1 = xui_vector<s32>(center.x-half+5, center.y+half-1);
	p2 = xui_vector<s32>(center.x+half-1, center.y-half+2);
	glVertex2f((f32)(p1.x  ), (f32)(p1.y  ));
	glVertex2f((f32)(p2.x  ), (f32)(p2.y  ));
	glVertex2f((f32)(p1.x-1), (f32)(p1.y  ));
	glVertex2f((f32)(p2.x-1), (f32)(p2.y  ));
	glVertex2f((f32)(p1.x  ), (f32)(p1.y-1));
	glVertex2f((f32)(p2.x  ), (f32)(p2.y-1));
	glVertex2f((f32)(p1.x-1), (f32)(p1.y-1));
	glVertex2f((f32)(p2.x-1), (f32)(p2.y-1));

	glEnd();
}

xui_family_create* xui_canvas::get_family_create( void )
{
	return m_familycreate;
}

xui_family_member* xui_canvas::get_family_member( const xui_family& family, u16 wc )
{
	xui_family_member* member = NULL;
	for (u32 i = 0; i < m_familybitmapvec.size(); ++i)
	{
		member = m_familybitmapvec[i]->get_member(family, wc);
		if (member)
			return member;
	}

	if (m_familybitmapvec.size() > 0)
		member = m_familybitmapvec.back()->add_member(family, wc);

	if (member == NULL)
	{
		xui_family_bitmap* bitmap = new xui_family_bitmap;
		m_familybitmapvec.push_back(bitmap);
		member = bitmap->add_member(family, wc);
	}

	return member;
}
