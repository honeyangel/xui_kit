#include "onity_propedit.h"

extern s32 xui_round( f64 value );

/*
//constructor
*/
xui_create_explain(onity_propedit)( void )
: onity_proproot()
, m_lockdata(false)
, m_position(xui_vector<s32>(0))
, m_bounding(xui_rect2d<s32>(0))
{}

/*
//virtual
*/
xui_method_explain(onity_propedit, get_position,	xui_vector<s32>	)( const xui_vector<s32>& trans, f64 ratio )
{
	if (m_lockdata == false)
	{
		m_position    = ori_position();
		m_position.x  = xui_round(m_position.x *ratio + trans.x*ratio);
		m_position.y  = xui_round(m_position.y *ratio + trans.y*ratio);
	}

	return m_position;
}
xui_method_explain(onity_propedit, set_position,	void			)( const xui_vector<s32>& pos )
{}
xui_method_explain(onity_propedit, ori_position,	xui_vector<s32>	)( void )
{
	return xui_vector<s32>(0);
}
xui_method_explain(onity_propedit, ori_bounding,	xui_rect2d<s32>	)( void )
{
	return xui_rect2d<s32>(0);
}
xui_method_explain(onity_propedit, get_bounding,	xui_rect2d<s32>	)( const xui_vector<s32>& trans, f64 ratio )
{
	if (m_lockdata == false)
	{
		m_bounding    = ori_bounding();
		m_bounding.ax = xui_round(m_bounding.ax*ratio + trans.x*ratio);
		m_bounding.ay = xui_round(m_bounding.ay*ratio + trans.y*ratio);
		m_bounding.bx = xui_round(m_bounding.bx*ratio + trans.x*ratio);
		m_bounding.by = xui_round(m_bounding.by*ratio + trans.y*ratio);
	}

	return m_bounding;
}
xui_method_explain(onity_propedit, mov_position,	void			)( const xui_vector<s32>& delta )
{
	m_bounding += delta;
	m_position += delta;
}
xui_method_explain(onity_propedit, set_lockdata,	void			)( bool flag )
{
	m_lockdata = flag;
}