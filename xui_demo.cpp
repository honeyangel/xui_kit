#include "xui_demo.h"

void xui_demo::test_button( xui_window* window )
{
	xui_button* button = NULL;
	button = xui_button::create(xui_bitmap::create("icon/edit.png"));
	button->set_renderpt(xui_vector<s32>(10, 10));
	window->add_child(button);
	button = xui_button::create(xui_bitmap::create("icon/edit.png"), L"Translate", 110);
	button->set_renderpt(xui_vector<s32>(50, 10));
	window->add_child(button);
}

void xui_demo::test_toggle( xui_window* window )
{
	xui_toggle* toggle = NULL;
	toggle = xui_toggle::create(xui_bitmap::create("icon/edit.png"));
	toggle->set_renderpt(xui_vector<s32>(10,  40));
	window->add_child(toggle);
	toggle = xui_toggle::create(xui_bitmap::create("icon/edit.png"), L"Translate", 110);
	toggle->set_renderpt(xui_vector<s32>(50,  40));
	window->add_child(toggle);
	toggle = xui_toggle::create();
	toggle->set_renderpt(xui_vector<s32>(170, 44));
	window->add_child(toggle);
	toggle = xui_toggle::circle();
	toggle->set_renderpt(xui_vector<s32>(190, 44));
	window->add_child(toggle);
}

void xui_demo::test_textbox( xui_window* window )
{
	xui_textbox* textbox = NULL;
	textbox = xui_textbox::create(80);
	textbox->set_renderpt(xui_vector<s32>(10, 70));
	textbox->set_hinttext(L"username");
	window->add_child(textbox);
	textbox = xui_textbox::create(80);
	textbox->set_renderpt(xui_vector<s32>(100, 70));
	textbox->set_password(true);
	textbox->set_hinttext(L"password");
	window->add_child(textbox);
}
void xui_demo::test_linebox( xui_window* window )
{
	xui_linebox* linebox = xui_linebox::create();
	linebox->set_renderpt(xui_vector<s32>(10, 100));
	window->add_child(linebox);

	linebox->add_linectrl(xui_button::create(xui_bitmap::create("icon/edit.png"), L"Translate", 110));
	linebox->add_linectrl(xui_textbox::create(80));
	xui_toggle* toggle = xui_toggle::create(xui_bitmap::create("icon/edit.png"));
	toggle->set_downcolor(xui_colour::red);
	toggle->ini_toggle(true);
	linebox->add_linectrl(toggle);
}
void xui_demo::test_gridbox( xui_window* window )
{
	xui_gridbox* gridbox = xui_gridbox::create(4, 4, 80);
	gridbox->set_renderpt(xui_vector<s32>(10, 150));
	window->add_child(gridbox);

	gridbox->set_gridctrl(0, 1, xui_drawer::create(xui_bitmap::create("icon/edit.png")));
	gridbox->set_gridctrl(0, 2, xui_drawer::create(xui_bitmap::create("icon/edit.png")));
	gridbox->set_gridctrl(0, 3, xui_drawer::create(xui_bitmap::create("icon/edit.png")));
	gridbox->set_gridctrl(1, 0, xui_drawer::create(L"Bones"));
	gridbox->set_gridctrl(2, 0, xui_drawer::create(L"Images"));
	gridbox->set_gridctrl(3, 0, xui_drawer::create(L"Bounds"));
	for (u32 irow = 1; irow < 4; ++irow)
	{
		for (u32 icol = 1; icol < 4; ++icol)
		{
			xui_toggle* toggle = xui_toggle::circle();
			toggle->ini_toggle((irow+icol)%2 == 0);
			gridbox->set_gridctrl(irow, icol, toggle);
		}
	}
}
void xui_demo::test_toolbar( xui_window* window )
{
	xui_toolbar* toolbar = xui_toolbar::create();
	toolbar->set_renderpt(xui_vector<s32>(10, 260));
	window->add_child(toolbar);

	toolbar->add_item(xui_button::create(xui_bitmap::create("icon/edit.png")));
	toolbar->add_item(xui_button::create(xui_bitmap::create("icon/edit.png")));
	toolbar->add_separate();
	toolbar->add_item(xui_toggle::create(xui_bitmap::create("icon/edit.png")));
	toolbar->add_separate();

	xui_dropbox* dropbox= xui_dropbox::create(120, xui_bitmap::create("icon/edit.png"));
	dropbox->add_item(L"ImageL");
	dropbox->add_item(L"ImageR");
	dropbox->add_item(L"ImageT");
	dropbox->add_item(L"ImageB");
	dropbox->add_item(L"ImageC");
	dropbox->ini_dropbox(0);

	toolbar->add_item(dropbox);
}
void xui_demo::test_scroll( xui_window* window )
{
	xui_scroll* scroll = NULL;
	scroll = xui_scroll::create(FLOWSTYLE_H);
	scroll->set_renderpt(xui_vector<s32>(10, 300));
	window->add_child(scroll);
	scroll = xui_scroll::create(FLOWSTYLE_V);
	scroll->set_renderpt(xui_vector<s32>(120, 300));
	window->add_child(scroll);
}
void xui_demo::test_slider( xui_window* window )
{
	xui_slider* slider = NULL;
	slider = xui_slider::create(FLOWSTYLE_H);
	slider->set_renderpt(xui_vector<s32>(10, 330));
	window->add_child(slider);
	slider = xui_slider::create(FLOWSTYLE_V);
	slider->set_renderpt(xui_vector<s32>(150, 300));
	window->add_child(slider);
}
void xui_demo::test_listview( xui_window* window )
{
	xui_listview* listview = xui_listview::create(xui_vector<s32>(200, 100), false);
	listview->set_renderpt(xui_vector<s32>(250, 10));
	window->add_child(listview);

	listview->add_item(L"Window", xui_bitmap::create("icon/edit.png"));
	listview->add_item(L"Panel");
	listview->add_item(L"Button");
	listview->add_item(L"TextBox", xui_bitmap::create("icon/edit.png"));
	listview->add_item(L"ListView");
	listview->add_item(L"Menu", xui_bitmap::create("icon/edit.png"));
}
void xui_demo::test_dropbox( xui_window* window )
{
	xui_dropbox* dropbox = NULL;
	dropbox= xui_dropbox::create(120);
	dropbox->set_renderpt(xui_vector<s32>(250, 120));
	window->add_child(dropbox);

	dropbox->add_item(L"DockLeft");
	dropbox->add_item(L"DockRight");
	dropbox->add_item(L"DockTop");
	dropbox->add_item(L"DockBottom");
	dropbox->add_item(L"DockFill");
	dropbox->ini_dropbox(0);

	dropbox= xui_dropbox::create(120, xui_bitmap::create("icon/edit.png"));
	dropbox->set_renderpt(xui_vector<s32>(250, 150));
	window->add_child(dropbox);

	dropbox->add_item(L"ImageL");
	dropbox->add_item(L"ImageR");
	dropbox->add_item(L"ImageT");
	dropbox->add_item(L"ImageB");
	dropbox->add_item(L"ImageC");
	dropbox->ini_dropbox(0);
}

//test treenode drag
void treeview_dragitem(xui_component* sender, xui_method_dragdrop& args )
{
	xui_treeview* treeview = (xui_treeview*)sender;
	std::vector<xui_treenode*> selectednode = treeview->get_selectednode();
	if (selectednode.size() > 0)
	{
		args.type = "xui_treenode";
		args.data = selectednode.front();
	}
}

void xui_demo::test_treeview( xui_window* window )
{
	std::vector<xui_treecolumn> columninfo;
	columninfo.push_back(xui_treecolumn(TREECOLUMN_BOOL,  24, L"", xui_bitmap::create("icon/edit.png"), 0, false, TOGGLE_CIRCLE));
	columninfo.push_back(xui_treecolumn(TREECOLUMN_TEXT,  60, L"desc", NULL, 0, false, 0, xui_rect2d<s32>(2), IMAGE_FRONT_TEXT));
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 100, L"main", NULL, 0, true));
	xui_treeview* treeview = xui_treeview::create(columninfo);
	treeview->set_renderpt(xui_vector<s32>(250, 180));
	treeview->xm_mousedragitem += new xui_method_static<xui_method_dragdrop>(treeview_dragitem);
	window->add_child(treeview);

	for (int i = 0; i < 10; ++i)
	{
		std::wstringstream text;
		text << L"test";
		text << i+5;
		xui_treedata* data = new xui_treedata(text.str(), xui_bitmap::create("icon/edit.png"));
		xui_treenode* node = treeview->add_upmostnode(i, data);

		if (i % 2 == 0)
		{
			for (int j = 0; j < 3; ++j)
			{
				std::wstringstream childtext;
				childtext << L"child";
				childtext << j;

				xui_treedata* childdata = new xui_treedata(childtext.str());
				xui_treenode* childnode = node->add_leafnode(j, childdata);
			}
		}
	}
}
void xui_demo::test_timeview( xui_window* window )
{
	std::vector<xui_treecolumn> columninfo;
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 250, L"Total", NULL, 0, false));
	columninfo.push_back(xui_treecolumn(TREECOLUMN_BOOL,  24, L"",      xui_bitmap::create("icon/edit.png"), 0, false, TOGGLE_NORMAL));
	columninfo.push_back(xui_treecolumn(TREECOLUMN_BOOL,  24, L"",      xui_bitmap::create("icon/edit.png"), 0, false, TOGGLE_NORMAL));
	xui_timeview* timeview = xui_timeview::create(960, columninfo);
	timeview->set_renderpt(xui_vector<s32>(10, 420));
	window->add_child(timeview);

	std::map<s32, u08> keyframe;
	keyframe[ 0] = 0;
	keyframe[ 5] = 0;
	keyframe[ 8] = 1;
	keyframe[15] = 1;
	keyframe[24] = 2;
	keyframe[35] = 1;
	keyframe[50] = 1;
	keyframe[80] = 2;
	for (int i = 0; i < 4; ++i)
	{
		std::wstringstream text;
		text << L"test";
		text << i;
		xui_timedata* data = new xui_timedata(text.str(), xui_bitmap::create("icon/edit.png"), keyframe);
		xui_timeline* line = timeview->add_timeline(i, data);

		if (i % 2 == 0)
		{
			for (int j = 0; j < 3; ++j)
			{
				std::wstringstream childtext;
				childtext << L"child";
				childtext << j;

				xui_timedata* childdata = new xui_timedata(childtext.str(), xui_bitmap::create("icon/edit.png"), keyframe);
				xui_timeline* childline = line->add_timeline(j, childdata);
			}
		}
	}
}

//test propview prop
s32  prop_value = 0;
bool bool_value = false;
u08  enum_value = TA_LB;
std::wstring string_value = L"abc";
xui_propenum_map enum_map;
xui_vector<s32> vector_value(0);
xui_rect2d<s32> rect2d_value(0);
std::vector<xui_treenode*> stdvec_value;
xui_treenode* object_value = NULL;

//test pickwnd
class test_pickwnd : public xui_pickwnd
{
public:
	static xui_pickwnd* create( xui_propctrl* propctrl )
	{
		return new test_pickwnd(propctrl);
	}

	test_pickwnd(xui_propctrl* propctrl)
	: xui_pickwnd(propctrl)
	{
		m_render	= xui_rect2d<s32>(0, 0, 280, 160);
		m_sidestyle	= SIDESTYLE_S;
		m_alignhorz = ALIGNHORZ_C;
		m_alignvert = ALIGNVERT_C;

		xui_button* button = xui_button::create(NULL, L"Accept", 100);
		button->ini_component(ALIGNHORZ_C, ALIGNVERT_C, 0);
		button->xm_click += new xui_method_member<xui_method_args, test_pickwnd>(this, &test_pickwnd::on_accept);
		add_child(button);
	}

	virtual void* get_value( void )
	{
		if (stdvec_value.size() > 0)
			return (void*)stdvec_value.back();

		return NULL;
	}
	virtual void  set_value( void* value )
	{

	}
};

//test propview func
xui_bitmap* treenode_geticon( xui_propdata* propdata)
{
	return xui_bitmap::create("icon/edit.png");
}
std::wstring treenode_getname(xui_propdata* propdata)
{
	xui_propdata_object* dataobject = (xui_propdata_object*)propdata;
	xui_treenode* node = (xui_treenode*)dataobject->get_value();
	return node->get_linkdata()->get_text(2);
}
void kind_flagchanged( xui_component* sender, xui_method_args& args )
{

}
void kind_textchanged( xui_component* sender, xui_method_args& args )
{

}
xui_propdata* stdvec_newprop(void* ptr, xui_propkind* kind)
{
	return new xui_propdata_object_impl<xui_treenode*>(kind, L"", xui_propctrl_object::create, "xui_treenode", test_pickwnd::create, treenode_geticon, treenode_getname, (xui_treenode**)ptr);
}

void xui_demo::test_propview( xui_window* window )
{
	xui_propview* propview = xui_propview::create();
	propview->set_renderpt(xui_vector<s32>(500, 10));
	window->add_child(propview);

	xui_proproot* proproot = new xui_proproot();
	xui_propkind* headkind = new xui_propkind(proproot, L"Transform", xui_kindctrl::create, xui_bitmap::create("icon/edit.png"), true);
	headkind->xm_flagchanged += new xui_method_static<xui_method_args>(kind_flagchanged);
	//headkind->xm_namechanged += new xui_method_static<xui_method_args>(kind_textchanged);
	headkind->add_propdata(new xui_propdata_vector_impl<s32>(headkind, L"P", xui_propctrl_vector_button::create, &vector_value, 1));
	headkind->add_propdata(new xui_propdata_vector_impl<s32>(headkind, L"S", xui_propctrl_vector_button::create, &vector_value, 1, xui_vector<s32>(1)));
	headkind->add_propdata(new xui_propdata_vector_impl<s32>(headkind, L"R", xui_propctrl_vector_button::create, &vector_value, 1));
	headkind->add_propdata(new xui_propdata_rect2d_impl<s32>(headkind, L"rect2d", xui_propctrl_rect2d::create, &rect2d_value, 1));
	headkind->add_propdata(new xui_propdata_stdvec_impl<xui_treenode*>(headkind, L"stdvec", xui_propctrl_stdvec::create, NULL, NULL, stdvec_newprop, &stdvec_value));
	headkind->add_propdata(new xui_propdata_object_impl<xui_treenode*>(headkind, L"object", xui_propctrl_object::create, "xui_treenode", test_pickwnd::create, treenode_geticon, treenode_getname, &object_value));
	proproot->add_propkind(headkind);

	xui_propkind* propkind = new xui_propkind(proproot, L"test", xui_kindctrl::create, xui_bitmap::create("icon/edit.png"), true);
	proproot->add_propkind(propkind);
	xui_propdata* propdata = new xui_propdata_number_impl<s32>(propkind, L"s32", xui_propctrl_number::create, &prop_value);
	propkind->add_propdata(propdata);
	propkind->add_propdata(new xui_propdata_bool(propkind, L"bool", xui_propctrl_bool::create, &bool_value));
	propkind->add_propdata(new xui_propdata_number_impl<s32>(propkind, L"s32", xui_propctrl_slider::create, &prop_value, 2, -50, 50));
	enum_map[TA_LT] = L"LeftTop";
	enum_map[TA_LC] = L"LeftCenter";
	enum_map[TA_LB] = L"LeftBottom";
	propkind->add_propdata(new xui_propdata_enum_impl<u08>(propkind, L"enum", xui_propctrl_enum::create, enum_map, &enum_value));
	propkind->add_propdata(new xui_propdata_string(propkind, L"string", xui_propctrl_string::create, &string_value));

	xui_propdata_vec subprop;
	subprop.push_back(new xui_propdata_bool(propkind, L"sub_bool", xui_propctrl_bool::create, &bool_value));
	subprop.push_back(new xui_propdata_number_impl<s32>(propkind, L"sub_s32", xui_propctrl_slider::create, &prop_value, 2, -50, 50));
	subprop.push_back(new xui_propdata_enum_impl<u08>(propkind, L"sub_enum", xui_propctrl_enum::create, enum_map, &enum_value));
	subprop.push_back(new xui_propdata_string(propkind, L"sub_string", xui_propctrl_string::create, &string_value));
	std::map<u08, std::vector<u32>> showmap;
	std::map<u08, std::vector<u32>> editmap;
	showmap[TA_LT].push_back(0);
	showmap[TA_LT].push_back(1);
	showmap[TA_LC].push_back(2);
	showmap[TA_LB].push_back(3);
	xui_propdata_expand_enum<u08>* dataexpand = new xui_propdata_expand_enum<u08>(propkind, L"expand", xui_propctrl_expand_enum::create, enum_map, &enum_value, subprop, true, showmap, editmap);
	propkind->add_propdata(dataexpand);

	propview->set_proproot(proproot);
}