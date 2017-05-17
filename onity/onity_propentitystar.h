#ifndef __onity_propentitystar_h__
#define __onity_propentitystar_h__

#include "onity_propedit.h"

class onity_propentity;
class onity_propentitystar : public onity_propedit
{
public:
	/*
	//constructor
	*/
	onity_propentitystar( onity_propentity* node, u32 index );

	/*
	//method
	*/
	bool					was_visible		( void );
	virtual xui_rect2d<s32>	ori_bounding	( void );
	virtual xui_vector<s32> ori_position	( void );
	virtual void			set_position	( const xui_vector<s32>& pos );

protected:
	/*
	//static
	*/
	static xui_vector<f64>	get_position	( void* userptr );
	static void				set_position	( void* userptr, const xui_vector<f64>& value );
	static bool				was_visible		( void* userptr );

	/*
	//member
	*/
	u32						m_subindex;
	onity_propentity*		m_starnode;
	xui_propkind*			m_starkind;
};

#endif//__onity_propentitystar_h__