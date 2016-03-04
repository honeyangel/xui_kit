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
xui_method_explain(xui_propdata,				get_kind,			xui_propkind*			)( void ) const
{
	return m_kind;
}
xui_method_explain(xui_propdata,				get_name,			const std::wstring&		)( void ) const
{
	return m_name;
}
xui_method_explain(xui_propdata,				set_name,			void					)( const std::wstring& name )
{
	m_name = name;
}
xui_method_explain(xui_propdata,				get_func,			xui_prop_newctrl		)( void ) const
{
	return m_func;
}
xui_method_explain(xui_propdata,				get_ctrl,			xui_propctrl*			)( void )
{
	return m_ctrl;
}
xui_method_explain(xui_propdata,				set_ctrl,			void					)( xui_propctrl* ctrl )
{
	m_ctrl = ctrl;
}
xui_method_explain(xui_propdata,				can_edit,			bool					)( void ) const
{
	return m_edit;
}
xui_method_explain(xui_propdata,				set_edit,			void					)( bool flag )
{
	m_edit = flag;
}
xui_method_explain(xui_propdata,				can_show,			bool					)( void ) const
{
	return m_show;
}
xui_method_explain(xui_propdata,				set_show,			void					)( bool flag )
{
	m_show = flag;
}

/*
//virtual
*/
xui_method_explain(xui_propdata,				on_valuechanged,	void					)( void )
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
xui_create_explain(xui_propdata_bool)( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr ) 
: xui_propdata(kind, name, func)
{
	m_userget = userget;
	m_userset = userset;
	m_userptr = userptr;
}

/*
//virtual
*/
xui_method_explain(xui_propdata_bool,			get_value,			bool					)( void ) const
{
	if (m_userget)
	{
		return (*m_userget)(m_userptr);
	}
	else
	{
		bool* ptr = (bool*)m_userptr;
		return (*ptr);
	}
}
xui_method_explain(xui_propdata_bool,			set_value,			void					)( bool value )
{
	if (get_value() != value)
	{
		if (m_userset)
		{
			(*m_userset)(m_userptr, value);
		}
		else
		{
			bool* ptr = (bool*)m_userptr;
			(*ptr) = value;
		}

		on_valuechanged();
	}
}

//////////////////////////////////////////////////////////////////////////
//propdata_stdstring
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(xui_propdata_string)( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr )
: xui_propdata(kind, name, func)
{
	m_userget = userget;
	m_userset = userset;
	m_userptr = userptr;
}

/*
//virtual
*/
xui_method_explain(xui_propdata_string,			get_value,			std::wstring			)( void ) const
{
	if (m_userget)
	{
		return (*m_userget)(m_userptr);
	}
	else
	{
		std::wstring* ptr = (std::wstring*)m_userptr;
		return (*ptr);
	}
}
xui_method_explain(xui_propdata_string,			set_value,			void					)( const std::wstring& value )
{
	if (get_value() != value)
	{
		if (m_userset)
		{
			(*m_userset)(m_userptr, value);
		}
		else
		{
			std::wstring* ptr = (std::wstring*)m_userptr;
			(*ptr) = value;
		}

		on_valuechanged();
	}
}

//////////////////////////////////////////////////////////////////////////
//propdata_number
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(xui_propdata_number)(xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, f64 interval, f64 minvalue, f64 maxvalue ) 
: xui_propdata(kind, name, func)
{
	m_interval = interval;
	m_minvalue = minvalue;
	m_maxvalue = maxvalue;
}

/*
//method
*/
xui_method_explain(xui_propdata_number,			get_interval,		f64						)( void ) const
{
	return m_interval;
}
xui_method_explain(xui_propdata_number,			get_minvalue,		f64						)( void ) const
{
	return m_minvalue;
}
xui_method_explain(xui_propdata_number,			get_maxvalue,		f64						)( void ) const
{
	return m_maxvalue;
}

/*
//constructor
*/
xui_create_explain(xui_propdata_number_func)( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr, f64 interval, f64 minvalue, f64 maxvalue )
: xui_propdata_number(kind, name, func, interval, minvalue, maxvalue)
{
	m_userget = userget;
	m_userset = userset;
	m_userptr = userptr;
}

/*
//override
*/
xui_method_explain(xui_propdata_number_func,	get_value,			f64						)( void ) const
{
	return (*m_userget)(m_userptr);
}
xui_method_explain(xui_propdata_number_func,	set_value,			void					)( f64 value )
{
	if (get_value() != value)
	{
		(*m_userset)(m_userptr, value);
		on_valuechanged();
	}
}

//////////////////////////////////////////////////////////////////////////
//propdata_enum
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(xui_propdata_enum)(xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, const xui_propenum_map& textmap ) 
: xui_propdata(kind, name, func)
{
	m_textmap = textmap;
}

/*
//method
*/
xui_method_explain(xui_propdata_enum,			get_textmap,		const xui_propenum_map&	)( void ) const
{
	return m_textmap;
}

/*
//constructor
*/
xui_create_explain(xui_propdata_enum_func)( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, const xui_propenum_map& textmap, get_func userget, set_func userset, void* userptr )
: xui_propdata_enum(kind, name, func, textmap)
{
	m_userget = userget;
	m_userset = userset;
	m_userptr = userptr;
}

/*
//override
*/
xui_method_explain(xui_propdata_enum_func,		get_index,			u32						)( void ) const
{
	u32 index = 0;
	s32 value = (*m_userget)(m_userptr);
	for (xui_propenum_map::const_iterator itor = m_textmap.begin(); itor != m_textmap.end(); ++itor)
	{
		if ((*itor).first == value)
			break;

		++index;
	}

	return index;
}
xui_method_explain(xui_propdata_enum_func,		set_index,			void					)( u32 index )
{
	if (get_index() != index)
	{
		xui_propenum_map::iterator itor = m_textmap.begin();
		std::advance(itor, index);
		(*m_userset)(m_userptr, (*itor).first);
		on_valuechanged();
	}
}

//////////////////////////////////////////////////////////////////////////
//propdata_expand
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(xui_propdata_expand)(xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, const xui_propdata_vec& subprop, bool subfold ) 
: xui_propdata(kind, name, func), xui_expandbase(subprop, subfold)
{}

/*
//constructor
*/
xui_create_explain(xui_propdata_expand_plus)( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, const xui_propdata_vec& subprop, bool subfold )
: xui_propdata_expand(kind, name, func, subprop, subfold)
{}

/*
//method
*/
xui_method_explain(xui_propdata_expand_plus,	add_subprop,		void					)( xui_propdata* propdata )
{
	m_subprop.push_back(propdata);
	refresh();
}
xui_method_explain(xui_propdata_expand_plus,	del_subprop,		void					)( xui_propdata* propdata )
{
	xui_propdata_vec::iterator itor = std::find(m_subprop.begin(), m_subprop.end(), propdata);
	if (itor != m_subprop.end())
	{
		m_subprop.erase(itor);
		delete propdata;
		refresh();
	}
}
xui_method_explain(xui_propdata_expand_plus,	del_subpropall,		void					)( void )
{
	for (u32 i = 0; i < m_subprop.size(); ++i)
		delete m_subprop[i];

	m_subprop.clear();
	refresh();
}

/*
//virtual
*/
xui_method_explain(xui_propdata_expand_plus,	refresh,			void					)( void )
{
	if (m_ctrl && m_ctrl->has_propdata(this))
	{
		m_ctrl->on_linkpropdata();
		m_ctrl->refresh();
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
//propdata_colour
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(xui_propdata_colour)( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, xui_colour* ptr )
: xui_propdata(kind, name, func)
{
	m_ptr = ptr;
}

/*
//virtual
*/
xui_method_explain(xui_propdata_colour,			get_value,			const xui_colour&		)( void ) const
{
	return (*m_ptr);
}
xui_method_explain(xui_propdata_colour,			set_value,			void					)( const xui_colour& value )
{
	if ((*m_ptr) != value)
	{
		(*m_ptr)  = value;
		on_valuechanged();
	}
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
xui_method_explain(xui_propdata_object,			get_droptype,		const std::string&		)( void ) const
{
	return m_droptype;
}
xui_method_explain(xui_propdata_object,			get_pickfunc,		xui_prop_newpick		)( void ) const
{
	return m_pickfunc;
}
xui_method_explain(xui_propdata_object,			get_iconfunc,		xui_prop_geticon		)( void ) const
{
	return m_iconfunc;
}
xui_method_explain(xui_propdata_object,			get_namefunc,		xui_prop_getname		)( void ) const
{
	return m_namefunc;
}