#ifndef INCREMENTATOR_H
#define INCREMENTATOR_H

typedef unsigned int IncrementatorType;

template<IncrementatorType>
struct Incrementator
{
   static unsigned int GetIncrement()
   {
      static unsigned int sequence = 0;
      return sequence++;
   }
};

#define STATIC_INCREMENTATOR(Name, Value) static_cast<IncrementatorType>(Value)


#endif INCREMENTATOR_H