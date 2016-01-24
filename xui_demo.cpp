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
	toolbar->add_item(xui_textbox::create(80));
}