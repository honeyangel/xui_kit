#ifndef __xui_propdata_h__
#define __xui_propdata_h__

#include "xui_component.h"

typedef xui_propctrl*		(*xui_prop_newctrl)( xui_propdata* propdata );
typedef void				(*xui_prop_additem)( void* userptr );
typedef void				(*xui_prop_delitem)( void* userptr );
typedef xui_propdata*		(*xui_prop_newprop)( void* userptr, u32 index, xui_propkind* kind );

class   xui_object_pickwnd;
typedef xui_object_pickwnd*	(*xui_prop_newpick)( void );
typedef xui_bitmap*			(*xui_prop_geticon)( xui_propdata* propdata );
typedef std::wstring		(*xui_prop_getname)( xui_propdata* propdata );

typedef std::map<s32, std::wstring> xui_propenum_map;
typedef std::vector<xui_propdata*>  xui_propdata_vec;
typedef std::vector<xui_proproot*>	xui_proproot_vec;
typedef std::vector<std::string>	xui_droptype_vec;

class xui_propdata
{
public:
	xui_propdata( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func );
	virtual ~xui_propdata( void );

	xui_propkind*				get_kind		( void ) const;
	xui_prop_newctrl			get_func		( void ) const;
	const std::wstring&			get_name		( void ) const;
	void						set_name		( const std::wstring& name );
	bool						can_edit		( void ) const;
	void						set_edit		( bool flag );
	bool						can_show		( void ) const;
	void						set_show		( bool flag );
	u08*						get_byte		( void );
	bool						has_byte		( void ) const;
	std::wstring				get_path		( void );
	xui_propctrl*				get_ctrl		( void );
	void						set_ctrl		( xui_propctrl* ctrl );
	virtual void				non_ctrl		( void );
	virtual xui_propdata*		get_data		( const std::wstring& name );
	virtual std::wstring		get_path		( xui_propdata* prop );

	void						backups			( void );
	void						restore			( u08* byte );
	void						refresh			( void );

protected:
	virtual void				on_valuechanged	( void );
	virtual u08*				do_serialize	( void );
	virtual void				un_serialize	( u08* byte );

	template<typename T>
	u08*						get_byte		( const T& value )
	{
		u08* data = new u08[sizeof(T)];
		memcpy(data, &value, sizeof(T));
		return data;
	}
	template<typename T>
	T							get_cast		( u08* data )
	{
		return *((T*)data);
	}

	xui_propkind*				m_kind;
	std::wstring				m_name;
	xui_prop_newctrl			m_func;
	xui_propctrl*				m_ctrl;
	bool						m_edit;
	bool						m_show;
	u08*						m_byte;
};

class xui_propdata_bool : public xui_propdata
{
public:
	typedef bool				(*get_func)( void* userptr );
	typedef void				(*set_func)( void* userptr, bool value );

	xui_propdata_bool( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr );

	virtual bool				get_value		( void ) const;
	virtual void				set_value		( bool value );

protected:
	virtual u08*				do_serialize	( void );
	virtual void				un_serialize	( u08* byte );

	get_func					m_userget;
	set_func					m_userset;
	void*						m_userptr;
};

class xui_propdata_string : public xui_propdata
{
public:
	typedef std::wstring		(*get_func)( void* userptr );
	typedef void				(*set_func)( void* userptr, const std::wstring& value );

	xui_propdata_string( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr );

	virtual std::wstring		get_value		( void ) const;
	virtual void				set_value		( const std::wstring& value );

protected:
	virtual u08*				do_serialize	( void );
	virtual void				un_serialize	( u08* byte );

	get_func					m_userget;
	set_func					m_userset;
	void*						m_userptr;
};

class xui_propdata_number : public xui_propdata
{
public:
	xui_propdata_number( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, u08 numbtype, f64 interval, f64 minvalue, f64 maxvalue );

	u08							get_numbtype	( void ) const;
	f64							get_interval	( void ) const;
	f64							get_minvalue	( void ) const;
	f64							get_maxvalue	( void ) const;

	virtual f64					get_value		( void ) const = 0;
	virtual void				set_value		( f64 value )  = 0;

protected:
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

	xui_propdata_number_func( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr, u08 numbtype, f64 interval = 1, f64 minvalue = 0, f64 maxvalue = 0 );

	virtual f64					get_value		( void ) const;
	virtual void				set_value		( f64 value );

protected:
	virtual u08*				do_serialize	( void );
	virtual void				un_serialize	( u08* byte );

	get_func					m_userget;
	set_func					m_userset;
	void*						m_userptr;
};

template<typename T>
class xui_propdata_number_impl : public xui_propdata_number
{
public:
	xui_propdata_number_impl( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, T* ptr, u08 numbtype, f64 interval = 1, f64 minvalue = 0, f64 maxvalue = 0 )
	: xui_propdata_number(kind, name, func, numbtype, interval, minvalue, maxvalue)
	{
		m_ptr = ptr;
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
	virtual u08*				do_serialize	( void )
	{
		return get_byte<f64>(get_value());
	}
	virtual void				un_serialize	( u08* byte )
	{
		(*m_ptr) = (T)get_cast<f64>(byte);
	}

	T*							m_ptr;
};

class xui_propdata_enum : public xui_propdata
{
public:
	xui_propdata_enum( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, const xui_propenum_map& textmap );

	const xui_propenum_map&		get_textmap		( void ) const;

	virtual u32					get_value		( void ) const = 0;
	virtual void				set_value		( u32 index )  = 0;

protected:
	xui_propenum_map			m_textmap;
};

class xui_propdata_enum_func : public xui_propdata_enum
{
public:
	typedef s32					(*get_func)( void* userptr );
	typedef void				(*set_func)( void* userptr, s32 value );

	xui_propdata_enum_func( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, const xui_propenum_map& textmap, get_func userget, set_func userset, void* userptr );

	virtual u32					get_value		( void ) const;
	virtual void				set_value		( u32 index );

protected:
	virtual u08*				do_serialize	( void );
	virtual void				un_serialize	( u08* byte );

	get_func					m_userget;
	set_func					m_userset;
	void*						m_userptr;
};

template<typename T>
class xui_propdata_enum_impl : public xui_propdata_enum
{
public:
	xui_propdata_enum_impl( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, const xui_propenum_map& textmap, T* ptr )
	: xui_propdata_enum(kind, name, func, textmap)
	{
		m_ptr = ptr;
	}

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
	virtual u08*				do_serialize	( void )
	{
		return get_byte<u32>(get_value());
	}
	virtual void				un_serialize	( u08* byte )
	{
		xui_propenum_map::iterator itor = m_textmap.begin();
		std::advance(itor, get_cast<u32>(byte));
		(*m_ptr) = (T)(*itor).first;
	}

	T*							m_ptr;
};

class xui_expandbase
{
public:
	xui_expandbase( const xui_propdata_vec& subprop, bool subfold = true )
	{
		m_subprop = subprop;
		m_subfold = subfold;
	}

    ~xui_expandbase( void )
	{
		for (u32 i = 0; i < m_subprop.size(); ++i)
			delete m_subprop[i];
	}

	bool						can_subfold		( void ) const
	{
		return m_subfold;
	}
	const xui_propdata_vec&		get_subprop		( void ) const
	{
		return m_subprop;
	}

protected:
	bool						m_subfold;
	xui_propdata_vec			m_subprop;
};

class xui_expandvary : public xui_expandbase
{
public:
	typedef std::map<s32, std::vector<u32> > xui_propdata_map;
	static const xui_propdata_map empty_map;

	xui_expandvary( const xui_propdata_vec& subprop, bool subfold = true, const xui_propdata_map& showmap = empty_map, const xui_propdata_map& editmap = empty_map )
	: xui_expandbase(subprop, subfold)
	{
		m_showmap = showmap;
		m_editmap = editmap;
	}

	virtual void				syn_subprop		( void ) = 0;

protected:
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

	xui_propdata_map			m_showmap;
	xui_propdata_map			m_editmap;
};

class xui_propdata_expand : public xui_propdata, public xui_expandbase
{
public:
	xui_propdata_expand( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, const xui_propdata_vec& subprop, bool subfold = true );

	virtual void				non_ctrl		( void );
	virtual xui_propdata*		get_data		( const std::wstring& name );
	virtual std::wstring		get_path		( xui_propdata* prop );
};

class xui_propdata_expand_number : public xui_propdata_number_func, public xui_expandbase
{
public:
	xui_propdata_expand_number( 
		xui_propkind*								kind, 
		const std::wstring&							name, 
		xui_prop_newctrl							func, 
		const xui_propdata_vec&						subprop, 
		get_func									userget,
		set_func									userset,
		void*										userptr,
		u08											numbtype,
		f64											interval = 1.0,
		f64											minvalue = 0.0,
		f64											maxvalue = 0.0 )
		: xui_propdata_number_func(kind, name, func, userget, userset, userptr, numbtype, interval, minvalue, maxvalue), xui_expandbase(subprop)
	{}

	virtual void				non_ctrl		( void )
	{
		xui_propdata_number_func::non_ctrl();
		for (u32 i = 0; i < m_subprop.size(); ++i)
			m_subprop[i]->non_ctrl();
	}
};

class xui_propdata_expand_plus : public xui_propdata_expand
{
public:
	xui_propdata_expand_plus( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, const xui_propdata_vec& subprop, bool subfold = true );

	void						add_subprop		( xui_propdata* propdata );
	void						del_subprop		( xui_propdata* propdata );
	void						del_subpropall	( void );
};

class xui_propdata_expand_bool : public xui_propdata_bool, public xui_expandvary
{
public:
	xui_propdata_expand_bool( 
		xui_propkind*								kind, 
		const std::wstring&							name, 
		xui_prop_newctrl							func, 
		const xui_propdata_vec&						subprop, 
		get_func									userget,
		set_func									userset,
		void*										userptr,
		bool										subfold = true, 
		const std::map<s32, std::vector<u32> >&		showmap = xui_expandvary::empty_map,
		const std::map<s32, std::vector<u32> >&		editmap = xui_expandvary::empty_map )
	: xui_propdata_bool(kind, name, func, userget, userset, userptr), xui_expandvary(subprop, subfold, showmap, editmap)
	{
		syn_subprop();
	}

	virtual void				non_ctrl		( void )
	{
		xui_propdata_bool::non_ctrl();
		for (u32 i = 0; i < m_subprop.size(); ++i)
			m_subprop[i]->non_ctrl();
	}
	virtual void				syn_subprop		( void )
	{
		bool value = get_value();
		set_subshow(value ? 1 : 0);
		set_subedit(value ? 1 : 0);
	}

protected:
	virtual void				on_valuechanged	( void )
	{
		syn_subprop();
		xui_propdata_bool::on_valuechanged();
	}
	virtual void				un_serialize	( u08* byte )
	{
		xui_propdata_bool::un_serialize(byte);
		syn_subprop();
	}
};

class xui_propdata_expand_enum_func : public xui_propdata_enum_func, public xui_expandvary
{
public:
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
		syn_subprop();
	}

	virtual void				non_ctrl		( void )
	{
		xui_propdata_enum_func::non_ctrl();
		for (u32 i = 0; i < m_subprop.size(); ++i)
			m_subprop[i]->non_ctrl();
	}
	virtual void				syn_subprop		( void )
	{
		s32 value = (*m_userget)(m_userptr);
		set_subshow(value);
		set_subedit(value);
	}

protected:
	virtual void				on_valuechanged	( void )
	{
		syn_subprop();
		xui_propdata_enum_func::on_valuechanged();
	}
	virtual void				un_serialize	( u08* byte )
	{
		xui_propdata_enum_func::un_serialize(byte);
		syn_subprop();
	}
};

template<typename T>
class xui_propdata_expand_enum_impl : public xui_propdata_enum_impl<T>, public xui_expandvary
{
public:
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
		syn_subprop();
	}

	virtual void				non_ctrl		( void )
	{
		xui_propdata_enum_impl<T>::non_ctrl();
		for (u32 i = 0; i < m_subprop.size(); ++i)
			m_subprop[i]->non_ctrl();
	}
	virtual void				syn_subprop		( void )
	{
		s32 value = (s32)(*(xui_propdata_enum_impl<T>::m_ptr));
		set_subshow(value);
		set_subedit(value);
	}

protected:
	virtual void				on_valuechanged	( void )
	{
		syn_subprop();
		xui_propdata_enum_impl<T>::on_valuechanged();
	}
	virtual void				un_serialize	( u08* byte )
	{
		xui_propdata_enum_impl<T>::un_serialize(byte);
		syn_subprop();
	}
};

class xui_propdata_stdvec: public xui_propdata
{
public:
	xui_propdata_stdvec( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, xui_prop_additem addfunc, xui_prop_delitem delfunc, xui_prop_newprop newfunc );
	virtual ~xui_propdata_stdvec( void );

	const xui_propdata_vec&		get_propvec		( void ) const;

	virtual u32					get_value		( void ) const					= 0;
	virtual void				set_value		( u32 value )					= 0;
	virtual void				set_index		( u32 oldindex, u32 newindex )	= 0;

	virtual void				non_ctrl		( void );
	virtual xui_propdata*		get_data		( const std::wstring& name );
	virtual std::wstring		get_path		( xui_propdata* prop );

protected:
	xui_prop_additem			m_additem;
	xui_prop_delitem			m_delitem;
	xui_prop_newprop			m_newprop;
	xui_propdata_vec			m_propvec;
};

class xui_propdata_stdvec_root : public xui_propdata
{
public:
	typedef xui_proproot_vec	(*get_func)( void* userptr );
	typedef void				(*add_root)( void* userptr );
	typedef void				(*del_root)( void* userptr, xui_proproot* root );

	xui_propdata_stdvec_root( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, add_root addfunc, del_root delfunc, get_func userget, void* userptr );

	xui_proproot_vec			get_rootvec		( void );
	void						set_rootadd		( void );
	void						set_rootdel		( xui_proproot* root );
	bool						can_rootadd		( void );
	bool						can_rootdel		( void );

protected:
	get_func					m_userget;
	void*						m_userptr;
	add_root					m_addfunc;
	del_root					m_delfunc;
};

template<typename T>
class xui_propdata_stdvec_func : public xui_propdata_stdvec
{
public:
	typedef std::vector<T>&		(*get_func)( void* userptr );

	xui_propdata_stdvec_func( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, xui_prop_additem addfunc, xui_prop_delitem delfunc, xui_prop_newprop newfunc, get_func userget, void* userptr )
	: xui_propdata_stdvec(kind, name, func, addfunc, delfunc, newfunc)
	{
		m_userget = userget;
		m_userptr = userptr;
		realloc();
	}

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
			xui_propdata* propdata = (*m_newprop)(m_userptr, i, m_kind);
			propdata->set_name(name.str());

			m_propvec.push_back(propdata);
		}

		refresh();
	}

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
					(*m_additem)(m_userptr);
				}
				else
				{
					T ins;
					memset(&ins, 0, sizeof(T));
					vec.push_back(ins);
				}
			}

			realloc();
			on_valuechanged();
		}
		else
		if (vec.size() > value)
		{
			u32 count = vec.size() - value;
			for (u32 i = 0; i < count; ++i)
			{
				if (m_delitem)
				{
					(*m_delitem)(m_userptr);
				}
				else
				{
					u32 index = vec.size()-1;
					vec.erase(vec.begin()+index);
				}
			}

			realloc();
			on_valuechanged();
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
		on_valuechanged();
	}

protected:
	virtual u08*				do_serialize	( void )
	{
		std::vector<T>& vec = (*m_userget)(m_userptr);
		u08* data = new u08[sizeof(u32) + sizeof(T)*vec.size()];
		*((u32*)data) = vec.size();
		for (u32 i = 0; i < vec.size(); ++i)
		{
			u32 offset = sizeof(u32) + i * sizeof(T);
			memcpy(data+offset, &vec[i], sizeof(T));
		}

		return data;
	}
	virtual void				un_serialize	( u08* byte )
	{
		std::vector<T>& vec = (*m_userget)(m_userptr);
		vec.clear();

		u32 count = get_cast<u32>(byte);
		for (u32 i = 0; i < count; ++i)
		{
			u32 offset = sizeof(u32) + i * sizeof(T);
			vec.push_back(get_cast<T>(byte+offset));
		}

		realloc();
	}

	get_func					m_userget;
	void*						m_userptr;
};

template<typename T>
class xui_propdata_stdlst_func : public xui_propdata_stdvec
{
public:
	typedef std::list<T>&		(*get_func)( void* userptr );

	xui_propdata_stdlst_func( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, xui_prop_additem addfunc, xui_prop_delitem delfunc, xui_prop_newprop newfunc, get_func userget, void* userptr )
	: xui_propdata_stdvec(kind, name, func, addfunc, delfunc, newfunc)
	{
		m_userget = userget;
		m_userptr = userptr;
		realloc();
	}

	void						realloc			( void )
	{
		for (u32 i = 0; i < m_propvec.size(); ++i)
			delete m_propvec[i];

		m_propvec.clear();
		std::list<T>& lst = (*m_userget)(m_userptr);
		u32 i = 0;
        for (typename std::list<T>::iterator itor = lst.begin(); itor != lst.end(); ++itor, ++i)
		{
			std::wstringstream name;
			name << "Element_";
			name << i;
			xui_propdata* propdata = (*m_newprop)(m_userptr, i, m_kind);
			propdata->set_name(name.str());

			m_propvec.push_back(propdata);
		}

		refresh();
	}

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
					(*m_additem)(m_userptr);
				}
				else
				{
					T ins;
					memset(&ins, 0, sizeof(T));
					lst.push_back(ins);
				}
			}

			realloc();
			on_valuechanged();
		}
		else
		if (lst.size() > value)
		{
			u32 count = lst.size() - value;
			for (u32 i = 0; i < count; ++i)
			{
				if (m_delitem)
				{
					(*m_delitem)(m_userptr);
				}
				else
				{
					lst.pop_back();
				}
			}

			realloc();
			on_valuechanged();
		}
	}
	virtual void				set_index		( u32 oldindex, u32 newindex )
	{
        std::list<T>& lst = (*m_userget)(m_userptr);
		if (oldindex >= lst.size() ||
			newindex >  lst.size())
			return;

        typename std::list<T>::iterator itor;
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
		on_valuechanged();
	}

protected:
	virtual u08*				do_serialize	( void )
	{
		std::list<T>& lst = (*m_userget)(m_userptr);
		u08* data = new u08[sizeof(u32) + sizeof(T)*lst.size()];
		*((u32*)data) = lst.size();
		u32 offset = sizeof(u32);
        for (typename std::list<T>::iterator itor = lst.begin(); itor != lst.end(); ++itor)
		{
			memcpy(data+offset, &(*itor), sizeof(T));
			offset += sizeof(T);
		}

		return data;
	}
	virtual void				un_serialize	( u08* byte )
	{
		std::list<T>& lst = (*m_userget)(m_userptr);
		lst.clear();

		u32 count = get_cast<u32>(byte);
		for (u32 i = 0; i < count; ++i)
		{
			u32 offset = sizeof(u32) + i * sizeof(T);
			lst.push_back(get_cast<T>(byte+offset));
		}

		realloc();
	}

	get_func					m_userget;
	void*						m_userptr;
};

class xui_propdata_vector : public xui_propdata
{
public:
	typedef xui_vector<f64>		(*get_func)( void* userptr );
	typedef void				(*set_func)( void* userptr, const xui_vector<f64>& value );

	xui_propdata_vector( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr, u08 numbtype, f64 interval = 1, const xui_vector<f64>& defvalue = xui_vector<f64>(0) );

	u08							get_numbtype( void ) const;
	virtual f64					get_interval( void ) const;
	virtual void 				set_defvalue( void );
	virtual xui_vector<f64>		get_value	( void ) const;
	virtual void				set_value	( const xui_vector<f64>& value );

protected:
	virtual u08*				do_serialize( void );
	virtual void				un_serialize( u08* byte );

	get_func					m_userget;
	set_func					m_userset;
	void*						m_userptr;
	xui_vector<f64>				m_defvalue;
	f64							m_interval;
	u08							m_numbtype;
};

class xui_propdata_rect2d : public xui_propdata
{
public:
	typedef xui_rect2d<f64>		(*get_func)( void* userptr );
	typedef void				(*set_func)( void* userptr, const xui_rect2d<f64>& value );

	xui_propdata_rect2d( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr, u08 numbtype, f64 interval = 1 );

	u08							get_numbtype( void ) const;
	virtual f64					get_interval( void ) const;
	virtual xui_rect2d<f64>		get_value	( void ) const;
	virtual void				set_value	( const xui_rect2d<f64>& value );

protected:
	virtual u08*				do_serialize( void );
	virtual void				un_serialize( u08* byte );

	get_func					m_userget;
	set_func					m_userset;
	void*						m_userptr;
	f64							m_interval;
	u08							m_numbtype;
};

class xui_propdata_colour : public xui_propdata
{
public:
	typedef xui_colour			(*get_func)( void* userptr );
	typedef void				(*set_func)( void* userptr, const xui_colour& value );

	xui_propdata_colour( xui_propkind* kind, const std::wstring& name, xui_prop_newctrl func, get_func userget, set_func userset, void* userptr );

	virtual xui_colour			get_value	( void ) const;
	virtual void				set_value	( const xui_colour& value );

protected:
	virtual u08*				do_serialize( void );
	virtual void				un_serialize( u08* byte );

	get_func					m_userget;
	set_func					m_userset;
	void*						m_userptr;
};

class xui_propdata_object : public xui_propdata
{
public:
	xui_propdata_object( 
		xui_propkind*			kind, 
		const std::wstring&		name, 
		xui_prop_newctrl		func, 
		const std::string&		droptype, 
		xui_prop_newpick		pickfunc,
		xui_prop_geticon		iconfunc, 
		xui_prop_getname		namefunc );

	const xui_droptype_vec&		get_droptype( void ) const;
	bool						has_droptype( const std::string& type ) const;
	void						add_droptype( const std::string& type );
	xui_prop_newpick			get_pickfunc( void ) const;
	xui_prop_geticon			get_iconfunc( void ) const;
	xui_prop_getname			get_namefunc( void ) const;

	virtual void*				get_value	( void ) const  = 0;
	virtual void				set_value	( void* value ) = 0;
	virtual void				old_value	( void );
	virtual void				syn_value	( void );

	xui_method<xui_method_args>	xm_doubleclick;

protected:
	xui_droptype_vec			m_droptype;
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

	virtual void*				get_value	( void ) const;
	virtual void				set_value	( void* value );

protected:
	virtual u08*				do_serialize( void );
	virtual void				un_serialize( u08* byte );

	get_func					m_userget;
	set_func					m_userset;
	void*						m_userptr;
};

template<typename T>
class xui_propdata_object_impl : public xui_propdata_object
{
public:
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
		}
	}

protected:
	virtual u08*				do_serialize( void )
	{
		return get_byte<void*>(get_value());
	}
	virtual void				un_serialize( u08* byte )
	{
		(*m_ptr) = (T)get_cast<void*>(byte);
		m_oldvalue = (void*)(*m_ptr);
	}

	T*							m_ptr;
};

#endif//__xui_propdata_h__