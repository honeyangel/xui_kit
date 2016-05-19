#ifndef __onity_proptexture_h__
#define __onity_proptexture_h__

#include "onity_propfile.h"

class NPSourceTexture;
class onity_proptexture : public onity_propfile
{
public:
	/*
	//constructor
	*/
	onity_proptexture( const std::wstring& full );

	/*
	//method
	*/
	NPSourceTexture*		get_texture		( void );

	/*
	//override
	*/
	virtual std::string		get_dragtype	( void );
	virtual void*			get_dragdata	( void );
};

#endif//__onity_proptexture_h__