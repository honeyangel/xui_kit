#ifndef __cocos_propnodelayoutcenter_h__
#define __cocos_propnodelayoutcenter_h__

#include "cocos_propnodecustom.h"

namespace cocos2d
{
	namespace ui
	{
		class LayoutCenter;
	}
}

class cocos_propnodelayoutcenter : public cocos_propnodecustom
{
public:
	/*
	//constructor
	*/
	cocos_propnodelayoutcenter( cocos_propcsd* file, cocos2d::Node* node );

	/*
	//method
	*/
	cocos2d::ui::LayoutCenter*	get_layoutcenter	( void );

protected:
	/*
	//method
	*/
	cocos2d::ui::LayoutCenter*	m_layoutcenter;
};

#endif//__cocos_propnodelayoutcenter_h__