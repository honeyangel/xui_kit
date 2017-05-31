#ifndef __onity_propcoursenode_h__
#define __onity_propcoursenode_h__

#include "NP2DSTransRef.h"
#include "onity_propeditnode.h"

class onity_propcoursenode : public onity_propeditnode
{
public:
	/*
	//constructor
	*/
	onity_propcoursenode( NP2DSTransRef* ref );

	/*
	//method
	*/
	void					set_newref		( NP2DSAsset* asset );
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
	static void*			get_asset		( void* userptr );
	static void				set_asset		( void* userptr, void* value );

	/*
	//member
	*/
	NP2DSTransRef*			m_2dsref;
	xui_propkind*			m_assetkind;
};

#endif//__onity_propcoursenode_h__