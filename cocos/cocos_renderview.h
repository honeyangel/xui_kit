#ifndef __cocos_renderview_h__
#define __cocos_renderview_h__

#include "xui_control.h"

namespace cocos2d
{
	class Scene;
}

class cocos_glview;
class cocos_renderview : public xui_control
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	cocos_renderview( const xui_vector<s32>& size, const xui_vector<s32>& buffersize );

	/*
	//destructor
	*/
	virtual ~cocos_renderview( void );

	/*
	//method
	*/
	cocos2d::Scene*	get_2droot			( void );

	/*
	//virtual
	*/
	virtual void	render				( void );

protected:
	/*
	//callback
	*/
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
protected:
	cocos2d::Scene*	m_cocos2droot;
	cocos_glview*	m_cocosglview;
	xui_bitmap*		m_colorbitmap;
	xui_vector<s32>	m_framebuffersize;
	GLuint			m_framebuffername;
	GLuint			m_depthbuffername;
	GLuint			m_framebufferlast;
};

#endif//__cocos_renderview_h__