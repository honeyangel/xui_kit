#ifndef __onity_proplayer_h__
#define __onity_proplayer_h__

#include "onity_propleaf.h"

class NP2DSLayer;
class onity_propactor;
class onity_proplayer : public onity_propleaf
{
public:
	/*
	//constructor
	*/
	onity_proplayer( onity_propfile* propfile, onity_propactor* propactor, NP2DSLayer* layer );

	/*
	//destructor
	*/
	virtual ~onity_proplayer( void );

	/*
	//method
	*/
	onity_propactor*		get_actor		( void );
	NP2DSLayer*				get_layer		( void );
	const xui_proproot_vec&	get_framekeys	( void ) const;
	xui_proproot*			add_framekey	( s32 time );
	void					del_framekey	( s32 time );
	xui_proproot*			get_framekey	( s32 time );
	void					ntf_rename		( const std::wstring& name );

protected:
	/*
	//event
	*/
	void					on_namechanged	( xui_component* sender, xui_method_args& args );

	/*
	//static
	*/
	static bool				has_particle	( void* userptr );
	static bool				has_sound		( void* userptr );
	static bool				has_event		( void* userptr );

	/*
	//member
	*/
	xui_propkind*			m_basekind;
	NP2DSLayer*				m_layer;
	onity_propactor*		m_propactor;
	xui_proproot_vec		m_propframekeys;
};

#endif//__onity_proplayer_h__