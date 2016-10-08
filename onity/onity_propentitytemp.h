#ifndef __onity_propentitytemp_h__
#define __onity_propentitytemp_h__

#include "EntityTemplate.h"
#include "onity_proproot.h"

class onity_propentitytemp : public onity_proproot
{
public:
	/*
	//constructor
	*/
	onity_propentitytemp( const std::string& tempname );

	/*
	//destructor
	*/
	virtual ~onity_propentitytemp( void );

	/*
	//method
	*/
	bool						was_modify		( void ) const;
	void						set_modify		( bool flag );
	const std::string&			get_tempname	( void ) const;
	Omiga::EntityTemplate*		get_template	( void );
	virtual void				rna_template	( const std::wstring& text );

	const xui_propkind_vec&		get_components	( void ) const;
	void						add_component	( const std::string&  type, const std::string& name );
	void						del_component	( xui_propkind* propkind );

	void						save			( void );
	void						load			( void );

protected:
	/*
	//method
	*/
	void						new_compkind	( BreezeGame::Json::Value* compnode );
	void						add_compkind	( void );
	void						del_compkind	( void );

	/*
	//event
	*/
	void						on_namechanged	( xui_component* sender, xui_method_args&     args );
	void						on_propchanged	( xui_component* sender, xui_method_propdata& args );

	/*
	//member
	*/
	bool						m_modify;
	std::string					m_tempname;
	BreezeGame::Json::Value*	m_jsonnode;
	xui_propkind*				m_basekind;
	xui_propkind*				m_tempkind;
	xui_propkind_vec			m_compkind;
	xui_propkind*				m_menukind;
};

#endif//__onity_propentitytemp_h__