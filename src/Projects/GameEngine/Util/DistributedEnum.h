#ifndef DISTRIBUTED_ENUM_H
#define DISTRIBUTED_ENUM_H

#define DISTRIBUTED_ENUM_DEFINITION_BEGIN(DistributedEnumName, ...)  \
   namespace DistributedEnumName {                                   \
      enum Enum 

#define DISTRIBUTED_ENUM_DEFINITION_END                              \
      ;                                                              \
   }

#define DISTRIBUTED_ENUM(DistributedEnumName, EnumName)

#endif DISTRIBUTED_ENUM_H