#ifndef __onity_propfile_h__
#define __onity_propfile_h__

#include "xui_proproot.h"

class onity_savekind;
class onity_propfile : public xui_proproot
{
public:
	/*
	//constructor
	*/
	onity_propfile( const std::wstring& fullname );

	/*
	//destructor
	*/
	virtual ~onity_propfile( void );

	/*
	//method
	*/
	const std::wstring&		get_fullname	( void ) const;
	xui_treedata*			get_linkdata	( void );
	void					set_linkdata	( xui_treedata* linkdata );

	/*
	//virtual
	*/
	virtual std::string		get_dragtype	( void );
	virtual void*			get_dragdata	( void );
	virtual bool			was_modify		( void );
	virtual void			ntf_rename		( const std::wstring& last, const std::wstring& curr );
	virtual void			load			( void );
	virtual void			save			( void );

	/*
	//override
	*/
	virtual void			on_detach		( const xui_proproot_vec& proproot );

	/*
	//static
	*/
	static void				on_notify_accept( xui_component* sender, xui_method_args& args );
	static void				on_notify_cancel( xui_component* sender, xui_method_args& args );

protected:
	/*
	//member
	*/
	xui_treedata*			m_linkdata;
	std::wstring			m_fullname;
	xui_propkind*			m_basekind;
	onity_savekind*			m_savekind;
};

#endif//__onity_propfile_h__