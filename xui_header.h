#ifndef __xui_header_h__
#define __xui_header_h__

#define xui_platform_unknown    0
#define xui_platform_win32		1
#define xui_platform_macos		2

#if defined(__APPLE__)
#define xui_target_platform     xui_platform_macos
#endif

#if (defined(_WIN32) || defined(__WIN32__) || defined(WIN32))
#define xui_target_platform     xui_platform_win32
#endif

#ifndef SIMPLE_TYPE_U08
#define SIMPLE_TYPE_U08
typedef unsigned char   u08;
#endif

#ifndef SIMPLE_TYPE_S08
#define SIMPLE_TYPE_S08
typedef   signed char   s08;
#endif

#ifndef SIMPLE_TYPE_U16
#define SIMPLE_TYPE_U16
typedef unsigned short  u16;
#endif

#ifndef SIMPLE_TYPE_S16
#define SIMPLE_TYPE_S16
typedef   signed short  s16;
#endif

#ifndef SIMPLE_TYPE_U32
#define SIMPLE_TYPE_U32
typedef unsigned int    u32;
#endif

#ifndef SIMPLE_TYPE_S32
#define SIMPLE_TYPE_S32
typedef   signed int    s32;
#endif

#ifndef SIMPLE_TYPE_F32
#define SIMPLE_TYPE_F32
typedef          float  f32;
#endif

#ifndef SIMPLE_TYPE_F64
#define SIMPLE_TYPE_F64
typedef          double f64;
#endif

#include <assert.h>
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
#include <functional>

#if   xui_target_platform == xui_platform_win32
#include <tchar.h>
#include <WinSock2.h>
#include <Windows.h>
#include <Shlobj.h>
#include <shellapi.h>
#include <timeapi.h>
#include <glew.h>
#elif xui_target_platform == xui_platform_macos
#define GL_SILENCE_DEPRECATION
#import <OpenGL/gl.h>
#endif

#define xui_pi				3.1415926535897932f
#define xui_pixel_align(x)	( (f32)(s32)(( x ) + (( x ) > 0.0f ? 0.5f : -0.5f)) )
#define xui_min(a, b)		((a) < (b) ? (a) : (b))
#define xui_max(a, b)		((a) > (b) ? (a) : (b))
#define xui_abs(a)			((a) >  0  ? (a) :-(a))

//#define xui_create_explain(class_name)					 class_name::class_name
//#define xui_delete_explain(class_name)					 class_name::~class_name
//#define xui_method_explain(class_name, name, type)	type class_name::name

#define xui_method_ptrcall(ptr,  method)				 ptr->method
#define xui_method_refcall(ref,  method)				 ref.method
#define xui_method_inscall(type, method)				 type::get_ins()->method
#define xui_static_inscall(type, method)				 type::method

class   xui_colour;
class   xui_bitmap;
class   xui_bitmap_buffer;
class   xui_family;
class   xui_family_bitmap;
class   xui_family_member;
class   xui_family_create;
class   xui_convas;
class	xui_syswnd;
class	xui_render_window;

class   xui_timer;
class   xui_component;
class   xui_control;
class   xui_container;
class   xui_panel;
class   xui_drawer;
class	xui_window;
class	xui_dialog;
class   xui_button;
class   xui_toggle;
class   xui_textbox;
class	xui_numbbox;
class   xui_linebox;
class   xui_gridbox;
class   xui_toolbar;
class	xui_separate;
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
class   xui_plusctrl;
class   xui_propview;
class   xui_menu;
class   xui_menuitem;
class   xui_dockpage;
class   xui_dockview;
class   xui_native_window;

class xui_rtti
{
public:
	xui_rtti( const char* name, const xui_rtti* base )
	{
		m_name = name;
		m_base = base;
	}
	
	const char*				get_name( void ) const	{ return m_name; }
	const xui_rtti*			get_base( void ) const	{ return m_base; }

protected:
	const char*				m_name;
	const xui_rtti*			m_base;
};

#define xui_declare_root(class_name)													\
	public:																				\
																						\
	static	const xui_rtti	k_rtti;														\
    static  const xui_rtti* ptr_rtti( void )       { return &k_rtti; }					\
	virtual const xui_rtti* get_rtti( void ) const { return &k_rtti; }					\
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


#define xui_declare_rtti																\
	public:																				\
	static	const xui_rtti	k_rtti;														\
																						\
	virtual const xui_rtti*	get_rtti( void ) const										\
	{																					\
		return &k_rtti;																	\
	}

#define xui_implement_root(class_name)				const xui_rtti class_name::k_rtti(#class_name, NULL);
#define xui_implement_rtti(class_name, base_name)	const xui_rtti class_name::k_rtti(#class_name, &base_name::k_rtti);


#define xui_equal_kindof(class_name, object)               class_name::equal(&class_name::k_rtti, object)
#define xui_issub_kindof(class_name, object)               class_name::issub(&class_name::k_rtti, object)
#define xui_dynamic_cast(class_name, object) ((class_name*)class_name::getas(&class_name::k_rtti, object))

#define xui_declare_instance(class_name)												\
	public:																				\
	static	void			init	( void );											\
	static	void			done	( void );											\
	static	class_name*		get_ins	( void )											\
	{																					\
		return s_instance;																\
	}																					\
																						\
	protected:																			\
	static	class_name*		s_instance;

#define xui_implement_instance_member(class_name)	class_name* class_name::s_instance = NULL;
#define xui_implement_instance_method(class_name)										\
	void	class_name::init		( void )											\
	{																					\
		s_instance = new class_name;												    \
	}																					\
	void	class_name::done		( void )											\
	{																					\
		delete s_instance;																\
	}

#endif//__xui_header_h__
