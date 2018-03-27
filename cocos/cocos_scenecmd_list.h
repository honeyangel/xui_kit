#ifndef __cocos_scenecmd_list_h__
#define __cocos_scenecmd_list_h__

#include "cocos_scenecmd.h"

class cocos_scenecmd_list : public cocos_scenecmd
{
public:
	/*
	//constructor
	*/
	cocos_scenecmd_list( void );

	/*
	//destructor
	*/
	virtual ~cocos_scenecmd_list( void );

	/*
	//method
	*/
	void						push( cocos_scenecmd* cmd );

	/*
	//override
	*/
	virtual void				undo( void );
	virtual void				redo( void );

protected:
	/*
	//member
	*/
	std::list<cocos_scenecmd*>	m_cmdlist;
};

#endif//__cocos_scenecmd_list_h__