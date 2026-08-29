/*
.##....##..#######..########.##....##
.###...##.##.....##....##.....##..##.
.####..##.##.....##....##......####..
.##.##.##.##.....##....##.......##...
.##..####.##.....##....##.......##...
.##...###.##.....##....##.......##...
.##....##..#######.....##.......##...
*/

// Hot Keys.

// Append -> ctrl+alt+s
// New_File -> ctrl+alt+n
// Exit_Program -> ctrl+alt+x

// HEADER Files
#include <cstddef>
#include <ctime>
#include <fstream>
#include <iostream>
#include <shlobj.h>
#include <windows.h>
#include <winuser.h>

// Namespace
using namespace std;

//-GLOBAL variables----------------------------------------------------------------------------

// "No Text Selected" Toast Msg, Global Switch
bool sw_e_1 = 0;
// "Exit" Toast Msg, Global Switch
bool sw_exit = 0;
// "New File Created" Toast Msg, Global switch
bool sw_mkfile = 0;
// "NOTY STARTED" Toast Msg, Global switch
bool sw_start = 0;
// Toast X location var.
int X_loc_t = 0;
// Toast Y location var.
int Y_loc_t = 0;

//-Global variables---------------------------------------------------------------------------

// Functions

// Return the current text in the Clipboard
char *TextInClipBoard() {
  // Checking if the hotkey was pressed without any text selected

  OpenClipboard(NULL);

  // getting text handle for the copied text
  HGLOBAL memory_handle1 = GetClipboardData(CF_TEXT);

  // getting address from that handle and as is void so casting it to char ptr
  // and storing it in.
  char *prevtext = (char *)GlobalLock(memory_handle1);

  // freeing the clipboard
  GlobalUnlock(memory_handle1);

  // closing the clipboard
  CloseClipboard();

  // When
  if (prevtext == NULL) {
    prevtext = "";
  }
  return prevtext;
}

// Call back function for Save_Toast.
LRESULT CALLBACK Save_toast_f(HWND hwnd, UINT msg, WPARAM wParam,
                              LPARAM lParam) {

  switch (msg) {
  // Case when the msg is paint
  case WM_PAINT: {
    // Create a Paint struct that windows will fill for you, that struct will have
    // info about the window I have made that will be used for painting the box
    PAINTSTRUCT toast_p;
    // This function will fill the paint struct, we have passed the hwnd that is
    // the handle for my window. HWND is the handle for the toast window and
    // hdc is the handle for the canvas, the drawable area on that window.
    BeginPaint(hwnd, &toast_p);

    // Fill Window with PAINT
    // Create a Rect struct to get the dimensions of the paintable area in the
    // window we created as all the window dims cannot be painted like borders etc.
    RECT rc;

    // rcPaint filled by the Begin Paint Function has the non painted area
    // dimensions and if an area somewhat has some already fixed pixels so the dim
    // will be wrong in my case so we will be using the GetClientRect function
    // that will always return the dimensions of our window irrespective of
    // the printable area.
    // cout<<toast_p.rcPaint.right<<"x"<<toast_p.rcPaint.bottom<<"
    // "<<rc.right<<"x"<<rc.bottom;
    // creating a brush object that will then be used to paint the specified
    // area. this mechanism is followed as there are many other objects like Pencil pen
    // and even in brush there are many variants so it is like terminal MS PAINT
    HBRUSH Brush = CreateSolidBrush(RGB(108, 108, 108));

    // Add Text to the window.
    // Making the bg of text transparent, it is like a shadow of the text which
    // by default is black so we are making it transparent
    SetBkMode(toast_p.hdc, TRANSPARENT);
    // Set Text color from below line onward to White on the canvas referred to.
    SetTextColor(toast_p.hdc, RGB(255, 255, 255));

    // Text to display.
    wchar_t *text = NULL;
    // -------------"New File Made !"-------------------
    if (sw_mkfile == 1) {
      // Text to display in toast
      text = L"New File Made !";
      // reset switch
      sw_mkfile = 0;
    }
    //-----------------"NOTY STARTED"-------------------------
    else if (sw_start == 1) {
      // Text to display in toast
      text = L"NOTY STARTED !";
      // reset switch
      sw_start = 0;

    }
    // --------------"NOTY TERMINATED !"--------------------------
    else if (sw_exit == 1) {
      // Text to display in toast
      text = L"NOTY TERMINATED !";
      // reset switch
      sw_exit = 0;
    }

    // ----------- "Error: No text selected / Same text!" -----------------------
    else if (sw_e_1 == 1) {
      // Text to display in toast
      text = L"Error: No text selected / Same text!";
      // reset switch
      sw_e_1 = 0;
    }
    // -------------"Saved !"-------------------
    else {
      text = L"Saved !";
    }

    // You cannot make these lines before the set pos, as they work on current
    // dim of the window. Fill the rectangle with the dimensions of the
    // printable area
    GetClientRect(hwnd, &rc);
    // Takes the canvas handle and ref to the rec struct i.e. the dimensions
    // where I will have to draw.
    FillRect(toast_p.hdc, &rc, Brush);
    // Drawing the text on canvas with, Horz Centered | On a single line | Vert
    // Centered, -1 means to calc str size on its own otherwise provide it the size
    // and L to indicate long str as our project uses UNICODE format which are L str
    // and obviously we will refer to our canvas by hdc
    DrawTextW(toast_p.hdc, text, -1, &rc,
              DT_CENTER | DT_SINGLELINE | DT_VCENTER);
    // as we made a handle by ourself and allocated it an address so we have to
    // free it manually.
    DeleteObject(Brush);
    // free memory
    EndPaint(hwnd, &toast_p);
    // handle to window, id for timer, time in ms, custom callback function ptr
    SetTimer(hwnd, 1, 1000, NULL);
    return 0;
  }
  // when timer completes
  case WM_TIMER: {
    // kill timer
    KillTimer(hwnd, 1);
    // kill window
    DestroyWindow(hwnd);
    return 0;
  }
  default:
    return DefWindowProc(hwnd, msg, wParam, lParam);
  }
  // function end
}

void Create_Toast() {
  // Switch for Registering class (to make class register only once) static var
  // has life till end so after going out of scope val still remains saved and
  // again on function call will be saved in the var and var will not be
  // created again lol
  static int registered = 0;
  // Store HINSTANCE for NOTY
  HINSTANCE NOTY_handle = GetModuleHandle(NULL);

  // We cannot register a class again and again, it will register windows with
  // same names illegal and there is no logic of doing it, we will just create
  // and display the window and will register it once on first run so I added a check that
  // makes this run only the first time and after it runs the switch goes off till prog ends
  // and this block will never run again, only the window will be created every time.
  if (!registered) {
    // CREATE toast window.
    // It is a C type struct that is predefined by winapi32, in C type structs
    // there is no constructor mechanism like C++. When in these C type structs
    // we do {} or ={} (both are the same, {} is modern C++ style and = {} is old C
    // style where a copy is made but modern C++ in it does copy elision), then
    // the structs members get initialized by default values and by just declaring
    // the struct the members get initialized by garbage values. When in C++
    // structs we do {} or ={} and we by our own define the default constructor then the
    // default constructor runs otherwise if there is the win provided constructor then the
    // constructor will not run and the compiler will initialize the struct members by
    // 0. So here we did {} and can also do ={}
    WNDCLASS Save_toast{};

    // Set a Call Back Function for save toast
    Save_toast.lpfnWndProc = Save_toast_f;
    // Make the window belong to NOTY by passing its handle
    Save_toast.hInstance = NOTY_handle;
    // Set name for Window class
    Save_toast.lpszClassName = "Save_toast_notification";
    // Register save_toast
    RegisterClass(&Save_toast);
    // Make this block not run ever again by turning the switch off
    registered = 1;
  }

  // Getting windows X Resolution
  int X_res = GetSystemMetrics(SM_CXSCREEN);

  // Getting windows Y Resolution
  int Y_res = GetSystemMetrics(SM_CYSCREEN);
  // cout<<X_res<<"x"<<Y_res;
  //  X Location of Toast window, to locate it at right bottom.
  X_loc_t = X_res - 93;

  // Get TaskBar Dimensions, to display the Toast over the Taskbar.
  //  ={} initializes the struct to default vals, 0, else it will have garbage, it is
  //  a C style struct
  APPBARDATA taskbar_dim = {};
  // Giving the size of the struct so that windows could match the correct ver for
  // the struct and could operate on it.
  taskbar_dim.cbSize = sizeof(APPBARDATA);
  // storing the dim in the appbardata struct
  SHAppBarMessage(ABM_GETTASKBARPOS, &taskbar_dim);
  // calc the Taskbar Y length
  int tkbr_Ylen = taskbar_dim.rc.bottom - taskbar_dim.rc.top;
  // cout<<"=="<< taskbar_dim.rc.bottom<<"-"<<taskbar_dim.rc.top<<"==";
  // cout<<"|"<<tkbr_Ylen<<"|";
  //  Y Location of Toast window, to locate it at right bottom. subtract the taskbar y
  //  dim with some margin lol, subtract the y len of toast also lol
  Y_loc_t = Y_res - tkbr_Ylen - 25;
  // cout<<Y_loc_t;

  // Create window class
  //  (Transparency effect enable | Invisible taskbar icon for window | Not to gain
  //  keyboard focus, Win_name, Win_title_name, Specify no titlebar or menu
  //  buttons, x coor, y coor, width, height, parent window, dropdown menu, window to
  //  which it belongs to, Extra Info)
  //  Handle to the windows.
  HWND Toast;
  //  CASE: "NO text selected / Same Text!"
  if (sw_e_1 == 1) {
    // Size the Toast wrt text size, 90, 30 was original size
    Toast =
        CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_TOPMOST,
                       "Save_toast_notification", "", WS_POPUP, X_loc_t - 171,
                       Y_loc_t, 250, 30, NULL, NULL, NOTY_handle, NULL);
  }
  // CASE: "New File Made !"
  else if (sw_mkfile == 1) {
    // Size the Toast wrt text size, 90, 30 was original size
    Toast =
        CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_TOPMOST,
                       "Save_toast_notification", "", WS_POPUP, X_loc_t - 51,
                       Y_loc_t, 140, 30, NULL, NULL, NOTY_handle, NULL);
  }
  // CASE:: "NOTY TERMINATED !"
  else if (sw_exit == 1) {
    Toast =
        CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_TOPMOST,
                       "Save_toast_notification", "", WS_POPUP, X_loc_t - 61,
                       Y_loc_t, 150, 30, NULL, NULL, NOTY_handle, NULL);

  } else if (sw_start == 1) {
    Toast =
        CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_TOPMOST,
                       "Save_toast_notification", "", WS_POPUP, X_loc_t - 51,
                       Y_loc_t, 140, 30, NULL, NULL, NOTY_handle, NULL);

  }

  // CASE: "Saved !"
  else {
    Toast = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_TOPMOST,
                           "Save_toast_notification", "", WS_POPUP, X_loc_t,
                           Y_loc_t, 90, 30, NULL, NULL, NOTY_handle, NULL);
  }

  ShowWindow(Toast, SW_SHOWNOACTIVATE);
}

// MAIN
int main() {

  // Display Program Start Toast Message
  sw_start = 1;
  Create_Toast();

  // adding this keycombo to registry, NULL so it puts the msg in the same current
  // thread
  RegisterHotKey(NULL, 1, MOD_CONTROL | MOD_ALT, 'S');

  // add ctrl+alt+n for new file
  RegisterHotKey(NULL, 2, MOD_CONTROL | MOD_ALT, 'N');

  // for prog exit
  RegisterHotKey(NULL, 0, MOD_CONTROL | MOD_ALT, 'X');

  // c-string var for path to save the file.
  char pathtosave[300];

  // var for config file path for user.
  char pathtoconfig[260];

  // storing the path to appdata for config path
  SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, pathtoconfig);

  // manually completing the config path
  strcat(pathtoconfig, "\\NOTY\\config.ini");

  // option for naming the file. 0-> wrt time 1-> wrt data
  char naming_option[2];

  // saving naming option in naming_option
  GetPrivateProfileString("Rename_Type", "option", "e", naming_option, 2,
                          pathtoconfig);

  // Exception check for no naming option
  if (naming_option[0] == 'e') {
    MessageBox(NULL,
               "NOTY found no naming data, reinstall the program for fix! \n "
               "\n \"Setting default renaming\" ",
               "NOTY", MB_OK);
  }

  // Endline Tweak detection

  // endl tweak
  char endline[2];

  // storing value of tweak
  GetPrivateProfileString("Tweaks", "t_endline", "e", endline, 2, pathtoconfig);

  // Exception handling for no file.
  if (endline[0] == 'e') {
    MessageBox(NULL,
               "NOTY found no Tweaks data, reinstall the program for fix! \n "
               "\n \"Setting default Tweaks\" ",
               "NOTY", MB_OK);
  }

  // bullets tweak
  char bullets[2];

  // storing value of tweak
  GetPrivateProfileString("Tweaks", "t_bullets", "e", bullets, 2, pathtoconfig);

  // Exception handling for no file.
  if (bullets[0] == 'e') {
    MessageBox(NULL,
               "NOTY found no Tweaks data, reinstall the program for fix! \n "
               "\n \"Setting default Tweaks\" ",
               "NOTY", MB_OK);
  }

  // double endline tweak
  char double_endline[2];

  // storing value of tweak
  GetPrivateProfileString("Tweaks", "t_double_endline", "e", double_endline, 2,
                          pathtoconfig);

  // Exception handling for no file.
  if (double_endline[0] == 'e') {
    MessageBox(NULL,
               "NOTY found no Tweaks data, reinstall the program for fix! \n "
               "\n \"Setting default Tweaks\" ",
               "NOTY", MB_OK);
  }

  while (1) {

    // storing file saving path.
    GetPrivateProfileString("LocationToSaveFile", "path", "e", pathtosave, 299,
                            pathtoconfig);

    // Exception handling for no config file found.
    if (pathtosave[0] == 'e') {
      MessageBox(NULL,
                 "NOTY found no config file, reinstall the program! \n \n "
                 "\"Program Terminated\" ",
                 "NOTY", MB_OK);
      return 0;
    }

    // correcting path
    strcat(pathtosave, "\\");

    // checking if the folder exists at the path where we have to save the file
    // or not.

    if (GetFileAttributes(pathtosave) == INVALID_FILE_ATTRIBUTES) {
      // if no directory, creating one
      if (!CreateDirectory(pathtosave, NULL)) {

        // If failed to create directory
        MessageBox(
            NULL,
            "The directory to save files is damaged, reinstall the program and "
            "set a healthy directory! \n \n \"Program Terminated\" ",
            "NOTY", MB_OK);
        return 0;
      }
    }

    // path to save, naming file wrt time
    if (naming_option[0] == '0') {
      // storing current time of machine for time based txt files
      time_t t = time(NULL);
      struct tm *tm_info = localtime(&t);

      // storing formatted time in cstr
      char tname[50];
      strftime(tname, 50, "%H-%M-%S_%Y-%m-%d", tm_info);

      // making a correct path for file to save
      strcat(tname, ".txt");
      strcat(pathtosave, tname);
    }

    // add struct for msg saving in message loop
    MSG add;

    // temp var for wrt text based rename
    char temptext[1000];

    //////////////////////////////////////
    // naming file wrt first copied text
    //////////////////////////////////////

    if (naming_option[0] == '1') {
      MSG temp_for_name;

      // getting the txt for rename.
      while (GetMessage(&temp_for_name, NULL, 0, 0)) {

        DispatchMessage(&temp_for_name);

        if (temp_for_name.message == WM_HOTKEY && temp_for_name.wParam == 1) {

          // Save current text in clipboard before copying, will then use it to
          // defend the exception when user presses hotkey without text selected
          char *PrevText = TextInClipBoard();

          // Ctrl+C action script forming in INPUT struct
          INPUT inputs[4] = {};

          // for pressing Ctrl
          inputs[0].type = INPUT_KEYBOARD;
          inputs[0].ki.wVk = VK_CONTROL;

          // for pressing C
          inputs[1].type = INPUT_KEYBOARD;
          inputs[1].ki.wVk = 'C';

          // for releasing Ctrl
          inputs[2].type = INPUT_KEYBOARD;
          inputs[2].ki.wVk = 'C';
          inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

          // for releasing C
          inputs[3].type = INPUT_KEYBOARD;
          inputs[3].ki.wVk = VK_CONTROL;
          inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

          // for releasing all the keys at time of sending the operation
          INPUT release[8] = {};

          release[0].type = INPUT_KEYBOARD;
          release[0].ki.wVk = VK_MENU;
          release[0].ki.dwFlags = KEYEVENTF_KEYUP;

          release[1].type = INPUT_KEYBOARD;
          release[1].ki.wVk = VK_SHIFT;
          release[1].ki.dwFlags = KEYEVENTF_KEYUP;

          release[2].type = INPUT_KEYBOARD;
          release[2].ki.wVk = VK_CONTROL;
          release[2].ki.dwFlags = KEYEVENTF_KEYUP;

          release[3].type = INPUT_KEYBOARD;
          release[3].ki.wVk = VK_LWIN;
          release[3].ki.dwFlags = KEYEVENTF_KEYUP;

          release[4].type = INPUT_KEYBOARD;
          release[4].ki.wVk = VK_LCONTROL;
          release[4].ki.dwFlags = KEYEVENTF_KEYUP;

          release[5].type = INPUT_KEYBOARD;
          release[5].ki.wVk = VK_RCONTROL;
          release[5].ki.dwFlags = KEYEVENTF_KEYUP;

          release[6].type = INPUT_KEYBOARD;
          release[6].ki.wVk = VK_LMENU;
          release[6].ki.dwFlags = KEYEVENTF_KEYUP;

          release[7].type = INPUT_KEYBOARD;
          release[7].ki.wVk = VK_RMENU;
          release[7].ki.dwFlags = KEYEVENTF_KEYUP;

          SendInput(8, release, sizeof(INPUT));
          Sleep(100);

          // performing the set script for action
          SendInput(4, inputs, sizeof(INPUT));

          // sleeps for some delays so operation actually occurs
          Sleep(100);

          // opening clipboard and locking it, and checking if clipboard opened
          // or not.
          if (OpenClipboard(NULL) == 1) {

            // getting text handle for the copied text
            HGLOBAL memory_handle = GetClipboardData(CF_TEXT);

            // getting address from that handle and as is void so casting it to
            // char ptr and storing it in.
            char *tempt = (char *)GlobalLock(memory_handle);

            // copying temp data
            strcpy(temptext, tempt);

            // freeing the clipboard
            GlobalUnlock(memory_handle);

            // closing the clipboard
            CloseClipboard();

            // Hotkey with no selected text check
            // if prev text in clipboard and the current text copied by hotkey are
            // the same, no need to do anything, retry is an option
            if (strcmp(temptext, PrevText) == 0)
              continue;
            break;
          }
        }
      }

      // the text first grabbed for making the file name and appending in file if
      // option is selected for file name
      // cout<<"RAW File Name & 1st append: "<<temptext<<endl;

      ///////////////////////////////
      // making the file name valid;
      ///////////////////////////////

      char filecpyname[50];
      // it will be used as a check to assist in removing the initial spaces from
      // the program

      int initail_space_check = 0;
      // append is the number of chars written in the filecpyname variable.
      int append = 0;

      // length of temptext.
      int len_temptext = 0;

      for (int i = 0; temptext[i] != '\0'; i++) {
        len_temptext++;
      }

      for (int i = 0; append < 49 && append < len_temptext; i++) {

        // ignoring the initial spaces
        if (temptext[i] != ' ' && temptext[i] != '\n') {
          initail_space_check = 1;
        }

        // It checks if the first character of temptext is a valid Windows
        // filename character.
        if (initail_space_check == 1 && temptext[i] >= 32 &&
            temptext[i] <= 126 && temptext[i] != 34 && temptext[i] != 42 &&
            temptext[i] != 47 && temptext[i] != 58 && temptext[i] != 60 &&
            temptext[i] != 62 && temptext[i] != 63 && temptext[i] != 92 &&
            temptext[i] != 124) {
          filecpyname[append] = temptext[i];
          append++;
        }
      }
      filecpyname[append] = '\0';

      strcat(pathtosave, filecpyname);
      strcat(pathtosave, ".txt");

      // FINAL FILE NAME wrt text
      // cout<<"Final File Name: "<<filecpyname<<endl;
    }

    char choice = 0;

    // check for setting the first copied text in txt file
    if (naming_option[0] == '1')
      choice = 1;

    // add struct will save the msg if any comes in queue, NULL to get the
    // message in the same thread
    while (GetMessage(&add, NULL, 0, 0)) {
      DispatchMessage(&add);
      // EXIT PROGRAM
      if (add.message == WM_HOTKEY && add.wParam == 0) {
        MessageBox(NULL, "Program Ended!", "NOTY", MB_OK);
        return 0;
      }

      // checking if the msg is for the required hotkey, wparam is id and message is
      // hotkey prop
      if (add.message == WM_HOTKEY && add.wParam == 1) {
        // path printer
        // cout<<"Path to save: "<<pathtosave<<endl;

        // creating the file.
        ofstream file(pathtosave, std::ios::app);

        // path error checker
        // cout<<"ERROR:"<<errno<<"++";

        // File successfully made and opened check
        if (!file.is_open()) {
          MessageBox(NULL, "Error in file formation! ", "NOTY", MB_OK);
        }

        // setting the buffered first copied text used for renaming
        if (choice) {
          if (endline[0] == '0') {
            if (bullets[0] == '1') {
              file << "• ";
            }
            // file<<temptext<<"pre 0";
            file << temptext;

            if (double_endline[0] == '1') {
              file << "\n";
            }
          }

          // writing with newline
          if (endline[0] == '1') {

            if (bullets[0] == '1') {
              file << "• ";
            }

            // file<<temptext<<"pre 1"<<"\n";
            file << temptext << "\n";

            if (double_endline[0] == '1') {
              file << "\n";
            }
          }
          choice = 0;
        }
        // Save current text in clipboard before copying, will then use it to
        // defend the exception when user presses hotkey without text selected
        char *PrevText = TextInClipBoard();
        // refocus
        // Ctrl+C action script forming in INPUT struct
        INPUT inputs[4] = {};

        // for pressing Ctrl
        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.wVk = VK_CONTROL;

        // for pressing C
        inputs[1].type = INPUT_KEYBOARD;
        inputs[1].ki.wVk = 'C';

        // for releasing Ctrl
        inputs[2].type = INPUT_KEYBOARD;
        inputs[2].ki.wVk = 'C';
        inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

        // for releasing C
        inputs[3].type = INPUT_KEYBOARD;
        inputs[3].ki.wVk = VK_CONTROL;
        inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

        // for releasing all the keys at time of sending the operation
        INPUT release[8] = {};

        release[0].type = INPUT_KEYBOARD;
        release[0].ki.wVk = VK_MENU;
        release[0].ki.dwFlags = KEYEVENTF_KEYUP;

        release[1].type = INPUT_KEYBOARD;
        release[1].ki.wVk = VK_SHIFT;
        release[1].ki.dwFlags = KEYEVENTF_KEYUP;

        release[2].type = INPUT_KEYBOARD;
        release[2].ki.wVk = VK_CONTROL;
        release[2].ki.dwFlags = KEYEVENTF_KEYUP;

        release[3].type = INPUT_KEYBOARD;
        release[3].ki.wVk = VK_LWIN;
        release[3].ki.dwFlags = KEYEVENTF_KEYUP;

        release[4].type = INPUT_KEYBOARD;
        release[4].ki.wVk = VK_LCONTROL;
        release[4].ki.dwFlags = KEYEVENTF_KEYUP;

        release[5].type = INPUT_KEYBOARD;
        release[5].ki.wVk = VK_RCONTROL;
        release[5].ki.dwFlags = KEYEVENTF_KEYUP;

        release[6].type = INPUT_KEYBOARD;
        release[6].ki.wVk = VK_LMENU;
        release[6].ki.dwFlags = KEYEVENTF_KEYUP;

        release[7].type = INPUT_KEYBOARD;
        release[7].ki.wVk = VK_RMENU;
        release[7].ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(8, release, sizeof(INPUT));
        Sleep(100);
        // performing the set script for action
        SendInput(4, inputs, sizeof(INPUT));
        // sleeps for some delays so operation actually occurs
        Sleep(100);
        // opening clipboard and locking it, and checking if clipboard opened or
        // not.
        OpenClipboard(NULL);

        // getting text handle for the copied text
        HGLOBAL memory_handle = GetClipboardData(CF_TEXT);

        // getting address from that handle and as is void so casting it to char
        // ptr and storing it in.
        char *text = (char *)GlobalLock(memory_handle);

        // Null Check for Text copied from the clipboard
        if (text == NULL) {
          continue;
        }
        // copying text to temp var;
        strcpy(temptext, text);

        // freeing the clipboard
        GlobalUnlock(memory_handle);

        // closing the clipboard
        CloseClipboard();

        // writing in file
        // If the selected text is same
        if (strcmp(PrevText, text) == 0) {
          sw_e_1 = 1;
          Create_Toast();

          continue;
        }

        // writing without newline
        if (endline[0] == '0') {

          // Display Toast Notification
          Create_Toast();

          if (bullets[0] == '1') {
            file << "• ";
          }

          // file<<text<<"main 0";
          file << text;

          if (double_endline[0] == '1') {
            file << "\n";
          }
        }

        // writing with newline
        if (endline[0] == '1') {

          // Display Toast Saved! Notification
          Create_Toast();

          if (bullets[0] == '1') {
            file << "• ";
          }

          // file<<text<<"main 1"<<"\n";
          file << text << "\n";

          // cout<<"\""<<text<<"\""<<endl;

          if (double_endline[0] == '1') {
            file << "\n";
          }
        }

        file.close();
      }

      // For making a new file
      if (add.message == WM_HOTKEY && add.wParam == 2) {
        sw_mkfile = 1;
        Create_Toast();
        // you cannot make the sw 0 here as the compiler will run this line before
        // running the wm paint case and so the sw case won't run, so make it
        // reset in the case
        PostQuitMessage(0);
      }
    }

    // Delete the file if it is empty and prog ended.
    ifstream file(pathtosave);
    file.seekg(0, ios::end);
    if (file.tellg() == 0) {
      DeleteFile(pathtosave);
    }
    file.close();
  }

  // IT ENDS
}

// Made by MUHAMMAD ARQAM GHAYOUR.
