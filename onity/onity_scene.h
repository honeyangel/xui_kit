#ifndef __onity_scene_h__
#define __onity_scene_h__

#include "xui_dockpage.h"

class onity_renderview;
class onity_gradpane;
class onity_scene : public xui_dockpage
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	onity_scene( void );

	/*
	//destructor
	*/
	virtual ~onity_scene( void );

	/*
	//method
	*/
	const xui_vector<s32>&	get_trans					( void ) const;
	void					set_trans					( const xui_vector<s32>& trans );
	f64						get_ratio					( void ) const;
	void					set_ratio					( f64 ratio );
	void					set_nodevisible				( xui_treenode* node );

protected:
	/*
	//override
	*/
	virtual void			on_updateself				( xui_method_update& args );

	/*
	//event
	*/
	void					on_buttonclick				( xui_component* sender, xui_method_args&		args );
	void					on_fillpaneperform			( xui_component* sender, xui_method_args&		args );
	void					on_fillpanerenderelse		( xui_component* sender, xui_method_args&		args );
	void					on_drawviewnoncatch			( xui_component* sender, xui_method_args&		args );
	void					on_drawviewupdateself		( xui_component* sender, xui_method_update&		args );
	void					on_drawviewrenderself		( xui_component* sender, xui_method_args&		args );
	void					on_drawviewrenderelse		( xui_component* sender, xui_method_args&		args );
	void					on_drawviewsetrendersz		( xui_component* sender, xui_method_args&		args );
	void					on_drawviewkeybddown		( xui_component* sender, xui_method_keybd&		args );
	void					on_drawviewmousedown		( xui_component* sender, xui_method_mouse&		args );
	void					on_drawviewmousemove		( xui_component* sender, xui_method_mouse&		args );
	void					on_drawviewmouserise		( xui_component* sender, xui_method_mouse&		args );
	void					on_drawviewmousewheel		( xui_component* sender, xui_method_mouse&		args );
	void					on_drawviewmousedragover	( xui_component* sender, xui_method_dragdrop&	args );
	void					on_drawviewmousedragdrop	( xui_component* sender, xui_method_dragdrop&	args );
	void					on_animctrltick				( xui_component* sender, xui_method_args&		args );

	/*
	//method
	*/
	f64						get_incratio				( void );
	f64						get_decratio				( void );
	xui_proproot_vec		get_propcand				( void );
	void					cal_snapinfo				( const xui_rect2d<s32>& self, xui_treenode* root, u08 mode );
	void					use_snapinfo				( void );
	bool					cal_linestep				( s32& last, s32 temp );
	xui_vector<s32>			cal_snapmove				( const std::vector<xui_treenode*>& nodevec, xui_vector<s32>& snap, xui_vector<s32>& step, xui_rect2d<s32>& self, xui_rect2d<s32>* horz, xui_rect2d<s32>* vert );

	/*
	//snapinfo
	*/
	class snap_info
	{
	public:
		snap_info( xui_proproot* _curr, xui_proproot* _next = NULL )
		{
			curr = _curr;
			next = _next;
		}

		xui_proproot*		curr;
		xui_proproot*		next;
	};

	/*
	//drawsnap
	*/
	void					draw_horzsnap				( const xui_rect2d<s32>& self, s32 snap, const std::vector<snap_info>& vec );
	void					draw_vertsnap				( const xui_rect2d<s32>& self, s32 snap, const std::vector<snap_info>& vec );
	void					draw_horzstep				( const xui_rect2d<s32>& self, s32 snap, const std::vector<snap_info>& vec, const xui_rect2d<s32>& curr );
	void					draw_vertstep				( const xui_rect2d<s32>& self, s32 snap, const std::vector<snap_info>& vec, const xui_rect2d<s32>& curr );
	void					draw_locknode				( const std::vector<xui_treenode*>& nodevec );				

	/*
	//typedef
	*/
	typedef std::map< s32, std::vector<snap_info> >
		onity_snapinfo_map;

	/*
	//member
	*/
	xui_vector<s32>			m_trans;
	f64						m_ratio;
	xui_button*				m_reset;
	xui_button*				m_large;
	xui_button*				m_small;
	xui_button*				m_clear;
	xui_panel*				m_cubepane;
	xui_panel*				m_headpane;
	xui_panel*				m_fillpane;
	onity_renderview*		m_drawview;
	onity_gradpane*			m_horzgrad;
	onity_gradpane*			m_vertgrad;
	xui_action_ctrl*		m_animctrl;
	xui_action_ctrl*		m_lockctrl;
	bool					m_dragview;
	bool					m_dragprop;
	onity_snapinfo_map		m_horzsnap;
	onity_snapinfo_map		m_vertsnap;
	onity_snapinfo_map		m_horzmidd;
	onity_snapinfo_map		m_vertmidd;
	onity_snapinfo_map		m_horzstep;
	onity_snapinfo_map		m_vertstep;
};

#endif//__onity_scene_h__