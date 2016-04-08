#ifndef __onity_proppath_h__
#define __onity_proppath_h__

#include "onity_propfile.h"

class onity_proppath : public onity_propfile
{
public:
	/*
	//constructor
	*/
	onity_proppath( const std::wstring& full );

protected:
	/*
	//member
	*/
	xui_propkind*	m_pathkind;
};

#endif//__onity_proppath_h__