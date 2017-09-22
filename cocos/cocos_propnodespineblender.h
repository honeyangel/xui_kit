#ifndef __cocos_propnodespineblender_h__
#define __cocos_propnodespineblender_h__

#include "cocos_propnodecustom.h"
#include "cocos_propdata_spineanim.h"

namespace cocos2d
{
	namespace ui
	{
		class SpineBlender;
	}
}

class cocos_propfile;
class cocos_propnodespineblender : public cocos_propnodecustom
{
public:
	/*
	//constructor
	*/
	cocos_propnodespineblender( cocos_propcsd* file, cocos2d::Node* node );

	/*
	//method
	*/
	cocos2d::ui::SpineBlender*	get_spineblender	( void );
	void						add_spineblender	( void );
	cocos_propfile*				get_spinefile		( void );
	void						set_spinefile		( cocos_propfile* file );
	cocos_propfile*				get_atlasfile		( void );
	void						set_atlasfile		( cocos_propfile* file );

protected:
	/*
	//static
	*/
	static void*				get_spine			( void* userptr );
	static void					set_spine			( void* userptr, void* value );
	static void*				get_atlas			( void* userptr );
	static void					set_atlas			( void* userptr, void* value );
	static cocos_string_vec		get_animvec			( void* userptr );
	static std::wstring			get_anim			( void* userptr );
	static void					set_anim			( void* userptr, const std::wstring& value );
	static bool					was_loop			( void* userptr );
	static void					set_loop			( void* userptr, bool  value );
	static void*				get_masktex			( void* userptr );
	static void					set_masktex			( void* userptr, void* value );
	static bool					was_rendertarget	( void* userptr );
	static void					set_rendertarget	( void* userptr, bool  value );
	static xui_vector<f64>		get_rendertargetsize( void* userptr );
	static void					set_rendertargetsize( void* userptr, const xui_vector<f64>& value );
	static xui_vector<f64>		get_spineoffset		( void* userptr );
	static void					set_spineoffset		( void* userptr, const xui_vector<f64>& value );
	static f64					get_spinescale		( void* userptr );
	static void					set_spinescale		( void* userptr, f64   value );

	/*
	//member
	*/
	cocos2d::ui::SpineBlender*	m_spineblender;
	cocos_propfile*				m_spinefile;
	cocos_propfile*				m_atlasfile;
};

#endif//__cocos_propnodespineblender_h__