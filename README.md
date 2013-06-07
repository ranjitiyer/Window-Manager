WindowManager
=============
Window Manager for Windows XP. A tool that allows the user to move windows around on Windows XP using the keyboard. This capability is very useful to organize windows especially if there are many of them. For example, you want to type in a word processing program but also refer to a web page inside a browser application. Highlighting the top level window of the word processing program and hitting the combination WIN + LEFT ARROW should move the windows to the left section of the screen. Similarly the browser window can be moved to the right using the WIN + RIGHT ARROW key combination. This functionality is available in Windows 7 onwards but lacking in previous version of the Windows OS.

The project compiles into a WindowManager.exe and KeyboardHook.dll. Navigate into the folder where the EXE and the DLL resides and launch the WindowManager.exe. The program injects KeyboardHook.dll into all running processes that have a top level window.

THe KeyboardHook.dll contains functionality that hooks into keyboard messages and reacts to WIN + keys by moving the top level window around using the MoveWindow API.
