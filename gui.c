#define _WIN32_IE 0x500
#include <windows.h>
#include <string.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"
#include "gui-functions.h"
#include "manager.h"

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgCreation(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgSearch(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgDelete(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgEdit(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgContact(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

//GLOBAL VARIABLES
contact *contacts;
int numOfContacts;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    InitCommonControls();
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgMain);
}

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch(uMsg) {
        case WM_INITDIALOG: {
            numOfContacts = readFromFile(&contacts);
            InitListViewColumns(GetDlgItem(hwndDlg, ID_CONTACT_LIST), 3, IDS_FIRSTCOLUMN);
            InsertListViewItems(GetDlgItem(hwndDlg, ID_CONTACT_LIST), numOfContacts, 3);
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
                    InsertListViewItems(GetDlgItem(hwndDlg, ID_CONTACT_LIST), numOfContacts, 3);
                    break;
                case ID_SEARCH:
                    DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG3), hwndDlg, (DLGPROC)DlgSearch);
                    break;
                case ID_DELETE:
                    DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG4), hwndDlg, (DLGPROC)DlgDelete);
                    numOfContacts = readFromFile(&contacts);
                    ListView_DeleteAllItems(GetDlgItem(hwndDlg, ID_CONTACT_LIST));
                    InsertListViewItems(GetDlgItem(hwndDlg, ID_CONTACT_LIST), numOfContacts, 3);
                    break;
                case ID_EDIT:
                    DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG5), hwndDlg, (DLGPROC)DlgEdit);
                    numOfContacts = readFromFile(&contacts);
                    ListView_DeleteAllItems(GetDlgItem(hwndDlg, ID_CONTACT_LIST));
                    InsertListViewItems(GetDlgItem(hwndDlg, ID_CONTACT_LIST), numOfContacts, 3);
                    break;
                case ID_ABOUT:
                    MessageBox(hwndDlg, "Contacts Manager is an application project written entirely in C by:\n"
                               "Aditya Sivaram Nair\nHarshvardhan Pandey\nManuj Narayanaa Rajaram\nKirnesh Walunj\nAbin Varghese",
                               "About...", 0);
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
                            char text[50] = "";
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
                case NM_CLICK: {
                    LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE)lParam;
                    if (lpnmitem->iItem != -1) { //check if click is on item
                        DialogBoxParam(NULL, MAKEINTRESOURCE(IDD_DIALOG6), hwndDlg, (DLGPROC)DlgContact, lpnmitem->iItem);
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
                if (errorId == ERR_NAN) MessageBox(hwndDlg, "You must enter a numerical value in the number field!", "Error", 0);
                else if (errorId == ERR_DUPLICATE_CONTACT) MessageBox(hwndDlg, "A contact with the same number is present in the list!", "Error: Duplicate Number", 0);
                else MessageBox(hwndDlg, "A contact has been created!", "Success", 0);
                GlobalFree((HANDLE)name);
                GlobalFree((HANDLE)number);
                if (errorId == 1) PostMessage(hwndDlg, WM_CLOSE, 0, 0);
            }
            else MessageBox(hwndDlg, "Please enter values in the fields!", "Error", 0);
        }
            break;
        case IDCANCEL:
            PostMessage(hwndDlg, WM_CLOSE, 0, 0);
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
    static contact *foundContacts;
    static unsigned int contactsFound = 0;
    switch(uMsg) {
    case WM_INITDIALOG: {
        InitListViewColumns(GetDlgItem(hwndDlg, IDC_FOUND_CONTACTS), 2, IDS_SECONDCOLUMN);
    }
    break;
    case WM_CLOSE: {
        EndDialog(hwndDlg, 0);
    }
    break;
    case WM_COMMAND: {
        switch(LOWORD(wParam)) {
            case IDC_SEARCH_INP: {
                //check notification code
                switch(HIWORD(wParam)) {
                    case CBN_SETFOCUS: {
                        SendDlgItemMessage(hwndDlg, IDC_SEARCH_INP, CB_SHOWDROPDOWN, (WPARAM)TRUE, (WPARAM)NULL);
                    }
                    break;
                    case CBN_KILLFOCUS: {
                        SendDlgItemMessage(hwndDlg, IDC_SEARCH_INP, CB_SHOWDROPDOWN, (WPARAM)FALSE, (WPARAM)NULL);
                    }
                    break;
                    case CBN_SELCHANGE: {
                        COMBOBOXINFO info = { sizeof(COMBOBOXINFO) };
                        GetComboBoxInfo(GetDlgItem(hwndDlg, IDC_SEARCH_INP), &info);
                        char *inputBuffer;
                        int selectedIndex = SendDlgItemMessage(hwndDlg, IDC_SEARCH_INP, CB_GETCURSEL, (WPARAM)NULL, (LPARAM)NULL);
                        if (selectedIndex != CB_ERR) {
                            int inputLen = SendDlgItemMessage(hwndDlg, IDC_SEARCH_INP, CB_GETLBTEXTLEN, (WPARAM)selectedIndex, (LPARAM)NULL);
                            inputBuffer = (char*)GlobalAlloc(GPTR, inputLen + 1);
                            SendDlgItemMessage(hwndDlg, IDC_SEARCH_INP, CB_GETLBTEXT, (WPARAM)selectedIndex, (LPARAM)inputBuffer);
                            searchContact(inputBuffer, 3, &foundContacts, &contactsFound);
                            ListView_DeleteAllItems(GetDlgItem(hwndDlg, IDC_FOUND_CONTACTS));
                            if (contactsFound > 0) {
                                InsertListViewItems(GetDlgItem(hwndDlg, IDC_FOUND_CONTACTS), contactsFound, 1);
                            }
                            GlobalFree((HANDLE)inputBuffer);
                        }
                    }
                    break;
                    case CBN_EDITCHANGE: {
                        COMBOBOXINFO info = { sizeof(COMBOBOXINFO) };
                        GetComboBoxInfo(GetDlgItem(hwndDlg, IDC_SEARCH_INP), &info);
                        char *inputBuffer;
                        int inputLen = GetWindowTextLength(info.hwndItem);
                        if (inputLen > 0) {
                            //process!
                            inputBuffer = (char*)GlobalAlloc(GPTR, inputLen + 1);
                            GetWindowText(info.hwndItem, inputBuffer, inputLen + 1);
                            searchContact(inputBuffer, 3, &foundContacts, &contactsFound);
                            ListView_DeleteAllItems(GetDlgItem(hwndDlg, IDC_FOUND_CONTACTS));
                            if (contactsFound > 0) {
                                InsertListViewItems(GetDlgItem(hwndDlg, IDC_FOUND_CONTACTS), contactsFound, 1);
                                //clean earlier
                                int numItems = SendDlgItemMessage(hwndDlg, IDC_SEARCH_INP, CB_GETCOUNT, (WPARAM)NULL, (LPARAM)NULL);
                                for (int i = 0; i < numItems; i++) {
                                    SendDlgItemMessage(hwndDlg, IDC_SEARCH_INP, CB_DELETESTRING, (WPARAM)0, (LPARAM)NULL);
                                }

                                for (int i = 0; i < contactsFound; i++) {
                                    SendDlgItemMessage(hwndDlg, IDC_SEARCH_INP, CB_ADDSTRING, (WPARAM)NULL, (LPARAM)foundContacts[i].name);
                                }
                            }
                            GlobalFree((HANDLE)inputBuffer);
                        }
                    }
                    break;
                }
            }
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
                        case 0:
                            plvdi->item.pszText = foundContacts[plvdi->item.iItem].name;
                            break;
                        case 1:
                            plvdi->item.pszText = foundContacts[plvdi->item.iItem].number;
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

BOOL CALLBACK DlgDelete(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch(uMsg) {
    case WM_INITDIALOG: {}
    break;
    case WM_CLOSE: {
        EndDialog(hwndDlg, 0);
    }
    break;
    case WM_COMMAND: {
        switch(LOWORD(wParam)) {
        case IDC_DELETE: {
            int idLen = GetWindowTextLength(GetDlgItem(hwndDlg, IDC_ID));
            if (idLen > 0) {
                char *idStr;
                unsigned int id;
                int result;
                idStr = (char*)GlobalAlloc(GPTR, idLen + 1);
                GetDlgItemText(hwndDlg, IDC_ID, idStr, idLen + 1);
                id = (unsigned int)strtol(idStr, NULL, 10) - 1;
                result = deleteContact(&id);
                GlobalFree((HANDLE)idStr);
                if (result == 0) MessageBox(hwndDlg, "Invalid ID!", "Error", 0);
                else {
                    MessageBox(hwndDlg, "Contact has been deleted!", "Success", 0);
                    PostMessage(hwndDlg, WM_CLOSE, 0, 0);
                }
            }
            else MessageBox(hwndDlg, "Please enter values in the fields!", "Error", 0);
        }
            break;
        case IDCANCEL:
            PostMessage(hwndDlg, WM_CLOSE, 0, 0);
            break;
        }
    }
    break;
    default:
        return FALSE;
    }
    return TRUE;
}

BOOL CALLBACK DlgEdit(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch(uMsg) {
    case WM_INITDIALOG: {}
    break;
    case WM_CLOSE: {
        EndDialog(hwndDlg, 0);
    }
    break;
    case WM_COMMAND: {
        switch(LOWORD(wParam)) {
        case IDC_LOAD: {
            int idLen = GetWindowTextLength(GetDlgItem(hwndDlg, IDC_ID));
            if (idLen > 0) {
                unsigned int id;
                char *idStr;
                idStr = (char*)GlobalAlloc(GPTR, idLen + 1);
                GetDlgItemText(hwndDlg, IDC_ID, idStr, idLen + 1);
                id = (unsigned int)strtol(idStr, NULL, 10) - 1;
                GlobalFree((HANDLE)idStr);
                numOfContacts = readFromFile(&contacts);
                if (id >= numOfContacts) MessageBox(hwndDlg, "Invalid ID!", "Error", 0);
                else {
                    SetWindowText(GetDlgItem(hwndDlg, IDC_NAME), contacts[id].name);
                    SetWindowText(GetDlgItem(hwndDlg, IDC_NUM), contacts[id].number);
                    EnableWindow(GetDlgItem(hwndDlg, IDC_NAME), TRUE);
                    EnableWindow(GetDlgItem(hwndDlg, IDC_NUM), TRUE);
                    EnableWindow(GetDlgItem(hwndDlg, IDC_OK), TRUE);
                }
            }
            else MessageBox(hwndDlg, "Please enter values in the fields!", "Error", 0);
        }
            break;
        case IDC_OK: {
            int change = 0;
            int result;
            unsigned int id;
            int idLen = GetWindowTextLength(GetDlgItem(hwndDlg, IDC_ID));
            int nameLen = GetWindowTextLength(GetDlgItem(hwndDlg, IDC_NAME));
            int numLen = GetWindowTextLength(GetDlgItem(hwndDlg, IDC_NUM));
            if (nameLen > 0 && numLen > 0 && idLen > 0) {
                char *name = (char*)GlobalAlloc(GPTR, nameLen + 1);
                char *number = (char*)GlobalAlloc(GPTR, numLen + 1);
                char *idStr = (char*)GlobalAlloc(GPTR, idLen + 1);
                GetDlgItemText(hwndDlg, IDC_NAME, name, nameLen + 1);
                GetDlgItemText(hwndDlg, IDC_NUM, number, numLen + 1);
                GetDlgItemText(hwndDlg, IDC_ID, idStr, idLen + 1);
                id = (unsigned int)strtol(idStr, NULL, 10) - 1;
                GlobalFree((HANDLE)idStr);
                numOfContacts = readFromFile(&contacts);
                if (id >= numOfContacts) MessageBox(hwndDlg, "Invalid ID!", "Error", 0);
                else {
                    if (strcmp(name, contacts[id].name) != 0) {
                        result = editContact(&id, 1, name);
                        change = 1;
                    }
                    if (strcmp(number, contacts[id].number) != 0) {
                        result = editContact(&id, 2, number);
                        change = 1;
                    }
                    GlobalFree((HANDLE)name);
                    GlobalFree((HANDLE)number);
                    if (!change) MessageBox(hwndDlg, "No changes done!", "Error", 0);
                    else if (result == ERR_INVALID_INDEX) MessageBox(hwndDlg, "Invalid ID!", "Error", 0);
                    else if (result == ERR_INVALID_PROPERTY) MessageBox(hwndDlg, "Invalid property passed!", "Error", 0);
                    else if (result == ERR_NAN) MessageBox(hwndDlg, "Number can only contain numerical values!", "Error", 0);
                    else if (result == ERR_DUPLICATE_CONTACT) MessageBox(hwndDlg, "A contact with the same number is present in the list!", "Error: Duplicate Number", 0);
                    else {
                        MessageBox(hwndDlg, "Contact has been edited successfully!", "Success", 0);
                        PostMessage(hwndDlg, WM_CLOSE, 0, 0);
                    }
                }
            }
            else MessageBox(hwndDlg, "Please enter values in the fields!", "Error", 0);
        }
            break;
        case IDCANCEL:
            PostMessage(hwndDlg, WM_CLOSE, 0, 0);
            break;
        }
    }
    break;
    default:
        return FALSE;
    }
    return TRUE;
}
BOOL CALLBACK DlgContact(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static char localDIR[MAX_PATH] = "";
    switch(uMsg) {
    case WM_INITDIALOG: {
        if (lParam >= 0) { //array index must not be negative!
            //set localDIR if not set
            if (strcmp(localDIR, "") == 0) {
                GetCurrentDirectory(sizeof(localDIR), localDIR);
                strcat(localDIR, "\\");
            }
            SetWindowText(GetDlgItem(hwndDlg, IDS_NAME), contacts[lParam].name);
            SetWindowText(GetDlgItem(hwndDlg, IDS_NUMBER), contacts[lParam].number);
            //check if image file exists
            FILE *file;
            char fileName[MAX_PATH] = "";
            strcpy(fileName, localDIR);
            strcat(fileName, contacts[lParam].number);
            strcat(fileName, ".bmp");
            HBITMAP bm;
            if ((file = fopen(fileName, "r")) != NULL) {
                //file exists
                fclose(file);
                //load image
                bm = (HBITMAP)LoadImage(NULL, fileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
            }
            else {
                //load default image
                strcpy(fileName, localDIR);
                strcat(fileName, "no-image.bmp");
                bm = (HBITMAP)LoadImage(NULL, fileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
            }
            SendDlgItemMessage(hwndDlg, IDC_PICTURE, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bm);
        }
    }
    break;
    case WM_CLOSE: {
        EndDialog(hwndDlg, 0);
    }
    break;
    case WM_COMMAND: {
        switch(LOWORD(wParam)) {
            case IDC_PICTUREBUTTON: {
                OPENFILENAME ofn;
                // Initialize OPENFILENAME
                ZeroMemory(&ofn, sizeof(ofn));
                char fileName[FILENAME_MAX] = "";
                ofn.lStructSize = sizeof(ofn);
                ofn.lpstrFilter = "Bitmap Image (*.bmp)\0*.bmp\0";
                ofn.lpstrFile = fileName;
                ofn.nMaxFile = MAX_PATH;
                ofn.hwndOwner = hwndDlg;
                ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
                //making name for new file
                char newFileName[MAX_PATH] = "";
                char number[NUMBER_SIZE] = "";
                strcpy(newFileName, localDIR);
                GetWindowText(GetDlgItem(hwndDlg, IDS_NUMBER), number, sizeof(number));
                strcat(newFileName, number);
                strcat(newFileName, ".bmp");
                if (GetOpenFileName(&ofn) != 0) {
                    //copy image
                    FILE *source, *destination;
                    int i;
                    destination = fopen(newFileName, "wb");
                    source = fopen(ofn.lpstrFile, "rb"); //open file
                    fseek(source, 0, SEEK_END); //seek till end to get last position
                    int length = ftell(source); //get length of data
                    fseek(source, 0, SEEK_SET); //reset
                    for(i = 0; i < length; i++){
                        fputc(fgetc(source), destination);
                    }
                    fclose(source);
                    fclose(destination);
                    //load image
                    HBITMAP bm = (HBITMAP)LoadImage(NULL, newFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
                    if (bm == NULL)
                    {
                        MessageBox(NULL, "Error loading image", "Error", 0);
                        return TRUE;
                    }
                    //set image
                    else SendDlgItemMessage(hwndDlg, IDC_PICTURE, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bm);

                }
                else MessageBox(hwndDlg, "File not selected!", "Error", 0);
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
