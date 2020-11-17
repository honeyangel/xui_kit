#include "xui_native_window.h"
#include "glfw3.h"
#ifndef GLFW_EXPOSE_NATIVE_NSGL
#define GLFW_EXPOSE_NATIVE_NSGL
#endif
#ifndef GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include "glfw3native.h"

id get_macos_handler( xui_native_window* hwnd )
{
    if (hwnd == NULL)
        return nil;
    
    GLFWwindow* glfw_window = hwnd->get_glfw_window();
    if (glfw_window == NULL)
        return nil;
    
    return glfwGetCocoaWindow(glfw_window);
}

bool xui_native_window::was_inside_minimized( const xui_vector<s32>& pt )
{
    return false;
}

void xui_native_window::capture( void )
{}

void xui_native_window::close( void )
{
    if (m_glfw_window)
    {
        id handle = get_macos_handler(this);
        
        // No decorated window cannot trigger the close event as it has no style (NSWindowStyleMaskTitled)
        if(glfwGetWindowAttrib(m_glfw_window, GLFW_DECORATED) == GLFW_TRUE)
        {
            [handle performClose:nil];
        }
        else
        {
            on_window_close(m_glfw_window);
        }
    }
}
