#ifndef __cocos_propnodesprite_h__
#define __cocos_propnodesprite_h__

#include "cocos_propnodebase.h"
#include "cocos_propdata_blend.h"

namespace cocos2d
{
	class Sprite;
}

class cocos_propnodesprite : public cocos_propnodebase
{
public:
	/*
	//constructor
	*/
	cocos_propnodesprite( cocos_propcsd* file, cocos2d::Node* node );

	/*
	//method
	*/
	cocos2d::Sprite*			get_sprite		( void );

protected:
	/*
	//method
	*/
	void						add_background	( void );

	/*
	//static
	*/
	static void*				get_background	( void* userptr );
	static void					set_background	( void* userptr, void* value );
	static cocos_blend_value	get_blendfunc	( void* userptr );
	static void					set_blendfunc	( void* userptr, const cocos_blend_value& value );
	//static cocos_value_unitvec	def_size		( void* userptr );

	/*
	//member
	*/
	cocos2d::Sprite*			m_sprite;
};

#endif//__cocos_propnodesprite_h__