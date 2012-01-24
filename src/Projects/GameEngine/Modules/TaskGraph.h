#ifndef TASK_GRAPH_H
#define TASK_GRAPH_H

#include <map>
#include "TaskNode.h"
#include "TaskGraph\TaskCallback.h"

#include "GameEngine/Macros/DLL.h"

class IEngineModule;


// Typedefs
typedef TaskCallback<void, float> TASK_TYPE;
typedef TASK_TYPE::Base MODULE_TASK;
typedef TaskNode<MODULE_TASK*> TaskFunctionNode;
typedef std::vector<TaskFunctionNode>::iterator TaskNodeIterator;
typedef std::vector<TaskFunctionNode*>::iterator AvailableTaskNodeIterator;

// DLL export forward declaration
EXPORT_STL_VECTOR(TaskFunctionNode);
EXPORT_STL_VECTOR(TaskFunctionNode*);
template class DLL_DECL TaskCallback<void, float>;
template class DLL_DECL TaskCallback<void, float>::Base;

class DLL_DECL TaskGraph
{
public:
   TaskGraph();
   TaskGraph(const TaskGraph& arTaskGraph);
   TaskGraph& operator=(const TaskGraph& arTaskGraph);
   ~TaskGraph();

   TaskFunctionNode& Add(MODULE_TASK* apTask);
   TaskFunctionNode& Add(MODULE_TASK* apTask, std::vector<TaskFunctionNode> aParentTasks);

   TaskFunctionNode& GetOne();
   void Remove(const TaskFunctionNode& arTask);
   unsigned int GetAvailableCount();
   unsigned int GetCount() const;
   

protected:
private:
   std::vector<TaskFunctionNode> mAllTasks; // Would be better if it was a dictionnary / map

   std::vector<TaskFunctionNode*> mAvailableTasks;
   bool mTaskGraphIsDirty;

   void Swap(TaskGraph &arTaskGraph);
   TaskNodeIterator GetNode(IncrementatorType aId);
   void UpdateAvailableTasks();

};

#endif TASK_GRAPH_H