#ifndef __cocos_propspineatlas_h__
#define __cocos_propspineatlas_h__

#include "cocos_propfile.h"

class cocos_propspineatlas : public cocos_propfile
{
public:
	/*
	//constructor
	*/
	cocos_propspineatlas( const std::wstring& full );

	/*
	//override
	*/
	virtual std::string			get_dragtype		( void );
	virtual void*				get_dragdata		( void );
};

#endif//__onity_propspineatlas_h__