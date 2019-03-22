#include <windows.h>
#include "resource.h"
#include "gui-functions.h"
#include "manager.h"

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgCreation(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgSearch(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

//GLOBAL VARIABLES
contact *contacts;
int numOfContacts;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgMain);
}

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch(uMsg) {
        case WM_INITDIALOG: {
            numOfContacts = readFromFile(&contacts);
            InitListViewColumns(GetDlgItem(hwndDlg, ID_CONTACT_LIST));
            InsertListViewItems(GetDlgItem(hwndDlg, ID_CONTACT_LIST), numOfContacts);
        }
        break;
        case WM_CLOSE: {
            EndDialog(hwndDlg, 0);
        }
        break;
        case WM_COMMAND: {
            switch(LOWORD(wParam)) {
                case ID_ADD:
                    DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG2), hwndDlg, (DLGPROC)DlgCreation);
                    numOfContacts = readFromFile(&contacts);
                    ListView_DeleteAllItems(GetDlgItem(hwndDlg, ID_CONTACT_LIST));
                    InsertListViewItems(GetDlgItem(hwndDlg, ID_CONTACT_LIST), numOfContacts);
                    break;
                case ID_SEARCH:
                    DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG3), hwndDlg, (DLGPROC)DlgSearch);
                    break;
                case ID_ABOUT:
                    MessageBox(hwndDlg, "Contacts Manager is an application project written entirely in C by:\n"
                               "Aditya Sivaram Nair\nHarshvardhan Pandey\nManuj Narayana\nKirnesh\nAbin",
                               "About...", NULL);
                    break;
                case ID_QUIT:
                    PostMessage(hwndDlg, WM_CLOSE, 0, 0);
                    break;
            }
        }
        break;
        case WM_NOTIFY: {
            switch(((LPNMHDR) lParam)->code) {
                case LVN_GETDISPINFO: {
                    NMLVDISPINFO* plvdi;
                    plvdi = (NMLVDISPINFO*)lParam;
                    switch (plvdi->item.iSubItem) {
                        case 0: {
                            char text[50];
                            sprintf(text, "%d", plvdi->item.iItem + 1);
                            plvdi->item.pszText = text;
                        }
                            break;
                        case 1:
                            plvdi->item.pszText = contacts[plvdi->item.iItem].name;
                            break;
                        case 2:
                            plvdi->item.pszText = contacts[plvdi->item.iItem].number;
                            break;

                        default:
                            break;
                    }
                }
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
                int errorId;
                name = (char*)GlobalAlloc(GPTR, nameLen + 1);
                number = (char*)GlobalAlloc(GPTR, numLen + 1);
                GetDlgItemText(hwndDlg, IDC_NAME, name, nameLen + 1);
                GetDlgItemText(hwndDlg, IDC_NUMBER, number, numLen + 1);
                //create contact
                errorId = createContact(name, number);
                if (errorId == 0) MessageBox(hwndDlg, "You must enter a numerical value in the number field!", "Error", NULL);
                else MessageBox(hwndDlg, "A contact has been created!", "Success", NULL);
                GlobalFree((HANDLE)name);
                GlobalFree((HANDLE)number);
                if (errorId == 1) EndDialog(hwndDlg, 0);
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
                char *output;
                name = (char*)GlobalAlloc(GPTR, nameLen + 1);
                GetDlgItemText(hwndDlg, IDC_NAME, name, nameLen + 1);
                //search
                searchContact(name, &output);
                MessageBox(hwndDlg, output, "Result", NULL);
                GlobalFree((HANDLE)name);
                free(output);
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
