#ifndef __cocos_propnodeshaderrect_h__
#define __cocos_propnodeshaderrect_h__

#include "cocos_propnodecustom.h"

namespace cocos2d
{
	namespace ui
	{
		class ShaderRect;
	}
}

class cocos_propfile;
class cocos_propnodeshaderrect : public cocos_propnodecustom
{
public:
	/*
	//constructor
	*/
	cocos_propnodeshaderrect( cocos_propcsd* file, cocos2d::Node* node );

	/*
	//method
	*/
	cocos2d::ui::ShaderRect*	get_shaderrect	( void );
	void						add_shaderrect	( void );
	cocos_propfile*				get_matfile		( void );
	void						set_matfile		( cocos_propfile* file );

protected:
	/*
	//static
	*/
	static void*				get_material	( void* userptr );
	static void					set_material	( void* userptr, void* value );

	/*
	//override
	*/
	virtual void				on_propchanged	( xui_component* sender, xui_method_propdata& args );

	/*
	//member
	*/
	cocos_propfile*				m_matfile;
	cocos2d::ui::ShaderRect*	m_shaderrect;
};

#endif//__cocos_propnodeshaderrect_h__