#ifndef __xui_bitmap_format_h__
#define __xui_bitmap_format_h__

#include "xui_header.h"

class xui_bitmap_format
{
public:
	enum
	{
		UNKNOWN,
		PAL8,
		PALA8,
		R8G8B8,
		R8G8B8A8,
		A8,
	};

	static u08 get_stride( u08 format );
};

#endif//__xui_bitmap_format_h__