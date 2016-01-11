#ifndef __xui_family_member_h__
#define __xui_family_member_h__

#include "xui_rect2d.h"

class xui_family_member 
{
public:
	u16				wc;
	xui_rect2d<s32>	rt;
	xui_bitmap*		bitmap;
};

#endif//__xui_family_member_h__