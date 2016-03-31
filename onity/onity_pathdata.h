#ifndef __onity_pathdata_h__
#define __onity_pathdata_h__

#include "xui_treedata.h"

enum 
{
	META_NONE,
	META_MODULE,
	META_SPRITE,
	META_ACTION,
};

class onity_proppath;
class onity_pathdata : public xui_treedata
{
public:
	/*
	//constructor
	*/
	onity_pathdata( const std::wstring& full, bool create_prop );

	/*
	//destructor
	*/
	virtual ~onity_pathdata( void );

	/*
	//method
	*/
	xui_proproot*			get_prop		( void );
	const std::wstring&		get_full		( void ) const;
	void					set_full		( const std::wstring& full );
	std::wstring			get_path		( void ) const;
	virtual std::wstring	get_text		( u32 index );
	virtual void			set_text		( u32 index, const std::wstring& text );

	/*
	//static
	*/
	static void				create_subnode	( xui_treenode* root );
	static void				load_meta		( u08 type, const std::string& fullname );

protected:
	/*
	//member
	*/
	xui_proproot*			m_prop;	
};

#endif//__onity_pathdata_h__