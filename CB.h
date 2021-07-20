#ifndef __CB_H__
#define __CB_H__

#include <FL/Fl_Widget.H>

// cb
void changed_cb(int, int nInserted, int nDeleted, int, const char*, void* v);
void copy_cb(Fl_Widget*, void* v);
void cut_cb(Fl_Widget*, void* v);
void delete_cb(Fl_Widget*, void* v);
void find_cb(Fl_Widget* w, void* v);
void find2_cb(Fl_Widget* w, void* v);
void new_cb(Fl_Widget*, void*);
void open_cb(Fl_Widget*, void*);
void paste_cb(Fl_Widget*, void* v);
void quit_cb(Fl_Widget*, void*);
void replace_cb(Fl_Widget*, void* v);
void replace2_cb(Fl_Widget*, void* v);
void replall_cb(Fl_Widget*, void* v);
void replcan_cb(Fl_Widget*, void* v);
void save_cb(void);
void saveas_cb(void);
void close_cb(Fl_Widget*, void* v);

// other function
int check_save(void);
void load_file(char* newfile, int ipos);
void changed_cb(int, int nInserted, int nDeleted, int, const char*, void* v);
void save_file(char* newfile);
void set_title(Fl_Window* w);

#endif // !__CB_H__