#ifndef __cocos_scenecmd_h__
#define __cocos_scenecmd_h__

#include "xui_header.h"

class cocos_scenecmd 
{
public:
	virtual void undo( void ) = 0;
	virtual void redo( void ) = 0;
};

#endif//__cocos_scenecmd_h__