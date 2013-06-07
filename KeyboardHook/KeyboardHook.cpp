// KeyboardHook.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <stdlib.h>
#include "stdio.h"
#include "KeyboardHook.h"
#include "windows.h"
#include "Commctrl.h"

////////////////////////////////////
// 
//  Globals
//
////////////////////////////////////

enum WIN_POSITION {
  LEFT,
  RIGHT,
  MAXIMIZED,
  MINIMIZED,
  RESTORED,
  LEFT_UP,
  LEFT_DOWN,
  RIGHT_UP,
  RIGHT_DOWN
};

RECT left;
RECT right;
RECT left_up;
RECT left_down;
RECT right_up;
RECT right_down;
RECT restored;

int numMonitors = 1;

UINT prevKey = 0;
bool prevKeyDown = false;

HWND topLevelWnd = NULL;
BOOL CALLBACK EnumWindowsProc(HWND topHwnd, LPARAM lParam) {
  if (IsChild(topHwnd, (HWND)lParam)) {
    topLevelWnd = topHwnd;
		return FALSE;
  }
  return TRUE;
}
HWND GetTopLevelWindow(HWND focusWnd)
{
	EnumWindows (EnumWindowsProc, (LPARAM)focusWnd);
	return topLevelWnd;
}

void MoveRight(HWND topWnd) {
  MoveWindow (topWnd, right.left, right.top, right.right-right.left, right.bottom-right.top, TRUE);
}

void MoveRightUp(HWND topWnd) {
  MoveWindow (topWnd, right_up.left, right_up.top, right_up.right-right_up.left, right_up.bottom-right_up.top, TRUE);
}

void MoveRightDown(HWND topWnd) {
  MoveWindow (topWnd, right_down.left, right_down.top, right_down.right-right_down.left, right_down.bottom-right_down.top, TRUE);
}

void MoveLeft(HWND topWnd) {
  MoveWindow (topWnd, left.left, left.top, left.right-left.left, left.bottom-left.top, TRUE);
}

void MoveLeftUp(HWND topWnd) {
  MoveWindow (topWnd, left_up.left, left_up.top, left_up.right-left_up.left, left_up.bottom-left_up.top, TRUE);
}

void MoveLeftDown(HWND topWnd) {
  MoveWindow (topWnd, left_down.left, left_down.top, left_down.right-left_down.left, left_down.bottom-left_down.top, TRUE);
}

void MaximizeWindow(HWND topWnd) {
  ShowWindow(topWnd, SW_MAXIMIZE);
}

void MinimizeWindow(HWND topWnd) {
  ShowWindow(topWnd, SW_MINIMIZE);
}

void RestoreWindow(HWND topWnd) {   
  MoveWindow (topWnd, restored.left, restored.top, restored.right-restored.left, restored.bottom-restored.top, TRUE);
}


/**
*  Detects the Win+Right-Arrow key combination
*/
bool MOVE_RIGHT(WPARAM wParam, bool keyDown)
{
  if (wParam == VK_RIGHT && keyDown) 
  	if (prevKey == VK_LWIN && prevKeyDown)
      return true;
  return false;
}

/**
*  Detects the Win+Left-Arrow key combination
*/
bool MOVE_LEFT(WPARAM wParam, bool keyDown)
{
  if (wParam == VK_LEFT && keyDown) 	
    if (prevKey == VK_LWIN && prevKeyDown) 
      return true;
  return false;
}

/**
*  Detects the Win+Up-Arrow key combination
*/
bool MOVE_UP(WPARAM wParam, bool keyDown)
{
  if (wParam == VK_UP && keyDown) 	
    if (prevKey == VK_LWIN && prevKeyDown) 
      return true;
  return false;
}

/**
*  Detects the Win+Down-Arrow key combination
*/
bool MOVE_DOWN(WPARAM wParam, bool keyDown)
{
  if (wParam == VK_DOWN && keyDown) 	
    if (prevKey == VK_LWIN && prevKeyDown) 
      return true;
  return false;
}

/**
* Return the current position of the window on the screen
*/
int GetWindowPos(HWND hWnd, RECT rect) {
  if (IsIconic(hWnd))
    return MINIMIZED;
  else if (IsZoomed(hWnd))
    return MAXIMIZED;
  else if (rect.left == left.left && rect.right == left.right) {
    if (rect.top == left.top && rect.bottom == left.bottom)
      return LEFT;
    else if (rect.top == left_up.top && rect.bottom == left_up.bottom)
      return LEFT_UP;
    else if (rect.top == left_down.top && rect.bottom == left_down.bottom)
      return LEFT_DOWN;
  }
  else if (rect.left == right.left && rect.right == right.right) {
    if (rect.top == right.top && rect.bottom == right.bottom)
      return RIGHT;
    else if (rect.top == right_up.top && rect.bottom == right_up.bottom)
      return RIGHT_UP;
    else if (rect.top == right_down.top && rect.bottom == right_down.bottom)
      return RIGHT_DOWN;
  }
  restored = rect;
  return RESTORED;
}

/**
* Keyboard hook procedure that detects Win+Array keystores and moves the window
* on the screen.
*/
__declspec(dllexport) LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam) {
  if (code >= 0) {
    // Should be a better way to test if this is a keydown
    char buffer[33];
    bool keyDown = true;
    _ltoa_s(lParam, buffer, 33, 2);

    // According to MSDN, if the 32'nd bit is on then the key is DOWN, 
    // but I see the opposite.
    if (strlen (buffer) == 32)
      keyDown = false;

		HWND focusWnd		= GetFocus();
		HWND topWnd			= GetTopLevelWindow(focusWnd);

		RECT topWndRect;		
		GetWindowRect(topWnd, &topWndRect);

		if (MOVE_RIGHT(wParam, keyDown)) {
      WIN_POSITION pos = (WIN_POSITION) GetWindowPos(topWnd, topWndRect);
      if (pos == LEFT || pos == LEFT_UP || pos == LEFT_DOWN) {
        RestoreWindow(topWnd);
      }
      else if (pos == RESTORED) {
        MoveRight(topWnd);
      }
      else if (pos == MAXIMIZED) {
        ShowWindow(topWnd, SW_RESTORE);
        MoveRight(topWnd);
      }
    }
    else if (MOVE_LEFT(wParam, keyDown)) {
      WIN_POSITION pos = (WIN_POSITION) GetWindowPos(topWnd, topWndRect);
      if (pos == RIGHT || pos == RIGHT_UP || pos == RIGHT_DOWN) {
        RestoreWindow(topWnd);
      }
      else if (pos == RESTORED) {
        MoveLeft(topWnd);
      }
      else if (pos == MAXIMIZED) {
        ShowWindow(topWnd, SW_RESTORE);
        MoveLeft(topWnd);
      }
    }
    else if (MOVE_UP(wParam, keyDown)) {
      WIN_POSITION pos = (WIN_POSITION) GetWindowPos(topWnd, topWndRect);
      if (pos == RESTORED || pos == LEFT_UP || pos == RIGHT_UP) {
        MaximizeWindow(topWnd);
      }
      else if (pos == LEFT_DOWN) {
        MoveLeft(topWnd);
      }
      else if (pos == LEFT) {
        MoveLeftUp(topWnd);
      }
      else if (pos == RIGHT_DOWN) {
        MoveRight(topWnd);
      } 
      else if (pos == RIGHT) {
        MoveRightUp(topWnd);
      }
      else if (pos == MINIMIZED) {
        ShowWindow(topWnd, SW_RESTORE);
      }
    }
    else if (MOVE_DOWN(wParam, keyDown)) {
      WIN_POSITION pos = (WIN_POSITION) GetWindowPos(topWnd, topWndRect);
      if (pos == LEFT) {
        MoveLeftDown(topWnd);
      }
      else if (pos == LEFT_UP) {
        MoveLeft(topWnd);
      }
      else if (pos == RIGHT) {
        MoveRightDown(topWnd);
      }
      else if (pos == RIGHT_UP) {
        MoveRight(topWnd);
      }
      else if (pos == MAXIMIZED) {
        ShowWindow(topWnd, SW_RESTORE);
      }
      else if (pos == RESTORED || pos == RIGHT_DOWN || pos == LEFT_DOWN) {
        MinimizeWindow(topWnd);
      }
    }
    prevKey = (UINT) wParam;
    prevKeyDown = keyDown;    
  }
  return CallNextHookEx (NULL, code, wParam, lParam);
}

void initRects(int screenWidth, int screenHeight) {
  left.left   = 0;
  left.right  = screenWidth/2;
  left.top    = 0;
  left.bottom = screenHeight;

  left_up.left    = 0;
  left_up.right   = screenWidth/2;
  left_up.top     = 0;
  left_up.bottom  = screenHeight/2;

  left_down.left   = 0;
  left_down.right  = screenWidth/2;
  left_down.top    = screenHeight/2;
  left_down.bottom = screenHeight;

  right.left    = screenWidth/2;
  right.right   = screenWidth;
  right.top     = 0;
  right.bottom  = screenHeight;

  right_up.left   = screenWidth/2;
  right_up.right  = screenWidth;
  right_up.top    = 0;
  right_up.bottom = screenHeight/2;

  right_down.left   = screenWidth/2;
  right_down.right  = screenWidth;
  right_down.top    = screenHeight/2;
  right_down.bottom = screenHeight;

  restored.left     = left.right/2;
  restored.right    = right.left + (right.right-right.left)/2;
  restored.top      = 0;
  restored.bottom   = screenHeight;
}

UINT_PTR uIdSubclass;
DWORD dwRefData = 0;
bool subclassRegistered = false;
LRESULT fnSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam,
  LPARAM lParam,
  UINT_PTR uIdSubclass,
  DWORD_PTR dwRefData) {
   switch (uMsg)
    {
    case WM_SETTINGCHANGE:        
      initRects(GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYMAXIMIZED));
      return TRUE;
    } 
    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
      uIdSubclass = (UINT_PTR) GetProcessId(GetCurrentProcess());
      GUITHREADINFO guiInfo = {0};
      guiInfo.cbSize = sizeof(GUITHREADINFO);
      GetGUIThreadInfo(GetCurrentThreadId(), &guiInfo);
      if (guiInfo.hwndFocus) {
        HWND topWnd = GetTopLevelWindow(guiInfo.hwndFocus);      
        if (SetWindowSubclass(topWnd, (SUBCLASSPROC) fnSubclassProc, uIdSubclass, dwRefData) == TRUE) {
          subclassRegistered = true;
        }
      }

      // Get monitor metrics
			numMonitors = GetSystemMetrics (SM_CMONITORS);
			if (numMonitors > 1) 
			{
        MessageBox(NULL, L"Error", L"Only one monitor supported at this time!", MB_ICONERROR);
				return FALSE;	
			}

      initRects(GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYMAXIMIZED));
      
			break;
		}
	case DLL_PROCESS_DETACH:
    {
      if (subclassRegistered) {
        RemoveWindowSubclass(topLevelWnd, (SUBCLASSPROC) fnSubclassProc, uIdSubclass);
      }
    }
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:	
		break;
	}
	return TRUE;
}
