#ifndef __cocos_propnodepageview_h__
#define __cocos_propnodepageview_h__

#include "cocos_propnodepanel.h"

class cocos_propnodepageview : public cocos_propnodepanel
{
public:
	/*
	//constructor
	*/
	cocos_propnodepageview( cocos_propcsd* file, cocos2d::Node* node );

protected:
	/*
	//member
	*/
	cocos2d::ui::PageView*	m_pageview;
};

#endif//__cocos_propnodepageview_h__