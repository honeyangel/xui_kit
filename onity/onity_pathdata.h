#ifndef __onity_pathdata_h__
#define __onity_pathdata_h__

#include "xui_propview.h"
#include "onity_filedata.h"

class onity_pathdata : public onity_filedata
{
public:
	/*
	//constructor
	*/
	onity_pathdata( const std::wstring& full, xui_proproot* prop, xui_treenode* link );

	/*
	//destructor
	*/
	virtual ~onity_pathdata( void );

	/*
	//method
	*/
	const xui_proproot_vec&	get_leaf		( void ) const;
	void					add_leaf		( xui_proproot* proproot );

	/*
	//override
	*/
	virtual std::wstring	get_text		( u32 index );
	virtual void			set_text		( u32 index, const std::wstring& text );

	/*
	//override
	*/
	virtual void			ntf_rename		( const std::wstring& last, const std::wstring& curr );

	/*
	//static
	*/
	static void				new_leafnode	( xui_treenode* root );

protected:
	/*
	//member
	*/
	xui_treenode*			m_link;
	xui_proproot_vec		m_leaf;
};

#endif//__onity_pathdata_h__