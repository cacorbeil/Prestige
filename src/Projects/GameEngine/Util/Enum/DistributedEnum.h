#ifndef DISTRIBUTED_ENUM_H
#define DISTRIBUTED_ENUM_H

/// Central point of a distributed enum.
/// When this macro is used, the enum will be injected at this place in the code
#define DISTRIBUTED_ENUM_DEFINITION_BEGIN(DistributedEnumName, ...)  \
   namespace DistributedEnumName {                                   \
      enum Enum 

/// End point of the distributed enum.
#define DISTRIBUTED_ENUM_DEFINITION_END                              \
      ;                                                              \
   }

/// Distributed enum value
/// \param DistributedEnumName The name of the distributed enum (must be defined in another file)
/// \param EnumName Name of the enum value
#define DISTRIBUTED_ENUM(DistributedEnumName, EnumName)

#endif DISTRIBUTED_ENUM_H