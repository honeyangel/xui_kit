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
void xui_demo::test_treeview( xui_window* window )
{
	std::vector<xui_treecolumn> columninfo;
	columninfo.push_back(xui_treecolumn(TREECOLUMN_BOOL,  20, L"", xui_bitmap::create("icon/edit.png")));
	columninfo.push_back(xui_treecolumn(TREECOLUMN_TEXT,  60, L"desc"));
	columninfo.push_back(xui_treecolumn(TREECOLUMN_MAIN, 160, L"main"));
	xui_treeview* treeview = xui_treeview::create(columninfo);
	treeview->set_renderpt(xui_vector<s32>(250, 180));
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