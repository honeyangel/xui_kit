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
xui_method_explain(xui_propdata,				non_ctrl,			void					)( void )
{
	m_ctrl = NULL;
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
//refresh
*/
xui_method_explain(xui_propdata,				refresh,			void					)( void )
{
	if (m_ctrl && m_ctrl->has_propdata(this))
	{
		m_ctrl->on_linkpropdata();
		m_ctrl->refresh();
	}
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
	set_edit(!(userget != NULL && userset == NULL));
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
	set_edit(!(userget != NULL && userset == NULL));
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
xui_create_explain(xui_propdata_number)(xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, u08 numbtype, f64 interval, f64 minvalue, f64 maxvalue ) 
: xui_propdata(kind, name, func)
{
	m_interval = interval;
	m_minvalue = minvalue;
	m_maxvalue = maxvalue;
	m_numbtype = numbtype;
}

/*
//method
*/
xui_method_explain(xui_propdata_number,			get_numbtype,		u08						)( void ) const
{
	return m_numbtype;
}
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
xui_create_explain(xui_propdata_number_func)( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr, u08 numbtype, f64 interval, f64 minvalue, f64 maxvalue )
: xui_propdata_number(kind, name, func, numbtype, interval, minvalue, maxvalue)
{
	m_userget = userget;
	m_userset = userset;
	m_userptr = userptr;
	set_edit(m_userset != NULL);
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
	set_edit(m_userset != NULL);
}

/*
//override
*/
xui_method_explain(xui_propdata_enum_func,		get_value,			u32						)( void ) const
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
xui_method_explain(xui_propdata_enum_func,		set_value,			void					)( u32 index )
{
	if (get_value() != index)
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
xui_method_explain(xui_propdata_expand,			non_ctrl,			void					)( void )
{
	xui_propdata::non_ctrl();
	for (u32 i = 0; i < m_subprop.size(); ++i)
		m_subprop[i]->non_ctrl();
}

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

//////////////////////////////////////////////////////////////////////////
//propdata_stdvec
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

/*
//method
*/
xui_method_explain(xui_propdata_stdvec,			get_propvec,		const xui_propdata_vec&	)( void ) const
{
	return m_propvec;
}

/*
//override
*/
xui_method_explain(xui_propdata_stdvec,			non_ctrl,			void					)( void )
{
	xui_propdata::non_ctrl();
	for (u32 i = 0; i < m_propvec.size(); ++i)
		m_propvec[i]->non_ctrl();
}

//////////////////////////////////////////////////////////////////////////
//propdata_vector
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(xui_propdata_vector)(xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr, u08 numbtype, f64 interval , const xui_vector<f64>& defvalue  ) 
: xui_propdata(kind, name, func)
{
	m_userget	= userget;
	m_userset	= userset;
	m_userptr	= userptr;
	m_defvalue	= defvalue;
	m_interval	= interval;
	m_numbtype	= numbtype;
	set_edit(m_userset != NULL);
}

/*
//method
*/
xui_method_explain(xui_propdata_vector,			get_numbtype,		u08						)( void ) const
{
	return m_numbtype;
}
xui_method_explain(xui_propdata_vector,			get_interval,		f64						)( void ) const
{
	return m_interval;
}
xui_method_explain(xui_propdata_vector,			set_defvalue,		void					)( void )
{
	set_value(m_defvalue);
}
xui_method_explain(xui_propdata_vector,			get_value,			xui_vector<f64>			)( void ) const
{
	return (*m_userget)(m_userptr);
}
xui_method_explain(xui_propdata_vector,			set_value,			void					)( const xui_vector<f64>& value )
{
	if (get_value() != value)
	{
		(*m_userset)(m_userptr, value);
		on_valuechanged();
	}
}

//////////////////////////////////////////////////////////////////////////
//propdata_rect2d
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(xui_propdata_rect2d)(xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr, u08 numbtype, f64 interval ) 
: xui_propdata(kind, name, func)
{
	m_userget	= userget;
	m_userset	= userset;
	m_userptr	= userptr;
	m_interval	= interval;
	m_numbtype	= numbtype;
	set_edit(m_userset != NULL);
}

/*
//method
*/
xui_method_explain(xui_propdata_rect2d,			get_numbtype,		u08						)( void ) const
{
	return m_numbtype;
}
xui_method_explain(xui_propdata_rect2d,			get_interval,		f64						)( void ) const
{
	return m_interval;
}
xui_method_explain(xui_propdata_rect2d,			get_value,			xui_rect2d<f64>			)( void ) const
{
	return (*m_userget)(m_userptr);
}
xui_method_explain(xui_propdata_rect2d,			set_value,			void					)( const xui_rect2d<f64>& value )
{
	if (get_value() != value)
	{
		(*m_userset)(m_userptr, value);
		on_valuechanged();
	}
}

//////////////////////////////////////////////////////////////////////////
//propdata_colour
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(xui_propdata_colour)( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr )
: xui_propdata(kind, name, func)
{
	m_userget = userget;
	m_userset = userset;
	m_userptr = userptr;
	set_edit(m_userset != NULL);
}

/*
//virtual
*/
xui_method_explain(xui_propdata_colour,			get_value,			xui_colour				)( void ) const
{
	return (*m_userget)(m_userptr);
}
xui_method_explain(xui_propdata_colour,			set_value,			void					)( const xui_colour& value )
{
	if (get_value() != value)
	{
		(*m_userset)(m_userptr, value);
		on_valuechanged();
	}
}

//////////////////////////////////////////////////////////////////////////
//propdata_object
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(xui_propdata_object)( 
	xui_propkind*			kind, 
	const std::wstring&		name, 
	xui_prop_newctrl		func, 
	const std::string&		droptype, 
	xui_prop_newpick		pickfunc, 
	xui_prop_geticon		iconfunc, 
	xui_prop_getname		namefunc )
: xui_propdata(kind, name, func)
{
	m_pickfunc = pickfunc;
	m_iconfunc = iconfunc;
	m_namefunc = namefunc;
	m_oldvalue = NULL;
	m_droptype.push_back(droptype);
}

/*
//method
*/
xui_method_explain(xui_propdata_object,			has_droptype,		bool					)( const std::string& type ) const
{
	for (u32 i = 0; i < m_droptype.size(); ++i)
	{
		if (m_droptype[i] == type)
			return true;
	}

	return false;
}
xui_method_explain(xui_propdata_object,			add_droptype,		void					)( const std::string& type )
{
	m_droptype.push_back(type);
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
xui_method_explain(xui_propdata_object,			old_value,			void					)( void )
{
	set_value(m_oldvalue);
}
xui_method_explain(xui_propdata_object,			syn_value,			void					)( void )
{
	m_oldvalue = get_value();
}

/*
//constructor
*/
xui_create_explain(xui_propdata_object_func)( 
	xui_propkind*			kind, 
	const std::wstring&		name, 
	xui_prop_newctrl		func, 
	const std::string&		droptype, 
	xui_prop_newpick		pickfunc, 
	xui_prop_geticon		iconfunc, 
	xui_prop_getname		namefunc, 
	get_func				userget, 
	set_func				userset, 
	void*					userptr )
: xui_propdata_object(kind, name, func, droptype, pickfunc, iconfunc, namefunc)
{
	m_userget = userget;
	m_userset = userset;
	m_userptr = userptr;
	set_edit(m_userset != NULL);
}

/*
//override
*/
xui_method_explain(xui_propdata_object_func,	get_value,			void*					)( void ) const
{
	return (*m_userget)(m_userptr);
}
xui_method_explain(xui_propdata_object_func,	set_value,			void					)( void* value )
{
	void* tempvalue = get_value();
	if (tempvalue != value)
	{
		m_oldvalue = tempvalue;
		(*m_userset)(m_userptr, value);
		on_valuechanged();
	}
}