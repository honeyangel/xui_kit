#include "NP2DSImageRef.h"
#include "NP2DSCollider.h"
#include "NP2DSFrame.h"

#include "xui_textbox.h"
#include "xui_kindctrl.h"
#include "xui_propkind.h"
#include "xui_propctrl_rect2d.h"
#include "xui_propctrl_expand.h"
#include "xui_propctrl_stdvec.h"
#include "onity_savekind.h"
#include "onity_propctrl_transref.h"
#include "onity_propcollider.h"
#include "onity_propimageref.h"
#include "onity_propframe.h"

/*
//constructor
*/
xui_create_explain(onity_propframe)( onity_propfile* propfile, u32 id )
: onity_prop2dsasset(propfile, id)
{
	NP2DSFrame* frame = (NP2DSFrame*)get_asset();
	for (npu16 i = 0; i < frame->GetImageRefCount(); ++i)
		m_propimageref.push_back(new onity_propimageref(propfile, frame->GetImageRef(i)));
	for (npu16 i = 0; i < frame->GetColliderCount(); ++i)
		m_propcollider.push_back(new onity_propcollider(propfile, frame->GetCollider(i)));

	m_framekind = new xui_propkind(this, L"", "2DSFrame", xui_kindctrl::create, NULL, true, false);
	std::vector<xui_propdata*>       subprop;
	std::map<s32, std::vector<u32> > editmap;
	editmap[1].push_back(0);
	subprop.push_back(new xui_propdata_rect2d(
		m_framekind,
		L"Bounding Box",
		xui_propctrl_rect2d::create,
		get_boundingbox,
		set_boundingbox,
		this,
		NT_UNSIGNEDINT));

	m_framekind->add_propdata(new xui_propdata_expand_bool(
		m_framekind,
		L"Bounding Custom",
		xui_propctrl_expand_bool::create,
		subprop,
		was_boundcustom,
		set_boundcustom,
		this, 
		true,
		xui_expandvary::empty_map,
		editmap));

	m_framekind->add_propdata(new xui_propdata_stdvec_func<NP2DSImageRef*>(
		m_framekind,
		L"Imagerefs",
		xui_propctrl_stdvec::create,
		add_imageref,
		del_imageref,
		new_propdata,
		get_imagerefs,
		this));
	//m_framekind->add_propdata(new xui_propdata_stdvec_root(
	//	m_framekind,
	//	L"ImageRefs",
	//	xui_propctrl_stdvec_root::create,
	//	add_imageref, 
	//	del_imageref,
	//	get_imagerefs,
	//	this));

	m_framekind->add_propdata(new xui_propdata_stdvec_root(
		m_framekind,
		L"Colliders",
		xui_propctrl_stdvec_root::create,
		add_collider, 
		del_collider,
		get_colliders,
		this));

	add_propkind(m_framekind);
}

/*
//destructor
*/
xui_delete_explain(onity_propframe)( void )
{
	for (u32 i = 0; i < m_propimageref.size(); ++i)
		delete m_propimageref[i];
	for (u32 i = 0; i < m_propcollider.size(); ++i)
		delete m_propcollider[i];
}

/*
//method
*/
xui_method_explain(onity_propframe, get_frame,			NP2DSFrame*				)( void )
{
	return NPDynamicCast(NP2DSFrame, get_asset());
}
xui_method_explain(onity_propframe, get_imagerefs,		const xui_proproot_vec&	)( void ) const
{
	return m_propimageref;
}
xui_method_explain(onity_propframe, get_colliders,		const xui_proproot_vec&	)( void ) const
{
	return m_propcollider;
}
xui_method_explain(onity_propframe, del_imageref,		void					)( NP2DSImageRef* imageref )
{
	for (u32 i = 0; i < m_propimageref.size(); ++i)
	{
		onity_propimageref* propimageref = dynamic_cast<onity_propimageref*>(m_propimageref[i]);
		if (propimageref->get_imageref() == imageref)
		{
			delete propimageref;
			m_propimageref.erase(m_propimageref.begin()+i);
			break;
		}
	}

	NP2DSFrame* frame = get_frame();
	frame->DelImageRef(imageref);
}
xui_method_explain(onity_propframe, del_collider,		void					)( NP2DSCollider* collider )
{
	for (u32 i = 0; i < m_propcollider.size(); ++i)
	{
		onity_propcollider* propcollider = dynamic_cast<onity_propcollider*>(m_propcollider[i]);
		if (propcollider->get_collider() == collider)
		{
			delete propcollider;
			m_propcollider.erase(m_propcollider.begin()+i);
			break;
		}
	}

	NP2DSFrame* frame = get_frame();
	frame->DelCollider(collider);
}
xui_method_explain(onity_propframe, add_imageref,		void					)( void )
{
	NP2DSFrame* frame = get_frame();
	NP2DSImageRef* imageref = frame->AddImageRef();
	m_propimageref.push_back(new onity_propimageref(m_savekind->get_propfile(), imageref));
}
xui_method_explain(onity_propframe, add_collider,		void					)( void )
{
	NP2DSFrame* frame = get_frame();
	NP2DSCollider* collider = frame->AddCollider();
	collider->SetName("Collider");
	m_propcollider.push_back(new onity_propcollider(m_savekind->get_propfile(), collider));
}

/*
//static
*/
xui_method_explain(onity_propframe, was_boundcustom,	bool					)( void* userptr )
{
	onity_propframe* propframe = (onity_propframe*)userptr;
	NP2DSFrame* frame = propframe->get_frame();
	return frame->WasBoundCustom();
}
xui_method_explain(onity_propframe, set_boundcustom,	void					)( void* userptr, bool value )
{
	onity_propframe* propframe = (onity_propframe*)userptr;
	NP2DSFrame* frame = propframe->get_frame();
	frame->SetBoundCustom(value);
}
xui_method_explain(onity_propframe, get_boundingbox,	xui_rect2d<f64>			)( void* userptr )
{
	onity_propframe* propframe = (onity_propframe*)userptr;
	NP2DSFrame* frame = propframe->get_frame();
	NPRect rect = frame->GetBounding();
	return xui_rect2d<f64>((f64)rect.LT, (f64)rect.TP, (f64)rect.RT, (f64)rect.BM);
}
xui_method_explain(onity_propframe, set_boundingbox,	void					)( void* userptr, const xui_rect2d<f64>& value )
{
	onity_propframe* propframe = (onity_propframe*)userptr;
	NP2DSFrame* frame = propframe->get_frame();
	NPRect rect;
	rect.LT = (nps32)value.ax;
	rect.TP = (nps32)value.ay;
	rect.RT = (nps32)value.bx;
	rect.BM = (nps32)value.by;
	frame->SetBounding(rect);
}
xui_method_explain(onity_propframe, get_imagerefs,		NP2DSFrame::ImageRefVec&)( void* userptr )
{
	onity_propframe* propframe = (onity_propframe*)userptr;
	return propframe->get_frame()->GetImageRefVec();
}
xui_method_explain(onity_propframe, get_proptotal,		xui_proproot_vec		)( void* userptr )
{
	onity_propframe* propframe = (onity_propframe*)userptr;
	return propframe->get_imagerefs();
}
xui_method_explain(onity_propframe, get_colliders,		xui_proproot_vec		)( void* userptr )
{
	onity_propframe* propframe = (onity_propframe*)userptr;
	return propframe->get_colliders();
}
xui_method_explain(onity_propframe, add_imageref,		void					)( void* userptr )
{
	onity_propframe* propframe = (onity_propframe*)userptr;
	propframe->add_imageref();
}
xui_method_explain(onity_propframe, add_collider,		void					)( void* userptr )
{
	onity_propframe* propframe = (onity_propframe*)userptr;
	propframe->add_collider();
}
xui_method_explain(onity_propframe, del_imageref,		void					)( void* userptr )
{
	onity_propframe* propframe = (onity_propframe*)userptr;
	onity_propimageref* propimageref = (onity_propimageref*)propframe->get_imagerefs().back();
	propframe->del_imageref(propimageref->get_imageref());
}
xui_method_explain(onity_propframe, del_collider,		void					)( void* userptr, xui_proproot* root )
{
	onity_propframe* propframe = (onity_propframe*)userptr;
	onity_propcollider* propcollider = dynamic_cast<onity_propcollider*>(root);
	propframe->del_collider(propcollider->get_collider());
}
xui_method_explain(onity_propframe, new_propdata,		xui_propdata*			)( void* userptr, u32 i, xui_propkind* propkind )
{
	onity_propframe* propframe = (onity_propframe*)userptr;
	xui_proproot* prop = propframe->get_imagerefs()[i];
	return new onity_propdata_transref(propkind, L"Imageref", onity_propctrl_transref::create, DROPTYPE_IMAGE, dynamic_cast<onity_proptransref*>(prop), onity_propframe::get_proptotal, userptr);
}