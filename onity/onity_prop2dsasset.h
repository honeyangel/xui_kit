#ifndef __onity_prop2dsasset_h__
#define __onity_prop2dsasset_h__

#include "onity_propleaf.h"

class NP2DSAsset;
class onity_prop2dsasset : public onity_propleaf
{
public:
	/*
	//constructor
	*/
	onity_prop2dsasset( xui_bitmap* icon, onity_propfile* propfile, u32 id );

	/*
	//method
	*/
	xui_bitmap*			get_resicon		( void );
	u32					get_assetid		( void ) const;
	NP2DSAsset*			get_asset		( void );

	/*
	//override
	*/
	virtual std::string	get_dragtype	( void );
	virtual void*		get_dragdata	( void );

protected:
	/*
	//static
	*/
	static s32			get_freetype	( void* userptr );
	static void			set_freetype	( void* userptr, s32 value );

	/*
	//event
	*/
	void				on_namechanged	( xui_component* sender, xui_method_args& args );

	/*
	//member
	*/
	xui_propkind*		m_reskind;
	u32					m_assetid;
};

#endif//__onity_prop2dsasset_h__