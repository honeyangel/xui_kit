#ifndef __cocos_propspine_h__
#define __cocos_propspine_h__

#include "cocos_propfile.h"

class cocos_propspine : public cocos_propfile
{
public:
	/*
	//constructor
	*/
	cocos_propspine( const std::wstring& full );

	/*
	//override
	*/
	virtual std::string			get_dragtype		( void );
	virtual void*				get_dragdata		( void );
};

#endif//__onity_propspine_h__