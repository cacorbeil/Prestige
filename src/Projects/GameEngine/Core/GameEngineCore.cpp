#define DLL_EXPORT
#include "GameEngineCore.h"

#include <iostream>

namespace GameEngine
{
	GameEngineCore::GameEngineCore(GameEngine::GAME_LOOP_FUNC apGameLoopFunction)
		: mIsTerminated(false)
		, mFrameCount(0LL)
		, mpGameLoopFunction(apGameLoopFunction)
	{
	}

	void GameEngineCore::StartEngine()
	{
		//Engine Loop
		while(EngineIteration())
		{ } // NULL BODY
	}

	bool GameEngineCore::IsTerminated()
	{
		return mIsTerminated;
	}

	bool GameEngineCore::EngineIteration()
	{
#if DEBUG
		std::cout << "Begin debug frame " << Incrementator<IncrementatorTypeEnum::FRAME_ID>::GetIncrement();
#else
		std::cout << "Begin frame " << Incrementator<IncrementatorTypeEnum::FRAME_ID>::GetIncrement();		
#endif


		return mpGameLoopFunction();
	}

}