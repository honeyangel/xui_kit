#ifndef __xui_treeview_h__
#define __xui_treeview_h__

#include "xui_canvas.h"
#include "xui_toggle.h"
#include "xui_plusctrl.h"
#include "xui_container.h"

enum
{
	k_treecolumn_main,
	k_treecolumn_text,
	k_treecolumn_icon,
	k_treecolumn_bool,
};

enum
{
	k_treedrop_notallow,
	k_treedrop_front,
	k_treedrop_inner,
	k_treedrop_after,
};

enum
{
	k_treesort_none,
	k_treesort_increment,
	k_treesort_decrement,
};

enum
{
	k_sorttype_none,
	k_sorttype_string,
	k_sorttype_number,
};

xui_declare_args(treedragdrop)
{
public:
	xui_treenode*		dragnode;
	xui_treenode*		dropnode;
	u08					allowplace;

	xui_method_treedragdrop( void )
	{
		dragnode	= NULL;
		dropnode	= NULL;
		allowplace	= k_treedrop_inner;
	}
};

class xui_treecolumn
{
public:
	u08							type;
	s32							size;
	std::wstring				name;
	xui_bitmap*					icon;
	u08							sort;
	s32							minsize;
	s32							maxsize;
	bool						editable;
	xui_rect2d<s32>				borderrt;
	xui_family					textfont;
	xui_family_render			textdraw;
	u08							boolmode;
	u08							textalign;
	u08							iconalign;

	xui_treecolumn( 
		u08						 _type, 
		s32						 _size, 
		const std::wstring&		 _name, 
		xui_bitmap*				 _icon		= NULL, 
		u08						 _sort		= 1,
		bool					 _editable	= false,
		u08						 _boolmode  = k_toggle_circle,
		const xui_rect2d<s32>&	 _borderrt  = xui_rect2d<s32>(2),
		u08                      _iconalign = k_image_c,
		u08                      _textalign = k_textalign_lc )
	{
		type		= _type;
		size		= _size;
		name		= _name;
		icon		= _icon;
		sort		= _sort;
		minsize		= _size;
		maxsize		= _size;
		editable	= _editable;
		boolmode	= _boolmode;
		borderrt	= _borderrt;
		textalign	= _textalign;
		iconalign	= _iconalign;
	}
};

class xui_treeview : public xui_container
{
	friend class xui_treenode;
	xui_declare_rtti

public:
	static const xui_colour				k_lighttrace_color;
	static const xui_colour				k_nodeselect_color;
	static xui_treeview*				create				( const std::vector<xui_treecolumn>& columninfo );

	xui_treeview( const xui_vector<s32>& size, const std::vector<xui_treecolumn>& columninfo, s32 lineheight = 20, u08 plusrender = k_plusrender_normal, bool rendergrid = true, bool renderhead = true, bool lighttrace = true );
	virtual ~xui_treeview( void );

	s32									get_lineheight		( void ) const;
	void								set_lineheight		( s32 height );
	s32									get_nodeindent		( void ) const;
	void								set_nodeindent		( s32 indent );
	bool								was_lighttrace		( void ) const;
	u08									get_plusrender		( void ) const;
	bool								was_allowmulti		( void ) const;
	void								set_allowmulti		( bool flag );
	bool								was_acceptdrag		( void ) const;
	void								set_acceptdrag		( bool flag );
	u08									get_allowplace		( void ) const;
	void								set_allowplace		( u08 place );
	u32									get_columninfocount	( void ) const;
	const std::vector<xui_treecolumn>&	get_columninfoarray	( void ) const;
	const xui_treecolumn&				get_columninfo		( u32 index ) const;
	void								set_columnsize		( u32 index, s32 size );
	void								set_columntext		( u32 index, const std::wstring& text );
	u08									get_columnsort		( void ) const;
	u32									get_sortcolumn		( void ) const;
	void								set_columnsort		( u32 index, u08 sort );
	void								non_columnsort		( void );
	const std::wstring&					get_searchtext		( void ) const;
	void								set_searchtext		( const std::wstring& text );
	void								ini_selectednode	( xui_treenode* node, bool selected );
	void								set_selectednode	( xui_treenode* node, bool selected );
	void								set_selectednode	( const std::vector<xui_treenode*>& nodes );
	void								non_selectednode	( bool firemethod = true );
	const std::vector<xui_treenode*>&	get_selectednode	( void ) const;
	std::vector<xui_treenode*>			get_entirenode		( bool total = true );
	u32									get_upmostnodecount	( void ) const;
	const std::vector<xui_treenode*>&	get_upmostnodearray	( void ) const;
	u32									get_upmostnodeindex	( xui_treenode* node ) const;
	void								set_upmostnodeindex	( xui_treenode* node, u32 index );
	xui_treenode*						get_upmostnode		( u32 index );
	xui_treenode*						add_upmostnode		( u32 index, xui_treedata* data );
	void								del_upmostnode		( xui_treenode* node );
	void								del_upmostnodeall	( void );
	void								set_nodevisible		( xui_treenode* node );

	virtual xui_rect2d<s32>				get_renderrtins		( void ) const;
	virtual xui_treenode*				choose_node			( const xui_vector<s32>& pt );
	virtual xui_component*				choose_else			( const xui_vector<s32>& pt );
	virtual void						update_else			( f32 delta );
	virtual void						render_else			( void );

	xui_method<xui_method_args>			xm_selectedchange;
	xui_method<xui_method_args>			xm_treenodeenter;
	xui_method<xui_method_args>			xm_treenodeleave;
	xui_method<xui_method_treedragdrop>	xm_treedragover;
	xui_method<xui_method_treedragdrop> xm_treedragdrop;

protected:
	virtual void						on_mousedoubleclick	( xui_method_mouse&		args );
	virtual void						on_mousedown		( xui_method_mouse&		args );
	virtual void						on_mousemove		( xui_method_mouse&		args );
	virtual void						on_mouserise		( xui_method_mouse&		args );
	virtual void						on_invalid			( xui_method_args&		args );
	virtual void						on_perform			( xui_method_args&		args );
	virtual void						on_horzvalue		( xui_method_args&		args );
	virtual void						on_updateself		( xui_method_update&	args );
	virtual void						on_renderself		( xui_method_args&		args );

	xui_treenode*						create_node			( xui_treedata* data );
	void								delete_node			( xui_treenode* node );

	void								on_headclick		( xui_component* sender, xui_method_mouse& args );
	void								on_headrenderself	( xui_component* sender, xui_method_args&  args );

	s32									m_lineheight;
	s32									m_nodeindent;
	u08									m_plusrender;
	bool								m_allowmulti;
	bool								m_acceptdrag;
	bool								m_rendergrid;
	bool								m_renderhead;
	bool								m_lighttrace;
	std::vector<xui_treenode*>			m_selectnode;
	std::vector<xui_treenode*>			m_upmostnode;
	std::vector<xui_treenode*>			m_upmostpart;
	std::vector<xui_treecolumn>			m_columninfo;
	std::vector<xui_treegrid*>			m_columngrid;
	std::vector<xui_drawer*>			m_columnhead;
	u08									m_columnsort;
	u32									m_sortcolumn;
	std::wstring						m_searchtext;
	xui_treenode*						m_mousecatch;
	xui_treenode*						m_mousehover;
	u08									m_allowplace;
};

#endif//__xui_treeview_h__