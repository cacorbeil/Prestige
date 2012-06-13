#include "PrecompiledHeader.h"

#include "TaskTree.h"

namespace GameEngine
{
   TaskTree::TaskTree()
      : mAvailableTasks()
      , mAllTasks()
      , mTaskGraphIsDirty(true)
   {
   }

   TaskTree::TaskTree(const TaskTree& arTaskTree)
      : mAvailableTasks(arTaskTree.mAvailableTasks)
      , mAllTasks(arTaskTree.mAllTasks)
      , mTaskGraphIsDirty(arTaskTree.mTaskGraphIsDirty)
   {

   }

   TaskTree& TaskTree::operator=(const TaskTree& arTaskTree)
   {
     TaskTree(arTaskTree).Swap(*this);
     return *this;
   } 

   TaskTree::~TaskTree()
   {
   }

   void TaskTree::Swap(TaskTree &arTaskTree)
   {
      // Todo, real swap, this only copy value from one side
      mAllTasks = std::vector<TaskFunctionNode>(arTaskTree.mAllTasks);
      mAvailableTasks = std::vector<TaskFunctionNode*>(arTaskTree.mAvailableTasks);
      mTaskGraphIsDirty = arTaskTree.mTaskGraphIsDirty;
   };

   TaskFunctionNode& TaskTree::Add(MODULE_TASK* apTask)
   {
      mAllTasks.push_back(TaskFunctionNode(apTask));
      mTaskGraphIsDirty = true;

      return mAllTasks[mAllTasks.size() - 1];
   }

   TaskFunctionNode& TaskTree::Add(MODULE_TASK* apTask, std::vector<TaskFunctionNode> aParentTasks)
   {
      TaskFunctionNode newTaskNode(apTask);
      for(TaskNodeIterator it = aParentTasks.begin(); it != aParentTasks.end(); ++it)
      {
         newTaskNode.AddParent(it->GetId());
      }
      mAllTasks.push_back(newTaskNode);
      mTaskGraphIsDirty = true;

      return mAllTasks[mAllTasks.size() - 1];
   }

   TaskFunctionNode& TaskTree::GetOne()
   {
      UpdateAvailableTasks();

      Assert(GetAvailableCount() > 0, "No tasks were available");

      TaskFunctionNode* node = mAvailableTasks.back();
      mAvailableTasks.pop_back();
      return *node;
   }

   void TaskTree::Remove(const TaskFunctionNode& arTask)
   {
      // Remove the task with this id
      for(TaskNodeIterator it = mAllTasks.begin(); it != mAllTasks.end(); ++it)
      {
         it->RemoveParent(arTask.GetId());
      }

      TaskNodeIterator toDeleteNodeIt(GetNode(arTask.GetId()));
      delete toDeleteNodeIt->GetTask();
      mAllTasks.erase(toDeleteNodeIt);
      mTaskGraphIsDirty = true;
   }

   unsigned int TaskTree::GetAvailableCount()
   {
      UpdateAvailableTasks();
      return mAvailableTasks.size();
   }

   unsigned int TaskTree::GetCount() const
   {
      return mAllTasks.size();
   }

   TaskNodeIterator TaskTree::GetNode(TaskFunctionNode::Id aId)
   {
      for(TaskNodeIterator it = mAllTasks.begin(); it != mAllTasks.end(); ++it)
      {
         if(it->GetId() == aId)
         {
            return it;
         }
      }

      // assert doesn't exists
      return mAllTasks.end();
   }

   void TaskTree::UpdateAvailableTasks()
   {
      if(mTaskGraphIsDirty)
      {
         mAvailableTasks.clear();
         // TODO: Seek tasks that are avaialable and copy them to mAvailableTasks
         for(TaskNodeIterator it = mAllTasks.begin(); it != mAllTasks.end(); ++it)
         {
            if(it->GetParentCount() == 0)
            {
               mAvailableTasks.push_back(&*it);
            }
         }

         mTaskGraphIsDirty = false;
      }
   }
}