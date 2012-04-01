#ifndef TASK_GRAPH_H
#define TASK_GRAPH_H

#include <map>
#include "TaskNode.h"
#include "GameEngine/Util/Callback/Callback.h"

#include "GameEngine/Macros/DLL.h"

namespace GameEngine
{
   // Typedefs
   typedef Util::Callback<void, float> TASK_TYPE;  ///< Generic type of a task
   typedef TASK_TYPE::BaseCallback MODULE_TASK; ///< Type of task getting polled from a module
   typedef TaskNode<MODULE_TASK*> TaskFunctionNode; ///< Type of task node
   typedef std::vector<TaskFunctionNode>::iterator TaskNodeIterator; ///< Iterator of task node

   // DLL export forward declaration
   EXPORT_STL_VECTOR(TaskFunctionNode);
   EXPORT_STL_VECTOR(TaskFunctionNode*);
   template class DLL_DECL Util::Callback<void, float>;
   template class DLL_DECL Util::Callback<void, float>::BaseCallback;
   template class DLL_DECL Util::Callback<void, float>::GlobalCallback;

   /// Task container in which tasks are ordered in a tree for execution.
   /// Tasks can be added to a parent task, and these will not be accessible until their parent is removed
   ///   from the tree. The GetOne() function returns a task and once it is remove it will unlock all it child
   ///   tasks. Tasks that have no parent task or that all their parent tasks have been removed are called 
   ///   'available tasks'
   class DLL_DECL TaskTree
   {
   public:
      TaskTree();
      TaskTree(const TaskTree& arTaskTree);
      TaskTree& operator=(const TaskTree& arTaskTree);
      ~TaskTree();

      /// Add a task without parent in the tree
      /// \param apTask Task to add to the tree
      /// \returns The task node associated to this operation
      /// \note The return task node can be used to create tasks parenting
      TaskFunctionNode& Add(MODULE_TASK* apTask);
      /// Add a task with parent tasks in the tree
      /// \param apTask Task to add to the tree
      /// \param aParentTasks List of task node that needs to be executed before this task
      /// \return The task node associated to this operation
      /// \note The return task node can be used to create tasks parenting
      TaskFunctionNode& Add(MODULE_TASK* apTask, std::vector<TaskFunctionNode> aParentTasks);

      /// \returns An available task
      TaskFunctionNode& GetOne();
      /// Removes a task and updates all tasks that were waiting after it
      /// \param arTask Task node to remove
      void Remove(const TaskFunctionNode& arTask);
      /// \returns How many tasks that have no parent or that all their parent tasks were been removed
      unsigned int GetAvailableCount();
      /// \returns How many tasks available or not
      unsigned int GetCount() const;
   
   private:
      // Would be better if it was a dictionnary / map
      std::vector<TaskFunctionNode> mAllTasks;        ///< List of all the tasks

      std::vector<TaskFunctionNode*> mAvailableTasks; ///< List of all availabe tasks
      bool mTaskGraphIsDirty;                         ///< True if we need to update the available tasks

      /// Swap the values from this TaskTree and another
      /// \param arTaskTree The other TaskTree to swap with
      void Swap(TaskTree &arTaskTree);

      /// \param aId The task node id
      /// \return The task node associated with the id
      TaskNodeIterator GetNode(TaskFunctionNode::Id aId);

      void UpdateAvailableTasks();

   };
}
#endif TASK_GRAPH_H