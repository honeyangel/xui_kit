#ifndef __onity_propcourse_h__
#define __onity_propcourse_h__

#include "onity_propfile.h"

class NP2DSSceneFile;
class onity_propcourse : public onity_propfile
{
public:
	/*
	//constructor
	*/
	onity_propcourse( const std::wstring& full );

	/*
	//destructor
	*/
	virtual ~onity_propcourse( void );

	/*
	//method
	*/
	NP2DSSceneFile*		get_scenefile	( void );

	/*
	//override
	*/
	virtual bool		was_modify		( void );
	virtual void		save			( void );
	virtual void		load			( void );

protected:
	/*
	//member
	*/
	NP2DSSceneFile*		m_scenefile;
};

#endif//__onity_propcourse_h__