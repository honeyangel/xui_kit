#ifndef __xui_treenode_h__
#define __xui_treenode_h__

#include "xui_control.h"
#include "xui_plusctrl.h"

class xui_treenode : public xui_control
{
	friend class xui_treeview;
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	xui_treenode( xui_treedata* linkdata, xui_component* parent );

	/*
	//static compare
	*/
	static bool							compare				( xui_treenode* node1, xui_treenode* node2 );

	/*
	//selected
	//expanded
	*/
	bool								was_selected		( void ) const;
	bool								was_expanded		( void ) const;
	void								set_expanded		( bool flag, bool recursion = false );

	/*
	//node
	*/
	s32									get_currnodedepth	( void );
	xui_treenode*						get_rootnode		( void );
	void								get_leafnodetotal	( std::vector<xui_treenode*>& nodes, bool total = true );
	u32									get_leafnodecount	( void ) const;
	const std::vector<xui_treenode*>&	get_leafnodearray	( void ) const;
	xui_treenode*						get_leafnode		( u32 index );
	xui_treenode*						add_leafnode		( u32 index, xui_treedata* data );
	void								del_leafnode		( xui_treenode* node );

	/*
	//data
	*/
	xui_treedata*						get_linkdata		( void );
	void								use_linkdata		( void );

	/*
	//hold
	*/
	void								ini_holdtime		( void );

	/*
	//override
	*/
	virtual xui_colour					get_rendercolor		( void ) const;
	virtual xui_component*				choose				( const xui_vector<s32>& pt );

protected:
	/*
	//callback
	*/
	virtual void						on_keybddown		( xui_method_keybd&    args );
	virtual void						on_perform			( xui_method_args&     args );
	virtual void						on_updateself		( xui_method_args&     args );
	virtual void						on_topdraw			( xui_method_args&     args );

	/*
	//method
	*/
	void								set_selected		( bool flag );
	void								set_edittext		( u32 index );
	void								set_linktext		( void );
	void								set_leafsort		( void );
	void								non_leafsort		( void );
	bool								has_findtext		( u32 index, const std::wstring& text );
	void								non_findtext		( void );

	/*
	//event
	*/
	void								on_textnonfocus		( xui_component* sender, xui_method_args&  args );
	void								on_textkeybddown	( xui_component* sender, xui_method_keybd& args );
	void								on_nodeexpand		( xui_component* sender, xui_method_args&  args );

	/*
	//member
	*/
	bool								m_selected;
	xui_treedata*						m_linkdata;
	xui_treenode*						m_rootnode;
	std::vector<xui_treenode*>			m_leafnode;
	std::vector<xui_treenode*>			m_leafback;
	xui_plusctrl*						m_treeplus;
	xui_textbox*						m_edittext;
	s32									m_holdtime;
};

#endif//__xui_listitem_h__