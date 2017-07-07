#include <windows.h>
#include "xui_convas.h"
#include "xui_render_window.h"

/*
//static
*/
PIXELFORMATDESCRIPTOR	g_pfd;
int						g_format  = 0;
HGLRC					g_context = NULL;

xui_method_explain(xui_render_window, init, void)( HWND hwnd )
{
	HDC hdc = ::GetDC(hwnd);

	memset(&g_pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	g_pfd.nSize			= sizeof(PIXELFORMATDESCRIPTOR);
	g_pfd.nVersion		=  1;
	g_pfd.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	g_pfd.iPixelType	= PFD_TYPE_RGBA;
	g_pfd.cRedBits		=  8;
	g_pfd.cGreenBits	=  8;
	g_pfd.cBlueBits		=  8;
	g_pfd.cAlphaBits	=  8;
	g_pfd.cDepthBits	= 24;
	g_pfd.cStencilBits  =  8;

	g_format  = ChoosePixelFormat(hdc, &g_pfd);
	SetPixelFormat(hdc, g_format, &g_pfd);

	g_context = wglCreateContext(hdc);
	wglMakeCurrent(hdc, g_context);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_POINT_SMOOTH_HINT,   GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT,    GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

/*
//constructor
*/
xui_create_explain(xui_render_window)( HWND hwnd )
: m_hwnd(hwnd)
{
	m_hdc = ::GetDC(m_hwnd);
	SetPixelFormat(m_hdc, g_format, &g_pfd);
}

/*
//method
*/
xui_method_explain(xui_render_window, get_hwnd, HWND	)( void )
{
	return m_hwnd;
}
xui_method_explain(xui_render_window, current,	void	)( void )
{
	wglMakeCurrent(m_hdc, g_context);

	RECT rect;
	GetClientRect(m_hwnd, &rect);
	int w = rect.right-rect.left;
	int h = rect.bottom-rect.top;
	xui_method_inscall(xui_convas, set_viewport	)(xui_rect2d<s32>(0, 0, w, h));
	xui_method_inscall(xui_convas, clear		)(xui_colour(1.0f, 0.1f, 0.1f, 0.1f));
	xui_method_inscall(xui_convas, begin		)();
}
xui_method_explain(xui_render_window, present,	void	)( void )
{
	glFinish();
	SwapBuffers(m_hdc);
}