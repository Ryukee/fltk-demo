#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Text_Buffer.H>

#include "CB.h"
#include "EditorWindow.h"

#define TS 14	// default editor textsize

extern char filename[256];
extern Fl_Text_Buffer* textbuf;
extern char title[256];
extern int num_windows;
// Fl_Text_Buffer* stylebuf = 0;  // for Syntax Highlighting

Fl_Menu_Item menuitems[] = {
	{ "&File", 0, 0, 0, FL_SUBMENU },
	{ "&New File", 0, (Fl_Callback*)new_cb },
	{ "&Open File...", FL_COMMAND + 'o', (Fl_Callback*)open_cb },
	//{ "&Insert File...", FL_COMMAND + 'i', (Fl_Callback*)insert_cb, 0, FL_MENU_DIVIDER },
	{ "&Save File", FL_COMMAND + 's', (Fl_Callback*)save_cb },
	{ "Save File &As...", FL_COMMAND + FL_SHIFT + 's', (Fl_Callback*)saveas_cb, 0, FL_MENU_DIVIDER },
	//{ "New &View", FL_ALT + 'v', (Fl_Callback*)view_cb, 0 },
	//{ "&Close View", FL_COMMAND + 'w', (Fl_Callback*)close_cb, 0, FL_MENU_DIVIDER },
	{ "E&xit", FL_COMMAND + 'q', (Fl_Callback*)quit_cb, 0 },
	{ 0 },
	{ "&Edit", 0, 0, 0, FL_SUBMENU },
	//{ "&Undo", FL_COMMAND + 'z', (Fl_Callback*)undo_cb, 0, FL_MENU_DIVIDER },
	{ "Cu&t", FL_COMMAND + 'x', (Fl_Callback*)cut_cb },
	{ "&Copy", FL_COMMAND + 'c', (Fl_Callback*)copy_cb },
	{ "&Paste", FL_COMMAND + 'v', (Fl_Callback*)paste_cb },
	{ "&Delete", 0, (Fl_Callback*)delete_cb },
	{ 0 },
	{ "&Search", 0, 0, 0, FL_SUBMENU },
	{ "&Find...", FL_COMMAND + 'f', (Fl_Callback*)find_cb },
	{ "F&ind Again", FL_COMMAND + 'g', find2_cb },
	{ "&Replace...", FL_COMMAND + 'r', replace_cb },
	{ "Re&place Again", FL_COMMAND + 't', replace2_cb },
	{ 0 },
	{ 0 }
};

Fl_Window* new_view() {
	EditorWindow* w = new EditorWindow(660, 400, title);
	w->begin();
	Fl_Menu_Bar* m = new Fl_Menu_Bar(0, 0, 660, 30);
	m->copy(menuitems, w);
	w->editor = new Fl_Text_Editor(0, 30, 660, 370);
	w->editor->textfont(FL_COURIER);
	w->editor->textsize(TS);
	//w->editor->wrap_mode(Fl_Text_Editor::WRAP_AT_BOUNDS, 250);
	w->editor->buffer(textbuf);

	//w->editor->highlight_data(stylebuf, styletable,
	//	sizeof(styletable) / sizeof(styletable[0]),
	//	'a', style_unfinished_cb, 0);

	w->end();
	w->resizable(w->editor);
	w->size_range(300, 200);
	w->callback((Fl_Callback*)close_cb, w);

	//textbuf->add_modify_callback(style_update, w->editor);
	textbuf->add_modify_callback(changed_cb, w);
	textbuf->call_modify_callbacks();
	num_windows++;
	return w;
}

int main(int argc, char** argv) {
	textbuf = new Fl_Text_Buffer;
	Fl_Window* window = new_view();
	window->show(1, argv);
	if (argc > 1) 
		load_file(argv[1], -1);
	return Fl::run();
}