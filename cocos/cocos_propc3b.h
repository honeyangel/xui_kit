#ifndef __cocos_propc3b_h__
#define __cocos_propc3b_h__

#include "cocos_propfile.h"

class cocos_propc3b : public cocos_propfile
{
public:
	/*
	//constructor
	*/
	cocos_propc3b( const std::wstring& full );

	/*
	//override
	*/
	virtual std::string		get_dragtype	( void );
	virtual void*			get_dragdata	( void );
};

#endif//__cocos_propc3b_h__