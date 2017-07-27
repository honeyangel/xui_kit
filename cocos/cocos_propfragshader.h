#ifndef __cocos_propfragshader_h__
#define __cocos_propfragshader_h__

#include "cocos_propfile.h"

class cocos_propfragshader : public cocos_propfile
{
public:
	/*
	//constructor
	*/
	cocos_propfragshader( const std::wstring& full );

	/*
	//override
	*/
	virtual void				ntf_modify			( void );
	virtual std::string			get_dragtype		( void );
	virtual void*				get_dragdata		( void );
};

#endif//__onity_propfragshader_h__