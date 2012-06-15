#ifndef ASSERT_H
#define ASSERT_H

#if defined( WIN32 )
   #include "CustomAssertFunction.h"
#endif

#if defined( DEBUG ) && defined( WIN32 )
   static bool ignoreAll = false;
   bool CustomAssertFunction( bool, char*, int, char*, bool* );

   #define ASSERT_COND( expr, description )                                                                                  \
      do                                                                                                                \
      {                                                                                                                 \
         if ( !ignoreAll )                                                                                              \
         {                                                                                                              \
            if ( !(expr) )                                                                                              \
            {                                                                                                           \
               static bool ignoreThis = false;                                                                          \
               if ( !ignoreThis )                                                                                       \
               {                                                                                                        \
                  if ( CustomAssertFunction( (int)(expr), description, __LINE__, __FILE__, &ignoreThis, &ignoreAll ) )  \
                  {                                                                                                     \
                     __pragma(warning(push));                                                                           \
                     __pragma(warning(disable : 4793));                                                                 \
                     _asm { int 3 }                                                                                     \
                     __pragma(warning(pop));                                                                            \
                  }                                                                                                     \
               }                                                                                                        \
            }                                                                                                           \
         }                                                                                                              \
      }                                                                                                                 \
      while (0)

   #define ASSERT( description ) \
      ASSERT_COND(false, description)

   #define ASSERT_COND_EARLY_OUT( expr, description )                                                                              \
      do                                                                                                                \
      {                                                                                                                 \
         if ( !ignoreAll )                                                                                              \
         {                                                                                                              \
            if ( !(expr) )                                                                                              \
            {                                                                                                           \
               static bool ignoreThis = false;                                                                          \
               if ( !ignoreThis )                                                                                       \
               {                                                                                                        \
                  if ( CustomAssertFunction( (int)(expr), description, __LINE__, __FILE__, &ignoreThis, &ignoreAll ) )  \
                  {                                                                                                     \
                     __pragma(warning(push));                                                                           \
                     __pragma(warning(disable : 4793));                                                                 \
                     _asm { int 3 }                                                                                     \
                     __pragma(warning(pop));                                                                            \
                     return;                                                                                            \
                  }                                                                                                     \
               }                                                                                                        \
            }                                                                                                           \
         }                                                                                                              \
      }                                                                                                                 \
      while (0)

   #define ASSERT_COND_RETURN( expr, description, return_value )                                                         \                                                                                                             \
         if ( !ignoreAll )                                                                                              \
         {                                                                                                              \
            if ( !(expr) )                                                                                              \
            {                                                                                                           \
               static bool ignoreThis = false;                                                                          \
               if ( !ignoreThis )                                                                                       \
               {                                                                                                        \
                  if ( CustomAssertFunction( (int)(expr), description, __LINE__, __FILE__, &ignoreThis, &ignoreAll ) )  \
                  {                                                                                                     \
                     __pragma(warning(push));                                                                           \
                     __pragma(warning(disable : 4793));                                                                 \
                     _asm { int 3 }                                                                                     \
                     __pragma(warning(pop));                                                                            \
                     return return_value;                                                                               \
                  }                                                                                                     \
               }                                                                                                        \
            }                                                                                                           \
         }
#else
   #define ASSERT_COND( exp, description )
   #define ASSERT( description )
   #define ASSERT_COND_EARLY_OUT( exp, description )   \
         if(!(exp))                                   \
         {                                            \
            return;                                   \
         }
   #define ASSERT_COND_RETURN( exp, description, return_value )    \
      if(!(exp))                                                  \
      {                                                           \
         return return_value;                                     \
      }
#endif

#endif ASSERT_H
