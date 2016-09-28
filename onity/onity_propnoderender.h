#ifndef __onity_propnoderender_h__
#define __onity_propnoderender_h__

#include "onity_proproot.h"

class NPNode;
class onity_propnoderender : public onity_proproot
{
public:
	/*
	//constructor
	*/
	onity_propnoderender( NPNode* node );

	/*
	//method
	*/
	NPNode*			get_node( void );

protected:
	/*
	//member
	*/
	NPNode*			m_node;
};

#endif//__onity_propnoderender_h__