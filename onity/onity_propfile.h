#ifndef __onity_propfile_h__
#define __onity_propfile_h__

#include "xui_proproot.h"

class onity_propfile : public xui_proproot
{
public:
	/*
	//constructor
	*/
	onity_propfile( const std::wstring& full );

	/*
	//method
	*/
	const std::wstring&		get_full	( void ) const;

	/*
	//virtual
	*/
	virtual bool			was_modify	( void );
	virtual void			set_modify	( bool flag );
	virtual void			ntf_rename	( const std::wstring& last, const std::wstring& curr );
	virtual void			load		( void );
	virtual void			save		( void );

protected:
	/*
	//member
	*/
	bool					m_modify;
	std::wstring			m_fullname;
	xui_propkind*			m_basekind;
};

#endif//__onity_propfile_h__