#include "xui_bitmap.h"
#include "xui_family_bitmap.h"
#include "xui_family_member.h"
#include "xui_convas.h"

/*
//global
*/
xui_convas* g_convas = NULL;

/*
//constructor
*/
xui_convas::xui_convas( void )
{}

/*
//clip
*/
xui_method_explain(xui_convas, get_clip_rect,		xui_rect2d<s32>		)( void ) const
{
	return xui_rect2d<s32>();
}

xui_method_explain(xui_convas, set_clip_rect,		void				)( const xui_rect2d<s32>&	rect )
{
	xui_vector<s32> pt = rect.get_pt();
	xui_vector<s32> sz = rect.get_sz();
	if (sz.w == 0 || sz.h == 0)
	{
		glDisable(GL_SCISSOR_TEST);
	}
	else
	{
		glEnable (GL_SCISSOR_TEST);
		glScissor(
			pt.x, 
			pt.y, 
			sz.w, 
			sz.h);
	}
}

/*
//image
*/
xui_method_explain(xui_convas, draw_image,			void				)( xui_bitmap*				image,
																	       const xui_vector<s32>&	pt,
																	       const xui_colour&		color )
{
	draw_image(
		image,
		xui_rect2d<s32>(pt, image->get_size()),
		color);
}

xui_method_explain(xui_convas, draw_image,			void				)( xui_bitmap*				image,
																	       const xui_rect2d<s32>&	dst,
																	       const xui_colour&		color )
{
	draw_image(
		image,
		xui_rect2d<s32>(xui_vector<s32>(0, 0), image->get_size()),
		dst,
		color);
}

xui_method_explain(xui_convas, draw_image,			void				)( xui_bitmap*				image,
																	       const xui_rect2d<s32>&	src,
																	       const xui_rect2d<s32>&	dst,
																	       const xui_colour&		color )
{
	image->set_bind();

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

/*
//text
*/
xui_method_explain(xui_convas, calc_size,			xui_vector<s32>		)( const std::wstring&		text,
																		   const xui_family&		family,
																		   const xui_rect2d<s32>&	rect,
																		   bool						flag )
{
	xui_vector<s32> size;

	if (flag)
	{
		size.w = 0;
		size.h = family.size;
		for (u32 i = 0; i < text.length(); ++i)
		{
			//非可见字符
			if (text[i] < 0x20)
				continue;

			xui_family_member* member = get_family_member(family, text[i]);
			size.w += member->rt.get_sz().w;
		}
	}
	else
	{
		//绘制多行
		s32 line = 0;
		s32 curw = 0;
		s32 maxw = rect.get_sz().w;

		//single line buffer
		std::wstring buffer;

		wchar_t*p = (wchar_t*)text.c_str();
		while (*p)
		{
			if (*p == L'\n')
			{
				buffer.clear();

				curw = 0;
				++line;
				++p;
			}
			else
			{
				std::wstring word;
				s32 sw  = calc_word(std::wstring(p), family, maxw, word);
				if (sw == 0 && word.empty())
					break;

				if (sw+curw > maxw)
				{
					//宽度太小
					if (curw == 0)
						break;

					buffer.clear();
					curw = 0;
					++line;
				}
				else
				{
					curw	+= sw;
					buffer	+= word;
					p		+= word.length();
				}
			}
		}

		if (!buffer.empty())
			++line;

		size.w = line > 1 ? maxw : curw;
		size.h = line * family.size;
	}

	return size;
}

xui_method_explain(xui_convas, calc_rect,			xui_rect2d<s32>		)( const std::wstring&		text,
																		   const xui_family&		family,
																		   const xui_rect2d<s32>&	rect,
																		   u08						align,
																		   bool						flag )
{
	xui_rect2d<s32>  text_rect = rect;
	xui_vector<s32>  text_size = calc_size(text, family, rect, flag);
	text_rect.set_sz(text_size);

	//水平方向对齐
	switch (align)
	{
	case TA_CT:	//CT
	case TA_CC:	//CC
	case TA_CB:	//CB
		text_rect.oft_x((rect.get_sz().w - text_rect.get_sz().w) / 2);
		break;
	case TA_RT:	//RT
	case TA_RC:	//RC
	case TA_RB:	//RB
		text_rect.oft_x((rect.get_sz().w - text_rect.get_sz().w));
		break;
	}

	//竖直方向对齐
	switch (align)
	{
	case TA_LC:	//LC
	case TA_CC:	//CC
	case TA_RC:	//RC
		text_rect.oft_y((rect.get_sz().h - text_rect.get_sz().h) / 2);
		break;
	case TA_LB:	//LB
	case TA_CB:	//CB
	case TA_RB:	//RB
		text_rect.oft_y((rect.get_sz().h - text_rect.get_sz().h));
		break;
	}

	return text_rect;
}

xui_method_explain(xui_convas, calc_text,			std::wstring		)( const std::wstring&		text,
																		   const xui_family&		family,
																		   s32						maxw )
{
	//绘制多行
	std::wstring temp;

	s32 curw = 0;
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
			s32 sw =  calc_word(std::wstring(p), family, maxw, word);
			if (sw == 0 && word.empty())
				break;

			if (sw+curw > maxw)
			{
				break;
			}
			else
			{
				curw += sw;
				temp += word;
				p	 += word.length();
			}
		}
	}

	return temp;
}

xui_method_explain(xui_convas, calc_char,			u32					)( const std::wstring&		text,
																		   const xui_family&		family,
																		   s32						x )
{
	if (x < 0)	
		return 0;

	s32 currx  = 0;
	u32 index  = 0;
	for (u32 i = 0; i < text.length(); ++i)
	{			
		if (text[i] == L'\n')
			continue;

		xui_family_member* member = get_family_member(family, text[i]);
		if (x < currx + member->rt.get_sz().w/2)
			break;

		currx = currx + member->rt.get_sz().w;
		++index;
	}

	return index;
}

xui_method_explain(xui_convas, calc_word,			u32					)( const std::wstring&		text,
																		   const xui_family&		family,
																		   s32						maxw,
																		   std::wstring&			word )
{
	word.clear();

	s32 w = 0;
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

		xui_family_member* member = get_family_member(family, text[i]);

		//单词字符
		if ((text[i] >= L'a' && text[i] <= L'z') ||
			(text[i] >= L'A' && text[i] <= L'Z'))
		{
			if (w + member->rt.get_sz().w > maxw)
				break;

			w    += member->rt.get_sz().w;
			word += text[i];
		}
		//空格中文
		else
		{
			if (word.length())
				break;

			if (w + member->rt.get_sz().w > maxw)
				break;

			w	 += member->rt.get_sz().w;
			word += text[i];
			break;
		}
	}

	return w;
}

xui_method_explain(xui_convas, draw_text,			void				)( const std::wstring&		text,
																	       const xui_family&		family,
																	       const xui_rect2d<s32>&	rect,
																	       const xui_colour&		color )
{
	xui_vector<s32> pt = rect.get_pt();

	//绘制多行
	s32 curw = 0;
	s32 maxw = rect.get_sz().w;
	std::wstring buffer;

	wchar_t*p = (wchar_t*)text.c_str();
	while (*p)
	{
		if(*p == L'\n')
		{
			draw_text(buffer, family, pt, color);
			pt.x  = rect.ax;
			pt.y += family.size;
			buffer.clear();

			curw = 0;
			++p;
		}
		else
		{
			std::wstring word;
			s32 sw =  calc_word(std::wstring(p), family, maxw, word);
			if (sw == 0 && word.empty())
				break;

			if (sw + curw > maxw)
			{
				//宽度太小
				if (curw == 0)
					break;

				draw_text(buffer, family, pt, color);
				pt.x  = rect.ax;
				pt.y += family.size;
				buffer.clear();

				curw = 0;
			}
			else
			{
				curw	 += sw;
				buffer	 += word;
				p		 += word.length();
			}
		}
	}

	if (!buffer.empty())
		draw_text(buffer, family, pt, color);
}

xui_method_explain(xui_convas, draw_text,			void				)( const std::wstring&		text,
																		   const xui_family&		family,
																		   const xui_vector<s32>&	pt,
																		   const xui_colour&		color )
{
	xui_rect2d<s32> dst;
	dst.set_pt(pt);

	for (u32 i = 0; i < text.length(); ++i)
	{
		//非可见字符
		if (text[i] < 0x20)
			continue;

		xui_family_member* member = get_family_member(family, text[i]);
		dst.set_sz(member->rt.get_sz());

		draw_image(
			member->bitmap,
			member->rt,
			dst,
			color);

		dst.oft_x(member->rt.get_sz().w);
	}
}

/*
//line
*/
xui_method_explain(xui_convas, draw_line,			void				)( const xui_vector<s32>&	min, 
																	       const xui_vector<s32>&	max,
																	       const xui_colour&		color,
																	       s32						width )
{
	glBegin(GL_LINES);
	glColor4fv(color.value);

	glVertex2f((f32)min.x, (f32)min.y);
	glVertex2f((f32)max.x, (f32)max.y);

	glEnd();
}

/*
//path
*/
xui_method_explain(xui_convas, draw_path,			void				)( xui_vector<s32>*			path,
																	       u32						count,
																	       const xui_colour&		color )
{
	glBegin(GL_LINE_STRIP);
	glColor4fv(color.value);

	for (u32 i = 0; i < count; ++i)
		glVertex2f((f32)path[i].x-0.375f, (f32)path[i].y-0.375f);

	glEnd();
}

xui_method_explain(xui_convas, fill_poly,			void				)( xui_vector<s32>*			path,
																	       u32						count,
																	       const xui_colour&		color )
{
	glBegin(GL_TRIANGLES);
	glColor4fv(color.value);

	for (u32 i = 0; i < count; ++i)
		glVertex2f((f32)path[i].x-0.375f, (f32)path[i].y-0.375f);

	glEnd();
}

/*
//rectangle
*/
xui_method_explain(xui_convas, draw_rectangle,		void				)( const xui_rect2d<s32>&	rect,
																	       const xui_colour&		color )
{
	draw_line(xui_vector<s32>(rect.ax, rect.ay), xui_vector<s32>(rect.bx, rect.ay),		color);
	draw_line(xui_vector<s32>(rect.ax, rect.ay), xui_vector<s32>(rect.ax, rect.by+1),	color);
	draw_line(xui_vector<s32>(rect.ax, rect.by), xui_vector<s32>(rect.bx, rect.by),		color);
	draw_line(xui_vector<s32>(rect.bx, rect.ay), xui_vector<s32>(rect.bx, rect.by+1),	color);
}

xui_method_explain(xui_convas, fill_rectangle,		void				)( const xui_rect2d<s32>&	rect,
																	       const xui_colour&		color )
{
	glDisable(GL_POLYGON_SMOOTH);
	glBegin(GL_QUADS);
	glColor4fv(color.value);

	glVertex2f((f32)rect.ax-0.375f, (f32)rect.ay-0.375f);
	glVertex2f((f32)rect.ax-0.375f, (f32)rect.by-0.375f);
	glVertex2f((f32)rect.bx-0.375f, (f32)rect.by-0.375f);
	glVertex2f((f32)rect.bx-0.375f, (f32)rect.ay-0.375f);

	glEnd();
	glEnable(GL_POLYGON_SMOOTH);
}

/*
//round
*/
xui_method_explain(xui_convas, draw_round,			void				)( const xui_rect2d<s32>&	rect,
																	       const xui_colour&		color,
																	       s32						corner )
{
	draw_round(
		rect, 
		color, 
		xui_rect2d<s32>(corner));
}

xui_method_explain(xui_convas, draw_round,			void				)( const xui_rect2d<s32>&	rect,
																	       const xui_colour&		color,
																	       const xui_rect2d<s32>&   corner )
{
	//左上/右上/右下/左下
	s32 corner0 = corner.value[0];
	s32 corner1 = corner.value[1];
	s32 corner2 = corner.value[2];
	s32 corner3 = corner.value[3];
	if (corner0) 
		draw_arc(xui_rect2d<s32>(
			rect.ax,			 
			rect.ay,
			rect.ax+2*corner0, 
			rect.ay+2*corner0),	color, 180.0f, 90.0f, 1.0f);
	if (corner1) 
		draw_arc(xui_rect2d<s32>(
			rect.bx-2*corner1, 
			rect.ay,			
			rect.bx,		   
			rect.ay+2*corner1),	color, 270.0f, 90.0f, 1.0f);
	if (corner2) 
		draw_arc(xui_rect2d<s32>(
			rect.bx-2*corner2, 
			rect.by-2*corner2, 
			rect.bx,		   
			rect.by),			color,   0.0f, 90.0f, 1.0f);
	if (corner3) 
		draw_arc(xui_rect2d<s32>(
			rect.ax,			 
			rect.by-2*corner3, 
			rect.ax+2*corner3, 
			rect.by),			color,  90.0f, 90.0f, 1.0f);

	draw_line(xui_vector<s32>(rect.ax+corner0, rect.ay),		   xui_vector<s32>(rect.bx-corner1, rect.ay),			color);
	draw_line(xui_vector<s32>(rect.bx,		   rect.ay+corner1),   xui_vector<s32>(rect.bx,		    rect.by-corner2+1),	color);
	draw_line(xui_vector<s32>(rect.bx-corner2, rect.by),		   xui_vector<s32>(rect.ax+corner3, rect.by),			color);
	draw_line(xui_vector<s32>(rect.ax,		   rect.by-corner3+1), xui_vector<s32>(rect.ax,		    rect.ay+corner0),	color);
}

xui_method_explain(xui_convas, fill_round,			void				)( const xui_rect2d<s32>&	rect,
																	       const xui_colour&		color,
																	       s32						corner )
{
	fill_round(
		rect,
		color,
		xui_rect2d<s32>(corner));
}

xui_method_explain(xui_convas, fill_round,			void				)( const xui_rect2d<s32>&	rect,
																	       const xui_colour&		color,
																	       const xui_rect2d<s32>&   corner )
{
	//左上/右上/右下/左下
	s32 corner0 = corner.value[0];
	s32 corner1 = corner.value[1];
	s32 corner2 = corner.value[2];
	s32 corner3 = corner.value[3];
	if (corner0) 
		fill_arc(xui_rect2d<s32>(
			rect.ax,
			rect.ay,
			rect.ax+2*corner0, 
			rect.ay+2*corner0),	color, 180.0f, 90.0f, 1.0f);
	if (corner1) 
		fill_arc(xui_rect2d<s32>(
			rect.bx-2*corner1,	
			rect.ay,
			rect.bx,
			rect.ay+2*corner1),	color, 270.0f, 90.0f, 1.0f);
	if (corner2) 
		fill_arc(xui_rect2d<s32>(
			rect.bx-2*corner2,	
			rect.by-2*corner2, 
			rect.bx,
			rect.by),			color,   0.0f, 90.0f, 1.0f);
	if (corner3) 
		fill_arc(xui_rect2d<s32>(
			rect.ax,
			rect.by-2*corner3, 
			rect.ax+2*corner3, 
			rect.by),			color,  90.0f, 90.0f, 1.0f);

	//上边
	fill_rectangle(xui_rect2d<s32>(
		rect.ax + corner0,	
		rect.ay,		 
		rect.bx - corner1, 
		rect.ay +(corner0|corner1)),	
		color);
	//右边
	fill_rectangle(xui_rect2d<s32>(
		rect.bx -(corner1|corner2),	
		rect.ay + corner1, 
		rect.bx,		  
		rect.by - corner2),	
		color);
	//下边
	fill_rectangle(xui_rect2d<s32>(
		rect.ax + corner3,	
		rect.by -(corner2|corner3), 
		rect.bx - corner2, 
		rect.by),			
		color);
	//左边
	fill_rectangle(xui_rect2d<s32>(
		rect.ax,			
		rect.ay + corner0, 
		rect.ax +(corner3|corner0), 
		rect.by - corner3),	
		color);
	//中间
	fill_rectangle(xui_rect2d<s32>(
		rect.ax +(corner3|corner0),	
		rect.ay +(corner0|corner1), 
		rect.bx -(corner1|corner2), 
		rect.by -(corner2|corner3)),	
		color);
}

/*
//arc
*/
xui_method_explain(xui_convas, draw_arc,			void				)( const xui_rect2d<s32>&	rect,
																	       const xui_colour&		color,
																	       f32						start,
																	       f32						sweep,
																	       f32						precision )
{
	glBegin(GL_LINE_STRIP);
	glColor4fv(color.value);

	f32 halfw = rect.get_sz().w / 2.0f;
	f32 halfh = rect.get_sz().h / 2.0f;
	f32 lastx = -1.0f;
	f32 lasty = -1.0f;
	s32 count = (s32)(sweep / precision);
	for (s32 i = 0; i <= count; ++i)
	{
		f32 d = start + precision *  i;
		f32 x = cos(d*xui_pi/180.0f) * halfw + halfw + (f32)rect.ax;
		f32 y = sin(d*xui_pi/180.0f) * halfh + halfh + (f32)rect.ay;
		if (xui_pixel_align(x) == lastx && 
			xui_pixel_align(y) == lasty)
			continue;

		glVertex2f(xui_pixel_align(x), xui_pixel_align(y));
		lastx = xui_pixel_align(x);
		lasty = xui_pixel_align(y);
	}

	glDisable(GL_LINE_SMOOTH);
	glEnd();
}

xui_method_explain(xui_convas, fill_arc,			void				)( const xui_rect2d<s32>&	rect,
																		   const xui_colour&		color,
																	       f32						start,
																	       f32						sweep,
																	       f32						precision )
{
	glDisable(GL_POLYGON_SMOOTH);
	glBegin(GL_TRIANGLE_FAN);
	glColor4fv(color.value);

	f32 halfw = rect.get_sz().w / 2.0f;
	f32 halfh = rect.get_sz().h / 2.0f;
	f32 lastx = -1.0f;
	f32 lasty = -1.0f;
	s32 count = (s32)(sweep / precision);

	glVertex2f(halfw + (f32)rect.ax, halfh + (f32)rect.ay);
	for (s32 i = 0; i <= count; ++i)
	{
		f32 d = start + precision *  i;
		f32 x = cos(d*xui_pi/180.0f) * halfw + halfw + (f32)rect.ax;
		f32 y = sin(d*xui_pi/180.0f) * halfh + halfh + (f32)rect.ay;
		if (xui_pixel_align(x) == lastx && 
			xui_pixel_align(y) == lasty)
			continue;

		glVertex2f(xui_pixel_align(x)-0.375f, xui_pixel_align(y)-0.375f);
		lastx = xui_pixel_align(x);
		lasty = xui_pixel_align(y);
	}

	glEnd();
	glEnable(GL_POLYGON_SMOOTH);
}

xui_method_explain(xui_convas, get_family_bitmap,	xui_family_bitmap*	)( const xui_family&		family )
{
	return (xui_family_bitmap*)xui_bitmap::create(family);
}

xui_method_explain(xui_convas, get_family_member,	xui_family_member*	)( const xui_family&		family, u16 wc )
{
	return get_family_bitmap(family)->get_member(wc);
}