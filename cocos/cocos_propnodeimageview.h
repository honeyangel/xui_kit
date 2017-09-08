#ifndef __cocos_propnodeimageview_h__
#define __cocos_propnodeimageview_h__

#include "cocos_propnodewidget.h"
#include "cocos_propdata_scale9.h"

namespace cocos2d
{
	namespace ui
	{
		class ImageView;
	}
}

class cocos_propnodeimageview : public cocos_propnodewidget
{
	friend class cocos_hierarchy;

public:
	/*
	//constructor
	*/
	cocos_propnodeimageview( cocos_propcsd* file, cocos2d::Node* node );

	/*
	//method
	*/
	cocos2d::ui::ImageView*		get_imageview		( void );

protected:
	/*
	//method
	*/
	void						add_background		( void );

	/*
	//static
	*/
	static cocos_value_unitvec	def_size			( void* userptr );
	static void*				get_background		( void* userptr );
	static void					set_background		( void* userptr, void* value );
	static cocos_value_scale9	get_scale9			( void* userptr );
	static void					set_scale9			( void* userptr, const cocos_value_scale9& value );

	/*
	//member
	*/
	cocos2d::ui::ImageView*		m_imageview;
};

#endif//__cocos_propnodeimageview_h__