#include "LayoutCenter.h"
#include "cocos_propnodelayoutcenter.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodelayoutcenter)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodecustom(file, node)
, m_layoutcenter(dynamic_cast<cocos2d::ui::LayoutCenter*>(node))
{
	add_location	();
	add_anchor		();
	add_position	();
	add_size		(true, NULL);
	add_aligndock	(true);
	add_base		();
	add_touch		();
	add_common		();
	add_color		();
	add_advance		();
	add_callback	();
	add_userdata	();
}