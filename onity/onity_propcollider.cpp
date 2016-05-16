#include "NP2DSCollider.h"
#include "NP2DSAssetFile.h"

#include "xui_global.h"
#include "xui_textbox.h"
#include "xui_kindctrl.h"
#include "xui_propkind.h"
#include "xui_propctrl_rect2d.h"
#include "onity_mainform.h"
#include "onity_inspector.h"
#include "onity_preview.h"
#include "onity_propasset.h"
#include "onity_propcollider.h"

/*
//constructor
*/
xui_create_explain(onity_propcollider)( onity_propfile* propfile, NP2DSCollider* collider )
: onity_propasset(propfile)
, m_collider(collider)
{
	m_basekind = new xui_propkind(this, xui_global::ascii_to_unicode(collider->GetName()), "Collider", xui_kindctrl::create, NULL, true);
	m_basekind->xm_namechanged += new xui_method_member<xui_method_args,     onity_propcollider>(this, &onity_propcollider::on_namechanged);
	m_basekind->xm_propchanged += new xui_method_member<xui_method_propdata, onity_propcollider>(this, &onity_propcollider::on_propchanged);

	m_basekind->add_propdata(new xui_propdata_number_func(
		m_basekind,
		L"Type",
		xui_propctrl_number::create,
		get_type,
		set_type,
		this,
		NT_UNSIGNEDINT));
	m_basekind->add_propdata(new xui_propdata_string(
		m_basekind,
		L"User Text",
		xui_propctrl_string::create,
		get_user,
		set_user,
		this));
	m_basekind->add_propdata(new xui_propdata_rect2d(
		m_basekind,
		L"Rectangle",
		xui_propctrl_rect2d::create,
		get_rect,
		set_rect,
		this,
		NT_INT));

	add_propkind(m_basekind);
}

/*
//method
*/
xui_method_explain(onity_propcollider, get_collider,	NP2DSCollider*	)( void )
{
	return m_collider;
}

/*
//override
*/
xui_method_explain(onity_propcollider, on_attach,		void			)( void )
{
	set_drawrect();
}

/*
//event
*/
xui_method_explain(onity_propcollider, on_namechanged,	void			)( xui_component* sender, xui_method_args&     args )
{
	xui_textbox* textbox = xui_dynamic_cast(xui_textbox, sender);
	std::wstring text = textbox->get_text();
	std::string  name = xui_global::unicode_to_ascii(text);
	m_basekind->set_name(text);

	NP2DSCollider* collider = get_collider();
	if (collider->GetName() != name)
	{
		collider->SetName(name);
		collider->GetOwnedFile()->SetNeedSave(true);
	}
}
xui_method_explain(onity_propcollider, on_propchanged,	void			)( xui_component* sender, xui_method_propdata& args )
{
	if (args.propdata->get_name() == L"Rectangle")
	{
		set_drawrect();
	}
}

/*
//method
*/
xui_method_explain(onity_propcollider, set_drawrect,	void			)( void )
{
	NPRect rect = m_collider->GetRect();
	onity_preview* preview = onity_mainform::get_ptr()->get_inspector()->get_preview();
	preview->set_drawrect(xui_rect2d<s32>(rect.LT, rect.TP, rect.RT, rect.BM));
}

/*
//static
*/
xui_method_explain(onity_propcollider, get_type,		f64				)( void* userptr )
{
	onity_propcollider* propcollider = (onity_propcollider*)userptr;
	NP2DSCollider* collider = propcollider->get_collider();
	return (f64)collider->GetType();
}
xui_method_explain(onity_propcollider, set_type,		void			)( void* userptr, f64 value )
{
	onity_propcollider* propcollider = (onity_propcollider*)userptr;
	NP2DSCollider* collider = propcollider->get_collider();
	collider->SetType((npu08)value);
}
xui_method_explain(onity_propcollider, get_user,		std::wstring	)( void* userptr )
{
	onity_propcollider* propcollider = (onity_propcollider*)userptr;
	NP2DSCollider* collider = propcollider->get_collider();
	return xui_global::ascii_to_unicode(collider->GetUser());
}
xui_method_explain(onity_propcollider, set_user,		void			)( void* userptr, const std::wstring&   value )
{
	onity_propcollider* propcollider = (onity_propcollider*)userptr;
	NP2DSCollider* collider = propcollider->get_collider();
	collider->SetUser(xui_global::unicode_to_ascii(value));
}
xui_method_explain(onity_propcollider, get_rect,		xui_rect2d<f64>	)( void* userptr )
{
	onity_propcollider* propcollider = (onity_propcollider*)userptr;
	NP2DSCollider* collider = propcollider->get_collider();
	NPRect rect = collider->GetRect();
	return xui_rect2d<f64>((f64)rect.LT, (f64)rect.TP, (f64)rect.RT, (f64)rect.BM);
}
xui_method_explain(onity_propcollider, set_rect,		void			)( void* userptr, const xui_rect2d<f64>& value )
{
	onity_propcollider* propcollider = (onity_propcollider*)userptr;
	NP2DSCollider* collider = propcollider->get_collider();
	NPRect rect;
	rect.LT = (nps32)value.ax;
	rect.TP = (nps32)value.ay;
	rect.RT = (nps32)value.bx;
	rect.BM = (nps32)value.by;
	collider->SetRect(rect);
}