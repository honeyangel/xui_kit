#ifndef __xui_header_h__
#define __xui_header_h__

typedef unsigned char   u08;
typedef   signed char   s08;
typedef unsigned short  u16;
typedef   signed short  s16;
typedef unsigned long   u32;
typedef   signed long   s32;
typedef          float  f32;
typedef          double f64;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <sstream>
#include <algorithm>

#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define xui_pi				3.1415926535897932f
#define xui_pixel_align(x)	( (f32)(s32)(( x ) + (( x ) > 0.0f ? 0.5f : -0.5f)) )
#define xui_min(a, b)		((a) < (b) ? (a) : (b))
#define xui_max(a, b)		((a) > (b) ? (a) : (b))
#define xui_abs(a)			((a) >  0  ? (a) :-(a))

#define xui_create_explain(class_name)					 class_name::class_name
#define xui_delete_explain(class_name)					 class_name::~class_name
#define xui_method_explain(class_name, name, type)	type class_name::name

#define xui_method_ptrcall(ptr,  method)				 ptr->method
#define xui_method_refcall(ref,  method)				 ref.method
#define xui_method_inscall(type, method)				 type::get_ins()->method
#define xui_static_inscall(type, method)				 type::method

#define xui_lstptr_addloop(type, name)	for(std::list<type*>::iterator itor = name.begin(); itor != name.end(); ++itor)
#define xui_lstptr_delloop(type, name)	for(std::list<type*>::reverse_iterator itor = name.rbegin(); itor != name.rend(); ++itor)
#define xui_vecptr_addloop(name)		for(u32 i = 0; i < name.size(); ++i)
#define xui_vecptr_delloop(name)		for(s32 i = (s32)name.size()-1; i >= 0; --i)

class   xui_colour;
class   xui_bitmap;
class   xui_bitmap_buffer;
class   xui_family;
class   xui_family_bitmap;
class   xui_family_member;
class   xui_family_create;
class   xui_convas;
class   xui_action_ctrl;
class   xui_action_ctrl_move;

class   xui_timer;
class   xui_component;
class   xui_control;
class   xui_container;
class   xui_panel;
class   xui_drawer;
class	xui_window;
class   xui_button;
class   xui_toggle;
class   xui_textbox;
class   xui_linebox;
class   xui_gridbox;
class   xui_toolbar;
class	xui_slider;
class	xui_scroll;
class	xui_scrollarrow;
class	xui_scrollthumb;
class   xui_listitem;
class   xui_listview;
class   xui_treedata;
class   xui_treeplus;
class   xui_treenode;
class   xui_treegrid;
class   xui_treedata;
class   xui_treeview;
class   xui_itemtag;
class   xui_dropbox;
class   xui_timedata;
class   xui_timetool;
class   xui_timegrad;
class   xui_timeview;
class   xui_timeline;
class   xui_timehead;
class   xui_timerect;
class   xui_propdata;
class   xui_propkind;
class   xui_proproot;
class   xui_propedit;
class   xui_propctrl;
class   xui_kindctrl;
class   xui_propplus;
class   xui_propview;
class   xui_menu;
class   xui_menuitem;
class   xui_dockpage;
class   xui_dockview;

/*
//rtti
*/
class xui_rtti
{
public:
	/* 
	//constructor 
	*/
	xui_rtti( const char* name, const xui_rtti* base )
	{
		m_name = name;
		m_base = base;
	}
	
	/*
	//method
	*/
	const char*				get_name( void ) const	{ return m_name; }
	const xui_rtti*			get_base( void ) const	{ return m_base; }

protected:
	/*
	//member
	*/
	const char*				m_name;
	const xui_rtti*			m_base;
};

/* 
//root class macro declare
*/
#define xui_declare_root(class_name)													\
	public:																				\
																						\
	static	const xui_rtti	RTTI;														\
    static  const xui_rtti* RTTIPTR	( void )       { return &RTTI; }					\
	virtual const xui_rtti* get_rtti( void ) const { return &RTTI; }					\
																						\
	static	bool			equal	( const xui_rtti* rtti, const class_name* object )	\
	{																					\
		return object == NULL ? false : object->equal(rtti);							\
	}																					\
	bool					equal	( const xui_rtti* rtti ) const						\
	{																					\
		return get_rtti() == rtti;														\
	}																					\
	static	bool			issub	( const xui_rtti* rtti, const class_name* object )	\
	{																					\
		return object == NULL ? false : object->issub(rtti);							\
	}																					\
	bool					issub	( const xui_rtti* rtti ) const						\
	{																					\
		const xui_rtti* temp = get_rtti();												\
		while (temp)																	\
		{																				\
			if (temp == rtti)															\
				return true;															\
																						\
			temp = temp->get_base();													\
		}																				\
		return false;																	\
	}																					\
	static	class_name*		getas	( const xui_rtti* rtti, const class_name* object )	\
	{																					\
		return object == NULL ? NULL : object->getas(rtti);								\
	}																					\
	class_name*				getas	( const xui_rtti* rtti ) const						\
	{																					\
		return issub(rtti) ? (class_name*)this : NULL;									\
	}																																				


/* 
//RTTI class macro declare 
*/
#define xui_declare_rtti																\
	public:																				\
	static	const xui_rtti	RTTI;														\
    static  const xui_rtti* RTTIPTR	( void )											\
	{																					\
		return &RTTI;																	\
	}																					\
																						\
	virtual const xui_rtti*	get_rtti( void ) const										\
	{																					\
		return &RTTI;																	\
	}

/*
//RTTI class macro implement
*/
#define xui_implement_root(class_name)				const xui_rtti class_name::RTTI(#class_name, NULL)
#define xui_implement_rtti(class_name, base_name)	const xui_rtti class_name::RTTI(#class_name, &base_name::RTTI)

/*
//RTTI macro
*/
#define xui_equal_kindof(class_name, object)               class_name::equal(&class_name::RTTI, object)
#define xui_issub_kindof(class_name, object)               class_name::issub(&class_name::RTTI, object)
#define xui_dynamic_cast(class_name, object) ((class_name*)class_name::getas(&class_name::RTTI, object))

/* 
//Instance 
*/
#define xui_declare_instance(class_name)												\
	public:																				\
	static	void			init	( void );											\
	static	void			done	( void );											\
	static	class_name*		get_ins	( void )											\
	{																					\
		return INSTANCE;																\
	}																					\
																						\
	protected:																			\
	static	class_name*		INSTANCE;

#define xui_implement_instance_member(class_name)	class_name* class_name::INSTANCE = NULL
#define xui_implement_instance_method(class_name)										\
	void	class_name::init		( void )											\
	{																					\
		INSTANCE = new class_name;														\
	}																					\
	void	class_name::done		( void )											\
	{																					\
		delete INSTANCE;																\
	}

#endif//__xui_header_h__