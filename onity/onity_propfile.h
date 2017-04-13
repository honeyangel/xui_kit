#ifndef __onity_propfile_h__
#define __onity_propfile_h__

#include "onity_proproot.h"

class onity_savekind;
class onity_propfile : public onity_proproot
{
public:
	/*
	//constructor
	*/
	onity_propfile( xui_bitmap* icon, const std::wstring& fullname );

	/*
	//method
	*/
	xui_bitmap*				get_fileicon	( void );
	const std::wstring&		get_fullname	( void ) const;

	/*
	//virtual
	*/
	virtual std::string		get_dragtype	( void );
	virtual void*			get_dragdata	( void );
	virtual bool			was_modify		( void );
	virtual bool			can_rename		( void );
	virtual void			ntf_rename		( const std::wstring& last, const std::wstring& curr );
	virtual void 			ntf_modify		( void );
	virtual void			load			( void );
	virtual void			save			( void );
	virtual void			auto_save		( void );

	/*
	//override
	*/
	virtual void			on_detach		( const xui_proproot_vec& proproot );

	/*
	//static
	*/
	static void				on_notify_accept( xui_component* sender, xui_method_args& args );
	static void				on_notify_cancel( xui_component* sender, xui_method_args& args );
	static void				on_modify_reload( xui_component* sender, xui_method_args& args );

protected:
	/*
	//member
	*/
	std::wstring			m_fullname;
	xui_propkind*			m_basekind;
	onity_savekind*			m_savekind;
};

#endif//__onity_propfile_h__