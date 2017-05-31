#ifndef __onity_propentitystar_h__
#define __onity_propentitystar_h__

#include "onity_propeditnode.h"

class onity_propentitynode;
class onity_propentitystar : public onity_propeditnode
{
public:
	/*
	//constructor
	*/
	onity_propentitystar( onity_propentitynode* node, u32 index );

	/*
	//method
	*/
	bool					was_visible		( void );
	virtual NP2DSTransRef*	get_2dsref		( void );
	virtual xui_vector<f64>	get_scale		( void );
	virtual void			set_scale		( const xui_vector<f64>& value );
	virtual xui_rect2d<s32>	ori_bounding	( void );
	virtual xui_vector<s32> ori_position	( void );
	virtual void			set_position	( const xui_vector<s32>& pos );

protected:
	/*
	//static
	*/
	static bool				was_visible		( void* userptr );

	/*
	//member
	*/
	onity_propentitynode*	m_groupnode;
	u32						m_starindex;
	xui_propkind*			m_assetkind;
};

#endif//__onity_propentitystar_h__