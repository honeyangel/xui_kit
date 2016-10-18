#ifndef __onity_propscenelayer_h__
#define __onity_propscenelayer_h__

#include "onity_propentitytemp.h"

class NP2DSSceneLayer;
class onity_propscenelayer : public onity_propentitytemp
{
public:
	/*
	//constructor
	*/
	onity_propscenelayer( NP2DSSceneLayer* scenelayer );

	/*
	//method
	*/
	s32						get_snaplength	( void ) const;
	void					set_snaplength	( s32 length );
	NP2DSSceneLayer*		get_scenelayer	( void );
	virtual void			rna_template	( const std::wstring& text );

protected:
	/*
	//static
	*/
	static xui_vector<f64>	get_cellsize	( void* userptr );
	static void				set_cellsize	( void* userptr, const xui_vector<f64>& value );
	static f64				get_rows		( void* userptr );
	static void				set_rows		( void* userptr, f64 value );
	static f64				get_cols		( void* userptr );
	static void				set_cols		( void* userptr, f64 value );
	static f64				get_snaplength	( void* userptr );
	static void				set_snaplength	( void* userptr, f64 value );

	/*
	//member
	*/
	s32						m_snaplength;
	xui_propkind*			m_legendkind;
	xui_propkind*			m_designkind;
	NP2DSSceneLayer*		m_scenelayer;
};

#endif//__onity_propscenelayer_h__