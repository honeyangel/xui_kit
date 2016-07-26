#include "xui_global.h"
#include "xui_numbbox.h"
#include "xui_propctrl.h"
#include "xui_desktop.h"
#include "onity_propkind_compold.h"

//////////////////////////////////////////////////////////////////////////
//propkind
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(onity_propkind_compold)( xui_proproot* root, const std::wstring& name, xui_bitmap* icon, BreezeGame::Json::Value* node )
: xui_propkind(root, name, "ComponentOld", onity_kindctrl_compold::create, icon, true)
, m_node(node)
{
	std::vector<std::string> members = m_node->getMemberNames();
	for (u32 i = 0; i < members.size(); ++i)
	{
		std::string member = members[i];
		if (member == "ClassName" ||
			member == "Family")
			continue;

		BreezeGame::Json::Value* data = &((*m_node)[member]);
		if (data->isNumeric())
		{
			add_propdata(new xui_propdata_number_func(
				this,
				xui_global::ascii_to_unicode(member),
				xui_propctrl_number::create,
				get_number,
				set_number,
				data,
				NT_FLOAT));
		}
		else
		if (data->isBool())
		{
			add_propdata(new xui_propdata_bool(
				this, 
				xui_global::ascii_to_unicode(member), 
				xui_propctrl_bool::create, 
				get_bool, 
				set_bool, 
				data));
		}
		else
		if (data->isString())
		{
			add_propdata(new xui_propdata_string(
				this, 
				xui_global::ascii_to_unicode(member),
				xui_propctrl_string::create,
				get_string,
				set_string,
				data));
		}
	}
}

/*
//method
*/
xui_method_explain(onity_propkind_compold, get_node,			BreezeGame::Json::Value*)( void )
{
	return m_node;
}

/*
//static
*/
xui_method_explain(onity_propkind_compold, get_bool,			bool					)( void* userptr )
{
	BreezeGame::Json::Value* data = (BreezeGame::Json::Value*)userptr;
	return data->asBool();
}
xui_method_explain(onity_propkind_compold, set_bool,			void					)( void* userptr, bool value )
{
	BreezeGame::Json::Value* data = (BreezeGame::Json::Value*)userptr;
	*data = BreezeGame::Json::Value(value);
}
xui_method_explain(onity_propkind_compold, get_number,			f64						)( void* userptr )
{
	BreezeGame::Json::Value* data = (BreezeGame::Json::Value*)userptr;
	return data->asDouble();
}
xui_method_explain(onity_propkind_compold, set_number,			void					)( void* userptr, f64  value )
{
	BreezeGame::Json::Value* data = (BreezeGame::Json::Value*)userptr;
	*data = BreezeGame::Json::Value(value);
}
xui_method_explain(onity_propkind_compold, get_string,			std::wstring			)( void* userptr )
{
	BreezeGame::Json::Value* data = (BreezeGame::Json::Value*)userptr;
	return xui_global::ascii_to_unicode(data->asString());
}
xui_method_explain(onity_propkind_compold, set_string,			void					)( void* userptr, const std::wstring& value )
{
	BreezeGame::Json::Value* data = (BreezeGame::Json::Value*)userptr;
	*data = BreezeGame::Json::Value(xui_global::unicode_to_ascii(value));
}

//////////////////////////////////////////////////////////////////////////
//kindctrl
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(onity_kindctrl_compold, xui_kindctrl);

/*
//constructor
*/
xui_create_explain(onity_kindctrl_compold)( xui_propkind* propkind )
: xui_kindctrl(propkind)
{}

/*
//static
*/
xui_method_explain(onity_kindctrl_compold, create,				xui_kindctrl*			)( xui_propkind* propkind )
{
	return new onity_kindctrl_compold(NULL);
}

/*
//override
*/
xui_method_explain(onity_kindctrl_compold, set_propkindimpl,	void					)( void )
{
	for (u32 i = 0; i < m_propctrlvec.size(); ++i)
	{
		xui_propctrl* propctrl = m_propctrlvec[i];
		xui_propdata_vec vec = propctrl->get_propdata();
		for (xui_propdata_vec::iterator itor = vec.begin(); itor != vec.end(); ++itor)
			(*itor)->non_ctrl();

		std::vector<xui_component*>::iterator itor = std::find(
			m_widgetvec.begin(), 
			m_widgetvec.end(), 
			propctrl);

		if (itor != m_widgetvec.end())
		{
			m_widgetvec.erase(itor);
			propctrl->set_parent(NULL);
			xui_desktop::get_ins()->move_recycle(propctrl);
		}
	}
	m_propctrlvec.clear();

	bool same = true;
	onity_propkind_compold* propkind = dynamic_cast<onity_propkind_compold*>(m_propkind);
	BreezeGame::Json::Value* node = propkind->get_node();
	for (u32 i = 1; i < m_propkindvec.size(); ++i)
	{
		onity_propkind_compold* tempkind = dynamic_cast<onity_propkind_compold*>(m_propkindvec[i]);
		if (tempkind->get_node() != node)
		{
			same = false;
			break;
		}
	}

	if (same)
	{
		const xui_propdata_vec& vec = m_propkind->get_propdata();
		for (u32 i = 0; i < vec.size(); ++i)
		{
			xui_prop_newctrl  func = vec[i]->get_func();
			xui_propctrl* propctrl = (*func)(vec[i]);
			propctrl->refresh();
			propctrl->set_parent(this);
			m_widgetvec.push_back(propctrl);
			m_propctrlvec.push_back(propctrl);
		}

		xui_kindctrl::set_propkindimpl();
	}
}