#include "onity_resource.h"

xui_bitmap*	onity_resource::icon_animator	= NULL;
xui_bitmap*	onity_resource::icon_bigerror	= NULL;
xui_bitmap*	onity_resource::icon_bigmessage	= NULL;
xui_bitmap*	onity_resource::icon_bigwarning	= NULL;
xui_bitmap*	onity_resource::icon_center		= NULL;
xui_bitmap*	onity_resource::icon_clear		= NULL;
xui_bitmap*	onity_resource::icon_console	= NULL;
xui_bitmap*	onity_resource::icon_delete		= NULL;
xui_bitmap*	onity_resource::icon_error		= NULL;
xui_bitmap*	onity_resource::icon_file		= NULL;
xui_bitmap*	onity_resource::icon_folder		= NULL;
xui_bitmap*	onity_resource::icon_game		= NULL;
xui_bitmap*	onity_resource::icon_hierarchy	= NULL;
xui_bitmap*	onity_resource::icon_inspector	= NULL;
xui_bitmap*	onity_resource::icon_local		= NULL;
xui_bitmap*	onity_resource::icon_message	= NULL;
xui_bitmap*	onity_resource::icon_pivot		= NULL;
xui_bitmap*	onity_resource::icon_project	= NULL;
xui_bitmap*	onity_resource::icon_rotation	= NULL;
xui_bitmap*	onity_resource::icon_scale		= NULL;
xui_bitmap*	onity_resource::icon_scene		= NULL;
xui_bitmap*	onity_resource::icon_search		= NULL;
xui_bitmap*	onity_resource::icon_select		= NULL;
xui_bitmap*	onity_resource::icon_timeline	= NULL;
xui_bitmap*	onity_resource::icon_translate	= NULL;
xui_bitmap*	onity_resource::icon_warning	= NULL;
xui_bitmap*	onity_resource::icon_world		= NULL;
xui_bitmap* onity_resource::icon_state		= NULL;
xui_bitmap* onity_resource::icon_transition = NULL;

xui_method_explain(onity_resource, init, void)( void )
{
	icon_animator	= xui_bitmap::create("icon/animator.png");
	icon_bigerror	= xui_bitmap::create("icon/bigerror.png");
	icon_bigmessage	= xui_bitmap::create("icon/bigmessage.png");
	icon_bigwarning	= xui_bitmap::create("icon/bigwarning.png");
	icon_center		= xui_bitmap::create("icon/center.png");
	icon_clear		= xui_bitmap::create("icon/clear.png");
	icon_console	= xui_bitmap::create("icon/console.png");
	icon_delete		= xui_bitmap::create("icon/delete.png");
	icon_error		= xui_bitmap::create("icon/error.png");
	icon_file		= xui_bitmap::create("icon/file.png");
	icon_folder		= xui_bitmap::create("icon/folder.png");
	icon_game		= xui_bitmap::create("icon/game.png");
	icon_hierarchy	= xui_bitmap::create("icon/hierarchy.png");
	icon_inspector	= xui_bitmap::create("icon/inspector.png");
	icon_local		= xui_bitmap::create("icon/local.png");
	icon_message	= xui_bitmap::create("icon/message.png");
	icon_pivot		= xui_bitmap::create("icon/pivot.png");
	icon_project	= xui_bitmap::create("icon/project.png");
	icon_rotation	= xui_bitmap::create("icon/rotation.png");
	icon_scale		= xui_bitmap::create("icon/scale.png");
	icon_scene		= xui_bitmap::create("icon/scene.png");
	icon_search		= xui_bitmap::create("icon/search.png");
	icon_select		= xui_bitmap::create("icon/select.png");
	icon_timeline	= xui_bitmap::create("icon/timeline.png");
	icon_translate	= xui_bitmap::create("icon/translate.png");
	icon_warning	= xui_bitmap::create("icon/warning.png");
	icon_world		= xui_bitmap::create("icon/world.png");
	icon_state		= xui_bitmap::create("icon/state.png");
	icon_transition = xui_bitmap::create("icon/transition.png");
}