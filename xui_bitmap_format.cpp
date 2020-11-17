#include "xui_bitmap_format.h"

u08	xui_bitmap_format::get_stride( u08 format )
{
	switch (format)
	{
	case k_a8:
	case k_pal8:
		return 1;
	case k_pala8:
		return 2;
	case k_r8g8b8:
		return 3;
	case k_r8g8b8a8:
		return 4;
	}

	return 0;
}