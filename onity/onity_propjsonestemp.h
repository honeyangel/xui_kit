#ifndef __onity_propjsonestemp_h__
#define __onity_propjsonestemp_h__

#include "EntityTemplate.h"
#include "onity_propleaf.h"

class onity_propjsonestemp : public onity_propleaf
{
public:
	/*
	//constructor
	*/
	onity_propjsonestemp( onity_propfile* propfile, Omiga::EntityTemplate* temp );

	/*
	//method
	*/
	Omiga::EntityTemplate*		get_template	( void );
	void						rna_template	( const std::wstring& text );
	void						pst_template	( Omiga::EntityTemplate* temp );
	const xui_propkind_vec&		get_components	( void ) const;
	void						add_component	( const std::string&  type, const std::string& name );
	void						del_component	( xui_propkind* propkind );

	/*
	//override
	*/
	virtual std::string			get_dragtype	( void );
	virtual void*				get_dragdata	( void );

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
	Omiga::EntityTemplate*		m_template;
	xui_propkind*				m_basekind;
	xui_propkind_vec			m_compkind;
	xui_propkind*				m_menukind;
};

#endif//__onity_propjsonestemp_h__