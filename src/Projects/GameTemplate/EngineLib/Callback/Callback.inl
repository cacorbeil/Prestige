#ifdef CALLBACK_H

namespace Util
{
   template <typename Return, typename Param1>
   Callback<Return, Param1>::BaseCallback::~BaseCallback() { }

   template <typename Return, typename Param1>
   Return Callback<Return, Param1>::BaseCallback::operator()(Param1) { }

   template <typename Return, typename Param1>
   Callback<Return, Param1>::GlobalCallback::GlobalCallback(Func aFunc) : mFunc(aFunc) { }

   template <typename Return, typename Param1>
   Return Callback<Return, Param1>::GlobalCallback::operator()(Param1 arg1)
   {
      return mFunc(arg1);
   }

   template <typename Return, typename Param1>
   Callback<Return, Param1>::Callback(const Callback<Return, Param1>&) {};
   template <typename Return, typename Param1>
   Callback<Return, Param1>& Callback<Return, Param1>::operator=(const Callback<Return, Param1>&) { return *this; } //Should not be defined (static assert)
}

#endif CALLBACK_H