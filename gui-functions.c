#include <string.h>
#include "gui-functions.h"
#include "resource.h"
void InsertListViewSubItem(HWND hWndListView, int index, int subItemIndex) {
    LVITEM lvI;
    lvI.pszText   = LPSTR_TEXTCALLBACK;
    lvI.mask      = LVIF_TEXT | LVIF_IMAGE |LVIF_STATE;
    lvI.stateMask = 0;
    lvI.iSubItem  = subItemIndex;
    lvI.state = 0;
    lvI.iItem = index;
    lvI.iImage = index;
    ListView_InsertItem(hWndListView, &lvI);
}
// InsertListViewItems: Inserts items into a list view.
// hWndListView:        Handle to the list-view control.
// Returns TRUE if successful, and FALSE otherwise.
BOOL InsertListViewItems(HWND hWndListView, int cItems, int cSubItems)
{
    LVITEM lvI;

    // Initialize LVITEM members that are common to all items.
    lvI.pszText   = LPSTR_TEXTCALLBACK; // Sends an LVN_GETDISPINFO message.
    lvI.mask      = LVIF_TEXT | LVIF_IMAGE |LVIF_STATE;
    lvI.stateMask = 0;
    lvI.iSubItem  = 0;
    lvI.state     = 0;

    // Initialize LVITEM members that are different for each item.
    for (int index = 0; index < cItems; index++)
    {
        lvI.iItem  = index;
        lvI.iImage = index;

        // Insert items into the list.
        if (ListView_InsertItem(hWndListView, &lvI) == -1)
            return FALSE;
        else {
            for (int i = 1; i <= cSubItems; i++) {
                InsertListViewSubItem(hWndListView, index, i);
            }
        }
    }
    return TRUE;
}

// InitListViewColumns: Adds columns to a list-view control.
// hWndListView:        Handle to the list-view control.
// Returns TRUE if successful, and FALSE otherwise.
BOOL InitListViewColumns(HWND hWndListView)
{
    WCHAR szText[256];     // Temporary buffer.
    LVCOLUMN lvc;
    int iCol;

    // Initialize the LVCOLUMN structure.
    // The mask specifies that the format, width, text,
    // and subitem members of the structure are valid.
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

    // Add the columns.
    for (iCol = 0; iCol < 3; iCol++)
    {
        lvc.iSubItem = iCol;
        lvc.pszText = (LPSTR)szText;
        // Width of column in pixels.
        if (iCol == 0) lvc.cx = 30;
        else lvc.cx = 150;
        lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.

        // Load the names of the column headings from the string resources.
        LoadString(NULL,
                   IDS_FIRSTCOLUMN + iCol,
                   (LPSTR)szText,
                   sizeof(szText)/sizeof(szText[0]));

        // Insert the columns into the list view.
        if (ListView_InsertColumn(hWndListView, iCol, &lvc) == -1)
            return FALSE;
    }

    return TRUE;
}
