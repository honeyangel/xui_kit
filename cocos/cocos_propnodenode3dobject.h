#ifndef __cocos_propnodenode3dobject_h__
#define __cocos_propnodenode3dobject_h__

#include "cocos_propnodecustom.h"

namespace cocos2d
{
	namespace ui
	{
		class Node3DObject;
	}
}

class cocos_propfile;
class cocos_propnodenode3dobject : public cocos_propnodecustom
{
public:
	/*
	//constructor
	*/
	cocos_propnodenode3dobject( cocos_propcsd* file, cocos2d::Node* node );

	/*
	//method
	*/
	cocos2d::ui::Node3DObject*	get_node3dobject	( void );
	void						add_node3dobject	( void );
	cocos_propfile*				get_c3bfile			( void );
	void						set_c3bfile			( cocos_propfile* file );
	cocos_propfile*				get_matfile			( void );
	void						set_matfile			( cocos_propfile* file );

protected:
	/*
	//static
	*/
	static void*				get_mesh			( void* userptr );
	static void					set_mesh			( void* userptr, void* value );
	static void*				get_material		( void* userptr );
	static void					set_material		( void* userptr, void* value );
	static bool					was_playanim		( void* userptr );
	static void					set_playanim		( void* userptr, bool  value );
	static f64					get_position3dx		( void* userptr );
	static void					set_position3dx		( void* userptr, f64   value );
	static f64					get_position3dy		( void* userptr );
	static void					set_position3dy		( void* userptr, f64   value );
	static f64					get_position3dz		( void* userptr );
	static void					set_position3dz		( void* userptr, f64   value );
	static f64					get_rotation3dx		( void* userptr );
	static void					set_rotation3dx		( void* userptr, f64   value );
	static f64					get_rotation3dy		( void* userptr );
	static void					set_rotation3dy		( void* userptr, f64   value );
	static f64					get_rotation3dz		( void* userptr );
	static void					set_rotation3dz		( void* userptr, f64   value );
	static f64					get_scale3dx		( void* userptr );
	static void					set_scale3dx		( void* userptr, f64   value );
	static f64					get_scale3dy		( void* userptr );
	static void					set_scale3dy		( void* userptr, f64   value );
	static f64					get_scale3dz		( void* userptr );
	static void					set_scale3dz		( void* userptr, f64   value );

	/*
	//member
	*/
	cocos2d::ui::Node3DObject*	m_node3dobject;
	cocos_propfile*				m_c3bfile;
	cocos_propfile*				m_matfile;
};

#endif//__cocos_propnodenode3dobject_h__