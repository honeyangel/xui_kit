#ifndef __cocos_propnodeobject3d_h__
#define __cocos_propnodeobject3d_h__

#include "cocos_propnodecustom.h"

namespace cocos2d
{
	namespace ui
	{
		class Object3D;
	}
}

class cocos_propfile;
class cocos_propnodeobject3d : public cocos_propnodecustom
{
public:
	/*
	//constructor
	*/
	cocos_propnodeobject3d( cocos_propcsd* file, cocos2d::Node* node );

	/*
	//method
	*/
	cocos2d::ui::Object3D*	get_object3d	( void );
	void					add_object3d	( void );
	cocos_propfile*			get_c3bfile		( void );
	void					set_c3bfile		( cocos_propfile* file );

protected:
	/*
	//static
	*/
	static void*			get_mesh		( void* userptr );
	static void				set_mesh		( void* userptr, void* value );
	static bool				was_playanim	( void* userptr );
	static void				set_playanim	( void* userptr, bool  value );
	static f64				get_position3dx	( void* userptr );
	static void				set_position3dx	( void* userptr, f64   value );
	static f64				get_position3dy	( void* userptr );
	static void				set_position3dy	( void* userptr, f64   value );
	static f64				get_position3dz	( void* userptr );
	static void				set_position3dz	( void* userptr, f64   value );
	static f64				get_rotation3dx	( void* userptr );
	static void				set_rotation3dx	( void* userptr, f64   value );
	static f64				get_rotation3dy	( void* userptr );
	static void				set_rotation3dy	( void* userptr, f64   value );
	static f64				get_rotation3dz	( void* userptr );
	static void				set_rotation3dz	( void* userptr, f64   value );
	static f64				get_scale3dx	( void* userptr );
	static void				set_scale3dx	( void* userptr, f64   value );
	static f64				get_scale3dy	( void* userptr );
	static void				set_scale3dy	( void* userptr, f64   value );
	static f64				get_scale3dz	( void* userptr );
	static void				set_scale3dz	( void* userptr, f64   value );

	/*
	//member
	*/
	cocos2d::ui::Object3D*	m_object3d;
	cocos_propfile*			m_c3bfile;
};

#endif//__cocos_propnodeobject3d_h__