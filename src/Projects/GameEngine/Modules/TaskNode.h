#ifndef TASK_NODE_H
#define TASK_NODE_H

#include <vector>
#include "GameEngine/Util/Incrementator.h"

#include "GameEngine/Macros/DLL.h"

DISTRIBUTED_ENUM(INCREMENTATOR_ENUM, TASK_ID);

EXPORT_STL_VECTOR(const IncrementatorType);

template<typename Type>
class /*DLL_DECL*/ TaskNode
{
public:
   TaskNode();
   TaskNode(const Type& aType);
   TaskNode(const Type& aType, std::vector<IncrementatorType> aChildrenList);
   TaskNode(const TaskNode& arTaskNode);
   ~TaskNode();

   inline unsigned int GetParentCount() const;
   inline void IncrementParent(IncrementatorType aParentId);
   inline void RemoveParent(IncrementatorType aParentId);

   inline IncrementatorType GetId() const;
   inline Type& GetValue();

protected:
private:
   IncrementatorType mId;
   unsigned int mParentCount;
   Type mValue;
   std::vector<const IncrementatorType> mParentIdList;
};

#include "TaskNode.cpp"

#endif TASK_NODE_H