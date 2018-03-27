#ifndef __cocos_propdata_scale9_h__
#define __cocos_propdata_scale9_h__

#include "xui_propdata.h"
#include "xui_propctrl.h"
#include "xui_propedit.h"

class cocos_value_scale9
{
public:
	cocos_value_scale9( void )
	: flag(false)
	{}
	cocos_value_scale9( bool _flag, const xui_rect2d<s32>& _rect, const xui_vector<s32>& _size )
	: flag(_flag)
	, rect(_rect)
	, size(_size)
	{}

	bool operator == ( const cocos_value_scale9& other )
	{
		return (flag == other.flag &&
				rect == other.rect);
	}
	bool operator != ( const cocos_value_scale9& other )
	{
		return (flag != other.flag ||
				rect != other.rect);
	}

	bool			flag;
	xui_rect2d<s32> rect;
	xui_vector<s32>	size;
};

class cocos_propdata_scale9 : public xui_propdata
{
public:
	typedef cocos_value_scale9	(*get_func)( void* userptr );
	typedef void				(*set_func)( void* userptr, const cocos_value_scale9& value );

	/*
	//constructor
	*/
	cocos_propdata_scale9( xui_propkind* kind, const std::wstring& name, get_func userget, set_func userset, void* userptr );

	/*
	//method
	*/
	virtual cocos_value_scale9	get_value				( void ) const;
	virtual void				set_value				( const cocos_value_scale9& value );

protected:
	/*
	//override
	*/
	virtual u08*				do_serialize			( void );
	virtual void				un_serialize			( u08* byte );

	/*
	//member
	*/
	get_func					m_userget;
	set_func					m_userset;
	void*						m_userptr;
};

enum 
{
	SCALE9LINE_NONE,
	SCALE9LINE_L,
	SCALE9LINE_R,
	SCALE9LINE_T,
	SCALE9LINE_B,
};

class cocos_propctrl_scale9 : public xui_propctrl
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	cocos_propctrl_scale9( xui_propdata* propdata );

	/*
	//destructor
	*/
	virtual ~cocos_propctrl_scale9( void );

	/*
	//create
	*/
	static xui_propctrl*		create					( xui_propdata* propdata );

	/*
	//propdata
	*/
	virtual void				on_linkpropdata			( bool selfupdate = false );
	virtual void				on_editvalue			( xui_propedit* sender );

protected:
	/*
	//override
	*/
	virtual void				on_invalid				( xui_method_args& args );
	virtual void				on_perform				( xui_method_args& args );

	/*
	//event
	*/
	void						on_editpanegetfocus		( xui_component* sender, xui_method_args&  args );
	void						on_editpaneperform		( xui_component* sender, xui_method_args&  args );
	void						on_editpanerenderself	( xui_component* sender, xui_method_args&  args );
	void						on_editpanemousedown	( xui_component* sender, xui_method_mouse& args );
	void						on_editpanemousemove	( xui_component* sender, xui_method_mouse& args );
	void						on_editpanemouserise	( xui_component* sender, xui_method_mouse& args );

	/*
	//method
	*/
	u08							hit_line				( const xui_vector<s32>& screenpt, s32* downline );

	/*
	//member
	*/
	xui_propedit_bool*			m_booledit;
	xui_propedit_number*		m_lnumedit;
	xui_propedit_number*		m_rnumedit;
	xui_propedit_number*		m_tnumedit;
	xui_propedit_number*		m_bnumedit;
	xui_panel*					m_editpane;
	u08							m_dragline;
	s32							m_downline;
};

#endif//__cocos_propdata_scale9_h__