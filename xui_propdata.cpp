#include "xui_propctrl.h"
#include "xui_propkind.h"
#include "xui_proproot.h"
#include "xui_propdata.h"

//////////////////////////////////////////////////////////////////////////
//propdata
//////////////////////////////////////////////////////////////////////////
xui_propdata::xui_propdata( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func )
{
	m_kind = kind;
	m_name = name;
	m_func = func;
	m_ctrl = NULL;
	m_show = true;
	m_edit = true;
	m_byte = NULL;
}

xui_propdata::~xui_propdata( void )
{
	delete [] m_byte;
}

xui_propkind* xui_propdata::get_kind( void ) const
{
	return m_kind;
}

const std::wstring& xui_propdata::get_name( void ) const
{
	return m_name;
}

void xui_propdata::set_name( const std::wstring& name )
{
	m_name = name;
}

xui_prop_newctrl xui_propdata::get_func( void ) const
{
	return m_func;
}

xui_propctrl* xui_propdata::get_ctrl( void )
{
	return m_ctrl;
}

void xui_propdata::set_ctrl( xui_propctrl* ctrl )
{
	m_ctrl = ctrl;
}

u08* xui_propdata::get_byte( void )
{
	u08* temp = m_byte;
	m_byte = NULL;
	return temp;
}

bool xui_propdata::has_byte( void ) const
{
	return (m_byte != NULL);
}

void xui_propdata::non_ctrl( void )
{
	if (m_ctrl)
	{
		m_ctrl->del_propdata();
		m_ctrl = NULL;
	}
}

xui_propdata* xui_propdata::get_data( const std::wstring& name )
{
	return NULL;
}

std::wstring xui_propdata::get_path( void )
{
	return m_kind->get_proppath(this);
}

std::wstring xui_propdata::get_path( xui_propdata* prop )
{
	return (prop == this) ? (L"/"+m_name) : (L"");
}

bool xui_propdata::can_edit( void ) const
{
	return m_edit;
}

void xui_propdata::set_edit( bool flag )
{
	m_edit = flag;
}

bool xui_propdata::can_show( void ) const
{
	return m_show;
}

void xui_propdata::set_show( bool flag )
{
	m_show = flag;
}

void xui_propdata::backups( void )
{
	if (m_byte != NULL)
	{
		delete[] m_byte;
	}

	m_byte = do_serialize();
}

void xui_propdata::restore( u08* byte )
{
	if (m_byte != NULL)
	{
		delete[] m_byte;
        m_byte  = NULL;
	}

	un_serialize(byte);
	refresh();
}

void xui_propdata::refresh( void )
{
	if (m_ctrl && m_ctrl->has_propdata(this))
	{
		m_ctrl->on_linkpropdata();
		m_ctrl->refresh();
	}
}

void xui_propdata::on_valuechanged( void )
{
	if (m_kind)
	{
		xui_method_propdata args;
		args.propdata = this;
		m_kind->xm_propchanged(m_ctrl, args);
	}
}

u08* xui_propdata::do_serialize( void )
{
	return NULL;
}

void xui_propdata::un_serialize( u08* byte )
{}

//////////////////////////////////////////////////////////////////////////
//propdata_bool
//////////////////////////////////////////////////////////////////////////
xui_propdata_bool::xui_propdata_bool( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr ) 
: xui_propdata(kind, name, func)
{
	m_userget = userget;
	m_userset = userset;
	m_userptr = userptr;
	set_edit(!(userget != NULL && userset == NULL));
}

bool xui_propdata_bool::get_value( void ) const
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

void xui_propdata_bool::set_value( bool value )
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

u08* xui_propdata_bool::do_serialize( void )
{
	return get_byte<bool>(get_value());
}

void xui_propdata_bool::un_serialize( u08* byte )
{
	if (m_userset)
	{
		(*m_userset)(m_userptr, get_cast<bool>(byte));
	}
	else
	{
		bool* ptr = (bool*)m_userptr;
		(*ptr) = get_cast<bool>(byte);
	}
}

//////////////////////////////////////////////////////////////////////////
//propdata_stdstring
//////////////////////////////////////////////////////////////////////////
xui_propdata_string::xui_propdata_string( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr )
: xui_propdata(kind, name, func)
{
	m_userget = userget;
	m_userset = userset;
	m_userptr = userptr;
	set_edit(!(userget != NULL && userset == NULL));
}

std::wstring xui_propdata_string::get_value( void ) const
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

void xui_propdata_string::set_value( const std::wstring& value )
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

u08* xui_propdata_string::do_serialize( void )
{
	std::wstring value = get_value();
	wchar_t* byte = new wchar_t[value.length()+1];
	memset(byte, 0, sizeof(wchar_t)*(value.length()+1));
	memcpy(byte, value.c_str(), sizeof(wchar_t)*value.length());
	return (u08*)byte;
}

void xui_propdata_string::un_serialize( u08* byte )
{
	std::wstring value = (wchar_t*)byte;
	if (m_userset)
	{
		(*m_userset)(m_userptr, value);
	}
	else
	{
		std::wstring* ptr = (std::wstring*)m_userptr;
		(*ptr) = value;
	}
}

//////////////////////////////////////////////////////////////////////////
//propdata_number
//////////////////////////////////////////////////////////////////////////
xui_propdata_number::xui_propdata_number(xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, u08 numbtype, f64 interval, f64 minvalue, f64 maxvalue ) 
: xui_propdata(kind, name, func)
{
	m_interval = interval;
	m_minvalue = minvalue;
	m_maxvalue = maxvalue;
	m_numbtype = numbtype;
}

u08 xui_propdata_number::get_numbtype( void ) const
{
	return m_numbtype;
}

f64 xui_propdata_number::get_interval( void ) const
{
	return m_interval;
}

f64 xui_propdata_number::get_minvalue( void ) const
{
	return m_minvalue;
}

f64 xui_propdata_number::get_maxvalue( void ) const
{
	return m_maxvalue;
}

xui_propdata_number_func::xui_propdata_number_func( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr, u08 numbtype, f64 interval, f64 minvalue, f64 maxvalue )
: xui_propdata_number(kind, name, func, numbtype, interval, minvalue, maxvalue)
{
	m_userget = userget;
	m_userset = userset;
	m_userptr = userptr;
	set_edit(m_userset != NULL);
}

f64 xui_propdata_number_func::get_value( void ) const
{
	return (*m_userget)(m_userptr);
}

void xui_propdata_number_func::set_value( f64 value )
{
	if (get_value() != value)
	{
		(*m_userset)(m_userptr, value);
		on_valuechanged();
	}
}

u08* xui_propdata_number_func::do_serialize( void )
{
	return get_byte<f64>(get_value());
}

void xui_propdata_number_func::un_serialize( u08* byte )
{
	(*m_userset)(m_userptr, get_cast<f64>(byte));
}

//////////////////////////////////////////////////////////////////////////
//propdata_enum
//////////////////////////////////////////////////////////////////////////
xui_propdata_enum::xui_propdata_enum(xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, const xui_propenum_map& textmap ) 
: xui_propdata(kind, name, func)
{
	m_textmap = textmap;
}

const xui_propenum_map& xui_propdata_enum::get_textmap( void ) const
{
	return m_textmap;
}

xui_propdata_enum_func::xui_propdata_enum_func( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, const xui_propenum_map& textmap, get_func userget, set_func userset, void* userptr )
: xui_propdata_enum(kind, name, func, textmap)
{
	m_userget = userget;
	m_userset = userset;
	m_userptr = userptr;
	set_edit(m_userset != NULL);
}

u32 xui_propdata_enum_func::get_value( void ) const
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

void xui_propdata_enum_func::set_value( u32 index )
{
	if (get_value() != index)
	{
		xui_propenum_map::iterator itor = m_textmap.begin();
		std::advance(itor, index);
		(*m_userset)(m_userptr, (*itor).first);
		on_valuechanged();
	}
}

u08* xui_propdata_enum_func::do_serialize( void )
{
	return get_byte<u32>(get_value());
}

void xui_propdata_enum_func::un_serialize( u08* byte )
{
	xui_propenum_map::iterator itor = m_textmap.begin();
	std::advance(itor, get_cast<u32>(byte));
	(*m_userset)(m_userptr, (*itor).first);
}

//////////////////////////////////////////////////////////////////////////
//propdata_expand
//////////////////////////////////////////////////////////////////////////
const xui_expandvary::xui_propdata_map xui_expandvary::empty_map;

xui_propdata_expand::xui_propdata_expand(xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, const xui_propdata_vec& subprop, bool subfold ) 
: xui_propdata(kind, name, func), xui_expandbase(subprop, subfold)
{}

void xui_propdata_expand::non_ctrl( void )
{
	xui_propdata::non_ctrl();
	for (u32 i = 0; i < m_subprop.size(); ++i)
		m_subprop[i]->non_ctrl();
}

xui_propdata* xui_propdata_expand::get_data( const std::wstring& name )
{
	for (u32 i = 0; i < m_subprop.size(); ++i)
	{
		if (m_subprop[i]->get_name() == name)
			return m_subprop[i];
	}

	return NULL;
}

std::wstring xui_propdata_expand::get_path( xui_propdata* prop )
{
	for (u32 i = 0; i < m_subprop.size(); ++i)
	{
		std::wstring temp = m_subprop[i]->get_path(prop);
		if (temp.length() > 0)
		{
			return L"/"+m_name+temp;
		}
	}

	return L"";
}

xui_propdata_expand_plus::xui_propdata_expand_plus( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, const xui_propdata_vec& subprop, bool subfold )
: xui_propdata_expand(kind, name, func, subprop, subfold)
{}

void xui_propdata_expand_plus::add_subprop( xui_propdata* propdata )
{
	m_subprop.push_back(propdata);
	refresh();
}

void xui_propdata_expand_plus::del_subprop( xui_propdata* propdata )
{
	xui_propdata_vec::iterator itor = std::find(m_subprop.begin(), m_subprop.end(), propdata);
	if (itor != m_subprop.end())
	{
		m_subprop.erase(itor);
		delete propdata;
		refresh();
	}
}

void xui_propdata_expand_plus::del_subpropall( void )
{
	for (u32 i = 0; i < m_subprop.size(); ++i)
		delete m_subprop[i];

	m_subprop.clear();
	refresh();
}

//////////////////////////////////////////////////////////////////////////
//propdata_stdvec
//////////////////////////////////////////////////////////////////////////
xui_propdata_stdvec::xui_propdata_stdvec( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, xui_prop_additem addfunc, xui_prop_delitem delfunc, xui_prop_newprop newfunc )
: xui_propdata(kind, name, func)
{
	m_additem = addfunc;
	m_delitem = delfunc;
	m_newprop = newfunc;
}

xui_propdata_stdvec::~xui_propdata_stdvec( void )
{
	for (u32 i = 0; i < m_propvec.size(); ++i)
		delete m_propvec[i];
}

const xui_propdata_vec& xui_propdata_stdvec::get_propvec( void ) const
{
	return m_propvec;
}

void xui_propdata_stdvec::non_ctrl( void )
{
	xui_propdata::non_ctrl();
	for (u32 i = 0; i < m_propvec.size(); ++i)
		m_propvec[i]->non_ctrl();
}

xui_propdata* xui_propdata_stdvec::get_data( const std::wstring& name )
{
	int pos = name.find(L"_");
	if (pos != -1)
	{
		std::wstring text = name.substr(pos+1);
		std::wstringstream temp(text.c_str());
		u32 index = -1;
		temp >> index;
		if (index < m_propvec.size())
			return m_propvec[index];
	}

	return NULL;
}

std::wstring xui_propdata_stdvec::get_path( xui_propdata* prop )
{
	for (u32 i = 0; i < m_propvec.size(); ++i)
	{
		std::wstring temp = m_propvec[i]->get_path(prop);
		if (temp.length() > 0)
		{
			return L"/"+m_name+temp;
		}
	}

	return L"";
}

xui_propdata_stdvec_root::xui_propdata_stdvec_root( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, add_root addfunc, del_root delfunc, get_func userget, void* userptr )
: xui_propdata(kind, name, func)
, m_addfunc(addfunc)
, m_delfunc(delfunc)
, m_userget(userget)
, m_userptr(userptr)
{}

xui_proproot_vec xui_propdata_stdvec_root::get_rootvec( void )
{
	return (*m_userget)(m_userptr);
}

void xui_propdata_stdvec_root::set_rootadd( void )
{
	if (m_addfunc)
	{
		(*m_addfunc)(m_userptr);
		refresh();
	}
}

void xui_propdata_stdvec_root::set_rootdel( xui_proproot* root )
{
	if (m_delfunc)
	{
		(*m_delfunc)(m_userptr, root);
		refresh();
	}
}

bool xui_propdata_stdvec_root::can_rootadd( void )
{
	return m_addfunc != NULL;
}

bool xui_propdata_stdvec_root::can_rootdel( void )
{
	return m_delfunc != NULL;
}

//////////////////////////////////////////////////////////////////////////
//propdata_vector
//////////////////////////////////////////////////////////////////////////
xui_propdata_vector::xui_propdata_vector(xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr, u08 numbtype, f64 interval , const xui_vector<f64>& defvalue  ) 
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

u08 xui_propdata_vector::get_numbtype( void ) const
{
	return m_numbtype;
}

f64 xui_propdata_vector::get_interval( void ) const
{
	return m_interval;
}

void xui_propdata_vector::set_defvalue( void )
{
	set_value(m_defvalue);
}

xui_vector<f64> xui_propdata_vector::get_value( void ) const
{
	return (*m_userget)(m_userptr);
}

void xui_propdata_vector::set_value( const xui_vector<f64>& value )
{
	if (get_value() != value)
	{
		(*m_userset)(m_userptr, value);
		on_valuechanged();
	}
}

u08* xui_propdata_vector::do_serialize( void )
{
	return get_byte< xui_vector<f64> >(get_value());
}

void xui_propdata_vector::un_serialize( u08* byte )
{
	(*m_userset)(m_userptr, get_cast< xui_vector<f64> >(byte));
}

//////////////////////////////////////////////////////////////////////////
//propdata_rect2d
//////////////////////////////////////////////////////////////////////////
xui_propdata_rect2d::xui_propdata_rect2d(xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr, u08 numbtype, f64 interval ) 
: xui_propdata(kind, name, func)
{
	m_userget	= userget;
	m_userset	= userset;
	m_userptr	= userptr;
	m_interval	= interval;
	m_numbtype	= numbtype;
	set_edit(m_userset != NULL);
}

u08 xui_propdata_rect2d::get_numbtype( void ) const
{
	return m_numbtype;
}

f64 xui_propdata_rect2d::get_interval( void ) const
{
	return m_interval;
}

xui_rect2d<f64> xui_propdata_rect2d::get_value( void ) const
{
	return (*m_userget)(m_userptr);
}

void xui_propdata_rect2d::set_value( const xui_rect2d<f64>& value )
{
	if (get_value() != value)
	{
		(*m_userset)(m_userptr, value);
		on_valuechanged();
	}
}

u08* xui_propdata_rect2d::do_serialize( void )
{
	return get_byte< xui_rect2d<f64> >(get_value());
}

void xui_propdata_rect2d::un_serialize( u08* byte )
{
	(*m_userset)(m_userptr, get_cast< xui_rect2d<f64> >(byte));
}

//////////////////////////////////////////////////////////////////////////
//propdata_colour
//////////////////////////////////////////////////////////////////////////
xui_propdata_colour::xui_propdata_colour( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr )
: xui_propdata(kind, name, func)
{
	m_userget = userget;
	m_userset = userset;
	m_userptr = userptr;
	set_edit(m_userset != NULL);
}

xui_colour xui_propdata_colour::get_value( void ) const
{
	return (*m_userget)(m_userptr);
}

void xui_propdata_colour::set_value( const xui_colour& value )
{
	if (get_value() != value)
	{
		(*m_userset)(m_userptr, value);
		on_valuechanged();
	}
}

u08* xui_propdata_colour::do_serialize( void )
{
	return get_byte<xui_colour>(get_value());
}

void xui_propdata_colour::un_serialize( u08* byte )
{
	(*m_userset)(m_userptr, get_cast<xui_colour>(byte));
}

//////////////////////////////////////////////////////////////////////////
//propdata_object
//////////////////////////////////////////////////////////////////////////
xui_propdata_object::xui_propdata_object( 
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

const xui_droptype_vec& xui_propdata_object::get_droptype( void ) const
{
	return m_droptype;
}

bool xui_propdata_object::has_droptype( const std::string& type ) const
{
	for (u32 i = 0; i < m_droptype.size(); ++i)
	{
		if (m_droptype[i] == type)
			return true;
	}

	return false;
}

void xui_propdata_object::add_droptype( const std::string& type )
{
	m_droptype.push_back(type);
}

xui_prop_newpick xui_propdata_object::get_pickfunc( void ) const
{
	return m_pickfunc;
}

xui_prop_geticon xui_propdata_object::get_iconfunc( void ) const
{
	return m_iconfunc;
}

xui_prop_getname xui_propdata_object::get_namefunc( void ) const
{
	return m_namefunc;
}

void xui_propdata_object::old_value( void )
{
	set_value(m_oldvalue);
}

void xui_propdata_object::syn_value( void )
{
	if (m_oldvalue != get_value())
	{
		m_oldvalue  = get_value();
		on_valuechanged();
	}
}

xui_propdata_object_func::xui_propdata_object_func( 
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

void* xui_propdata_object_func::get_value( void ) const
{
	return (*m_userget)(m_userptr);
}

void xui_propdata_object_func::set_value( void* value )
{
	void* tempvalue = get_value();
	if (tempvalue != value)
	{
		m_oldvalue = tempvalue;
		(*m_userset)(m_userptr, value);
	}
}

u08* xui_propdata_object_func::do_serialize( void )
{
	return get_byte<void*>(get_value());
}

void xui_propdata_object_func::un_serialize( u08* byte )
{
	void* value = get_cast<void*>(byte);
	(*m_userset)(m_userptr, value);
	m_oldvalue = value;
}