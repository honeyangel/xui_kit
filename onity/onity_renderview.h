#ifndef __onity_renderview_h__
#define __onity_renderview_h__

#include "xui_control.h"

class onity_renderview : public xui_control
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_renderview( const xui_vector<s32>& size, const xui_vector<s32>& buffersize );

	/*
	//destructor
	*/
	virtual ~onity_renderview( void );

	/*
	//virtual
	*/
	virtual void	render				( void );

protected:
	/*
	//callback
	*/
	virtual void	on_load				( xui_method_args& args );
	virtual void	on_setrendersz		( xui_method_args& args );

	/*
	//method
	*/
	void			create_framebuffer	( void );
	void			delete_framebuffer	( void );
	void			attach_framebuffer	( void );
	void			detach_framebuffer	( void );

	/*
	//member
	*/
	xui_bitmap*		m_colorbitmap;
	xui_vector<s32> m_framebuffersize;
	GLuint			m_framebuffername;
	GLuint			m_depthbuffername;
	GLuint			m_framebufferlast;
};

#endif//__onity_renderview_h__