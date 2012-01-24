#include "TaskGraph.h"

TaskGraph::TaskGraph()
   : mAvailableTasks()
   , mAllTasks()
   , mTaskGraphIsDirty(true)
{
}

TaskGraph::TaskGraph(const TaskGraph& arTaskGraph)
   : mAvailableTasks(arTaskGraph.mAvailableTasks)
   , mAllTasks(arTaskGraph.mAllTasks)
   , mTaskGraphIsDirty(arTaskGraph.mTaskGraphIsDirty)
{

}

TaskGraph& TaskGraph::operator=(const TaskGraph& arTaskGraph)
{
  TaskGraph(arTaskGraph).Swap(*this);
  return *this;
} 

TaskGraph::~TaskGraph()
{
}

void TaskGraph::Swap(TaskGraph &arTaskGraph)
{
   // Todo, real swap, this only copy value from one side
   mAllTasks = std::vector<TaskFunctionNode>(arTaskGraph.mAllTasks);
   mAvailableTasks = std::vector<TaskFunctionNode*>(arTaskGraph.mAvailableTasks);
   mTaskGraphIsDirty = arTaskGraph.mTaskGraphIsDirty;
};

TaskFunctionNode& TaskGraph::Add(MODULE_TASK* apTask)
{
   mAllTasks.push_back(TaskFunctionNode(apTask));
   mTaskGraphIsDirty = true;

   return mAllTasks[mAllTasks.size() - 1];
}

TaskFunctionNode& TaskGraph::Add(MODULE_TASK* apTask, std::vector<TaskFunctionNode> aParentTasks)
{
   TaskFunctionNode newTaskNode(apTask);
   for(TaskNodeIterator it = aParentTasks.begin(); it != aParentTasks.end(); ++it)
   {
      newTaskNode.IncrementParent(it->GetId());
   }
   mAllTasks.push_back(newTaskNode);
   mTaskGraphIsDirty = true;

   return mAllTasks[mAllTasks.size() - 1];
}

TaskFunctionNode& TaskGraph::GetOne()
{
   UpdateAvailableTasks();

   // Assert available task
   //assert(GetAvailableCount() > 0);

   TaskFunctionNode* node = mAvailableTasks.back();
   mAvailableTasks.pop_back();
   return *node;
}

void TaskGraph::Remove(const TaskFunctionNode& arTask)
{
   // Remove the task with this id
   for(TaskNodeIterator it = mAllTasks.begin(); it != mAllTasks.end(); ++it)
   {
      it->RemoveParent(arTask.GetId());
   }

   TaskNodeIterator toDeleteNodeIt(GetNode(arTask.GetId()));
   delete toDeleteNodeIt->GetValue();
   mAllTasks.erase(toDeleteNodeIt);
   mTaskGraphIsDirty = true;
}

unsigned int TaskGraph::GetAvailableCount()
{
   UpdateAvailableTasks();
   return mAvailableTasks.size();
}

unsigned int TaskGraph::GetCount() const
{
   return mAllTasks.size();
}

TaskNodeIterator TaskGraph::GetNode(IncrementatorType aId)
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

void TaskGraph::UpdateAvailableTasks()
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