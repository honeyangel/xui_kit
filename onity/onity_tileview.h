#ifndef __onity_tileview_h__
#define __onity_tileview_h__

#include "xui_control.h"

class NP2DSImage;
class NP2DSFrame;
class NP2DSActor;
class onity_filedata;
class onity_propfile;
class onity_renderview;
class onity_tileview : public xui_control
{
	xui_declare_rtti

public:
	static const s32		name_size = 20;
	static const s32		vert_grap = 10;

	/*
	//constructor
	*/
	onity_tileview( void );

	/*
	//method
	*/
	xui_treeview*			get_lineview				( void );
	onity_renderview*		get_drawview				( void );
	xui_scroll*				get_viewroll				( void );
	xui_treenode*			get_viewfile				( void );
	void					set_viewfile				( xui_treenode* viewfile );
	void					set_tilesize				( s32 size );
	void					get_tileinfo				( s32& s, s32& c, s32& g, s32& w, s32& h );
	xui_treenode*			get_tilenode				( const xui_vector<s32>& mouse, xui_rect2d<s32>& tilert );
	void					set_tilevisible				( xui_treenode* node );

protected:
	/*
	//event
	*/
	void					on_drawviewinvalid			( xui_component* sender, xui_method_args&	  args );
	void					on_drawviewrenderself		( xui_component* sender, xui_method_args&	  args );
	//void					on_drawviewkeybddown		( xui_component* sender, xui_method_keybd&	  args );
	void					on_drawviewmousedown		( xui_component* sender, xui_method_mouse&	  args );
	void					on_drawviewmouseclick		( xui_component* sender, xui_method_mouse&	  args );
	void					on_drawviewmousewheel		( xui_component* sender, xui_method_mouse&	  args );
	void					on_drawviewmousedoubleclick	( xui_component* sender, xui_method_mouse&	  args );
	void					on_drawviewmousedragitem	( xui_component* sender, xui_method_dragdrop& args );

	/*
	//method
	*/
	void					draw_file					( const xui_rect2d<s32>& rt, xui_treenode* node );
	void					draw_icon					( const xui_rect2d<s32>& rt, xui_treenode* node );
	void					draw_name					( const xui_rect2d<s32>& rt, xui_treenode* node );
	void					draw_leaf					( const xui_rect2d<s32>& rt, xui_treenode* node );
	void					draw_texture				( const xui_rect2d<s32>& rt, xui_treenode* node );
	void					draw_image					( const xui_rect2d<s32>& rt, NP2DSImage* image );
	void					draw_frame					( const xui_rect2d<s32>& rt, NP2DSFrame* frame );
	void					draw_actor					( const xui_rect2d<s32>& rt, NP2DSActor* actor );
	void					draw_node					( s32 ic, s32 ir, s32 x, s32 y, s32 w, s32 s, xui_treenode* node );
	void					draw_background				( s32 ic, s32 ir, s32 c, s32 g, s32 w, s32 h, u32 count );
	std::wstring			trim_string					( const std::wstring& text, const xui_family& font, s32 maxwidth );

	/*
	//member
	*/
	s32						m_tilesize;
	xui_treenode*			m_viewfile;
	onity_renderview*		m_drawview;
	xui_scroll*				m_viewroll;
};

#endif//__onity_tileview_h__