#ifndef __xui_propdata_h__
#define __xui_propdata_h__

#include "xui_component.h"

typedef xui_propctrl*	(*xui_prop_newctrl)( xui_propdata* propdata );
typedef void			(*xui_prop_additem)( void* vec );
typedef void			(*xui_prop_delitem)( void* vec );
typedef xui_propdata*	(*xui_prop_newprop)( void* ptr, xui_propkind* kind );

class   xui_pickwnd;
typedef xui_pickwnd*	(*xui_prop_newpick)( xui_propctrl* propctrl );
typedef xui_bitmap*		(*xui_prop_geticon)( xui_propdata* propdata );
typedef std::wstring	(*xui_prop_getname)( xui_propdata* propdata );

typedef std::map<s32, std::wstring> xui_propenum_map;
typedef std::vector<xui_propdata*>  xui_propdata_vec;

/*
//propdata
*/
class xui_propdata
{
public:
	/*
	//constructor
	*/
	xui_propdata( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func );

	/*
	//method
	*/
	xui_propkind*				get_kind		( void ) const;
	xui_prop_newctrl			get_func		( void ) const;
	const std::wstring&			get_name		( void ) const;
	void						set_name		( const std::wstring& name );
	xui_propctrl*				get_ctrl		( void );
	void						set_ctrl		( xui_propctrl* ctrl );
	bool						can_edit		( void ) const;
	void						set_edit		( bool flag );
	bool						can_show		( void ) const;
	void						set_show		( bool flag );

protected:
	/*
	//virtual
	*/
	virtual void				on_valuechanged	( void );

	/*
	//member
	*/
	xui_propkind*				m_kind;
	std::wstring				m_name;
	xui_prop_newctrl			m_func;
	xui_propctrl*				m_ctrl;
	bool						m_edit;
	bool						m_show;
};

/*
//bool
*/
class xui_propdata_bool : public xui_propdata
{
public:
	/*
	//constructor
	*/
	xui_propdata_bool( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, bool* ptr );

	/*
	//virtual
	*/
	virtual bool				get_value		( void ) const;
	virtual void				set_value		( bool value );

protected:
	/*
	//member
	*/
	bool*						m_ptr;
};

/*
//string
*/
class xui_propdata_string : public xui_propdata
{
public:
	/*
	//constructor
	*/
	xui_propdata_string( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, std::wstring* ptr );

	/*
	//virtual
	*/
	virtual const std::wstring&	get_value		( void ) const;
	virtual void				set_value		( const std::wstring& value );

protected:
	/*
	//member
	*/
	std::wstring*				m_ptr;
};

/*
//number
*/
class xui_propdata_number : public xui_propdata
{
public:
	/*
	//constructor
	*/
	xui_propdata_number( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func )
	: xui_propdata(kind, name, func)
	{}

	/*
	//virtual
	*/
	virtual f64					get_interval	( void ) const = 0;
	virtual f64					get_minvalue	( void ) const = 0;
	virtual f64					get_maxvalue	( void ) const = 0;
	virtual f64					get_value		( void ) const = 0;
	virtual void				set_value		( f64 value )  = 0;
};
template<typename T>
class xui_propdata_number_impl : public xui_propdata_number
{
public:
	/*
	//constructor
	*/
	xui_propdata_number_impl( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, T* ptr, T interval = T(1), T minvalue = T(0), T maxvalue = T(0) )
	: xui_propdata_number(kind, name, func)
	{
		m_ptr		= ptr;
		m_interval	= interval;
		m_minvalue	= minvalue;
		m_maxvalue	= maxvalue;
	}

	/*
	//override
	*/
	virtual f64					get_interval	( void ) const
	{
		return (f64)m_interval;
	}
	virtual f64					get_minvalue	( void ) const
	{
		return (f64)m_minvalue;
	}
	virtual f64					get_maxvalue	( void ) const
	{
		return (f64)m_maxvalue;
	}
	virtual f64					get_value		( void ) const
	{
		return (f64)(*m_ptr);
	}
	virtual void				set_value		( f64 value )
	{
		if ((*m_ptr) != (T)value)
		{
			(*m_ptr)  = (T)value;
			on_valuechanged();
		}
	}

protected:
	/*
	//member
	*/
	T*							m_ptr;
	T							m_interval;
	T							m_minvalue;
	T							m_maxvalue;
};

/*
//enum
*/
class xui_propdata_enum : public xui_propdata
{
public:
	/*
	//constructor
	*/
	xui_propdata_enum( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, const xui_propenum_map& textmap )
	: xui_propdata(kind, name, func)
	{
		m_textmap = textmap;
	}

	/*
	//method
	*/
	const xui_propenum_map&		get_textmap		( void ) const
	{
		return m_textmap;
	}

	/*
	//virtual
	*/
	virtual u32					get_value		( void ) const = 0;
	virtual void				set_value		( u32 index )  = 0;

protected:
	/*
	//member
	*/
	xui_propenum_map			m_textmap;
};
template<typename T>
class xui_propdata_enum_impl : public xui_propdata_enum
{
public:
	/*
	//constructor
	*/
	xui_propdata_enum_impl( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, const xui_propenum_map& textmap, T* ptr )
	: xui_propdata_enum(kind, name, func, textmap)
	{
		m_ptr   = ptr;
	}

	/*
	//virtual
	*/
	virtual u32					get_value		( void ) const
	{
		u32 index = 0;;
		for (xui_propenum_map::const_iterator itor = m_textmap.begin(); itor != m_textmap.end(); ++itor)
		{
			if ((*itor).first == (s32)(*m_ptr))
				break;

			++index;
		}

		return index;
	}
	virtual void				set_value		( u32 index )
	{
		if (get_value() != index)
		{
			xui_propenum_map::iterator itor = m_textmap.begin();
			std::advance(itor, index);
			(*m_ptr) = (T)(*itor).first;
			on_valuechanged();
		}
	}

protected:
	/*
	//member
	*/
	T*							m_ptr;
};

/*
//expand
*/
class xui_expandbase
{
public:
	/*
	//constructor
	*/
	xui_expandbase( const xui_propdata_vec& subprop, bool subfold = true )
	{
		m_subprop = subprop;
		m_subfold = subfold;
	}

	/*
	//destructor
	*/
    ~xui_expandbase( void )
	{
		for (u32 i = 0; i < m_subprop.size(); ++i)
			delete m_subprop[i];
	}

	/*
	//method
	*/
	bool						can_subfold		( void ) const
	{
		return m_subfold;
	}
	const xui_propdata_vec&		get_subprop		( void ) const
	{
		return m_subprop;
	}

protected:
	/*
	//member
	*/
	bool						m_subfold;
	xui_propdata_vec			m_subprop;
};
template<typename T>
class xui_expandvary : public xui_expandbase
{
public:
	/*
	//def
	*/
	typedef std::map< T, std::vector<u32> > xui_propdata_map;
	static const xui_propdata_map empty_map;

	/*
	//constructor
	*/
	xui_expandvary( const xui_propdata_vec& subprop, bool subfold = true, const xui_propdata_map& showmap = empty_map, const xui_propdata_map& editmap = empty_map )
	: xui_expandbase(subprop, subfold)
	{
		m_showmap = showmap;
		m_editmap = editmap;
	}

protected:
	/*
	//method
	*/
	void						set_subshow		( T value )
	{
		if (m_showmap.size() > 0)
		{
			for (u32 i = 0; i < m_subprop.size(); ++i)
				m_subprop[i]->set_show(false);

			xui_propdata_map::iterator itor = m_showmap.find(value);
			if (itor != m_showmap.end())
			{
				std::vector<u32>& vec = (*itor).second;
				for (u32 i = 0; i < vec.size(); ++i)
				{
					u32 index = vec[i];
					if (index < m_subprop.size())
						m_subprop[index]->set_show(true);
				}
			}
		}
	}
	void						set_subedit		( T value )
	{
		if (m_editmap.size() > 0)
		{
			for (u32 i = 0; i < m_subprop.size(); ++i)
				m_subprop[i]->set_edit(false);

			xui_propdata_map::iterator itor = m_editmap.find(value);
			if (itor != m_editmap.end())
			{
				std::vector<u32>& vec = (*itor).second;
				for (u32 i = 0; i < vec.size(); ++i)
				{
					u32 index = vec[i];
					if (index < m_subprop.size())
						m_subprop[index]->set_edit(true);
				}
			}
		}
	}

	/*
	//member
	*/
	xui_propdata_map			m_showmap;
	xui_propdata_map			m_editmap;
};

class xui_propdata_expand : public xui_propdata, public xui_expandbase
{
public:
	/*
	//constructor
	*/
	xui_propdata_expand( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, const xui_propdata_vec& subprop, bool subfold = true )
	: xui_propdata(kind, name, func), xui_expandbase(subprop, subfold)
	{}
};
class xui_propdata_expand_bool : public xui_propdata_bool, public xui_expandvary<bool>
{
public:
	/*
	//constructor
	*/
	xui_propdata_expand_bool( 
		xui_propkind*								kind, 
		const std::wstring&							name, 
		xui_prop_newctrl							func, 
		bool*										ptr, 
		const xui_propdata_vec&						subprop, 
		bool										subfold = true, 
		const std::map<bool, std::vector<u32> >&	showmap = xui_expandvary<bool>::empty_map,
		const std::map<bool, std::vector<u32> >&	editmap = xui_expandvary<bool>::empty_map )
	: xui_propdata_bool(kind, name, func, ptr), xui_expandvary<bool>(subprop, subfold, showmap, editmap)
	{
		set_subshow(*m_ptr);
		set_subedit(*m_ptr);
	}

	/*
	//override
	*/
	virtual void				on_valuechanged	( void )
	{
		set_subshow(*m_ptr);
		set_subedit(*m_ptr);
		xui_propdata_bool::on_valuechanged();
	}
};
template<typename T>
class xui_propdata_expand_enum : public xui_propdata_enum_impl<T>, public xui_expandvary<T>
{
public:
	/*
	//constructor
	*/
	xui_propdata_expand_enum( 
		xui_propkind*								kind, 
		const std::wstring&							name, 
		xui_prop_newctrl							func, 
		const xui_propenum_map&						textmap,
		T*											ptr, 
		const xui_propdata_vec&						subprop, 
		bool										subfold = true, 
		const std::map<T, std::vector<u32> >&		showmap = xui_expandvary<T>::empty_map,
		const std::map<T, std::vector<u32> >&		editmap = xui_expandvary<T>::empty_map )
	: xui_propdata_enum_impl<T>(kind, name, func, textmap, ptr), xui_expandvary<T>(subprop, subfold, showmap, editmap)
	{
		set_subshow(*m_ptr);
		set_subedit(*m_ptr);
	}

	/*
	//method
	*/
	virtual void				on_valuechanged	( void )
	{
		set_subshow(*m_ptr);
		set_subedit(*m_ptr);
		xui_propdata_enum_impl<T>::on_valuechanged();
	}
};

/*
//stdvec
*/
class xui_propdata_stdvec: public xui_propdata
{
public:
	/*
	//constructor
	*/
	xui_propdata_stdvec( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, xui_prop_additem addfunc, xui_prop_delitem delfunc, xui_prop_newprop newfunc );

	/*
	//destructor
	*/
	virtual ~xui_propdata_stdvec( void );

	/*
	//method
	*/
	const xui_propdata_vec&		get_propvec		( void ) const
	{
		return m_propvec;
	}

	/*
	//virtual
	*/
	virtual u32					get_value		( void ) const					= 0;
	virtual void				set_value		( u32 value )					= 0;
	virtual void				set_index		( u32 oldindex, u32 newindex )	= 0;

protected:
	/*
	//member
	*/
	xui_prop_additem			m_additem;
	xui_prop_delitem			m_delitem;
	xui_prop_newprop			m_newprop;
	xui_propdata_vec			m_propvec;
};
template<typename T>
class xui_propdata_stdvec_impl : public xui_propdata_stdvec
{
public:
	/*
	//constructor
	*/
	xui_propdata_stdvec_impl( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, xui_prop_additem addfunc, xui_prop_delitem delfunc, xui_prop_newprop newfunc, std::vector<T>* ptr )
	: xui_propdata_stdvec(kind, name, func, addfunc, delfunc, newfunc)
	{
		m_ptr = ptr;
		refresh();
	}

	/*
	//refresh
	*/
	void						refresh			( void )
	{
		for (u32 i = 0; i < m_propvec.size(); ++i)
			delete m_propvec[i];

		m_propvec.clear();
		for (u32 i = 0; i < (*m_ptr).size(); ++i)
		{
			std::wstringstream name;
			name << "Element_";
			name << i;
			xui_propdata* propdata = (*m_newprop)((void*)(&(*m_ptr)[i]), m_kind);
			propdata->set_name(name.str());

			m_propvec.push_back(propdata);
		}

		if (m_ctrl)
		{
			m_ctrl->on_linkpropdata();
			m_ctrl->refresh();
		}
	}

	/*
	//override
	*/
	virtual u32					get_value		( void ) const
	{
		return (*m_ptr).size();
	}
	virtual void				set_value		( u32 value )
	{
		if ((*m_ptr).size() < value)
		{
			u32 count = value - (*m_ptr).size();
			for (u32 i = 0; i < count; ++i)
			{
				if (m_additem)
				{
					(*m_additem)((void*)m_ptr);
				}
				else
				{
					(*m_ptr).push_back(T(0));
				}
			}

			refresh();
		}
		else
		if ((*m_ptr).size() > value)
		{
			u32 count = (*m_ptr).size() - value;
			for (u32 i = 0; i < count; ++i)
			{
				if (m_delitem)
				{
					(*m_delitem)((void*)m_ptr);
				}
				else
				{
					u32 index = (*m_ptr).size()-1;
					(*m_ptr).erase((*m_ptr).begin()+index);
				}
			}

			refresh();
		}
	}
	virtual void				set_index		( u32 oldindex, u32 newindex )
	{
		if (oldindex >= (*m_ptr).size() ||
			newindex >  (*m_ptr).size())
			return;

		T value = (*m_ptr)[oldindex];
		(*m_ptr).insert((*m_ptr).begin()+newindex, value);

		if (newindex < oldindex)
			++oldindex;
		(*m_ptr).erase((*m_ptr).begin()+oldindex);

		refresh();
	}

protected:
	/*
	//member
	*/
	std::vector<T>*				m_ptr;
};
template<typename T>
class xui_propdata_stdlst_impl : public xui_propdata_stdvec
{
public:
	/*
	//constructor
	*/
	xui_propdata_stdlst_impl( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, xui_prop_additem addfunc, xui_prop_delitem delfunc, xui_prop_newprop newfunc, std::list<T>* ptr )
	: xui_propdata_stdvec(kind, name, func, addfunc, delfunc, newfunc)
	{
		m_ptr = ptr;
		refresh();
	}

	/*
	//refresh
	*/
	void						refresh			( void )
	{
		for (u32 i = 0; i < m_propvec.size(); ++i)
			delete m_propvec[i];

		m_propvec.clear();
		u32 i = 0;
		for (std::list<T>::iterator itor = (*m_ptr).begin(); itor != (*m_ptr).end(); ++itor, ++i)
		{
			std::wstringstream name;
			name << "Element_";
			name << i;
			xui_propdata* propdata = (*m_newprop)((void*)(&(*itor)), m_kind);
			propdata->set_name(name.str());

			m_propvec.push_back(propdata);
		}

		if (m_ctrl)
		{
			m_ctrl->on_linkpropdata();
			m_ctrl->refresh();
		}
	}

	/*
	//override
	*/
	virtual u32					get_value		( void ) const
	{
		return (*m_ptr).size();
	}
	virtual void				set_value		( u32 value )
	{
		if ((*m_ptr).size() < value)
		{
			u32 count = value - (*m_ptr).size();
			for (u32 i = 0; i < count; ++i)
			{
				if (m_additem)
				{
					(*m_additem)((void*)m_ptr);
				}
				else
				{
					(*m_ptr).push_back(T(0));
				}
			}

			refresh();
		}
		else
		if ((*m_ptr).size() > value)
		{
			u32 count = (*m_ptr).size() - value;
			for (u32 i = 0; i < count; ++i)
			{
				if (m_delitem)
				{
					(*m_delitem)((void*)m_ptr);
				}
				else
				{
					(*m_ptr).pop_back();
				}
			}

			refresh();
		}
	}
	virtual void				set_index		( u32 oldindex, u32 newindex )
	{
		if (oldindex >= (*m_ptr).size() ||
			newindex >  (*m_ptr).size())
			return;

		std::list<T>::iterator itor;
		itor = (*m_ptr).begin();
		std::advance(itor, oldindex);
		T value = (*itor);

		itor = (*m_ptr).begin();
		std::advance(itor, newindex);
		(*m_ptr).insert(itor, value);

		if (newindex < oldindex)
			++oldindex;
		itor = (*m_ptr).begin();
		std::advance(itor, oldindex);
		(*m_ptr).erase(itor);

		refresh();
	}

protected:
	/*
	//member
	*/
	std::list<T>*				m_ptr;
};

/*
//vector
*/
class xui_propdata_vector : public xui_propdata
{
public:
	/*
	//constructor
	*/
	xui_propdata_vector( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func )
	: xui_propdata(kind, name, func)
	{}

	/*
	//virtual
	*/
	virtual f64					get_interval( void ) const = 0;
	virtual void 				set_defvalue( void ) = 0;
	virtual xui_vector<f64>		get_value	( void ) const = 0;
	virtual void				set_value	( const xui_vector<f64>& value ) = 0;
};
template<typename T>
class xui_propdata_vector_impl : public xui_propdata_vector
{
public:
	/*
	//constructor
	*/
	xui_propdata_vector_impl( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, xui_vector<T>* ptr, T interval = T(1), const xui_vector<T>& defvalue = xui_vector<T>(0) )
	: xui_propdata_vector(kind, name, func)
	{
		m_ptr		= ptr;
		m_defvalue	= defvalue;
		m_interval	= interval;
	}

	/*
	//virtual
	*/
	virtual f64					get_interval( void ) const
	{
		return (f64)m_interval;
	}
	virtual void				set_defvalue( void )
	{
		if ((*m_ptr) != m_defvalue)
		{
			(*m_ptr)  = m_defvalue;
			on_valuechanged();
		}
	}
	virtual xui_vector<f64>		get_value	( void ) const
	{
		xui_vector<f64> result;
		result.x = (f64)(*m_ptr).x;
		result.y = (f64)(*m_ptr).y;
		return result;
	}
	virtual void				set_value	( const xui_vector<f64>& value )
	{
		xui_vector<T> temp;
		temp.x = (T)value.x;
		temp.y = (T)value.y;
		if ((*m_ptr) != temp)
		{
			(*m_ptr)  = temp;
			on_valuechanged();
		}
	}

protected:
	/*
	//member
	*/
	xui_vector<T>*				m_ptr;
	xui_vector<T>				m_defvalue;
	T							m_interval;
};

/*
//rect2d
*/
class xui_propdata_rect2d : public xui_propdata
{
public:
	/*
	//constructor
	*/
	xui_propdata_rect2d( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func )
	: xui_propdata(kind, name, func)
	{}

	/*
	//method
	*/
	virtual f64					get_interval( void ) const = 0;
	virtual xui_rect2d<f64>		get_value	( void ) const = 0;
	virtual void				set_value	( const xui_rect2d<f64>& value ) = 0;
};
template<typename T>
class xui_propdata_rect2d_impl : public xui_propdata_rect2d
{
public:
	/*
	//constructor
	*/
	xui_propdata_rect2d_impl( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, xui_rect2d<T>* ptr, T interval = T(1) )
	: xui_propdata_rect2d(kind, name, func)
	{
		m_ptr		= ptr;
		m_interval	= interval;
	}

	/*
	//virtual
	*/
	virtual f64					get_interval( void ) const
	{
		return (f64)m_interval;
	}
	virtual xui_rect2d<f64>		get_value	( void ) const
	{
		xui_rect2d<f64> result;
		result.ax = (f64)(*m_ptr).ax;
		result.ay = (f64)(*m_ptr).ay;
		result.bx = (f64)(*m_ptr).bx;
		result.by = (f64)(*m_ptr).by;
		return result;
	}
	virtual void				set_value	( const xui_rect2d<f64>& value )
	{
		xui_rect2d<T> temp;
		temp.ax = (T)value.ax;
		temp.ay = (T)value.ay;
		temp.bx = (T)value.bx;
		temp.by = (T)value.by;
		if ((*m_ptr) != temp)
		{
			(*m_ptr)  = temp;
			on_valuechanged();
		}
	}

protected:
	/*
	//member
	*/
	xui_rect2d<T>*				m_ptr;
	T							m_interval;
};

/*
//object
*/
class xui_propdata_object : public xui_propdata
{
public:
	/*
	//constructor
	*/
	xui_propdata_object( 
		xui_propkind*		kind, 
		const std::wstring& name, 
		xui_prop_newctrl	func, 
		const std::string&	droptype, 
		xui_prop_newpick	pickfunc,
		xui_prop_geticon	iconfunc, 
		xui_prop_getname	namefunc );

	/*
	//method
	*/
	const std::string&			get_droptype( void ) const;
	xui_prop_newpick			get_pickfunc( void ) const;
	xui_prop_geticon			get_iconfunc( void ) const;
	xui_prop_getname			get_namefunc( void ) const;

	/*
	//virtual
	*/
	virtual void*				get_value	( void ) const  = 0;
	virtual void				set_value	( void* value ) = 0;
	virtual void				old_value	( void )		= 0;
	virtual void				syn_value	( void )		= 0;

	/*
	//method
	*/
	xui_method<xui_method_args>	xm_click;

protected:
	/*
	//member
	*/
	std::string					m_droptype;
	xui_prop_newpick			m_pickfunc;
	xui_prop_geticon			m_iconfunc;
	xui_prop_getname			m_namefunc;
};
template<typename T>
class xui_propdata_object_impl : public xui_propdata_object
{
public:
	/*
	//constructor
	*/
	xui_propdata_object_impl( 
		xui_propkind*		kind, 
		const std::wstring& name, 
		xui_prop_newctrl	func, 
		const std::string&	droptype, 
		xui_prop_newpick	pickfunc, 
		xui_prop_geticon	iconfunc,
		xui_prop_getname	namefunc,
		T**					ptr )
	: xui_propdata_object(kind, name, func, droptype, pickfunc, iconfunc, namefunc)
	{
		m_ptr = ptr;
		m_old = (*m_ptr);
	}

	/*
	//virtual
	*/
	virtual void*				get_value	( void ) const
	{
		return (void*)(*m_ptr);
	}
	virtual void				set_value	( void* value )
	{
		if ((*m_ptr) != (T*)value)
		{
			  m_old   = (*m_ptr);
			(*m_ptr)  = (T*)value;
			on_valuechanged();
		}
	}
	virtual void				old_value	( void )
	{
		if ((*m_ptr) != m_old)
		{
			(*m_ptr)  = m_old;
			on_valuechanged();
		}
	}
	virtual void				syn_value	( void )
	{
		m_old = (*m_ptr);
	}

protected:
	/*
	//member
	*/
	T*							m_old;
	T**							m_ptr;
};

#endif//__xui_propdata_h__