#include "xui_bitmap_format.h"

u08	xui_bitmap_format::get_stride( u08 format )
{
	switch (format)
	{
	case A8:
	case PAL8:
		return 1;
	case PALA8:
		return 2;
	case R8G8B8:
		return 3;
	case R8G8B8A8:
		return 4;
	}

	return 0;
}