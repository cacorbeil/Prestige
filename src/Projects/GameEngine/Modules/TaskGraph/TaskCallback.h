#ifndef TASK_CALLBACK_H
#define TASK_CALLBACK_H

template <typename Return, typename Arg1>
class TaskCallback
{
  public:
    // Base class - specify the signature.
    class Base
    {
      public:
         // Required for polymorphic destruction to work properly.
         virtual ~Base() { }

         // Target signature. Invoked through base class pointers.
         virtual Return operator()(Arg1) { }
    };
    
  public:
    // Derived class - parametrise the implementation so we can specify the 
    // target class in client code. 
    template <typename Class>
    class MemberCallback : public Base
    {
    public:
      // Signature applied to a pointer-to-member for target class.
      typedef Return (Class::*Func)(Arg1);

      private:
        Class* mThis; // Pointer to the object we are delegating to.
        Func   mFunc; // Address of the function on the delegate object.

      public:
         // TODO support for static callback
        MemberCallback(Class* aThis, Func aFunc) : mThis(aThis), mFunc(aFunc) { }
     
        // Invoke the function throught the pointer-to-member.
        virtual Return operator()(Arg1 arg1)
        {
          return (mThis->*mFunc)(arg1);
        }
    };

   public:  
  
    // Derived class - this is not parametrised and is intended for 
    // static and free functions.
    class Callback : public Base
    {
      typedef Return (*Func)(Arg1);

      private:
        Func mFunc; 

      public:
        Callback(Func aFunc) : mFunc(aFunc) { }
      
        // Invoke the function throught the pointer-to-member.
        virtual Return operator()(Arg1 arg1)
        {
          return mFunc(arg1);
        }
    };

  private:
    // Can't allow copying unless we modify the class to make a deep
    // copy of each of the pointers it holds. Base class would need 
    // a virtual Clone() method to allow polymorphic copying.
     TaskCallback(const TaskCallback&) {}; //Should not be defined (static assert)
     TaskCallback& operator=(const TaskCallback&) { return *this; }; //Should not be defined (static assert)
};

#endif TASK_CALLBACK_H