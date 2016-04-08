#ifndef __onity_prop2dsres_h__
#define __onity_prop2dsres_h__

#include "onity_propfile.h"

enum 
{
	META_NONE,
	META_MODULE,
	META_SPRITE,
	META_ACTION,
};

class NP2DSAssetFile;
class onity_prop2dsres : public onity_propfile
{
public:
	/*
	//constructor
	*/
	onity_prop2dsres( const std::wstring& full );

	/*
	//virtual
	*/
	virtual NP2DSAssetFile*		get_resfile		( void );
	virtual void				ntf_rename		( const std::wstring& last, const std::wstring& curr );

	/*
	//meta
	*/
	static void					load_meta		( u08 type, const std::string& fullname );
	static void					save_meta		( u08 type, u32 id );

protected:
	/*
	//static
	*/
	static s32					get_freetype	( void* userptr );
	static void					set_freetype	( void* userptr, s32  value );
	static bool					get_loadtype	( void* userptr );
	static void					set_loadtype	( void* userptr, bool value );

	/*
	//member
	*/
	xui_propkind*				m_reskind;
	u32							m_resfile;
};

#endif//__onity_prop2dsres_h__