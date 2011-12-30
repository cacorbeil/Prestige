#ifndef GAME_ENGINE_CORE_H
#define GAME_ENGINE_CORE_H

#include "GameEngine/Macros/DLL.h"

#include "GameEngine/Util/Incrementator.h"
#include "GameEngine/Util/DistributedEnum.h"

namespace GameEngine
{
   typedef bool(*GAME_LOOP_FUNC)(void); 
    
   namespace IncrementatorTypeEnum
   { 
      enum Enum
      {
         FRAME_ID = 0,

         //Use this enum to as your first value in your game IncrementatorType enum
         INCREMENTOR_GAME_FOLLOW_UP, 
      };
   }

   //template<class GAME_LOOP_FUNCTOR>
   class DLL_DECL GameEngineCore
   {
   public:
      GameEngineCore(GAME_LOOP_FUNC apGameLoopFunction);

      void GameEngineCore::StartEngine();

      bool GameEngineCore::IsTerminated();

   protected:
      bool GameEngineCore::EngineIteration();

   private:
      bool mIsTerminated;
      long long mFrameCount;
      GAME_LOOP_FUNC mpGameLoopFunction;  
   };

   //#include "GameEngineCore.cpp"
}

#endif //GAME_ENGINE_CORE_H
