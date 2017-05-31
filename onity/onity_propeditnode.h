#ifndef __onity_propeditnode_h__
#define __onity_propeditnode_h__

#include "onity_bounding.h"
#include "onity_proproot.h"
#include "NP2DSTransRef.h"

typedef std::vector<NP2DSTransRef::SParam> ParamVec;
class onity_propeditnode : public onity_proproot, public onity_bounding
{
public:
	/*
	//constructor
	*/
	onity_propeditnode( void );

	/*
	//virtual
	*/
	virtual NP2DSTransRef*	get_2dsref		( void );
	virtual xui_vector<f64>	get_scale		( void );
	virtual void			set_scale		( const xui_vector<f64>& value );

protected:
	/*
	//static
	*/
	static xui_vector<f64>	get_pos			( void* userptr );
	static void				set_pos			( void* userptr, const xui_vector<f64>& value );
	static xui_vector<f64>	get_scale		( void* userptr );
	static void				set_scale		( void* userptr, const xui_vector<f64>& value );
	static ParamVec&		get_params		( void* userptr );
	static void				add_param		( void* userptr );
	static void				del_param		( void* userptr );
	static xui_propdata*	new_paramprop	( void* userptr, u32 i, xui_propkind* propkind );

	/*
	//member
	*/
	xui_propkind*			m_transkind;
	xui_propkind*			m_paramkind;
};

#endif