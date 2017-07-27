#ifndef __cocos_propttf_h__
#define __cocos_propttf_h__

#include "cocos_propfile.h"

class cocos_propttf : public cocos_propfile
{
public:
	/*
	//constructor
	*/
	cocos_propttf( const std::wstring& full );

	/*
	//override
	*/
	virtual std::string			get_dragtype		( void );
	virtual void*				get_dragdata		( void );
};

#endif//__onity_propttf_h__