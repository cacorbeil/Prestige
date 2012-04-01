#ifndef TASK_NODE_H
#define TASK_NODE_H

#include <vector>
#include "GameEngine/Util/Identifier/Incrementator.h"

#include "GameEngine/Macros/DLL.h"

DISTRIBUTED_ENUM(INCREMENTATOR_ENUM, TASK_ID);

EXPORT_STL_VECTOR(const Util::IncrementatorType);

namespace GameEngine
{
   /// Created when a task is added to a TaskTree.
   /// Contains the task and the information on parent tasks
   /// \see TaskTree
   template<typename Type>
   class TaskNode
   {
   public:
      typedef Util::IncrementatorType Id;

      TaskNode();
      /// Constructor without parent task
      /// \param aTask The task
      TaskNode(const Type& aTask);
      /// Constructor with parent task
      /// \param aTask The task
      /// \param aParentIdList The list of parent task id
      TaskNode(const Type& aTask, std::vector<Id> aParentIdList);
      TaskNode(const TaskNode& arTaskNode);
      ~TaskNode();

      inline unsigned int GetParentCount() const;
      /// Add a parent task
      inline void AddParent(Id aParentId);
      /// Remove a parent task
      inline void RemoveParent(Id aParentId);

      /// Get the unique identifier of this task node
      inline Id GetId() const;
      inline Type& GetTask();

   private:
      Id mId;                                ///< The node identifier
      Type mTask;
      std::vector<const Id> mParentIdList;   ///< List of parent task ids
   };
}


#include "TaskNode.inl"

#endif TASK_NODE_H