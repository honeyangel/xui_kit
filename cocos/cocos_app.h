#ifndef __cocos_app_h__
#define __cocos_app_h__

#include "platform/win32/CCApplication-win32.h"

class  cocos_app : private cocos2d::Application
{
public:
	cocos_app(){};

	virtual void initGLContextAttrs(){};

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
	virtual bool applicationDidFinishLaunching(){ return true; }

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
	virtual void applicationDidEnterBackground(){};

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
	virtual void applicationWillEnterForeground(){};
};

#endif//__cocos_app_h__