#ifndef __xui_propview_h__
#define __xui_propview_h__

#include "xui_propkind.h"
#include "xui_proproot.h"
#include "xui_container.h"

typedef std::vector<xui_proproot*>	xui_proproot_vec;
class xui_propview : public xui_container
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static const s32		default_lineheight;
	static const s32		default_nodeindent;
	static xui_propview*	create			( void );

	/*
	//constructor
	*/
	xui_propview( const xui_vector<s32>& size );

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
	void					add_propelse	( const xui_proproot_vec& lastroot, xui_proproot* propelse );
	void					del_propelse	( const xui_proproot_vec& lastroot );
	void					del_proproot	( xui_proproot* proproot );

	/*
	//virtual
	*/
	virtual void			render_else		( void );

	/*
	//method
	*/
	void					reset			( void );

protected:
	/*
	//override
	*/
	virtual void			on_invalid		( xui_method_args& args );
	virtual void			on_perform		( xui_method_args& args );

	/*
	//method
	*/
	void					del_kindctrl	( xui_proproot* proproot );
	void					del_kindctrl	( xui_propkind* propkind );
	void					del_kindctrlall	( void );
	xui_kindctrl*			get_kindctrl	( xui_propkind* propkind );
	xui_propkind_vec		get_samekind	( void );
	xui_propkind_vec		get_propkindall	( xui_propkind* propkind );

	/*
	//def
	*/
	typedef std::map< std::string, std::vector<xui_kindctrl*> >
		xui_kindctrl_map;

	/*
	//member
	*/
	xui_proproot*			m_proproot;
	xui_proproot_vec		m_proprootvec;
	xui_proproot_vec		m_propelsevec;
	xui_kindctrl_map		m_kindctrlmap;
};

#endif//__xui_propview_h__