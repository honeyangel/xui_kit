#include <fstream>
#include "Omiga.h"
#include "Common/Common.h"
#include "Network/NPHostInfo.h"
#include "Game/GameConfig.h"

#include "xui_desktop.h"
#include "xui_global.h"
#include "xui_button.h"
#include "xui_toggle.h"
#include "xui_textbox.h"
#include "xui_dropbox.h"
#include "onity_config.h"

struct QQInfo
{
	std::string	strQQ;
	std::string	strPWD;
};
typedef std::vector<QQInfo>	TQQPWDMap;
typedef TQQPWDMap::iterator	TQQPWDMapIter;
TQQPWDMap gQQPWDMap;

struct ServerInfo
{
	std::string ServerName;
	std::string ServerIP;
	std::string ServerPort;
	std::string ServerUpdatePort;
};
typedef std::vector<ServerInfo> TServerList;
TServerList gServerList;

xui_implement_rtti(onity_config, xui_window);

/*
//constructor
*/
xui_create_explain(onity_config)( void )
: xui_window(xui_vector<s32>(320, 320))
{
	m_modal		= true;
	m_border	= xui_rect2d<s32>(10);
	m_sidestyle = SIDESTYLE_S;
	m_alignhorz = ALIGNHORZ_C;
	m_alignvert = ALIGNVERT_C;

	m_servtext	= new xui_drawer(xui_vector<s32>(32));
	m_account	= new xui_drawer(xui_vector<s32>(32));
	m_other		= new xui_drawer(xui_vector<s32>(32));
	m_qqstring	= new xui_drawer(xui_vector<s32>(24));
	m_pwstring	= new xui_drawer(xui_vector<s32>(24));
	m_version	= new xui_drawer(xui_vector<s32>(24));
	m_patchtext	= new xui_drawer(xui_vector<s32>(24));
	m_legaltext = new xui_drawer(xui_vector<s32>(24));
	xui_method_ptrcall(m_qqstring,	ini_drawer		)(L"Username");
	xui_method_ptrcall(m_pwstring,	ini_drawer		)(L"Password");
	xui_method_ptrcall(m_version,	ini_drawer		)(L"Version");
	xui_method_ptrcall(m_patchtext,	ini_drawer		)(L"Skip Update");
	xui_method_ptrcall(m_legaltext, ini_drawer		)(L"Skip Illegal Tip");
	xui_method_ptrcall(m_servtext,	ini_drawer		)(L"Choose Service");
	xui_method_ptrcall(m_account,	ini_drawer		)(L"Account");
	xui_method_ptrcall(m_other,		ini_drawer		)(L"Other");

	m_servlist	= xui_dropbox::create(200);
	m_username	= xui_dropbox::create(160);
	m_password	= xui_textbox::create(160);
	m_vernumb	= xui_textbox::create(60);
	xui_method_ptrcall(m_username,	set_readonly	)(false);
	xui_method_ptrcall(m_username,	set_numbtype	)(NT_UNSIGNEDINT);
	xui_method_ptrcall(m_username,	set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_username,	xm_selection	) += new xui_method_member<xui_method_args, onity_config>(this, &onity_config::on_usernameselection);
	xui_method_ptrcall(m_password,	set_password	)(true);
	xui_method_ptrcall(m_password,	set_textalign	)(TEXTALIGN_LC);
	xui_method_ptrcall(m_vernumb,	set_numbtype	)(NT_UNSIGNEDINT);
	xui_method_ptrcall(m_vernumb,	set_textalign	)(TEXTALIGN_LC);

	m_skippatch = xui_toggle::create();
	m_skiplegal = xui_toggle::create();

	m_accept	= new xui_button(xui_vector<s32>(128, 24));
	xui_method_ptrcall(m_accept, xm_buttonclick		) += new xui_method_member<xui_method_args, onity_config>(this, &onity_config::on_buttonclick);
	xui_method_ptrcall(m_accept, ini_component		)(ALIGNHORZ_C, ALIGNVERT_B, 0);
	xui_method_ptrcall(m_accept, set_sidestyle		)(SIDESTYLE_S);
	xui_method_ptrcall(m_accept, set_corner			)(3);
	xui_method_ptrcall(m_accept, set_borderrt		)(xui_rect2d<s32>(4));
	xui_method_ptrcall(m_accept, set_drawcolor		)(true);
	xui_method_ptrcall(m_accept, set_iconsize		)(xui_vector<s32>(0));
	xui_method_ptrcall(m_accept, set_textalign		)(TEXTALIGN_CC);
	xui_method_ptrcall(m_accept, ini_drawer			)(L"Confirm");

	add_child(m_servtext);
	add_child(m_servlist);
	add_child(m_account);
	add_child(m_qqstring);
	add_child(m_pwstring);
	add_child(m_username);
	add_child(m_password);
	add_child(m_other);
	add_child(m_version);
	add_child(m_vernumb);
	add_child(m_skippatch);
	add_child(m_patchtext);
	add_child(m_skiplegal);
	add_child(m_legaltext);
	add_child(m_accept);

	load_config();
}

/*
//callback
*/
xui_method_explain(onity_config, on_perform,			void)( xui_method_args& args )
{
	xui_window::on_perform(args);
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt = rt.get_pt();

	//server
	xui_method_ptrcall(m_servtext,	on_perform_pt)(pt);
	xui_method_ptrcall(m_servtext,	on_perform_w )(rt.get_w());
	pt.y += xui_method_ptrcall(m_servtext,	get_renderh)();
	xui_method_ptrcall(m_servlist,	on_perform_pt)(pt);
	xui_method_ptrcall(m_servlist,	on_perform_w )(rt.get_w());
	pt.y += xui_method_ptrcall(m_servlist,	get_renderh)();

	//account
	xui_method_ptrcall(m_account,	on_perform_pt)(pt);
	xui_method_ptrcall(m_account,	on_perform_w )(rt.get_w());
	pt.y += xui_method_ptrcall(m_account,	get_renderh)();
	xui_method_ptrcall(m_qqstring,	on_perform_x )(pt.x+10);
	xui_method_ptrcall(m_qqstring,	on_perform_y )(pt.y);
	xui_method_ptrcall(m_qqstring,	on_perform_w )(80);
	xui_method_ptrcall(m_username,	on_perform_x )(pt.x+80);
	xui_method_ptrcall(m_username,	on_perform_y )(pt.y);
	xui_method_ptrcall(m_username,	on_perform_w )(rt.get_w()-80);
	pt.y += xui_method_ptrcall(m_username,	get_renderh)() + 4;
	xui_method_ptrcall(m_pwstring,	on_perform_x )(pt.x+10);
	xui_method_ptrcall(m_pwstring,	on_perform_y )(pt.y);
	xui_method_ptrcall(m_pwstring,	on_perform_w )(80);
	xui_method_ptrcall(m_password,	on_perform_x )(pt.x+80);
	xui_method_ptrcall(m_password,	on_perform_y )(pt.y);
	xui_method_ptrcall(m_password,	on_perform_w )(rt.get_w()-80);
	pt.y += xui_method_ptrcall(m_password,	get_renderh)();

	//other
	xui_method_ptrcall(m_other,		on_perform_pt)(pt);
	xui_method_ptrcall(m_other,		on_perform_w )(rt.get_w());
	pt.y += xui_method_ptrcall(m_other,		get_renderh)();
	xui_method_ptrcall(m_version,	on_perform_x )(pt.x+10);
	xui_method_ptrcall(m_version,	on_perform_y )(pt.y);
	xui_method_ptrcall(m_version,	on_perform_w )(80);
	xui_method_ptrcall(m_vernumb,	on_perform_x )(pt.x+80);
	xui_method_ptrcall(m_vernumb,	on_perform_y )(pt.y);
	xui_method_ptrcall(m_vernumb,	on_perform_w )(40);
	pt.y += xui_method_ptrcall(m_version,	get_renderh)() + 4;
	xui_method_ptrcall(m_skippatch,	on_perform_x )(pt.x+10);
	xui_method_ptrcall(m_skippatch, on_perform_y )(pt.y+4);
	xui_method_ptrcall(m_patchtext, on_perform_x )(pt.x+32);
	xui_method_ptrcall(m_patchtext, on_perform_y )(pt.y);
	xui_method_ptrcall(m_patchtext, on_perform_w )(100);
	pt.y += xui_method_ptrcall(m_patchtext,	get_renderh)() + 4;
	xui_method_ptrcall(m_skiplegal,	on_perform_x )(pt.x+10);
	xui_method_ptrcall(m_skiplegal, on_perform_y )(pt.y+4);
	xui_method_ptrcall(m_legaltext, on_perform_x )(pt.x+32);
	xui_method_ptrcall(m_legaltext, on_perform_y )(pt.y);
	xui_method_ptrcall(m_legaltext, on_perform_w )(100);
}
xui_method_explain(onity_config, on_accept,				void)( xui_component* sender, xui_method_args& args )
{
	save_config();
	xui_window::on_accept(sender, args);
}

/*
//event
*/
xui_method_explain(onity_config, on_buttonclick,		void)( xui_component* sender, xui_method_args& args )
{
	extern std::string g_strQQ;
	extern std::string g_strPWD;
	g_strQQ  = xui_global::unicode_to_ascii(m_username->get_text());
	g_strPWD = xui_global::unicode_to_ascii(m_password->get_text());

	if(g_strQQ.empty() || g_strPWD.empty())
	{
		xui_desktop::get_ins()->show_message(L"Please input username and password", 1);
		return;
	}

	if(g_strPWD.find(' ') != std::string::npos)
	{
		xui_desktop::get_ins()->show_message(L"Password can not have space", 1);
		return;
	}

	if (m_vernumb->get_text().length() == 0)
	{
		xui_desktop::get_ins()->show_message(L"Please input version", 1);
		return;
	}

	xui_method_args other_args;
	on_accept(this, other_args);
}
xui_method_explain(onity_config, on_usernameselection,	void)( xui_component* sender, xui_method_args& args )
{
	std::string username = xui_global::unicode_to_ascii(m_username->get_text());
	std::string password;
	for(u32 i = 0; i < gQQPWDMap.size(); ++i)
	{
		if (gQQPWDMap[i].strQQ == username)
		{
			password = gQQPWDMap[i].strPWD;
			break;
		}
	}

	m_password->set_text(xui_global::ascii_to_unicode(password));
}

/*
//method
*/
xui_method_explain(onity_config, load_config,			void)( void )
{
	//server
	std::ifstream server("serverlist.win32");
	if (server)
	{
		gServerList.clear();
		while(!server.eof())
		{
			std::string serverName;
			std::string serverIP;
			std::string serverPort;
			std::string serverUpdatePort;
			server >> serverName >> serverIP >> serverPort >> serverUpdatePort;

			ServerInfo ser;
			ser.ServerName			= serverName;
			ser.ServerIP			= serverIP;
			ser.ServerPort			= serverPort;
			ser.ServerUpdatePort	= serverUpdatePort;
			gServerList.push_back(ser);
			std::string str = serverName+"("+serverIP+":"+serverPort+")";
			if (str.length() > 0)
				m_servlist->add_item(xui_global::ascii_to_unicode(str));
		}

		if (m_servlist->get_itemcount() > 0)
			m_servlist->set_selecteditem(m_servlist->get_item(0));
	}
	
	//account
	FILE* pFile = fopen("account.win32", "rb");
	if(pFile)
	{
		while( true )
		{
			char buffer[256];
			int readSize = fread(buffer, 1, 256, pFile);
			if (readSize != 256)
				break;

			char buffer1[256];
			char buffer2[256];
			sscanf(buffer, "%s %s", buffer1, buffer2);
			QQInfo info;
			info.strQQ  = buffer1;
			info.strPWD = buffer2;

			gQQPWDMap.push_back(info);
			m_username->add_item(xui_global::ascii_to_unicode(info.strQQ));
		}
		fclose(pFile);

		if (m_username->get_itemcount() > 0)
			m_username->set_selecteditem(m_username->get_item(0));
	}

	//other
	std::string version;
	std::string igpatch;
	std::string iglegal;
	std::ifstream fileOther("other.txt");
	if (fileOther)
	{	
		fileOther >> version >> igpatch >> iglegal;
	}
	if (version.empty())
		version = "35";
	m_vernumb->set_text(xui_global::ascii_to_unicode(version));
	if (igpatch.empty())
		igpatch = "0";
	m_skippatch->set_push(igpatch == "1");
	if (iglegal.empty())
		iglegal = "0";
	m_skiplegal->set_push(iglegal == "1");

	//select
	u32 select1 = 0;
	u32 select2 = 0;
	u32 select3 = 0;
	std::ifstream file2("select.win32");
	if( file2 )
	{
		char text[64] = {0};
		file2.read(text, 64);
		sscanf(text, "%d,%d,%d", &select1, &select2, &select3);
	}
	file2.close();

	if (select2 < m_servlist->get_itemcount())
		m_servlist->set_selecteditem(m_servlist->get_item(select2));
	if (select3 < m_username->get_itemcount())
		m_username->set_selecteditem(m_username->get_item(select3));
}
xui_method_explain(onity_config, save_config,			void)( void )
{
	extern std::string g_strQQ;
	extern std::string g_strPWD;

	bool found = false;
	for (u32 i = 0; i < gQQPWDMap.size(); ++i)
	{
		if (gQQPWDMap[i].strQQ == g_strQQ)
		{
			found = true;
			gQQPWDMap[i].strPWD = g_strPWD;
			break;
		}
	}
	if (found == false)
	{
		QQInfo info;
		info.strQQ  = g_strQQ;
		info.strPWD = g_strPWD;
		gQQPWDMap.push_back(info);
	}

	u32 select1 = 0;
	u32 select2 = m_servlist->get_selectedindex();
	if (select2 >= 0)
	{
		//Game server
		std::vector<NPHostInfo> hostInfoVec;
		NPHostInfo info;
		info.m_timeout = 20;
		strcpy(info.m_host, gServerList[select2].ServerIP.c_str());
		info.m_port = BreezeGame::StrToUInt(gServerList[select2].ServerPort);
		hostInfoVec.push_back(info);
		BreezeGame::GameConfig::Create();
		BreezeGame::GameConfig::Instance()->SetQQHostVec(hostInfoVec);

		//Update server
		info.m_port = BreezeGame::StrToUInt(gServerList[select2].ServerUpdatePort);
		BreezeGame::GameConfig::Instance()->SetWINUpdateHostVec(info);
	}

	u32 select3 = m_username->get_selectedindex();
	if (select3 < 0)
		select3 = 0;

	//Save local config
	std::ofstream file("select.win32");
	if (file)
	{
		char text[64] = {0};
		sprintf(text, "%d,%d,%d", select1, select2, select3);
		file << text;
	}
	file.close();

	//Save QQ and password
	FILE* pFile = fopen("account.win32", "wb+");
	if (pFile)
	{
		for (u32 i = 0; i < gQQPWDMap.size(); ++i)
		{
			char buffer[256];
			sprintf(buffer, "%s %s", gQQPWDMap[i].strQQ.c_str(), gQQPWDMap[i].strPWD.c_str());
			fwrite(buffer, 1, 256, pFile);
		}
		fclose(pFile);
	}

	std::ofstream fileOther("other.txt");
	if (fileOther)
	{
		std::string version = xui_global::unicode_to_ascii(m_vernumb->get_text());
		extern std::string DebugPCVersion;
		DebugPCVersion = "1.0." + version + std::string(".0");

		char text[64] = { 0 };
		sprintf(text, "%s %d %d", version.c_str(), (m_skippatch->was_push() ? 1 : 0), (m_skiplegal->was_push() ? 1 : 0));
		fileOther << text;
		fileOther.close();
	}

	extern bool SkipUpdate;
	extern bool SkipMd5Check;
	SkipUpdate	 = m_skippatch->was_push();
	SkipMd5Check = m_skiplegal->was_push();
}