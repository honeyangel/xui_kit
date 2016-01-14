#ifndef __xui_treedata_h__
#define __xui_treedata_h__

#include "xui_component.h"

class xui_treedata
{
	friend class xui_treenode;

public:
	/*
	//constructor
	*/
	xui_treedata( void );
	xui_treedata( const std::wstring& text );

	/*
	//method
	*/
	xui_treenode*			get_node		( void );

	/*
	//interface
	*/
	virtual std::wstring	get_text		( u32 index );
	virtual xui_colour		get_textcolor	( u32 index );
	virtual bool			get_flag		( u32 index );
	virtual xui_bitmap*		get_icon		( u32 index );
	virtual void			set_text		( u32 index, const std::wstring& text );
	virtual void			set_flag		( u32 index, bool flag );

protected:
	/*
	//method
	*/
	void					set_node		( xui_treenode* node );

	/*
	//member
	*/
	xui_treenode*			m_node;
	std::wstring			m_text;
};

#endif//__xui_treedata_h__