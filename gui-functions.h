#ifndef GUI_FUNCTIONS_H_INCLUDED
#define GUI_FUNCTIONS_H_INCLUDED
#include <windows.h>
#include <commctrl.h>
#include "manager.h"

BOOL InsertListViewItems(HWND hWndListView, int cItems, int cSubItems);
BOOL InitListViewColumns(HWND hWndListView, int numCol, int colNames);


#endif // GUI_FUNCTIONS_H_INCLUDED
