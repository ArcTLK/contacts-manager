#include <windows.h>
#include "resource.h"

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch(uMsg) {
    case WM_INITDIALOG: {}
    break;
    case WM_CLOSE: {
        EndDialog(hwndDlg, 0);
    }
    break;
    case WM_COMMAND: {
        switch(LOWORD(wParam)) {}
    }
    break;
    default:
        return FALSE;
    }
    return TRUE;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    return DialogBox(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
