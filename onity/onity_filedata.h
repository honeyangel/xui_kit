#ifndef __onity_filedata_h__
#define __onity_filedata_h__

#include "onity_treedata.h"

class onity_filedata : public onity_treedata
{
public:
	/*
	//constructor
	*/
	onity_filedata( xui_bitmap* icon, const std::wstring& full, xui_proproot* prop );

	/*
	//method
	*/
	std::wstring			get_path		( void ) const;
	std::wstring			get_file		( void ) const;
	std::wstring			get_suff		( void ) const;
	const std::wstring&		get_full		( void ) const;
	void					set_full		( const std::wstring& full );

	/*
	//override
	*/
	virtual xui_bitmap*		get_icon		( u32 index );
	virtual std::wstring	get_text		( u32 index );
	virtual void			set_text		( u32 index, const std::wstring& text );

	/*
	//virtual
	*/
	virtual void			ntf_rename		( const std::wstring& last, const std::wstring& curr );

	/*
	//static
	*/
	static std::wstring		get_path		( const std::wstring& full );
	static std::wstring		get_file		( const std::wstring& full );
	static std::wstring		get_suff		( const std::wstring& full );
	static std::wstring		get_safe		( const std::wstring& full );
};

#endif//__onity_filedata_h__