#ifndef __onity_propframekey_h__
#define __onity_propframekey_h__

#include "onity_propleaf.h"

class NP2DSFrameKey;
class onity_propframekey : public onity_propleaf
{
public:
	/*
	//constructor
	*/
	onity_propframekey( onity_propfile* propfile, NP2DSFrameKey* framekey );

	/*
	//method
	*/
	NP2DSFrameKey*		get_framekey( void );

protected:
	/*
	//member
	*/
	xui_propkind*		m_basekind;
	NP2DSFrameKey*		m_framekey;
};

#endif//__onity_propframekey_h__