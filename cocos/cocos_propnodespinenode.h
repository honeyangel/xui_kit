#ifndef __cocos_propnodespinenode_h__
#define __cocos_propnodespinenode_h__

#include "cocos_propnodecustom.h"
#include "cocos_propdata_spineanim.h"

namespace cocos2d
{
	namespace ui
	{
		class SpineNode;
	}
}

class cocos_propfile;
class cocos_propnodespinenode : public cocos_propnodecustom
{
public:
	/*
	//constructor
	*/
	cocos_propnodespinenode( cocos_propcsd* file, cocos2d::Node* node );

	/*
	//method
	*/
	cocos2d::ui::SpineNode*		get_spinenode	( void );
	void						add_spinenode	( void );
	cocos_propfile*				get_spinefile	( void );
	void						set_spinefile	( cocos_propfile* file );
	cocos_propfile*				get_atlasfile	( void );
	void						set_atlasfile	( cocos_propfile* file );

protected:
	/*
	//static
	*/
	static void*				get_spine		( void* userptr );
	static void					set_spine		( void* userptr, void* value );
	static void*				get_atlas		( void* userptr );
	static void					set_atlas		( void* userptr, void* value );
	static bool					was_loop		( void* userptr );
	static void					set_loop		( void* userptr, bool  value );
	static cocos_string_vec		get_animvec		( void* userptr );
	static std::wstring			get_anim		( void* userptr );
	static void					set_anim		( void* userptr, const std::wstring& value );

	/*
	//member
	*/
	cocos2d::ui::SpineNode*		m_spinenode;
	cocos_propfile*				m_spinefile;
	cocos_propfile*				m_atlasfile;
};

#endif//__cocos_propnodespinenode_h__