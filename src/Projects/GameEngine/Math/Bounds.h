#ifndef MATH_BOUNDS_H
#define MATH_BOUNDS_H

/// Mathematical functions
namespace Math
{
   /// Returns the max of any two objects
   /// \note The objects needs to have the > operator defined
   template<typename T>
   static T& Max(T& lhr, T& rhr)
   {
      return lhr > rhr ? lhr : rhr;
   }

   /// Returns the min of any two objects
   /// \note The objects needs to have the < operator defined
   template<typename T>
   static T& Min(T& lhr, T& rhr)
   {
      return lhr < rhr ? lhr : rhr;
   }
}

#endif MATH_BOUNDS_H