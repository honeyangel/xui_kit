#ifndef __xui_bitmap_format_h__
#define __xui_bitmap_format_h__

#include "xui_header.h"

class xui_bitmap_format
{
public:
	enum
	{
		k_unknown,
		k_pal8,
		k_pala8,
		k_r8g8b8,
		k_r8g8b8a8,
		k_a8,
	};

	static u08 get_stride( u08 format );
};

#endif//__xui_bitmap_format_h__