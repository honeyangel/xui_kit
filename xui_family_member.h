#ifndef __xui_family_member_h__
#define __xui_family_member_h__

#include "xui_rect2d.h"

class xui_family_member 
{
public:
	u16				wc;
	xui_bitmap*		bitmap;
	xui_rect2d<s32>	normal;
	xui_rect2d<s32> stroke;
	xui_vector<s32>	bearing;
	s32				advance;
};

#endif//__xui_family_member_h__