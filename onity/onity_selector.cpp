#include "xui_toggle.h"
#include "xui_textbox.h"
#include "xui_linebox.h"
#include "xui_slider.h"
#include "xui_desktop.h"
#include "onity_resource.h"
#include "onity_fileview.h"
#include "onity_preview.h"
#include "onity_selector.h"

xui_implement_rtti(onity_selector, xui_pickwnd);

/*
//constructor
*/
xui_create_explain(onity_selector)( void )
: xui_pickwnd()
{
	m_title = new xui_drawer(xui_vector<s32>(24));
	xui_method_ptrcall(m_title,		ini_component		)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_title,		ini_drawer			)(L"Select");
	xui_method_ptrcall(m_title,		set_textalign		)(TEXTALIGN_LT);

	m_search = new xui_textbox(xui_vector<s32>(100, 20));
	//xui_method_ptrcall(m_search,	xm_textchanged	) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_searchtextchanged);
	//xui_method_ptrcall(m_search,	xm_textenter	) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_searchtextenter);
	xui_method_ptrcall(m_search,	ini_component		)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_search,	ini_drawer			)(onity_resource::icon_search);
	xui_method_ptrcall(m_search,	set_backcolor		)(xui_colour(1.0f, 0.20f));
	xui_method_ptrcall(m_search,	set_drawcolor		)(true);
	xui_method_ptrcall(m_search,	set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_search,	set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_search,	set_textoffset		)(xui_vector<s32>(2, 0));
	xui_method_ptrcall(m_search,	set_borderrt		)(xui_rect2d<s32>(8, 2, 24, 2));
	xui_method_ptrcall(m_search,	set_corner			)(10);

	m_asset		= new xui_toggle(xui_vector<s32>(60, 24), TOGGLE_BUTTON);
	//xui_method_ptrcall(m_asset,		xm_toggleclick		) += new xui_method_member<xui_method_args, onity_recent>(this, &onity_recent::on_buttonclick);
	xui_method_ptrcall(m_asset,		set_corner			)(3);
	xui_method_ptrcall(m_asset,		set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_asset,		set_drawcolor		)(true);
	xui_method_ptrcall(m_asset,		set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_asset,		set_textalign		)(TEXTALIGN_CC);
	xui_method_ptrcall(m_asset,		ini_drawer			)(L"Asset");

	m_scene		= new xui_toggle(xui_vector<s32>(60, 24), TOGGLE_BUTTON);
	xui_method_ptrcall(m_scene,		set_corner			)(3);
	xui_method_ptrcall(m_scene,		set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_scene,		set_drawcolor		)(true);
	xui_method_ptrcall(m_scene,		set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_scene,		set_textalign		)(TEXTALIGN_CC);
	xui_method_ptrcall(m_scene,		ini_drawer			)(L"Scene");

	xui_linebox* linebox = new xui_linebox(xui_vector<s32>(24), FLOWSTYLE_H);
	xui_method_ptrcall(linebox,		ini_component		)(ALIGNHORZ_L, ALIGNVERT_C, 0);
	xui_method_ptrcall(linebox,		set_corner			)(3);
	xui_method_ptrcall(linebox,		set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(linebox,		add_linectrl		)(m_asset);
	xui_method_ptrcall(linebox,		add_linectrl		)(m_scene);

	m_slider	= new xui_slider(xui_vector<s32>(64, 16), FLOWSTYLE_H, ARROWDRAW_NONE);
	xui_method_ptrcall(m_slider,	set_backcolor		)(xui_colour(1.0f, 0.2f));
	xui_method_ptrcall(m_slider,	set_borderrt		)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_slider,	ini_component		)(ALIGNHORZ_R, ALIGNVERT_C, 0);
	xui_method_ptrcall(m_slider,	ini_scroll			)(60, 0);
	//xui_method_ptrcall(m_slider,	xm_scroll		) += new xui_method_member<xui_method_args,		onity_project>(this, &onity_project::on_sliderscroll);

	m_middle	= new xui_panel(xui_vector<s32>(32));
	xui_method_ptrcall(m_middle,	ini_component		)(0, 0, DOCKSTYLE_T);
	xui_method_ptrcall(m_middle,	set_drawcolor		)(false);
	xui_method_ptrcall(m_middle,	add_child			)(linebox);
	xui_method_ptrcall(m_middle,	add_child			)(m_slider);

	m_fileview	= new onity_fileview;
	//xui_method_ptrcall(m_fileview,	xm_fileviewnodeclick) += new xui_method_member<xui_method_mouse,	onity_project>(this, &onity_project::on_fileviewnodeclick);
	//xui_method_ptrcall(m_fileview,	xm_fileviewdoubleclk) += new xui_method_member<xui_method_mouse,	onity_project>(this, &onity_project::on_fileviewdoubleclk);
	//xui_method_ptrcall(m_fileview,	xm_fileviewassetdrag) += new xui_method_member<xui_method_dragdrop, onity_project>(this, &onity_project::on_fileviewassetdrag);
	xui_method_ptrcall(m_fileview,	ini_component		)(0, 0, DOCKSTYLE_F);
	xui_method_ptrcall(m_fileview,	set_sidestyle		)(SIDESTYLE_S);

	m_sizectrl	= new xui_control(xui_vector<s32>(4));
	//xui_method_ptrcall(m_sizectrl,	xm_mousemove		) += new xui_method_member<xui_method_mouse,	onity_project>(this, &onity_project::on_sizectrlmousemove);
	xui_method_ptrcall(m_sizectrl,	ini_component		)(0, 0, DOCKSTYLE_U);
	xui_method_ptrcall(m_sizectrl,	set_cursor			)(CURSOR_WE);

	m_drawview	= new onity_preview;
	xui_method_ptrcall(m_drawview,	set_backcolor		)(xui_colour(1.0f, 0.15f));
	xui_method_ptrcall(m_drawview,	set_drawcolor		)(true);
	xui_method_ptrcall(m_drawview,	set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_drawview,	ini_component		)(0, 0, DOCKSTYLE_L);

	m_accept	= new xui_button(xui_vector<s32>(128, 24));
	//xui_method_ptrcall(m_accept,	xm_buttonclick		) += new xui_method_member<xui_method_args, onity_recent>(this, &onity_recent::on_buttonclick);
	xui_method_ptrcall(m_accept,	set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_accept,	set_corner			)(3);
	xui_method_ptrcall(m_accept,	set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_accept,	set_drawcolor		)(true);
	xui_method_ptrcall(m_accept,	set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_accept,	set_textalign		)(TEXTALIGN_CC);
	xui_method_ptrcall(m_accept,	ini_drawer			)(L"Accept");

	m_cancel	= new xui_button(xui_vector<s32>(128, 24));
	//xui_method_ptrcall(m_browse, xm_buttonclick		) += new xui_method_member<xui_method_args, onity_recent>(this, &onity_recent::on_buttonclick);
	xui_method_ptrcall(m_cancel,	set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_cancel,	set_corner			)(3);
	xui_method_ptrcall(m_cancel,	set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_cancel,	set_drawcolor		)(true);
	xui_method_ptrcall(m_cancel,	set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_cancel,	set_textalign		)(TEXTALIGN_CC);
	xui_method_ptrcall(m_cancel,	ini_component		)(ALIGNHORZ_R, ALIGNVERT_B, 0);
	xui_method_ptrcall(m_cancel,	ini_drawer			)(L"Cancel");

	m_bottom	= new xui_panel(xui_vector<s32>(128, 176));
	xui_method_ptrcall(m_bottom,	ini_component		)(0, 0, DOCKSTYLE_B);
	xui_method_ptrcall(m_bottom,	set_borderrt		)(xui_rect2d<s32>(0, 4, 0, 0));
	xui_method_ptrcall(m_bottom,	add_child			)(m_drawview);
	//xui_method_ptrcall(m_bottom,	add_child			)(m_accept);
	xui_method_ptrcall(m_bottom,	add_child			)(m_cancel);

	add_child(m_title);
	add_child(m_search);
	add_child(m_middle);
	add_child(m_bottom);
	add_child(m_fileview);
}

/*
//static
*/
xui_method_explain(onity_selector, get_ptr,		xui_pickwnd*)( void )
{
	static xui_pickwnd* ptr = NULL;
	if (ptr == NULL)
	{
		ptr = new onity_selector;
		xui_method_ptrcall(ptr, set_sidestyle	)(SIDESTYLE_S);
		xui_method_ptrcall(ptr, set_rendersz	)(xui_vector<s32>(800, 600));
		xui_method_ptrcall(ptr, set_renderpt	)(xui_vector<s32>(200,  100));
		xui_method_ptrcall(ptr, set_borderrt	)(xui_rect2d<s32>(8));
		xui_method_ptrcall(ptr, ini_component	)(true, false);
		xui_desktop::get_ins()->add_child(ptr);
	}

	return ptr;
}

/*
//override
*/
xui_method_explain(onity_selector, get_value,	void*		)( void )
{
	return NULL;
}
xui_method_explain(onity_selector, set_value,	void		)( void* value )
{

}