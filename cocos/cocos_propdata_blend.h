#ifndef __cocos_propdata_blend_h__
#define __cocos_propdata_blend_h__

#include "xui_propdata.h"
#include "xui_propctrl.h"

class cocos_blend_value
{
public:
	cocos_blend_value()
	{}
	cocos_blend_value( s32 _src, s32 _dst )
	: src(_src)
	, dst(_dst)
	{}

	bool operator == ( const cocos_blend_value& other )
	{
		return src == other.src && dst == other.dst;
	}
	bool operator != ( const cocos_blend_value& other )
	{
		return src != other.src || dst != other.dst;
	}

	s32 src;
	s32 dst;
};

class cocos_propdata_blend : public xui_propdata
{
public:
	/*
	//typedef
	*/
	typedef cocos_blend_value	(*getfunc)( void* userptr );
	typedef void				(*setfunc)( void* userptr, const cocos_blend_value& value );

	/*
	//constructor
	*/
	cocos_propdata_blend( xui_propkind* kind, getfunc userget, setfunc userset, void* userptr );

	/*
	//method
	*/
	cocos_blend_value			get_value			( void );
	void						set_value			( const cocos_blend_value& value );

protected:
	///*
	////static
	//*/
	//static s32			get_blendfunction	( void* userptr );
	//static void			set_blendfunction	( void* userptr, s32 value );
	//static s32			get_blendsrc		( void* userptr );
	//static void			set_blendsrc		( void* userptr, s32 value );
	//static s32			get_blenddst		( void* userptr);
	//static void			set_blenddst		( void* userptr, s32 value );

	/*
	//member
	*/
	getfunc						m_userget;
	setfunc						m_userset;
	void*						m_userptr;
};

class cocos_propctrl_blend : public xui_propctrl
{
	xui_declare_rtti

public:
	/*
	//create
	*/
	static xui_propctrl*		create			( xui_propdata* propdata );

	/*
	//constructor
	*/
	cocos_propctrl_blend( xui_propdata* propdata );

	/*
	//destructor
	*/
	virtual ~cocos_propctrl_blend( void );

	/*
	//propdata
	*/
	virtual void				on_linkpropdata	( bool selfupdate = false );
	virtual void				on_editvalue	( xui_propedit* sender );

protected:
	/*
	//override
	*/
	virtual void				on_invalid		( xui_method_args& args );
	virtual void				on_perform		( xui_method_args& args );

	/*
	//event
	*/
	void						on_buttonclick	( xui_component* sender, xui_method_args& args );

	/*
	//method
	*/
	u32							get_valueindex	( s32 value );
	s32							get_indexvalue	( u32 index );

	/*
	//member
	*/
	xui_propedit_enum*			m_srcedit;
	xui_propedit_enum*			m_dstedit;
	xui_linebox*				m_linebox;
	xui_button*					m_linedef;
	xui_button*					m_lineadd;
};

#endif//__cocos_propdata_blend_h__