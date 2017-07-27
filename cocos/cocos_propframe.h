#ifndef __cocos_propframe_h__
#define __cocos_propframe_h__

#include "cocos_propleaf.h"

namespace cocos2d
{
	class SpriteFrame;
}

class cocos_propframe : public cocos_propleaf
{
public:
	/*
	//constructor
	*/
	cocos_propframe( xui_bitmap* icon, cocos_propfile* propfile, const std::string& name );

	/*
	//method
	*/
	xui_bitmap*				get_frameicon		( void );
	const std::string&		get_framename		( void ) const;
	cocos2d::SpriteFrame*	get_frame			( void );

	/*
	//override
	*/
	virtual std::string		get_dragtype		( void );
	virtual void*			get_dragdata		( void );

protected:
	/*
	//static
	*/
	static xui_vector<f64>	get_originalsize	( void* userptr );
	static xui_vector<f64>	get_offset			( void* userptr );
	static xui_rect2d<f64>	get_rect			( void* userptr );
	static bool				was_rotated			( void* userptr );

	/*
	//member
	*/
	xui_propkind*			m_framekind;
	std::string				m_framename;
};

#endif//__cocos_propframe_h__