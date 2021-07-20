#ifndef __EditorWindow_H__
#define __EditorWindow_H__

#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Menu_Bar.H>
#include "CB.h"

class EditorWindow : public	Fl_Double_Window {
public:
	EditorWindow(int w, int h, const char* t);
	~EditorWindow();
	Fl_Window* replace_dlg;
	Fl_Input* replace_find;
	Fl_Input* replace_with;
	Fl_Button* replace_all;
	Fl_Return_Button* replace_next;
	Fl_Button* replace_cancel;
	Fl_Text_Editor* editor;
	//int	wrap_mode;
	//int	line_numbers;
	char search[256];
};

#endif