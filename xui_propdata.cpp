#include "xui_propctrl.h"
#include "xui_propkind.h"
#include "xui_propdata.h"

//////////////////////////////////////////////////////////////////////////
//propdata
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(xui_propdata)( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func )
{
	m_kind = kind;
	m_name = name;
	m_func = func;
	m_ctrl = NULL;
	m_show = true;
	m_edit = true;
}

/*
//method
*/
xui_method_explain(xui_propdata,		get_kind,			xui_propkind*		)( void ) const
{
	return m_kind;
}
xui_method_explain(xui_propdata,		get_name,			const std::wstring&	)( void ) const
{
	return m_name;
}
xui_method_explain(xui_propdata,		set_name,			void				)( const std::wstring& name )
{
	m_name = name;
}
xui_method_explain(xui_propdata,		get_func,			xui_prop_newctrl	)( void ) const
{
	return m_func;
}
xui_method_explain(xui_propdata,		get_ctrl,			xui_propctrl*		)( void )
{
	return m_ctrl;
}
xui_method_explain(xui_propdata,		set_ctrl,			void				)( xui_propctrl* ctrl )
{
	m_ctrl = ctrl;
}
xui_method_explain(xui_propdata,		can_edit,			bool				)( void ) const
{
	return m_edit;
}
xui_method_explain(xui_propdata,		set_edit,			void				)( bool flag )
{
	m_edit = flag;
}
xui_method_explain(xui_propdata,		can_show,			bool				)( void ) const
{
	return m_show;
}
xui_method_explain(xui_propdata,		set_show,			void				)( bool flag )
{
	m_show = flag;
}

/*
//virtual
*/
xui_method_explain(xui_propdata,		on_valuechanged,	void				)( void )
{
	if (m_kind)
	{
		xui_method_propdata args;
		args.propdata = this;
		m_kind->xm_propchanged(m_ctrl, args);
	}
}

//////////////////////////////////////////////////////////////////////////
//propdata_bool
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(xui_propdata_bool)( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, bool* ptr ) 
: xui_propdata(kind, name, func)
{
	m_ptr = ptr;
}

/*
//virtual
*/
xui_method_explain(xui_propdata_bool,	get_value,			bool				)( void ) const
{
	return (*m_ptr);
}
xui_method_explain(xui_propdata_bool,	set_value,			void				)( bool value )
{
	if ((*m_ptr) != value)
	{
		(*m_ptr)  = value;
		on_valuechanged();
	}
}

//////////////////////////////////////////////////////////////////////////
//propdata_stdstring
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(xui_propdata_string)( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, std::wstring* ptr )
: xui_propdata(kind, name, func)
{
	m_ptr = ptr;
}

/*
//virtual
*/
xui_method_explain(xui_propdata_string,	get_value,			const std::wstring&	)( void ) const
{
	return (*m_ptr);
}
xui_method_explain(xui_propdata_string,	set_value,			void				)( const std::wstring& value )
{
	if ((*m_ptr) != value)
	{
		(*m_ptr)  = value;
		on_valuechanged();
	}
}

//////////////////////////////////////////////////////////////////////////
//propdata_stdvector
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(xui_propdata_stdvec)( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, xui_prop_additem addfunc, xui_prop_delitem delfunc, xui_prop_newprop newfunc )
: xui_propdata(kind, name, func)
{
	m_additem = addfunc;
	m_delitem = delfunc;
	m_newprop = newfunc;
}

/*
//destructor
*/
xui_delete_explain(xui_propdata_stdvec)( void )
{
	for (u32 i = 0; i < m_propvec.size(); ++i)
		delete m_propvec[i];
}

//////////////////////////////////////////////////////////////////////////
//propdata_object
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(xui_propdata_object)( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, const std::string& droptype, xui_prop_newpick pickfunc, xui_prop_geticon iconfunc, xui_prop_getname namefunc )
: xui_propdata(kind, name, func)
{
	m_droptype = droptype;
	m_pickfunc = pickfunc;
	m_iconfunc = iconfunc;
	m_namefunc = namefunc;
}

/*
//method
*/
xui_method_explain(xui_propdata_object, get_droptype,		const std::string&	)( void ) const
{
	return m_droptype;
}
xui_method_explain(xui_propdata_object, get_pickfunc,		xui_prop_newpick	)( void ) const
{
	return m_pickfunc;
}
xui_method_explain(xui_propdata_object, get_iconfunc,		xui_prop_geticon	)( void ) const
{
	return m_iconfunc;
}
xui_method_explain(xui_propdata_object, get_namefunc,		xui_prop_getname	)( void ) const
{
	return m_namefunc;
}