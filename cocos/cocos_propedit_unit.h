#ifndef __cocos_propedit_unit_h__
#define __cocos_propedit_unit_h__

#include "xui_propedit.h"

class cocos_value_unit
{
public:
	cocos_value_unit( void )
	: pix(0.0)
	, per(0.0)
	, useper(false)
	{}
	cocos_value_unit( f64 _pix, f64 _per, bool _useper )
	: pix(_pix)
	, per(_per)
	, useper(_useper)
	{}

	bool operator == ( const cocos_value_unit& other )
	{
		return (useper == other.useper && pix == other.pix && per == other.per);
	}
	bool operator != ( const cocos_value_unit& other )
	{
		return (useper != other.useper || pix != other.pix || per != other.per);
	}

	f64		pix;
	f64		per;
	bool	useper;
};

class cocos_propedit_unit : public xui_propedit_base
{
public:
	/*
	//constructor
	*/
	cocos_propedit_unit( xui_propctrl* propctrl );

	/*
	//method
	*/
	cocos_value_unit		get_value				( void );
	void					set_value				( const cocos_value_unit& value );
	xui_toggle*				get_menuctrl			( void );
	f64						get_textnumb			( bool byname );
	void					set_textnumb			( f64 value, bool toname );

	/*
	//override
	*/
	virtual void			reset					( void );

	/*
	//event
	*/
	void					on_textctrltextchanged	( xui_component* sender, xui_method_args&  args );
	//void					on_textctrlmousewheel	( xui_component* sender, xui_method_mouse& args );
	void					on_namectrlmousemove	( xui_component* sender, xui_method_mouse& args );
	void					on_menuitemclick		( xui_component* sender, xui_method_args&  args );

protected:
	/*
	//member
	*/
	xui_toggle*				m_menuctrl;
	xui_menuitem*			m_pixcheck;
	xui_menuitem*			m_percheck;
};

#endif//__cocos_propedit_unit_h__