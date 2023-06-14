#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include "table.h"

int read_val(int*);
int read_nat(int*);
void print(Table*, FILE*);
int menu(const char*[], int);
int dlg_add(Table*);
int dlg_find(Table*);
int dlg_find_kids(Table*);
int dlg_delete(Table*);
int dlg_print(Table*);
int dlg_import(Table*);

#endif
