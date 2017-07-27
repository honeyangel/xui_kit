#ifndef __cocos_propvertshader_h__
#define __cocos_propvertshader_h__

#include "cocos_propfile.h"

class cocos_propvertshader : public cocos_propfile
{
public:
	/*
	//constructor
	*/
	cocos_propvertshader( const std::wstring& full );

	/*
	//override
	*/
	virtual void				ntf_modify			( void );
	virtual std::string			get_dragtype		( void );
	virtual void*				get_dragdata		( void );
};

#endif//__onity_propvertshader_h__