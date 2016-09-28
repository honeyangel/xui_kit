#include "NP2DSImage.h"
#include "NP2DSFrame.h"
#include "NP2DSActor.h"
#include "NP2DSImageRef.h"
#include "NP2DSFrameRef.h"
#include "NP2DSActorRef.h"
#include "NPParticleSFX.h"

#include "xui_convas.h"
#include "xui_treeview.h"
#include "xui_treenode.h"
#include "xui_treedata.h"
#include "xui_toggle.h"
#include "xui_menu.h"
#include "xui_menuitem.h"
#include "xui_propview.h"
#include "onity_resource.h"
#include "onity_renderdata.h"
#include "onity_propnodemodule.h"
#include "onity_propnodesprite.h"
#include "onity_propnodeaction.h"
#include "onity_propnodeparticle.h"
#include "onity_mainform.h"
#include "onity_inspector.h"
#include "onity_propctrl_rendertree.h"

//////////////////////////////////////////////////////////////////////////
//propdata
//////////////////////////////////////////////////////////////////////////
/*
//constructor
*/
xui_create_explain(onity_propdata_rendertree)( xui_propkind* kind, const std::wstring& name, NPNode* root )
: xui_propdata(kind, name, onity_propctrl_rendertree::create)
, m_root(root)
{}

/*
//method
*/
xui_method_explain(onity_propdata_rendertree, get_root,				NPNode*			)( void )
{
	return m_root;
}

//////////////////////////////////////////////////////////////////////////
//propctrl
//////////////////////////////////////////////////////////////////////////
xui_implement_rtti(onity_propctrl_rendertree, xui_propctrl);

/*
//constructor
*/
xui_create_explain(onity_propctrl_rendertree)( void )
: xui_propctrl()
{
	m_border	= xui_rect2d<s32>(0, 8, 0, 8);
	m_header	= new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_header,	set_parent			)(this);
	xui_method_ptrcall(m_header,	set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_header,	set_borderrt		)(xui_rect2d<s32>(8, 2, 2, 2));
	xui_method_ptrcall(m_header,	ini_drawer			)(L"Graphics");
	m_multip	= new xui_drawer(xui_vector<s32>(128, 20));
	xui_method_ptrcall(m_multip,	set_parent			)(this);
	xui_method_ptrcall(m_multip,	set_textalign		)(TEXTALIGN_LC);
	xui_method_ptrcall(m_multip,	set_borderrt		)(xui_rect2d<s32>(8, 2, 2, 2));
	xui_method_ptrcall(m_multip,	ini_component		)(true, false);
	xui_method_ptrcall(m_multip,	ini_drawer			)(L"Multi-object");

	m_delete	= new xui_button(xui_vector<s32>(24, 16));
	xui_method_ptrcall(m_delete,	set_parent			)(this);
	xui_method_ptrcall(m_delete,	set_backcolor		)(xui_colour::white);
	xui_method_ptrcall(m_delete,	set_movecolor		)(xui_button::default_downcolor);
	xui_method_ptrcall(m_delete,	xm_buttonclick		) += new xui_method_member<xui_method_args, onity_propctrl_rendertree>(this, &onity_propctrl_rendertree::on_deleteclick);
	xui_method_ptrcall(m_delete,	xm_renderself		) += new xui_method_member<xui_method_args, onity_propctrl_rendertree>(this, &onity_propctrl_rendertree::on_deleterenderself);

	std::vector<xui_treecolumn> columninfo;
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 200, L"name", NULL, 0, false));
	m_middle	= new xui_treeview(xui_vector<s32>(200), columninfo, 20, PLUSRENDER_SYMBOL, false, false, true);
	xui_method_ptrcall(m_middle,	set_parent			)(this);
	xui_method_ptrcall(m_middle,	set_drawcolor		)(false);
	xui_method_ptrcall(m_middle,	set_sidestyle		)(SIDESTYLE_N);
	xui_method_ptrcall(m_middle,	set_borderrt		)(xui_rect2d<s32>(2));
	xui_method_ptrcall(m_middle,	set_hscrollauto		)(false);
	xui_method_ptrcall(m_middle,	set_allowmulti		)(false);
	xui_method_ptrcall(m_middle,	set_acceptdrag		)(false);
	xui_method_ptrcall(m_middle,	xm_invalid			) += new xui_method_member<xui_method_args, onity_propctrl_rendertree>(this, &onity_propctrl_rendertree::on_middleinvalid);
	xui_method_ptrcall(m_middle,	xm_selectedchange	) += new xui_method_member<xui_method_args, onity_propctrl_rendertree>(this, &onity_propctrl_rendertree::on_middleselection);
	xui_method_ptrcall(m_middle,	xm_treenodeenter	) += new xui_method_member<xui_method_args, onity_propctrl_rendertree>(this, &onity_propctrl_rendertree::on_middlenodeenter);
	xui_method_ptrcall(m_middle,	xm_treenodeleave	) += new xui_method_member<xui_method_args, onity_propctrl_rendertree>(this, &onity_propctrl_rendertree::on_middlenodeleave);

	xui_menu* menu = xui_menu::create(80);
	m_module	= menu->add_item(NULL, L"Module");
	m_sprite	= menu->add_item(NULL, L"Sprite");
	m_action	= menu->add_item(NULL, L"Action");
	m_particle	= menu->add_item(NULL, L"Particle");
	m_container = menu->add_item(NULL, L"Container");
	xui_method_ptrcall(m_module,	xm_click			) += new xui_method_member<xui_method_args, onity_propctrl_rendertree>(this, &onity_propctrl_rendertree::on_menuclick);
	xui_method_ptrcall(m_sprite,	xm_click			) += new xui_method_member<xui_method_args, onity_propctrl_rendertree>(this, &onity_propctrl_rendertree::on_menuclick);
	xui_method_ptrcall(m_action,	xm_click			) += new xui_method_member<xui_method_args, onity_propctrl_rendertree>(this, &onity_propctrl_rendertree::on_menuclick);
	xui_method_ptrcall(m_particle,	xm_click			) += new xui_method_member<xui_method_args, onity_propctrl_rendertree>(this, &onity_propctrl_rendertree::on_menuclick);
	xui_method_ptrcall(m_container,	xm_click			) += new xui_method_member<xui_method_args, onity_propctrl_rendertree>(this, &onity_propctrl_rendertree::on_menuclick);

	m_insert = new xui_toggle(xui_vector<s32>(24, 16), TOGGLE_BUTTON);
	xui_method_ptrcall(m_insert,	set_parent			)(this);
	xui_method_ptrcall(m_insert,	set_backcolor		)(xui_colour::white);
	xui_method_ptrcall(m_insert,	set_movecolor		)(xui_button::default_downcolor);
	xui_method_ptrcall(m_insert,	set_menu			)(menu);
	xui_method_ptrcall(m_insert,	xm_renderself		) += new xui_method_member<xui_method_args, onity_propctrl_rendertree>(this, &onity_propctrl_rendertree::on_insertrenderself);

	m_widgetvec.push_back(m_header);
	m_widgetvec.push_back(m_insert);
	m_widgetvec.push_back(m_delete);
	m_widgetvec.push_back(m_middle);
	m_widgetvec.push_back(m_multip);
}

/*
//static
*/
xui_method_explain(onity_propctrl_rendertree, create,				xui_propctrl*	)( xui_propdata* propdata )
{
	return new onity_propctrl_rendertree();
}

/*
//override
*/
xui_method_explain(onity_propctrl_rendertree, on_linkpropdata,		void			)( void )
{
	xui_method_ptrcall(m_middle, del_upmostnodeall	)();
	if (m_propdatavec.size() == 1)
	{
		onity_propdata_rendertree* datarender = dynamic_cast<onity_propdata_rendertree*>(m_propdata);
		NPNode* renderroot = datarender->get_root();
		add_rendernode(NULL, renderroot);
	}

	xui_method_ptrcall(m_multip, ini_component		)(true, m_middle->get_upmostnodecount() == 0);
	xui_method_ptrcall(m_delete, set_enable			)(false);
	xui_method_ptrcall(m_middle, refresh			)();
}
xui_method_explain(onity_propctrl_rendertree, on_editvalue,			void			)( xui_propedit* sender )
{}

/*
//callback
*/
xui_method_explain(onity_propctrl_rendertree, on_renderback,		void			)( xui_method_args& args )
{
	xui_propctrl::on_renderback(args);

	xui_rect2d<s32> rt;
	xui_colour		color = get_vertexcolor();
	rt = m_header->get_renderrtabs();
	xui_convas::get_ins()->fill_round(rt, xui_colour(1.0f, 0.4f) * color, xui_rect2d<s32>(3, 3, 0, 0));
	rt = m_middle->get_renderrtabs();
	xui_convas::get_ins()->fill_round(rt, xui_colour(1.0f, 0.3f) * color, xui_rect2d<s32>(0, 0, 0, 3));
	rt = m_insert->get_renderrtabs().get_union(m_delete->get_renderrtabs());
	xui_convas::get_ins()->fill_round(rt, xui_colour(1.0f, 0.3f) * color, xui_rect2d<s32>(0, 0, 3, 3));

	color *= m_sidecolor;
	rt = m_header->get_renderrtabs();
	xui_convas::get_ins()->draw_round(rt, color, xui_rect2d<s32>(3, 3, 0, 0));
	rt = m_middle->get_renderrtabs();
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax,		rt.by-3	), xui_vector<s32>(rt.ax,		rt.ay	), color);
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx-48,	rt.by	), xui_vector<s32>(rt.ax+3,		rt.by   ), color);
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx,		rt.ay	), xui_vector<s32>(rt.bx,		rt.by	), color);
	xui_convas::get_ins()->draw_arc (xui_rect2d<s32>(
		rt.ax,			 
		rt.by-6,
		rt.ax+6, 
		rt.by), color, 90, 90, 1);

	rt = m_insert->get_renderrtabs().get_union(m_delete->get_renderrtabs());
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx,		rt.ay	), xui_vector<s32>(rt.bx,		rt.by-3	), color);
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.bx-3,	rt.by	), xui_vector<s32>(rt.ax+3,		rt.by	), color);
	xui_convas::get_ins()->draw_line(xui_vector<s32>(rt.ax,		rt.by-3	), xui_vector<s32>(rt.ax,		rt.ay	), color);
	xui_convas::get_ins()->draw_arc (xui_rect2d<s32>(
		rt.ax,			 
		rt.by-6,
		rt.ax+6, 
		rt.by), color, 90, 90, 1);
	xui_convas::get_ins()->draw_arc (xui_rect2d<s32>(
		rt.bx-6, 
		rt.by-6,			
		rt.bx,		   
		rt.by), color,  0, 90, 1);
}
xui_method_explain(onity_propctrl_rendertree, on_invalid,			void			)( xui_method_args& args )
{
	xui_vector<s32> sz;
	sz.w  = get_renderw();
	sz.h  = m_border.ay + m_border.by;
	sz.h += m_header->get_renderh();
	sz.h += m_middle->get_renderh();
	sz.h += m_insert->get_renderh();
	if (get_rendersz() != sz)
	{
		set_rendersz(sz);
	}
	else
	{
		perform();
	}
}
xui_method_explain(onity_propctrl_rendertree, on_perform,			void			)( xui_method_args& args )
{
	xui_propctrl::on_perform(args);

	s32 indent = get_indent();
	xui_rect2d<s32> rt = get_renderrtins();
	xui_vector<s32> pt = rt.get_pt();
	//header
	m_header->on_perform_x (pt.x+indent);
	m_header->on_perform_y (pt.y);
	m_header->on_perform_w (rt.get_w()-indent);
	//middle
	pt.y += m_header->get_renderh();
	m_middle->on_perform_x (pt.x+indent);
	m_middle->on_perform_y (pt.y);
	m_middle->on_perform_w (rt.get_w()-indent);
	m_multip->on_perform_x (pt.x+indent);
	m_multip->on_perform_y (pt.y+m_middle->get_borderrt().ay);
	m_multip->on_perform_w (rt.get_w()-indent);
	//delete
	pt.x  = rt.bx - m_delete->get_renderw();
	pt.y += m_middle->get_renderh();
	m_delete->on_perform_x (pt.x);
	m_delete->on_perform_y (pt.y);
	//insert
	pt.x -= m_insert->get_renderw();
	m_insert->on_perform_x (pt.x);
	m_insert->on_perform_y (pt.y);
}

/*
//event
*/
xui_method_explain(onity_propctrl_rendertree, on_middlenodeenter,	void			)( xui_component* sender, xui_method_args& args )
{
	xui_treenode*		  node = (xui_treenode*)args.wparam;
	onity_treedata*		  data = (onity_treedata*)node->get_linkdata();
	onity_propnoderender* prop = dynamic_cast<onity_propnoderender*>(data->get_prop());
	NPNode* rendernode  = prop->get_node();
	NPObjectRef* value  = NULL;
	if (NPIsExaKindOf(NP2DSImageRef, rendernode))
	{
		NP2DSImageRef* imageref = NPDynamicCast(NP2DSImageRef, rendernode);
		value = imageref->GetImage();
	}
	else
	if (NPIsExaKindOf(NP2DSFrameRef, rendernode))
	{
		NP2DSFrameRef* frameref = NPDynamicCast(NP2DSFrameRef, rendernode);
		value = frameref->GetFrame();
	}
	else
	if (NPIsExaKindOf(NP2DSActorRef, rendernode))
	{
		NP2DSActorRef* actorref = NPDynamicCast(NP2DSActorRef, rendernode);
		value = actorref->GetActor();
	}
	else
	if (NPIsExaKindOf(NPParticleSFX, rendernode))
	{
		value = rendernode;
	}

	if (value == NULL)
		return;

	onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
	inspector->show_tips(value, node->get_screenpt());
}
xui_method_explain(onity_propctrl_rendertree, on_middlenodeleave,	void			)( xui_component* sender, xui_method_args& args )
{
	onity_inspector* inspector = onity_mainform::get_ptr()->get_inspector();
	inspector->hide_tips();
}
xui_method_explain(onity_propctrl_rendertree, on_middleinvalid,		void			)( xui_component* sender, xui_method_args& args )
{
	std::vector<xui_treenode*> nodes = m_middle->get_entirenode(false);
	s32 h = (nodes.size() == 0) ? m_multip->get_renderh() : (nodes.size() * m_middle->get_lineheight());
	h += m_middle->get_borderrt().ay;
	h += m_middle->get_borderrt().by;
	m_middle->set_renderh(h);
}
xui_method_explain(onity_propctrl_rendertree, on_middleselection,	void			)( xui_component* sender, xui_method_args& args )
{
	xui_propview* propview = m_propdata->get_ctrl()->get_propview();
	xui_proproot_vec props = get_renderprop();
	std::vector<xui_treenode*> nodes = m_middle->get_selectednode();
	if (nodes.size() > 0)
	{
		xui_treenode*   node = nodes.front();
		onity_treedata* data = (onity_treedata*)node->get_linkdata();
		propview->add_propelse(props, data->get_prop());
		xui_method_ptrcall(m_delete, set_enable)(node->get_rootnode() != NULL);
	}
	else
	{
		propview->del_propelse(props);
		xui_method_ptrcall(m_delete, set_enable)(false);
	}
}
xui_method_explain(onity_propctrl_rendertree, on_deleteclick,		void			)( xui_component* sender, xui_method_args& args )
{
	std::vector<xui_treenode*> nodes = m_middle->get_selectednode();
	if (nodes.size() > 0)
	{
		xui_treenode* node = nodes.front();
		xui_treenode* root = node->get_rootnode();
		if (root)
		{
			onity_treedata*		  rootdata = (onity_treedata*)root->get_linkdata();
			onity_propnoderender* rootprop = dynamic_cast<onity_propnoderender*>(rootdata->get_prop());
			onity_treedata*		  nodedata = (onity_treedata*)node->get_linkdata();
			onity_propnoderender* nodeprop = dynamic_cast<onity_propnoderender*>(nodedata->get_prop());
			rootprop->get_node()->DelChild(nodeprop->get_node());
			root->del_leafnode(node);
		}
	}
}
xui_method_explain(onity_propctrl_rendertree, on_deleterenderself,	void			)( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt		= sender->get_renderrtabs();
	xui_colour		color   = sender->get_rendercolor() * sender->get_vertexcolor();
	xui_vector<s32> center	= xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);

	xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
		center.x-4,
		center.y-1,
		center.x+4,
		center.y+1), color);
}
xui_method_explain(onity_propctrl_rendertree, on_insertrenderself,	void			)( xui_component* sender, xui_method_args& args )
{
	xui_rect2d<s32> rt		= sender->get_renderrtabs();
	xui_colour		color   = sender->get_rendercolor() * sender->get_vertexcolor();
	xui_vector<s32> center	= xui_vector<s32>(rt.ax+rt.get_w()/2, rt.ay+rt.get_h()/2);

	xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
		center.x-4,
		center.y-1,
		center.x+4,
		center.y+1), color);
	xui_convas::get_ins()->fill_rectangle(xui_rect2d<s32>(
		center.x-1,
		center.y-4,
		center.x+1,
		center.y+4), color);
}
xui_method_explain(onity_propctrl_rendertree, on_menuclick,			void			)( xui_component* sender, xui_method_args& args )
{
	NPNode* rendernode = NULL;
	if		(sender == m_module)	rendernode = new NP2DSImageRef;
	else if (sender == m_sprite)	rendernode = new NP2DSFrameRef;
	else if (sender == m_action)	rendernode = new NP2DSActorRef;
	else if (sender == m_particle)	rendernode = new NPParticleSFX;
	else							rendernode = new NP2DSTransRef;

	xui_treenode* root = m_middle->get_upmostnode(0);
	std::vector<xui_treenode*> nodes = m_middle->get_selectednode();
	if (nodes.size() > 0)
		root = nodes.front();

	onity_treedata*		  rootdata = (onity_treedata*)root->get_linkdata();
	onity_propnoderender* rootprop = dynamic_cast<onity_propnoderender*>(rootdata->get_prop());
	rootprop->get_node()->AddChild(rendernode);
	add_rendernode(root, rendernode);
}

/*
//method
*/
xui_method_explain(onity_propctrl_rendertree, add_rendernode,		void			)( xui_treenode* root, NPNode* rendernode )
{
	xui_treenode* node = NULL;
	xui_proproot* prop = NULL;
	if		(NPIsExaKindOf(NPParticleSFX, rendernode))	prop = new onity_propnodeparticle	(NPDynamicCast(NPParticleSFX, rendernode));
	else if (NPIsExaKindOf(NP2DSImageRef, rendernode))	prop = new onity_propnodemodule		(NPDynamicCast(NP2DSTransRef, rendernode));
	else if (NPIsExaKindOf(NP2DSFrameRef, rendernode))	prop = new onity_propnodesprite		(NPDynamicCast(NP2DSTransRef, rendernode));
	else if (NPIsExaKindOf(NP2DSActorRef, rendernode))	prop = new onity_propnodeaction		(NPDynamicCast(NP2DSTransRef, rendernode));
	else if (NPIsExaKindOf(NP2DSTransRef, rendernode))	prop = new onity_propnode2dsref		(NPDynamicCast(NP2DSTransRef, rendernode));
	else												prop = new onity_propnoderender		(rendernode);
	if (root == NULL)
	{
		node = m_middle->add_upmostnode(0, new onity_renderdata(prop));
	}
	else
	{
		node = root->add_leafnode(root->get_leafnodecount(), new onity_renderdata(prop));
	}

	std::list<NPRenderObject*> children = rendernode->GetChildren();
	for (std::list<NPRenderObject*>::iterator itor = children.begin(); itor != children.end(); ++itor)
	{
		add_rendernode(node, NPDynamicCast(NPNode, (*itor)));
	}
}
xui_method_explain(onity_propctrl_rendertree, get_renderprop,		xui_proproot_vec)( void )
{
	xui_proproot_vec props;
	std::vector<xui_treenode*> nodes = m_middle->get_entirenode();
	for (u32 i = 0; i < nodes.size(); ++i)
	{
		onity_treedata* data = (onity_treedata*)nodes[i]->get_linkdata();
		props.push_back(data->get_prop());
	}

	return props;
}