#include "NP2DSSceneFile.h"
#include "NPFileName.h"

#include "xui_treedata.h"
#include "xui_treenode.h"
#include "xui_propdata.h"
#include "xui_kindctrl.h"
#include "xui_textbox.h"
#include "onity_propctrl_transref.h"
#include "onity_proplevelconfig.h"

/*
//constructor
*/
xui_create_explain(onity_proplevelconfig)( NP2DSSceneFile* file )
: m_file(file)
, m_change(false)
, m_playerspeed(600)
, m_speedlevel(1)
, m_minspeedlevel(540)
, m_maxspeedlevel(640)
, m_bounsaddspeed(5)
, m_mudspeed(400)
, m_flystar(2)
{
	m_basekind = new xui_propkind(this, L"LevelConfig", "LevelConfig", xui_kindctrl::create, NULL, true);
	m_basekind->xm_propchanged += new xui_method_member<xui_method_propdata, onity_proplevelconfig>(this, &onity_proplevelconfig::on_propchanged);
	m_basekind->add_propdata(new onity_propdata_course(
		m_basekind,
		L"Level",
		&m_file));
	m_basekind->add_propdata(new xui_propdata_bool(
		m_basekind,
		L"Change",
		xui_propctrl_bool::create,
		NULL,
		NULL,
		&m_change));
	m_basekind->add_propdata(new xui_propdata_number_impl<u32>(
		m_basekind,
		L"PlayerSpeed",
		xui_propctrl_number::create,
		&m_playerspeed,
		NT_UNSIGNEDINT));
	m_basekind->add_propdata(new xui_propdata_number_impl<u32>(
		m_basekind,
		L"SpeedLevel",
		xui_propctrl_number::create,
		&m_speedlevel,
		NT_UNSIGNEDINT));
	m_basekind->add_propdata(new xui_propdata_number_impl<u32>(
		m_basekind,
		L"MinSpeedLevel",
		xui_propctrl_number::create,
		&m_minspeedlevel,
		NT_UNSIGNEDINT));
	m_basekind->add_propdata(new xui_propdata_number_impl<u32>(
		m_basekind,
		L"MaxSpeedLevel",
		xui_propctrl_number::create,
		&m_maxspeedlevel,
		NT_UNSIGNEDINT));
	m_basekind->add_propdata(new xui_propdata_number_impl<u32>(
		m_basekind,
		L"BounsAddSpeed",
		xui_propctrl_number::create,
		&m_bounsaddspeed,
		NT_UNSIGNEDINT));
	m_basekind->add_propdata(new xui_propdata_number_impl<u32>(
		m_basekind,
		L"MudSpeed",
		xui_propctrl_number::create,
		&m_mudspeed,
		NT_UNSIGNEDINT));
	m_basekind->add_propdata(new xui_propdata_number_impl<u32>(
		m_basekind,
		L"FlyStar",
		xui_propctrl_number::create,
		&m_flystar,
		NT_UNSIGNEDINT));

	add_propkind(m_basekind);
}

/*
//method
*/
xui_method_explain(onity_proplevelconfig, get_level,			NP2DSSceneFile*	)( void )
{
	return m_file;
}
xui_method_explain(onity_proplevelconfig, set_level,			void			)( NP2DSSceneFile* file )
{
	m_file = file;
}
xui_method_explain(onity_proplevelconfig, get_configstring,	std::string		)( void )
{
	std::string name = NPFileNameHelper::SafeName(m_file->GetName());
	std::stringstream text;
	text << (m_change ? 1 : 0);		text << ";";
	text << m_playerspeed;			text << ";";
	text << name;					text << ";";
	text << "1";					text << ";";
	text << m_speedlevel;			text << ";";
	text << m_minspeedlevel;		text << ";";
	text << m_maxspeedlevel;		text << ";";
	text << m_bounsaddspeed;		text << ";";
	text << m_mudspeed;				text << ";";
	text << m_flystar;				text << ";";
	text << "1";					text << ";";

	return text.str();
}

/*
//event
*/
xui_method_explain(onity_proplevelconfig, on_propchanged,		void			)( xui_component* sender, xui_method_propdata& args )
{
	if (args.propdata->get_name() == L"Course")
	{
		for (u32 i = 0; i < m_linkdatavec.size(); ++i)
		{
			m_linkdatavec[i]->get_node()->use_linkdata();
		}
	}
}