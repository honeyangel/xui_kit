#ifndef __cocos_game_h__
#define __cocos_game_h__

#include "xui_dockpage.h"

namespace cocos2d
{
	class Node;
}

typedef std::vector< xui_vector<f64> > cocos_sizedata;
class cocos_renderview;
class cocos_propcsd;
class cocos_game : public xui_dockpage
{
	xui_declare_rtti

public:
	/*
	//constructor
	*/
	cocos_game( void );

	/*
	//destructor
	*/
	virtual ~cocos_game( void );

	/*
	//method
	*/
	cocos_propcsd*			get_viewprop			( void );
	void					set_viewprop			( cocos_propcsd* prop );
	cocos_sizedata&			get_sizedata			( void );
	void					add_sizedata			( void );
	void					del_sizedata			( void );
	xui_propdata*			new_sizeprop			( u32 index, xui_propkind* kind );
	void					set_dropctrlupdate		( void );

	/*
	//config
	*/
	void					load_config				( void );
	void					save_config				( void );

protected:
	/*
	//callback
	*/
	virtual void			on_perform				( xui_method_args& args );
	virtual void			on_renderself			( xui_method_args& args );

	/*
	//event
	*/
	void					on_userctrlclick		( xui_component* sender, xui_method_args&		args );
	void					on_dropctrlselection	( xui_component* sender, xui_method_args&		args );
	void					on_drawviewupdateself	( xui_component* sender, xui_method_update&		args );
	void					on_drawviewrenderself	( xui_component* sender, xui_method_args&		args );
	void					on_drawviewrenderelse	( xui_component* sender, xui_method_args&		args );
	void					on_propsizepropchanged	( xui_component* sender, xui_method_propdata&	args );

	/*
	//static
	*/
	static cocos_sizedata&	get_sizedata			( void* userptr );
	static void				add_sizedata			( void* userptr );
	static void				del_sizedata			( void* userptr );
	static xui_propdata*	new_sizeprop			( void* userptr, u32 index, xui_propkind* kind );
	static xui_vector<f64>	get_size				( void* userptr );
	static void				set_size				( void* userptr, const xui_vector<f64>& value );

	/*
	//member
	*/
	cocos_propcsd*			m_viewprop;
	cocos2d::Node*			m_rootnode;
	xui_panel*				m_headpane;
	xui_toolbar*			m_linetool;
	cocos_renderview*		m_drawview;
	xui_dropbox*			m_dropctrl;
	xui_button*				m_userctrl;
	xui_proproot*			m_propsize;
	cocos_sizedata			m_sizedata;
};

#endif//__cocos_game_h__