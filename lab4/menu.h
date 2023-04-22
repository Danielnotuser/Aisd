#ifndef MENU_H
#define MENU_H

#include "tree.h"

int read_nat(int*);
int read_str(char**);

int menu(const char *opts[], int num_opts);
int dlg_add(Tree*);
int dlg_delete(Tree*);
int dlg_find(Tree*);
int dlg_special_find(Tree*);
int dlg_import(Tree*);
int dlg_detour(Tree*);
int dlg_show(Tree*);
int dlg_rand();
void free_tree(Tree*);


#endif
