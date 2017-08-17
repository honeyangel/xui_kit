#ifndef __cocos_propcsd_h__
#define __cocos_propcsd_h__

#include "cocos_propfile.h"

namespace cocos2d
{
	class Node;
}

class cocos_propnodebase;
class cocos_propcsd : public cocos_propfile
{
public:
	/*
	//constructor
	*/
	cocos_propcsd( const std::wstring& full );

	/*
	//destructor
	*/
	virtual ~cocos_propcsd( void );

	/*
	//method
	*/
	cocos2d::Node*			get_node			( void );
	cocos_propnodebase*		get_prop			( void );
	void					set_modify			( bool flag );

	/*
	//override
	*/
	virtual void			ntf_rename			( const std::wstring& last, const std::wstring& curr );
	virtual bool			was_modify			( void );
	virtual std::string		get_dragtype		( void );
	virtual void*			get_dragdata		( void );
	virtual void			load				( void );

protected:
	/*
	//override
	*/
	virtual void			save_as				( const std::wstring& fullname, bool modify );
	virtual void			on_detach			( const xui_proproot_vec& proproot );

	/*
	//member
	*/
	bool					m_modify;
	cocos2d::Node*			m_node;
	cocos_propnodebase*		m_prop;
};

#endif//__cocos_propcsd_h__