#ifndef ASSERT_H
#define ASSERT_H

#if defined( WIN32 )
   #include "CustomAssertFunction.h"
#endif

#if defined( _DEBUG ) && defined( WIN32 )
   static bool ignoreAll = false;
   bool CustomAssertFunction( bool, char*, int, char*, bool* );

   #define Assert( expr, description ) \
   do { \
      if ( !ignoreAll ) \
      { \
         if ( !(expr) ) \
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

#endif ASSERT_H
