#ifndef __onity_proplevelconfig_h__
#define __onity_proplevelconfig_h__

#include "onity_proproot.h"

class NP2DSSceneFile;
class onity_proplevelconfig : public onity_proproot
{
public:
	/*
	//constructor
	*/
	onity_proplevelconfig( NP2DSSceneFile* file );

	/*
	//method
	*/
	NP2DSSceneFile*		get_level			( void );
	void				set_level			( NP2DSSceneFile* file );
	std::string			get_configstring	( void );

protected:
	/*
	//event
	*/
	void				on_propchanged		( xui_component* sender, xui_method_propdata& args );

	/*
	//member
	*/
	NP2DSSceneFile*		m_file;
	bool				m_change;
	u32					m_playerspeed;
	u32					m_speedlevel;
	u32					m_minspeedlevel;
	u32					m_maxspeedlevel;
	u32					m_bounsaddspeed;
	u32					m_mudspeed;
	u32					m_flystar;
	xui_propkind*		m_basekind;
};

#endif//__onity_proplevelconfig_h__