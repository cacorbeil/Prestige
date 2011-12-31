#include <Windows.h>

#define ASSERT_MESSAGE_SIZE 4096

bool __stdcall CustomAssertFunction( bool expr, char* description, int line, char* file, bool* ignoreAlways )
{
   static TCHAR assertMsg [ ASSERT_MESSAGE_SIZE ] ;

   wsprintf( assertMsg, TEXT("Debug Assertion Failed!\n\nFile : %hs\nLine : %d\nDescription : %hs\nValue : %d\n"), file, line, description, expr );

   int msgboxID = MessageBox(
        NULL,
        assertMsg,
        TEXT("Assertion failure..."),
        MB_ABORTRETRYIGNORE | MB_DEFBUTTON2 );
   
   switch ( msgboxID )
   {
   case IDABORT:
      ExitProcess(-1);
      break;
   case IDRETRY:
      return true; // debugger breaks
      break;
   case IDIGNORE:
      (*ignoreAlways) = true;
      break;
   }

   return false;
}
