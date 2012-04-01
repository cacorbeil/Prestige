#include "IncrementatorEnum.h"

#ifndef INCREMENTATOR_H
#define INCREMENTATOR_H

namespace Util
{
   /// The type of the increment value
   typedef unsigned int IncrementatorType;

   /// Manage a simple increment
   /// \tparam IncrementatorType An identifier for the incrementator, by re-using this identifier 
   ///   you'll share the incrementation continuity
   template<IncrementatorType>
   struct Incrementator
   {
      /// \returns An incremented number that is unique to the identifier given in the template
      static unsigned int GetIncrement()
      {
         static unsigned int sequence = 0;
         return sequence++;
      }
   };
}

#endif INCREMENTATOR_H