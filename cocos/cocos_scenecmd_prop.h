#ifndef __cocos_scenecmd_prop_h__
#define __cocos_scenecmd_prop_h__

#include "cocos_scenecmd.h"

class cocos_proproot;
class cocos_scenecmd_prop : public cocos_scenecmd
{
public:
	/*
	//constructor
	*/
	cocos_scenecmd_prop( cocos_proproot* root, xui_propdata* data );

	/*
	//destructor
	*/
	virtual ~cocos_scenecmd_prop( void );

	/*
	//override
	*/
	virtual void	undo( void );
	virtual void	redo( void );

protected:
	/*
	//member
	*/
	cocos_proproot*	m_root;
	std::wstring	m_path;
	u08*			m_undo;
	u08*			m_redo;
};

#endif//__cocos_scenecmd_prop_h__