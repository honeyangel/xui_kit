#ifndef __cocos_propdata_shader_h__
#define __cocos_propdata_shader_h__

#include "xui_propdata.h"

class cocos_propdata_shader : public xui_propdata_object_func
{
public:
	/*
	//constructor
	*/
	cocos_propdata_shader( 
		xui_propkind*		kind, 
		const std::wstring& name, 
		const std::string&	droptype,
		get_func			userget, 
		set_func			userset, 
		void*				userptr );

protected:
	/*
	//event
	*/
	void					on_doubleclick	( xui_component* sender, xui_method_args& args );

	/*
	//static
	*/
	static xui_bitmap*		get_icon		( xui_propdata* propdata );
	static std::wstring		get_name		( xui_propdata* propdata );
};

class cocos_propdata_vertshader : public cocos_propdata_shader
{
public:
	/*
	//constructor
	*/
	cocos_propdata_vertshader( 
		xui_propkind*		kind, 
		const std::wstring& name, 
		get_func			userget, 
		set_func			userset, 
		void*				userptr )
	: cocos_propdata_shader(kind, name, "VertShader", userget, userset, userptr)
	{}
};

class cocos_propdata_fragshader : public cocos_propdata_shader
{
public:
	/*
	//constructor
	*/
	cocos_propdata_fragshader( 
		xui_propkind*		kind, 
		const std::wstring& name, 
		get_func			userget, 
		set_func			userset, 
		void*				userptr )
	: cocos_propdata_shader(kind, name, "FragShader", userget, userset, userptr)
	{}
};

#endif//__cocos_propdata_shader_h__