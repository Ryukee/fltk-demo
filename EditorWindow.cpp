#include "EditorWindow.h"
EditorWindow::EditorWindow(int w, int h, const char* t) : Fl_Double_Window(w, h, t) {
	replace_dlg = new Fl_Window(300, 105, "Replace");
	replace_find = new Fl_Input(80, 10, 210, 25, "Find:");
	replace_find->align(FL_ALIGN_LEFT);

	replace_with = new Fl_Input(80, 40, 210, 25, "Replace:");
	replace_with->align(FL_ALIGN_LEFT);

	replace_all = new Fl_Button(10, 70, 90, 25, "Replace All");
	replace_all->callback((Fl_Callback*)replall_cb, this);

	replace_next = new Fl_Return_Button(105, 70, 120, 25, "Replace Next");
	replace_next->callback((Fl_Callback*)replace2_cb, this);

	replace_cancel = new Fl_Button(230, 70, 60, 25, "Cancel");
	replace_cancel->callback((Fl_Callback*)replcan_cb, this);
	replace_dlg->end();
	replace_dlg->set_non_modal();
	editor = 0;
	*search = (char)0;
	// wrap_mode = 0;
	// line_numbers = 0;
}

EditorWindow::~EditorWindow() {
	delete replace_dlg;
}