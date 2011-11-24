#ifndef GAME_ENGINE_CORE_H
#define GAME_ENGINE_CORE_H

#include "../Macros/DLL.h"

template <class GAME_LOOP_FUNCTOR>
class DllExport GameEngineCore
{
public:
	GameEngineCore(GAME_LOOP_FUNCTOR aGameLoopFunctor) :
		mIsTerminated(false) ,
		mFrameCount(0LL),
		mGameLoopFunctor(aGameLoopFunctor)
	{
	}

	void StartEngine()
	{
		//Engine Loop
		while(EngineIteration())
		{ } // NULL BODY
	}

	inline bool IsTerminated()
	{
		return mIsTerminated;
	}

protected:
	bool GameEngineCore::EngineIteration()
	{
		std::cout << ".";

		return mGameLoopFunctor();
	}

private:
	bool mIsTerminated;
	long long mFrameCount;
	GAME_LOOP_FUNCTOR mGameLoopFunctor;
};

#endif //GAME_ENGINE_CORE_H
