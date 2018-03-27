#include "xui_kindctrl.h"
#include "xui_propkind.h"
#include "xui_propctrl_vector.h"
#include "xui_textbox.h"
#include "cocos_resource.h"
#include "cocos_boundbox.h"
#include "cocos_propctrl_location.h"
#include "cocos_propnodereal.h"

/*
//constructor
*/
xui_create_explain(cocos_propnodereal)( cocos_propcsd* file, cocos2d::Node* node )
: cocos_propnodebase(cocos_resource::icon_local, file, node)
{
	add_location	();
	add_position	();
	add_aligndock	(false);
	add_base		();
	add_common		();
	add_color		();
	add_advance		();
	add_userdata	();

	m_boundbox->add_operator(OP_SCALE);
	m_boundbox->set_ignoresz(true);
}
