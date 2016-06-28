#ifndef __onity_propcontroller_h__
#define __onity_propcontroller_h__

#include "onity_propfile.h"

class NP2DSStateCtrl;
class onity_propcontroller : public onity_propfile
{
public:
	/*
	//constructor
	*/
	onity_propcontroller( const std::wstring& full );

	/*
	//destructor
	*/
	virtual ~onity_propcontroller( void );

	/*
	//method
	*/
	NP2DSStateCtrl*		get_statectrl	( void );
	void				ntf_detach		( xui_proproot* nextfile );

	/*
	//override
	*/
	virtual bool		was_modify		( void );
	virtual void		save			( void );
	virtual void		load			( void );
	virtual void		on_detach		( const xui_proproot_vec& proproot ){}

protected:
	/*
	//member
	*/
	NP2DSStateCtrl*		m_statectrl;
};

#endif//__onity_propcontroller_h__