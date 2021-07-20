#define _CRT_SECURE_NO_WARNINGS
#include <FL/fl_ask.H> // for fl_input()
#include <FL/Fl_File_Chooser.H>	// fl_file_chooser()
#include <FL/Fl_Text_Buffer.H>
#include "CB.h"
#include "EditorWindow.h"

int changed = 0;
char filename[256] = "";
Fl_Text_Buffer* textbuf;
char title[256] = "";	// there isn't that statement

// other function:
int check_save(void) {
	if (!changed) return 1;
	int r = fl_choice("The current file has not been saved.\n"
		"Would you like to save it now?",
		"Cancel", "Save", "Discard");
	if (r == 1) {
		save_cb(); // Save the file...
		return !changed;
	}
	return (r == 2) ? 1 : 0;
}

int loading = 0;
void load_file(char* newfile, int ipos) {
	loading = 1;
	int insert = (ipos != -1);
	changed = insert;
	if (!insert) strcpy(filename, "");
	int r;
	if (!insert)
		r = textbuf->loadfile(newfile);
	else
		r = textbuf->insertfile(newfile, ipos);
	if (r)
		fl_alert("Error reading from file \'%s\':\n%s.", newfile, strerror(errno));
	else if (!insert) 
		strcpy(filename, newfile);
	loading = 0;
	textbuf->call_modify_callbacks();
}


void changed_cb(int, int nInserted, int nDeleted, int, const char*, void* v) {
	if ((nInserted || nDeleted) && !loading) changed = 1;
	EditorWindow* w = (EditorWindow*)v;
	set_title(w);
	if (loading) w->editor->show_insert_position();
}

void save_file(char* newfile) {
	if (textbuf->savefile(newfile))
		fl_alert("Error writing to file \'%s\':\n%s.", newfile, strerror(errno));
	else
		strcpy(filename, newfile);
	changed = 0;
	textbuf->call_modify_callbacks();
}

void set_title(Fl_Window* w) {
	if (filename[0] == '\0') strcpy(title, "Untitled");
	else {
		char* slash;
		slash = strrchr(filename, ' / ');
#ifdef WIN32
		if (slash == NULL) slash = strrchr(filename, '\\');
#endif
		if (slash != NULL)
			strcpy(title, slash + 1);
		else
			strcpy(title, filename);
	}
	if (changed) strcat(title, " (modified)");
	w->label(title);
}


void copy_cb(Fl_Widget*, void* v) {
	EditorWindow* e = (EditorWindow*)v;
	Fl_Text_Editor::kf_copy(0, e->editor);
}

void cut_cb(Fl_Widget*, void* v) {
	EditorWindow* e = (EditorWindow*)v;
	Fl_Text_Editor::kf_cut(0, e->editor);
}

void delete_cb(Fl_Widget*, void* v) {
	textbuf->remove_selection();
}

void find_cb(Fl_Widget* w, void* v) {
	EditorWindow* e = (EditorWindow*)v;
	const char* val;
	val = fl_input("Search String:", e->search);
	if (val != NULL) {
		// User entered a string - go find it!
		strcpy(e->search, val);
		find2_cb(w, v);
	}
}

void find2_cb(Fl_Widget* w, void* v) {
	EditorWindow* e = (EditorWindow*)v;
	if (e->search[0] == '\0') {
		// Search string is blank; get a new one...
		find_cb(w, v);
		return;
	}
	int pos = e->editor->insert_position();
	int found = textbuf->search_forward(pos, e->search, &pos);
	if (found) {
		// Found a match; select and update the position...
		textbuf->select(pos, pos + strlen(e->search));
		e->editor->insert_position(pos + strlen(e->search));
		e->editor->show_insert_position();
	}
	else
		fl_alert("No occurrences of \'%s\' found!", e->search);
}

void new_cb(Fl_Widget*, void*) {
	if (!check_save()) return;
	filename[0] = '\0';
	textbuf->select(0, textbuf->length());
	textbuf->remove_selection();
	changed = 0;
	textbuf->call_modify_callbacks();
}

void open_cb(Fl_Widget*, void*) {
	if (!check_save()) return;
	char* newfile = fl_file_chooser("Open File?", "*", filename);
	if (newfile != NULL) load_file(newfile, -1);
}

void paste_cb(Fl_Widget*, void* v) {
	EditorWindow* e = (EditorWindow*)v;
	Fl_Text_Editor::kf_paste(0, e->editor);
}

void quit_cb(Fl_Widget*, void*) {
	if (changed && !check_save())
		return;
	exit(0);
}

void replace_cb(Fl_Widget*, void* v) {
	EditorWindow* e = (EditorWindow*)v;
	e->replace_dlg->show();
}

void replace2_cb(Fl_Widget*, void* v) {
	EditorWindow* e = (EditorWindow*)v;
	const char* find = e->replace_find->value();
	const char* replace = e->replace_with->value();
	if (find[0] == '\0') {
		// Search string is blank; get a new one...
		e->replace_dlg->show();
		return;
	}
	e->replace_dlg->hide();
	int pos = e->editor->insert_position();
	int found = textbuf->search_forward(pos, find, &pos);
	if (found) {
		// Found a match; update the position and replace text...
		textbuf->select(pos, pos + strlen(find));
		textbuf->remove_selection();
		textbuf->insert(pos, replace);
		textbuf->select(pos, pos + strlen(replace));
		e->editor->insert_position(pos + strlen(replace));
		e->editor->show_insert_position();
	}
	else
		fl_alert("No occurrences of \'%s\' found!", find);
}

void replall_cb(Fl_Widget*, void* v) {
	EditorWindow* e = (EditorWindow*)v;
	const char* find = e->replace_find->value();
	const char* replace = e->replace_with->value();
	find = e->replace_find->value();
	if (find[0] == '\0') {
		// Search string is blank; get a new one...
		e->replace_dlg->show();
		return;
	}
	e->replace_dlg->hide();
	e->editor->insert_position(0);
	int times = 0;
	// Loop through the whole string
	for (int found = 1; found;) {
		int pos = e->editor->insert_position();
		found = textbuf->search_forward(pos, find, &pos);
		if (found) {
			// Found a match; update the position and replace text...
			textbuf->select(pos, pos + strlen(find));
			textbuf->remove_selection();
			textbuf->insert(pos, replace);
			e->editor->insert_position(pos + strlen(replace));
			e->editor->show_insert_position();
			times++;
		}
	}
	if (times) fl_message("Replaced %d occurrences.", times);
	else fl_alert("No occurrences of \'%s\' found!", find);
}

void replcan_cb(Fl_Widget*, void* v) {
	EditorWindow* e = (EditorWindow*)v;
	e->replace_dlg->hide();
}

void save_cb(void) {
	if (filename[0] == '\0') {
		// No filename - get one!
		saveas_cb();
		return;
	}
	else save_file(filename);
}

void saveas_cb(void) {
	char* newfile;
	newfile = fl_file_chooser("Save File As?", "*", filename);
	if (newfile != NULL) save_file(newfile);
}

int num_windows = 0;
void close_cb(Fl_Widget*, void* v) {
	EditorWindow* w = (EditorWindow*)v;

	if (num_windows == 1) {
		if (!check_save())
			return;
	}

	w->hide();
	w->editor->buffer(0);
	//textbuf->remove_modify_callback(style_update, w->editor);
	textbuf->remove_modify_callback(changed_cb, w);
	Fl::delete_widget(w);

	num_windows--;
	if (!num_windows) exit(0);
}