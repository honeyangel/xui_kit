#ifndef __onity_propcourseconfig_h__
#define __onity_propcourseconfig_h__

#include "onity_proproot.h"

class NP2DSSceneFile;
class onity_propcourseconfig : public onity_proproot
{
public:
	/*
	//constructor
	*/
	onity_propcourseconfig( NP2DSSceneFile* course );

	/*
	//method
	*/
	NP2DSSceneFile*		get_course			( void );
	void				set_course			( NP2DSSceneFile* course );
	std::string			get_configstring	( void );

protected:
	/*
	//event
	*/
	void				on_propchanged		( xui_component* sender, xui_method_propdata& args );

	/*
	//member
	*/
	bool				m_change;
	u32					m_playerspeed;
	NP2DSSceneFile*		m_course;
	u32					m_speedlevel;
	u32					m_minspeedlevel;
	u32					m_maxspeedlevel;
	u32					m_bounsaddspeed;
	u32					m_mudspeed;
	u32					m_flystar;
	xui_propkind*		m_basekind;
};

#endif//__onity_propcourseconfig_h__