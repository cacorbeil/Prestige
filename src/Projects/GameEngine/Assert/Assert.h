#if defined( _DEBUG )
extern bool __stdcall CustomAssertFunction( bool, char*, int, char*, bool* );

//#pragma warning ( push )
//#pragma warning ( disable : 4793 )

   #define Assert( expr, description ) \
   do { \
      if ( !expr ) \
      { \
         static bool ignoreAlways = false; \
         if ( !ignoreAlways ) \
         { \
            if ( CustomAssertFunction( (int)(expr), description, __LINE__, __FILE__, &ignoreAlways ) ) \
            {  \
               __pragma(warning(push)); \
               __pragma(warning(disable : 4793)); \
               _asm { int 3 } \
               __pragma(warning(pop)); \
            } \
         } \
      } \
   } while (0)

//#pragma warning ( pop )

#else
   #define Assert( exp, description )
#endif
