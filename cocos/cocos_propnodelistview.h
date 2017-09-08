#ifndef __cocos_propnodelistview_h__
#define __cocos_propnodelistview_h__

#include "cocos_propnodescroll.h"

namespace cocos2d
{
	namespace ui
	{
		class ListView;
	}
}

class cocos_propnodelistview : public cocos_propnodescroll
{
public:
	/*
	//constructor
	*/
	cocos_propnodelistview( cocos_propcsd* file, cocos2d::Node* node );

	/*
	//method
	*/
	cocos2d::ui::ListView*		get_listview	( void );

protected:
	/*
	//method
	*/
	void						add_listview	( void );

	/*
	//static
	*/
	static s32					get_subalign	( void* userptr );
	static void					set_subalign	( void* userptr, s32 value );
	static f64					get_submargin	( void* userptr );
	static void					set_submargin	( void* userptr, f64 value );

	/*
	//member
	*/
	cocos2d::ui::ListView*		m_listview;
};

#endif//__cocos_propnodelistview_h__