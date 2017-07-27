#ifndef __cocos_propctrl_asset_h__
#define __cocos_propctrl_asset_h__

#include "xui_propdata.h"
#include "xui_propctrl_object.h"

//enum 
//{
//	DROPTYPE_IMAGE = 0x01,
//	DROPTYPE_FRAME = 0x02,
//	DROPTYPE_ACTOR = 0x04,
//};

class cocos_propctrl_asset : public xui_propctrl_object
{
	xui_declare_rtti

public:
	/*
	//static
	*/
	static xui_propctrl*		create					( xui_propdata* propdata );

	/*
	//constructor
	*/
	cocos_propctrl_asset( xui_propdata* propdata );

protected:
	/*
	//event
	*/
	void						on_editctrlmouseenter	( xui_component* sender, xui_method_mouse& args );
	void						on_editctrlmouseleave	( xui_component* sender, xui_method_mouse& args );
};

//class onity_propdata_particle : public xui_propdata_object_func
//{
//public:
//	/*
//	//constructor
//	*/
//	onity_propdata_particle( 
//		xui_propkind*			kind,
//		const std::wstring&		name,
//		get_func				userget,
//		set_func				userset,
//		void*					userptr );
//
//protected:
//	/*
//	//event
//	*/
//	void						on_doubleclick			( xui_component* sender, xui_method_args& args );
//
//	/*
//	//static
//	*/
//	static xui_bitmap*			get_icon				( xui_propdata* propdata );
//	static std::wstring			get_name				( xui_propdata* propdata );
//};

//class onity_propdata_2dsasset : public xui_propdata_object_func
//{
//public:
//	/*
//	//constructor
//	*/
//	onity_propdata_2dsasset( 
//		xui_propkind*			kind,
//		const std::wstring&		name,
//		xui_prop_newctrl		func,
//		u32						droptype,
//		get_func				userget,
//		set_func				userset,
//		void*					userptr );
//
//protected:
//	/*
//	//event
//	*/
//	void						on_doubleclick			( xui_component* sender, xui_method_args& args );
//
//	/*
//	//static
//	*/
//	static xui_bitmap*			get_icon				( xui_propdata* propdata );
//	static std::wstring			get_name				( xui_propdata* propdata );
//};

//class onity_propdata_transref : public onity_propdata_2dsasset
//{
//public:
//	/*
//	//typedef
//	*/
//	typedef xui_proproot_vec	(*get_total)( void* userptr );
//
//	/*
//	//constructor
//	*/
//	onity_propdata_transref(
//		xui_propkind*			kind,
//		const std::wstring&		name,
//		xui_prop_newctrl		func,
//		u32						droptype,
//		onity_proptransref*		proptransref,
//		get_total				usergettotal,
//		void*					userptrtotal );
//
//	/*
//	//method
//	*/
//	onity_proptransref*			get_proptransref		( void );
//	xui_proproot_vec			get_proptotal			( void );
//
//protected:
//	/*
//	//static
//	*/
//	static void*				get_asset				( void* userptr );
//	static void					set_asset				( void* userptr, void* value );		
//
//	/*
//	//member
//	*/
//	onity_proptransref*			m_proptransref;
//	get_total					m_usergettotal;
//	void*						m_userptrtotal;
//};
//
//class onity_propctrl_transref : public onity_propctrl_asset
//{
//	xui_declare_rtti
//
//public:
//	/*
//	//static
//	*/
//	static xui_propctrl*		create					( xui_propdata* propdata );
//
//	/*
//	//constructor
//	*/
//	onity_propctrl_transref( xui_propdata* propdata );
//
//protected:
//	/*
//	//event
//	*/
//	void						on_editctrlclick		( xui_component* sender, xui_method_mouse& args );
//};

#endif//__cocos_propctrl_asset_h__