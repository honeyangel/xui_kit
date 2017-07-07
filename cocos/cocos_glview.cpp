#include "cocos_glview.h"
#include "platform/CCApplication.h"
#include "base/CCDirector.h"
#include "base/CCTouch.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventKeyboard.h"
#include "base/CCEventMouse.h"
#include "base/CCIMEDispatcher.h"
#include "base/ccUtils.h"
#include "base/ccUTF8.h"
#include "2d/CCCamera.h"
#include "deprecated/CCString.h"
#include "xui_header.h"

/*
//constructor
*/
xui_create_explain(cocos_glview)( HWND hwnd )
: m_hwnd(hwnd)
{}

/*
//static
*/
xui_method_explain(cocos_glview, create,				cocos_glview*	)( HWND hwnd )
{
    return new cocos_glview(hwnd);
}

/*
//method
*/
xui_method_explain(cocos_glview, isOpenGLReady,			bool			)( void )
{
	return true;
}
xui_method_explain(cocos_glview, end,					void			)( void )
{
    release();
}
xui_method_explain(cocos_glview, swapBuffers,			void			)( void )
{}
xui_method_explain(cocos_glview, setIMEKeyboardState,	void			)( bool flag )
{}
xui_method_explain(cocos_glview, appendEditBoxText,		void			)( const std::string& text )
{}
xui_method_explain(cocos_glview, moveCursorToEnd,		void			)( int pos )
{}
xui_method_explain(cocos_glview, ClearTextToNavtive,	void			)( void )
{}
xui_method_explain(cocos_glview, getWin32Window,		HWND			)( void )
{ 
	return m_hwnd; 
}

