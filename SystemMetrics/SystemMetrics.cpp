// SystemMetrics.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <conio.h>
#include <stdio.h>

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	printf ("Monitor        : %d\n", hMonitor);

	MONITORINFO mi;
	mi.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(hMonitor, &mi);

	RECT rcMonitor = mi.rcMonitor;
	RECT rcWork		 = mi.rcWork;

	printf ("Monitor virtual xmin : %d\n", rcMonitor.left);
	printf ("Monitor virtual ymin : %d\n", rcMonitor.top);
	printf ("Monitor virtual xmax : %d\n", rcMonitor.right);
	printf ("Monitor virtual ymax : %d\n", rcMonitor.bottom);

	printf ("Monitor working area virtual xmin : %d\n", rcWork.left);
	printf ("Monitor working area virtual ymin : %d\n", rcWork.top);
	printf ("Monitor working area virtual xmax : %d\n", rcWork.right);
	printf ("Monitor working area virtual ymax : %d\n\n", rcWork.bottom);

	//printf ("Monitor top    : %d \n", lprcMonitor->top);
	//printf ("Monitor left   : %d \n", lprcMonitor->left);
	//printf ("Monitor bottom : %d \n", lprcMonitor->bottom);
	//printf ("Monitor right  : %d \n", lprcMonitor->right);
	return TRUE;
}


typedef struct myTagRECT
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
} MyRECT;

int _tmain(int argc, _TCHAR* argv[])
{
	MyRECT rect;
	rect.top = 1;
	rect.left = 2;
	rect.bottom = 3;
	rect.right = 4;

	MyRECT rect1 = rect;
	int dispMonitors = GetSystemMetrics (SM_CMONITORS);
	int screenWidth = GetSystemMetrics (SM_CXFULLSCREEN);
	int screenHeight = GetSystemMetrics (SM_CYFULLSCREEN);
	int primaryMonitorWidth = GetSystemMetrics (SM_CXSCREEN);
	int primaryMonitorHeight = GetSystemMetrics (SM_CYSCREEN);
	int virtScreenBBOXWidth = GetSystemMetrics (SM_CXVIRTUALSCREEN);
	int virtScreenBBOXHeight = GetSystemMetrics (SM_CYVIRTUALSCREEN);
	int leftVirtScreenBBOXWidth = GetSystemMetrics (SM_XVIRTUALSCREEN);	
	int leftVirtScreenBBOXHeight = GetSystemMetrics (SM_YVIRTUALSCREEN);	
	int maxHeightFramePrimaryScreen = GetSystemMetrics (SM_CYMAXIMIZED);

	printf ("Display Monitors             =	%d \n", dispMonitors);
	printf ("Full screen width            = %d \n", screenWidth);
	printf ("Full screen height           = %d \n", screenHeight);
	printf ("Primary monitor width        = %d \n", primaryMonitorWidth);
	printf ("Primary monitor height       = %d \n", primaryMonitorHeight);
	printf ("Virt screen BBOX width       = %d \n", virtScreenBBOXWidth);
	printf ("Virt screen BBOX height      = %d \n", virtScreenBBOXHeight);
	printf ("Left Virt screen BBOX width  = %d \n", leftVirtScreenBBOXWidth);
	printf ("Left Virt screen BBOX height = %d \n", leftVirtScreenBBOXHeight);
	printf ("Max height on primary screen = %d \n\n", maxHeightFramePrimaryScreen);
	
	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0);		

	int i = 0;
	scanf("%d", i);
}

