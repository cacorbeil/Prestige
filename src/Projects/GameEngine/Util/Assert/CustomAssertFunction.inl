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
LPWORD lpwAlign( LPWORD lpIn )
{
   ULONG ul;

   ul = (ULONG)lpIn;
   ul ++;
   ul >>=1;
   ul <<=1;
   return (LPWORD)ul;
}

bool __stdcall DialogProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
   switch (uMsg)
   {
   case WM_INITDIALOG:
      return true;
      break;
   case WM_COMMAND:
      switch (LOWORD(wParam))
      {
      case ID_BREAK:
         EndDialog(hwndDlg, ID_BREAK);
         break;
      case ID_SKIP:
         EndDialog(hwndDlg, ID_SKIP);
         break;
      case ID_IGNORE:
         EndDialog(hwndDlg, ID_IGNORE);
         break;
      case ID_IGNOREALL:
         EndDialog(hwndDlg, ID_IGNOREALL);
         break;
      }
      return true;
      break;
   }

   return false;
}

bool __stdcall CustomAssertFunction( bool expr, char* description, int line, char* file, bool* ignoreThis, bool* ignoreAll )
{
   HGLOBAL hgDialogBox;
   LPDLGTEMPLATE dlgTemplate;
   LPDLGITEMTEMPLATE dlgItemTemplate;
   LPWORD lpw;

   char assertMsg[ASSERT_MESSAGE_SIZE];
   sprintf_s(assertMsg, ASSERT_MESSAGE_SIZE, "Assertion Failed!\n\nFile : %hs\nLine : %d\nDescription : %hs\nValue : %d\n", file, line, description, expr );

   // Gets the current Window information
   HWND hwndActiveWindow = GetActiveWindow();
   HINSTANCE hinstActiveWindow = (HINSTANCE)GetWindowLong(hwndActiveWindow, GWL_HINSTANCE);

   // Allocate memory for on-the-fly dialog box
   hgDialogBox = GlobalAlloc(GMEM_ZEROINIT, 1024);

   if (!hgDialogBox)
      return false;

   dlgTemplate = (LPDLGTEMPLATE) GlobalLock(hgDialogBox);

   // Defines our assert dialog box.
   //    typedef struct {
   //       DWORD style;            // Window/Dialog styles
   //       DWORD dwExtendedStyle;  // Extended Window styles
   //       WORD  cdit;             // The number of controls in the dialog box
   //       short x;                // Upper-left corner in dialog box units
   //       short y;                // Upper-left corner in dialog box units
   //       short cx;               // Width in dialog box units
   //       short cy;               // Height in dialog box units
   //    } DLGTEMPLATE, *LPDLGTEMPLATE;

   dlgTemplate->style = WS_BORDER | WS_POPUP | DS_CENTER | DS_MODALFRAME | WS_SYSMENU | WS_CAPTION; // Modal centered pop-up window with caption
   dlgTemplate->dwExtendedStyle = 0; // No extended styles
   dlgTemplate->cdit = 5;  // 4 buttons = Break / Skip / Ignore / IgnoreAll; 1 text control
   dlgTemplate->x = 10;    // X
   dlgTemplate->y = 10;    // Y
   dlgTemplate->cx = 195;  // WIDTH
   dlgTemplate->cy = 175;  // HEIGHT

   // Defines title
   lpw = (LPWORD)(dlgTemplate + 1);
   *lpw++ = 0; // No menu
   *lpw++ = 0; // Predefined dialog box class (by default)

   // LPW must be aligned, sometimes takes +1 with MultiByteToWideChar, sometimes not.
   lpw += MultiByteToWideChar(CP_ACP, 0, "Assertion Failed", -1, (LPWSTR)lpw, 128);

   // Defines Break button
   lpw = lpwAlign(lpw); // Align DLGITEMTEMPLATE on DWORD boundary
   dlgItemTemplate = (LPDLGITEMTEMPLATE) lpw;
   dlgItemTemplate->x  = 10;  // X
   dlgItemTemplate->y  = 150; // Y
   dlgItemTemplate->cx = 40;  // WIDTH
   dlgItemTemplate->cy = 15;  // HEIGHT
   dlgItemTemplate->id = ID_BREAK;
   dlgItemTemplate->style = WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON;

   lpw = (LPWORD)(dlgItemTemplate + 1);
   *lpw++ = 0xFFFF;
   *lpw++ = 0x0080; // Button class

   lpw += MultiByteToWideChar(CP_ACP, 0, "Break", -1, (LPWSTR)lpw, 128);
   *lpw++ = 0; // No creation data

   // Defines Skip button
   lpw = lpwAlign(lpw); // Align DLGITEMTEMPLATE on DWORD boundary
   dlgItemTemplate = (LPDLGITEMTEMPLATE) lpw;
   dlgItemTemplate->x  = 55;  // X
   dlgItemTemplate->y  = 150; // Y
   dlgItemTemplate->cx = 40;  // WIDTH
   dlgItemTemplate->cy = 15;  // HEIGHT
   dlgItemTemplate->id = ID_SKIP;
   dlgItemTemplate->style = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;

   lpw = (LPWORD)(dlgItemTemplate + 1);
   *lpw++ = 0xFFFF;
   *lpw++ = 0x0080; // Button class

   lpw += 1 + MultiByteToWideChar(CP_ACP, 0, "Skip", -1, (LPWSTR)lpw, 128);
   *lpw++ = 0; // No creation data

   // Defines Ignore button
   lpw = lpwAlign(lpw); // Align DLGITEMTEMPLATE on DWORD boundary
   dlgItemTemplate = (LPDLGITEMTEMPLATE) lpw;
   dlgItemTemplate->x  = 100; // X
   dlgItemTemplate->y  = 150; // Y
   dlgItemTemplate->cx = 40;  // WIDTH
   dlgItemTemplate->cy = 15;  // HEIGHT
   dlgItemTemplate->id = ID_IGNORE;
   dlgItemTemplate->style = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;

   lpw = (LPWORD)(dlgItemTemplate + 1);
   *lpw++ = 0xFFFF;
   *lpw++ = 0x0080; // Button class

   lpw += 1 + MultiByteToWideChar(CP_ACP, 0, "Ignore", -1, (LPWSTR)lpw, 128);
   *lpw++ = 0; // No creation data

   // Defines IgnoreAll button
   lpw = lpwAlign(lpw); // Align DLGITEMTEMPLATE on DWORD boundary
   dlgItemTemplate = (LPDLGITEMTEMPLATE) lpw;
   dlgItemTemplate->x  = 145; // X
   dlgItemTemplate->y  = 150; // Y
   dlgItemTemplate->cx = 40;  // WIDTH
   dlgItemTemplate->cy = 15;  // HEIGHT
   dlgItemTemplate->id = ID_IGNOREALL;
   dlgItemTemplate->style = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;

   lpw = (LPWORD)(dlgItemTemplate + 1);
   *lpw++ = 0xFFFF;
   *lpw++ = 0x0080; // Button class

   lpw += MultiByteToWideChar(CP_ACP, 0, "IgnoreAll", -1, (LPWSTR)lpw, 128);
   *lpw++ = 0; // No creation data

   // Defines text control
   lpw = lpwAlign(lpw); // Align DLGITEMTEMPLATE on DWORD boundary
   dlgItemTemplate = (LPDLGITEMTEMPLATE) lpw;
   dlgItemTemplate->x  = 10;
   dlgItemTemplate->y  = 10;
   dlgItemTemplate->cx = 155;
   dlgItemTemplate->cy = 130;
   dlgItemTemplate->id = ID_TEXT; // Text identifier
   dlgItemTemplate->style = WS_CHILD | WS_VISIBLE | SS_LEFT;

   lpw = (LPWORD)(dlgItemTemplate + 1);
   *lpw++ = 0xFFFF;
   *lpw++ = 0x0082; // Static class

   lpw += MultiByteToWideChar (CP_ACP, 0, assertMsg, -1, (LPWSTR)lpw, 512);
   *lpw++ = 0; // No creation data

   GlobalUnlock(dlgTemplate);

   // Creates the dialog box
   LRESULT ret = DialogBoxIndirect(hinstActiveWindow, 
                     (LPDLGTEMPLATE) dlgTemplate, 
                     hwndActiveWindow, 
                     (DLGPROC) DialogProc); 

   GlobalFree(dlgTemplate);

   // Verify return value
   switch (ret)
   {
   case -1:
      MessageBox(NULL, TEXT("Could not create Assertion Window."), TEXT("Assertion failure..."), MB_OK );
      break;
   case ID_BREAK:
      return true; // debugger breaks
      break;
   case ID_SKIP:
      break;
   case ID_IGNORE:
      *ignoreThis = true;
      break;
   case ID_IGNOREALL:
      *ignoreAll = true;
      break;
   }

   return false;
}
