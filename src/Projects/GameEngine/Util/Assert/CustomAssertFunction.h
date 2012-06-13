#ifdef ASSERT_H

#include <Windows.h>
#include <stdio.h>

#define ASSERT_MESSAGE_SIZE 512

#define ID_BREAK        500
#define ID_SKIP         501
#define ID_IGNORE       502
#define ID_IGNOREALL    503
#define ID_TEXT         550

// CustomAssertFunction based off Microsoft Example
// URL("http://msdn.microsoft.com/en-us/library/ms644996(VS.85).aspx#template_in_memory")

LPWORD lpwAlign( LPWORD lpIn );

bool __stdcall DialogProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
bool __stdcall CustomAssertFunction( bool expr, char* description, int line, char* file, bool* ignoreThis, bool* ignoreAll );

#include "CustomAssertFunction.inl"

#endif ASSERT_H