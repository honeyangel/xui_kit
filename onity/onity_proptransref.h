#ifndef __onity_proptransref_h__
#define __onity_proptransref_h__

#include "onity_propasset.h"

enum
{
	BM_USER,
	BM_DARK,
	BM_ADD,
	BM_DEFAULT,
};

class NP2DSTransRef;
class onity_proptransref : public onity_propasset
{
public:
	/*
	//constructor
	*/
	onity_proptransref( onity_propfile* propfile, NP2DSTransRef* transref );

	/*
	//method
	*/
	NP2DSTransRef*				get_transref		( void );

protected:
	/*
	//method
	*/
	void						add_localkind		( void );
	void						add_worldkind		( void );
	void						add_graphkind		( void );

	/*
	//static
	*/
	static f64					get_localflip		( void* userptr );
	static void					set_localflip		( void* userptr, f64  value );
	static f64					get_localangle		( void* userptr );
	static void					set_localangle		( void* userptr, f64  value );
	static f64					get_localscaleunit	( void* userptr );
	static void					set_localscaleunit	( void* userptr, f64  value );
	static xui_vector<f64>		get_localscale		( void* userptr );
	static void					set_localscale		( void* userptr, const xui_vector<f64>& value );
	static xui_vector<f64>		get_localpivot		( void* userptr );
	static void					set_localpivot		( void* userptr, const xui_vector<f64>& value );
	static xui_rect2d<f64>		get_pivotboundbox	( void* userptr );

	static f64					get_worldflip		( void* userptr );
	static void					set_worldflip		( void* userptr, f64  value );
	static f64					get_worldangle		( void* userptr );
	static void					set_worldangle		( void* userptr, f64  value );
	static f64					get_worldscaleunit	( void* userptr );
	static void					set_worldscaleunit	( void* userptr, f64  value );
	static xui_vector<f64>		get_worldscale		( void* userptr );
	static void					set_worldscale		( void* userptr, const xui_vector<f64>& value );
	static xui_vector<f64>		get_worldtrans		( void* userptr );
	static void					set_worldtrans		( void* userptr, const xui_vector<f64>& value );

	static s32					get_filter			( void* userptr );
	static void					set_filter			( void* userptr, s32  value );
	static s32					get_texenv			( void* userptr );
	static void					set_texenv			( void* userptr, s32  value );
	static xui_colour			get_vertexcolor		( void* userptr );
	static void					set_vertexcolor		( void* userptr, const xui_colour&      value );
	static s32					get_blendmode		( void* userptr );
	static void					set_blendmode		( void* userptr, s32  value );
	static s32					get_blendsrc		( void* userptr );
	static void					set_blendsrc		( void* userptr, s32  value );
	static s32					get_blenddst		( void* userptr );
	static void					set_blenddst		( void* userptr, s32  value );
	static bool					was_outline			( void* userptr );
	static void					set_outline			( void* userptr, bool value );
	static f64					get_outlinewidth	( void* userptr );
	static void					set_outlinewidth	( void* userptr, f64  value );
	static xui_colour			get_outlinecolor	( void* userptr );
	static void					set_outlinecolor	( void* userptr, const xui_colour&		value );

	/*
	//member
	*/
	xui_propkind*				m_localkind;
	xui_propkind*				m_worldkind;
	xui_propkind*				m_graphkind;
	NP2DSTransRef*				m_transref;
};

#endif//__onity_proptransref_h__