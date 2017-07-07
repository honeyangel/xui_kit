#ifndef __cocos_propleaf_h__
#define __cocos_propleaf_h__

#include "cocos_proproot.h"

class cocos_savekind;
class cocos_propfile;
class cocos_propleaf : public cocos_proproot
{
public:
	/*
	//constructor
	*/
	cocos_propleaf( cocos_propfile* propfile );

	/*
	//method
	*/
	cocos_propfile*			get_propfile	( void );

	/*
	//virtual
	*/
	virtual std::string		get_dragtype	( void );
	virtual void*			get_dragdata	( void );

	/*
	//override
	*/
	virtual void			on_detach		( const xui_proproot_vec& proproot );

protected:
	/*
	//member
	*/
	cocos_savekind*			m_savekind;
};

#endif//__cocos_propleaf_h__