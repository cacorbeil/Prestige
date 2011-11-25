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

#endif INCREMENTATOR_H