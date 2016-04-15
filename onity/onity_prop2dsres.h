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
	//destructor
	*/
	virtual ~onity_prop2dsres( void );

	/*
	//virtual
	*/
	std::vector<xui_proproot*>	get_subprop		( void );
	virtual NP2DSAssetFile*		get_resfile		( void );
	virtual xui_proproot*		new_subprop		( u32 id );
	virtual bool				was_modify		( void );
	virtual void				ntf_rename		( const std::wstring& last, const std::wstring& curr );
	virtual void				load			( void );
	virtual void				save			( void );

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
	std::vector<xui_proproot*>	m_subprop;
};

#endif//__onity_prop2dsres_h__