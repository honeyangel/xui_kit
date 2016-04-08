#include "GLee.h"
#include "xui_convas.h"
#include "xui_bitmap.h"
#include "xui_bitmap_format.h"
#include "onity_renderview.h"

xui_implement_rtti(onity_renderview, xui_control);

/*
//constructor
*/
xui_create_explain(onity_renderview)( const xui_vector<s32>& size, const xui_vector<s32>& buffersize )
: xui_control(size)
{
	m_colorbitmap		= NULL;
	m_framebuffersize	= buffersize;
	m_framebuffername	= -1;
	m_depthbuffername	= -1;
	m_framebufferlast	= -1;
}

/*
//destructor
*/
xui_delete_explain(onity_renderview)( void )
{
	delete_framebuffer();
}

/*
//virtual
*/
xui_method_explain(onity_renderview, render,				void)( void )
{
	xui_rect2d<s32> viewport = xui_convas::get_ins()->get_viewport();
	xui_rect2d<s32> cliprect = xui_convas::get_ins()->get_cliprect();
	attach_framebuffer();
	xui_convas::get_ins()->set_viewport(get_renderrt());
	xui_convas::get_ins()->set_cliprect(xui_rect2d<s32>(0));
	xui_convas::get_ins()->begin();
	xui_method_args     args; 
	on_renderself(      args);
	xm_renderself(this, args);
	detach_framebuffer();
	xui_convas::get_ins()->set_viewport(viewport);
	xui_convas::get_ins()->set_cliprect(cliprect);
	xui_convas::get_ins()->begin();

	xui_rect2d<s32> src;
	src.ax = 0;
	src.bx = get_renderw();
	src.ay = get_renderh();
	src.by = 0;
	xui_rect2d<s32> dst = get_renderrtabs();
	xui_convas::get_ins()->draw_image(m_colorbitmap, src, dst, get_vertexcolor());

	xui_control::render_else();
}

/*
//callback
*/
xui_method_explain(onity_renderview, on_load,				void)( xui_method_args& args )
{
	xui_control::on_load(args);
	create_framebuffer();
}

/*
//method
*/
xui_method_explain(onity_renderview, create_framebuffer,	void)( void )
{
	if (m_framebuffername == -1)
	{
		glGenFramebuffers (1, &m_framebuffername);
		glGenRenderbuffers(1, &m_depthbuffername);
		attach_framebuffer();

		xui_vector<s32> size = get_rendersz();
		m_colorbitmap = new xui_bitmap(m_framebuffersize, xui_bitmap_format::R8G8B8, NULL);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D,
			m_colorbitmap->get_object(),
			0);

		glBindRenderbuffer			(GL_RENDERBUFFER, m_depthbuffername);
		glRenderbufferStorage		(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_framebuffersize.w, m_framebuffersize.h);
		glFramebufferRenderbuffer	(GL_FRAMEBUFFER,  GL_DEPTH_ATTACHMENT,  GL_RENDERBUFFER, m_depthbuffername);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		detach_framebuffer();
	}
}
xui_method_explain(onity_renderview, delete_framebuffer,	void)( void )
{
	if (m_colorbitmap)
	{
		delete m_colorbitmap;
		m_colorbitmap = NULL;
	}

	if (m_framebuffername != -1)
	{
		glDeleteFramebuffers (1, &m_framebuffername);
		m_framebuffername  = -1;
	}

	if (m_depthbuffername != -1)
	{
		glDeleteRenderbuffers(1, &m_depthbuffername);
		m_depthbuffername  = -1;
	}
}
xui_method_explain(onity_renderview, attach_framebuffer,	void)( void )
{
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&m_framebufferlast);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffername);
}
xui_method_explain(onity_renderview, detach_framebuffer,	void)( void )
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferlast);
}