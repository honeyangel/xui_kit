#ifndef __xui_action_ctrl_h__
#define __xui_action_ctrl_h__

#include "xui_header.h"

template<class T>
class xui_action_ctrl
{
public:
	/*
	//constructor
	*/
	xui_action_ctrl( xui_component* comp );

	/*
	//method
	*/
	void						add_time			( f32 time );
	void						set_timevec			( const std::vector<f32>& timevec );
	bool						has_data			( void ) const;
	T							get_data			( u32 index ) const;
	void						add_data			( const T& data );
	void						set_datavec			( const std::vector<T>& datavec );

	bool						was_play			( void ) const;
	void						set_play			( bool flag );
	bool						was_loop			( void ) const;
	void						set_loop			( bool flag );
	bool						was_soft			( void ) const;
	void						set_soft			( bool flag );

	f32							get_timetotal		( void ) const;
	f32							get_time			( void ) const;
	void						set_time			( f32 time );

	void						reset				( void );
	void						clear				( void );
	void						update				( f32 delta );
	T							sample				( void );

	/*
	//method
	*/
	xui_method<xui_method_args> xm_play;
	xui_method<xui_method_args> xm_stop;
	xui_method<xui_method_args> xm_loop;
	xui_method<xui_method_args>	xm_tick;

protected:
	/*
	//method
	*/
	u32							get_currtimeidx		( void ) const;
	u32							get_nexttimeidx		( void ) const;

	/*
	//member
	*/
	xui_component*				m_comp;
	bool						m_loop;
	bool						m_soft;
	bool						m_play;
	f32							m_time;
	std::vector<f32>			m_timevec;
	std::vector<T>				m_datavec;
};

/*
//constructor
*/
template<class T>
xui_action_ctrl<T>::xui_action_ctrl( xui_component* comp )
: m_comp(comp)
, m_loop(false)
, m_soft(false)
, m_play(false)
, m_time(0.0f)
{}

/*
//method
*/
template<class T>
inline
void xui_action_ctrl<T>::add_time		( f32 time )
{
	m_timevec.push_back(time);
}
template<typename T>
void xui_action_ctrl<T>::set_timevec	( const std::vector<f32>& timevec )
{
	m_timevec = timevec;
}
template<typename T>
bool xui_action_ctrl<T>::has_data		( void ) const
{
	return m_datavec.size() > 0;
}
template<typename T>
T    xui_action_ctrl<T>::get_data		( u32 index ) const
{
	return m_datavec[index];
}
template<typename T>
void xui_action_ctrl<T>::add_data		( const T& data )
{
	m_datavec.push_back(data);
}
template<typename T>
void xui_action_ctrl<T>::set_datavec	( const std::vector<T>& datavec )
{
	m_datavec = datavec;
}
template<typename T>
bool xui_action_ctrl<T>::was_play		( void ) const
{
	return m_play;
}
template<typename T>
void xui_action_ctrl<T>::set_play		( bool flag )
{
	if (m_play != flag)
	{
		m_play  = flag;

		xui_method_args args;
		args.wparam = this;

		if (m_play)	xm_play(m_comp, args);
		else		xm_stop(m_comp, args);
	}
}
template<typename T>
bool xui_action_ctrl<T>::was_loop		( void ) const
{
	return m_loop;
}
template<typename T>
void xui_action_ctrl<T>::set_loop		( bool flag )
{
	m_loop = flag;
}
template<typename T>
bool xui_action_ctrl<T>::was_soft		( void ) const
{
	return m_soft;
}
template<typename T>
void xui_action_ctrl<T>::set_soft		( bool flag )
{
	m_soft = flag;
}
template<typename T>
f32	 xui_action_ctrl<T>::get_timetotal	( void ) const
{
	return m_timevec.empty() ? 0.0f : m_timevec.back();
}
template<typename T>
f32  xui_action_ctrl<T>::get_time		( void ) const
{
	return m_time;
}
template<typename T>
void xui_action_ctrl<T>::set_time		( f32 time )
{
	m_time = time;
}
template<typename T>
void xui_action_ctrl<T>::reset			( void )
{
	m_time = 0.0f;
	m_play = false;
	m_loop = false;
}
template<typename T>
void xui_action_ctrl<T>::clear			( void )
{
	reset();
	m_timevec.clear();
	m_datavec.clear();
}
template<typename T>
void xui_action_ctrl<T>::update			( f32 delta )
{
	if (m_play)
	{
		m_time += delta;

		xui_method_args args;
		args.wparam = this;
		xm_tick(m_comp, args);

		if (m_time > get_timetotal())
		{
			m_play = m_loop;
			m_time = 0.0f;

			if (m_play)	xm_loop(m_comp, args); 
			else		xm_stop(m_comp, args);
		}
	}
}
template<typename T>
T	 xui_action_ctrl<T>::sample			( void )
{
	u32 curridx = get_currtimeidx();
	u32 nextidx = get_nexttimeidx();

	T result(0);
	if (curridx != -1)
	{
		f32 s = (nextidx != -1)
			? (f32)(m_time - m_timevec[curridx]) / (f32)(m_timevec[nextidx] - m_timevec[curridx])
			: 1.0f;

		result = m_datavec[curridx];
		if (nextidx != -1)
		{
			if (m_soft)	result += (T)(((m_datavec[nextidx] - m_datavec[curridx]) * sin(xui_pi / 2.0f * s)));
			else		result += (T)(((m_datavec[nextidx] - m_datavec[curridx]) * s));
		}
	}

	return result;
}
template<typename T>
u32  xui_action_ctrl<T>::get_currtimeidx( void ) const
{
	if (!m_timevec.empty())
	{
		for (u32 i = m_timevec.size()-1; i >= 0; --i)
		{
			if (m_timevec[i] <= m_time)
				return i;
		}
	}

	return -1;
}
template<typename T>
u32  xui_action_ctrl<T>::get_nexttimeidx( void ) const
{
	if (!m_timevec.empty())
	{
		for (u32 i = 0; i < m_timevec.size(); ++i)
		{
			if (m_timevec[i] >  m_time)
				return i;
		}
	}

	return -1;
}

#endif//__xui_action_ctrl_h__