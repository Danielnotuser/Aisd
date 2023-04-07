#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include "table.h"

int read_val(int*);
int read_nat(int*);

int menu(const char*[], int);
int dlg_load(Table*);
int dlg_add(Table*);
int dlg_find(Table*);
int dlg_delete(Table*);
int dlg_save(Table*);
int dlg_import(Table*);
int print(Table*);

#endif
