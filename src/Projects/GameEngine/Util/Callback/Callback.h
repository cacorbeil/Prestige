#ifndef CALLBACK_H
#define CALLBACK_H

namespace Util
{
   /// Function call back, can be member function or global function
   /// \tparam Return Type of return parameter
   /// \tparam Arg1 Type of the first parameter
   /// \note If the need arise to have more parameter, a copy of this call would be needed with more ParamX template parameter
   /// \todo Support for static member callback
   template <typename Return, typename Param1>
   class Callback
   {
   public:
      /// This is the core of the callback
      class BaseCallback
      {
      public:
         // Required for polymorphic destruction to work properly.
         virtual ~BaseCallback() {}

         /// Base virtual call operator.
         /// This is where the actual callback is called
         /// \note If the need arise to have more parameter, a copy of this call would be needed with more ParamX
         ///   template parameter
         virtual Return operator()(Param1) {}
      };

      /// Callback that is member of an instance of a class
      /// \tparam Class The class the callback is a member of
      template <typename Class>
      class MemberCallback : public BaseCallback
      {
      public:
         typedef Return (Class::*Func)(Param1); ///< Function pointer definition of the callback.

         /// \param aThis The object owner of the callback
         /// \param aFunc The function pointer of the callback
         MemberCallback(Class* aThis, Func aFunc) : mThis(aThis), mFunc(aFunc) { }

         /// Invoke the callback throught the pointer-to-member.
         virtual Return operator()(Param1 arg1) { return (mThis->*mFunc)(arg1); }

      private:
         Class* mThis; ///< Pointer to the object we are member of.
         Func   mFunc; ///< Address of the function on the object.
      };

      /// Callback that is global
      class GlobalCallback : public BaseCallback
      {
      public:
         typedef Return (*Func)(Param1); ///< Function pointer definition of the callback.

         /// \param aFunc The function pointer of the callback 
         GlobalCallback(Func aFunc);

         /// Invoke the function
         virtual Return operator()(Param1 arg1);

      private:
         Func mFunc; ///< Address of the function.
      };

   private:
      // Can't allow copying unless we modify the class to make a deep
      // copy of each of the pointers it holds. Base class would need 
      // a virtual Clone() method to allow polymorphic copying.
      Callback(const Callback&);
      Callback& operator=(const Callback&);
   };
}

#include "Callback.inl"

template class DLL_DECL Util::Callback<void, float>;
template class DLL_DECL Util::Callback<void, float>::BaseCallback;
template class DLL_DECL Util::Callback<void, float>::GlobalCallback;

#endif CALLBACK_H