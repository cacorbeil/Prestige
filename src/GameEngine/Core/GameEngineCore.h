#ifndef GAME_ENGINE_CORE_H
#define GAME_ENGINE_CORE_H

#include "../Macros/DLL.h"

#include "Incrementator.h"

namespace GameEngineCore
{

	namespace IncrementatorTypeEnum
	{
		enum Enum
		{
			FRAME_ID = 0,

			//Use this enum to as your first value in your game IncrementatorType enum
			INCREMENTOR_GAME_FOLLOW_UP, 
		};
	}

	template <class GAME_LOOP_FUNCTOR>
	class DllExport GameEngineCore
	{
	public:
		GameEngineCore()
			: mIsTerminated(false)
			, mFrameCount(0LL)
			, mGameLoopFunctor()
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
			std::cout << "Begin frame " << Incrementator<IncrementatorTypeEnum::FRAME_ID>::GetIncrement();

			return mGameLoopFunctor();
		}

	private:
		bool mIsTerminated;
		long long mFrameCount;
		GAME_LOOP_FUNCTOR mGameLoopFunctor;
	};
}

#endif //GAME_ENGINE_CORE_H
