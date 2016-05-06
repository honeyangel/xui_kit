#include "NP2DSImageRef.h"
#include "onity_propimageref.h"

/*
//constructor
*/
xui_create_explain(onity_propimageref)( onity_propfile* propfile, NP2DSTransRef* transref )
: onity_proptransref(propfile, transref)
{
	//TODO
}

/*
//method
*/
xui_method_explain(onity_propimageref, get_imageref,	NP2DSImageRef*	)( void )
{
	return NPDynamicCast(NP2DSImageRef, m_transref);
}

/*
//static
*/
xui_method_explain(onity_propimageref, get_image,		void*			)( void* userptr )
{
	return NULL;
}
xui_method_explain(onity_propimageref, set_image,		void			)( void* userptr, void* value )
{

}