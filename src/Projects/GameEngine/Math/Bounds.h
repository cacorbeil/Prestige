#ifndef MATH_BOUNDS_H
#define MATH_BOUNDS_H

namespace Math
{
   template<typename T>
   static T& Max(T& lhr, T& rhr)
   {
      return lhr > rhr ? lhr : rhr;
   }

   template<typename T>
   static T& Min(T& lhr, T& rhr)
   {
      return lhr < rhr ? lhr : rhr;
   }

   template<int>
   static int& Min(int& lhr, int& rhr)
   {
      return r = lhr ^ ((rhr ^ lhr) & -(rhr < lhr));
   }

}

#endif MATH_BOUNDS_H