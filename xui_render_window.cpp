#include <windows.h>
#include "xui_render_window.h"

/*
//constructor
*/
xui_create_explain(xui_render_window)( void* hwnd )
: m_hwnd(hwnd)
{
	// get dc
	m_hdc = ::GetDC((HWND)m_hwnd);

	PIXELFORMATDESCRIPTOR pfd;
	int format;

	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize		= sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion	= 1;
	pfd.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType	= PFD_TYPE_RGBA;
	pfd.cRedBits	= 8;
	pfd.cGreenBits	= 8;
	pfd.cBlueBits	= 8;
	pfd.cAlphaBits	= 8;
	pfd.cDepthBits	= 16;

	format = ChoosePixelFormat((HDC)m_hdc, &pfd);

	SetPixelFormat((HDC)m_hdc, format, &pfd);
	m_context = (void*)wglCreateContext((HDC)m_hdc);
	current();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glHint(GL_POINT_SMOOTH_HINT,   GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT,    GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

/*
//destructor
*/
xui_render_window::~xui_render_window( void )
{
	wglDeleteContext((HGLRC)m_context);
}

/*
//method
*/
xui_method_explain(xui_render_window, get_hwnd, void*	)( void )
{
	return m_hwnd;
}
xui_method_explain(xui_render_window, current,	void	)( void )
{
	wglMakeCurrent((HDC)m_hdc, (HGLRC)m_context);
}
xui_method_explain(xui_render_window, present,	void	)( void )
{
	glFinish();
	SwapBuffers((HDC)m_hdc);
}