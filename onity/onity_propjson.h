#ifndef __onity_propjson_h__
#define __onity_propjson_h__

#include "onity_propfile.h"

class onity_propjson : public onity_propfile
{
public:
	/*
	//constructor
	*/
	onity_propjson( const std::wstring& fullname );

	/*
	//destructor
	*/
	virtual ~onity_propjson( void );

	/*
	//method
	*/
	xui_proproot_vec		get_templates	( void );
	void					set_modify		( bool flag );

	/*
	//virtual
	*/
	virtual bool			can_rename		( void );
	virtual bool			was_modify		( void );
	virtual void			load			( void );
	virtual void			save			( void );

protected:
	/*
	//member
	*/
	bool					m_modify;
	xui_proproot_vec		m_templates;
};

#endif//__onity_propjson_h__