#include "CustomAssertFunction.inl"

static bool ignoreAll = false;

#if defined( _DEBUG ) && defined( WIN32 )
   extern bool __stdcall CustomAssertFunction( bool, char*, int, char*, bool* );
   #define Assert( expr, description ) \
   do { \
      if ( !ignoreAll ) \
      { \
         if ( !expr ) \
         { \
            static bool ignoreThis = false; \
            if ( !ignoreThis ) \
            { \
               if ( CustomAssertFunction( (int)(expr), description, __LINE__, __FILE__, &ignoreThis, &ignoreAll ) ) \
               {  \
                  __pragma(warning(push)); \
                  __pragma(warning(disable : 4793)); \
                  _asm { int 3 } \
                  __pragma(warning(pop)); \
               } \
            } \
         } \
      } \
   } while (0)
#else
   #define Assert( exp, description )
#endif
