#ifndef __cocos_propnodetextifbm_h__
#define __cocos_propnodetextifbm_h__

#include "cocos_propnodecustom.h"

namespace cocos2d
{
	namespace ui
	{
		class TextIFBM;
	}
}

class cocos_propnodetextifbm : public cocos_propnodecustom
{
public:
	/*
	//constructor
	*/
	cocos_propnodetextifbm( cocos_propcsd* file, cocos2d::Node* node );

	/*
	//method
	*/
	cocos2d::ui::TextIFBM*		get_textifbm	( void );
	void						add_textifbm	( void );

protected:
	/*
	//static
	*/
	static f64					get_linegrap	( void* userptr );
	static void					set_linegrap	( void* userptr, f64 value );
	static f64					get_chargrap	( void* userptr );
	static void					set_chargrap	( void* userptr, f64 value );

	/*
	//member
	*/
	cocos2d::ui::TextIFBM*		m_textifbm;
};

#endif//__cocos_propnodetextifbm_h__