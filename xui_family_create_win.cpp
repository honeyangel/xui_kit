#include "xui_family_create_win.h"

#pragma comment (lib, "gdiplus.lib")

/*
//constructor
*/
xui_create_explain(xui_family_create_win)( void )
{
	GdiplusStartup(&m_startup_token, &m_startup_input, NULL);
}

/*
//destructor
*/
xui_delete_explain(xui_family_create_win)( void )
{
	for (std::map<std::string, gdiinfo*>::iterator itor = m_gdiinfo_map.begin(); itor != m_gdiinfo_map.end(); ++itor)
	{
		delete (*itor).second->gdif;
		delete (*itor).second->gdib;
		delete (*itor).second->gdig;
		delete (*itor).second->bits;
		delete (*itor).second;
	}

	m_gdiinfo_map.clear();
	GdiplusShutdown(m_startup_token);
}

/*
method
*/
xui_method_explain(xui_family_create_win, add_font, void			)( const xui_family& family )
{
	std::map<std::string, gdiinfo*>::iterator itor = m_gdiinfo_map.find(family.to_string());
	if (itor == m_gdiinfo_map.end())
	{
		gdiinfo* info = new gdiinfo();

		//font
		wchar_t name[64];
		MultiByteToWideChar(CP_ACP, 0, family.face.c_str(), family.face.length()+1, name, sizeof(name)/sizeof(wchar_t));
		info->gdif = new Font(name, (REAL)family.size, family.bold ? FontStyleBold : FontStyleRegular, UnitPixel);

		//bitmap
		s32 w = family.size*2;
		s32 h = family.size*2;
		s32 a = w*h;
		info->gdib = new Bitmap(w, h);
		info->bits = new u08[a];

		//graphics
		info->gdig = Graphics::FromImage(info->gdib);

		//add to map
		m_gdiinfo_map[family.to_string()] = info;
	}
}

xui_method_explain(xui_family_create_win, get_bits, void*			)( const xui_family& family, u16 wc )
{
	add_font(family);

	gdiinfo* info = m_gdiinfo_map[family.to_string()];

	//draw char
	SolidBrush brush(Color::White);
	info->gdig->SetTextRenderingHint(TextRenderingHintAntiAlias);
	info->gdig->Clear(Color(0));
	info->gdig->DrawString(
		(LPWSTR)(&wc), 
		1, 
		info->gdif, 
		PointF(0.0f,0.0f), 
		StringFormat::GenericTypographic(), 
		&brush);

	//char size
	RectF rect;
	info->gdig->MeasureString(
		(LPWSTR)(&wc), 
		1, 
		info->gdif, 
		PointF(0.0f, 0.0f), 
		StringFormat::GenericTypographic(), 
		&rect);

	info->size.w = (s32)rect.Width ; 
	info->size.h = (s32)rect.Height;

	//copy buffer
	s32 w = info->size.w;
	s32 h = info->size.h;

	Color color;
	for (s32 y = 0; y < h; ++y)
	{
		for (s32 x = 0; x < w; ++x)
		{
			info->gdib->GetPixel(x, y, &color);
			info->bits[y*w+x] = color.GetAlpha();
		}
	}

	if (info->size.w == 0)
		info->size.w =  family.size / 2;

	return info->bits;
}

xui_method_explain(xui_family_create_win, get_size, xui_vector<s32>	)( const xui_family& family )
{
	return m_gdiinfo_map[family.to_string()]->size;
}
