#if 0
#include "xui_widget.h"
#include "xui_action_ctrl.h"

/*
//constructor
*/
xui_create_explain(xui_action_ctrl)( xui_widget* widget )
: m_loop(false)
, m_soft(false)
, m_play(false)
, m_steptime(0.0f)
, m_lasttime(0.0f)
, m_currtime(0.0f)
, m_event(-1)
, m_widget(NULL)
{}

/*
//method
*/
xui_method_explain(xui_action_ctrl,		set_event,			void)( u32 event )
{
	m_event = event;
}

xui_method_explain(xui_action_ctrl,		add_frame,			void)( s32 frame )
{
	m_framevec.push_back(frame);
}

xui_method_explain(xui_action_ctrl,		set_framevec,		void)( const std::vector<s32>& framevec )
{
	m_framevec = framevec;
}

xui_method_explain(xui_action_ctrl,		was_play,			bool)( void ) const
{
	return m_play;
}

xui_method_explain(xui_action_ctrl,		set_play,			void)( bool flag )
{
	if (m_play != flag)
	{
		m_play  = flag;

		xui_event_arg_action arg;
		arg.event = m_event;

		//回调
		if (m_play) 
		{
			m_widget->on_actionplay(m_widget, &arg);
			m_widget->xe_actionplay(m_widget, &arg);
		}
		else
		{
			m_widget->on_actionstop(m_widget, &arg);
			m_widget->xe_actionstop(m_widget, &arg);
		}
	}
}

xui_method_explain(xui_action_ctrl,		was_loop,			bool)( void ) const
{
	return m_loop;
}

xui_method_explain(xui_action_ctrl,		set_loop,			void)( bool flag )
{
	m_loop = flag;
}

xui_method_explain(xui_action_ctrl,		was_soft,			bool)( void ) const
{
	return m_soft;
}

xui_method_explain(xui_action_ctrl,		set_soft,			void)( bool flag )
{
	m_soft = flag;
}

xui_method_explain(xui_action_ctrl,		get_timetotal,		f32	)( void ) const
{
	return m_framevec.empty() ? 0.0f : (f32)m_framevec.back();
}

xui_method_explain(xui_action_ctrl,		get_time,			f32	)( void ) const
{
	return m_steptime;
}

xui_method_explain(xui_action_ctrl,		set_time,			void)( f32 time )
{
	m_steptime = time;
	m_lasttime = time * 0.016f;
	m_currtime = time * 0.016f;
}

xui_method_explain(xui_action_ctrl,		reset,				void)( void )
{
	m_steptime = 0.0f;
	m_lasttime = 0.0f;
	m_currtime = 0.0f;
	m_play = false;
}

xui_method_explain(xui_action_ctrl,		clear,				void)( void )
{
	reset();
	m_framevec.clear();
	m_soft = false;
	m_loop = false;
}

xui_method_explain(xui_action_ctrl,		update,				void)( f32 delta_time )
{
	if (m_play)
	{
		m_lasttime  = m_currtime;
		m_currtime += delta_time;
		m_steptime  = m_currtime / 0.016f;

		if (m_steptime > get_timetotal())
		{
			update_widget();

			m_play = m_loop;
			m_steptime = 0.0f;
			m_lasttime = 0.0f;
			m_currtime = 0.0f;

			xui_event_arg_action arg;
			arg.event = m_event;
			if (m_play) 
			{
				m_widget->on_actionloop(m_widget, &arg);
				m_widget->xe_actionloop(m_widget, &arg);
			}
			else
			{
				m_widget->on_actionstop(m_widget, &arg);
				m_widget->xe_actionloop(m_widget, &arg);
			}
		}
	}

	if (m_play)
		update_widget();
}

xui_method_explain(xui_action_ctrl,		get_currframeidx,	u32	)( void ) const
{
	if (!m_framevec.empty())
	{
		for (u32 i = m_framevec.size()-1; i >= 0; --i)
		{
			if (m_framevec[i] <= (s32)m_steptime)
				return i;
		}
	}

	return -1;
}

xui_method_explain(xui_action_ctrl,		get_nextframeidx,	u32	)( void ) const
{
	if (!m_framevec.empty())
	{
		for (u32 i = 0; i < m_framevec.size(); ++i)
		{
			if (m_framevec[i] >  (s32)m_steptime)
				return i;
		}
	}

	return -1;
}

xui_method_explain(xui_action_ctrl_move,add_movex,			void)( s32				 movex )
{
	m_movexvec.push_back(movex);
}

xui_method_explain(xui_action_ctrl_move,add_movey,			void)( s32				 movey )
{
	m_moveyvec.push_back(movey);
}

xui_method_explain(xui_action_ctrl_move,add_sizew,			void)( s32				 sizew )
{
	m_sizewvec.push_back(sizew);
}

xui_method_explain(xui_action_ctrl_move,add_sizeh,			void)( s32				 sizeh )
{
	m_sizehvec.push_back(sizeh);
}

xui_method_explain(xui_action_ctrl_move,add_alpha,			void)( f32				 alpha )
{
	m_alphavec.push_back(alpha);
}

xui_method_explain(xui_action_ctrl_move,add_color,			void)( const xui_colour& color )
{
	m_colorvec.push_back(color);
}

xui_method_explain(xui_action_ctrl_move,set_movexvec,		void)( const std::vector<s32>&		  movexvec )
{
	m_movexvec = movexvec;
}

xui_method_explain(xui_action_ctrl_move,set_moveyvec,		void)( const std::vector<s32>&		  moveyvec )
{
	m_moveyvec = moveyvec;
}

xui_method_explain(xui_action_ctrl_move,set_sizewvec,		void)( const std::vector<s32>&		  sizewvec )
{
	m_sizewvec = sizewvec;
}

xui_method_explain(xui_action_ctrl_move,set_sizehvec,		void)( const std::vector<s32>&		  sizehvec )
{
	m_sizehvec = sizehvec;
}

xui_method_explain(xui_action_ctrl_move,set_alphavec,		void)( const std::vector<f32>&		  alphavec )
{
	m_alphavec = alphavec;
}

xui_method_explain(xui_action_ctrl_move,set_colorvec,		void)( const std::vector<xui_colour>& colorvec )
{
	m_colorvec = colorvec;
}

xui_method_explain(xui_action_ctrl_move,set_actiondata,		void)( const xui_action_data& data )
{
	m_soft = data.soft;
	m_loop = data.loop;

	m_framevec = data.framevec;
	m_movexvec = data.movexvec;
	m_moveyvec = data.moveyvec;
	m_sizewvec = data.sizehvec;
	m_sizehvec = data.sizehvec;
	m_alphavec = data.alphavec;
	m_colorvec = data.colorvec;
}

xui_method_explain(xui_action_ctrl_move,clear,				void)( void )
{
	xui_action_ctrl::clear();
	m_movexvec.clear();
	m_moveyvec.clear();
	m_sizewvec.clear();
	m_sizehvec.clear();
	m_alphavec.clear();
	m_colorvec.clear();
}

xui_method_explain(xui_action_ctrl_move,update_widget,		void)( void )
{
	u32 curridx = get_currframeidx();
	u32 nextidx = get_nextframeidx();

	if (curridx != -1)
	{
		f32 s = (nextidx != -1)
			? (f32)(m_steptime - m_framevec[curridx]) / (f32)(m_framevec[nextidx] - m_framevec[curridx])
			: 1.0f;

		//位移
		if (m_movexvec.size() > 0			&&
			m_widget->get_alignhorz() == 0	&& 
			m_widget->get_dockstyle() == 0)
		{
			s32				movex  =        m_movexvec[curridx];
			if (nextidx != -1)
			{
				if (m_soft)	movex += (s32)((m_movexvec[nextidx] - m_movexvec[curridx]) * sin(xui_pi / 2.0f * s));
				else		movex += (s32)((m_movexvec[nextidx] - m_movexvec[curridx]) * s);
			}

			//设置
			m_widget->set_renderx(movex);
		}
		if (m_moveyvec.size() > 0			&&
			m_widget->get_alignvert() == 0	&& 
			m_widget->get_dockstyle() == 0)
		{
			s32				movey  =        m_moveyvec[curridx];
			if (nextidx != -1)
			{
				if (m_soft)	movey += (s32)((m_moveyvec[nextidx] - m_moveyvec[curridx]) * sin(xui_pi / 2.0f * s));
				else		movey += (s32)((m_moveyvec[nextidx] - m_moveyvec[curridx]) * s);
			}

			//设置
			m_widget->set_rendery(movey);
		}

		//大小
		if (m_sizewvec.size() > 0)
		{
			s32				sizew  =        m_sizewvec[curridx];
			if (nextidx != -1)
			{
				if (m_soft)	sizew += (s32)((m_sizewvec[nextidx] - m_sizewvec[curridx]) * sin(xui_pi / 2.0f * s));
				else		sizew += (s32)((m_sizewvec[nextidx] - m_sizewvec[curridx]) * s);
			}

			//设置
			m_widget->set_renderw(sizew);
		}
		if (m_sizehvec.size() > 0)
		{
			s32				sizeh  =        m_sizehvec[curridx];
			if (nextidx != -1)
			{
				if (m_soft)	sizeh += (s32)((m_sizehvec[nextidx] - m_sizehvec[curridx]) * sin(xui_pi / 2.0f * s));
				else		sizeh += (s32)((m_sizehvec[nextidx] - m_sizehvec[curridx]) * s);
			}

			//设置
			m_widget->set_renderh(sizeh);
		}


		//透明
		if (m_alphavec.size() > 0)
		{
			f32				alpha  =        m_alphavec[curridx];
			if (nextidx != -1)
			{
				if (m_soft)	alpha +=       (m_alphavec[nextidx] - m_alphavec[curridx]) * sin(xui_pi / 2.0f * s);
				else		alpha +=       (m_alphavec[nextidx] - m_alphavec[curridx]) * s;
			}

			//设置
			m_widget->set_backalpha(alpha);
		}

		//颜色
		if (m_colorvec.size() > 0)
		{
			xui_colour		color  =        m_colorvec[curridx];
			if (nextidx != -1)
			{
				if (m_soft)	color +=       (m_colorvec[nextidx] - m_colorvec[curridx]) * sin(xui_pi / 2.0f * s);
				else		color +=       (m_colorvec[nextidx] - m_colorvec[curridx]) * s;
			}

			//设置
			m_widget->set_backcolor(color);
		}
	}
}
#endif