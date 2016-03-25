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
	bool						can_edit		( void ) const;
	void						set_edit		( bool flag );
	bool						can_show		( void ) const;
	void						set_show		( bool flag );
	xui_propctrl*				get_ctrl		( void );
	void						set_ctrl		( xui_propctrl* ctrl );
	virtual void				non_ctrl		( void );

	/*
	//refresh
	*/
	void						refresh			( void );

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
	typedef bool				(*get_func)( void* userptr );
	typedef void				(*set_func)( void* userptr, bool value );

	/*
	//constructor
	*/
	xui_propdata_bool( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr );

	/*
	//virtual
	*/
	virtual bool				get_value		( void ) const;
	virtual void				set_value		( bool value );

protected:
	/*
	//member
	*/
	get_func					m_userget;
	set_func					m_userset;
	void*						m_userptr;
};

/*
//string
*/
class xui_propdata_string : public xui_propdata
{
public:
	typedef std::wstring		(*get_func)( void* userptr );
	typedef void				(*set_func)( void* userptr, const std::wstring& value );

	/*
	//constructor
	*/
	xui_propdata_string( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr );

	/*
	//virtual
	*/
	virtual std::wstring		get_value		( void ) const;
	virtual void				set_value		( const std::wstring& value );

protected:
	/*
	//member
	*/
	get_func					m_userget;
	set_func					m_userset;
	void*						m_userptr;
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
	xui_propdata_number( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, u08 numbtype, f64 interval, f64 minvalue, f64 maxvalue );

	/*
	//virtual
	*/
	u08							get_numbtype	( void ) const;
	f64							get_interval	( void ) const;
	f64							get_minvalue	( void ) const;
	f64							get_maxvalue	( void ) const;

	virtual f64					get_value		( void ) const = 0;
	virtual void				set_value		( f64 value )  = 0;

protected:
	/*
	//member
	*/
	u08							m_numbtype;
	f64							m_interval;
	f64							m_minvalue;
	f64							m_maxvalue;
};
class xui_propdata_number_func : public xui_propdata_number
{
public:
	typedef f64					(*get_func)( void* userptr );
	typedef void				(*set_func)( void* userptr, f64 value );

	/*
	//constructor
	*/
	xui_propdata_number_func( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr, u08 numbtype, f64 interval = 1, f64 minvalue = 0, f64 maxvalue = 0 );

	/*
	//override
	*/
	virtual f64					get_value		( void ) const;
	virtual void				set_value		( f64 value );

protected:
	/*
	//member
	*/
	get_func					m_userget;
	set_func					m_userset;
	void*						m_userptr;
};
template<typename T>
class xui_propdata_number_impl : public xui_propdata_number
{
public:
	/*
	//constructor
	*/
	xui_propdata_number_impl( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, T* ptr, u08 numbtype, f64 interval = 1, f64 minvalue = 0, f64 maxvalue = 0 )
	: xui_propdata_number(kind, name, func, numbtype, interval, minvalue, maxvalue)
	{
		m_ptr = ptr;
	}

	/*
	//override
	*/
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
	xui_propdata_enum( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, const xui_propenum_map& textmap );

	/*
	//method
	*/
	const xui_propenum_map&		get_textmap		( void ) const;

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
class xui_propdata_enum_func : public xui_propdata_enum
{
public:
	typedef s32					(*get_func)( void* userptr );
	typedef void				(*set_func)( void* userptr, s32 value );

	/*
	//constructor
	*/
	xui_propdata_enum_func( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, const xui_propenum_map& textmap, get_func userget, set_func userset, void* userptr );

	/*
	//override
	*/
	virtual u32					get_value		( void ) const;
	virtual void				set_value		( u32 index );

protected:
	/*
	//member
	*/
	get_func					m_userget;
	set_func					m_userset;
	void*						m_userptr;
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
		m_ptr = ptr;
	}

	/*
	//virtual
	*/
	virtual u32					get_value		( void ) const
	{
		u32 index = 0;
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
class xui_expandvary : public xui_expandbase
{
public:
	/*
	//def
	*/
	typedef std::map<s32, std::vector<u32> > xui_propdata_map;
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
	void						set_subshow		( s32 value )
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
	void						set_subedit		( s32 value )
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
	xui_propdata_expand( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, const xui_propdata_vec& subprop, bool subfold = true );

	/*
	//override
	*/
	virtual void				non_ctrl		( void );
};
class xui_propdata_expand_plus : public xui_propdata_expand
{
public:
	/*
	//constructor
	*/
	xui_propdata_expand_plus( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, const xui_propdata_vec& subprop, bool subfold = true );

	/*
	//method
	*/
	void						add_subprop		( xui_propdata* propdata );
	void						del_subprop		( xui_propdata* propdata );
	void						del_subpropall	( void );
};

class xui_propdata_expand_bool : public xui_propdata_bool, public xui_expandvary
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
		get_func									userget,
		set_func									userset,
		void*										userptr,
		bool										subfold = true, 
		const std::map<s32, std::vector<u32> >&		showmap = xui_expandvary::empty_map,
		const std::map<s32, std::vector<u32> >&		editmap = xui_expandvary::empty_map )
	: xui_propdata_bool(kind, name, func, userget, userset, userptr), xui_expandvary(subprop, subfold, showmap, editmap)
	{
		bool value = get_value();
		set_subshow(value ? 1 : 0);
		set_subedit(value ? 1 : 0);
	}

	/*
	//override
	*/
	virtual void				non_ctrl		( void )
	{
		xui_propdata_bool::non_ctrl();
		for (u32 i = 0; i < m_subprop.size(); ++i)
			m_subprop[i]->non_ctrl();
	}

protected:
	/*
	//override
	*/
	virtual void				on_valuechanged	( void )
	{
		bool value = get_value();
		set_subshow(value ? 1 : 0);
		set_subedit(value ? 1 : 0);
		xui_propdata_bool::on_valuechanged();
	}
};
class xui_propdata_expand_enum_func : public xui_propdata_enum_func, public xui_expandvary
{
public:
	/*
	//constructor
	*/
	xui_propdata_expand_enum_func( 
		xui_propkind*								kind, 
		const std::wstring&							name, 
		xui_prop_newctrl							func, 
		const xui_propenum_map&						textmap,
		get_func									userget,
		set_func									userset,
		void*										userptr, 
		const xui_propdata_vec&						subprop, 
		bool										subfold = true, 
		const std::map<s32, std::vector<u32> >&		showmap = xui_expandvary::empty_map,
		const std::map<s32, std::vector<u32> >&		editmap = xui_expandvary::empty_map )
	: xui_propdata_enum_func(kind, name, func, textmap, userget, userset, userptr), xui_expandvary(subprop, subfold, showmap, editmap)
	{
		s32 value = (*m_userget)(m_userptr);
		set_subshow(value);
		set_subedit(value);
	}

	/*
	//override
	*/
	virtual void				non_ctrl		( void )
	{
		xui_propdata_enum_func::non_ctrl();
		for (u32 i = 0; i < m_subprop.size(); ++i)
			m_subprop[i]->non_ctrl();
	}

protected:
	/*
	//override
	*/
	virtual void				on_valuechanged	( void )
	{
		s32 value = (*m_userget)(m_userptr);
		set_subshow(value);
		set_subedit(value);
		xui_propdata_enum_func::on_valuechanged();
	}
};
template<typename T>
class xui_propdata_expand_enum_impl : public xui_propdata_enum_impl<T>, public xui_expandvary
{
public:
	/*
	//constructor
	*/
	xui_propdata_expand_enum_impl( 
		xui_propkind*								kind, 
		const std::wstring&							name, 
		xui_prop_newctrl							func, 
		const xui_propenum_map&						textmap,
		T*											ptr, 
		const xui_propdata_vec&						subprop, 
		bool										subfold = true, 
		const std::map<s32, std::vector<u32> >&		showmap = xui_expandvary::empty_map,
		const std::map<s32, std::vector<u32> >&		editmap = xui_expandvary::empty_map )
	: xui_propdata_enum_impl<T>(kind, name, func, textmap, ptr), xui_expandvary(subprop, subfold, showmap, editmap)
	{
		s32 value = (s32)(*m_ptr);
		set_subshow(value);
		set_subedit(value);
	}

	/*
	//override
	*/
	virtual void				non_ctrl		( void )
	{
		xui_propdata_enum_impl<T>::non_ctrl();
		for (u32 i = 0; i < m_subprop.size(); ++i)
			m_subprop[i]->non_ctrl();
	}

protected:
	/*
	//override
	*/
	virtual void				on_valuechanged	( void )
	{
		s32 value = (s32)(*m_ptr);
		set_subshow(value);
		set_subedit(value);
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
	const xui_propdata_vec&		get_propvec		( void ) const;

	/*
	//virtual
	*/
	virtual u32					get_value		( void ) const					= 0;
	virtual void				set_value		( u32 value )					= 0;
	virtual void				set_index		( u32 oldindex, u32 newindex )	= 0;

	/*
	//override
	*/
	virtual void				non_ctrl		( void );

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
class xui_propdata_stdvec_func : public xui_propdata_stdvec
{
public:
	typedef std::vector<T>&		(*get_func)( void* userptr );

	/*
	//constructor
	*/
	xui_propdata_stdvec_func( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, xui_prop_additem addfunc, xui_prop_delitem delfunc, xui_prop_newprop newfunc, get_func userget, void* userptr )
	: xui_propdata_stdvec(kind, name, func, addfunc, delfunc, newfunc)
	{
		m_userget = userget;
		m_userptr = userptr;
		realloc();
	}

	/*
	//refresh
	*/
	void						realloc			( void )
	{
		for (u32 i = 0; i < m_propvec.size(); ++i)
			delete m_propvec[i];

		m_propvec.clear();
		std::vector<T>& vec = (*m_userget)(m_userptr);
		for (u32 i = 0; i < vec.size(); ++i)
		{
			std::wstringstream name;
			name << "Element_";
			name << i;
			xui_propdata* propdata = (*m_newprop)((void*)(&vec[i]), m_kind);
			propdata->set_name(name.str());

			m_propvec.push_back(propdata);
		}

		refresh();
	}

	/*
	//override
	*/
	virtual u32					get_value		( void ) const
	{
		std::vector<T>& vec = (*m_userget)(m_userptr);
		return vec.size();
	}
	virtual void				set_value		( u32 value )
	{
		std::vector<T>& vec = (*m_userget)(m_userptr);
		if (vec.size() < value)
		{
			u32 count = value - vec.size();
			for (u32 i = 0; i < count; ++i)
			{
				if (m_additem)
				{
					(*m_additem)((void*)(&vec));
				}
				else
				{
					vec.push_back(T(0));
				}
			}

			realloc();
		}
		else
		if (vec.size() > value)
		{
			u32 count = vec.size() - value;
			for (u32 i = 0; i < count; ++i)
			{
				if (m_delitem)
				{
					(*m_delitem)((void*)(&vec));
				}
				else
				{
					u32 index = vec.size()-1;
					vec.erase(vec.begin()+index);
				}
			}

			realloc();
		}
	}
	virtual void				set_index		( u32 oldindex, u32 newindex )
	{
		std::vector<T>& vec = (*m_userget)(m_userptr);
		if (oldindex >= vec.size() ||
			newindex >  vec.size())
			return;

		T value = vec[oldindex];
		vec.insert(vec.begin()+newindex, value);

		if (newindex < oldindex)
			++oldindex;
		vec.erase (vec.begin()+oldindex);

		realloc();
	}

protected:
	/*
	//member
	*/
	get_func					m_userget;
	void*						m_userptr;
};
template<typename T>
class xui_propdata_stdlst_func : public xui_propdata_stdvec
{
public:
	typedef std::list<T>&		(*get_func)( void* userptr );

	/*
	//constructor
	*/
	xui_propdata_stdlst_func( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, xui_prop_additem addfunc, xui_prop_delitem delfunc, xui_prop_newprop newfunc, get_func userget, void* userptr )
	: xui_propdata_stdvec(kind, name, func, addfunc, delfunc, newfunc)
	{
		m_userget = userget;
		m_userptr = userptr;
		realloc();
	}

	/*
	//realloc
	*/
	void						realloc			( void )
	{
		for (u32 i = 0; i < m_propvec.size(); ++i)
			delete m_propvec[i];

		m_propvec.clear();
		std::list<T>& lst = (*m_userget)(m_userptr);
		u32 i = 0;
		for (std::list<T>::iterator itor = vec.begin(); itor != vec.end(); ++itor, ++i)
		{
			std::wstringstream name;
			name << "Element_";
			name << i;
			xui_propdata* propdata = (*m_newprop)((void*)itor, m_kind);
			propdata->set_name(name.str());

			m_propvec.push_back(propdata);
		}

		refresh();
	}

	/*
	//override
	*/
	virtual u32					get_value		( void ) const
	{
		std::list<T>& lst = (*m_userget)(m_userptr);
		return lst.size();
	}
	virtual void				set_value		( u32 value )
	{
		std::list<T>& lst = (*m_userget)(m_userptr);
		if (lst.size() < value)
		{
			u32 count = value - lst.size();
			for (u32 i = 0; i < count; ++i)
			{
				if (m_additem)
				{
					(*m_additem)((void*)(&lst));
				}
				else
				{
					lst.push_back(T(0));
				}
			}

			realloc();
		}
		else
		if (lst.size() > value)
		{
			u32 count = lst.size() - value;
			for (u32 i = 0; i < count; ++i)
			{
				if (m_delitem)
				{
					(*m_delitem)((void*)(&lst));
				}
				else
				{
					lst.pop_back();
				}
			}

			realloc();
		}
	}
	virtual void				set_index		( u32 oldindex, u32 newindex )
	{
		std::list<T>& lst = (*m_userget)(m_userptr)
		if (oldindex >= lst.size() ||
			newindex >  lst.size())
			return;

		std::list<T>::iterator itor;
		itor = lst.begin();
		std::advance(itor, oldindex);
		T value = (*itor);

		itor = lst.begin();
		std::advance(itor, newindex);
		lst.insert(itor, value);

		if (newindex < oldindex)
			++oldindex;
		itor = lst.begin();
		std::advance(itor, oldindex);
		lst.erase (itor);

		realloc();
	}

protected:
	/*
	//member
	*/
	get_func					m_userget;
	void*						m_userptr;
};

/*
//vector
*/
class xui_propdata_vector : public xui_propdata
{
public:
	typedef xui_vector<f64>		(*get_func)( void* userptr );
	typedef void				(*set_func)( void* userptr, const xui_vector<f64>& value );

	/*
	//constructor
	*/
	xui_propdata_vector( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr, u08 numbtype, f64 interval = 1, const xui_vector<f64>& defvalue = xui_vector<f64>(0) );

	/*
	//method
	*/
	u08							get_numbtype( void ) const;
	virtual f64					get_interval( void ) const;
	virtual void 				set_defvalue( void );
	virtual xui_vector<f64>		get_value	( void ) const;
	virtual void				set_value	( const xui_vector<f64>& value );

protected:
	/*
	//member
	*/
	get_func					m_userget;
	set_func					m_userset;
	void*						m_userptr;
	xui_vector<f64>				m_defvalue;
	f64							m_interval;
	u08							m_numbtype;
};

/*
//rect2d
*/
class xui_propdata_rect2d : public xui_propdata
{
public:
	typedef xui_rect2d<f64>		(*get_func)( void* userptr );
	typedef void				(*set_func)( void* userptr, const xui_rect2d<f64>& value );

	/*
	//constructor
	*/
	xui_propdata_rect2d( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr, u08 numbtype, f64 interval = 1 );

	/*
	//method
	*/
	u08							get_numbtype( void ) const;
	virtual f64					get_interval( void ) const;
	virtual xui_rect2d<f64>		get_value	( void ) const;
	virtual void				set_value	( const xui_rect2d<f64>& value );

protected:
	/*
	//member
	*/
	get_func					m_userget;
	set_func					m_userset;
	void*						m_userptr;
	f64							m_interval;
	u08							m_numbtype;
};

/*
//colour
*/
class xui_propdata_colour : public xui_propdata
{
public:
	typedef xui_colour			(*get_func)( void* userptr );
	typedef void				(*set_func)( void* userptr, const xui_colour& value );

	/*
	//constructor
	*/
	xui_propdata_colour( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr );

	/*
	//virtual
	*/
	virtual xui_colour			get_value		( void ) const;
	virtual void				set_value		( const xui_colour& value );

protected:
	/*
	//member
	*/
	get_func					m_userget;
	set_func					m_userset;
	void*						m_userptr;
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
		xui_propkind*			kind, 
		const std::wstring&		name, 
		xui_prop_newctrl		func, 
		const std::string&		droptype, 
		xui_prop_newpick		pickfunc,
		xui_prop_geticon		iconfunc, 
		xui_prop_getname		namefunc );

	/*
	//method
	*/
	bool						has_droptype( const std::string& type ) const;
	void						add_droptype( const std::string& type );
	xui_prop_newpick			get_pickfunc( void ) const;
	xui_prop_geticon			get_iconfunc( void ) const;
	xui_prop_getname			get_namefunc( void ) const;

	/*
	//virtual
	*/
	virtual void*				get_value	( void ) const  = 0;
	virtual void				set_value	( void* value ) = 0;
	virtual void				old_value	( void );
	virtual void				syn_value	( void );

	/*
	//method
	*/
	xui_method<xui_method_args>	xm_click;

protected:
	/*
	//member
	*/
	std::vector<std::string>	m_droptype;
	xui_prop_newpick			m_pickfunc;
	xui_prop_geticon			m_iconfunc;
	xui_prop_getname			m_namefunc;
	void*						m_oldvalue;
};
class xui_propdata_object_func : public xui_propdata_object
{
public:
	typedef void*	(*get_func)( void* userptr );
	typedef void	(*set_func)( void* userptr, void* value );

	/*
	//constructor
	*/
	xui_propdata_object_func(
		xui_propkind*			kind,
		const std::wstring&		name, 
		xui_prop_newctrl		func,
		const std::string&		droptype,
		xui_prop_newpick		pickfunc,
		xui_prop_geticon		iconfunc,
		xui_prop_getname		namefunc,
		get_func				userget,
		set_func				userset,
		void*					userptr );

	/*
	//override
	*/
	virtual void*				get_value	( void ) const;
	virtual void				set_value	( void* value );

protected:
	/*
	//member
	*/
	get_func					m_userget;
	set_func					m_userset;
	void*						m_userptr;
};
template<typename T>
class xui_propdata_object_impl : public xui_propdata_object
{
public:
	/*
	//constructor
	*/
	xui_propdata_object_impl( 
		xui_propkind*			kind, 
		const std::wstring&		name, 
		xui_prop_newctrl		func, 
		const std::string&		droptype, 
		xui_prop_newpick		pickfunc, 
		xui_prop_geticon		iconfunc,
		xui_prop_getname		namefunc,
		T*						ptr )
	: xui_propdata_object(kind, name, func, droptype, pickfunc, iconfunc, namefunc)
	{
		m_ptr = ptr;
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
		if (get_value() != value)
		{
			m_oldvalue = (void*)(*m_ptr);
			(*m_ptr) = (T)value;
			on_valuechanged();
		}
	}

protected:
	/*
	//member
	*/
	T*							m_ptr;
};

#endif//__xui_propdata_h__