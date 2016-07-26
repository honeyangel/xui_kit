#ifndef __onity_proptempold_h__
#define __onity_proptempold_h__

#include "EntityTemplate.h"
#include "onity_propleaf.h"

class onity_proptempold : public onity_propleaf
{
public:
	/*
	//constructor
	*/
	onity_proptempold( onity_propfile* propfile, Omiga::EntityTemplate* temp );

	/*
	//method
	*/
	Omiga::EntityTemplate*		get_template	( void );

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

	/*
	//event
	*/
	void						on_propchanged	( xui_component* sender, xui_method_propdata& args );

	/*
	//member
	*/
	Omiga::EntityTemplate*		m_template;
	xui_propkind*				m_basekind;
	xui_propkind_vec			m_compkind;
};

#endif//__onity_proptempold_h__