#ifndef __xui_propview_h__
#define __xui_propview_h__

#include "xui_propkind.h"
#include "xui_proproot.h"
#include "xui_container.h"

typedef std::vector<xui_proproot*>	xui_proproot_vec;
class xui_propview : public xui_container
{
public:
	/*
	//static
	*/
	static const s32 LINE_HEIGHT;
	static const s32 NODE_INDENT;

	/*
	//constructor
	*/
	xui_propview( const std::string& name, const xui_rect2d<s32>& rect );

	/*
	//destructor
	*/
	virtual ~xui_propview( void );

	/*
	//property
	*/
	const xui_proproot_vec&	get_proproot	( void ) const;
	void					set_proproot	( xui_proproot* proproot );
	void					set_proproot	( const xui_proproot_vec& proproot );

protected:
	/*
	//override
	*/
	virtual void			on_invalid		( xui_method_args& args );
	virtual void			on_perform		( xui_method_args& args );
	virtual void			on_renderself	( xui_method_args& args );

	/*
	//method
	*/
	void					del_kindctrl	( void );
	xui_kindctrl*			get_kindctrl	( xui_propkind* propkind );
	xui_propkind_vec		get_samekind	( void );
	xui_propkind_vec		get_propkindall	( xui_propkind* propkind );

	/*
	//def
	*/
	typedef std::map< xui_kind_newctrl, std::vector<xui_kindctrl*> >
		xui_kindctrl_map;

	/*
	//member
	*/
	xui_proproot*			m_proproot;
	xui_proproot_vec		m_proprootvec;
	xui_kindctrl_map		m_kindctrlmap;
};

#endif//__xui_propview_h__