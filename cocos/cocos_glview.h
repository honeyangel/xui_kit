#ifndef __cocos_glview_h__
#define __cocos_glview_h__

#include "base/CCRef.h"
#include "platform/CCCommon.h"
#include "platform/CCGLView.h"

class cocos_glview : public cocos2d::GLView
{
public:
	/*
	//static
	*/
    static cocos_glview* create( HWND hwnd );

    /* 
	//override functions 
	*/
    virtual bool	isOpenGLReady		( void );
    virtual void	end					( void );
    virtual void	swapBuffers			( void );
	virtual void	setIMEKeyboardState	( bool flag );
	virtual void	appendEditBoxText	( const std::string& text );
	virtual void	moveCursorToEnd		( int pos = -1 );
	virtual void	ClearTextToNavtive	( void );
    virtual HWND	getWin32Window		( void );

protected:
	/*
	//constructor
	*/
    cocos_glview( HWND hwnd );

	/*
	//member
	*/
	HWND			m_hwnd;
};

#endif//__cocos_glview_h__
