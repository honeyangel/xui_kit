#include "ui/UIPageView.h"
#include "cocos_propnodepageview.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodepageview)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodepanel(file, node)
, m_pageview(dynamic_cast<cocos2d::ui::PageView*>(node))
{
	add_location	();
	add_anchor		();
	add_position	();
	add_size		(true, def_size);
	add_aligndock	(true);
	add_base		();
	add_touch		();
	add_common		();
	add_color		();
	add_scale9		(get_scale9, set_scale9);
	add_special		();
	add_clip		();
	add_background	();
	add_advance		();
	add_callback	();
	add_userdata	();
}