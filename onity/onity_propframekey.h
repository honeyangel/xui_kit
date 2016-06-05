#ifndef __onity_propframekey_h__
#define __onity_propframekey_h__

#include "onity_proptransref.h"

class NP2DSFrameKey;
class onity_proplayer;
class onity_propframekey : public onity_proptransref
{
public:
	/*
	//constructor
	*/
	onity_propframekey( onity_propfile* propfile, onity_proplayer* proplayer, NP2DSFrameKey* framekey );

	/*
	//method
	*/
	void					set_transref	( void );
	NP2DSFrameKey*			get_framekey	( void );
	onity_proplayer*		get_layer		( void );

protected:
	/*
	//static
	*/
	static bool				was_smooth		( void* userptr );
	static void				set_smooth		( void* userptr, bool  value );
	static void*			get_spark		( void* userptr );
	static void				set_spark		( void* userptr, void* value );
	static xui_bitmap*		get_sparkicon	( xui_propdata* propdata );
	static std::wstring		get_sparkname	( xui_propdata* propdata );
	static std::wstring		get_event		( void* userptr );
	static void				set_event		( void* userptr, const std::wstring& value );
	static std::wstring		get_sound		( void* userptr );
	static void				set_sound		( void* userptr, const std::wstring& value );
	static void*			get_asset		( void* userptr );
	static void				set_asset		( void* userptr, void* value );

	/*
	//member
	*/
	xui_propkind*			m_reskind;
	xui_propkind*			m_keykind;
	onity_proplayer*		m_proplayer;
	NP2DSFrameKey*			m_framekey;
};

#endif//__onity_propframekey_h__