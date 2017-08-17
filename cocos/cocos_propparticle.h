#ifndef __cocos_propparticle_h__
#define __cocos_propparticle_h__

#include "cocos_propfile.h"
#include "cocos_propdata_blend.h"

namespace cocos2d
{
	class ParticleSystem;
}

class cocos_propparticle : public cocos_propfile
{
public:
	/*
	//constructor
	*/
	cocos_propparticle( const std::wstring& full );

	/*
	//destructor
	*/
	virtual ~cocos_propparticle( void );

	/*
	//method
	*/
	cocos2d::ParticleSystem*	get_particle		( void );
	//void					add_spritesrc		( void );
	//void					del_spritesrc		( xui_propkind* propkind );
	//void					set_spritesrcindex	( xui_propkind* propkind, u32 oldindex, u32 newindex );

	/*
	//override
	*/
	virtual std::string			get_dragtype		( void );
	virtual void*				get_dragdata		( void );
	virtual bool				was_modify			( void );
	virtual void				load				( void );

protected:
	/*
	//event
	*/
	void						on_propchanged		( xui_component* sender, xui_method_propdata&	args );
	void						on_flagchanged		( xui_component* sender, xui_method_args&		args );

	/*
	//override
	*/
	virtual void				save_as				( const std::wstring& fullname, bool modify );

	/*
	//static
	*/
	static void*				get_texture			( void* userptr );
	static void					set_texture			( void* userptr, void* value );

	static f64					get_duration		( void* userptr );
	static void					set_duration		( void* userptr, f64 value );
	static f64					get_maxparticle		( void* userptr );
	static void					set_maxparticle		( void* userptr, f64 value );
	static f64					get_lifespan		( void* userptr );
	static void					set_lifespan		( void* userptr, f64 value );
	static f64					get_lifespanvar		( void* userptr );
	static void					set_lifespanvar		( void* userptr, f64 value );
	static f64					get_startsize		( void* userptr );
	static void					set_startsize		( void* userptr, f64 value );
	static f64					get_startsizevar	( void* userptr );
	static void					set_startsizevar	( void* userptr, f64 value );
	static f64					get_finishsize		( void* userptr );
	static void					set_finishsize		( void* userptr, f64 value );
	static f64					get_finishsizevar	( void* userptr );
	static void					set_finishsizevar	( void* userptr, f64 value );
	static f64					get_angle			( void* userptr );
	static void					set_angle			( void* userptr, f64 value );
	static f64					get_anglevar		( void* userptr );
	static void					set_anglevar		( void* userptr, f64 value );
	static f64					get_rotationstart	( void* userptr );
	static void					set_rotationstart	( void* userptr, f64 value );
	static f64					get_rotationstartvar( void* userptr );
	static void					set_rotationstartvar( void* userptr, f64 value );
	static f64					get_rotationend		( void* userptr );
	static void					set_rotationend		( void* userptr, f64 value );
	static f64					get_rotationendvar	( void* userptr );
	static void					set_rotationendvar	( void* userptr, f64 value );

	static xui_colour			get_startcolor		( void* userptr );
	static void					set_startcolor		( void* userptr, const xui_colour& value );
	static xui_colour			get_startcolorvar	( void* userptr );
	static void					set_startcolorvar	( void* userptr, const xui_colour& value );
	static xui_colour			get_endcolor		( void* userptr );
	static void					set_endcolor		( void* userptr, const xui_colour& value );
	static xui_colour			get_endcolorvar		( void* userptr );
	static void					set_endcolorvar		( void* userptr, const xui_colour& value );

	static s32					get_emittertype		( void* userptr );
	static void					set_emittertype		( void* userptr, s32 value );
	static f64					get_speed			( void* userptr );
	static void					set_speed			( void* userptr, f64 value );
	static f64					get_speedvar		( void* userptr );
	static void					set_speedvar		( void* userptr, f64 value );
	static f64					get_radialacc		( void* userptr );
	static void					set_radialacc		( void* userptr, f64 value );
	static f64					get_radialaccvar	( void* userptr );
	static void					set_radialaccvar	( void* userptr, f64 value );
	static f64					get_tangenacc		( void* userptr );
	static void					set_tangenacc		( void* userptr, f64 value );
	static f64					get_tangenaccvar	( void* userptr );
	static void					set_tangenaccvar	( void* userptr, f64 value );
	static f64					get_maxradius		( void* userptr );
	static void					set_maxradius		( void* userptr, f64 value );
	static f64					get_maxradiusvar	( void* userptr );
	static void					set_maxradiusvar	( void* userptr, f64 value );
	static f64					get_minradius		( void* userptr );
	static void					set_minradius		( void* userptr, f64 value );
	static f64					get_minradiusvar	( void* userptr );
	static void					set_minradiusvar	( void* userptr, f64 value );
	static f64					get_rotatespeed		( void* userptr );
	static void					set_rotatespeed		( void* userptr, f64 value );
	static f64					get_rotatespeedvar	( void* userptr );
	static void					set_rotatespeedvar	( void* userptr, f64 value );

	static xui_vector<f64>		get_position		( void* userptr );
	static void					set_position		( void* userptr, const xui_vector<f64>& value );
	static xui_vector<f64>		get_positionvar		( void* userptr );
	static void					set_positionvar		( void* userptr, const xui_vector<f64>& value );

	static cocos_blend_value	get_blendfunc		( void* userptr );
	static void					set_blendfunc		( void* userptr, const cocos_blend_value& value );
	//static s32					get_blendfunction	( void* userptr );
	//static void					set_blendfunction	( void* userptr, s32 value );
	//static s32					get_blendsrc		( void* userptr );
	//static void					set_blendsrc		( void* userptr, s32 value );
	//static s32					get_blenddst		( void* userptr );
	//static void					set_blenddst		( void* userptr, s32 value );

	/*
	//member
	*/
	bool						m_modify;
	cocos2d::ParticleSystem*	m_particle;
	xui_propkind*				m_valuekind;
	xui_propkind*				m_blendkind;
	xui_propkind*				m_colorkind;
	xui_propkind*				m_imagekind;
	xui_propkind*				m_transkind;
	xui_propkind*				m_emittkind;
};

#endif//__onity_propparticle_h__