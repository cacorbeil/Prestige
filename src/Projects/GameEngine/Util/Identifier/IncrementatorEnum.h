#ifndef INCREMENTATOR_ENUM_H
#define INCREMENTATOR_ENUM_H

#include "GameEngine/Util/Enum/DistributedEnum.h"

/// Type of incrementators
DISTRIBUTED_ENUM_DEFINITION_BEGIN(INCREMENTATOR_ENUM)
{
   FRAME_ID, // From file: 'Core\Engine.h'
   MODULE_ID, // From file: 'Modules\IEngineModule.h'
   TASK_ID, // From file: 'Modules\TaskNode.h'
}
DISTRIBUTED_ENUM_DEFINITION_END

#endif INCREMENTATOR_ENUM_H