#include "NP2DSImageFileMgr.h"
#include "NP2DSFrameFileMgr.h"
#include "NP2DSActorFileMgr.h"
#include "NP2DSAssetFile.h"
#include "NP2DSImageFile.h"
#include "NP2DSFrameFile.h"
#include "NP2DSActorFile.h"
#include "NP2DSAsset.h"

#include "xui_global.h"
#include "xui_textbox.h"
#include "xui_propview.h"
#include "xui_toggle.h"
#include "xui_menuitem.h"
#include "xui_menu.h"
#include "xui_toggle.h"
#include "onity_prop2dsres.h"
#include "onity_pathdata.h"
#include "onity_proppath.h"
#include "onity_pathctrl.h"

xui_implement_rtti(onity_pathctrl, xui_kindctrl);

/*
//constructor
*/
xui_create_explain(onity_pathctrl)( xui_propkind* propkind )
: xui_kindctrl(propkind)
{}

/*
//static
*/
xui_method_explain(onity_pathctrl, create,				xui_kindctrl*	)( xui_propkind* propkind )
{
	return new onity_pathctrl(propkind);
}