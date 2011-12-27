#ifndef GAME_ENGINE_CORE_H
#define GAME_ENGINE_CORE_H

#include "GameEngine/Macros/DLL.h"

#include "GameEngine/Util/Incrementator.h"
#include "GameEngine/Util/DistributedEnum.h"

namespace GameEngine
{
   DISTRIBUTED_ENUM_DEFINITION_BEGIN(FIRST_DISTRIBUTED_ENUM, COUNT, NULLABLE)
   {
      NONE, // Enum definition: NULLABLE
      NEW_VALUEA, // From file: 'Core\GameEngineCore.cpp'
      NEW_VALUEB, // From file: 'Core\GameEngineCore.cpp'
      NEW_VALUEC, // From file: 'Core\GameEngineCore.cpp'
      NEW_VALUED, // From file: 'Core\GameEngineCore.cpp'
      NEW_VALUEE, // From file: 'Core\GameEngineCore.cpp'
      NEW_VALUEK, // From file: 'Core\GameEngineCore.cpp'
      NEW_VALUEJ, // From file: 'Core\GameEngineCore.cpp'
      NEW_VALUEF, // From file: 'Core\GameEngineCore.cpp'
      NEW_VALUEH, // From file: 'Core\GameEngineCore.cpp'
      NEW_VALUEI, // From file: 'Core\GameEngineCore.cpp'
      NEW_VALUEG = NEW_VALUEA, // From file: 'Core\GameEngineCore.cpp'
      COUNT = 12, // Enum definition: COUNT
   }
   DISTRIBUTED_ENUM_DEFINITION_END

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
