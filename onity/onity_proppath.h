#ifndef __onity_proppath_h__
#define __onity_proppath_h__

#include "xui_propkind.h"

class onity_pathdata;
class onity_proppath : public xui_propkind
{
public:
	/*
	//constructor
	*/
	onity_proppath( xui_proproot* root, const std::wstring& name, onity_pathdata* pathdata );

	/*
	//method
	*/
	onity_pathdata*			get_pathdata	( void ) const;

protected:
	/*
	//member
	*/
	onity_pathdata*			m_pathdata;
};

#endif//__onity_proppath_h__