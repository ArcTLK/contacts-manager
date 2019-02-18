#include <windows.h>
#include "resource.h"

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgCreation(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgSearch(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgMain);
}

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch(uMsg) {
    case WM_INITDIALOG: {}
    break;
    case WM_CLOSE: {
        EndDialog(hwndDlg, 0);
    }
    break;
    case WM_COMMAND: {
        switch(LOWORD(wParam)) {
        case IDCREATE:
            DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG2), hwndDlg, (DLGPROC)DlgCreation);
            break;
        case IDSEARCH:
            DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG3), hwndDlg, (DLGPROC)DlgSearch);
            break;
        }
    }
    break;
    default:
        return FALSE;
    }
    return TRUE;
}

BOOL CALLBACK DlgCreation(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch(uMsg) {
    case WM_INITDIALOG: {}
    break;
    case WM_CLOSE: {
        EndDialog(hwndDlg, 0);
    }
    break;
    case WM_COMMAND: {
        switch(LOWORD(wParam)) {
        case IDOK: {
            int nameLen = GetWindowTextLength(GetDlgItem(hwndDlg, IDC_NAME));
            int numLen = GetWindowTextLength(GetDlgItem(hwndDlg, IDC_NUMBER));
            if (nameLen > 0 && numLen > 0) {
                char *name;
                char *number;
                name = (char*)GlobalAlloc(GPTR, nameLen + 1);
                number = (char*)GlobalAlloc(GPTR, numLen + 1);
                GetDlgItemText(hwndDlg, IDC_NAME, name, nameLen + 1);
                GetDlgItemText(hwndDlg, IDC_NUMBER, number, numLen + 1);
                //create contact
                createContact(name, number);
                MessageBox(hwndDlg, "A contact has been created!", "Success", NULL);
                GlobalFree((HANDLE)name);
                GlobalFree((HANDLE)number);
                EndDialog(hwndDlg, 0);
            }
            else MessageBox(hwndDlg, "Please enter values in the fields!", "Error", NULL);
        }
            break;
        case IDCANCEL:
            EndDialog(hwndDlg, 0);
            break;
        }
    }
    break;
    default:
        return FALSE;
    }
    return TRUE;
}

BOOL CALLBACK DlgSearch(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch(uMsg) {
    case WM_INITDIALOG: {}
    break;
    case WM_CLOSE: {
        EndDialog(hwndDlg, 0);
    }
    break;
    case WM_COMMAND: {
        switch(LOWORD(wParam)) {
        case IDOK: {
            int nameLen = GetWindowTextLength(GetDlgItem(hwndDlg, IDC_NAME));
            if (nameLen > 0) {
                char *name;
                char output[1000];
                name = (char*)GlobalAlloc(GPTR, nameLen + 1);
                GetDlgItemText(hwndDlg, IDC_NAME, name, nameLen + 1);
                //search
                searchContact(name, output);
                MessageBox(hwndDlg, output, "Success", NULL);
                GlobalFree((HANDLE)name);
                EndDialog(hwndDlg, 0);
            }
            else MessageBox(hwndDlg, "Please enter values in the fields!", "Error", NULL);
        }
            break;
        case IDCANCEL:
            EndDialog(hwndDlg, 0);
            break;
        }
    }
    break;
    default:
        return FALSE;
    }
    return TRUE;
}
