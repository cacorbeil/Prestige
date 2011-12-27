#define DLL_EXPORT
#include "GameEngineCore.h"

#include <iostream>
#include <iomanip>

   DISTRIBUTED_ENUM(FIRST_DISTRIBUTED_ENUM, NEW_VALUEA);
   DISTRIBUTED_ENUM(FIRST_DISTRIBUTED_ENUM, NEW_VALUEB);
   
   DISTRIBUTED_ENUM(FIRST_DISTRIBUTED_ENUM, NEW_VALUEC);
   DISTRIBUTED_ENUM(FIRST_DISTRIBUTED_ENUM, NEW_VALUED); 
   DISTRIBUTED_ENUM(FIRST_DISTRIBUTED_ENUM, NEW_VALUEE); 
   DISTRIBUTED_ENUM(FIRST_DISTRIBUTED_ENUM, NEW_VALUEK); 
   DISTRIBUTED_ENUM(FIRST_DISTRIBUTED_ENUM, NEW_VALUEJ);
   DISTRIBUTED_ENUM(FIRST_DISTRIBUTED_ENUM, NEW_VALUEF);
   DISTRIBUTED_ENUM(FIRST_DISTRIBUTED_ENUM, NEW_VALUEG = NEW_VALUEA); 
   DISTRIBUTED_ENUM(FIRST_DISTRIBUTED_ENUM, NEW_VALUEH);
   DISTRIBUTED_ENUM(FIRST_DISTRIBUTED_ENUM, NEW_VALUEI); 


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
      IncrementatorType frameId(Incrementator<IncrementatorTypeEnum::FRAME_ID>::GetIncrement());

      std::cout << std::setfill('0');
#if DEBUG
      std::cout << "D";
#endif
      std::cout << "(" << std::setw(6) << frameId << ") ";

      return mpGameLoopFunction();
   }

}