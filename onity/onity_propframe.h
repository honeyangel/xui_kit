#ifndef __onity_propframe_h__
#define __onity_propframe_h__

#include "onity_prop2dsasset.h"

class NP2DSFrame;
class NP2DSImageRef;
class NP2DSCollider;
class onity_propframe : public onity_prop2dsasset
{
public:
	/*
	//constructor
	*/
	onity_propframe( onity_propfile* propfile, u32 id );

	/*
	//destructor
	*/
	virtual ~onity_propframe( void );

	/*
	//method
	*/
	NP2DSFrame*							get_frame		( void );
	const xui_proproot_vec&				get_colliders	( void ) const;
	const xui_proproot_vec&				get_imagerefs	( void ) const;
	void								add_imageref	( void );
	void								add_collider	( void );
	void								del_imageref	( NP2DSImageRef* imageref );
	void								del_collider	( NP2DSCollider* collider );

protected:
	/*
	//static
	*/
	static bool							was_boundcustom	( void* userptr );
	static void							set_boundcustom	( void* userptr, bool value );
	static xui_rect2d<f64>				get_boundingbox	( void* userptr );
	static void							set_boundingbox	( void* userptr, const xui_rect2d<f64>& value );
	static std::vector<NP2DSImageRef*>&	get_imagerefs	( void* userptr );
	static xui_proproot_vec				get_proptotal	( void* userptr );
	static xui_proproot_vec				get_colliders	( void* userptr );
	static void							del_imageref	( void* userptr );
	static void							del_collider	( void* userptr, xui_proproot* root );
	static void							add_imageref	( void* userptr );
	static void							add_collider	( void* userptr );
	static xui_propdata*				new_propdata	( void* userptr, u32 i, xui_propkind* propkind );

	/*
	//member
	*/
	xui_propkind*						m_framekind;
	xui_proproot_vec					m_propimageref;
	xui_proproot_vec					m_propcollider;
};

#endif//__onity_propframe_h__