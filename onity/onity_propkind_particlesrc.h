#ifndef __onity_propkind_particlesrc_h__
#define __onity_propkind_particlesrc_h__

#include "xui_propkind.h"
#include "xui_kindctrl.h"

class NPParticleSpriteSRC;
//////////////////////////////////////////////////////////////////////////
//propkind
//////////////////////////////////////////////////////////////////////////
class onity_propkind_particlesrc : public xui_propkind
{
public:
	/*
	//constructor
	*/
	onity_propkind_particlesrc( xui_proproot* root, NPParticleSpriteSRC* spritesrc );

	/*
	//method
	*/
	NPParticleSpriteSRC*	get_spritesrc		( void );

protected:
	/*
	//method
	*/
	void					add_common			( void );
	void					add_sprite			( void );
	void					add_senior			( void );

	/*
	//static
	*/
	static f64				get_maxamount		( void* userptr );
	static void				set_maxamount		( void* userptr, f64 value );
	static s32				get_origin			( void* userptr );
	static void				set_origin			( void* userptr, s32 value );
	static void*			get_texfile			( void* userptr );
	static void				set_texfile			( void* userptr, void* value );

	/*
	//member
	*/
	NPParticleSpriteSRC*	m_spritesrc;
};

///////////////////////////////////////////////////////////////////////////
//kindctrl
//////////////////////////////////////////////////////////////////////////
class onity_renderview;
class onity_kindctrl_particlesrc : public xui_kindctrl
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static xui_kindctrl*	create				( xui_propkind* propkind );

	/*
	//constructor
	*/
	onity_kindctrl_particlesrc( xui_propkind* propkind );

protected:
	/*
	//callback
	*/
	virtual void			on_perform			( xui_method_args& args );

	/*
	//event
	*/
	void					on_killctrlclick	( xui_component* sender, xui_method_args& args );
	void					on_iconrenderself	( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	onity_renderview*		m_iconview;
	xui_button*				m_killctrl;
};
class onity_kindctrl_particleadd : public xui_kindctrl
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static xui_kindctrl*	create				( xui_propkind* propkind );

	/*
	//constructor
	*/
	onity_kindctrl_particleadd( xui_propkind* propkind );

protected:
	/*
	//callback
	*/
	virtual void			on_perform			( xui_method_args& args );

	/*
	//override
	*/
	virtual s32				get_elsectrlsize	( void );
	virtual s32				get_prevctrlsize	( void );

	/*
	//event
	*/
	void					on_prevctrlclick	( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_button*				m_prevctrl;
};

#endif//__onity_propkind_particlesrc_h__