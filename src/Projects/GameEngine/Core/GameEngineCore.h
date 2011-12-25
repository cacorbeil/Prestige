#ifndef GAME_ENGINE_CORE_H
#define GAME_ENGINE_CORE_H

#include "GameEngine/Macros/DLL.h"

#include "GameEngine/Util/Incrementator.h"
#include "GameEngine/Util/StaticDistributedEnums.h"

namespace GameEngine
{
   STATIC_DISTRIBUTED_ENUM_DEFINITION(FIRST_DISTRIBUTED_ENUM, COUNT|VALUE_COUNT, NULLABLE|VALUE_NULL)
   {
      //COUNT: VALUE_COUNT
      //NULLABLE: VALUE_NULL
      VALUE_NULL, // Enum definition: NULLABLE
      VALUE_A, // From file: 'Core\GameEngineCore.h'
      VALUE_C = VALUE_A, // From file: 'Core\GameEngineCore.h'
      VALUE_COUNT, // Enum definition: COUNT
   };

   STATIC_DISTRIBUTED_ENUM_DEFINITION(SECOND_DISTRIBUTED_ENUM, COUNT, NULLABLE)
   {
      //COUNT: SECOND_DISTRIBUTED_ENUM_COUNT
      //NULLABLE: SECOND_DISTRIBUTED_ENUM_NONE
      SECOND_DISTRIBUTED_ENUM_NONE, // Enum definition: NULLABLE
      SECOND_DISTRIBUTED_ENUM_COUNT, // Enum definition: COUNT
   };
     

   STATIC_DISTRIBUTED_ENUM(FIRST_DISTRIBUTED_ENUM, VALUE_C = VALUE_A);
   STATIC_DISTRIBUTED_ENUM(FIRST_DISTRIBUTED_ENUM, VALUE_A);

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
