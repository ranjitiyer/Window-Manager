// HookInstaller.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include <string>

using namespace std;

typedef LRESULT (CALLBACK *FnKeyboardProc) (int code, WPARAM wParam, LPARAM lParam);
FnKeyboardProc fnKeyboardProc = 0;

HHOOK hookProc;

void PrintUsage()
{
  cout << endl;
  cout << "WindowManager is a tool designed for Windows XP that allows you to precisely position application windows on the screen to simply working with multiple running applications." << endl << endl;
  cout << "Press the 'Windows' key (typically found between the Ctrl and Alt keys on the keyboard) and then an suitable 'Arrow' key to move windows around on the screen." << endl << endl;

  cout << "An application starts out in the 'Restored' position (it's position when it was last closed) after which it can be moved to the Right, Right-top, Right-bottom, ";
  cout << "Left, Left-top, Left-bottom and can be Maximized and Minimized." << endl << endl;
  
  cout << "WindowManager [OPTION]" << endl;
  cout << "\t -start : Starts WindowManager." << endl;
}


void RegisterKeyboardHook() {
  HMODULE hLibrary = LoadLibraryA ("KeyboardHook.dll");
  if (hLibrary) {
    fnKeyboardProc = (FnKeyboardProc) GetProcAddress (hLibrary, "KeyboardProc");
    if (fnKeyboardProc) { 
      hookProc = SetWindowsHookEx (WH_KEYBOARD, fnKeyboardProc, hLibrary, 0);
      if (hookProc) 
				printf("WindowManager started...\n");
    }
    else {
      MessageBox (NULL, L"Unable to obtain proc address", L"Error", MB_ICONERROR);
    }
  }
  else
	{
    MessageBox (NULL, L"Unable to load library", L"Error", MB_ICONERROR);
  }
}

void UnregisterKeyboardHook() {
  HMODULE hLibrary = LoadLibraryA ("KeyboardHook.dll");
  if (hLibrary) {
    fnKeyboardProc = (FnKeyboardProc) GetProcAddress (hLibrary, "KeyboardProc");
    if (fnKeyboardProc) { 
			BOOL flag = UnhookWindowsHookEx(hookProc);
		  if (flag)
			  printf ("WindowManager stopped...\n");
    }
    else {
      MessageBox (NULL, L"Unable to obtain proc address", L"Error", MB_ICONERROR);
    }
  }
  else
	{
    MessageBox (NULL, L"Unable to load library", L"Error", MB_ICONERROR);
  }
}



int _tmain(int argc, _TCHAR* argv[])
{
  if(argc <= 1) 
    PrintUsage();
  else if(0 == _tcscmp(argv[1],L"-start")) {
    RegisterKeyboardHook();
    cout << "To stop WindowManager type 'stop' " << endl;

    string input = "";
    bool didntGetit = true;

    while (didntGetit) {    
      getline(cin, input);
      if (input.compare(string("stop")) == 0) {
        UnregisterKeyboardHook();
        break;
      }
      else {
        cout << "Try again" << endl;
      }
    }
  }
  else 
    PrintUsage();
  
  return 1;
}



