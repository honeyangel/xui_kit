#ifndef __cocos_project_h__
#define __cocos_project_h__

#include "xui_dockpage.h"
#include "xui_proproot.h"

enum
{
	FILTER_NONE = -1,
	FILTER_ALL,
	FILTER_ATLAS,
	FILTER_TEXTURE,
	FILTER_PARTICLE,
	FILTER_MATERIAL,
	FILTER_VERTSHADER,
	FILTER_FRAGSHADER,
	FILTER_TTF,
	FILTER_FNT,
	FILTER_CSD,
};

class cocos_fileview;
class cocos_propfile;
class cocos_proppath;
class cocos_proplike;
class cocos_project : public xui_dockpage
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	cocos_project( void );

	/*
	//destructor
	*/
	virtual ~cocos_project( void );

	/*
	//method
	*/
	void						ini_pathtree		( void );
	void						get_pathfile		( s32 type, xui_proproot_vec& filevec );
	cocos_propfile*				get_pathfile		( const std::wstring& path, const std::wstring& file );
	void						loc_filenode		( const std::wstring& path, const std::wstring& file, u32 id );
	void						loc_filenode		( const std::wstring& path, const std::wstring& file, const std::wstring& name );
	xui_treeview*				get_pathview		( void );
	cocos_fileview*				get_fileview		( void );

	/*
	//notify
	*/
	void						ntf_load			( cocos_propfile* propfile );
	void						ntf_rename			( const std::wstring& last, const std::wstring& curr );
	void						ntf_modify			( const std::wstring& path, const std::wstring& file );

protected:
	/*
	//event
	*/
	void						on_timertick		( xui_component* sender, xui_method_args&	  args );
	void						on_clearclick		( xui_component* sender, xui_method_args&	  args );
	void						on_searchtextchanged( xui_component* sender, xui_method_args&	  args );
	void						on_filterselection	( xui_component* sender, xui_method_args&	  args );
	void						on_headperform		( xui_component* sender, xui_method_args&	  args );
	void						on_fillperform		( xui_component* sender, xui_method_args&	  args );
	void						on_fillrenderelse	( xui_component* sender, xui_method_args&	  args );
	void						on_sizectrlmousemove( xui_component* sender, xui_method_mouse&	  args );
	void						on_pathviewselection( xui_component* sender, xui_method_args&	  args );
	void						on_folderclick		( xui_component* sender, xui_method_args&	  args );
	void						on_particleclick	( xui_component* sender, xui_method_args&	  args );
	void						on_materialclick	( xui_component* sender, xui_method_args&	  args );
	void						on_pathitemclick	( xui_component* sender, xui_method_args&	  args );
	void						on_pathtogglerender	( xui_component* sender, xui_method_args&	  args );
	void						on_pathtoggleclick	( xui_component* sender, xui_method_args&	  args );
	void						on_sizerollscroll	( xui_component* sender, xui_method_args&	  args );
	void						on_fileviewnodeclick( xui_component* sender, xui_method_mouse&	  args );
	void						on_fileviewdoubleclk( xui_component* sender, xui_method_mouse&	  args );
	void						on_fileviewassetdrag( xui_component* sender, xui_method_dragdrop& args );
	void						on_pathloadclick	( xui_component* sender, xui_method_args&	  args );
	void						on_showfindclick	( xui_component* sender, xui_method_args&	  args );
	void						on_propertyclick	( xui_component* sender, xui_method_args&	  args );
	void						on_linetoolclick	( xui_component* sender, xui_method_args&	  args );
	void						on_timersavetick	( xui_component* sender, xui_method_args&	  args );

	/*
	//method
	*/
	void						refresh_fileview	( void );
	void						refresh_pathpane	( void );
	void						refresh_linetool	( void );
	void						refresh_pathmenu	( void );
	void						refresh_tileview	( void );
	void						add_propleaf		( void );
	void						del_propleaf		( const xui_proproot_vec& propvec );
	void						pst_propleaf		( void );

	/*
	//member
	*/
	cocos_proppath*				m_root;
	cocos_proplike*				m_like;
	xui_panel*					m_head;
	xui_toggle*					m_create;
	xui_menuitem*				m_folder;
	xui_menuitem*				m_particle;
	xui_menuitem*				m_material;
	xui_textbox*				m_search;
	xui_button*					m_clear;
	xui_timer*					m_timer;
	xui_timer*					m_timer_save;

	xui_panel*					m_fill;
	xui_control*				m_sizectrl;
	xui_treeview*				m_pathview;
	cocos_fileview*				m_fileview;
	xui_panel*					m_pathpane;
	xui_panel*					m_toolpane;
	xui_slider*					m_sizeroll;
	xui_dropbox*				m_filter;
	xui_drawer*					m_status;

	xui_toolbar*				m_linetool;
	xui_button*					m_backpath;
	xui_button*					m_forepath;
	xui_button*					m_add;
	xui_button*					m_del;
	xui_button*					m_copy;
	xui_button*					m_move;
	xui_button*					m_paste;
	xui_toggle*					m_favorite;
	xui_proproot_vec			m_menuprop;
	xui_menuitem*				m_pathload;
	xui_menuitem*				m_showfind;
	xui_menuitem*				m_property;

	std::vector<xui_treenode*>	m_histroy;
	s32							m_curridx;
};

#endif//__cocos_project_h__